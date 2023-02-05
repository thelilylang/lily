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

#ifndef LILY_BASE_TYPES_H
#define LILY_BASE_TYPES_H

#include <stdint.h>

#if __clang_major__ > 15
typedef _BitInt(8) Int8;
typedef _BitInt(16) Int16;
typedef _BitInt(32) Int32;
typedef _BitInt(64) Int64;

typedef unsigned _BitInt(8) Uint8;
typedef unsigned _BitInt(16) Uint16;
typedef unsigned _BitInt(32) Uint32;
typedef unsigned _BitInt(64) Uint64;

#if defined(__x86_64__) || defined(_WIN64)
typedef _BitInt(64) Isize;
typedef unsigned _BitInt(64) Usize;
#else
typedef _BitInt(32) Isize;
typedef unsigned _BitInt(32) Usize;
#endif

#else
typedef int8_t Int8;
typedef int16_t Int16;
typedef int32_t Int32;
typedef int64_t Int64;

typedef uint8_t Uint8;
typedef uint16_t Uint16;
typedef uint32_t Uint32;
typedef uint64_t Uint64;

#if defined(__x86_64__) || defined(_WIN64)
typedef int64_t Isize;
typedef uint64_t Usize;
#else
typedef int32_t Isize;
typedef uint32_t Usize;
#endif

#endif

typedef float Float32;
typedef double Float64;

#endif // LILY_BASE_TYPES_H
