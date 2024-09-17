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

#include <core/cc/ci/primary_data_types.h>

// See: https://en.cppreference.com/w/cpp/language/types
static CIDataType *primary_data_type_bool = NULL;            // bool
static CIDataType *primary_data_type_char = NULL;            // char
static CIDataType *primary_data_type_double = NULL;          // double
static CIDataType *primary_data_type_double__complex = NULL; // double _Complex
static CIDataType *primary_data_type_double__imaginary =
  NULL;                                                     // double _Imaginary
static CIDataType *primary_data_type__decimal32 = NULL;     // _Decimal32
static CIDataType *primary_data_type__decimal64 = NULL;     // _Decimal64
static CIDataType *primary_data_type__decimal128 = NULL;    // _Decimal128
static CIDataType *primary_data_type_float = NULL;          // float
static CIDataType *primary_data_type_float__complex = NULL; // float _Complex
static CIDataType *primary_data_type_float__imaginary =
  NULL;                                                  // float _Imaginary
static CIDataType *primary_data_type_int = NULL;         // int
static CIDataType *primary_data_type_long_double = NULL; // long double
static CIDataType *primary_data_type_long_double__complex =
  NULL; // long double _Complex
static CIDataType *primary_data_type_long_double__imaginary =
  NULL;                                               // long double _Imaginary
static CIDataType *primary_data_type_long_int = NULL; // long int
static CIDataType *primary_data_type_long_long_int = NULL; // long long int
static CIDataType *primary_data_type_short_int = NULL;     // short int
static CIDataType *primary_data_type_signed_char = NULL;   // signed char
static CIDataType *primary_data_type_unsigned_int = NULL;  // unsigned int
static CIDataType *primary_data_type_unsigned_char = NULL; // unsigned char
static CIDataType *primary_data_type_unsigned_long_int =
  NULL; // unsigned long int
static CIDataType *primary_data_type_unsigned_long_long_int =
  NULL; // unsigned long long int
static CIDataType *primary_data_type_unsigned_short_int =
  NULL;                                           // unsigned short int
static CIDataType *primary_data_type_void = NULL; // void

void
init__PrimaryDataTypes()
{
    primary_data_type_bool = NEW(CIDataType, CI_DATA_TYPE_KIND_BOOL);
    primary_data_type_char = NEW(CIDataType, CI_DATA_TYPE_KIND_CHAR);
    primary_data_type_double = NEW(CIDataType, CI_DATA_TYPE_KIND_DOUBLE);
    primary_data_type_double__complex =
      NEW(CIDataType, CI_DATA_TYPE_KIND_DOUBLE__COMPLEX);
    primary_data_type_double__imaginary =
      NEW(CIDataType, CI_DATA_TYPE_KIND_DOUBLE__IMAGINARY);
    primary_data_type__decimal32 =
      NEW(CIDataType, CI_DATA_TYPE_KIND__DECIMAL32);
    primary_data_type__decimal64 =
      NEW(CIDataType, CI_DATA_TYPE_KIND__DECIMAL64);
    primary_data_type__decimal128 =
      NEW(CIDataType, CI_DATA_TYPE_KIND__DECIMAL128);
    primary_data_type_float = NEW(CIDataType, CI_DATA_TYPE_KIND_FLOAT);
    primary_data_type_float__complex =
      NEW(CIDataType, CI_DATA_TYPE_KIND_FLOAT__COMPLEX);
    primary_data_type_float__imaginary =
      NEW(CIDataType, CI_DATA_TYPE_KIND_FLOAT__IMAGINARY);
    primary_data_type_int = NEW(CIDataType, CI_DATA_TYPE_KIND_INT);
    primary_data_type_long_double =
      NEW(CIDataType, CI_DATA_TYPE_KIND_LONG_DOUBLE);
    primary_data_type_long_double__complex =
      NEW(CIDataType, CI_DATA_TYPE_KIND_LONG_DOUBLE__COMPLEX);
    primary_data_type_long_double__imaginary =
      NEW(CIDataType, CI_DATA_TYPE_KIND_LONG_DOUBLE__IMAGINARY);
    primary_data_type_long_int = NEW(CIDataType, CI_DATA_TYPE_KIND_LONG_INT);
    primary_data_type_long_long_int =
      NEW(CIDataType, CI_DATA_TYPE_KIND_LONG_LONG_INT);
    primary_data_type_short_int = NEW(CIDataType, CI_DATA_TYPE_KIND_SHORT_INT);
    primary_data_type_signed_char =
      NEW(CIDataType, CI_DATA_TYPE_KIND_SIGNED_CHAR);
    primary_data_type_unsigned_int =
      NEW(CIDataType, CI_DATA_TYPE_KIND_UNSIGNED_INT);
    primary_data_type_unsigned_char =
      NEW(CIDataType, CI_DATA_TYPE_KIND_UNSIGNED_CHAR);
    primary_data_type_unsigned_long_int =
      NEW(CIDataType, CI_DATA_TYPE_KIND_UNSIGNED_LONG_INT);
    primary_data_type_unsigned_long_long_int =
      NEW(CIDataType, CI_DATA_TYPE_KIND_UNSIGNED_LONG_LONG_INT);
    primary_data_type_unsigned_short_int =
      NEW(CIDataType, CI_DATA_TYPE_KIND_UNSIGNED_SHORT_INT);
    primary_data_type_void = NEW(CIDataType, CI_DATA_TYPE_KIND_VOID);
}

