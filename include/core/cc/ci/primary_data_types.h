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

#ifndef LILY_CORE_CC_CI_PRIMARY_DATA_TYPES_H
#define LILY_CORE_CC_CI_PRIMARY_DATA_TYPES_H

#include <core/cc/ci/ast.h>

/**
 *
 * @brief Initialize all primary data types.
 */
void
init__PrimaryDataTypes();

/**
 *
 * @brief Get Primary data type.
 * @return CIDataType*
 */
#define PRIMARY_FN_DECL(dt) CIDataType *dt##__PrimaryDataTypes();

PRIMARY_FN_DECL(any);
PRIMARY_FN_DECL(bool);
PRIMARY_FN_DECL(char);
PRIMARY_FN_DECL(double);
PRIMARY_FN_DECL(double__complex);
PRIMARY_FN_DECL(double__imaginary);
PRIMARY_FN_DECL(_decimal32);
PRIMARY_FN_DECL(_decimal64);
PRIMARY_FN_DECL(_decimal128);
PRIMARY_FN_DECL(float);
PRIMARY_FN_DECL(float__complex);
PRIMARY_FN_DECL(float__imaginary);
PRIMARY_FN_DECL(int);
PRIMARY_FN_DECL(long_double);
PRIMARY_FN_DECL(long_double__complex);
PRIMARY_FN_DECL(long_double__imaginary);
PRIMARY_FN_DECL(long_int);
PRIMARY_FN_DECL(long_long_int);
PRIMARY_FN_DECL(short_int);
PRIMARY_FN_DECL(signed_char);
PRIMARY_FN_DECL(type_info);
PRIMARY_FN_DECL(unsigned_int);
PRIMARY_FN_DECL(unsigned_char);
PRIMARY_FN_DECL(unsigned_long_int);
PRIMARY_FN_DECL(unsigned_long_long_int);
PRIMARY_FN_DECL(unsigned_short_int);
PRIMARY_FN_DECL(void);

#undef PRIMARY_FN_DECL

/**
 *
 * @brief Free all primary data types.
 */
void
free__PrimaryDataTypes();

#endif // LILY_CORE_CC_CI_PRIMARY_DATA_TYPES_H
