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

#ifndef LILY_BASE_BUFFER_H
#define LILY_BASE_BUFFER_H

#include <assert.h>
#include <base/allocator.h>

#define Buffer(T)               \
    struct                      \
    {                           \
        Allocator *allocator;   \
        T *mem; /* T*? */       \
        Usize len;              \
        Usize default_capacity; \
        Usize capacity;         \
        Usize resize_coeff;     \
    }

#define __new__Buffer(a, c, rc)                                       \
    {                                                                 \
        .allocator = a, .mem = NULL, .len = 0, .default_capacity = c, \
        .capacity = c, .resize_coeff = rc                             \
    }

#define push__Buffer(self, item)                                            \
    do {                                                                    \
        if (!(self).mem) {                                                  \
            (self).mem = A_ALLOC(                                           \
              __typeof__(*(self).mem), *(self).allocator, (self).capacity); \
            (self).mem[0] = item;                                           \
            ++(self).len;                                                   \
                                                                            \
            break;                                                          \
        }                                                                   \
                                                                            \
        if ((self).capacity == (self).len) {                                \
            grow__Buffer(self);                                             \
        }                                                                   \
                                                                            \
        (self).mem[(self).len++] = item;                                    \
    } while (0);

#define get__Buffer(self, index)                           \
    {                                                      \
        (ASSERT(index < (self).len);                       \
         __typeof__(*(self).mem) _res = (self).mem[index]; \
         res;)                                             \
    }

#define grow__Buffer(self)                  \
    ASSERT((self).mem);                     \
    (self).capacity *= (self).resize_coeff; \
    A_RESIZE(__typeof__(*(self).mem),       \
             *(self).allocator,             \
             (self).mem,                    \
             (self).capacity);

#define __free__Buffer(self)                   \
    if ((self).mem) {                          \
        A_FREE(*(self).allocator, (self).mem); \
    }

#endif // LILY_BASE_BUFFER_H
