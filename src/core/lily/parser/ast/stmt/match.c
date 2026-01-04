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
#include <core/lily/parser/ast/stmt/match.h>

#ifdef ENV_DEBUG
#include <base/format.h>
#endif

CONSTRUCTOR(LilyAstStmtMatchCase *,
            LilyAstStmtMatchCase,
            LilyAstPattern *pattern,
            LilyAstExpr *cond,
            LilyAstBodyFunItem *body_item)
{
    LilyAstStmtMatchCase *self = lily_malloc(sizeof(LilyAstStmtMatchCase));

    self->pattern = pattern;
    self->cond = cond;
    self->body_item = body_item;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyAstStmtMatchCase,
               const LilyAstStmtMatchCase *self)
{
    String *res =
      format__String("LilyAstStmtMatchCase{{ pattern = {Sr}",
                     to_string__Debug__LilyAstPattern(self->pattern));

    if (self->cond) {
        char *s = format(", cond = {Sr}, body_item = {Sr} }",
                         to_string__Debug__LilyAstExpr(self->cond),
                         to_string__Debug__LilyAstBodyFunItem(self->body_item));

        PUSH_STR_AND_FREE(res, s);
    } else {
        char *s = format(", cond = NULL, body_item = {Sr} }",
                         to_string__Debug__LilyAstBodyFunItem(self->body_item));

        PUSH_STR_AND_FREE(res, s);
    }

    return res;
}
#endif

DESTRUCTOR(LilyAstStmtMatchCase, LilyAstStmtMatchCase *self)
{
    FREE(LilyAstPattern, self->pattern);

    if (self->cond) {
        FREE(LilyAstExpr, self->cond);
    }

    FREE(LilyAstBodyFunItem, self->body_item);
    lily_free(self);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyAstStmtMatch, const LilyAstStmtMatch *self)
{
    String *res = format__String("LilyAstStmtMatch{{ expr = {Sr}, cases =",
                                 to_string__Debug__LilyAstExpr(self->expr));

    DEBUG_VEC_STRING(self->cases, res, LilyAstStmtMatchCase);

    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(LilyAstStmtMatch, const LilyAstStmtMatch *self)
{
    FREE(LilyAstExpr, self->expr);

    FREE_BUFFER_ITEMS(
      self->cases->buffer, self->cases->len, LilyAstStmtMatchCase);
    FREE(Vec, self->cases);
}
