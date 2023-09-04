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

#define RefMut(T)                                               \
    /* RefMut<T> */                                             \
    typedef struct RefMut__##T                                  \
    {                                                           \
        PtrMut(T) v; /* T* (&) */                               \
    } RefMut__##T;                                              \
    /**                                                         \
     *                                                          \
     * @brief Construct RefMut type.                            \
     */                                                         \
    inline CONSTRUCTOR(RefMut__##T, RefMut__##T, PtrMut(T) v)   \
    {                                                           \
        ASSERT(v);                                              \
        return (RefMut__##T){ .v = v };                         \
    }                                                           \
                                                                \
    /**                                                         \
     *                                                          \
     * @brief Unwrap the pointer of the reference.              \
     */                                                         \
    inline PtrMut(T) unwrap__RefMut__##T(Ptr(RefMut__##T) self) \
    {                                                           \
        ASSERT(self);                                           \
        return self->v;                                         \
    }                                                           \
                                                                \
    /**                                                         \
     *                                                          \
     * @brief Implement Eq trait.                               \
     */                                                         \
    inline IMPL_FOR_EQ(                                         \
      Ptr(RefMut__##T), RefMut__##T, return EQ(T, *self->v, *other->v););

#define MAKE_REF_MUT(T, v) NEW(RefMut__##T, v)

RefMut(Int8);
RefMut(Int16);
RefMut(Int32);
RefMut(Int64);

RefMut(Uint8);
RefMut(Uint16);
RefMut(Uint32);
RefMut(Uint64);

RefMut(Isize);
RefMut(Usize);

RefMut(Float32);
RefMut(Float64);

RefMut(Uptr);

RefMut(Bool);
RefMut(Char);
RefMut(Short);
RefMut(Int);
RefMut(Long);
RefMut(Longlong);

RefMut(Uchar);
RefMut(Ushort);
RefMut(Uint);
RefMut(Ulong);
RefMut(Ulonglong);

#endif // LILY_BASE_REF_MUT_H
