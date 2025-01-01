/*
 * MIT License
 *
 * Copyright (c) 2022-2025 ArthurPV
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

#ifndef LILY_CORE_LILY_INTERPRETER_RUNTIME_OPERATOR_H
#define LILY_CORE_LILY_INTERPRETER_RUNTIME_OPERATOR_H

#include <base/types.h>

#include <math.h>

/**
 *
 * @brief Add two Int8 integers and check whether the operation is an overflow.
 * If the operation overflows, execution is crash (this function is only used in
 * debug mode, not in release mode).
 */
Int8
add_with_overflow__Int8(Int8 lhs, Int8 rhs);

/**
 *
 * @brief Add two Int16 integers and check whether the operation is an overflow.
 * If the operation overflows, execution is crash (this function is only used in
 * debug mode, not in release mode).
 */
Int16
add_with_overflow__Int16(Int16 lhs, Int16 rhs);

/**
 *
 * @brief Add two Int32 integers and check whether the operation is an overflow.
 * If the operation overflows, execution is crash (this function is only used in
 * debug mode, not in release mode).
 */
Int32
add_with_overflow__Int32(Int32 lhs, Int32 rhs);

/**
 *
 * @brief Add two Int64 integers and check whether the operation is an overflow.
 * If the operation overflows, execution is crash (this function is only used in
 * debug mode, not in release mode).
 */
Int64
add_with_overflow__Int64(Int64 lhs, Int64 rhs);

/**
 *
 * @brief Add two Isize integers and check whether the operation is an overflow.
 * If the operation overflows, execution is crash (this function is only used in
 * debug mode, not in release mode).
 */
Isize
add_with_overflow__Isize(Isize lhs, Isize rhs);

/**
 *
 * @brief Add two Uint8 integers and check whether the operation is an overflow.
 * If the operation overflows, execution is crash (this function is only used in
 * debug mode, not in release mode).
 */
Uint8
add_with_overflow__Uint8(Uint8 lhs, Uint8 rhs);

/**
 *
 * @brief Add two Uint16 integers and check whether the operation is an
 * overflow. If the operation overflows, execution is crash (this function is
 * only used in debug mode, not in release mode).
 */
Uint16
add_with_overflow__Uint16(Uint16 lhs, Uint16 rhs);

/**
 *
 * @brief Add two Uint32 integers and check whether the operation is an
 * overflow. If the operation overflows, execution is crash (this function is
 * only used in debug mode, not in release mode).
 */
Uint32
add_with_overflow__Uint32(Uint32 lhs, Uint32 rhs);

/**
 *
 * @brief Add two Uint64 integers and check whether the operation is an
 * overflow. If the operation overflows, execution is crash (this function is
 * only used in debug mode, not in release mode).
 */
Uint64
add_with_overflow__Uint64(Uint64 lhs, Uint64 rhs);

/**
 *
 * @brief Add two Usize integers and check whether the operation is an
 * overflow. If the operation overflows, execution is crash (this function is
 * only used in debug mode, not in release mode).
 */
Usize
add_with_overflow__Usize(Usize lhs, Usize rhs);

/**
 *
 * @brief Divide two Int8 integers and check whether the operation is an
 * overflow. If the operation overflows, execution is crash (this function is
 * only used in debug mode, not in release mode).
 */
Int8
div_with_overflow__Int8(Int8 lhs, Int8 rhs);

/**
 *
 * @brief Divide two Int16 integers and check whether the operation is an
 * overflow. If the operation overflows, execution is crash (this function is
 * only used in debug mode, not in release mode).
 */
Int16
div_with_overflow__Int16(Int16 lhs, Int16 rhs);

/**
 *
 * @brief Divide two Int32 integers and check whether the operation is an
 * overflow. If the operation overflows, execution is crash (this function is
 * only used in debug mode, not in release mode).
 */
Int32
div_with_overflow__Int32(Int32 lhs, Int32 rhs);

/**
 *
 * @brief Divide two Int64 integers and check whether the operation is an
 * overflow. If the operation overflows, execution is crash (this function is
 * only used in debug mode, not in release mode).
 */
Int64
div_with_overflow__Int64(Int64 lhs, Int64 rhs);

/**
 *
 * @brief Divide two Isize integers and check whether the operation is an
 * overflow. If the operation overflows, execution is crash (this function is
 * only used in debug mode, not in release mode).
 */
Isize
div_with_overflow__Isize(Isize lhs, Isize rhs);

