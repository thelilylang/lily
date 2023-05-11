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
                case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_ADD:
                case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_BIT_AND:
                case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_BIT_L_SHIFT:
                case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_BIT_OR:
                case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_BIT_R_SHIFT:
                case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_DIV:
                case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_EXP:
                case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_MOD:
                case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_MUL:
                case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_SUB:
                case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_XOR:
                case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN: {
                    LLVMValueRef assigned = generate_expr__LilyIrLlvm(
                      self, expr->binary.right, scope);

                    switch (expr->binary.left->kind) {
                        case LILY_CHECKED_EXPR_KIND_CALL:
                            switch (expr->binary.left->call.kind) {
                                case LILY_CHECKED_EXPR_CALL_KIND_VARIABLE: {
                                    LLVMValueRef variable_ptr =
                                      search_value__LilyLlvmScope(
                                        scope,
                                        expr->binary.left->call.global_name)
                                        ->value;
                                    LLVMTypeRef variable_type =
                                      generate_data_type__LilyIrLlvm(
                                        self, expr->binary.left->data_type);
                                    LLVMValueRef variable_load =
                                      load_value__LilyLlvmScope(
                                        scope,
                                        self,
                                        variable_type,
                                        expr->binary.left->call.global_name);

                                    switch (expr->binary.kind) {
                                        case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_ADD: {
                                            LLVMValueRef add =
                                              LLVMBuildAdd(self->builder,
                                                           variable_load,
                                                           assigned,
                                                           "");

                                            return LLVMBuildStore(
                                              self->builder, add, variable_ptr);
                                        }
                                        case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_BIT_AND: {
                                            LLVMValueRef bit_and =
                                              LLVMBuildAnd(self->builder,
                                                           variable_load,
                                                           assigned,
                                                           "");

                                            return LLVMBuildStore(self->builder,
                                                                  bit_and,
                                                                  variable_ptr);
                                        }
                                        case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_BIT_L_SHIFT: {
                                            LLVMValueRef bit_l_shift =
                                              LLVMBuildShl(self->builder,
                                                           variable_load,
                                                           assigned,
                                                           "");

                                            return LLVMBuildStore(self->builder,
                                                                  bit_l_shift,
                                                                  variable_ptr);
                                        }
                                        case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_BIT_OR: {
                                            LLVMValueRef bit_or =
                                              LLVMBuildOr(self->builder,
                                                          variable_load,
                                                          assigned,
                                                          "");

                                            return LLVMBuildStore(self->builder,
                                                                  bit_or,
                                                                  variable_ptr);
                                        }
                                        case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_BIT_R_SHIFT: {
                                            LLVMValueRef bit_r_shift =
                                              LLVMBuildLShr(self->builder,
                                                            variable_load,
                                                            assigned,
                                                            "");

                                            return LLVMBuildStore(self->builder,
                                                                  bit_r_shift,
                                                                  variable_ptr);
                                        }
                                        case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_DIV: {
                                            LLVMValueRef div = NULL;

                                            switch (expr->binary.left->data_type
                                                      ->kind) {
                                                case LILY_CHECKED_DATA_TYPE_KIND_INT16:
                                                case LILY_CHECKED_DATA_TYPE_KIND_INT32:
                                                case LILY_CHECKED_DATA_TYPE_KIND_INT64:
                                                case LILY_CHECKED_DATA_TYPE_KIND_INT8:
                                                    div = LLVMBuildSDiv(
                                                      self->builder,
                                                      variable_load,
                                                      assigned,
                                                      "");

                                                    break;
                                                case LILY_CHECKED_DATA_TYPE_KIND_UINT16:
                                                case LILY_CHECKED_DATA_TYPE_KIND_UINT32:
                                                case LILY_CHECKED_DATA_TYPE_KIND_UINT64:
                                                case LILY_CHECKED_DATA_TYPE_KIND_UINT8:
                                                    div = LLVMBuildUDiv(
                                                      self->builder,
                                                      variable_load,
                                                      assigned,
                                                      "");

                                                    break;
                                                case LILY_CHECKED_DATA_TYPE_KIND_FLOAT32:
                                                case LILY_CHECKED_DATA_TYPE_KIND_FLOAT64:
                                                    div = LLVMBuildFDiv(
                                                      self->builder,
                                                      variable_load,
                                                      assigned,
                                                      "");

                                                    break;
                                                default:
                                                    UNREACHABLE("the analysis "
                                                                "have a bug!!");
                                            }

                                            return LLVMBuildStore(
                                              self->builder, div, variable_ptr);
                                        }
                                        case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_EXP:
                                            TODO("generate exponent operator");
                                        case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_MOD: {
                                            LLVMValueRef mod = NULL;

                                            switch (expr->binary.left->data_type
                                                      ->kind) {
                                                case LILY_CHECKED_DATA_TYPE_KIND_INT16:
                                                case LILY_CHECKED_DATA_TYPE_KIND_INT32:
                                                case LILY_CHECKED_DATA_TYPE_KIND_INT64:
                                                case LILY_CHECKED_DATA_TYPE_KIND_INT8:
                                                    mod = LLVMBuildSRem(
                                                      self->builder,
                                                      variable_load,
                                                      assigned,
                                                      "");

                                                    break;
                                                case LILY_CHECKED_DATA_TYPE_KIND_UINT16:
                                                case LILY_CHECKED_DATA_TYPE_KIND_UINT32:
                                                case LILY_CHECKED_DATA_TYPE_KIND_UINT64:
                                                case LILY_CHECKED_DATA_TYPE_KIND_UINT8:
                                                    mod = LLVMBuildURem(
                                                      self->builder,
                                                      variable_load,
                                                      assigned,
                                                      "");

                                                    break;
                                                case LILY_CHECKED_DATA_TYPE_KIND_FLOAT32:
                                                case LILY_CHECKED_DATA_TYPE_KIND_FLOAT64:
                                                    mod = LLVMBuildFRem(
                                                      self->builder,
                                                      variable_load,
                                                      assigned,
                                                      "");

                                                    break;
                                                default:
                                                    UNREACHABLE("the analysis "
                                                                "have a bug");
                                            }

                                            return LLVMBuildStore(
                                              self->builder, mod, variable_ptr);
                                        }
                                        case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_MUL: {
                                            LLVMValueRef mul = NULL;

                                            switch (expr->binary.left->data_type
                                                      ->kind) {
                                                case LILY_CHECKED_DATA_TYPE_KIND_INT16:
                                                case LILY_CHECKED_DATA_TYPE_KIND_INT32:
                                                case LILY_CHECKED_DATA_TYPE_KIND_INT64:
                                                case LILY_CHECKED_DATA_TYPE_KIND_INT8:
                                                case LILY_CHECKED_DATA_TYPE_KIND_UINT16:
                                                case LILY_CHECKED_DATA_TYPE_KIND_UINT32:
                                                case LILY_CHECKED_DATA_TYPE_KIND_UINT64:
                                                case LILY_CHECKED_DATA_TYPE_KIND_UINT8:
                                                    mul = LLVMBuildMul(
                                                      self->builder,
                                                      variable_load,
                                                      assigned,
                                                      "");

                                                    break;
                                                case LILY_CHECKED_DATA_TYPE_KIND_FLOAT32:
                                                case LILY_CHECKED_DATA_TYPE_KIND_FLOAT64:
                                                    mul = LLVMBuildFMul(
                                                      self->builder,
                                                      variable_load,
                                                      assigned,
                                                      "");

                                                    break;
                                                default:
                                                    UNREACHABLE("the analysis "
                                                                "have a bug");
                                            }

                                            return LLVMBuildStore(
                                              self->builder, mul, variable_ptr);
                                        }
                                        case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_SUB: {
                                            LLVMValueRef sub = NULL;

                                            switch (expr->binary.left->data_type
                                                      ->kind) {
                                                case LILY_CHECKED_DATA_TYPE_KIND_INT16:
                                                case LILY_CHECKED_DATA_TYPE_KIND_INT32:
                                                case LILY_CHECKED_DATA_TYPE_KIND_INT64:
                                                case LILY_CHECKED_DATA_TYPE_KIND_INT8:
                                                case LILY_CHECKED_DATA_TYPE_KIND_UINT16:
                                                case LILY_CHECKED_DATA_TYPE_KIND_UINT32:
                                                case LILY_CHECKED_DATA_TYPE_KIND_UINT64:
                                                case LILY_CHECKED_DATA_TYPE_KIND_UINT8:
                                                    sub = LLVMBuildSub(
                                                      self->builder,
                                                      variable_load,
                                                      assigned,
                                                      "");

                                                    break;
                                                case LILY_CHECKED_DATA_TYPE_KIND_FLOAT32:
                                                case LILY_CHECKED_DATA_TYPE_KIND_FLOAT64:
                                                    sub = LLVMBuildFSub(
                                                      self->builder,
                                                      variable_load,
                                                      assigned,
                                                      "");

                                                    break;
                                                default:
                                                    UNREACHABLE("the analysis "
                                                                "have a bug");
                                            }

                                            return LLVMBuildStore(
                                              self->builder, sub, variable_ptr);
                                        }
                                        case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_XOR: {
                                            LLVMValueRef xor
                                              = LLVMBuildXor(self->builder,
                                                             variable_load,
                                                             assigned,
                                                             "");

                                            return LLVMBuildStore(
                                              self->builder, xor, variable_ptr);
                                        }
                                        case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN:
                                            return LLVMBuildStore(self->builder,
                                                                  assigned,
                                                                  variable_ptr);
                                        default:
                                            UNREACHABLE("unknown variant");
                                    }
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
                    return load_value__LilyLlvmScope(
                      scope, self, type, expr->call.global_name);
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
