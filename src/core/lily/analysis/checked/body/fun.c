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

#include <base/alloc.h>

#include <core/lily/analysis/checked/body/fun.h>
#include <core/lily/analysis/checked/parent.h>

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

LilyCheckedBodyFunInfo
wrap_item_in_body__LilyCheckedBodyFunItem(LilyCheckedBodyFunItem *self,
                                          LilyCheckedScope *parent)
{
    switch (self->kind) {
        case LILY_CHECKED_BODY_FUN_ITEM_KIND_STMT:
            switch (self->stmt.kind) {
                case LILY_CHECKED_STMT_KIND_ASM:
                case LILY_CHECKED_STMT_KIND_AWAIT:
                case LILY_CHECKED_STMT_KIND_BREAK:
                case LILY_CHECKED_STMT_KIND_DROP:
                case LILY_CHECKED_STMT_KIND_FOR:
                case LILY_CHECKED_STMT_KIND_IF:
                case LILY_CHECKED_STMT_KIND_MATCH:
                case LILY_CHECKED_STMT_KIND_NEXT:
                case LILY_CHECKED_STMT_KIND_RAISE:
                case LILY_CHECKED_STMT_KIND_RETURN:
                case LILY_CHECKED_STMT_KIND_TRY:
                case LILY_CHECKED_STMT_KIND_UNSAFE:
                case LILY_CHECKED_STMT_KIND_VARIABLE:
                case LILY_CHECKED_STMT_KIND_WHILE:
                    goto simple_wrap;
                case LILY_CHECKED_STMT_KIND_BLOCK: {
                    Vec *body = self->stmt.block.body;
                    LilyCheckedScope *scope = self->stmt.block.scope;

                    lily_free(self);

                    return NEW(LilyCheckedBodyFunInfo, body, scope);
                }
                default:
                    UNREACHABLE("unknown variant");
            }
        case LILY_CHECKED_BODY_FUN_ITEM_KIND_EXPR: {
        simple_wrap: {
            Vec *body = init__Vec(1, self);

            switch (parent->decls.kind) {
                case LILY_CHECKED_SCOPE_DECLS_KIND_DECL:
                    return NEW(
                      LilyCheckedBodyFunInfo,
                      body,
                      NEW(
                        LilyCheckedScope,
                        NEW_VARIANT(
                          LilyCheckedParent, decl, parent, parent->decls.decl),
                        NEW_VARIANT(LilyCheckedScopeDecls, scope, body)));
                case LILY_CHECKED_SCOPE_DECLS_KIND_SCOPE:
                    return NEW(
                      LilyCheckedBodyFunInfo,
                      body,
                      NEW(LilyCheckedScope,
                          NEW_VARIANT(LilyCheckedParent,
                                      scope,
                                      parent,
                                      parent->decls.scope),
                          NEW_VARIANT(LilyCheckedScopeDecls, scope, body)));
                case LILY_CHECKED_SCOPE_DECLS_KIND_STMT:
                    return NEW(
                      LilyCheckedBodyFunInfo,
                      body,
                      NEW(
                        LilyCheckedScope,
                        NEW_VARIANT(
                          LilyCheckedParent, stmt, parent, parent->decls.stmt),
                        NEW_VARIANT(LilyCheckedScopeDecls, scope, body)));
                case LILY_CHECKED_SCOPE_DECLS_KIND_MODULE:
                    return NEW(
                      LilyCheckedBodyFunInfo,
                      body,
                      NEW(LilyCheckedScope,
                          NEW_VARIANT(LilyCheckedParent,
                                      module,
                                      parent,
                                      parent->decls.module),
                          NEW_VARIANT(LilyCheckedScopeDecls, scope, body)));
                default:
                    UNREACHABLE("unknown variant");
            }
        }
        }
    }
}

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
    self->ref_count = 0;
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
    self->ref_count = 0;
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
    if (self->ref_count > 0) {
        --self->ref_count;

        return;
    }

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
