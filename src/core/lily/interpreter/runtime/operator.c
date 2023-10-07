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

#include <base/types.h>

#include <core/lily/interpreter/runtime/error.h>
#include <core/lily/interpreter/runtime/operator.h>

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
