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

#include <base/types.h>

#include <core/lily/interpreter/vm/runtime/error.h>
#include <core/lily/interpreter/vm/runtime/operator.h>

#include <stdint.h>

Int8
add_with_overflow__Int8(Int8 lhs, Int8 rhs)
{
    Int8 res = lhs + rhs;

    if ((lhs ^ res) & (rhs ^ res) & INT8_MIN) {
        // TODO: add line and column to improve the message.
        RUNTIME_ERROR_COMMON("Int8 overflow during an addition operation");
    }

    return res;
}

Int16
add_with_overflow__Int16(Int16 lhs, Int16 rhs)
{
    Int16 res = lhs + rhs;

    if ((lhs ^ res) & (rhs ^ res) & INT16_MIN) {
        // TODO: add line and column to improve the message.
        RUNTIME_ERROR_COMMON("Int16 overflow during an addition operation");
    }

    return res;
}

Int32
add_with_overflow__Int32(Int32 lhs, Int32 rhs)
{
    Int32 res = lhs + rhs;

    if ((lhs ^ res) & (rhs ^ res) & INT32_MIN) {
        // TODO: add line and column to improve the message.
        RUNTIME_ERROR_COMMON("Int32 overflow during an addition operation");
    }

    return res;
}

Int64
add_with_overflow__Int64(Int64 lhs, Int64 rhs)
{
    Int64 res = lhs + rhs;

    if ((lhs ^ res) & (rhs ^ res) & INT64_MIN) {
        // TODO: add line and column to improve the message.
        RUNTIME_ERROR_COMMON("Int64 overflow during an addition operation");
    }

    return res;
}

Isize
add_with_overflow__Isize(Isize lhs, Isize rhs)
{
    Isize res = lhs + rhs;

#ifdef PLATFORM_64
    if ((lhs ^ res) & (rhs ^ res) & INT64_MIN) {
        // TODO: add line and column to improve the message.
        RUNTIME_ERROR_COMMON("Isize overflow during an addition operation");
    }
#else
    if ((lhs ^ res) & (rhs ^ res) & INT32_MIN) {
        // TODO: add line and column to improve the message.
        RUNTIME_ERROR_COMMON("Isize overflow during an addition operation");
    }
#endif

    return res;
}

Uint8
add_with_overflow__Uint8(Uint8 lhs, Uint8 rhs)
{
    Uint8 res = lhs + rhs;

    if (res < lhs || res < rhs) {
        // TODO: add line and column to improve the message.
        RUNTIME_ERROR_COMMON("Uint8 overflow during an addition operation");
    }

    return res;
}

Uint16
add_with_overflow__Uint16(Uint16 lhs, Uint16 rhs)
{
    Uint16 res = lhs + rhs;

    if (res < lhs || res > rhs) {
        // TODO: add line and column to improve the message.
        RUNTIME_ERROR_COMMON("Uint16 overflow during an addition operation");
    }

    return res;
}

Uint32
add_with_overflow__Uint32(Uint32 lhs, Uint32 rhs)
{
    Uint32 res = lhs + rhs;

    if (res < lhs || res > rhs) {
        // TODO: add line and column to improve the message.
        RUNTIME_ERROR_COMMON("Uint32 overflow during an addition operation");
    }

    return res;
}

Uint64
add_with_overflow__Uint64(Uint64 lhs, Uint64 rhs)
{
    Uint64 res = lhs + rhs;

    if (res < lhs || res > rhs) {
        // TODO: add line and column to improve the message.
        RUNTIME_ERROR_COMMON("Uint64 overflow during an addition operation");
    }

    return res;
}

Usize
add_with_overflow__Usize(Usize lhs, Usize rhs)
{
    Usize res = lhs + rhs;

    if (res < lhs || res > rhs) {
        // TODO: add line and column to improve the message.
        RUNTIME_ERROR_COMMON("Usize overflow during an addition operation");
    }

    return res;
}

Int8
div_with_overflow__Int8(Int8 lhs, Int8 rhs)
{
    if (rhs == 0) {
        // TODO: add line and column to improve the message.
        RUNTIME_ERROR_COMMON("Division by 0");
    }

    if (lhs == INT8_MIN && rhs == -1) {
        // TODO: add line and column to improve the message.
        RUNTIME_ERROR_COMMON("Int8 overflow during a division operation");
    }

    return lhs / rhs;
}

Int16
div_with_overflow__Int16(Int16 lhs, Int16 rhs)
{
    if (rhs == 0) {
        // TODO: add line and column to improve the message.
        RUNTIME_ERROR_COMMON("Division by 0");
    }

    if (lhs == INT16_MIN && rhs == -1) {
        // TODO: add line and column to improve the message.
        RUNTIME_ERROR_COMMON("Int16 overflow during a division operation");
    }

    return lhs / rhs;
}

Int32
div_with_overflow__Int32(Int32 lhs, Int32 rhs)
{
    if (rhs == 0) {
        // TODO: add line and column to improve the message.
        RUNTIME_ERROR_COMMON("Division by 0");
    }

    if (lhs == INT32_MIN && rhs == -1) {
        // TODO: add line and column to improve the message.
        RUNTIME_ERROR_COMMON("Int32 overflow during a division operation");
    }

    return lhs / rhs;
}

Int64
div_with_overflow__Int64(Int64 lhs, Int64 rhs)
{
    if (rhs == 0) {
        // TODO: add line and column to improve the message.
        RUNTIME_ERROR_COMMON("Division by 0");
    }

    if (lhs == INT64_MIN && rhs == -1) {
        // TODO: add line and column to improve the message.
        RUNTIME_ERROR_COMMON("Int64 overflow during a division operation");
    }

    return lhs / rhs;
}

