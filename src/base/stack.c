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
#include <base/assert.h>
#include <base/stack.h>

#include <stdio.h>
#include <stdlib.h>

CONSTRUCTOR(Stack *, Stack, Usize max_size)
{
    Stack *self = lily_malloc(sizeof(Stack));

    self->top = NULL;
    self->buffer = NULL;
    self->len = 0;
    self->max_size = max_size;

    return self;
}

bool
empty__Stack(const Stack *self)
{
    return self->len == 0;
}

void
push__Stack(Stack *self, void *item)
{
    ASSERT(self->len < self->max_size);

    if (!self->top) {
        self->top = item;
        ++self->len;

        return;
    } else if (!self->buffer) {
        self->buffer = lily_malloc(PTR_SIZE);
        self->buffer[0] = self->top;
        self->top = item;
        ++self->len;

        return;
    }

    self->buffer = lily_realloc(self->buffer, PTR_SIZE * self->len);
    self->buffer[self->len - 1] = self->top;
    self->top = item;
    ++self->len;
}

void *
pop__Stack(Stack *self)
{
    ASSERT(self->len > 0);

    void *res = NULL;
    SWAP(res, self->top);

    --self->len;

    if (self->len > 0) {
        SWAP(self->top, self->buffer[self->len - 1]);
    }

    return res;
}

void *
safe_pop__Stack(Stack *self)
{
    if (self->len == 0) {
        return NULL;
    }

    return pop__Stack(self);
}

void *
peek__Stack(const Stack *self)
{
    ASSERT(self->top);

    return self->top;
}

void *
visit__Stack(const Stack *self, Usize rsp)
{
    if (rsp == 0) {
        return peek__Stack(self);
    }

    ASSERT(self->buffer[rsp - 1]);

    return self->buffer[rsp - 1];
}

DESTRUCTOR(Stack, Stack *self)
{
    if (self->buffer) {
        lily_free(self->buffer);
    }

    lily_free(self);
}
