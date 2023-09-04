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

#ifndef LILY_BASE_TRACE_MUT_H
#define LILY_BASE_TRACE_MUT_H

#include <base/assert.h>
#include <base/eq.h>
#include <base/macros.h>
#include <base/ptr.h>

#include <stdio.h>
#include <stdlib.h>

/* NOTE: You cannot free a trace mut pointer. */
#define TraceMut(T)                                         \
    /* TraceMutMut<T> */                                    \
    typedef struct TraceMut__##T                            \
    {                                                       \
        T **v; /* T** (&) */                                \
    } TraceMut__##T;                                        \
                                                            \
    /**                                                     \
     *                                                      \
     * @brief Construct TraceMutMut type.                   \
     */                                                     \
    inline CONSTRUCTOR(TraceMut__##T, TraceMut__##T, T **v) \
    {                                                       \
        ASSERT(v);                                          \
        return (TraceMut__##T){ .v = v };                   \
    }                                                       \
                                                            \
    /**                                                     \
     *                                                      \
     * @brief Get the mut traced pointer.                   \
     */                                                     \
    inline T *get__TraceMut__##T(Ptr(TraceMut__##T) self)   \
    {                                                       \
        ASSERT(self);                                       \
        return *self->v;                                    \
    }                                                       \
                                                            \
    /**                                                     \
     *                                                      \
     * @brief Implement Eq trait.                           \
     */                                                     \
    inline IMPL_FOR_EQ(Ptr(TraceMut__##T),                  \
                       TraceMut__##T,                       \
                       return EQ(T, **self->v, **other->v););

#define MAKE_TRACE_MUT(T, v) NEW(TraceMut__##T, v)

TraceMut(Int8);
TraceMut(Int16);
TraceMut(Int32);
TraceMut(Int64);

TraceMut(Uint8);
TraceMut(Uint16);
TraceMut(Uint32);
TraceMut(Uint64);

TraceMut(Isize);
TraceMut(Usize);

TraceMut(Float32);
TraceMut(Float64);

TraceMut(Uptr);

TraceMut(Bool);
TraceMut(Char);
TraceMut(Short);
TraceMut(Int);
TraceMut(Long);
TraceMut(Longlong);

TraceMut(Uchar);
TraceMut(Ushort);
TraceMut(Uint);
TraceMut(Ulong);
TraceMut(Ulonglong);

#endif // LILY_BASE_TRACE_MUT_H
