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

#ifndef LILY_BASE_HASH_SET_H
#define LILY_BASE_HASH_SET_H

#include <base/macros.h>
#include <base/types.h>
#include <base/vec.h>

#include <stdbool.h>

typedef struct HashSet
{
    Vec *values;
    Usize len;
} HashSet;

/**
 *
 * @brief Construct HashSet type.
 */
CONSTRUCTOR(HashSet *, HashSet);

/**
 *
 * @brief Get value from values.
 * @return If the value does not exist, return NULL.
 */
void *
get__HashSet(HashSet *self, void *value);

/**
 *
 * @brief Insert value into HashSet.
 * @return If the value already exists, return false otherwise return true.
 */
bool
insert__HashSet(HashSet *self, void *value);

/**
 *
 * @brief Free HashSet type.
 */
DESTRUCTOR(HashSet, HashSet *self);

#endif // LILY_BASE_HASH_SET_H
