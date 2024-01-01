/*
 * MIT License
 *
 * Copyright (c) 2022-2024 ArthurPV
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

#ifndef LILY_BASE_NEW_H
#define LILY_BASE_NEW_H

#include <base/macros.h>
#include <base/types.h>

#define NEW(name, ...) __new__##name(__VA_ARGS__)
#define NEW_VARIANT(name, variant, ...) __new__##name##__##variant(__VA_ARGS__)
#define FREE(name, ...) __free__##name(__VA_ARGS__)
#define FREE_VARIANT(name, variant, ...) \
    __free__##name##__##variant(__VA_ARGS__)

inline DESTRUCTOR(Int8, Int8) {}
inline DESTRUCTOR(Int16, Int16) {}
inline DESTRUCTOR(Int32, Int32) {}
inline DESTRUCTOR(Int64, Int64) {}

inline DESTRUCTOR(Uint8, Uint8) {}
inline DESTRUCTOR(Uint16, Uint16) {}
inline DESTRUCTOR(Uint32, Uint32) {}
inline DESTRUCTOR(Uint64, Uint64) {}

inline DESTRUCTOR(Isize, Isize) {}
inline DESTRUCTOR(Usize, Usize) {}

inline DESTRUCTOR(Float32, Float32) {}
inline DESTRUCTOR(Float64, Float64) {}

inline DESTRUCTOR(Uptr, Uptr) {}

inline DESTRUCTOR(Bool, Bool) {}
inline DESTRUCTOR(Char, Char) {}
inline DESTRUCTOR(Short, Short) {}
inline DESTRUCTOR(Int, Int) {}
inline DESTRUCTOR(Long, Long) {}
inline DESTRUCTOR(Longlong, Longlong) {}

inline DESTRUCTOR(Uchar, Uchar) {}
inline DESTRUCTOR(Ushort, Ushort) {}
inline DESTRUCTOR(Uint, Uint) {}
inline DESTRUCTOR(Ulong, Ulong) {}
inline DESTRUCTOR(Ulonglong, Ulonglong) {}

#endif // LILY_BASE_NEW_H
