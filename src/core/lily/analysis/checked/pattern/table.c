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

#include <base/alloc.h>

#include <core/lily/analysis/checked/pattern.h>
#include <core/lily/analysis/checked/pattern/table.h>

#ifdef ENV_DEBUG
#include <base/format.h>
#endif

CONSTRUCTOR(LilyCheckedPatternTableItem *,
            LilyCheckedPatternTableItem,
            Usize id,
            LilyCheckedPattern *value)
{
    LilyCheckedPatternTableItem *self =
      lily_malloc(sizeof(LilyCheckedPatternTableItem));

    self->id = id;
    self->value = value;

    return self;
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedPatternTableItem,
               const LilyCheckedPatternTableItem *self)
{
    return format("LilyCheckedPatternTableItem {{ id = {zu}, value = {Sr} }",
                  self->id,
                  to_string__Debug__LilyCheckedPattern(self->value));
}
#endif

DESTRUCTOR(LilyCheckedPatternTableItem, LilyCheckedPatternTableItem *self)
{
    FREE(LilyCheckedPattern, self->value);
    lily_free(self);
}

LilyCheckedPatternTableItem *
get__LilyCheckedPatternTableItem(LilyCheckedPatternTable self, Usize id)
{
    for (Usize i = 0; i < self->len; ++i) {
        LilyCheckedPatternTableItem *item = get__Vec(self, i);

        if (item->id == id) {
            return item;
        }
    }

    return NULL;
}
