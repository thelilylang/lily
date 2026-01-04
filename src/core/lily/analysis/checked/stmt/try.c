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

#include <core/lily/analysis/checked/body/fun.h>
#include <core/lily/analysis/checked/stmt/try.h>

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyCheckedStmtTry, const LilyCheckedStmtTry *self)
{
    String *res = from__String("LilyCheckedStmtTry{ try_body =");

    DEBUG_VEC_STRING(self->try_body, res, LilyCheckedBodyFunItem);

    push_str__String(res, ", try_scope = ");

    {
        String *s = to_string__Debug__LilyCheckedScope(self->try_scope);

        APPEND_AND_FREE(res, s);
    }

    if (self->catch_body) {
        push_str__String(res, ", catch_body =");
        DEBUG_VEC_STRING(self->catch_body, res, LilyCheckedBodyFunItem);

        push_str__String(res, ", catch_scope = ");

        {
            String *s = to_string__Debug__LilyCheckedScope(self->catch_scope);

            APPEND_AND_FREE(res, s);
        }

        push_str__String(res, " }");
    } else {
        push_str__String(res, " }");
    }

    return res;
}
#endif

DESTRUCTOR(LilyCheckedStmtTry, const LilyCheckedStmtTry *self)
{
    FREE_BUFFER_ITEMS(
      self->try_body->buffer, self->try_body->len, LilyCheckedBodyFunItem);
    FREE(Vec, self->try_body);

    FREE(LilyCheckedScope, self->try_scope);

    if (self->catch_body) {
        FREE_BUFFER_ITEMS(self->catch_body->buffer,
                          self->catch_body->len,
                          LilyCheckedBodyFunItem);
        FREE(Vec, self->catch_body);

        FREE(LilyCheckedScope, self->catch_scope);
    }
}
