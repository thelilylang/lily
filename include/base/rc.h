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

#ifndef LILY_BASE_RC_H
#define LILY_BASE_RC_H

#include <base/macros.h>
#include <base/types.h>

#define FREE_RC(T, self)             \
    do {                             \
        if ((self)->ref_count > 0) { \
            --(self)->ref_count;     \
            break;                   \
        }                            \
                                     \
        FREE(T, self->ptr);          \
        lily_free((self));           \
    } while (0);

#define GET_PTR_RC(T, self) ((T *)((self)->ptr))

typedef struct Rc
{
    void *ptr;
    Usize ref_count;
} Rc;

/**
 *
 * @brief Construct Rc type.
 */
CONSTRUCTOR(Rc *, Rc, void *ptr);

/**
 *
 * @brief Increment the count of Rc type.
 * @return Rc*
 */
inline Rc *
ref__Rc(Rc *self)
{
    ++self->ref_count;
    return self;
}

#endif // LILY_BASE_RC_H
