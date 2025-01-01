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

#ifndef LILY_BASE_ITOA_H
#define LILY_BASE_ITOA_H

#include <base/types.h>

/**
 *
 * @brief Convert integer to string.
 * @param v The integer to be converted.
 * @param base The base of the integer.
 * @return The string.
 */
char *
itoa__Int8(Int8 v, int base);

/**
 *
 * @brief Convert unsigned integer to string.
 * @param v The unsigned integer to be converted.
 * @param base The base of the unsigned integer.
 * @return The string.
 */
char *
itoa__Uint8(Uint8 v, int base);

/**
 *
 * @brief Convert integer to string.
 * @param v The integer to be converted.
 * @param base The base of the integer.
 * @return The string.
 */
char *
itoa__Int16(Int16 v, int base);

/**
 *
 * @brief Convert unsigned integer to string.
 * @param v The unsigned integer to be converted.
 * @param base The base of the unsigned integer.
 * @return The string.
 */
char *
itoa__Uint16(Uint16 v, int base);

/**
 *
 * @brief Convert integer to string.
 * @param v The integer to be converted.
 * @param base The base of the integer.
 * @return The string.
 */
char *
itoa__Int32(Int32 v, int base);

/**
 *
 * @brief Convert unsigned integer to string.
 * @param v The unsigned integer to be converted.
 * @param base The base of the unsigned integer.
 * @return The string.
 */
char *
itoa__Uint32(Uint32 v, int base);

/**
 *
 * @brief Convert integer to string.
 * @param v The integer to be converted.
 * @param base The base of the integer.
 * @return The string.
 */
char *
itoa__Int64(Int64 v, int base);

/**
 *
 * @brief Convert unsigned integer to string.
 * @param v The unsigned integer to be converted.
 * @param base The base of the unsigned integer.
 * @return The string.
 */
char *
itoa__Uint64(Uint64 v, int base);

#endif // LILY_BASE_ITOA_H
