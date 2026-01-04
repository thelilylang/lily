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

#include <base/alloc.h>

#include <core/lily/parser/ast/body/fun.h>

#include <stdio.h>
#include <stdlib.h>

// Free LilyAstBodyFunItem type (LILY_AST_BODY_FUN_ITEM_KIND_EXPR).
static VARIANT_DESTRUCTOR(LilyAstBodyFunItem, expr, LilyAstBodyFunItem *self);

// Free LilyAstBodyFunItem type (LILY_AST_BODY_FUN_ITEM_KIND_STMT).
static VARIANT_DESTRUCTOR(LilyAstBodyFunItem, stmt, LilyAstBodyFunItem *self);

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyAstBodyFunItemKind,
               enum LilyAstBodyFunItemKind self)
{
    switch (self) {
        case LILY_AST_BODY_FUN_ITEM_KIND_EXPR:
            return "LILY_AST_BODY_FUN_ITEM_KIND_EXPR";
        case LILY_AST_BODY_FUN_ITEM_KIND_STMT:
            return "LILY_AST_BODY_FUN_ITEM_KIND_STMT";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_CONSTRUCTOR(LilyAstBodyFunItem *,
                    LilyAstBodyFunItem,
                    expr,
                    LilyAstExpr *expr)
{
    LilyAstBodyFunItem *self = lily_malloc(sizeof(LilyAstBodyFunItem));

    self->kind = LILY_AST_BODY_FUN_ITEM_KIND_EXPR;
    self->expr = expr;

    return self;
}

VARIANT_CONSTRUCTOR(LilyAstBodyFunItem *,
                    LilyAstBodyFunItem,
                    stmt,
                    LilyAstStmt stmt)
{
    LilyAstBodyFunItem *self = lily_malloc(sizeof(LilyAstBodyFunItem));

    self->kind = LILY_AST_BODY_FUN_ITEM_KIND_STMT;
    self->stmt = stmt;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyAstBodyFunItem, const LilyAstBodyFunItem *self)
{
    switch (self->kind) {
        case LILY_AST_BODY_FUN_ITEM_KIND_EXPR:
            return to_string__Debug__LilyAstExpr(self->expr);
        case LILY_AST_BODY_FUN_ITEM_KIND_STMT:
            return to_string__Debug__LilyAstStmt(&self->stmt);
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_DESTRUCTOR(LilyAstBodyFunItem, expr, LilyAstBodyFunItem *self)
{
    FREE(LilyAstExpr, self->expr);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyAstBodyFunItem, stmt, LilyAstBodyFunItem *self)
{
    FREE(LilyAstStmt, &self->stmt);
    lily_free(self);
}

DESTRUCTOR(LilyAstBodyFunItem, LilyAstBodyFunItem *self)
{
    switch (self->kind) {
        case LILY_AST_BODY_FUN_ITEM_KIND_EXPR:
            FREE_VARIANT(LilyAstBodyFunItem, expr, self);
            break;
        case LILY_AST_BODY_FUN_ITEM_KIND_STMT:
            FREE_VARIANT(LilyAstBodyFunItem, stmt, self);
            break;
        default:
            UNREACHABLE("unknown variant");
    }
}
