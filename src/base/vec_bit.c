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
#include <base/vec_bit.h>

#include <stdio.h>
#include <string.h>

#define ELEMENT_BY_ITEM 8

CONSTRUCTOR(VecBit *, VecBit)
{
    VecBit *self = lily_malloc(sizeof(VecBit));

    self->items = lily_calloc(VEC_BIT_DEFAULT_CAPACITY, sizeof(Uint8));
    self->capacity = VEC_BIT_DEFAULT_CAPACITY;
    self->count = 0;

    return self;
}

void
add__VecBit(VecBit *self, Usize element)
{
    Usize element_index = element / ELEMENT_BY_ITEM;
    Usize new_capacity = self->capacity;

    while (element_index >= new_capacity) {
        new_capacity *= 2;
    }

    if (self->capacity != new_capacity) {
        self->items = lily_realloc(self->items, sizeof(Uint8) * new_capacity);

        memset(self->items + self->capacity, 0, new_capacity - self->capacity);

        self->capacity = new_capacity;
    }

    Uint8 old_item = self->items[element_index];

    self->items[element_index] |= 1 << (element % ELEMENT_BY_ITEM);

    if (old_item != self->items[element_index]) {
        ++self->count;
    }
}

bool
has__VecBit(VecBit *self, Usize element)
{
    Usize element_index = element / ELEMENT_BY_ITEM;

    return element_index >= self->capacity
             ? false
             : self->items[element_index] >> (element % ELEMENT_BY_ITEM) & 0x1;
}

void
remove__VecBit(VecBit *self, Usize element)
{
    Usize element_index = element / ELEMENT_BY_ITEM;

    ASSERT(element_index < self->capacity);

    Uint8 old_item = self->items[element_index];

    self->items[element_index] &= ~(1 << (element % ELEMENT_BY_ITEM));

    if (old_item != self->items[element_index]) {
        --self->count;
    }
}

DESTRUCTOR(VecBit, VecBit *self)
{
    lily_free(self->items);
    lily_free(self);
}
