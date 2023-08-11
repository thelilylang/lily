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
#include <base/assert.h>

#include <core/lily/analysis/checked/body/fun.h>
#include <core/lily/analysis/checked/stmt/match.h>

#include <stdio.h>
#include <stdlib.h>

#ifdef ENV_DEBUG
#include <base/format.h>
#endif

bool
eq__LilyCheckedStmtMatchCaseValue(const LilyCheckedStmtMatchCaseValue *self,
                                  const LilyCheckedStmtMatchCaseValue *other)
{
    if (self->kind != other->kind) {
        UNREACHABLE("value must be the same");
    }

    switch (self->kind) {
        case LILY_CHECKED_STMT_MATCH_CASE_VALUE_KIND_INT:
            return self->int_ == other->int_;
        case LILY_CHECKED_STMT_MATCH_CASE_VALUE_KIND_FLOAT:
            return self->float_ == other->float_;
        case LILY_CHECKED_STMT_MATCH_CASE_VALUE_KIND_UINT:
            return self->uint == other->uint;
        default:
            UNREACHABLE("unknown variant");
    }
}

CONSTRUCTOR(LilyCheckedStmtMatchCase *,
            LilyCheckedStmtMatchCase,
            LilyCheckedExpr *cond,
            LilyCheckedStmtMatchCaseValue value,
            LilyCheckedBodyFunItem *body_item,
            LilyCheckedScope *parent)
{
    LilyCheckedStmtMatchCase *self =
      lily_malloc(sizeof(LilyCheckedStmtMatchCase));

    self->captured_variables = NEW(Vec);
    self->value = value;

    if (cond) {
        LilyCheckedBodyFunInfo if_info =
          wrap_item_in_body__LilyCheckedBodyFunItem(body_item, parent);

        self->body_item = NEW_VARIANT(
          LilyCheckedBodyFunItem,
          stmt,
          NEW_VARIANT(
            LilyCheckedStmt,
            if,
            cond->location,
            NULL,
            NEW(LilyCheckedStmtIf,
                NEW(LilyCheckedStmtIfBranch, cond, if_info.body, if_info.scope),
                NULL,
                NULL)));
    } else {
        self->body_item = body_item;
    }

    return self;
}

void
add_subcase__LilyCheckedStmtMatchCase(const LilyCheckedStmtMatchCase *self,
                                      LilyCheckedExpr *cond,
                                      LilyCheckedBodyFunItem *body_item,
                                      LilyCheckedScope *parent)
{
    if (cond) {
        LilyCheckedBodyFunInfo elif_info =
          wrap_item_in_body__LilyCheckedBodyFunItem(body_item, parent);

        ASSERT(self->body_item->kind == LILY_CHECKED_BODY_FUN_ITEM_KIND_STMT);
        ASSERT(self->body_item->stmt.kind == LILY_CHECKED_STMT_KIND_IF);

        if (self->body_item->stmt.if_.elifs) {
            push__Vec(self->body_item->stmt.if_.elifs,
                      NEW(LilyCheckedStmtIfBranch,
                          cond,
                          elif_info.body,
                          elif_info.scope));
        } else {
            self->body_item->stmt.if_.elifs =
              init__Vec(1,
                        NEW(LilyCheckedStmtIfBranch,
                            cond,
                            elif_info.body,
                            elif_info.scope));
        }
    } else {
        LilyCheckedBodyFunInfo else_info =
          wrap_item_in_body__LilyCheckedBodyFunItem(body_item, parent);

        ASSERT(self->body_item->kind == LILY_CHECKED_BODY_FUN_ITEM_KIND_STMT);
        ASSERT(self->body_item->stmt.kind == LILY_CHECKED_STMT_KIND_IF);

        self->body_item->stmt.if_.else_ =
          NEW(LilyCheckedStmtElseBranch, else_info.body, else_info.scope);
    }
}

