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
#include <base/assert.h>
#include <base/list.h>

#include <stdio.h>
#include <stdlib.h>

CONSTRUCTOR(List *, List)
{
    List *self = lily_malloc(sizeof(List));

    self->buffer = NULL;
    self->len = 0;

    return self;
}

void
add__List(List *self, void *item)
{
    if (self->buffer) {
        self->buffer = lily_realloc(self->buffer, ++self->len);
        self->buffer[self->len - 1] = item;
    } else {
        self->buffer = lily_malloc(PTR_SIZE);
        self->buffer[0] = item;
        ++self->len;
    }
}

void
append__List(List *self, const List *other)
{
    for (Usize i = 0; i < other->len; ++i) {
        add__List(self, other->buffer[i]);
    }
}

void *
get__List(List *self, Usize index)
{
    ASSERT(index < self->len);

    return self->buffer[index];
}

DESTRUCTOR(List, List *self)
{
    lily_free(self->buffer);
    lily_free(self);
}
