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

#include <core/lily/compiler/ir/llvm/attr.h>
#include <core/lily/compiler/ir/llvm/generator/body/function.h>
#include <core/lily/compiler/ir/llvm/generator/data_type.h>
#include <core/lily/compiler/ir/llvm/generator/expr.h>
#include <core/lily/compiler/ir/llvm/generator/main.h>
#include <core/lily/compiler/ir/llvm/generator/stmt.h>
#include <core/lily/compiler/ir/llvm/primary.h>

#include <stdio.h>
#include <stdlib.h>

LLVMValueRef
generate_main_function__LilyIrLlvm(const LilyIrLlvm *self,
                                   const LilyCheckedDeclFun *fun,
                                   LilyLlvmScope *scope,
                                   const Location *location)
{
    // Generate return data type of the function
    LLVMTypeRef return_data_type =
      generate_data_type__LilyIrLlvm(self, fun->return_data_type, scope);

    // Generate function parameters
    LLVMTypeRef params[2] = {
        ptr__LilyIrLlvm(self, ptr__LilyIrLlvm(self, i8__LilyIrLlvm(self))),
        i32__LilyIrLlvm(self)
    };

    LLVMTypeRef fun_data_type =
      LLVMFunctionType(return_data_type, params, 2, false);
    LLVMValueRef fun_llvm =
      LLVMAddFunction(self->module, "main", fun_data_type);

    LLVMBasicBlockRef entry_block = LLVMAppendBasicBlock(fun_llvm, "entry");
    LLVMPositionBuilderAtEnd(self->builder, entry_block);

    LilyLlvmScope *fun_scope = NEW(LilyLlvmScope, scope);

    GENERATE_FUNCTION_BODY(fun->body, fun_llvm, NULL, NULL, fun_scope);

    FREE(LilyLlvmScope, fun_scope);

    return fun_llvm;
}
