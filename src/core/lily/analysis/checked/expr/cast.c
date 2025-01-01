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

#include <core/lily/analysis/checked/expr.h>
#include <core/lily/analysis/checked/expr/cast.h>

#include <stdio.h>
#include <stdlib.h>

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprCastKind,
               enum LilyCheckedExprCastKind self)
{
    switch (self) {
        case LILY_CHECKED_EXPR_CAST_KIND_DYNAMIC:
            return "LILY_CHECKED_EXPR_CAST_KIND_DYNAMIC";
        case LILY_CHECKED_EXPR_CAST_KIND_LITERAL:
            return "LILY_CHECKED_EXPR_CAST_KIND_LITERAL";
        case LILY_CHECKED_EXPR_CAST_KIND_STRING:
            return "LILY_CHECKED_EXPR_CAST_KIND_STRING";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyCheckedExprCast, const LilyCheckedExprCast *self)
{
    return format__String(
      "LilyCheckedExprCast{{ kind = {s}, expr = {Sr}, dest_data_type = {Sr} }",
      to_string__Debug__LilyCheckedExprCastKind(self->kind),
      to_string__Debug__LilyCheckedExpr(self->expr),
      to_string__Debug__LilyCheckedDataType(self->dest_data_type));
}
#endif

bool
is_llvm_bitcast__LilyCheckedExprCast(const LilyCheckedExprCast *self)
{
    switch (self->expr->data_type->kind) {
        case LILY_CHECKED_DATA_TYPE_KIND_BOOL:
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
            switch (self->dest_data_type->kind) {
                case LILY_CHECKED_DATA_TYPE_KIND_BOOL:
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
                    return true;
                default:
                    return false;
            }
        default:
            return false;
    }
}

bool
is_llvm_trunc__LilyCheckedExprCast(const LilyCheckedExprCast *self)
{
    return is_llvm_bitcast__LilyCheckedExprCast(self)
             ? get_integer_size__LilyCheckedDataType(self->expr->data_type) >
                 get_integer_size__LilyCheckedDataType(self->dest_data_type)
             : 0;
}

bool
is_llvm_sext__LilyCheckedExprCast(const LilyCheckedExprCast *self)
{
    return is_llvm_bitcast__LilyCheckedExprCast(self)
             ? get_integer_size__LilyCheckedDataType(self->expr->data_type) <
                 get_integer_size__LilyCheckedDataType(self->dest_data_type)
             : 0;
}

bool
eq__LilyCheckedExprCast(const LilyCheckedExprCast *self,
                        const LilyCheckedExprCast *other)
{
    return self->kind == other->kind &&
           eq__LilyCheckedExpr(self->expr, other->expr) &&
           eq__LilyCheckedDataType(self->dest_data_type, other->dest_data_type);
}

DESTRUCTOR(LilyCheckedExprCast, const LilyCheckedExprCast *self)
{
    FREE(LilyCheckedExpr, self->expr);
    FREE(LilyCheckedDataType, self->dest_data_type);
}
