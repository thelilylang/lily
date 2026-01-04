/*
 * MIT License
 *
 * Copyright (c) 2022-2026 ArthurPV
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

#include <base/new.h>

#include <core/lily/analysis/checked/expr.h>
#include <core/lily/analysis/checked/expr/unary.h>

#include <stdio.h>
#include <stdlib.h>

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprUnaryKind,
               enum LilyCheckedExprUnaryKind self)
{
    switch (self) {
        case LILY_CHECKED_EXPR_UNARY_KIND_DEREFERENCE:
            return "LILY_CHECKED_EXPR_UNARY_KIND_DEREFERENCE";
        case LILY_CHECKED_EXPR_UNARY_KIND_NEG:
            return "LILY_CHECKED_EXPR_UNARY_KIND_NEG";
        case LILY_CHECKED_EXPR_UNARY_KIND_NOT:
            return "LILY_CHECKED_EXPR_UNARY_KIND_NOT";
        case LILY_CHECKED_EXPR_UNARY_KIND_REF:
            return "LILY_CHECKED_EXPR_UNARY_KIND_REF";
        case LILY_CHECKED_EXPR_UNARY_KIND_REF_MUT:
            return "LILY_CHECKED_EXPR_UNARY_KIND_REF_MUT";
        case LILY_CHECKED_EXPR_UNARY_KIND_TRACE:
            return "LILY_CHECKED_EXPR_UNARY_KIND_TRACE";
        case LILY_CHECKED_EXPR_UNARY_KIND_TRACE_MUT:
            return "LILY_CHECKED_EXPR_UNARY_KIND_TRACE_MUT";
        default:
            UNREACHABLE("unknown variant");
    }
}

String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprUnary,
               const LilyCheckedExprUnary *self)
{
    return format__String(
      "LilyCheckedExprUnary{{ kind = {s}, right = {Sr} }",
      to_string__Debug__LilyCheckedExprUnaryKind(self->kind),
      to_string__Debug__LilyCheckedExpr(self->right));
}
#endif

char *
to_string__LilyCheckedExprUnaryKind(enum LilyCheckedExprUnaryKind kind)
{
    switch (kind) {
        case LILY_CHECKED_EXPR_UNARY_KIND_DEREFERENCE:
            return ".*";
        case LILY_CHECKED_EXPR_UNARY_KIND_NEG:
            return "-";
        case LILY_CHECKED_EXPR_UNARY_KIND_NOT:
            return "not";
        case LILY_CHECKED_EXPR_UNARY_KIND_REF:
            return "ref";
        case LILY_CHECKED_EXPR_UNARY_KIND_REF_MUT:
            return "ref mut";
        case LILY_CHECKED_EXPR_UNARY_KIND_TRACE:
            return "trace";
        case LILY_CHECKED_EXPR_UNARY_KIND_TRACE_MUT:
            return "trace mut";
        default:
            UNREACHABLE("unknown variant");
    }
}

bool
eq__LilyCheckedExprUnary(const LilyCheckedExprUnary *self,
                         const LilyCheckedExprUnary *other)
{
    return self->kind == other->kind &&
           eq__LilyCheckedExpr(self->right, other->right);
}

DESTRUCTOR(LilyCheckedExprUnary, const LilyCheckedExprUnary *self)
{
    FREE(LilyCheckedExpr, self->right);
}
