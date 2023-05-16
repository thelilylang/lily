/*
 * MIT License
 *
 * Copyright (c) 2022-2023 ArthurPV
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
#include <base/new.h>
#include <base/platform.h>

#include <core/lily/ir/llvm.h>

#include <llvm-c/DebugInfo.h>

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

String *
get_filename_from_path(const char *path)
{
    Usize path_len = strlen(path);
    String *filename = NEW(String);

    for (Usize i = path_len - 1; i > 0; --i) {
#ifdef LILY_WINDOWS_OS
        if (path[i] != '\\') {
            push__String(filename, path[i]);
        } else {
            break;
        }
#else
        if (path[i] != '/') {
            push__String(filename, path[i]);
        } else {
            break;
        }
#endif
    }

    reverse__String(filename);

    return filename;
}

CONSTRUCTOR(LilyIrLlvm, LilyIrLlvm, const char *module_name)
{
    LLVMInitializeAllTargets();
    LLVMInitializeAllTargetMCs();
    LLVMInitializeNativeTarget();
    LLVMInitializeNativeAsmPrinter();
    LLVMInitializeNativeAsmParser();
    LLVMInitializeCore(LLVMGetGlobalPassRegistry());

    LLVMModuleRef module = LLVMModuleCreateWithName(module_name);
    LLVMTargetRef target = LLVMGetFirstTarget();

    char *triple = get_triple();
    char *cpu = get_cpu();
    char *cpu_features = get_cpu_features();

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

    lily_free(triple);

    return (LilyIrLlvm){ .context = LLVMContextCreate(),
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
