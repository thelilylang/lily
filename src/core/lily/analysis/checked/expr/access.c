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

DESTRUCTOR(LilyCheckedExprAccessHook, const LilyCheckedExprAccessHook *self)
{
    FREE(LilyCheckedExpr, self->left);
    FREE(LilyCheckedExpr, self->index);
}

DESTRUCTOR(LilyCheckedExprAccessTuple, const LilyCheckedExprAccessTuple *self)
{
    FREE(LilyCheckedExpr, self->left);
    FREE(LilyCheckedExpr, self->n);
}

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
