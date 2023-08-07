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
#include <core/lily/analysis/checked/stmt/match.h>

#ifdef ENV_DEBUG
#include <base/format.h>
#endif

CONSTRUCTOR(LilyCheckedStmtMatchCase *,
            LilyCheckedStmtMatchCase,
            OrderedHashMap *captured_variables,
            LilyCheckedPattern *pattern,
            LilyCheckedExpr *cond,
            LilyCheckedBodyFunItem *body_item)
{
    LilyCheckedStmtMatchCase *self =
      lily_malloc(sizeof(LilyCheckedStmtMatchCase));

    self->captured_variables = captured_variables;
    self->pattern = pattern;
    self->cond = cond;
    self->body_item = body_item;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedStmtMatchCase,
               const LilyCheckedStmtMatchCase *self)
{
    String *res =
      from__String("LilyCheckedStmtMatchCase{ captured_variables =");

    DEBUG_ORD_HASH_MAP_STRING(
      self->captured_variables, res, LilyCheckedCapturedVariable);

    {
        char *s = format(", pattern = {Sr}",
                         to_string__Debug__LilyCheckedPattern(self->pattern));

        PUSH_STR_AND_FREE(res, s);
    }

    if (self->cond) {
        char *s =
          format(", cond = {Sr}, body_item = {Sr} }",
                 to_string__Debug__LilyCheckedExpr(self->cond),
                 to_string__Debug__LilyCheckedBodyFunItem(self->body_item));

        PUSH_STR_AND_FREE(res, s);
    } else {
        char *s =
          format(", cond = NULL, body_item = {Sr} }",
                 to_string__Debug__LilyCheckedBodyFunItem(self->body_item));

        PUSH_STR_AND_FREE(res, s);
    }

    return res;
}
#endif

DESTRUCTOR(LilyCheckedStmtMatchCase, LilyCheckedStmtMatchCase *self)
{
    FREE(OrderedHashMap, self->captured_variables);
    FREE_ORD_HASHMAP_VALUES(self->captured_variables,
                            LilyCheckedCapturedVariable);

    FREE(LilyCheckedPattern, self->pattern);

    if (self->cond) {
        FREE(LilyCheckedExpr, self->cond);
    }

    FREE(LilyCheckedBodyFunItem, self->body_item);
    lily_free(self);
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
