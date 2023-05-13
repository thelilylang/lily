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
#include <string.h>

static LLVMValueRef
generate_literal_expr__LilyIrLlvm(const LilyIrLlvm *self,
                                  const LilyCheckedExprLiteral *literal,
                                  LilyLlvmScope *scope);

LLVMValueRef
generate_literal_expr__LilyIrLlvm(const LilyIrLlvm *self,
                                  const LilyCheckedExprLiteral *literal,
                                  LilyLlvmScope *scope)
{
    switch (literal->kind) {
        case LILY_CHECKED_EXPR_LITERAL_KIND_BOOL:
            return LLVMConstInt(i1__LilyIrLlvm(self), literal->bool_, false);
        case LILY_CHECKED_EXPR_LITERAL_KIND_BYTE:
            return LLVMConstInt(i8__LilyIrLlvm(self), literal->byte, false);
        case LILY_CHECKED_EXPR_LITERAL_KIND_BYTES:
            return LLVMConstString((const char *)literal->bytes,
                                   strlen((char *)literal->bytes),
                                   false);
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
        case LILY_CHECKED_EXPR_LITERAL_KIND_STR: {
            LLVMTypeRef str_type =
              LLVMArrayType(i8__LilyIrLlvm(self), literal->str->len + 1);
            LLVMValueRef global_str =
              LLVMAddGlobal(self->module, str_type, "str");

            LLVMSetInitializer(
              global_str,
              LLVMConstString(literal->str->buffer, literal->str->len, false));
            LLVMSetUnnamedAddr(global_str, true);

            return global_str;
        }
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
                          LilyLlvmScope *scope,
                          LLVMValueRef fun,
                          LLVMValueRef ptr)
{
    switch (expr->kind) {
        case LILY_CHECKED_EXPR_KIND_BINARY: {
            switch (expr->binary.kind) {
                case LILY_CHECKED_EXPR_BINARY_KIND_ADD:
                case LILY_CHECKED_EXPR_BINARY_KIND_BIT_AND:
                case LILY_CHECKED_EXPR_BINARY_KIND_BIT_L_SHIFT:
                case LILY_CHECKED_EXPR_BINARY_KIND_BIT_OR:
                case LILY_CHECKED_EXPR_BINARY_KIND_BIT_R_SHIFT:
                case LILY_CHECKED_EXPR_BINARY_KIND_DIV:
                case LILY_CHECKED_EXPR_BINARY_KIND_MOD:
                case LILY_CHECKED_EXPR_BINARY_KIND_MUL:
                case LILY_CHECKED_EXPR_BINARY_KIND_GREATER:
                case LILY_CHECKED_EXPR_BINARY_KIND_GREATER_EQ:
                case LILY_CHECKED_EXPR_BINARY_KIND_LESS:
                case LILY_CHECKED_EXPR_BINARY_KIND_LESS_EQ:
                case LILY_CHECKED_EXPR_BINARY_KIND_EXP:
                case LILY_CHECKED_EXPR_BINARY_KIND_SUB: {
                    LLVMValueRef left = generate_expr__LilyIrLlvm(
                      self, expr->binary.left, scope, fun, ptr);
                    LLVMValueRef right = generate_expr__LilyIrLlvm(
                      self, expr->binary.right, scope, fun, ptr);

                    if (is_integer_data_type__LilyCheckedDataType(
                          expr->binary.left->data_type) &&
                        is_integer_data_type__LilyCheckedDataType(
                          expr->binary.right->data_type) &&
                        eq__LilyCheckedDataType(
                          expr->binary.left->data_type,
                          expr->binary.right->data_type)) {
                        switch (expr->binary.kind) {
                            case LILY_CHECKED_EXPR_BINARY_KIND_ADD:
                                return LLVMBuildAdd(
                                  self->builder, left, right, "");
                            case LILY_CHECKED_EXPR_BINARY_KIND_BIT_AND:
                                return LLVMBuildAnd(
                                  self->builder, left, right, "");
                            case LILY_CHECKED_EXPR_BINARY_KIND_BIT_L_SHIFT:
                                return LLVMBuildShl(
                                  self->builder, left, right, "");
                            case LILY_CHECKED_EXPR_BINARY_KIND_BIT_OR:
                                return LLVMBuildOr(
                                  self->builder, left, right, "");
                            case LILY_CHECKED_EXPR_BINARY_KIND_BIT_R_SHIFT:
                                return LLVMBuildLShr(
                                  self->builder, left, right, "");
                            case LILY_CHECKED_EXPR_BINARY_KIND_DIV:
                                return LLVMBuildSDiv(
                                  self->builder, left, right, "");
                            case LILY_CHECKED_EXPR_BINARY_KIND_MOD:
                                return LLVMBuildSRem(
                                  self->builder, left, right, "");
                            case LILY_CHECKED_EXPR_BINARY_KIND_MUL:
                                return LLVMBuildMul(
                                  self->builder, left, right, "");
                            case LILY_CHECKED_EXPR_BINARY_KIND_GREATER:
                                return LLVMBuildICmp(
                                  self->builder, LLVMIntSGT, left, right, "");
                            case LILY_CHECKED_EXPR_BINARY_KIND_GREATER_EQ:
                                return LLVMBuildICmp(
                                  self->builder, LLVMIntSGE, left, right, "");
                            case LILY_CHECKED_EXPR_BINARY_KIND_LESS:
                                return LLVMBuildICmp(
                                  self->builder, LLVMIntSLT, left, right, "");
                            case LILY_CHECKED_EXPR_BINARY_KIND_LESS_EQ:
                                return LLVMBuildICmp(
                                  self->builder, LLVMIntSLE, left, right, "");
                            case LILY_CHECKED_EXPR_BINARY_KIND_EXP:
                                TODO("generate exp operator");
                            case LILY_CHECKED_EXPR_BINARY_KIND_SUB:
                                return LLVMBuildSub(
                                  self->builder, left, right, "");
                            default:
                                UNREACHABLE("unknown variant");
                        }
                    } else if (is_float_data_type__LilyCheckedDataType(
                                 expr->binary.left->data_type) &&
                               is_float_data_type__LilyCheckedDataType(
                                 expr->binary.right->data_type) &&
                               eq__LilyCheckedDataType(
                                 expr->binary.left->data_type,
                                 expr->binary.right->data_type)) {
                        switch (expr->binary.kind) {
                            case LILY_CHECKED_EXPR_BINARY_KIND_ADD:
                                return LLVMBuildFAdd(
                                  self->builder, left, right, "");
                            case LILY_CHECKED_EXPR_BINARY_KIND_DIV:
                                return LLVMBuildFDiv(
                                  self->builder, left, right, "");
                            case LILY_CHECKED_EXPR_BINARY_KIND_MOD:
                                return LLVMBuildFRem(
                                  self->builder, left, right, "");
                            case LILY_CHECKED_EXPR_BINARY_KIND_MUL:
                                return LLVMBuildFMul(
                                  self->builder, left, right, "");
                            case LILY_CHECKED_EXPR_BINARY_KIND_GREATER:
                                return LLVMBuildFCmp(
                                  self->builder, LLVMRealOGT, left, right, "");
                            case LILY_CHECKED_EXPR_BINARY_KIND_GREATER_EQ:
                                return LLVMBuildFCmp(
                                  self->builder, LLVMRealOGE, left, right, "");
                            case LILY_CHECKED_EXPR_BINARY_KIND_LESS:
                                return LLVMBuildFCmp(
                                  self->builder, LLVMRealOLT, left, right, "");
                            case LILY_CHECKED_EXPR_BINARY_KIND_LESS_EQ:
                                return LLVMBuildFCmp(
                                  self->builder, LLVMRealOLT, left, right, "");
                            case LILY_CHECKED_EXPR_BINARY_KIND_EXP:
                                TODO("generate exp operator");
                            case LILY_CHECKED_EXPR_BINARY_KIND_SUB:
                                return LLVMBuildFSub(
                                  self->builder, left, right, "");
                            default:
                                UNREACHABLE("the analysis have a bug!!");
                        }
                    } else {
                        TODO("generate operator defined by the user");
                    }
                }
                case LILY_CHECKED_EXPR_BINARY_KIND_AND:
                case LILY_CHECKED_EXPR_BINARY_KIND_OR:
                case LILY_CHECKED_EXPR_BINARY_KIND_XOR: {
                    LLVMValueRef left = generate_expr__LilyIrLlvm(
                      self, expr->binary.left, scope, fun, ptr);
                    LLVMValueRef right = generate_expr__LilyIrLlvm(
                      self, expr->binary.right, scope, fun, ptr);

                    switch (expr->binary.kind) {
                        case LILY_CHECKED_EXPR_BINARY_KIND_AND:
                            return LLVMBuildAnd(self->builder, left, right, "");
                        case LILY_CHECKED_EXPR_BINARY_KIND_OR:
                            return LLVMBuildOr(self->builder, left, right, "");
                        case LILY_CHECKED_EXPR_BINARY_KIND_XOR:
                            return LLVMBuildXor(self->builder, left, right, "");
                        default:
                            UNREACHABLE("unknown variant");
                    }

                    break;
                }
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
                      self, expr->binary.right, scope, fun, ptr);

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
                                        self,
                                        expr->binary.left->data_type,
                                        scope);
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
                case LILY_CHECKED_EXPR_BINARY_KIND_EQ: {
                    LLVMValueRef left = generate_expr__LilyIrLlvm(
                      self, expr->binary.left, scope, fun, ptr);
                    LLVMValueRef right = generate_expr__LilyIrLlvm(
                      self, expr->binary.right, scope, fun, ptr);

                    switch (expr->binary.left->data_type->kind) {
                        case LILY_CHECKED_DATA_TYPE_KIND_ANY:
                            TODO("implements equal for any");
                        case LILY_CHECKED_DATA_TYPE_KIND_ARRAY:
                            TODO("implements equal for array");
                        case LILY_CHECKED_DATA_TYPE_KIND_BOOL:
                        case LILY_CHECKED_DATA_TYPE_KIND_BYTE:
                        case LILY_CHECKED_DATA_TYPE_KIND_CHAR:
                        case LILY_CHECKED_DATA_TYPE_KIND_INT16:
                        case LILY_CHECKED_DATA_TYPE_KIND_INT32:
                        case LILY_CHECKED_DATA_TYPE_KIND_INT64:
                        case LILY_CHECKED_DATA_TYPE_KIND_INT8:
                        case LILY_CHECKED_DATA_TYPE_KIND_ISIZE:
                        case LILY_CHECKED_DATA_TYPE_KIND_UINT16:
                        case LILY_CHECKED_DATA_TYPE_KIND_UINT32:
                        case LILY_CHECKED_DATA_TYPE_KIND_UINT64:
                        case LILY_CHECKED_DATA_TYPE_KIND_UINT8:
                        case LILY_CHECKED_DATA_TYPE_KIND_USIZE:
                            return LLVMBuildICmp(
                              self->builder, LLVMIntEQ, left, right, "");
                        case LILY_CHECKED_DATA_TYPE_KIND_BYTES:
                            TODO("implements equal for bytes");
                        case LILY_CHECKED_DATA_TYPE_KIND_CUSTOM:
                            TODO("implements equal for custom");
                        case LILY_CHECKED_DATA_TYPE_KIND_EXCEPTION:
                            TODO("implements equal for exception");
                        case LILY_CHECKED_DATA_TYPE_KIND_FLOAT32:
                        case LILY_CHECKED_DATA_TYPE_KIND_FLOAT64:
                            return LLVMBuildFCmp(
                              self->builder, LLVMRealOEQ, left, right, "");
                        case LILY_CHECKED_DATA_TYPE_KIND_LAMBDA:
                            TODO("implements equal for lambda");
                        case LILY_CHECKED_DATA_TYPE_KIND_LIST:
                            TODO("implements equal for list");
                        case LILY_CHECKED_DATA_TYPE_KIND_MUT:
                            TODO("implements equal for mut");
                        case LILY_CHECKED_DATA_TYPE_KIND_NEVER:
                        case LILY_CHECKED_DATA_TYPE_KIND_UNIT:
                        case LILY_CHECKED_DATA_TYPE_KIND_UNKNOWN:
                            TODO("analysis have a bug!!");
                        case LILY_CHECKED_DATA_TYPE_KIND_OPTIONAL:
                            TODO("implements equal for optional");
                        case LILY_CHECKED_DATA_TYPE_KIND_PTR:
                            TODO("implements equal for ptr");
                        case LILY_CHECKED_DATA_TYPE_KIND_REF:
                            TODO("implements equal for ref");
                        case LILY_CHECKED_DATA_TYPE_KIND_STR:
                            TODO("implements equal for str");
                        case LILY_CHECKED_DATA_TYPE_KIND_TRACE:
                            TODO("implements equal for trace");
                        case LILY_CHECKED_DATA_TYPE_KIND_TUPLE:
                            TODO("implements equal for tuple");
                        default:
                            UNREACHABLE("unknown variant");
                    }
                }
                default:
                    TODO("generate binary expression");
            }

            break;
        }
        case LILY_CHECKED_EXPR_KIND_CALL: {
            LLVMTypeRef type =
              generate_data_type__LilyIrLlvm(self, expr->data_type, scope);

            switch (expr->call.kind) {
                case LILY_CHECKED_EXPR_CALL_KIND_VARIABLE:
                case LILY_CHECKED_EXPR_CALL_KIND_CONSTANT:
                    return load_value__LilyLlvmScope(
                      scope, self, type, expr->call.global_name);
                case LILY_CHECKED_EXPR_CALL_KIND_FUN_PARAM: {
                    return LLVMGetParam(fun, expr->call.fun_param);
                }
                case LILY_CHECKED_EXPR_CALL_KIND_FUN: {
                    LilyLlvmFun *called_fun =
                      search_fun__LilyLlvmScope(scope, expr->call.global_name);
                    LLVMValueRef params[252] = { 0 };
                    Usize params_len = 0;

                    if (expr->call.fun.params) {
                        params_len = expr->call.fun.params->len;

                        for (Usize i = 0; i < expr->call.fun.params->len; ++i) {
                            LilyCheckedExprCallFunParam *param =
                              get__Vec(expr->call.fun.params, i);

                            params[i] = generate_expr__LilyIrLlvm(
                              self, param->value, scope, fun, ptr);
                        }
                    }

                    return LLVMBuildCall2(self->builder,
                                          called_fun->fun_type,
                                          called_fun->fun,
                                          params,
                                          params_len,
                                          "");
                }
                case LILY_CHECKED_EXPR_CALL_KIND_FUN_SYS: {
                    LilyLlvmFun *called_fun = search_fun__LilyLlvmScope(
                      scope, expr->call.fun_sys.sys_fun_signature->real_name);
                    LLVMValueRef params[252] = {};
                    Usize params_len = 0;

                    if (expr->call.fun_sys.params) {
                        params_len = expr->call.fun_sys.params->len;

                        for (Usize i = 0; i < expr->call.fun_sys.params->len;
                             ++i) {
                            LilyCheckedExprCallFunParam *param =
                              get__Vec(expr->call.fun_sys.params, i);

                            params[i] = generate_expr__LilyIrLlvm(
                              self, param->value, scope, fun, ptr);
                        }
                    }

                    return LLVMBuildCall2(self->builder,
                                          called_fun->fun_type,
                                          called_fun->fun,
                                          params,
                                          params_len,
                                          "");
                }
                case LILY_CHECKED_EXPR_CALL_KIND_RECORD: {
                    LLVMTypeRef type = generate_data_type__LilyIrLlvm(
                      self, expr->data_type, scope);
                    LLVMValueRef record_value =
                      ptr ? ptr : LLVMBuildAlloca(self->builder, type, "");

                    if (expr->call.record.params->len > 0) {
                        for (Usize i = 0; i < expr->call.record.params->len;
                             ++i) {
                            LilyCheckedExprCallRecordParam *param =
                              get__Vec(expr->call.record.params, i);

                            LLVMValueRef ptr_param =
                              LLVMBuildStructGEP2(self->builder,
                                                  type,
                                                  record_value,
                                                  param->field_index,
                                                  "");
                            LLVMValueRef value = generate_expr__LilyIrLlvm(
                              self, param->value, scope, fun, NULL);

                            LLVMBuildStore(self->builder, value, ptr_param);
                        }
                    }

                    return record_value;
                }
                case LILY_CHECKED_EXPR_CALL_KIND_RECORD_FIELD_ACCESS: {
                    LLVMValueRef variable =
                      search_value__LilyLlvmScope(
                        scope,
                        CAST(
                          LilyCheckedExpr *,
                          get__Vec(expr->call.record_field_access.accesses, 0))
                          ->call.global_name)
                        ->value;
                    LLVMValueRef indices[252] = { 0 };

                    for (Usize i = 1;
                         i < expr->call.record_field_access.accesses->len;
                         ++i) {
                        LilyCheckedExpr *field =
                          get__Vec(expr->call.record_field_access.accesses, i);

                        indices[i - 1] =
                          LLVMConstInt(i32__LilyIrLlvm(self),
                                       field->call.record_field_single.index,
                                       false);
                    }

                    LLVMValueRef field_access = LLVMBuildGEP2(
                      self->builder,
                      generate_data_type__LilyIrLlvm(
                        self, expr->data_type, scope),
                      variable,
                      indices,
                      expr->call.record_field_access.accesses->len - 1,
                      "");

                    if (expr->data_type->kind ==
                        LILY_CHECKED_DATA_TYPE_KIND_CUSTOM) {
                        return LLVMBuildStore(self->builder, field_access, ptr);
                    }

                    return field_access;
                }
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
                          self, expr->cast.expr, scope, fun, ptr);
                        LLVMTypeRef dest_llvm = generate_data_type__LilyIrLlvm(
                          self, expr->cast.dest_data_type, scope);

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
            return generate_literal_expr__LilyIrLlvm(
              self, &expr->literal, scope);
        default:
            TODO("generate expression in LLVM IR");
    }
}
