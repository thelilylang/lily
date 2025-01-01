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
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, PATTERNESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <base/new.h>

#include <core/lily/parser/ast/expr.h>
#include <core/lily/parser/ast/pattern.h>
#include <core/lily/parser/ast/pattern/error.h>

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyAstPatternError, const LilyAstPatternError *self)
{
    if (self->pattern) {
        return format__String(
          "LilyAstPatternError{{ name = {Sr}, pattern = {Sr} }",
          to_string__Debug__LilyAstExpr(self->id),
          to_string__Debug__LilyAstPattern(self->pattern));
    }

    return format__String("LilyAstPatternError{{ name = {Sr}, pattern = NULL }",
                          to_string__Debug__LilyAstExpr(self->id));
}
#endif

DESTRUCTOR(LilyAstPatternError, const LilyAstPatternError *self)
{
    FREE(LilyAstExpr, self->id);

    if (self->pattern) {
        FREE(LilyAstPattern, self->pattern);
    }
}
