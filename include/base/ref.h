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

// NOTE: You cannot free a Ref
#define Ref(T) Ref__##T

#define DEF_REF(T)                              \
    /* Ref<T> */                                \
    typedef const T *const Ref(T);              \
                                                \
    /**                                         \
     *                                          \
     * @brief Check if the pointer is not NULL. \
     */                                         \
    inline Ref(T) make__Ref__##T(Ref(T) v)      \
    {                                           \
        ASSERT(v);                              \
        return v;                               \
    }                                           \
                                                \
    /**                                         \
     *                                          \
     * @brief Implement Eq trait.               \
     */                                         \
    inline IMPL_FOR_EQ(Ref(T), Ref(T), return EQ(T, *self, *other););

#define MAKE_REF(T, v) make__Ref__##T(v)

DEF_REF(Int8);
DEF_REF(Int16);
DEF_REF(Int32);
DEF_REF(Int64);

DEF_REF(Uint8);
DEF_REF(Uint16);
DEF_REF(Uint32);
DEF_REF(Uint64);

DEF_REF(Isize);
DEF_REF(Usize);

DEF_REF(Float32);
DEF_REF(Float64);

DEF_REF(Uptr);

DEF_REF(Bool);
DEF_REF(Char);
DEF_REF(Short);
DEF_REF(Int);
DEF_REF(Long);
DEF_REF(Longlong);

DEF_REF(Uchar);
DEF_REF(Ushort);
DEF_REF(Uint);
DEF_REF(Ulong);
DEF_REF(Ulonglong);

#endif // LILY_BASE_REF_H