#define PRIMARY_FN_DEF(dt)                              \
    CIDataType *dt##__PrimaryDataTypes()                \
    {                                                   \
        return ref__CIDataType(primary_data_type_##dt); \
    }

PRIMARY_FN_DEF(bool);
PRIMARY_FN_DEF(char);
PRIMARY_FN_DEF(double);
PRIMARY_FN_DEF(double__complex);
PRIMARY_FN_DEF(double__imaginary);
PRIMARY_FN_DEF(_decimal32);
PRIMARY_FN_DEF(_decimal64);
PRIMARY_FN_DEF(_decimal128);
PRIMARY_FN_DEF(float);
PRIMARY_FN_DEF(float__complex);
PRIMARY_FN_DEF(float__imaginary);
PRIMARY_FN_DEF(int);
PRIMARY_FN_DEF(long_double);
PRIMARY_FN_DEF(long_double__complex);
PRIMARY_FN_DEF(long_double__imaginary);
PRIMARY_FN_DEF(long_int);
PRIMARY_FN_DEF(long_long_int);
PRIMARY_FN_DEF(short_int);
PRIMARY_FN_DEF(signed_char);
PRIMARY_FN_DEF(unsigned_int);
PRIMARY_FN_DEF(unsigned_char);
PRIMARY_FN_DEF(unsigned_long_int);
PRIMARY_FN_DEF(unsigned_long_long_int);
PRIMARY_FN_DEF(unsigned_short_int);
PRIMARY_FN_DEF(void);

#undef PRIMARY_FN_DEF

void
free__PrimaryDataTypes()
{
    FREE(CIDataType, primary_data_type_bool);
    FREE(CIDataType, primary_data_type_char);
    FREE(CIDataType, primary_data_type_double);
    FREE(CIDataType, primary_data_type_double__complex);
    FREE(CIDataType, primary_data_type_double__imaginary);
    FREE(CIDataType, primary_data_type__decimal32);
    FREE(CIDataType, primary_data_type__decimal64);
    FREE(CIDataType, primary_data_type__decimal128);
    FREE(CIDataType, primary_data_type_float);
    FREE(CIDataType, primary_data_type_float__complex);
    FREE(CIDataType, primary_data_type_float__imaginary);
    FREE(CIDataType, primary_data_type_int);
    FREE(CIDataType, primary_data_type_long_double);
    FREE(CIDataType, primary_data_type_long_double__complex);
    FREE(CIDataType, primary_data_type_long_double__imaginary);
    FREE(CIDataType, primary_data_type_long_int);
    FREE(CIDataType, primary_data_type_long_long_int);
    FREE(CIDataType, primary_data_type_short_int);
    FREE(CIDataType, primary_data_type_signed_char);
    FREE(CIDataType, primary_data_type_unsigned_int);
    FREE(CIDataType, primary_data_type_unsigned_char);
    FREE(CIDataType, primary_data_type_unsigned_long_int);
    FREE(CIDataType, primary_data_type_unsigned_long_long_int);
    FREE(CIDataType, primary_data_type_unsigned_short_int);
    FREE(CIDataType, primary_data_type_void);
}
