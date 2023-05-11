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

#include <core/lily/ir/llvm/generator/data_type.h>
#include <core/lily/ir/llvm/generator/expr.h>
#include <core/lily/ir/llvm/primary.h>

#include <stdio.h>
#include <stdlib.h>

static LLVMValueRef
generate_literal_expr__LilyIrLlvm(const LilyIrLlvm *self,
                                  const LilyCheckedExprLiteral *literal);

LLVMValueRef
generate_literal_expr__LilyIrLlvm(const LilyIrLlvm *self,
                                  const LilyCheckedExprLiteral *literal)
{
    switch (literal->kind) {
        case LILY_CHECKED_EXPR_LITERAL_KIND_BOOL:
            return LLVMConstInt(i1__LilyIrLlvm(self), literal->bool_, false);
        case LILY_CHECKED_EXPR_LITERAL_KIND_BYTE:
            return LLVMConstInt(i8__LilyIrLlvm(self), literal->byte, false);
        case LILY_CHECKED_EXPR_LITERAL_KIND_BYTES:
            TODO("generate bytes expression");
        case LILY_CHECKED_EXPR_LITERAL_KIND_CHAR:
            return LLVMConstInt(i8__LilyIrLlvm(self), literal->char_, false);
        case LILY_CHECKED_EXPR_LITERAL_KIND_FLOAT32:
            return LLVMConstReal(float__LilyIrLlvm(self), literal->float32);
        case LILY_CHECKED_EXPR_LITERAL_KIND_FLOAT64:
            return LLVMConstReal(double__LilyIrLlvm(self), literal->float64);
        case LILY_CHECKED_EXPR_LITERAL_KIND_INT32:
            return LLVMConstInt(i32__LilyIrLlvm(self), literal->int32, true);
        case LILY_CHECKED_EXPR_LITERAL_KIND_INT64:
            return LLVMConstInt(i64__LilyIrLlvm(self), literal->int64, true);
        case LILY_CHECKED_EXPR_LITERAL_KIND_NIL:
            return LLVMConstNull(ptr__LilyIrLlvm(self, i8__LilyIrLlvm(self)));
        case LILY_CHECKED_EXPR_LITERAL_KIND_NONE:
            TODO("generate none expression");
        case LILY_CHECKED_EXPR_LITERAL_KIND_STR:
            return LLVMConstString(
              literal->str->buffer, literal->str->len, true);
        case LILY_CHECKED_EXPR_LITERAL_KIND_SUFFIX_FLOAT32:
            return LLVMConstReal(float__LilyIrLlvm(self),
                                 literal->suffix_float32);
        case LILY_CHECKED_EXPR_LITERAL_KIND_SUFFIX_FLOAT64:
            return LLVMConstReal(double__LilyIrLlvm(self),
                                 literal->suffix_float64);
        case LILY_CHECKED_EXPR_LITERAL_KIND_SUFFIX_INT16:
            return LLVMConstInt(
              i16__LilyIrLlvm(self), literal->suffix_int16, true);
        case LILY_CHECKED_EXPR_LITERAL_KIND_SUFFIX_INT32:
            return LLVMConstInt(
              i32__LilyIrLlvm(self), literal->suffix_int32, true);
        case LILY_CHECKED_EXPR_LITERAL_KIND_SUFFIX_INT64:
            return LLVMConstInt(
              i64__LilyIrLlvm(self), literal->suffix_int64, true);
        case LILY_CHECKED_EXPR_LITERAL_KIND_SUFFIX_INT8:
            return LLVMConstInt(
              i8__LilyIrLlvm(self), literal->suffix_int8, true);
        case LILY_CHECKED_EXPR_LITERAL_KIND_SUFFIX_ISIZE:
            return LLVMConstInt(
              intptr__LilyIrLlvm(self), literal->suffix_isize, true);
        case LILY_CHECKED_EXPR_LITERAL_KIND_SUFFIX_UINT16:
            return LLVMConstInt(
              i16__LilyIrLlvm(self), literal->suffix_uint16, false);
        case LILY_CHECKED_EXPR_LITERAL_KIND_SUFFIX_UINT32:
            return LLVMConstInt(
              i32__LilyIrLlvm(self), literal->suffix_uint32, false);
        case LILY_CHECKED_EXPR_LITERAL_KIND_SUFFIX_UINT64:
            return LLVMConstInt(
              i64__LilyIrLlvm(self), literal->suffix_uint64, false);
        case LILY_CHECKED_EXPR_LITERAL_KIND_SUFFIX_UINT8:
            return LLVMConstInt(
              i8__LilyIrLlvm(self), literal->suffix_uint8, false);
        case LILY_CHECKED_EXPR_LITERAL_KIND_SUFFIX_USIZE:
            return LLVMConstInt(
              intptr__LilyIrLlvm(self), literal->suffix_usize, false);
        case LILY_CHECKED_EXPR_LITERAL_KIND_UINT32:
            return LLVMConstInt(i32__LilyIrLlvm(self), literal->uint32, false);
        case LILY_CHECKED_EXPR_LITERAL_KIND_UINT64:
            return LLVMConstInt(i64__LilyIrLlvm(self), literal->uint64, false);
        case LILY_CHECKED_EXPR_LITERAL_KIND_UNDEF:
            return LLVMGetUndef(ptr__LilyIrLlvm(self, i8__LilyIrLlvm(self)));
        case LILY_CHECKED_EXPR_LITERAL_KIND_UNIT:
            UNREACHABLE("cannot return unit value");
        default:
            UNREACHABLE("unknown variant");
    }
}

