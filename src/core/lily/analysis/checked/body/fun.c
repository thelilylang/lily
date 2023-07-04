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

#include <core/lily/analysis/checked/body/fun.h>

#include <stdio.h>
#include <stdlib.h>

// Free LilyCheckedBodyFunItem type (LILY_CHECKED_BODY_FUN_ITEM_KIND_EXPR).
static VARIANT_DESTRUCTOR(LilyCheckedBodyFunItem,
                          expr,
                          LilyCheckedBodyFunItem *self);

// Free LilyCheckedBodyFunItem type (LILY_CHECKED_BODY_FUN_ITEM_KIND_STMT).
static VARIANT_DESTRUCTOR(LilyCheckedBodyFunItem,
                          stmt,
                          LilyCheckedBodyFunItem *self);

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedBodyFunItemKind,
               enum LilyCheckedBodyFunItemKind self)
{
    switch (self) {
        case LILY_CHECKED_BODY_FUN_ITEM_KIND_EXPR:
            return "LILY_CHECKED_BODY_FUN_ITEM_KIND_EXPR";
        case LILY_CHECKED_BODY_FUN_ITEM_KIND_STMT:
            return "LILY_CHECKED_BODY_FUN_ITEM_KIND_STMT";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_CONSTRUCTOR(LilyCheckedBodyFunItem *,
                    LilyCheckedBodyFunItem,
                    expr,
                    LilyCheckedExpr *expr)
{
    LilyCheckedBodyFunItem *self = lily_malloc(sizeof(LilyCheckedBodyFunItem));

    self->kind = LILY_CHECKED_BODY_FUN_ITEM_KIND_EXPR;
    self->expr = expr;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedBodyFunItem *,
                    LilyCheckedBodyFunItem,
                    stmt,
                    LilyCheckedStmt stmt)
{
    LilyCheckedBodyFunItem *self = lily_malloc(sizeof(LilyCheckedBodyFunItem));

    self->kind = LILY_CHECKED_BODY_FUN_ITEM_KIND_STMT;
    self->stmt = stmt;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedBodyFunItem,
               const LilyCheckedBodyFunItem *self)
{
    switch (self->kind) {
        case LILY_CHECKED_BODY_FUN_ITEM_KIND_EXPR:
            return to_string__Debug__LilyCheckedExpr(self->expr);
        case LILY_CHECKED_BODY_FUN_ITEM_KIND_STMT:
            return to_string__Debug__LilyCheckedStmt(&self->stmt);
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_DESTRUCTOR(LilyCheckedBodyFunItem, expr, LilyCheckedBodyFunItem *self)
{
    FREE(LilyCheckedExpr, self->expr);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyCheckedBodyFunItem, stmt, LilyCheckedBodyFunItem *self)
{
    FREE(LilyCheckedStmt, &self->stmt);
    lily_free(self);
}

DESTRUCTOR(LilyCheckedBodyFunItem, LilyCheckedBodyFunItem *self)
{
    switch (self->kind) {
        case LILY_CHECKED_BODY_FUN_ITEM_KIND_EXPR:
            FREE_VARIANT(LilyCheckedBodyFunItem, expr, self);
            break;
        case LILY_CHECKED_BODY_FUN_ITEM_KIND_STMT:
            FREE_VARIANT(LilyCheckedBodyFunItem, stmt, self);
            break;
        default:
            UNREACHABLE("unknown variant");
    }
}
