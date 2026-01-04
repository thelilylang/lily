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
#include <core/lily/analysis/checked/pattern/list_tail.h>

bool
eq__LilyCheckedPatternListTail(const LilyCheckedPatternListTail *self,
                               const LilyCheckedPatternListTail *other)
{
    return self->left && self->right && other->left && other->right
             ? eq__LilyCheckedPattern(self->left, other->left) &&
                 eq__LilyCheckedPattern(self->right, other->right)
           : !self->left && !self->right && !other->left && !other->right
             ? true
             : false;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedPatternListTail,
               const LilyCheckedPatternListTail *self)
{
    return format__String(
      "LilyCheckedPatternListTail{{ left = {Sr}, right = {Sr} }",
      self->left ? to_string__Debug__LilyCheckedPattern(self->left)
                 : from__String("NULL"),
      self->right ? to_string__Debug__LilyCheckedPattern(self->right)
                  : from__String("NULL"));
}
#endif

DESTRUCTOR(LilyCheckedPatternListTail, const LilyCheckedPatternListTail *self)
{
    if (self->left) {
        FREE(LilyCheckedPattern, self->left);
    }

    if (self->right) {
        FREE(LilyCheckedPattern, self->right);
    }
}
