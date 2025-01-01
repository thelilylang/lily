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

#include <core/lily/analysis/checked/body/fun.h>
#include <core/lily/analysis/checked/stmt/if.h>

#ifdef ENV_DEBUG
#include <base/string.h>
#endif

CONSTRUCTOR(LilyCheckedStmtIfBranch *,
            LilyCheckedStmtIfBranch,
            LilyCheckedExpr *cond,
            Vec *body,
            LilyCheckedScope *scope)
{
    LilyCheckedStmtIfBranch *self =
      lily_malloc(sizeof(LilyCheckedStmtIfBranch));

    self->cond = cond;
    self->body = body;
    self->scope = scope;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedStmtIfBranch,
               const LilyCheckedStmtIfBranch *self)
{
    String *res =
      format__String("LilyCheckedStmtIfBranch{{ cond = {Sr}, body =",
                     to_string__Debug__LilyCheckedExpr(self->cond));

    DEBUG_VEC_STRING(self->body, res, LilyCheckedBodyFunItem);

    push_str__String(res, ", scope = ");

    {
        String *s = to_string__Debug__LilyCheckedScope(self->scope);

        APPEND_AND_FREE(res, s);
    }

    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(LilyCheckedStmtIfBranch, LilyCheckedStmtIfBranch *self)
{
    FREE(LilyCheckedExpr, self->cond);
    FREE_BUFFER_ITEMS(
      self->body->buffer, self->body->len, LilyCheckedBodyFunItem);
    FREE(Vec, self->body);
    FREE(LilyCheckedScope, self->scope);
    lily_free(self);
}

CONSTRUCTOR(LilyCheckedStmtElseBranch *,
            LilyCheckedStmtElseBranch,
            Vec *body,
            LilyCheckedScope *scope)
{
    LilyCheckedStmtElseBranch *self =
      lily_malloc(sizeof(LilyCheckedStmtElseBranch));

    self->body = body;
    self->scope = scope;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedStmtElseBranch,
               const LilyCheckedStmtElseBranch *self)
{
    String *res = from__String("LilyCheckedStmtElseBranch{ body =");

    DEBUG_VEC_STRING(self->body, res, LilyCheckedBodyFunItem);

    push_str__String(res, ", scope = ");

    {
        String *s = to_string__Debug__LilyCheckedScope(self->scope);

        APPEND_AND_FREE(res, s);
    }

    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(LilyCheckedStmtElseBranch, LilyCheckedStmtElseBranch *self)
{
    FREE_BUFFER_ITEMS(
      self->body->buffer, self->body->len, LilyCheckedBodyFunItem);
    FREE(Vec, self->body);
    FREE(LilyCheckedScope, self->scope);
    lily_free(self);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyCheckedStmtIf, const LilyCheckedStmtIf *self)
{
    String *res =
      format__String("LilyCheckedStmtIf{{ if_ = {Sr}, elifs =",
                     to_string__Debug__LilyCheckedStmtIfBranch(self->if_));

    if (self->elifs) {
        DEBUG_VEC_STRING(self->elifs, res, LilyCheckedStmtIfBranch);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, ", else_ = ");

    if (self->else_) {
        String *s = to_string__Debug__LilyCheckedStmtElseBranch(self->else_);

        APPEND_AND_FREE(res, s);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(LilyCheckedStmtIf, const LilyCheckedStmtIf *self)
{
    FREE(LilyCheckedStmtIfBranch, self->if_);

    if (self->elifs) {
        FREE_BUFFER_ITEMS(
          self->elifs->buffer, self->elifs->len, LilyCheckedStmtIfBranch);
        FREE(Vec, self->elifs);
    }

    if (self->else_) {
        FREE(LilyCheckedStmtElseBranch, self->else_);
    }
}
