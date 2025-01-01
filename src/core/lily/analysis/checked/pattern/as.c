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

#include <core/lily/analysis/checked/pattern.h>
#include <core/lily/analysis/checked/pattern/as.h>

bool
eq__LilyCheckedPatternAs(const LilyCheckedPatternAs *self,
                         const LilyCheckedPatternAs *other)
{
    return eq__LilyCheckedPattern(self->pattern, other->pattern);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedPatternAs,
               const LilyCheckedPatternAs *self)
{
    return format__String("LilyCheckedPatternAs{{ pattern = {Sr}, as = {S} }",
                          to_string__Debug__LilyCheckedPattern(self->pattern),
                          self->name);
}
#endif

DESTRUCTOR(LilyCheckedPatternAs, const LilyCheckedPatternAs *self)
{
    FREE(LilyCheckedPattern, self->pattern);
    FREE_MOVE(self->name, FREE(String, self->name));
}
