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

#include <base/format.h>
#include <base/new.h>
#include <base/string.h>

#include <core/lily/analysis/checked/expr.h>
#include <core/lily/analysis/checked/expr/binary.h>

#include <stdio.h>
#include <stdlib.h>

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprBinaryKind,
               enum LilyCheckedExprBinaryKind self)
{
    switch (self) {
        case LILY_CHECKED_EXPR_BINARY_KIND_ADD:
            return "LILY_CHECKED_EXPR_BINARY_KIND_ADD";
        case LILY_CHECKED_EXPR_BINARY_KIND_AND:
            return "LILY_CHECKED_EXPR_BINARY_KIND_AND";
        case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_ADD:
            return "LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_ADD";
        case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_BIT_AND:
            return "LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_BIT_AND";
        case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_BIT_L_SHIFT:
            return "LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_BIT_L_SHIFT";
        case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_BIT_OR:
            return "LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_BIT_OR";
        case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_BIT_R_SHIFT:
            return "LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_BIT_R_SHIFT";
        case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_DIV:
            return "LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_DIV";
        case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_EXP:
            return "LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_EXP";
        case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_MOD:
            return "LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_MOD";
        case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_MUL:
            return "LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_MUL";
        case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_SUB:
            return "LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_SUB";
        case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN:
            return "LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN";
        case LILY_CHECKED_EXPR_BINARY_KIND_BIT_AND:
            return "LILY_CHECKED_EXPR_BINARY_KIND_BIT_AND";
        case LILY_CHECKED_EXPR_BINARY_KIND_BIT_OR:
            return "LILY_CHECKED_EXPR_BINARY_KIND_BIT_OR";
        case LILY_CHECKED_EXPR_BINARY_KIND_CHAIN:
            return "LILY_CHECKED_EXPR_BINARY_KIND_CHAIN";
        case LILY_CHECKED_EXPR_BINARY_KIND_DIV:
            return "LILY_CHECKED_EXPR_BINARY_KIND_DIV";
        case LILY_CHECKED_EXPR_BINARY_KIND_EXP:
            return "LILY_CHECKED_EXPR_BINARY_KIND_EXP";
        case LILY_CHECKED_EXPR_BINARY_KIND_EQ:
            return "LILY_CHECKED_EXPR_BINARY_KIND_EQ";
        case LILY_CHECKED_EXPR_BINARY_KIND_GREATER_EQ:
            return "LILY_CHECKED_EXPR_BINARY_KIND_GREATER_EQ";
        case LILY_CHECKED_EXPR_BINARY_KIND_GREATER:
            return "LILY_CHECKED_EXPR_BINARY_KIND_GREATER";
        case LILY_CHECKED_EXPR_BINARY_KIND_BIT_L_SHIFT:
            return "LILY_CHECKED_EXPR_BINARY_KIND_L_SHIFT";
        case LILY_CHECKED_EXPR_BINARY_KIND_LESS_EQ:
            return "LILY_CHECKED_EXPR_BINARY_KIND_LESS_EQ";
        case LILY_CHECKED_EXPR_BINARY_KIND_LESS:
            return "LILY_CHECKED_EXPR_BINARY_KIND_LESS";
        case LILY_CHECKED_EXPR_BINARY_KIND_MOD:
            return "LILY_CHECKED_EXPR_BINARY_KIND_MOD";
        case LILY_CHECKED_EXPR_BINARY_KIND_MUL:
            return "LILY_CHECKED_EXPR_BINARY_KIND_MUL";
        case LILY_CHECKED_EXPR_BINARY_KIND_NOT_EQ:
            return "LILY_CHECKED_EXPR_BINARY_KIND_NOT_EQ";
        case LILY_CHECKED_EXPR_BINARY_KIND_OR:
            return "LILY_CHECKED_EXPR_BINARY_KIND_OR";
        case LILY_CHECKED_EXPR_BINARY_KIND_BIT_R_SHIFT:
            return "LILY_CHECKED_EXPR_BINARY_KIND_R_SHIFT";
        case LILY_CHECKED_EXPR_BINARY_KIND_RANGE:
            return "LILY_CHECKED_EXPR_BINARY_KIND_RANGE";
        case LILY_CHECKED_EXPR_BINARY_KIND_SUB:
            return "LILY_CHECKED_EXPR_BINARY_KIND_SUB";
        case LILY_CHECKED_EXPR_BINARY_KIND_XOR:
            return "LILY_CHECKED_EXPR_BINARY_KIND_XOR";
        default:
            UNREACHABLE("unknown variant");
    }
}

