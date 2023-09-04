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

#ifndef LILY_BASE_PTR_MUT_H
#define LILY_BASE_PTR_MUT_H

#include <base/eq.h>
#include <base/types.h>

/* PtrMut<T> */
#define PtrMut(T) T *

#define DEF_PTR_MUT(T) typedef T *PtrMut__##T
#define AS_PTR(T, r) ((const T *)r)

DEF_PTR_MUT(Int8);
DEF_PTR_MUT(Int16);
DEF_PTR_MUT(Int32);
DEF_PTR_MUT(Int64);

DEF_PTR_MUT(Uint8);
DEF_PTR_MUT(Uint16);
DEF_PTR_MUT(Uint32);
DEF_PTR_MUT(Uint64);

DEF_PTR_MUT(Isize);
DEF_PTR_MUT(Usize);

DEF_PTR_MUT(Float32);
DEF_PTR_MUT(Float64);

DEF_PTR_MUT(Uptr);

DEF_PTR_MUT(Bool);
DEF_PTR_MUT(Char);
DEF_PTR_MUT(Short);
DEF_PTR_MUT(Int);
DEF_PTR_MUT(Long);
DEF_PTR_MUT(Longlong);

DEF_PTR_MUT(Uchar);
DEF_PTR_MUT(Ushort);
DEF_PTR_MUT(Uint);
DEF_PTR_MUT(Ulong);
DEF_PTR_MUT(Ulonglong);

// Impl Eq
inline IMPL_FOR_EQ(PtrMut__Int8, PtrMut__Int8, return *self == *other;);
inline IMPL_FOR_EQ(PtrMut__Int16, PtrMut__Int16, return *self == *other;);
inline IMPL_FOR_EQ(PtrMut__Int32, PtrMut__Int32, return *self == *other;);
inline IMPL_FOR_EQ(PtrMut__Int64, PtrMut__Int64, return *self == *other;);
inline IMPL_FOR_EQ(PtrMut__Isize, PtrMut__Isize, return *self == *other;);

inline IMPL_FOR_EQ(PtrMut__Uint8, PtrMut__Uint8, return *self == *other;);
inline IMPL_FOR_EQ(PtrMut__Uint16, PtrMut__Uint16, return *self == *other;);
inline IMPL_FOR_EQ(PtrMut__Uint32, PtrMut__Uint32, return *self == *other;);
inline IMPL_FOR_EQ(PtrMut__Uint64, PtrMut__Uint64, return *self == *other;);
inline IMPL_FOR_EQ(PtrMut__Usize, PtrMut__Usize, return *self == *other;);

inline IMPL_FOR_EQ(PtrMut__Float32, PtrMut__Float32, return *self == *other;);
inline IMPL_FOR_EQ(PtrMut__Float64, PtrMut__Float64, return *self == *other;);

inline IMPL_FOR_EQ(PtrMut__Bool, PtrMut__Bool, return *self == *other;);
inline IMPL_FOR_EQ(PtrMut__Char, PtrMut__Char, return *self == *other;);
inline IMPL_FOR_EQ(PtrMut__Short, PtrMut__Short, return *self == *other;);
inline IMPL_FOR_EQ(PtrMut__Int, PtrMut__Int, return *self == *other;);
inline IMPL_FOR_EQ(PtrMut__Long, PtrMut__Long, return *self == *other;);
inline IMPL_FOR_EQ(PtrMut__Longlong, PtrMut__Longlong, return *self == *other;);

inline IMPL_FOR_EQ(PtrMut__Uchar, PtrMut__Uchar, return *self == *other;);
inline IMPL_FOR_EQ(PtrMut__Ushort, PtrMut__Ushort, return *self == *other;);
inline IMPL_FOR_EQ(PtrMut__Uint, PtrMut__Uint, return *self == *other;);
inline IMPL_FOR_EQ(PtrMut__Ulong, PtrMut__Ulong, return *self == *other;);
inline IMPL_FOR_EQ(PtrMut__Ulonglong,
                   PtrMut__Ulonglong,
                   return *self == *other;);

inline IMPL_FOR_EQ(PtrMut__Uptr, PtrMut__Uptr, return *self == *other;);

#endif // LILY_BASE_PTR_MUT_H
