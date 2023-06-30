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

#ifndef LILY_BASE_SIZED_ARRAY_H
#define LILY_BASE_SIZED_ARRAY_H

#include <base/alloc.h>
#include <base/macros.h>
#include <base/types.h>

typedef struct SizedArray
{
    void **items;
    Usize len;
} SizedArray;

/**
 *
 * @brief Construct SizedArray type.
 */
CONSTRUCTOR(SizedArray *, SizedArray, void **items, Usize len);

/**
 *
 * @brief Free SizedArray type.
 */
inline DESTRUCTOR(SizedArray, SizedArray *self)
{
    lily_free(self);
}

#endif // LILY_BASE_SIZED_ARRAY_H
