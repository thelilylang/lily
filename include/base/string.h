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

#ifndef LILY_BASE_STRING_H
#define LILY_BASE_STRING_H

#include <base/types.h>

typedef struct String {
  char *buffer;
  Usize len;
  Usize capacity;
  Usize default_capacity;
} String;

/**
 *
 * @brief Construct String type.
 */
String *__new__String();

/**
 *
 * @brief Construct String type with default buffer items.
 */
String *from__String(char *buffer);

/**
 *
 * @brief Get item from String.
 */
char get__String(String *self, Usize index);

/**
 *
 * @brief Grow String buffer.
 * @param new_capacity New buffer capacity.
 */
void grow__String(String *self, Usize new_capacity);

/**
 *
 * @brief Remove last item from String.
 */
char pop__String(String *self);

/**
 *
 * @brief Push char to String.
 */
void push__String(String *self, char item);

/**
 *
 * @brief Ungrow String buffer.
 */
void ungrow__String(String *self);

/**
 *
 * @brief Free String buffer.
 */
void __free__String(String *self);

#endif // LILY_BASE_STRING_H
