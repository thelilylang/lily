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

#ifndef LILY_BASE_TRACE_H
#define LILY_BASE_TRACE_H

#include <base/assert.h>
#include <base/eq.h>
#include <base/macros.h>

#include <stdio.h>
#include <stdlib.h>

// NOTE: You cannot free a trace pointer.
#define Trace(T) Trace__##T

#define DEF_TRACE(T)                             \
    /* Trace<T> */                               \
    typedef const T **Trace(T);                  \
                                                 \
    inline Trace(T) make__Trace__##T(Trace(T) v) \
    {                                            \
        return v;                                \
    }                                            \
                                                 \
    /**                                          \
     *                                           \
     * @brief Implement Eq trait.                \
     */                                          \
    inline IMPL_FOR_EQ(Trace(T), Trace(T), return EQ(T, **self, **other););

#define MAKE_TRACE(T, v) make__Trace__##T(v)

DEF_TRACE(Int8);
DEF_TRACE(Int16);
DEF_TRACE(Int32);
DEF_TRACE(Int64);

DEF_TRACE(Uint8);
DEF_TRACE(Uint16);
DEF_TRACE(Uint32);
DEF_TRACE(Uint64);

DEF_TRACE(Isize);
DEF_TRACE(Usize);

DEF_TRACE(Float32);
DEF_TRACE(Float64);

DEF_TRACE(Uptr);

DEF_TRACE(Bool);
DEF_TRACE(Char);
DEF_TRACE(Short);
DEF_TRACE(Int);
DEF_TRACE(Long);
DEF_TRACE(Longlong);

DEF_TRACE(Uchar);
DEF_TRACE(Ushort);
DEF_TRACE(Uint);
DEF_TRACE(Ulong);
DEF_TRACE(Ulonglong);

#endif // LILY_BASE_TRACE_H
