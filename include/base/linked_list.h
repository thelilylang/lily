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

#ifndef LILY_BASE_LINKED_LIST_H
#define LILY_BASE_LINKED_LIST_H

#include <base/alloc.h>
#include <base/macros.h>
#include <base/types.h>

#define FREE_LINKED_LIST_ITEMS(ll, type)      \
    {                                         \
        LinkedListNode *current = ll->first;  \
                                              \
        for (Usize i = 0; i < ll->len; ++i) { \
            FREE(type, current->value);       \
            current = current->next;          \
        }                                     \
    }

typedef struct LinkedListNode
{
    void *value;
    struct LinkedListNode *next; // struct LinkedListNode*? (&)
} LinkedListNode;

/**
 *
 * @brief Construct LinkedListNode type.
 */
CONSTRUCTOR(LinkedListNode *, LinkedListNode, void *value);

/**
 *
 * @brief Free LinkedListNode type.
 */
inline DESTRUCTOR(LinkedListNode, LinkedListNode *self)
{
    lily_free(self);
}

typedef struct LinkedList
{
    LinkedListNode *first; // LinkedListNode*?
    LinkedListNode *last;  // LinkedListNode*? (&)
    Usize len;
} LinkedList;

/**
 *
 * @brief Construct LinkedList type.
 */
CONSTRUCTOR(LinkedList *, LinkedList);

/**
 *
 * @brief Append node to the end of the list.
 */
void
append__LinkedList(LinkedList *self, void *value);

/**
 *
 * @brief Append node the begin of the list.
 */
void
prepend__LinkedList(LinkedList *self, void *value);

/**
 *
 * @brief Remove item at the specified index.
 * @return the removed value.
 */
void *
remove__LinkedList(LinkedList *self, Usize index);

/**
 *
 * @brief Insert a value at specified index.
 */
void
insert__LinkedList(LinkedList *self, Usize index, void *value);

/**
 *
 * @brief Free LinkedList type.
 */
DESTRUCTOR(LinkedList, LinkedList *self);

#endif // LILY_BASE_LINKED_LIST_H
