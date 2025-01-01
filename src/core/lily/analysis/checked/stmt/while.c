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

#include <base/new.h>

#include <core/lily/analysis/checked/body/fun.h>
#include <core/lily/analysis/checked/stmt/while.h>

#include <stdlib.h>

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedStmtWhile,
               const LilyCheckedStmtWhile *self)
{
    String *res = NULL;

    if (self->name) {
        res = format__String("LilyCheckedStmtWhile{{ name = {S}, cond = {Sr}",
                             self->name,
                             to_string__Debug__LilyCheckedExpr(self->cond));
    } else {
        res = format__String("LilyCheckedStmtWhile{{ name = NULL, cond = {Sr}",
                             to_string__Debug__LilyCheckedExpr(self->cond));
    }

    push_str__String(res, ", body =");

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

DESTRUCTOR(LilyCheckedStmtWhile, const LilyCheckedStmtWhile *self)
{
    FREE(LilyCheckedExpr, self->cond);

    FREE_BUFFER_ITEMS(
      self->body->buffer, self->body->len, LilyCheckedBodyFunItem);
    FREE(Vec, self->body);

    FREE(LilyCheckedScope, self->scope);
}