LLVMValueRef
generate_expr__LilyIrLlvm(const LilyIrLlvm *self,
                          const LilyCheckedExpr *expr,
                          LilyLlvmScope *scope)
{
    switch (expr->kind) {
        case LILY_CHECKED_EXPR_KIND_BINARY: {
            switch (expr->binary.kind) {
                case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN: {
                    LLVMValueRef assigned = generate_expr__LilyIrLlvm(
                      self, expr->binary.right, scope);

                    switch (expr->binary.left->kind) {
                        case LILY_CHECKED_EXPR_KIND_CALL:
                            switch (expr->binary.left->call.kind) {
                                case LILY_CHECKED_EXPR_CALL_KIND_VARIABLE: {
                                    LLVMValueRef variable =
                                      search_value__LilyLlvmScope(
                                        scope,
                                        expr->binary.left->call.global_name)
                                        ->value;

                                    return LLVMBuildStore(
                                      self->builder, assigned, variable);
                                }
                                default:
                                    UNREACHABLE("the analysis have a bug!!");
                            }

                            break;
                        default:
                            UNREACHABLE("the analysis have a bug!!");
                    }
                    break;
                }
                default:
                    TODO("generate binary expression");
            }

            break;
        }
        case LILY_CHECKED_EXPR_KIND_CALL: {
            LLVMTypeRef type =
              generate_data_type__LilyIrLlvm(self, expr->data_type);

            switch (expr->call.kind) {
                case LILY_CHECKED_EXPR_CALL_KIND_VARIABLE:
                case LILY_CHECKED_EXPR_CALL_KIND_CONSTANT:
                    return LLVMBuildLoad2(
                      self->builder,
                      type,
                      search_value__LilyLlvmScope(scope, expr->call.global_name)
                        ->value,
                      "");
                default:
                    TODO("generatte call expression in LLVM IR");
            }

            break;
        }
        case LILY_CHECKED_EXPR_KIND_CAST: {
            switch (expr->cast.kind) {
                case LILY_CHECKED_EXPR_CAST_KIND_DYNAMIC:
                    TODO("generate dynamic cast");
                case LILY_CHECKED_EXPR_CAST_KIND_LITERAL:
                    if (is_llvm_bitcast__LilyCheckedExprCast(&expr->cast)) {
                        LLVMValueRef expr_llvm = generate_expr__LilyIrLlvm(
                          self, expr->cast.expr, scope);
                        LLVMTypeRef dest_llvm = generate_data_type__LilyIrLlvm(
                          self, expr->cast.dest_data_type);

                        if (is_llvm_trunc__LilyCheckedExprCast(&expr->cast)) {
                            return LLVMBuildTrunc(
                              self->builder, expr_llvm, dest_llvm, "");
                        } else if (is_llvm_sext__LilyCheckedExprCast(
                                     &expr->cast)) {
                            return LLVMBuildSExt(
                              self->builder, expr_llvm, dest_llvm, "");
                        } else {
                            return LLVMBuildBitCast(
                              self->builder, expr_llvm, dest_llvm, "");
                        }
                    } else {
                        TODO("do other literal casts");
                    }

                    break;
                default:
                    UNREACHABLE("unknown variant");
            }

            break;
        }
        case LILY_CHECKED_EXPR_KIND_LITERAL:
            return generate_literal_expr__LilyIrLlvm(self, &expr->literal);
        default:
            TODO("generate expression in LLVM IR");
    }
}
