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
#include <core/lily/ir/llvm/generator/expr/literal.h>
#include <core/lily/ir/llvm/primary.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

LLVMValueRef
generate_literal_expr__LilyIrLlvm(const LilyIrLlvm *self,
                                  const LilyCheckedExpr *expr,
                                  LLVMValueRef ptr,
                                  LilyLlvmScope *scope)
{
    switch (expr->literal.kind) {
        case LILY_CHECKED_EXPR_LITERAL_KIND_BOOL:
            return LLVMConstInt(
              i1__LilyIrLlvm(self), expr->literal.bool_, false);
        case LILY_CHECKED_EXPR_LITERAL_KIND_BYTE:
            return LLVMConstInt(
              i8__LilyIrLlvm(self), expr->literal.byte, false);
        case LILY_CHECKED_EXPR_LITERAL_KIND_BYTES: {
            Usize bytes_len = strlen((char *)expr->literal.bytes) + 1;
            LLVMTypeRef bytes_type =
              LLVMArrayType(i8__LilyIrLlvm(self), bytes_len);
            LLVMValueRef global_str =
              LLVMAddGlobal(self->module, bytes_type, "bytes");

            LLVMSetInitializer(
              global_str,
              LLVMConstString((char *)expr->literal.bytes, bytes_len, false));
            LLVMSetUnnamedAddr(global_str, true);

            return global_str;
        }
        case LILY_CHECKED_EXPR_LITERAL_KIND_CHAR:
            return LLVMConstInt(
              i8__LilyIrLlvm(self), expr->literal.char_, false);
        case LILY_CHECKED_EXPR_LITERAL_KIND_FLOAT32:
            return LLVMConstReal(float__LilyIrLlvm(self),
                                 expr->literal.float32);
        case LILY_CHECKED_EXPR_LITERAL_KIND_FLOAT64:
            return LLVMConstReal(double__LilyIrLlvm(self),
                                 expr->literal.float64);
        case LILY_CHECKED_EXPR_LITERAL_KIND_INT32:
            switch (expr->data_type->kind) {
                case LILY_CHECKED_DATA_TYPE_KIND_INT16:
                    return LLVMConstInt(
                      i16__LilyIrLlvm(self), expr->literal.int32, true);
                case LILY_CHECKED_DATA_TYPE_KIND_UINT16:
                    return LLVMConstInt(
                      i16__LilyIrLlvm(self), expr->literal.int32, false);
                case LILY_CHECKED_DATA_TYPE_KIND_INT32:
                    return LLVMConstInt(
                      i32__LilyIrLlvm(self), expr->literal.int32, true);
                case LILY_CHECKED_DATA_TYPE_KIND_UINT32:
                    return LLVMConstInt(
                      i32__LilyIrLlvm(self), expr->literal.int32, false);
                case LILY_CHECKED_DATA_TYPE_KIND_INT64:
                    return LLVMConstInt(
                      i32__LilyIrLlvm(self), expr->literal.int32, true);
                case LILY_CHECKED_DATA_TYPE_KIND_UINT64:
                    return LLVMConstInt(
                      i64__LilyIrLlvm(self), expr->literal.int32, false);
                case LILY_CHECKED_DATA_TYPE_KIND_INT8:
                    return LLVMConstInt(
                      i8__LilyIrLlvm(self), expr->literal.int32, true);
                case LILY_CHECKED_DATA_TYPE_KIND_UINT8:
                    return LLVMConstInt(
                      i8__LilyIrLlvm(self), expr->literal.int32, false);
                case LILY_CHECKED_DATA_TYPE_KIND_ISIZE:
                    return LLVMConstInt(
                      intptr__LilyIrLlvm(self), expr->literal.int32, true);
                case LILY_CHECKED_DATA_TYPE_KIND_USIZE:
                    return LLVMConstInt(
                      intptr__LilyIrLlvm(self), expr->literal.int32, false);
                default:
                    UNREACHABLE("the analysis have a bug!!");
            }
        case LILY_CHECKED_EXPR_LITERAL_KIND_INT64:
            switch (expr->data_type->kind) {
                case LILY_CHECKED_DATA_TYPE_KIND_INT16:
                    return LLVMConstInt(
                      i16__LilyIrLlvm(self), expr->literal.int64, true);
                case LILY_CHECKED_DATA_TYPE_KIND_UINT16:
                    return LLVMConstInt(
                      i16__LilyIrLlvm(self), expr->literal.int64, false);
                case LILY_CHECKED_DATA_TYPE_KIND_INT32:
                    return LLVMConstInt(
                      i32__LilyIrLlvm(self), expr->literal.int64, true);
                case LILY_CHECKED_DATA_TYPE_KIND_UINT32:
                    return LLVMConstInt(
                      i32__LilyIrLlvm(self), expr->literal.int64, false);
                case LILY_CHECKED_DATA_TYPE_KIND_INT64:
                    return LLVMConstInt(
                      i32__LilyIrLlvm(self), expr->literal.int64, true);
                case LILY_CHECKED_DATA_TYPE_KIND_UINT64:
                    return LLVMConstInt(
                      i64__LilyIrLlvm(self), expr->literal.int64, false);
                case LILY_CHECKED_DATA_TYPE_KIND_INT8:
                    return LLVMConstInt(
                      i8__LilyIrLlvm(self), expr->literal.int64, true);
                case LILY_CHECKED_DATA_TYPE_KIND_UINT8:
                    return LLVMConstInt(
                      i8__LilyIrLlvm(self), expr->literal.int64, false);
                case LILY_CHECKED_DATA_TYPE_KIND_ISIZE:
                    return LLVMConstInt(
                      intptr__LilyIrLlvm(self), expr->literal.int64, true);
                case LILY_CHECKED_DATA_TYPE_KIND_USIZE:
                    return LLVMConstInt(
                      intptr__LilyIrLlvm(self), expr->literal.int64, false);
                default:
                    UNREACHABLE("the analysis have a bug!!");
            }
        case LILY_CHECKED_EXPR_LITERAL_KIND_NIL:
        case LILY_CHECKED_EXPR_LITERAL_KIND_NONE:
            return LLVMConstNull(ptr__LilyIrLlvm(
              self,
              generate_data_type__LilyIrLlvm(self, expr->data_type, scope)));
        case LILY_CHECKED_EXPR_LITERAL_KIND_STR: {
            LLVMTypeRef str_type =
              LLVMArrayType(i8__LilyIrLlvm(self), expr->literal.str->len + 1);
            LLVMValueRef global_str =
              LLVMAddGlobal(self->module, str_type, "str");

            LLVMSetInitializer(global_str,
                               LLVMConstString(expr->literal.str->buffer,
                                               expr->literal.str->len,
                                               false));
            LLVMSetUnnamedAddr(global_str, true);

            // Construct { i8*, i64 }

            LLVMTypeRef str_struct_type = LLVMStructType(
              (LLVMTypeRef[]){ LLVMPointerType(i8__LilyIrLlvm(self), 0),
                               i64__LilyIrLlvm(self) },
              2,
              false);

            LLVMValueRef str_len = LLVMConstInt(
              i64__LilyIrLlvm(self), expr->literal.str->len, false);
            LLVMValueRef str_ptr =
              ptr ? ptr : LLVMBuildAlloca(self->builder, str_struct_type, "");

            LLVMValueRef buffer_ptr = LLVMBuildStructGEP2(
              self->builder, str_struct_type, str_ptr, 0, "");

            LLVMBuildStore(self->builder, global_str, buffer_ptr);

            LLVMValueRef len_ptr = LLVMBuildStructGEP2(
              self->builder, str_struct_type, str_ptr, 1, "");

            LLVMBuildStore(self->builder, str_len, len_ptr);

            return ptr ? NULL : str_ptr;
        }
        case LILY_CHECKED_EXPR_LITERAL_KIND_SUFFIX_FLOAT32:
            return LLVMConstReal(float__LilyIrLlvm(self),
                                 expr->literal.suffix_float32);
        case LILY_CHECKED_EXPR_LITERAL_KIND_SUFFIX_FLOAT64:
            return LLVMConstReal(double__LilyIrLlvm(self),
                                 expr->literal.suffix_float64);
        case LILY_CHECKED_EXPR_LITERAL_KIND_SUFFIX_INT16:
            return LLVMConstInt(
              i16__LilyIrLlvm(self), expr->literal.suffix_int16, true);
        case LILY_CHECKED_EXPR_LITERAL_KIND_SUFFIX_INT32:
            return LLVMConstInt(
              i32__LilyIrLlvm(self), expr->literal.suffix_int32, true);
        case LILY_CHECKED_EXPR_LITERAL_KIND_SUFFIX_INT64:
            return LLVMConstInt(
              i64__LilyIrLlvm(self), expr->literal.suffix_int64, true);
        case LILY_CHECKED_EXPR_LITERAL_KIND_SUFFIX_INT8:
            return LLVMConstInt(
              i8__LilyIrLlvm(self), expr->literal.suffix_int8, true);
        case LILY_CHECKED_EXPR_LITERAL_KIND_SUFFIX_ISIZE:
            return LLVMConstInt(
              intptr__LilyIrLlvm(self), expr->literal.suffix_isize, true);
        case LILY_CHECKED_EXPR_LITERAL_KIND_SUFFIX_UINT16:
            return LLVMConstInt(
              i16__LilyIrLlvm(self), expr->literal.suffix_uint16, false);
        case LILY_CHECKED_EXPR_LITERAL_KIND_SUFFIX_UINT32:
            return LLVMConstInt(
              i32__LilyIrLlvm(self), expr->literal.suffix_uint32, false);
        case LILY_CHECKED_EXPR_LITERAL_KIND_SUFFIX_UINT64:
            return LLVMConstInt(
              i64__LilyIrLlvm(self), expr->literal.suffix_uint64, false);
        case LILY_CHECKED_EXPR_LITERAL_KIND_SUFFIX_UINT8:
            return LLVMConstInt(
              i8__LilyIrLlvm(self), expr->literal.suffix_uint8, false);
        case LILY_CHECKED_EXPR_LITERAL_KIND_SUFFIX_USIZE:
            return LLVMConstInt(
              intptr__LilyIrLlvm(self), expr->literal.suffix_usize, false);
        case LILY_CHECKED_EXPR_LITERAL_KIND_UINT32:
            return LLVMConstInt(
              i32__LilyIrLlvm(self), expr->literal.uint32, false);
        case LILY_CHECKED_EXPR_LITERAL_KIND_UINT64:
            return LLVMConstInt(
              i64__LilyIrLlvm(self), expr->literal.uint64, false);
        case LILY_CHECKED_EXPR_LITERAL_KIND_UNDEF:
            return LLVMGetUndef(ptr__LilyIrLlvm(self, i8__LilyIrLlvm(self)));
        case LILY_CHECKED_EXPR_LITERAL_KIND_UNIT:
            UNREACHABLE("cannot return unit value");
        default:
            UNREACHABLE("unknown variant");
    }
}
