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

#ifndef LILY_BASE_REF_H
#define LILY_BASE_REF_H

#include <base/assert.h>
#include <base/eq.h>
#include <base/macros.h>
#include <base/ptr.h>
#include <base/types.h>

#include <stdio.h>
#include <stdlib.h>

#define Ref(T)                                         \
    /* Ref<T> */                                       \
    typedef struct Ref__##T                            \
    {                                                  \
        Ptr(T) v; /* const T* (&) */                   \
    } Ref__##T;                                        \
    /**                                                \
     *                                                 \
     * @brief Construct Ref type.                      \
     */                                                \
    inline CONSTRUCTOR(Ref__##T, Ref__##T, Ptr(T) v)   \
    {                                                  \
        ASSERT(v);                                     \
        return (Ref__##T){ .v = v };                   \
    }                                                  \
                                                       \
    /**                                                \
     *                                                 \
     * @brief Unwrap the pointer of the reference.     \
     */                                                \
    inline Ptr(T) unwrap__Ref__##T(Ptr(Ref__##T) self) \
    {                                                  \
        ASSERT(self);                                  \
        return self->v;                                \
    }                                                  \
                                                       \
    /**                                                \
     *                                                 \
     * @brief Implement Eq trait.                      \
     */                                                \
    inline IMPL_FOR_EQ(                                \
      Ptr(Ref__##T), Ref__##T, return EQ(T, *self->v, *other->v););

#define MAKE_REF(T, v) NEW(Ref__##T, v)

Ref(Int8);
Ref(Int16);
Ref(Int32);
Ref(Int64);

Ref(Uint8);
Ref(Uint16);
Ref(Uint32);
Ref(Uint64);

Ref(Isize);
Ref(Usize);

Ref(Float32);
Ref(Float64);

Ref(Uptr);

Ref(Bool);
Ref(Char);
Ref(Short);
Ref(Int);
Ref(Long);
Ref(Longlong);

Ref(Uchar);
Ref(Ushort);
Ref(Uint);
Ref(Ulong);
Ref(Ulonglong);

#endif // LILY_BASE_REF_H
