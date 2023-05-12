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
                          LLVMValueRef fun,
                          LLVMBasicBlockRef exit_block,
                          LLVMBasicBlockRef cond_block,
                          LilyLlvmScope *scope)
{
    switch (stmt->kind) {
        case LILY_CHECKED_STMT_KIND_ASM:
            TODO("generate asm stmt");
        case LILY_CHECKED_STMT_KIND_AWAIT:
            TODO("generate await stmt");
        case LILY_CHECKED_STMT_KIND_BLOCK:
            if (stmt->block.body->len > 0) {
                LLVMBasicBlockRef block = LLVMAppendBasicBlock(fun, "block");
                LilyLlvmScope *block_scope = NEW(LilyLlvmScope, scope);

                LLVMBuildBr(self->builder, block);
                LLVMPositionBuilderAtEnd(self->builder, block);

                GENERATE_FUNCTION_BODY(
                  stmt->block.body, fun, NULL, NULL, block_scope);

                LLVMBasicBlockRef block_exit =
                  LLVMAppendBasicBlock(fun, "exit_block");

                LLVMBuildBr(self->builder, block_exit);

                LLVMPositionBuilderAtEnd(self->builder, block_exit);

                FREE(LilyLlvmScope, block_scope);
            }

            return NULL;
        case LILY_CHECKED_STMT_KIND_BREAK: {
            LLVMBuildBr(self->builder, exit_block);

            return NULL;
        }
        case LILY_CHECKED_STMT_KIND_DROP:
            TODO("generate drop stmt");
        case LILY_CHECKED_STMT_KIND_FOR:
            TODO("generate for stmt");
        case LILY_CHECKED_STMT_KIND_IF: {
            LilyLlvmScope *if_scope = NEW(LilyLlvmScope, scope);

            LLVMBasicBlockRef if_block_exit =
              LLVMAppendBasicBlock(fun, "if_exit");
            LLVMBasicBlockRef if_block_cond =
              LLVMAppendBasicBlock(fun, "if_cond");
            LLVMBasicBlockRef if_block = LLVMAppendBasicBlock(fun, "if_block");

            LLVMBuildBr(self->builder, if_block_cond);

            LLVMPositionBuilderAtEnd(self->builder, if_block_cond);

            LLVMValueRef if_cond =
              generate_expr__LilyIrLlvm(self, stmt->if_.if_->cond, scope, fun);

            if (stmt->if_.elifs) {
                if (stmt->if_.elifs->len > 1) {
                    LLVMBasicBlockRef current_elif_block_cond =
                      LLVMAppendBasicBlock(fun, "elif_cond");
                    LLVMBasicBlockRef current_elif_block =
                      LLVMAppendBasicBlock(fun, "elif_block");
                    LLVMBasicBlockRef next_elif_block_cond =
                      LLVMAppendBasicBlock(fun, "elif_cond");
                    LLVMBasicBlockRef next_elif_block =
                      LLVMAppendBasicBlock(fun, "elif_block");

                    LLVMBuildCondBr(self->builder,
                                    if_cond,
                                    if_block,
                                    current_elif_block_cond);

                    LLVMPositionBuilderAtEnd(self->builder, if_block);

                    GENERATE_FUNCTION_BODY(
                      stmt->if_.if_->body, fun, NULL, NULL, if_scope);
                    LLVMBuildBr(self->builder, if_block_exit);

                    for (Usize i = 0; i < stmt->if_.elifs->len - 1; ++i) {
                        LilyLlvmScope *elif_scope = NEW(LilyLlvmScope, scope);

                        LilyCheckedStmtIfBranch *elif =
                          get__Vec(stmt->if_.elifs, i);

                        LLVMPositionBuilderAtEnd(self->builder,
                                                 current_elif_block_cond);

                        LLVMValueRef elif_cond =
                          generate_expr__LilyIrLlvm(self, elif->cond, scope, fun);

                        LLVMBuildCondBr(self->builder,
                                        elif_cond,
                                        current_elif_block,
                                        next_elif_block_cond);

                        LLVMPositionBuilderAtEnd(self->builder,
                                                 current_elif_block);

                        GENERATE_FUNCTION_BODY(
                          elif->body, fun, NULL, NULL, elif_scope);
                        LLVMBuildBr(self->builder, if_block_exit);

                        current_elif_block_cond = next_elif_block_cond;
                        current_elif_block = next_elif_block;

                        if (i < stmt->if_.elifs->len - 2) {
                            next_elif_block_cond =
                              LLVMAppendBasicBlock(fun, "elif_cond");
                            next_elif_block =
                              LLVMAppendBasicBlock(fun, "elif_block");
                        }

                        FREE(LilyLlvmScope, elif_scope);
                    }

                    LilyLlvmScope *last_elif_scope = NEW(LilyLlvmScope, scope);

                    LilyCheckedStmtIfBranch *last_elif =
                      last__Vec(stmt->if_.elifs);

                    LLVMPositionBuilderAtEnd(self->builder,
                                             current_elif_block_cond);

                    LLVMValueRef last_elif_cond =
                      generate_expr__LilyIrLlvm(self, last_elif->cond, scope, fun);

                    LLVMBasicBlockRef else_block = NULL;

                    if (stmt->if_.else_) {
                        else_block = LLVMAppendBasicBlock(fun, "else_block");

                        LLVMBuildCondBr(self->builder,
                                        last_elif_cond,
                                        current_elif_block,
                                        else_block);
                    } else {
                        LLVMBuildCondBr(self->builder,
                                        last_elif_cond,
                                        current_elif_block,
                                        if_block_exit);
                    }

                    LLVMPositionBuilderAtEnd(self->builder, current_elif_block);

                    GENERATE_FUNCTION_BODY(
                      last_elif->body, fun, NULL, NULL, last_elif_scope);
                    LLVMBuildBr(self->builder, if_block_exit);

                    FREE(LilyLlvmScope, last_elif_scope);

                    if (else_block) {
                        LilyLlvmScope *else_scope = NEW(LilyLlvmScope, scope);

                        LLVMPositionBuilderAtEnd(self->builder, else_block);

                        GENERATE_FUNCTION_BODY(
                          stmt->if_.else_->body, fun, NULL, NULL, else_scope);
                        LLVMBuildBr(self->builder, if_block_exit);

                        FREE(LilyLlvmScope, else_scope);
                    }
                } else if (stmt->if_.elifs->len == 1) {
                    LilyCheckedStmtIfBranch *elif =
                      get__Vec(stmt->if_.elifs, 0);
                    LLVMBasicBlockRef elif_block_cond = NULL;
                    LLVMBasicBlockRef else_block = NULL;

                    if (stmt->if_.else_) {
                        if (stmt->if_.else_->body->len > 0) {
                            else_block =
                              LLVMAppendBasicBlock(fun, "else_block");
                        }
                    }

                    if (elif->body->len > 0) {
                        elif_block_cond =
                          LLVMAppendBasicBlock(fun, "elif_cond");

                        LLVMBuildCondBr(
                          self->builder, if_cond, if_block, elif_block_cond);
                    } else if (else_block) {
                        if (stmt->if_.else_->body->len > 0) {
                            LLVMBuildCondBr(
                              self->builder, if_cond, if_block, else_block);
                        } else {
                            LLVMBuildCondBr(
                              self->builder, if_cond, if_block, if_block_exit);
                        }
                    } else {
                        LLVMBuildCondBr(
                          self->builder, if_cond, if_block, if_block_exit);
                    }

                    LLVMPositionBuilderAtEnd(self->builder, if_block);

                    GENERATE_FUNCTION_BODY(
                      stmt->if_.if_->body, fun, NULL, NULL, if_scope);
                    LLVMBuildBr(self->builder, if_block_exit);

                    if (elif_block_cond) {
                        LLVMBasicBlockRef elif_block = NULL;

                        LLVMPositionBuilderAtEnd(self->builder,
                                                 elif_block_cond);

                        LLVMValueRef elif_cond =
                          generate_expr__LilyIrLlvm(self, elif->cond, scope, fun);

                        if (elif->body->len > 0) {
                            LilyLlvmScope *elif_scope =
                              NEW(LilyLlvmScope, scope);

                            elif_block =
                              LLVMAppendBasicBlock(fun, "elif_block");

                            if (else_block) {
                                LLVMBuildCondBr(self->builder,
                                                elif_cond,
                                                elif_block,
                                                else_block);
                            } else {
                                LLVMBuildCondBr(self->builder,
                                                elif_cond,
                                                elif_block,
                                                if_block_exit);
                            }

                            LLVMPositionBuilderAtEnd(self->builder, elif_block);

                            GENERATE_FUNCTION_BODY(
                              elif->body, fun, NULL, NULL, elif_scope);
                            LLVMBuildBr(self->builder, if_block_exit);

                            FREE(LilyLlvmScope, elif_scope);
                        } else {
                            if (else_block) {
                                LLVMBuildCondBr(self->builder,
                                                elif_cond,
                                                elif_block,
                                                else_block);
                            } else {
                                LLVMBuildCondBr(self->builder,
                                                elif_cond,
                                                elif_block,
                                                if_block_exit);
                            }
                        }

                        if (else_block) {
                            LilyLlvmScope *else_scope =
                              NEW(LilyLlvmScope, scope);

                            LLVMPositionBuilderAtEnd(self->builder, else_block);

                            GENERATE_FUNCTION_BODY(stmt->if_.else_->body,
                                                   fun,
                                                   NULL,
                                                   NULL,
                                                   else_scope);
                            LLVMBuildBr(self->builder, if_block_exit);

                            FREE(LilyLlvmScope, else_scope);
                        }
                    } else if (else_block) {
                        LilyLlvmScope *else_scope = NEW(LilyLlvmScope, scope);

                        LLVMPositionBuilderAtEnd(self->builder, else_block);

                        GENERATE_FUNCTION_BODY(
                          stmt->if_.else_->body, fun, NULL, NULL, else_scope);
                        LLVMBuildBr(self->builder, if_block_exit);

                        FREE(LilyLlvmScope, else_scope);
                    }
                } else {
                    UNREACHABLE("this situation is impossible");
                }
            } else {
                LLVMBasicBlockRef else_block = NULL;

                if (stmt->if_.else_) {
                    if (stmt->if_.else_->body->len > 0) {
                        else_block = LLVMAppendBasicBlock(fun, "else_block");

                        LLVMBuildCondBr(
                          self->builder, if_cond, if_block, else_block);
                    } else {
                        LLVMBuildCondBr(
                          self->builder, if_cond, if_block, if_block_exit);
                    }
                } else {
                    LLVMBuildCondBr(
                      self->builder, if_cond, if_block, if_block_exit);
                }

                LLVMPositionBuilderAtEnd(self->builder, if_block);

                GENERATE_FUNCTION_BODY(
                  stmt->if_.if_->body, fun, NULL, NULL, if_scope);
                LLVMBuildBr(self->builder, if_block_exit);

                if (else_block) {
                    LilyLlvmScope *else_scope = NEW(LilyLlvmScope, scope);

                    LLVMPositionBuilderAtEnd(self->builder, else_block);

                    GENERATE_FUNCTION_BODY(
                      stmt->if_.else_->body, fun, NULL, NULL, else_scope);
                    LLVMBuildBr(self->builder, if_block_exit);

                    FREE(LilyLlvmScope, else_scope);
                }
            }

            LLVMPositionBuilderAtEnd(self->builder, if_block_exit);

            FREE(LilyLlvmScope, if_scope);

            return NULL;
        }
        case LILY_CHECKED_STMT_KIND_MATCH:
            TODO("generate match stmt");
        case LILY_CHECKED_STMT_KIND_NEXT: {
            LLVMBuildBr(self->builder, cond_block);

            return NULL;
        }
        case LILY_CHECKED_STMT_KIND_RAISE:
            TODO("generate raise stmt");
        case LILY_CHECKED_STMT_KIND_RETURN:
            return LLVMBuildRet(
              self->builder,
              generate_expr__LilyIrLlvm(self, stmt->return_.expr, scope, fun));
        case LILY_CHECKED_STMT_KIND_TRY:
            TODO("generate try stmt");
        case LILY_CHECKED_STMT_KIND_UNSAFE:
            if (stmt->unsafe.body->len > 0) {
                LilyLlvmScope *unsafe_scope = NEW(LilyLlvmScope, scope);
                LLVMBasicBlockRef unsafe_block =
                  LLVMAppendBasicBlock(fun, "ublock");

                LLVMBuildBr(self->builder, unsafe_block);
                LLVMPositionBuilderAtEnd(self->builder, unsafe_block);

                GENERATE_FUNCTION_BODY(
                  stmt->unsafe.body, fun, NULL, NULL, unsafe_scope);

                LLVMBasicBlockRef unsafe_block_exit =
                  LLVMAppendBasicBlock(fun, "exit_ublock");

                LLVMBuildBr(self->builder, unsafe_block_exit);

                LLVMPositionBuilderAtEnd(self->builder, unsafe_block_exit);

                FREE(LilyLlvmScope, unsafe_scope);
            }

            return NULL;
        case LILY_CHECKED_STMT_KIND_VARIABLE: {
            LLVMTypeRef variable_data_type =
              generate_data_type__LilyIrLlvm(self, stmt->variable.data_type);
            LLVMValueRef variable_expr =
              generate_expr__LilyIrLlvm(self, stmt->variable.expr, scope, fun);
            LLVMValueRef variable = LLVMBuildAlloca(
              self->builder, variable_data_type, stmt->variable.name->buffer);

            push__Vec(scope->values,
                      NEW(LilyLlvmValue, stmt->variable.name, variable));

            LLVMBuildStore(self->builder, variable_expr, variable);

            return variable;
        }
        case LILY_CHECKED_STMT_KIND_WHILE: {
            LilyLlvmScope *while_scope = NEW(LilyLlvmScope, scope);

            LLVMBasicBlockRef loop_while_block_cond =
              LLVMAppendBasicBlock(fun, "loop_w_cond");

            LLVMBuildBr(self->builder, loop_while_block_cond);

            LLVMPositionBuilderAtEnd(self->builder, loop_while_block_cond);

            LLVMValueRef loop_while_cond =
              generate_expr__LilyIrLlvm(self, stmt->while_.expr, scope, fun);

            LLVMBasicBlockRef loop_while_block =
              LLVMAppendBasicBlock(fun, "loop_w");
            LLVMBasicBlockRef loop_while_exit =
              LLVMAppendBasicBlock(fun, "loop_w_exit");

            LLVMBuildCondBr(self->builder,
                            loop_while_cond,
                            loop_while_block,
                            loop_while_exit);

            LLVMPositionBuilderAtEnd(self->builder, loop_while_block);

            GENERATE_FUNCTION_BODY(stmt->while_.body,
                                   fun,
                                   loop_while_exit,
                                   loop_while_block_cond,
                                   while_scope);
            LLVMBuildBr(self->builder, loop_while_block_cond);

            LLVMPositionBuilderAtEnd(self->builder, loop_while_exit);

            FREE(LilyLlvmScope, while_scope);

            return NULL;
        }
        default:
            UNREACHABLE("unknown variant");
    }
}
