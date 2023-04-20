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

#include <base/alloc.h>

#include <core/lily/checked/expr.h>

#include <stdio.h>
#include <stdlib.h>

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, LilyCheckedExprKind, enum LilyCheckedExprKind self)
{
    switch (self) {
        case LILY_CHECKED_EXPR_KIND_ACCESS:
            return "LILY_CHECKED_EXPR_KIND_ACCESS";
        case LILY_CHECKED_EXPR_KIND_ARRAY:
            return "LILY_CHECKED_EXPR_KIND_ARRAY";
        case LILY_CHECKED_EXPR_KIND_BINARY:
            return "LILY_CHECKED_EXPR_KIND_BINARY";
        case LILY_CHECKED_EXPR_KIND_CALL:
            return "LILY_CHECKED_EXPR_KIND_CALL";
        case LILY_CHECKED_EXPR_KIND_CAST:
            return "LILY_CHECKED_EXPR_KIND_CAST";
        case LILY_CHECKED_EXPR_KIND_IDENTIFIER:
            return "LILY_CHECKED_EXPR_KIND_IDENTIFIER";
        case LILY_CHECKED_EXPR_KIND_LAMBDA:
            return "LILY_CHECKED_EXPR_KIND_LAMBDA";
        case LILY_CHECKED_EXPR_KIND_LIST:
            return "LILY_CHECKED_EXPR_KIND_LIST";
        case LILY_CHECKED_EXPR_KIND_LITERAL:
            return "LILY_CHECKED_EXPR_KIND_LITERAL";
        case LILY_CHECKED_EXPR_KIND_SELF:
            return "LILY_CHECKED_EXPR_KIND_SELF";
        case LILY_CHECKED_EXPR_KIND_TUPLE:
            return "LILY_CHECKED_EXPR_KIND_TUPLE";
        case LILY_CHECKED_EXPR_KIND_UNARY:
            return "LILY_CHECKED_EXPR_KIND_UNARY";
        case LILY_CHECKED_EXPR_KIND_GROUPING:
            return "LILY_CHECKED_EXPR_KIND_GROUPING";
        case LILY_CHECKED_EXPR_KIND_WILDCARD:
            return "LILY_CHECKED_EXPR_KIND_WILDCARD";
        default:
            UNREACHABLE("unknown variant");
    }
}

String *
IMPL_FOR_DEBUG(to_string, LilyCheckedExpr, const LilyCheckedExpr *self)
{
    TODO("to_string__Debug__LilyCheckedExpr");
}
#endif

String *
to_string__LilyCheckedExpr(const LilyCheckedExpr *self)
{
    TODO("to_string__LilyCheckedExpr");
}

DESTRUCTOR(LilyCheckedExpr, LilyCheckedExpr *self)
{
    lily_free(self);
}
