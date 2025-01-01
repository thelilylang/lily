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

#ifndef LILY_BASE_OPTIONAL_H
#define LILY_BASE_OPTIONAL_H

#include <base/alloc.h>
#include <base/assert.h>
#include <base/macros.h>
#include <base/new.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Optional
{
    union
    {
        void *some;
    };
    enum
    {
        OPTIONAL_SOME = 1,
        OPTIONAL_NONE = 0
    } state;
} Optional;

/**
 *
 * @brief Construct Optional type (OPTIONAL_SOME).
 */
VARIANT_CONSTRUCTOR(Optional *, Optional, some, void *some);

/**
 *
 * @brief Construct Optional type (OPTIONAL_NONE).
 */
VARIANT_CONSTRUCTOR(Optional *, Optional, none);

/**
 *
 * @brief Get some value.
 */
void *
get__Optional(const Optional *self);

/**
 *
 * @brief Return 1 if optional is some otherwise return 0.
 */
inline bool
is_some__Optional(const Optional *self)
{
    return self->state;
}

/**
 *
 * @brief Return 1 if optional is none otherwise return 0.
 */
inline bool
is_none__Optional(const Optional *self)
{
    return !self->state;
}

/**
 *
 * @brief Free Optional type.
 */
DESTRUCTOR(Optional, Optional *self);

#define SOME(x) NEW_VARIANT(Optional, some, x)
#define NONE NEW_VARIANT(Optional, none)

#endif // LILY_BASE_OPTIONAL_H
