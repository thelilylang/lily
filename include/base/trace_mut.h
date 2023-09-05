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

#include <stdio.h>
#include <stdlib.h>

/* NOTE: You cannot free a trace mut pointer. */
#define TraceMut(T) TraceMut__##T

#define DEF_TRACE_MUT(T)                                  \
    /* TraceMut<T> */                                     \
    typedef T **TraceMut(T);                              \
                                                          \
    inline TraceMut(T) make__TraceMut__##T(TraceMut(T) v) \
    {                                                     \
        return v;                                         \
    }                                                     \
                                                          \
    /**                                                   \
     *                                                    \
     * @brief Implement Eq trait.                         \
     */                                                   \
    inline DEF_FOR_EQ(TraceMut(T), TraceMut(T));          \
    inline IMPL_FOR_EQ(                                   \
      TraceMut(T), TraceMut(T), return EQ(T, **self, **other););

#define MAKE_TRACE_MUT(T, v) make__TraceMut__##T(v)

DEF_TRACE_MUT(Int8);
DEF_TRACE_MUT(Int16);
DEF_TRACE_MUT(Int32);
DEF_TRACE_MUT(Int64);

DEF_TRACE_MUT(Uint8);
DEF_TRACE_MUT(Uint16);
DEF_TRACE_MUT(Uint32);
DEF_TRACE_MUT(Uint64);

DEF_TRACE_MUT(Isize);
DEF_TRACE_MUT(Usize);

DEF_TRACE_MUT(Float32);
DEF_TRACE_MUT(Float64);

DEF_TRACE_MUT(Uptr);

DEF_TRACE_MUT(Bool);
DEF_TRACE_MUT(Char);
DEF_TRACE_MUT(Short);
DEF_TRACE_MUT(Int);
DEF_TRACE_MUT(Long);
DEF_TRACE_MUT(Longlong);

DEF_TRACE_MUT(Uchar);
DEF_TRACE_MUT(Ushort);
DEF_TRACE_MUT(Uint);
DEF_TRACE_MUT(Ulong);
DEF_TRACE_MUT(Ulonglong);

#endif // LILY_BASE_TRACE_MUT_H
