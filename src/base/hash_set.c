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

#include <base/alloc.h>
#include <base/hash_set.h>
#include <base/new.h>

#include <stdlib.h>

CONSTRUCTOR(HashSet *, HashSet)
{
    HashSet *self = lily_malloc(sizeof(HashSet));

    self->values = NEW(Vec);
    self->len = 0;

    return self;
}

void *
get__HashSet(HashSet *self, void *value)
{
    for (Usize i = 0; i < self->len; i++) {
        if (self->values->buffer[i] == value) {
            return self->values->buffer[i];
        }
    }

    return NULL;
}

bool
insert__HashSet(HashSet *self, void *value)
{
    for (Usize i = 0; i < self->len; i++) {
        if (self->values->buffer[i] == value) {
            return false;
        }
    }

    push__Vec(self->values, value);
    self->len++;

    return true;
}

DESTRUCTOR(HashSet, HashSet *self)
{
    FREE(Vec, self->values);
    lily_free(self);
}
