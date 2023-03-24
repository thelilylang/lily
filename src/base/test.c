/*
 * MIT License
 *
 * Copyright (c) 2022-2023 ArthurPV
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
#include <base/new.h>
#include <base/test.h>

#include <stdio.h>

// Free TestItem type (TEST_ITEM_KIND_SIMPLE).
static inline VARIANT_DESTRUCTOR(TestItem, simple, TestItem *self);

// Free TestItem type (TEST_ITEM_KIND_SUITE).
static VARIANT_DESTRUCTOR(TestItem, suite, TestItem *self);

CONSTRUCTOR(TestCase *, TestCase, char *name, int (*f)(TestSuite *))
{
    TestCase *self = lily_malloc(sizeof(TestCase));

    self->name = name;
    self->f = f;

    return self;
}

DESTRUCTOR(TestSuite, const TestSuite *self)
{
    FREE_BUFFER_ITEMS(self->cases->buffer, self->cases->len, TestCase);
    FREE(Vec, self->cases);
}

VARIANT_CONSTRUCTOR(TestItem *, TestItem, simple, TestSimple simple)
{
    TestItem *self = lily_malloc(sizeof(TestItem));

    self->kind = TEST_ITEM_KIND_SIMPLE;
    self->simple = simple;

    return self;
}

VARIANT_CONSTRUCTOR(TestItem *, TestItem, suite, TestSuite suite)
{
    TestItem *self = lily_malloc(sizeof(TestItem));

    self->kind = TEST_ITEM_KIND_SUITE;
    self->suite = suite;

    return self;
}

VARIANT_DESTRUCTOR(TestItem, simple, TestItem *self)
{
    lily_free(self);
}

VARIANT_DESTRUCTOR(TestItem, suite, TestItem *self)
{
    FREE(TestSuite, &self->suite);
    lily_free(self);
}

DESTRUCTOR(TestItem, TestItem *self)
{
    switch (self->kind) {
        case TEST_ITEM_KIND_SIMPLE:
            FREE_VARIANT(TestItem, simple, self);
            break;
        case TEST_ITEM_KIND_SUITE:
            FREE_VARIANT(TestItem, suite, self);
            break;
        default:
            UNREACHABLE("unknown variant");
    }
}

DESTRUCTOR(Test, const Test *self)
{
    FREE_BUFFER_ITEMS(self->items->buffer, self->items->len, TestItem);
    FREE(Vec, self->items);
}