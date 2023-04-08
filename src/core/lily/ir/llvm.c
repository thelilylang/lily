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

#include <core/lily/ir/llvm.h>

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

CONSTRUCTOR(LilyIrLlvm, LilyIrLlvm, const char *module_name)
{
    LLVMInitializeAllTargets();
    LLVMInitializeAllTargetMCs();
    LLVMInitializeNativeTarget();
    LLVMInitializeNativeAsmPrinter();
    LLVMInitializeNativeAsmParser();

    LLVMModuleRef module = LLVMModuleCreateWithName(module_name);
    LLVMTargetRef target = LLVMGetFirstTarget();

    char *triple = get_triple();
    char *cpu = get_cpu();
    char *cpu_features = get_cpu_features();

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
    lily_free(cpu);
    lily_free(cpu_features);

    return (LilyIrLlvm){ .context = LLVMContextCreate(),
                         .module = module,
                         .builder = LLVMCreateBuilder(),
                         .target = target,
                         .target_data = target_data,
                         .machine = machine };
}

DESTRUCTOR(LilyIrLlvm, const LilyIrLlvm *self)
{
    LLVMContextDispose(self->context);
    LLVMDisposeModule(self->module);
    LLVMDisposeBuilder(self->builder);
    LLVMDisposeTargetData(self->target_data);
    LLVMDisposeTargetMachine(self->machine);
}
