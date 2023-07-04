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

#include <core/lily/compiler/ir/llvm/generator/data_type.h>
#include <core/lily/compiler/ir/llvm/generator/expr.h>
#include <core/lily/compiler/ir/llvm/generator/stmt/variable.h>
#include <core/lily/compiler/ir/llvm/primary.h>
#include <core/lily/compiler/ir/llvm/store.h>

LLVMValueRef
generate_variable_stmt__LilyIrLlvm(const LilyIrLlvm *self,
                                   const LilyCheckedStmt *stmt,
                                   LLVMValueRef fun,
                                   LilyLlvmScope *scope)
{
    LLVMTypeRef variable_type =
      generate_data_type__LilyIrLlvm(self, stmt->variable.data_type, scope);
    LLVMValueRef variable = LLVMBuildAlloca(
      self->builder, variable_type, stmt->variable.name->buffer);
    LLVMValueRef variable_value = generate_expr__LilyIrLlvm(
      self, stmt->variable.expr, scope, fun, variable);

    push__Vec(scope->values, NEW(LilyLlvmValue, stmt->variable.name, variable));

    if (variable_value) {
        LLVMBuildStore(self->builder, variable_value, variable);
    }

    return variable;
}