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

#ifndef LILY_BASE_EQ_H
#define LILY_BASE_EQ_H

#include <base/bool.h>
#include <base/macros.h>
#include <base/types.h>

#define IMPL_FOR_EQ(T, struct_name, body)                       \
    IMPL_FOR(bool, eq, Eq, struct_name, T self, T other)        \
    {                                                           \
        body;                                                   \
    }                                                           \
    inline IMPL_FOR(bool, ne, Eq, struct_name, T self, T other) \
    {                                                           \
        return !CALL_IMPL(eq, Eq, struct_name, self, other);    \
    }

#define EQ(struct_name, ...) CALL_IMPL(eq, Eq, struct_name, __VA_ARGS__)
#define NE(struct_name, ...) CALL_IMPL(ne, Eq, struct_name, __VA_ARGS__)

inline IMPL_FOR_EQ(Int8, Int8, return self == other;);
inline IMPL_FOR_EQ(Int16, Int16, return self == other;);
inline IMPL_FOR_EQ(Int32, Int32, return self == other;);
inline IMPL_FOR_EQ(Int64, Int64, return self == other;);
inline IMPL_FOR_EQ(Isize, Isize, return self == other;);

inline IMPL_FOR_EQ(Uint8, Uint8, return self == other;);
inline IMPL_FOR_EQ(Uint16, Uint16, return self == other;);
inline IMPL_FOR_EQ(Uint32, Uint32, return self == other;);
inline IMPL_FOR_EQ(Uint64, Uint64, return self == other;);
inline IMPL_FOR_EQ(Usize, Usize, return self == other;);

inline IMPL_FOR_EQ(Float32, Float32, return self == other;);
inline IMPL_FOR_EQ(Float64, Float64, return self == other;);

inline IMPL_FOR_EQ(Char, Char, return self == other;);
inline IMPL_FOR_EQ(Short, Short, return self == other;);
inline IMPL_FOR_EQ(Int, Int, return self == other;);
inline IMPL_FOR_EQ(Long, Long, return self == other;);
inline IMPL_FOR_EQ(Longlong, Longlong, return self == other;);

inline IMPL_FOR_EQ(Uchar, Uchar, return self == other;);
inline IMPL_FOR_EQ(Ushort, Ushort, return self == other;);
inline IMPL_FOR_EQ(Uint, Uint, return self == other;);
inline IMPL_FOR_EQ(Ulong, Ulong, return self == other;);
inline IMPL_FOR_EQ(Ulonglong, Ulonglong, return self == other;);

inline IMPL_FOR_EQ(Uptr, Uptr, return self == other;);

#endif // LILY_BASE_EQ_H
