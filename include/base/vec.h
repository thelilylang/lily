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

#ifndef LILY_BASE_VEC_H
#define LILY_BASE_VEC_H

#include <base/macros.h>
#include <base/types.h>

#include <stdbool.h>

typedef struct Vec
{
    void **buffer;
    Usize len;
    Usize capacity;
    Usize default_capacity;
} Vec;

/**
 *
 * @brief Construct Vec type.
 */
CONSTRUCTOR(Vec *, Vec);

/**
 *
 * @brief Construct Vec type with default buffer items.
 */
Vec *
from__Vec(void **buffer, Usize len);

/**
 * 
 * @brief Initialize Vec with `...` parameter.
*/
Vec *
init__Vec(Usize len, ...);

/**
 *
 * @brief Get item from Vec.
 */
void *
get__Vec(Vec *self, Usize index);

/**
 *
 * @brief Grow Vec buffer.
 * @param new_capacity New buffer capacity.
 */
void
grow__Vec(Vec *self, Usize new_capacity);

/**
 *
 * @brief Remove the last item from Vec.
 */
void *
pop__Vec(Vec *self);

/**
 *
 * @brief Push item to Vec buffer.
 */
void
push__Vec(Vec *self, void *item);

/**
 *
 * @brief Remove item from Vec buffer.
 */
void *
remove__Vec(Vec *self, Usize index);

/**
 *
 * @brief Reverse Vec buffer.
 */
void
reverse__Vec(Vec *self);

/**
 *
 * @brief Ungrow Vec buffer.
 * @param new_capacity New buffer capacity.
 */
void
ungrow__Vec(Vec *self);

/**
 *
 * @brief Free Vec type.
 */
DESTRUCTOR(Vec, Vec *self);

#endif // LILY_BASE_VEC_H
