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

#ifndef LILY_CORE_LILY_COMPILER_IR_LLVM_H
#define LILY_CORE_LILY_COMPILER_IR_LLVM_H

#include <base/macros.h>
#include <base/string.h>
#include <base/types.h>

#include <llvm-c/Core.h>
#include <llvm-c/Target.h>
#include <llvm-c/TargetMachine.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct LilyIrLlvm
    {
        LLVMContextRef context;
        LLVMModuleRef module;
        LLVMBuilderRef builder;
        LLVMDIBuilderRef di_builder;
        LLVMTargetRef target;
        LLVMTargetDataRef target_data;
        LLVMTargetMachineRef machine;
        LLVMMetadataRef file;
        LLVMMetadataRef compile_unit;
        char *cpu;
        char *features;
        Usize cpu_len;
        Usize features_len;
    } LilyIrLlvm;

    /**
     *
     * @brief Construct LilyIrLlvm type.
     */
    CONSTRUCTOR(LilyIrLlvm, LilyIrLlvm, const char *module_name);

    /**
     *
     * @brief Get the current scope of the instruction.
     */
    LLVMMetadataRef get_current_scope__LilyIrLlvm(LLVMValueRef inst);

    /**
     *
     * @brief Free LilyIrLlvm type.
     */
    DESTRUCTOR(LilyIrLlvm, const LilyIrLlvm *self);

    /**
     *
     * @brief Remove fatal error handler.
     */
    void LilyLLVMRemoveFatalErrorHandler();

#ifdef __cplusplus
}
#endif

#endif // LILY_CORE_LILY_COMPILER_IR_LLVM_H
