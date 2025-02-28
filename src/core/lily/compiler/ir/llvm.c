/*
 * MIT License
 *
 * Copyright (c) 2022-2025 ArthurPV
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <base/alloc.h>
#include <base/assert.h>
#include <base/new.h>
#include <base/platform.h>

#include <cli/emit.h>

#include <core/lily/compiler/ir/llvm.h>

#include <llvm-c/Core.h>
#include <llvm-c/DebugInfo.h>
#include <llvm-c/Support.h>
#include <llvm-c/TargetMachine.h>

#include <stdio.h>
#include <string.h>

// Get default target triple.
static inline char *
get_triple();

// Get CPU of the host.
static inline char *
get_cpu();

// Get CPU features of the host.
static inline char *
get_cpu_features();

static inline void
lily_fatal_error(const char *reason);

/// Init all LLVM features.
static void
LilyLLVMInit();

char *
get_triple()
{
    return LLVMGetDefaultTargetTriple();
}

char *
get_cpu()
{
    return LLVMGetHostCPUName();
}

char *
get_cpu_features()
{
    return LLVMGetHostCPUFeatures();
}

void
lily_fatal_error(const char *reason)
{
    printf("Lily(Fatal): LLVM error: %s\n", reason);
}

CONSTRUCTOR(LilyIrLlvm, LilyIrLlvm, const char *module_name)
{
    // TODO: Check whether we need to initialize LLVM for each LLVM module
    LilyLLVMInit();

    char *triple = get_triple();
    char *cpu = get_cpu();
    char *cpu_features = get_cpu_features();

    LLVMContextRef context = LLVMContextCreate();
    LLVMModuleRef module =
      LLVMModuleCreateWithNameInContext(module_name, context);
    LLVMTargetRef target = NULL;
    char *target_error_msg = NULL;

    if (LLVMGetTargetFromTriple(triple, &target, &target_error_msg) != 0) {
        EMIT_ERROR(target_error_msg);
        LLVMDisposeMessage(target_error_msg);
        exit(1);
    }

    // LLVMTargetRef target = LLVMGetFirstTarget();

    LLVMSetTarget(module, triple);

    LLVMTargetMachineRef machine =
      LLVMCreateTargetMachine(target,
                              triple,
                              cpu,
                              cpu_features,
                              LLVMCodeGenLevelDefault,
                              LLVMRelocDefault,
                              LLVMCodeModelDefault);

    LLVMTargetDataRef target_data =
      LLVMCreateTargetData(LLVMGetDataLayoutStr(module));

    LLVMSetModuleDataLayout(module, target_data);

    lily_free(triple);

    return (LilyIrLlvm){ .context = context,
                         .module = module,
                         .builder = LLVMCreateBuilder(),
                         .di_builder = LLVMCreateDIBuilder(module),
                         .target = target,
                         .target_data = target_data,
                         .machine = machine,
                         .cpu = cpu,
                         .features = cpu_features,
                         .cpu_len = strlen(cpu),
                         .features_len = strlen(cpu_features) };
}

LLVMMetadataRef
get_current_scope__LilyIrLlvm(LLVMValueRef inst)
{
    LLVMMetadataRef debug_loc = LLVMInstructionGetDebugLoc(inst);

    if (!debug_loc)
        return NULL;

    return LLVMDILocationGetScope(debug_loc);
}

void
LilyLLVMInit()
{
    LLVMInitializeNativeTarget();
    LLVMInitializeAllTargets();
    LLVMInitializeAllTargetMCs();
    LLVMInitializeAllTargetInfos();
    LLVMInitializeAllAsmPrinters();
    LLVMInitializeAllAsmParsers();
    LLVMEnablePrettyStackTrace();
    // NOTE: This avoids the situation where an error handler is already
    // installed and causes the program to crash.
    LilyLLVMRemoveFatalErrorHandler();
    LLVMInstallFatalErrorHandler(lily_fatal_error);

    LLVMLoadLibraryPermanently(NULL);
}

DESTRUCTOR(LilyIrLlvm, const LilyIrLlvm *self)
{
    LLVMDisposeModule(self->module);
    LLVMDisposeBuilder(self->builder);
    LLVMDisposeDIBuilder(self->di_builder);
    LLVMDisposeTargetData(self->target_data);
    LLVMDisposeTargetMachine(self->machine);
    LLVMContextDispose(self->context);
    lily_free(self->cpu);
    lily_free(self->features);
}
