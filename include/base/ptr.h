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

#ifndef LILY_BASE_PTR_H
#define LILY_BASE_PTR_H

#include <base/eq.h>
#include <base/types.h>

/* Ptr<T> */
#define Ptr(T) const T *

#define DEF_PTR(T) typedef const T *Ptr__##T
#define AS_PTR_MUT(T, r) ((T *)r)

DEF_PTR(Int8);
DEF_PTR(Int16);
DEF_PTR(Int32);
DEF_PTR(Int64);

DEF_PTR(Uint8);
DEF_PTR(Uint16);
DEF_PTR(Uint32);
DEF_PTR(Uint64);

DEF_PTR(Isize);
DEF_PTR(Usize);

DEF_PTR(Float32);
DEF_PTR(Float64);

DEF_PTR(Uptr);

DEF_PTR(Bool);
DEF_PTR(Char);
DEF_PTR(Short);
DEF_PTR(Int);
DEF_PTR(Long);
DEF_PTR(Longlong);

DEF_PTR(Uchar);
DEF_PTR(Ushort);
DEF_PTR(Uint);
DEF_PTR(Ulong);
DEF_PTR(Ulonglong);

// Impl Eq
inline IMPL_FOR_EQ(Ptr__Int8, Ptr__Int8, return *self == *other;);
inline IMPL_FOR_EQ(Ptr__Int16, Ptr__Int16, return *self == *other;);
inline IMPL_FOR_EQ(Ptr__Int32, Ptr__Int32, return *self == *other;);
inline IMPL_FOR_EQ(Ptr__Int64, Ptr__Int64, return *self == *other;);
inline IMPL_FOR_EQ(Ptr__Isize, Ptr__Isize, return *self == *other;);

inline IMPL_FOR_EQ(Ptr__Uint8, Ptr__Uint8, return *self == *other;);
inline IMPL_FOR_EQ(Ptr__Uint16, Ptr__Uint16, return *self == *other;);
inline IMPL_FOR_EQ(Ptr__Uint32, Ptr__Uint32, return *self == *other;);
inline IMPL_FOR_EQ(Ptr__Uint64, Ptr__Uint64, return *self == *other;);
inline IMPL_FOR_EQ(Ptr__Usize, Ptr__Usize, return *self == *other;);

inline IMPL_FOR_EQ(Ptr__Float32, Ptr__Float32, return *self == *other;);
inline IMPL_FOR_EQ(Ptr__Float64, Ptr__Float64, return *self == *other;);

inline IMPL_FOR_EQ(Ptr__Bool, Ptr__Bool, return *self == *other;);
inline IMPL_FOR_EQ(Ptr__Char, Ptr__Char, return *self == *other;);
inline IMPL_FOR_EQ(Ptr__Short, Ptr__Short, return *self == *other;);
inline IMPL_FOR_EQ(Ptr__Int, Ptr__Int, return *self == *other;);
inline IMPL_FOR_EQ(Ptr__Long, Ptr__Long, return *self == *other;);
inline IMPL_FOR_EQ(Ptr__Longlong, Ptr__Longlong, return *self == *other;);

inline IMPL_FOR_EQ(Ptr__Uchar, Ptr__Uchar, return *self == *other;);
inline IMPL_FOR_EQ(Ptr__Ushort, Ptr__Ushort, return *self == *other;);
inline IMPL_FOR_EQ(Ptr__Uint, Ptr__Uint, return *self == *other;);
inline IMPL_FOR_EQ(Ptr__Ulong, Ptr__Ulong, return *self == *other;);
inline IMPL_FOR_EQ(Ptr__Ulonglong, Ptr__Ulonglong, return *self == *other;);

inline IMPL_FOR_EQ(Ptr__Uptr, Ptr__Uptr, return *self == *other;);

#endif // LILY_BASE_PTR_H
