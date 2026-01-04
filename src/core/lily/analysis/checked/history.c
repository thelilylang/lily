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

#include <core/lily/analysis/checked/history.h>

#include <string.h>

CONSTRUCTOR(LilyCheckedHistory *, LilyCheckedHistory)
{
    LilyCheckedHistory *self = lily_malloc(sizeof(LilyCheckedHistory));

    self->items = NEW(Vec);

    return self;
}

bool
contains_for_fun__LilyCheckedHistory(const LilyCheckedHistory *self,
                                     LilyCheckedSignatureFun *signature)
{
    if (!signature) {
        return false;
    }

    for (Usize i = 0; i < self->items->len; ++i) {
        if (!strcmp(get_global_name__LilyCheckedDecl(get__Vec(self->items, i))
                      ->buffer,
                    signature->global_name->buffer)) {
            return true;
        }
    }

    return false;
}

bool
contains__LilyCheckedHistory(const LilyCheckedHistory *self,
                             String *global_name)
{
    for (Usize i = 0; i < self->items->len; ++i) {
        if (!strcmp(get_global_name__LilyCheckedDecl(get__Vec(self->items, i))
                      ->buffer,
                    global_name->buffer)) {
            return true;
        }
    }

    return false;
}

DESTRUCTOR(LilyCheckedHistory, LilyCheckedHistory **self)
{
    if ((*self)->items->len > 0) {
        pop__LilyCheckedHistory(*self);
    } else if ((*self)->items->len == 0) {
        FREE(Vec, (*self)->items);
        lily_free(*self);
        *self = NULL;
    }
}