char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprBinary,
               const LilyCheckedExprBinary *self)
{
    return format(
      "LilyCheckedExprBinary{{ kind = {s}, left = {Sr}, right = {Sr} }",
      to_string__Debug__LilyCheckedExprBinaryKind(self->kind),
      to_string__Debug__LilyCheckedExpr(self->left),
      to_string__Debug__LilyCheckedExpr(self->right));
}
#endif

char *
to_string__LilyCheckedExprBinaryKind(enum LilyCheckedExprBinaryKind kind)
{
    switch (kind) {
        case LILY_CHECKED_EXPR_BINARY_KIND_ADD:
            return "+";
        case LILY_CHECKED_EXPR_BINARY_KIND_AND:
            return "and";
        case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_ADD:
            return "+=";
        case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_BIT_AND:
            return "&=";
        case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_BIT_L_SHIFT:
            return "<<=";
        case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_BIT_OR:
            return "|=";
        case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_BIT_R_SHIFT:
            return ">>=";
        case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_DIV:
            return "/=";
        case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_EXP:
            return "**=";
        case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_MOD:
            return "%=";
        case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_MUL:
            return "*=";
        case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_SUB:
            return "-=";
        case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_XOR:
            return "xor=";
        case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN:
            return "=";
        case LILY_CHECKED_EXPR_BINARY_KIND_BIT_AND:
            return "&";
        case LILY_CHECKED_EXPR_BINARY_KIND_BIT_OR:
            return "|";
        case LILY_CHECKED_EXPR_BINARY_KIND_CHAIN:
            return "|>";
        case LILY_CHECKED_EXPR_BINARY_KIND_DIV:
            return "/";
        case LILY_CHECKED_EXPR_BINARY_KIND_EXP:
            return "**";
        case LILY_CHECKED_EXPR_BINARY_KIND_EQ:
            return "==";
        case LILY_CHECKED_EXPR_BINARY_KIND_GREATER_EQ:
            return ">=";
        case LILY_CHECKED_EXPR_BINARY_KIND_GREATER:
            return ">";
        case LILY_CHECKED_EXPR_BINARY_KIND_BIT_L_SHIFT:
            return "|";
        case LILY_CHECKED_EXPR_BINARY_KIND_LESS_EQ:
            return "<=";
        case LILY_CHECKED_EXPR_BINARY_KIND_LESS:
            return "<";
        case LILY_CHECKED_EXPR_BINARY_KIND_MOD:
            return "%";
        case LILY_CHECKED_EXPR_BINARY_KIND_MUL:
            return "*";
        case LILY_CHECKED_EXPR_BINARY_KIND_NOT_EQ:
            return "not=";
        case LILY_CHECKED_EXPR_BINARY_KIND_OR:
            return "or";
        case LILY_CHECKED_EXPR_BINARY_KIND_BIT_R_SHIFT:
            return ">>";
        case LILY_CHECKED_EXPR_BINARY_KIND_RANGE:
            return "..";
        case LILY_CHECKED_EXPR_BINARY_KIND_SUB:
            return "-";
        case LILY_CHECKED_EXPR_BINARY_KIND_XOR:
            return "xor";
        case LILY_CHECKED_EXPR_BINARY_KIND_LIST_HEAD:
            return "->";
        case LILY_CHECKED_EXPR_BINARY_KIND_LIST_TAIL:
            return "<-";
        default:
            UNREACHABLE("unknown variant");
    }
}

bool
is_assign__LilyCheckedExprBinaryKind(enum LilyCheckedExprBinaryKind kind)
{
    switch (kind) {
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
        case LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN:
            return true;
        default:
            return false;
    }
}

bool
eq__LilyCheckedExprBinary(const LilyCheckedExprBinary *self,
                          const LilyCheckedExprBinary *other)
{
    return self->kind == other->kind &&
           eq__LilyCheckedExpr(self->left, other->left) &&
           eq__LilyCheckedExpr(self->right, other->right);
}

DESTRUCTOR(LilyCheckedExprBinary, const LilyCheckedExprBinary *self)
{
    FREE(LilyCheckedExpr, self->left);
    FREE(LilyCheckedExpr, self->right);
}
