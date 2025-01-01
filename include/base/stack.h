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

#ifndef LILY_BASE_STACK_H
#define LILY_BASE_STACK_H

#include <base/macros.h>
#include <base/types.h>

#include <stdbool.h>

#define FREE_STACK_ITEMS(stack, type)                \
    if (stack->top) {                                \
        FREE(type, stack->top);                      \
    }                                                \
    if (stack->buffer && stack->len > 1) {           \
        for (Usize i = 0; i < stack->len - 1; ++i) { \
            FREE(type, stack->buffer[i]);            \
        }                                            \
    }

/*
    Stack<T>
*/
typedef struct Stack
{
    void *top;     // void*?
    void **buffer; // void**?
    Usize len;
    Usize max_size;
} Stack;

/**
 *
 * @brief Construct Stack type.
 */
CONSTRUCTOR(Stack *, Stack, Usize max_size);

/**
 *
 * @brief Check if the stack is empty.
 */
bool
empty__Stack(const Stack *self);

/**
 *
 * @brief Push an item to the Stack.
 */
void
push__Stack(Stack *self, void *item);

/**
 *
 * @brief Remove top element.
 */
void *
pop__Stack(Stack *self);

/**
 *
 * @brief Remove top element safely (without assertion failure).
 * @return void*?
 */
void *
safe_pop__Stack(Stack *self);

/**
 *
 * @brief Get the top element without removing it.
 */
void *
peek__Stack(const Stack *self);

/**
 *
 * @brief Visit the stack at n index.
 */
void *
visit__Stack(const Stack *self, Usize rsp);

/**
 *
 * @brief Free Stack type.
 */
DESTRUCTOR(Stack, Stack *self);

#endif // LILY_BASE_STACK_H
