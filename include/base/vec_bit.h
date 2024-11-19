/*
 * MIT License
 *
 * Copyright (c) 2022-2024 ArthurPV
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

#ifndef LILY_BASE_VEC_BIT_H
#define LILY_BASE_VEC_BIT_H

#include <base/macros.h>
#include <base/types.h>

#define VEC_BIT_DEFAULT_CAPACITY 4

typedef struct VecBit
{
    Uint8 *items;
    Usize capacity;
    Usize count;
} VecBit;

/**
 *
 * @brief Construct VecBit type.
 */
CONSTRUCTOR(VecBit *, VecBit);

/**
 *
 * @brief Add element to the VecBit.
 */
void
add__VecBit(VecBit *self, Usize element);

/**
 *
 * @brief Check if the element is set.
 */
bool
has__VecBit(VecBit *self, Usize element);

/**
 *
 * @brief Remove element to the VecBit.
 */
void
remove__VecBit(VecBit *self, Usize element);

/**
 *
 * @brief Free VecBit type.
 */
DESTRUCTOR(VecBit, VecBit *self);

#endif // LILY_BASE_VEC_BIT_H
