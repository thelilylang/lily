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

#include <core/lily/parser/ast/pattern.h>
#include <core/lily/parser/ast/pattern/array.h>

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyAstPatternArray, const LilyAstPatternArray *self)
{
    String *res = from__String("LilyAstPatternArray{ patterns = { ");

    DEBUG_VEC_STRING(self->patterns, res, LilyAstPattern);

    push_str__String(res, "} }");

    return res;
}
#endif

String *
to_string__LilyAstPatternArray(const LilyAstPatternArray *self)
{
    String *res = from__String("[");

    for (Usize i = 0; i < self->patterns->len; ++i) {
        String *s = to_string__LilyAstPattern(get__Vec(self->patterns, i));

        APPEND_AND_FREE(res, s);

        if (i != self->patterns->len - 1) {
            push_str__String(res, ", ");
        }
    }

    push__String(res, ']');

    return res;
}

DESTRUCTOR(LilyAstPatternArray, LilyAstPatternArray *self)
{
    FREE_BUFFER_ITEMS(
      self->patterns->buffer, self->patterns->len, LilyAstPattern);
    FREE(Vec, self->patterns);
}