Isize
div_with_overflow__Isize(Isize lhs, Isize rhs)
{
    if (rhs == 0) {
        // TODO: add line and column to improve the message.
        RUNTIME_ERROR_COMMON("Division by 0");
    }

    if (lhs == INT64_MIN && rhs == -1) {
        // TODO: add line and column to improve the message.
        RUNTIME_ERROR_COMMON("Isize overflow during a division operation");
    }

    return lhs / rhs;
}

Uint8
div_with_overflow__Uint8(Uint8 lhs, Uint8 rhs)
{
    if (rhs == 0) {
        // TODO: add line and column to improve the message.
        RUNTIME_ERROR_COMMON("Division by 0");
    }

    return lhs / rhs;
}

Uint16
div_with_overflow__Uint16(Uint16 lhs, Uint16 rhs)
{
    if (rhs == 0) {
        // TODO: add line and column to improve the message.
        RUNTIME_ERROR_COMMON("Division by 0");
    }

    return lhs / rhs;
}

Uint32
div_with_overflow__Uint32(Uint32 lhs, Uint32 rhs)
{
    if (rhs == 0) {
        // TODO: add line and column to improve the message.
        RUNTIME_ERROR_COMMON("Division by 0");
    }

    return lhs / rhs;
}

Uint64
div_with_overflow__Uint64(Uint64 lhs, Uint64 rhs)
{
    if (rhs == 0) {
        // TODO: add line and column to improve the message.
        RUNTIME_ERROR_COMMON("Division by 0");
    }

    return lhs / rhs;
}

Usize
div_with_overflow__Usize(Usize lhs, Usize rhs)
{
    if (rhs == 0) {
        // TODO: add line and column to improve the message.
        RUNTIME_ERROR_COMMON("Division by 0");
    }

    return lhs / rhs;
}

Int8
mul_with_overflow__Int8(Int8 lhs, Int8 rhs)
{
    Int8 res = lhs * rhs;

    if (lhs != 0 && rhs != 0 && res / lhs != rhs) {
        // TODO: add line and column to improve the message.
        RUNTIME_ERROR_COMMON("Int8 overflow during a multiplication operation");
    }

    return res;
}

Int16
mul_with_overflow__Int16(Int16 lhs, Int16 rhs)
{
    Int16 res = lhs * rhs;

    if (lhs != 0 && rhs != 0 && res / lhs != rhs) {
        // TODO: add line and column to improve the message.
        RUNTIME_ERROR_COMMON(
          "Int16 overflow during a multiplication operation");
    }

    return res;
}

Int32
mul_with_overflow__Int32(Int32 lhs, Int32 rhs)
{
    Int32 res = lhs * rhs;

    if (lhs != 0 && rhs != 0 && res / lhs != rhs) {
        // TODO: add line and column to improve the message.
        RUNTIME_ERROR_COMMON(
          "Int32 overflow during a multiplication operation");
    }

    return res;
}

Int64
mul_with_overflow__Int64(Int64 lhs, Int64 rhs)
{
    Int64 res = lhs * rhs;

    if (lhs != 0 && rhs != 0 && res / lhs != rhs) {
        // TODO: add line and column to improve the message.
        RUNTIME_ERROR_COMMON(
          "Int64 overflow during a multiplication operation");
    }

    return res;
}

Isize
mul_with_overflow__Isize(Isize lhs, Isize rhs)
{
    Isize res = lhs * rhs;

    if (lhs != 0 && rhs != 0 && res / lhs != rhs) {
        // TODO: add line and column to improve the message.
        RUNTIME_ERROR_COMMON(
          "Isize overflow during a multiplication operation");
    }

    return res;
}

Uint8
mul_with_overflow__Uint8(Uint8 lhs, Uint8 rhs)
{
    Uint8 res = lhs * rhs;

    if (lhs != 0 && rhs != 0 && res / lhs != rhs) {
        // TODO: add line and column to improve the message.
        RUNTIME_ERROR_COMMON(
          "Uint8 overflow during a multiplication operation");
    }

    return res;
}

Uint16
mul_with_overflow__Uint16(Uint16 lhs, Uint16 rhs)
{
    Uint16 res = lhs * rhs;

    if (lhs != 0 && rhs != 0 && res / lhs != rhs) {
        // TODO: add line and column to improve the message.
        RUNTIME_ERROR_COMMON(
          "Uint16 overflow during a multiplication operation");
    }

    return res;
}

Uint32
mul_with_overflow__Uint32(Uint32 lhs, Uint32 rhs)
{
    Uint32 res = lhs * rhs;

    if (lhs != 0 && rhs != 0 && res / lhs != rhs) {
        // TODO: add line and column to improve the message.
        RUNTIME_ERROR_COMMON(
          "Uint32 overflow during a multiplication operation");
    }

    return res;
}

Uint64
mul_with_overflow__Uint64(Uint64 lhs, Uint64 rhs)
{
    Uint64 res = lhs * rhs;

    if (lhs != 0 && rhs != 0 && res / lhs != rhs) {
        // TODO: add line and column to improve the message.
        RUNTIME_ERROR_COMMON(
          "Uint64 overflow during a multiplication operation");
    }

    return res;
}

Usize
mul_with_overflow__Usize(Usize lhs, Usize rhs)
{
    Usize res = lhs * rhs;

    if (lhs != 0 && rhs != 0 && res / lhs != rhs) {
        // TODO: add line and column to improve the message.
        RUNTIME_ERROR_COMMON(
          "Usize overflow during a multiplication operation");
    }

    return res;
}