/**
 *
 * @brief Divide two Uint8 integers and check whether the operation is an
 * overflow. If the operation overflows, execution is crash (this function is
 * only used in debug mode, not in release mode).
 */
Uint8
div_with_overflow__Uint8(Uint8 lhs, Uint8 rhs);

/**
 *
 * @brief Divide two Uint16 integers and check whether the operation is an
 * overflow. If the operation overflows, execution is crash (this function is
 * only used in debug mode, not in release mode).
 */
Uint16
div_with_overflow__Uint16(Uint16 lhs, Uint16 rhs);

/**
 *
 * @brief Divide two Uint32 integers and check whether the operation is an
 * overflow. If the operation overflows, execution is crash (this function is
 * only used in debug mode, not in release mode).
 */
Uint32
div_with_overflow__Uint32(Uint32 lhs, Uint32 rhs);

/**
 *
 * @brief Divide two Uint64 integers and check whether the operation is an
 * overflow. If the operation overflows, execution is crash (this function is
 * only used in debug mode, not in release mode).
 */
Uint64
div_with_overflow__Uint64(Uint64 lhs, Uint64 rhs);

/**
 *
 * @brief Divide two Usize integers and check whether the operation is an
 * overflow. If the operation overflows, execution is crash (this function is
 * only used in debug mode, not in release mode).
 */
Usize
div_with_overflow__Usize(Usize lhs, Usize rhs);

/**
 *
 * @brief Multiply two Int8 integers and check whether the operation is an
 * overflow. If the operation overflows, execution is crash (this function is
 * only used in debug mode, not in release mode).
 */
Int8
mul_with_overflow__Int8(Int8 lhs, Int8 rhs);

/**
 *
 * @brief Multiply two Int16 integers and check whether the operation is an
 * overflow. If the operation overflows, execution is crash (this function is
 * only used in debug mode, not in release mode).
 */
Int16
mul_with_overflow__Int16(Int16 lhs, Int16 rhs);

/**
 *
 * @brief Multiply two Int32 integers and check whether the operation is an
 * overflow. If the operation overflows, execution is crash (this function is
 * only used in debug mode, not in release mode).
 */
Int32
mul_with_overflow__Int32(Int32 lhs, Int32 rhs);

/**
 *
 * @brief Multiply two Int64 integers and check whether the operation is an
 * overflow. If the operation overflows, execution is crash (this function is
 * only used in debug mode, not in release mode).
 */
Int64
mul_with_overflow__Int64(Int64 lhs, Int64 rhs);

/**
 *
 * @brief Multiply two Isize integers and check whether the operation is an
 * overflow. If the operation overflows, execution is crash (this function is
 * only used in debug mode, not in release mode).
 */
Isize
mul_with_overflow__Isize(Isize lhs, Isize rhs);

/**
 *
 * @brief Multiply two Uint8 integers and check whether the operation is an
 * overflow. If the operation overflows, execution is crash (this function is
 * only used in debug mode, not in release mode).
 */
Uint8
mul_with_overflow__Uint8(Uint8 lhs, Uint8 rhs);

/**
 *
 * @brief Multiply two Uint16 integers and check whether the operation is an
 * overflow. If the operation overflows, execution is crash (this function is
 * only used in debug mode, not in release mode).
 */
Uint16
mul_with_overflow__Uint16(Uint16 lhs, Uint16 rhs);

/**
 *
 * @brief Multiply two Uint32 integers and check whether the operation is an
 * overflow. If the operation overflows, execution is crash (this function is
 * only used in debug mode, not in release mode).
 */
Uint32
mul_with_overflow__Uint32(Uint32 lhs, Uint32 rhs);

/**
 *
 * @brief Multiply two Uint64 integers and check whether the operation is an
 * overflow. If the operation overflows, execution is crash (this function is
 * only used in debug mode, not in release mode).
 */
Uint64
mul_with_overflow__Uint64(Uint64 lhs, Uint64 rhs);

/**
 *
 * @brief Multiply two Usize integers and check whether the operation is an
 * overflow. If the operation overflows, execution is crash (this function is
 * only used in debug mode, not in release mode).
 */
Usize
mul_with_overflow__Usize(Usize lhs, Usize rhs);

/**
 *
 * @brief Modulo operation on Float64.
 */
inline Float64
mod__Float64(Float64 lhs, Float64 rhs)
{
    return fmod(lhs, rhs);
}

#endif // LILY_CORE_LILY_INTERPRETER_RUNTIME_OPERATOR_H
