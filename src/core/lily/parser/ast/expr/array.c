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

#include <core/lily/parser/ast/expr.h>
#include <core/lily/parser/ast/expr/array.h>

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyAstExprArray, const LilyAstExprArray *self)
{
    String *res = from__String("LilyAstExprArray{ items = { ");

    DEBUG_VEC_STRING(self->items, res, LilyAstExpr);

    push_str__String(res, "} }");

    return res;
}
#endif

String *
to_string__LilyAstExprArray(const LilyAstExprArray *self)
{
    String *res = from__String("[");

    for (Usize i = 0; i < self->items->len; ++i) {
        String *s = to_string__LilyAstExpr(get__Vec(self->items, i));

        APPEND_AND_FREE(res, s);

        if (i != self->items->len - 1) {
            push_str__String(res, ", ");
        }
    }

    push__String(res, ']');

    return res;
}

DESTRUCTOR(LilyAstExprArray, LilyAstExprArray *self)
{
    FREE_BUFFER_ITEMS(self->items->buffer, self->items->len, LilyAstExpr);
    FREE(Vec, self->items);
}
