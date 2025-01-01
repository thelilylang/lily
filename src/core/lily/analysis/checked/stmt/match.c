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
#include <core/lily/analysis/checked/stmt/match.h>

#ifdef ENV_DEBUG
#include <base/format.h>
#endif

CONSTRUCTOR(LilyCheckedStmtMatchSubCase *,
            LilyCheckedStmtMatchSubCase,
            LilyCheckedExpr *cond,
            LilyCheckedBodyFunItem *body_item)
{
    LilyCheckedStmtMatchSubCase *self =
      lily_malloc(sizeof(LilyCheckedStmtMatchSubCase));

    self->cond = cond;
    self->body_item = body_item;

    return self;
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedStmtMatchSubCase,
               const LilyCheckedStmtMatchSubCase *self)
{
    return format(
      "LilyCheckedStmtMatchSubCase{{ cond = {Sr}, body_item = {Sr} }",
      self->cond ? to_string__Debug__LilyCheckedExpr(self->cond)
                 : from__String("NULL"),
      to_string__Debug__LilyCheckedBodyFunItem(self->body_item));
}
#endif

DESTRUCTOR(LilyCheckedStmtMatchSubCase, LilyCheckedStmtMatchSubCase *self)
{
    if (self->cond) {
        FREE(LilyCheckedExpr, self->cond);
    }

    FREE(LilyCheckedBodyFunItem, self->body_item);
    lily_free(self);
}

CONSTRUCTOR(LilyCheckedStmtMatchCase *,
            LilyCheckedStmtMatchCase,
            LilyCheckedPattern *pattern,
            LilyCheckedExpr *cond,
            LilyCheckedBodyFunItem *body_item)
{
    LilyCheckedStmtMatchCase *self =
      lily_malloc(sizeof(LilyCheckedStmtMatchCase));

    self->pattern = pattern;
    self->sub_cases =
      init__Vec(1, NEW(LilyCheckedStmtMatchSubCase, cond, body_item));

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedStmtMatchCase,
               const LilyCheckedStmtMatchCase *self)
{
    String *res =
      format__String("LilyCheckedStmtMatchCase{{ pattern = {Sr}, sub_cases =",
                     to_string__Debug__LilyCheckedPattern(self->pattern));

    DEBUG_VEC_STR(self->sub_cases, res, LilyCheckedStmtMatchSubCase);

    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(LilyCheckedStmtMatchCase, LilyCheckedStmtMatchCase *self)
{
    FREE(LilyCheckedPattern, self->pattern);
    FREE_BUFFER_ITEMS(self->sub_cases->buffer,
                      self->sub_cases->len,
                      LilyCheckedStmtMatchSubCase);
    FREE(Vec, self->sub_cases);
    lily_free(self);
}

void
add_case__LilyCheckedStmtMatch(const LilyCheckedStmtMatch *self,
                               LilyCheckedPattern *pattern,
                               LilyCheckedExpr *cond,
                               LilyCheckedBodyFunItem *body_item)
{
    for (Usize i = 0; i < self->cases->len; ++i) {
        LilyCheckedStmtMatchCase *case_ = get__Vec(self->cases, i);

        if (eq__LilyCheckedPattern(pattern, case_->pattern)) {
            FREE(LilyCheckedPattern, pattern);

            push__Vec(case_->sub_cases,
                      NEW(LilyCheckedStmtSwitchSubCase, cond, body_item));

            return;
        }
    }

    push__Vec(self->cases,
              NEW(LilyCheckedStmtMatchCase, pattern, cond, body_item));
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

        for (Usize j = 0; j < case_->sub_cases->len; ++j) {
            LilyCheckedStmtMatchSubCase *sub_case =
              get__Vec(case_->sub_cases, j);

            if (sub_case->cond && cond) {
                if (eq__LilyCheckedExpr(sub_case->cond, cond)) {
                    return 2;
                }
            } else if (!sub_case->cond && !cond) {
                return 2;
            }

            // Check for unused case
            if (j + 1 == case_->sub_cases->len && cond && !sub_case->cond) {
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
        char *s = format(", has_else = {b} }", self->has_else);

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
