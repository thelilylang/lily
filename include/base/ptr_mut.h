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
#define PtrMut(T) PtrMut__##T

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
inline DEF_FOR_EQ(PtrMut(Int8), PtrMut(Int8));
inline IMPL_FOR_EQ(PtrMut(Int8), PtrMut(Int8), return *self == *other;);

inline DEF_FOR_EQ(PtrMut(Int16), PtrMut(Int16));
inline IMPL_FOR_EQ(PtrMut(Int16), PtrMut(Int16), return *self == *other;);

inline DEF_FOR_EQ(PtrMut(Int32), PtrMut(Int32));
inline IMPL_FOR_EQ(PtrMut(Int32), PtrMut(Int32), return *self == *other;);

inline DEF_FOR_EQ(PtrMut(Int64), PtrMut(Int64));
inline IMPL_FOR_EQ(PtrMut(Int64), PtrMut(Int64), return *self == *other;);

inline DEF_FOR_EQ(PtrMut(Isize), PtrMut(Isize));
inline IMPL_FOR_EQ(PtrMut(Isize), PtrMut(Isize), return *self == *other;);

inline DEF_FOR_EQ(PtrMut(Uint8), PtrMut(Uint8));
inline IMPL_FOR_EQ(PtrMut(Uint8), PtrMut(Uint8), return *self == *other;);

inline DEF_FOR_EQ(PtrMut(Uint16), PtrMut(Uint16));
inline IMPL_FOR_EQ(PtrMut(Uint16), PtrMut(Uint16), return *self == *other;);

inline DEF_FOR_EQ(PtrMut(Uint32), PtrMut(Uint32));
inline IMPL_FOR_EQ(PtrMut(Uint32), PtrMut(Uint32), return *self == *other;);

inline DEF_FOR_EQ(PtrMut(Uint64), PtrMut(Uint64));
inline IMPL_FOR_EQ(PtrMut(Uint64), PtrMut(Uint64), return *self == *other;);

inline DEF_FOR_EQ(PtrMut(Usize), PtrMut(Usize));
inline IMPL_FOR_EQ(PtrMut(Usize), PtrMut(Usize), return *self == *other;);

inline DEF_FOR_EQ(PtrMut(Float32), PtrMut(Float32));
inline IMPL_FOR_EQ(PtrMut(Float32), PtrMut(Float32), return *self == *other;);

inline DEF_FOR_EQ(PtrMut(Float64), PtrMut(Float64));
inline IMPL_FOR_EQ(PtrMut(Float64), PtrMut(Float64), return *self == *other;);

inline DEF_FOR_EQ(PtrMut(Bool), PtrMut(Bool));
inline IMPL_FOR_EQ(PtrMut(Bool), PtrMut(Bool), return *self == *other;);

inline DEF_FOR_EQ(PtrMut(Char), PtrMut(Char));
inline IMPL_FOR_EQ(PtrMut(Char), PtrMut(Char), return *self == *other;);

inline DEF_FOR_EQ(PtrMut(Short), PtrMut(Short));
inline IMPL_FOR_EQ(PtrMut(Short), PtrMut(Short), return *self == *other;);

inline DEF_FOR_EQ(PtrMut(Int), PtrMut(Int));
inline IMPL_FOR_EQ(PtrMut(Int), PtrMut(Int), return *self == *other;);

inline DEF_FOR_EQ(PtrMut(Long), PtrMut(Long));
inline IMPL_FOR_EQ(PtrMut(Long), PtrMut(Long), return *self == *other;);

inline DEF_FOR_EQ(PtrMut(Longlong), PtrMut(Longlong));
inline IMPL_FOR_EQ(PtrMut(Longlong), PtrMut(Longlong), return *self == *other;);

inline DEF_FOR_EQ(PtrMut(Uchar), PtrMut(Uchar));
inline IMPL_FOR_EQ(PtrMut(Uchar), PtrMut(Uchar), return *self == *other;);

inline DEF_FOR_EQ(PtrMut(Ushort), PtrMut(Ushort));
inline IMPL_FOR_EQ(PtrMut(Ushort), PtrMut(Ushort), return *self == *other;);

inline DEF_FOR_EQ(PtrMut(Uint), PtrMut(Uint));
inline IMPL_FOR_EQ(PtrMut(Uint), PtrMut(Uint), return *self == *other;);

inline DEF_FOR_EQ(PtrMut(Ulong), PtrMut(Ulong));
inline IMPL_FOR_EQ(PtrMut(Ulong), PtrMut(Ulong), return *self == *other;);

inline DEF_FOR_EQ(PtrMut(Ulonglong), PtrMut(Ulonglong));
inline IMPL_FOR_EQ(PtrMut(Ulonglong),
                   PtrMut(Ulonglong),
                   return *self == *other;);

inline DEF_FOR_EQ(PtrMut(Uptr), PtrMut(Uptr));
inline IMPL_FOR_EQ(PtrMut(Uptr), PtrMut(Uptr), return *self == *other;);

#endif // LILY_BASE_PTR_MUT_H
