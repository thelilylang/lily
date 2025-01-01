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

#include <base/assert.h>
#include <base/linked_list.h>
#include <base/new.h>

#include <stdio.h>
#include <stdlib.h>

CONSTRUCTOR(LinkedListNode *, LinkedListNode, void *value)
{
    LinkedListNode *self = lily_malloc(sizeof(LinkedListNode));

    self->value = value;
    self->next = NULL;

    return self;
}

CONSTRUCTOR(LinkedList *, LinkedList)
{
    LinkedList *self = lily_malloc(sizeof(LinkedList));

    self->first = NULL;
    self->last = NULL;
    self->len = 0;

    return self;
}

void
append__LinkedList(LinkedList *self, void *value)
{
    if (!self->first) {
        ASSERT(!self->last);

        self->first = NEW(LinkedListNode, value);
        self->last = self->first;
        ++self->len;

        return;
    }

    LinkedListNode *tmp = self->last;

    tmp->next = NEW(LinkedListNode, value);
    self->last = tmp->next;
    ++self->len;
}

void
prepend__LinkedList(LinkedList *self, void *value)
{
    if (!self->first) {
        return append__LinkedList(self, value);
    }

    LinkedListNode *tmp = self->first;

    self->first = NEW(LinkedListNode, value);
    self->first->next = tmp;
    ++self->len;
}

void *
remove__LinkedList(LinkedList *self, Usize index)
{
    ASSERT(index < self->len);

    LinkedListNode *current = self->first;
    LinkedListNode *prev = NULL; // LinkedListNode*?

    ASSERT(current);

    for (Usize i = 0; i < index; ++i) {
        prev = current;
        current = current->next;
    }

    void *res = current->value;

    if (prev) {
        prev->next = current->next;
    } else {
        self->first = current->next;
    }

    if (!current->next) {
        self->last = prev;
    }

    --self->len;

    FREE(LinkedListNode, current);

    return res;
}

void
insert__LinkedList(LinkedList *self, Usize index, void *value)
{
    ASSERT(index < self->len);

    LinkedListNode *current = self->first;
    LinkedListNode *prev = NULL;

    ASSERT(current);

    for (Usize i = 0; i < self->len; ++i) {
        prev = current;
        current = current->next;
    }

    LinkedListNode *new_node = NEW(LinkedListNode, value);

    if (prev) {
        prev->next = new_node;
    } else {
        self->first = new_node;
    }

    ++self->len;
}

DESTRUCTOR(LinkedList, LinkedList *self)
{
    LinkedListNode *current = self->first;
    LinkedListNode *prev = NULL;

    for (Usize i = 0; i < self->len; ++i) {
        prev = current;
        current = current->next;

        FREE(LinkedListNode, prev);
    }

    lily_free(self);
}
