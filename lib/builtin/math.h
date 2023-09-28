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

#ifndef LILY_BUILTIN_MATH_H
#define LILY_BUILTIN_MATH_H

#include <base/types.h>

#include <api.h>

#if defined(__cplusplus)
extern "C"
{
#endif

    /**
     *
     * @brief Return the maximum value.
     */
    LILY_API inline Int8 _max__$Int8(Int8 x, Int8 y)
      __attribute__((always_inline));

    LILY_API inline Int16 _max__$Int16(Int16 x, Int16 y)
      __attribute__((always_inline));

    LILY_API inline Int32 _max__$Int32(Int32 x, Int32 y)
      __attribute__((always_inline));

    LILY_API inline Int64 _max__$Int64(Int64 x, Int64 y)
      __attribute__((always_inline));

    LILY_API inline Isize _max__$Isize(Isize x, Isize y)
      __attribute__((always_inline));

    LILY_API inline Uint8 _max__$Uint8(Uint8 x, Uint8 y)
      __attribute__((always_inline));

    LILY_API inline Uint16 _max__$Uint16(Uint16 x, Uint16 y)
      __attribute__((always_inline));

    LILY_API inline Uint32 _max__$Uint32(Uint32 x, Uint32 y)
      __attribute__((always_inline));

    LILY_API inline Uint64 _max__$Uint64(Uint64 x, Uint64 y)
      __attribute__((always_inline));

    LILY_API inline Usize _max__$Usize(Usize x, Usize y)
      __attribute__((always_inline));

    LILY_API inline Float32 _max__$Float32(Float32 x, Float32 y)
      __attribute__((always_inline));

    LILY_API inline Float64 _max__$Float64(Float64 x, Float64 y)
      __attribute__((always_inline));

    /**
     *
     * @brief Return the minimum value.
     */
    LILY_API inline Int8 _min__$Int8(Int8 x, Int8 y)
      __attribute__((always_inline));

    LILY_API inline Int16 _min__$Int16(Int16 x, Int16 y)
      __attribute__((always_inline));

    LILY_API inline Int32 _min__$Int32(Int32 x, Int32 y)
      __attribute__((always_inline));

    LILY_API inline Int64 _min__$Int64(Int64 x, Int64 y)
      __attribute__((always_inline));

    LILY_API inline Uint8 _min__$Uint8(Uint8 x, Uint8 y)
      __attribute__((always_inline));

    LILY_API inline Uint16 _min__$Uint16(Uint16 x, Uint16 y)
      __attribute__((always_inline));

    LILY_API inline Uint32 _min__$Uint32(Uint32 x, Uint32 y)
      __attribute__((always_inline));

    LILY_API inline Uint64 _min__$Uint64(Uint64 x, Uint64 y)
      __attribute__((always_inline));

    LILY_API inline Float32 _min__$Float32(Float32 x, Float32 y)
      __attribute__((always_inline));

    LILY_API inline Float64 _min__$Float64(Float64 x, Float64 y)
      __attribute__((always_inline));

#if defined(__cplusplus)
}
#endif

#endif // LILY_BUILTIN_MATH_H
