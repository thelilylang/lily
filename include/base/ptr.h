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
#define Ptr(T) Ptr__##T

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
inline DEF_FOR_EQ(Ptr(Int8), Ptr(Int8));
inline IMPL_FOR_EQ(Ptr(Int8), Ptr(Int8), return *self == *other;);

inline DEF_FOR_EQ(Ptr(Int16), Ptr(Int16));
inline IMPL_FOR_EQ(Ptr(Int16), Ptr(Int16), return *self == *other;);

inline DEF_FOR_EQ(Ptr(Int32), Ptr(Int32));
inline IMPL_FOR_EQ(Ptr(Int32), Ptr(Int32), return *self == *other;);

inline DEF_FOR_EQ(Ptr(Int64), Ptr(Int64));
inline IMPL_FOR_EQ(Ptr(Int64), Ptr(Int64), return *self == *other;);

inline DEF_FOR_EQ(Ptr(Isize), Ptr(Isize));
inline IMPL_FOR_EQ(Ptr(Isize), Ptr(Isize), return *self == *other;);

inline DEF_FOR_EQ(Ptr(Uint8), Ptr(Uint8));
inline IMPL_FOR_EQ(Ptr(Uint8), Ptr(Uint8), return *self == *other;);

inline DEF_FOR_EQ(Ptr(Uint16), Ptr(Uint16));
inline IMPL_FOR_EQ(Ptr(Uint16), Ptr(Uint16), return *self == *other;);

inline DEF_FOR_EQ(Ptr(Uint32), Ptr(Uint32));
inline IMPL_FOR_EQ(Ptr(Uint32), Ptr(Uint32), return *self == *other;);

inline DEF_FOR_EQ(Ptr(Uint64), Ptr(Uint64));
inline IMPL_FOR_EQ(Ptr(Uint64), Ptr(Uint64), return *self == *other;);

inline DEF_FOR_EQ(Ptr(Usize), Ptr(Usize));
inline IMPL_FOR_EQ(Ptr(Usize), Ptr(Usize), return *self == *other;);

inline DEF_FOR_EQ(Ptr(Float32), Ptr(Float32));
inline IMPL_FOR_EQ(Ptr(Float32), Ptr(Float32), return *self == *other;);

inline DEF_FOR_EQ(Ptr(Float64), Ptr(Float64));
inline IMPL_FOR_EQ(Ptr(Float64), Ptr(Float64), return *self == *other;);

inline DEF_FOR_EQ(Ptr(Bool), Ptr(Bool));
inline IMPL_FOR_EQ(Ptr(Bool), Ptr(Bool), return *self == *other;);

inline DEF_FOR_EQ(Ptr(Char), Ptr(Char));
inline IMPL_FOR_EQ(Ptr(Char), Ptr(Char), return *self == *other;);

inline DEF_FOR_EQ(Ptr(Short), Ptr(Short));
inline IMPL_FOR_EQ(Ptr(Short), Ptr(Short), return *self == *other;);

inline DEF_FOR_EQ(Ptr(Int), Ptr(Int));
inline IMPL_FOR_EQ(Ptr(Int), Ptr(Int), return *self == *other;);

inline DEF_FOR_EQ(Ptr(Long), Ptr(Long));
inline IMPL_FOR_EQ(Ptr(Long), Ptr(Long), return *self == *other;);

inline DEF_FOR_EQ(Ptr(Longlong), Ptr(Longlong));
inline IMPL_FOR_EQ(Ptr(Longlong), Ptr(Longlong), return *self == *other;);

inline DEF_FOR_EQ(Ptr(Uchar), Ptr(Uchar));
inline IMPL_FOR_EQ(Ptr(Uchar), Ptr(Uchar), return *self == *other;);

inline DEF_FOR_EQ(Ptr(Ushort), Ptr(Ushort));
inline IMPL_FOR_EQ(Ptr(Ushort), Ptr(Ushort), return *self == *other;);

inline DEF_FOR_EQ(Ptr(Uint), Ptr(Uint));
inline IMPL_FOR_EQ(Ptr(Uint), Ptr(Uint), return *self == *other;);

inline DEF_FOR_EQ(Ptr(Ulong), Ptr(Ulong));
inline IMPL_FOR_EQ(Ptr(Ulong), Ptr(Ulong), return *self == *other;);

inline DEF_FOR_EQ(Ptr(Ulonglong), Ptr(Ulonglong));
inline IMPL_FOR_EQ(Ptr(Ulonglong), Ptr(Ulonglong), return *self == *other;);

inline DEF_FOR_EQ(Ptr(Uptr), Ptr(Uptr));
inline IMPL_FOR_EQ(Ptr(Uptr), Ptr(Uptr), return *self == *other;);

#endif // LILY_BASE_PTR_H
