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

#include <core/lily/analysis/checked/expr.h>
#include <core/lily/analysis/checked/expr/access.h>

#include <stdio.h>
#include <stdlib.h>

static inline VARIANT_DESTRUCTOR(LilyCheckedExprAccess,
                                 hook,
                                 const LilyCheckedExprAccess *self);

static inline VARIANT_DESTRUCTOR(LilyCheckedExprAccess,
                                 tuple,
                                 const LilyCheckedExprAccess *self);

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprAccessKind,
               enum LilyCheckedExprAccessKind kind)
{
    switch (kind) {
        case LILY_CHECKED_EXPR_ACCESS_KIND_HOOK:
            return "LILY_CHECKED_EXPR_ACCESS_KIND_HOOK";
        case LILY_CHECKED_EXPR_ACCESS_KIND_TUPLE:
            return "LILY_CHECKED_EXPR_ACCESS_KIND_TUPLE";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

bool
eq__LilyCheckedExprAccessHook(const LilyCheckedExprAccessHook *self,
                              const LilyCheckedExprAccessHook *other)
{
    return eq__LilyCheckedExpr(self->index, other->index) &&
           eq__LilyCheckedExpr(self->left, other->left);
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprAccessHook,
               const LilyCheckedExprAccessHook *self)
{
    return format("LilyCheckedExprAccessHook{{ left = {Sr}, index = {Sr} }",
                  to_string__Debug__LilyCheckedExpr(self->left),
                  to_string__Debug__LilyCheckedExpr(self->index));
}
#endif

DESTRUCTOR(LilyCheckedExprAccessHook, const LilyCheckedExprAccessHook *self)
{
    FREE(LilyCheckedExpr, self->left);
    FREE(LilyCheckedExpr, self->index);
}

bool
eq__LilyCheckedExprAccessTuple(const LilyCheckedExprAccessTuple *self,
                               const LilyCheckedExprAccessTuple *other)
{
    return eq__LilyCheckedExpr(self->left, other->left) &&
           eq__LilyCheckedExpr(self->n, other->n);
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprAccessTuple,
               const LilyCheckedExprAccessTuple *self)
{
    return format("LilyCheckedExprAccessTuple{{ left = {Sr}, n = {Sr} }",
                  to_string__Debug__LilyCheckedExpr(self->left),
                  to_string__Debug__LilyCheckedExpr(self->n));
}
#endif

DESTRUCTOR(LilyCheckedExprAccessTuple, const LilyCheckedExprAccessTuple *self)
{
    FREE(LilyCheckedExpr, self->left);
    FREE(LilyCheckedExpr, self->n);
}

bool
eq__LilyCheckedExprAccess(const LilyCheckedExprAccess *self,
                          const LilyCheckedExprAccess *other)
{
    if (self->kind != other->kind) {
        return false;
    }

    switch (self->kind) {
        case LILY_CHECKED_EXPR_ACCESS_KIND_HOOK:
            return eq__LilyCheckedExprAccessHook(&self->hook, &other->hook);
        case LILY_CHECKED_EXPR_ACCESS_KIND_TUPLE:
            return eq__LilyCheckedExprAccessTuple(&self->tuple, &other->tuple);
        default:
            UNREACHABLE("unknown variant");
    }
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprAccess,
               const LilyCheckedExprAccess *self)
{
    switch (self->kind) {
        case LILY_CHECKED_EXPR_ACCESS_KIND_HOOK:
            return to_string__Debug__LilyCheckedExprAccessHook(&self->hook);
        case LILY_CHECKED_EXPR_ACCESS_KIND_TUPLE:
            return to_string__Debug__LilyCheckedExprAccessTuple(&self->tuple);
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_DESTRUCTOR(LilyCheckedExprAccess,
                   hook,
                   const LilyCheckedExprAccess *self)
{
    FREE(LilyCheckedExprAccessHook, &self->hook);
}

VARIANT_DESTRUCTOR(LilyCheckedExprAccess,
                   tuple,
                   const LilyCheckedExprAccess *self)
{
    FREE(LilyCheckedExprAccessTuple, &self->tuple);
}

DESTRUCTOR(LilyCheckedExprAccess, const LilyCheckedExprAccess *self)
{
    switch (self->kind) {
        case LILY_CHECKED_EXPR_ACCESS_KIND_HOOK:
            FREE_VARIANT(LilyCheckedExprAccess, hook, self);
            break;
        case LILY_CHECKED_EXPR_ACCESS_KIND_TUPLE:
            FREE_VARIANT(LilyCheckedExprAccess, tuple, self);
            break;
        default:
            UNREACHABLE("unknown variant");
    }
}
