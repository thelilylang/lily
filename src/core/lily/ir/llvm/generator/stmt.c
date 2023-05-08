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

#include <core/lily/ir/llvm/generator/body/function.h>
#include <core/lily/ir/llvm/generator/data_type.h>
#include <core/lily/ir/llvm/generator/expr.h>
#include <core/lily/ir/llvm/generator/stmt.h>
#include <core/lily/ir/llvm/primary.h>

#include <stdio.h>
#include <stdlib.h>

LLVMValueRef
generate_stmt__LilyIrLlvm(const LilyIrLlvm *self,
                          const LilyCheckedStmt *stmt,
                          LLVMValueRef fun)
{
    switch (stmt->kind) {
        case LILY_CHECKED_STMT_KIND_ASM:
            TODO("generate asm stmt");
        case LILY_CHECKED_STMT_KIND_AWAIT:
            TODO("generate await stmt");
        case LILY_CHECKED_STMT_KIND_BLOCK:
            TODO("generate block stmt");
        case LILY_CHECKED_STMT_KIND_BREAK:
            TODO("generate break stmt");
        case LILY_CHECKED_STMT_KIND_DROP:
            TODO("generate drop stmt");
        case LILY_CHECKED_STMT_KIND_FOR:
            TODO("generate for stmt");
        case LILY_CHECKED_STMT_KIND_IF:
            TODO("generate if stmt");
        case LILY_CHECKED_STMT_KIND_MATCH:
            TODO("generate match stmt");
        case LILY_CHECKED_STMT_KIND_NEXT:
            TODO("generate next stmt");
        case LILY_CHECKED_STMT_KIND_RAISE:
            TODO("generate raise stmt");
        case LILY_CHECKED_STMT_KIND_RETURN:
            return LLVMBuildRet(
              self->builder,
              generate_expr__LilyIrLlvm(self, stmt->return_.expr));
        case LILY_CHECKED_STMT_KIND_TRY:
            TODO("generate try stmt");
        case LILY_CHECKED_STMT_KIND_UNSAFE:
            TODO("generate unsafe stmt");
        case LILY_CHECKED_STMT_KIND_VARIABLE: {
            LLVMTypeRef variable_data_type =
              generate_data_type__LilyIrLlvm(self, stmt->variable.data_type);
            LLVMValueRef variable_expr =
              generate_expr__LilyIrLlvm(self, stmt->variable.expr);
            LLVMValueRef variable = LLVMBuildAlloca(
              self->builder, variable_data_type, stmt->variable.name->buffer);

            LLVMBuildStore(self->builder, variable_expr, variable);

            return variable;
        }
        case LILY_CHECKED_STMT_KIND_WHILE: {
            LLVMValueRef loop_while_cond =
              generate_expr__LilyIrLlvm(self, stmt->while_.expr);

            LLVMBasicBlockRef loop_while_block =
              LLVMAppendBasicBlock(fun, "lwb");
            LLVMBuildBr(self->builder, loop_while_block);
            LLVMBasicBlockRef loop_while_exit =
              LLVMAppendBasicBlock(fun, "lwe");
            LLVMPositionBuilderAtEnd(self->builder, loop_while_block);
            LLVMBuildCondBr(self->builder,
                            loop_while_cond,
                            loop_while_block,
                            loop_while_exit);

            GENERATE_FUNCTION_BODY(stmt->while_.body, fun);

            LLVMPositionBuilderAtEnd(self->builder, loop_while_exit);

            return NULL;
        }
        default:
            UNREACHABLE("unknown variant");
    }
}
