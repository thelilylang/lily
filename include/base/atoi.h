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

#ifndef LILY_BASE_ATOI_H
#define LILY_BASE_ATOI_H

#include <base/types.h>

/**
 *
 * @brief Convert to string an Int8.
 */
Int8
atoi__Int8(const char *s);

/**
 *
 * @brief Convert to string an Int16.
 */
Int16
atoi__Int16(const char *s);

/**
 *
 * @brief Convert to string an Int32.
 */
Int32
atoi__Int32(const char *s);

/**
 *
 * @brief convert to string an int64.
 */
Int64
atoi__Int64(const char *s);

/**
 *
 * @brief Convert to string an Uint8.
 */
Uint8
atoi__Uint8(const char *s);

/**
 *
 * @brief Convert to string an Uint16.
 */
Uint16
atoi__Uint16(const char *s);

/**
 *
 * @brief convert to string an Uint32.
 */
Uint32
atoi__Uint32(const char *s);

/**
 *
 * @brief convert to string an Uint64.
 */
Uint64
atoi__Uint64(const char *s);

/**
 *
 * @brief convert to string an Isize.
 */
Isize
atoi__Isize(const char *s);

/**
 *
 * @brief convert to string a Usize.
 */
Usize
atoi__Usize(const char *s);

#endif // LILY_BASE_ATOI_H
