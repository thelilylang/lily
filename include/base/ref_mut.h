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

#ifndef LILY_BASE_REF_MUT_H
#define LILY_BASE_REF_MUT_H

#include <base/assert.h>
#include <base/eq.h>
#include <base/macros.h>
#include <base/ptr.h>
#include <base/ptr_mut.h>
#include <base/types.h>

#include <stdio.h>
#include <stdlib.h>

// NOTE: You cannot free a RefMutMut
#define RefMut(T) RefMut__##T

#define DEF_REF_MUT(T)                              \
    /* RefMut<T> */                                 \
    typedef T *const RefMut(T);                     \
                                                    \
    /**                                             \
     *                                              \
     * @brief Check if the pointer is not NULL.     \
     */                                             \
    inline RefMut(T) make__RefMut__##T(RefMut(T) v) \
    {                                               \
        ASSERT(v);                                  \
        return v;                                   \
    }                                               \
                                                    \
    /**                                             \
     *                                              \
     * @brief Implement Eq trait.                   \
     */                                             \
    inline DEF_FOR_EQ(RefMut(T), RefMut(T));        \
    inline IMPL_FOR_EQ(RefMut(T), RefMut(T), return EQ(T, *self, *other););

#define MAKE_REF_MUT(T, v) make__RefMut__##T(v)

DEF_REF_MUT(Int8);
DEF_REF_MUT(Int16);
DEF_REF_MUT(Int32);
DEF_REF_MUT(Int64);

DEF_REF_MUT(Uint8);
DEF_REF_MUT(Uint16);
DEF_REF_MUT(Uint32);
DEF_REF_MUT(Uint64);

DEF_REF_MUT(Isize);
DEF_REF_MUT(Usize);

DEF_REF_MUT(Float32);
DEF_REF_MUT(Float64);

DEF_REF_MUT(Uptr);

DEF_REF_MUT(Bool);
DEF_REF_MUT(Char);
DEF_REF_MUT(Short);
DEF_REF_MUT(Int);
DEF_REF_MUT(Long);
DEF_REF_MUT(Longlong);

DEF_REF_MUT(Uchar);
DEF_REF_MUT(Ushort);
DEF_REF_MUT(Uint);
DEF_REF_MUT(Ulong);
DEF_REF_MUT(Ulonglong);

#endif // LILY_BASE_REF_MUT_H
