/*
 * MIT License
 *
 * Copyright (c) 2022-2026 ArthurPV
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

#ifndef LILY_EX_LIB_LILY_BUILTIN_C
#define LILY_EX_LIB_LILY_BUILTIN_C

#include <builtin/math.h>

// <builtin/math.h>
extern Int8
__max__$Int8(Int8 x, Int8 y) __attribute__((always_inline));

extern Int16
__max__$Int16(Int16 x, Int16 y) __attribute__((always_inline));

extern Int32
__max__$Int32(Int32 x, Int32 y) __attribute__((always_inline));

extern Int64
__max__$Int64(Int64 x, Int64 y) __attribute__((always_inline));

extern Isize
__max__$Isize(Isize x, Isize y) __attribute__((always_inline));

extern Uint8
__max__$Uint8(Uint8 x, Uint8 y) __attribute__((always_inline));

extern Uint16
__max__$Uint16(Uint16 x, Uint16 y) __attribute__((always_inline));

extern Uint32
__max__$Uint32(Uint32 x, Uint32 y) __attribute__((always_inline));

extern Uint64
__max__$Uint64(Uint64 x, Uint64 y) __attribute__((always_inline));

extern Usize
__max__$Usize(Usize x, Usize y) __attribute__((always_inline));

extern Float32
__max__$Float32(Float32 x, Float32 y) __attribute__((always_inline));

extern Float64
__max__$Float64(Float64 x, Float64 y) __attribute__((always_inline));

extern Int8
__min__$Int8(Int8 x, Int8 y) __attribute__((always_inline));

extern Int16
__min__$Int16(Int16 x, Int16 y) __attribute__((always_inline));

extern Int32
__min__$Int32(Int32 x, Int32 y) __attribute__((always_inline));

extern Int64
__min__$Int64(Int64 x, Int64 y) __attribute__((always_inline));

extern Uint8
__min__$Uint8(Uint8 x, Uint8 y) __attribute__((always_inline));

extern Uint16
__min__$Uint16(Uint16 x, Uint16 y) __attribute__((always_inline));

extern Uint32
__min__$Uint32(Uint32 x, Uint32 y) __attribute__((always_inline));

extern Uint64
__min__$Uint64(Uint64 x, Uint64 y) __attribute__((always_inline));

extern Float32
__min__$Float32(Float32 x, Float32 y) __attribute__((always_inline));

extern Float64
__min__$Float64(Float64 x, Float64 y) __attribute__((always_inline));

#endif // LILY_EX_LIB_LILY_BUILTIN_C