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

#ifndef LILY_BASE_BUFFER_H
#define LILY_BASE_BUFFER_H

#include <assert.h>
#include <base/allocator.h>

#define Buffer(T)             \
    struct                    \
    {                         \
        Allocator *allocator; \
        T *mem; /* T*? */     \
        Usize len;            \
        Usize capacity;       \
        Usize resize_coeff;   \
    }

#define __new__Buffer(a, c, rc) \
    { .allocator = a, .mem = NULL, .len = 0, .capacity = c, .resize_coeff = rc }

#define push__Buffer(buffer, item)                            \
    do {                                                      \
        if (!(buffer).mem) {                                  \
            (buffer).mem = A_ALLOC(__typeof__(*(buffer).mem), \
                                   *(buffer).allocator,       \
                                   (buffer).capacity);        \
            (buffer).mem[0] = item;                           \
            ++(buffer).len;                                   \
                                                              \
            break;                                            \
        }                                                     \
                                                              \
        if ((buffer).capacity == (buffer).len) {              \
            grow__Buffer(buffer);                             \
        }                                                     \
                                                              \
        (buffer).mem[(buffer).len++] = item;                  \
    } while (0);

#define grow__Buffer(buffer)                    \
    ASSERT((buffer).mem);                       \
    (buffer).capacity *= (buffer).resize_coeff; \
    A_RESIZE(__typeof__(*(buffer).mem),         \
             *(buffer).allocator,               \
             (buffer).mem,                      \
             (buffer).capacity);

#define __free__Buffer(buffer) A_FREE(*(buffer).allocator, (buffer).mem);

#endif // LILY_BASE_BUFFER_H
