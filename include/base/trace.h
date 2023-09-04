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
#include <base/ptr.h>

#include <stdio.h>
#include <stdlib.h>

// NOTE: You cannot free a trace pointer.
#define Trace(T)                                            \
    /* Trace<T> */                                          \
    typedef struct Trace__##T                               \
    {                                                       \
        const T **v; /* const T** (&) */                    \
    } Trace__##T;                                           \
                                                            \
    /**                                                     \
     *                                                      \
     * @brief Construct Trace type.                         \
     */                                                     \
    inline CONSTRUCTOR(Trace__##T, Trace__##T, const T **v) \
    {                                                       \
        ASSERT(v);                                          \
        return (Trace__##T){ .v = v };                      \
    }                                                       \
                                                            \
    /**                                                     \
     *                                                      \
     * @brief Get the traced pointer.                       \
     */                                                     \
    inline const T *get__Trace__##T(Ptr(Trace__##T) self)   \
    {                                                       \
        ASSERT(self);                                       \
        return *self->v;                                    \
    }                                                       \
                                                            \
    /**                                                     \
     *                                                      \
     * @brief Implement Eq trait.                           \
     */                                                     \
    inline IMPL_FOR_EQ(                                     \
      Ptr(Trace__##T), Trace__##T, return EQ(T, **self->v, **other->v););

#define MAKE_TRACE(T, v) NEW(Trace__##T, v)

Trace(Int8);
Trace(Int16);
Trace(Int32);
Trace(Int64);

Trace(Uint8);
Trace(Uint16);
Trace(Uint32);
Trace(Uint64);

Trace(Isize);
Trace(Usize);

Trace(Float32);
Trace(Float64);

Trace(Uptr);

Trace(Bool);
Trace(Char);
Trace(Short);
Trace(Int);
Trace(Long);
Trace(Longlong);

Trace(Uchar);
Trace(Ushort);
Trace(Uint);
Trace(Ulong);
Trace(Ulonglong);

#endif // LILY_BASE_TRACE_H