void
add__LilyCheckedStmtMatchCase(const LilyCheckedStmtMatchCase *self,
                              LilyCheckedExpr *cond,
                              LilyCheckedBodyFunItem *body_item,
                              LilyCheckedScope *parent,
                              OrderedHashMap *captured_variables)
{
    add_captured_variables__LilyCheckedStmtMatchCase(self, captured_variables);
    add_subcase__LilyCheckedStmtMatchCase(self, cond, body_item, parent);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedStmtMatchCase,
               const LilyCheckedStmtMatchCase *self)
{
    String *res =
      from__String("LilyCheckedStmtMatchCase{ captured_variables = { ");

    for (Usize i = 0; i < self->captured_variables->len; ++i) {
        OrderedHashMap *captured_variables =
          get__Vec(self->captured_variables,
                   i); // OrderedHashMap<LilyCheckedCapturedVariable*>* (&)

        DEBUG_ORD_HASH_MAP_STRING(
          captured_variables, res, LilyCheckedCapturedVariable);

        if (i + 1 != self->captured_variables->len) {
            push_str__String(res, ", ");
        }
    }

    {
        char *s = format(" }, pattern = {Sr}",
                         to_string__Debug__LilyCheckedPattern(self->pattern));

        PUSH_STR_AND_FREE(res, s);
    }

    push_str__String(res, ", conds = { ");

    for (Usize i = 0; i < self->conds->len; ++i) {
        LilyCheckedExpr *cond =
          get__Vec(self->conds, i); // LilyCheckedExpr*? (&)

        if (cond) {
            String *s = to_string__Debug__LilyCheckedExpr(cond);

            APPEND_AND_FREE(res, s);
        } else {
            push_str__String(res, "NULL");
        }

        if (i + 1 != self->conds->len) {
            push_str__String(res, ", ");
        }
    }

    push_str__String(res, " }, body_items = {");

    DEBUG_VEC_STRING(self->body_items, res, LilyCheckedBodyFunItem);

    push_str__String(res, "}");

    return res;
}
#endif

DESTRUCTOR(LilyCheckedStmtMatchCase, LilyCheckedStmtMatchCase *self)
{
    for (Usize i = 0; i < self->captured_variables->len; ++i) {
        OrderedHashMap *captured_variables =
          get__Vec(self->captured_variables, i);

        FREE_ORD_HASHMAP_VALUES(captured_variables,
                                LilyCheckedCapturedVariable);
        FREE(OrderedHashMap, captured_variables);
    }

    FREE(Vec, self->captured_variables);
    FREE(LilyCheckedPattern, self->pattern);

    for (Usize i = 0; i < self->conds->len; ++i) {
        LilyCheckedExpr *cond = get__Vec(self->conds, i);

        if (cond) {
            FREE(LilyCheckedExpr, cond);
        }
    }

    FREE(Vec, self->conds);
    FREE_BUFFER_ITEMS(
      self->body_items->buffer, self->body_items->len, LilyCheckedBodyFunItem);
    FREE(Vec, self->body_items);
    lily_free(self);
}

void
add_case__LilyCheckedStmtMatch(const LilyCheckedStmtMatch *self,
                               LilyCheckedPattern *pattern,
                               OrderedHashMap *captured_variables,
                               LilyCheckedExpr *cond,
                               LilyCheckedBodyFunItem *body_item)
{
    for (Usize i = 0; i < self->cases->len; ++i) {
        LilyCheckedStmtMatchCase *case_ = get__Vec(self->cases, i);

        if (eq__LilyCheckedPattern(pattern, case_->pattern)) {
            FREE(LilyCheckedPattern, pattern);

            return add__LilyCheckedStmtMatchCase(
              case_, captured_variables, cond, body_item);
        }
    }

    LilyCheckedStmtMatchCase *case_ = NEW(LilyCheckedStmtMatchCase, pattern);

    push__Vec(self->cases, case_);

    return add__LilyCheckedStmtMatchCase(
      case_, captured_variables, cond, body_item);
}

int
look_for_case_error__LilyCheckedStmtMatch(const LilyCheckedStmtMatch *self,
                                          LilyCheckedPattern *pattern,
                                          LilyCheckedExpr *cond)
{
    for (Usize i = 0; i < self->cases->len; ++i) {
        LilyCheckedStmtMatchCase *case_ = get__Vec(self->cases, i);

        if (!eq__LilyCheckedPattern(case_->pattern, pattern)) {
            continue;
        }

        ASSERT(case_->captured_variables->len == case_->body_items->len);
        ASSERT(case_->body_items->len == case_->conds->len);

        for (Usize j = 0; j < case_->conds->len; ++j) {
            LilyCheckedExpr *case_cond = get__Vec(case_->conds, j);

            if (case_cond && cond) {
                // TODO: check conditon
            } else if (!case_cond && !cond) {
                return 2;
            }

            // Check for unused case
            if (j + 1 == case_->conds->len && cond && !case_cond) {
                return 1;
            }
        }

        break;
    }

    return 0;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedStmtMatch,
               const LilyCheckedStmtMatch *self)
{
    String *res = format__String("LilyCheckedStmtMatch{{ expr = {Sr}, cases =",
                                 to_string__Debug__LilyCheckedExpr(self->expr));

    DEBUG_VEC_STRING(self->cases, res, LilyCheckedStmtMatchCase);

    {
        char *s = format(", use_switch = {b} }", self->use_switch);

        PUSH_STR_AND_FREE(res, s);
    }

    return res;
}
#endif

DESTRUCTOR(LilyCheckedStmtMatch, const LilyCheckedStmtMatch *self)
{
    FREE(LilyCheckedExpr, self->expr);

    FREE_BUFFER_ITEMS(
      self->cases->buffer, self->cases->len, LilyCheckedStmtMatchCase);
    FREE(Vec, self->cases);
}
