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

#ifndef LILY_BASE_SIZED_ARRAY_H
#define LILY_BASE_SIZED_ARRAY_H

#include <base/alloc.h>
#include <base/macros.h>
#include <base/new.h>
#include <base/types.h>
#include <base/vec.h>

typedef struct SizedArray
{
    void **buffer;
    Usize len;
} SizedArray;

/**
 *
 * @brief Construct SizedArray type.
 */
CONSTRUCTOR(SizedArray *, SizedArray, void **buffer, Usize len);

/**
 *
 * @brief Convert basic array to SizedArray.
 */
inline SizedArray *
from__SizedArray(void **buffer, Usize len)
{
    return NEW(SizedArray, buffer, len);
}

/**
 *
 * @brief Convert Vec to SizedArray.
 */
inline SizedArray
from_vec__SizedArray(Vec *vec)
{
    return (SizedArray){ .buffer = vec->buffer, .len = vec->len };
}

/**
 *
 * @brief Get item from SizedArray.
 */
void *
get__SizedArray(const SizedArray *self, Usize index);

/**
 *
 * @brief Get item from SizedArray (no assert).
 */
inline void *
safe_get__SizedArray(const SizedArray *self, Usize index)
{
    return index < self->len ? self->buffer[index] : NULL;
}

/**
 *
 * @brief Free SizedArray type.
 */
inline DESTRUCTOR(SizedArray, SizedArray *self)
{
    lily_free(self);
}

typedef struct SizedArrayIter
{
    const SizedArray *sized_array;
    Usize count;
} SizedArrayIter;

/**
 *
 * @brief Construct SizedArrayIter type.
 */
inline CONSTRUCTOR(SizedArrayIter,
                   SizedArrayIter,
                   const SizedArray *sized_array)
{
    return (SizedArrayIter){ .sized_array = sized_array, .count = 0 };
}

/**
 *
 * @brief Get the next value.
 */
inline void *
next__SizedArrayIter(SizedArrayIter *self)
{
    return safe_get__SizedArray(self->sized_array, self->count++);
}

#endif // LILY_BASE_SIZED_ARRAY_H
