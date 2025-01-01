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

#include <core/lily/analysis/checked/operator.h>

#include <string.h>

CONSTRUCTOR(LilyCheckedOperator *,
            LilyCheckedOperator,
            String *name,
            Vec *signature,
            bool is_default)
{
    LilyCheckedOperator *self = lily_malloc(sizeof(LilyCheckedOperator));

    self->name = name;
    self->signature = signature;
    self->is_default = is_default;
    self->ref_count = 0;

    return self;
}

bool
valid_operator__LilyCheckedOperator(String *name)
{
    if (!strcmp(name->buffer, "+") || !strcmp(name->buffer, "and") ||
        !strcmp(name->buffer, "+=") || !strcmp(name->buffer, "&=") ||
        !strcmp(name->buffer, "<<=") || !strcmp(name->buffer, "|=") ||
        !strcmp(name->buffer, ">>=") || !strcmp(name->buffer, "/=") ||
        !strcmp(name->buffer, "**=") || !strcmp(name->buffer, "%=") ||
        !strcmp(name->buffer, "*=") || !strcmp(name->buffer, "-=") ||
        !strcmp(name->buffer, "xor=") || !strcmp(name->buffer, "=") ||
        !strcmp(name->buffer, "&") || !strcmp(name->buffer, "|") ||
        !strcmp(name->buffer, "/") || !strcmp(name->buffer, "==") ||
        !strcmp(name->buffer, "**") || !strcmp(name->buffer, ">=") ||
        !strcmp(name->buffer, ">") || !strcmp(name->buffer, "<<") ||
        !strcmp(name->buffer, "<=") || !strcmp(name->buffer, "<") ||
        !strcmp(name->buffer, "%") || !strcmp(name->buffer, "*") ||
        !strcmp(name->buffer, "not=") || !strcmp(name->buffer, "or") ||
        !strcmp(name->buffer, ">>") || !strcmp(name->buffer, ">>") ||
        !strcmp(name->buffer, "..") || !strcmp(name->buffer, "-") ||
        !strcmp(name->buffer, "xor") || !strcmp(name->buffer, "->") ||
        !strcmp(name->buffer, "<-") || !strcmp(name->buffer, "[n]") ||
        !strcmp(name->buffer, "[n..n]") || !strcmp(name->buffer, "[n..]") ||
        !strcmp(name->buffer, "[..n]") || !strcmp(name->buffer, ".*") ||
        !strcmp(name->buffer, "not") || !strcmp(name->buffer, "ref") ||
        !strcmp(name->buffer, "ref mut") || !strcmp(name->buffer, "trace") ||
        !strcmp(name->buffer, "trace mut")) {
        return true;
    }

    return false;
}

// NOTE: You'll probably notice that types that are sence and mutable as in
// assignment operators are not. This is because the compiler checks whether all
// assigned types are mutable.

#define BASIC_ARITHMETIC_BINARY_OPERATOR(op_name)                              \
    default_operators[idx++] =                                                 \
      NEW(LilyCheckedOperator,                                                 \
          from__String(op_name),                                               \
          init__Vec(                                                           \
            3,                                                                 \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_BYTE, NULL),  \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_BYTE, NULL),  \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_BYTE, NULL)), \
          true);                                                               \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CSHORT, NULL),    \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CSHORT, NULL),    \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CSHORT, NULL)),   \
      true);                                                                   \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CUSHORT, NULL),   \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CUSHORT, NULL),   \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CUSHORT, NULL)),  \
      true);                                                                   \
                                                                               \
    default_operators[idx++] =                                                 \
      NEW(LilyCheckedOperator,                                                 \
          from__String(op_name),                                               \
          init__Vec(                                                           \
            3,                                                                 \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CINT, NULL),  \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CINT, NULL),  \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CINT, NULL)), \
          true);                                                               \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CUINT, NULL),     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CUINT, NULL),     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CUINT, NULL)),    \
      true);                                                                   \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CLONG, NULL),     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CLONG, NULL),     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CLONG, NULL)),    \
      true);                                                                   \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CULONG, NULL),    \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CULONG, NULL),    \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CULONG, NULL)),   \
      true);                                                                   \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CLONGLONG, NULL), \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CLONGLONG, NULL), \
        NEW(                                                                   \
          LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CLONGLONG, NULL)),  \
      true);                                                                   \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(                                                                   \
          LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CULONGLONG, NULL),  \
        NEW(                                                                   \
          LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CULONGLONG, NULL),  \
        NEW(                                                                   \
          LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CULONGLONG, NULL)), \
      true);                                                                   \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CFLOAT, NULL),    \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CFLOAT, NULL),    \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CFLOAT, NULL)),   \
      true);                                                                   \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CDOUBLE, NULL),   \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CDOUBLE, NULL),   \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CDOUBLE, NULL)),  \
      true);                                                                   \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_FLOAT32, NULL),   \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_FLOAT32, NULL),   \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_FLOAT32, NULL)),  \
      true);                                                                   \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_FLOAT64, NULL),   \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_FLOAT64, NULL),   \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_FLOAT64, NULL)),  \
      true);                                                                   \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_INT16, NULL),     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_INT16, NULL),     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_INT16, NULL)),    \
      true);                                                                   \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_INT32, NULL),     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_INT32, NULL),     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_INT32, NULL)),    \
      true);                                                                   \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_INT64, NULL),     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_INT64, NULL),     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_INT64, NULL)),    \
      true);                                                                   \
                                                                               \
    default_operators[idx++] =                                                 \
      NEW(LilyCheckedOperator,                                                 \
          from__String(op_name),                                               \
          init__Vec(                                                           \
            3,                                                                 \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_INT8, NULL),  \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_INT8, NULL),  \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_INT8, NULL)), \
          true);                                                               \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_ISIZE, NULL),     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_ISIZE, NULL),     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_ISIZE, NULL)),    \
      true);                                                                   \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UINT16, NULL),    \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UINT16, NULL),    \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UINT16, NULL)),   \
      true);                                                                   \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UINT32, NULL),    \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UINT32, NULL),    \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UINT32, NULL)),   \
      true);                                                                   \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UINT64, NULL),    \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UINT64, NULL),    \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UINT64, NULL)),   \
      true);                                                                   \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UINT8, NULL),     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UINT8, NULL),     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UINT8, NULL)),    \
      true);                                                                   \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_USIZE, NULL),     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_USIZE, NULL),     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_USIZE, NULL)),    \
      true);

#define BASIC_BITWISE_BINARY_OPERATOR(op_name)                                 \
    default_operators[idx++] =                                                 \
      NEW(LilyCheckedOperator,                                                 \
          from__String(op_name),                                               \
          init__Vec(                                                           \
            3,                                                                 \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_BOOL, NULL),  \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_BOOL, NULL),  \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_BOOL, NULL)), \
          true);                                                               \
                                                                               \
    default_operators[idx++] =                                                 \
      NEW(LilyCheckedOperator,                                                 \
          from__String(op_name),                                               \
          init__Vec(                                                           \
            3,                                                                 \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_BYTE, NULL),  \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_BYTE, NULL),  \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_BYTE, NULL)), \
          true);                                                               \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CSHORT, NULL),    \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CSHORT, NULL),    \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CSHORT, NULL)),   \
      true);                                                                   \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CUSHORT, NULL),   \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CUSHORT, NULL),   \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CUSHORT, NULL)),  \
      true);                                                                   \
                                                                               \
    default_operators[idx++] =                                                 \
      NEW(LilyCheckedOperator,                                                 \
          from__String(op_name),                                               \
          init__Vec(                                                           \
            3,                                                                 \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CINT, NULL),  \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CINT, NULL),  \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CINT, NULL)), \
          true);                                                               \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CUINT, NULL),     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CUINT, NULL),     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CUINT, NULL)),    \
      true);                                                                   \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CLONG, NULL),     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CLONG, NULL),     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CLONG, NULL)),    \
      true);                                                                   \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CULONG, NULL),    \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CULONG, NULL),    \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CULONG, NULL)),   \
      true);                                                                   \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CLONGLONG, NULL), \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CLONGLONG, NULL), \
        NEW(                                                                   \
          LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CLONGLONG, NULL)),  \
      true);                                                                   \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(                                                                   \
          LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CULONGLONG, NULL),  \
        NEW(                                                                   \
          LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CULONGLONG, NULL),  \
        NEW(                                                                   \
          LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CULONGLONG, NULL)), \
      true);                                                                   \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CFLOAT, NULL),    \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CFLOAT, NULL),    \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CFLOAT, NULL)),   \
      true);                                                                   \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CDOUBLE, NULL),   \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CDOUBLE, NULL),   \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CDOUBLE, NULL)),  \
      true);                                                                   \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_INT16, NULL),     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_INT16, NULL),     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_INT16, NULL)),    \
      true);                                                                   \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_INT32, NULL),     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_INT32, NULL),     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_INT32, NULL)),    \
      true);                                                                   \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_INT64, NULL),     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_INT64, NULL),     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_INT64, NULL)),    \
      true);                                                                   \
                                                                               \
    default_operators[idx++] =                                                 \
      NEW(LilyCheckedOperator,                                                 \
          from__String(op_name),                                               \
          init__Vec(                                                           \
            3,                                                                 \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_INT8, NULL),  \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_INT8, NULL),  \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_INT8, NULL)), \
          true);                                                               \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_ISIZE, NULL),     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_ISIZE, NULL),     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_ISIZE, NULL)),    \
      true);                                                                   \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UINT16, NULL),    \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UINT16, NULL),    \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UINT16, NULL)),   \
      true);                                                                   \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UINT32, NULL),    \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UINT32, NULL),    \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UINT32, NULL)),   \
      true);                                                                   \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UINT64, NULL),    \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UINT64, NULL),    \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UINT64, NULL)),   \
      true);                                                                   \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UINT8, NULL),     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UINT8, NULL),     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UINT8, NULL)),    \
      true);                                                                   \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_USIZE, NULL),     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_USIZE, NULL),     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_USIZE, NULL)),    \
      true);

#define BASIC_ASSIGN_BINARY_OPERATOR(op_name)                                  \
    default_operators[idx++] =                                                 \
      NEW(LilyCheckedOperator,                                                 \
          from__String(op_name),                                               \
          init__Vec(                                                           \
            3,                                                                 \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_BOOL, NULL),  \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_BOOL, NULL),  \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UNIT, NULL)), \
          true);                                                               \
                                                                               \
    default_operators[idx++] =                                                 \
      NEW(LilyCheckedOperator,                                                 \
          from__String(op_name),                                               \
          init__Vec(                                                           \
            3,                                                                 \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_BYTE, NULL),  \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_BYTE, NULL),  \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UNIT, NULL)), \
          true);                                                               \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CSHORT, NULL),    \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CSHORT, NULL),    \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UNIT, NULL)),     \
      true);                                                                   \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CUSHORT, NULL),   \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CUSHORT, NULL),   \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UNIT, NULL)),     \
      true);                                                                   \
                                                                               \
    default_operators[idx++] =                                                 \
      NEW(LilyCheckedOperator,                                                 \
          from__String(op_name),                                               \
          init__Vec(                                                           \
            3,                                                                 \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CINT, NULL),  \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CINT, NULL),  \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UNIT, NULL)), \
          true);                                                               \
                                                                               \
    default_operators[idx++] =                                                 \
      NEW(LilyCheckedOperator,                                                 \
          from__String(op_name),                                               \
          init__Vec(                                                           \
            3,                                                                 \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CUINT, NULL), \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CUINT, NULL), \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UNIT, NULL)), \
          true);                                                               \
                                                                               \
    default_operators[idx++] =                                                 \
      NEW(LilyCheckedOperator,                                                 \
          from__String(op_name),                                               \
          init__Vec(                                                           \
            3,                                                                 \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CLONG, NULL), \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CLONG, NULL), \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UNIT, NULL)), \
          true);                                                               \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CULONG, NULL),    \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CULONG, NULL),    \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UNIT, NULL)),     \
      true);                                                                   \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CLONGLONG, NULL), \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CLONGLONG, NULL), \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UNIT, NULL)),     \
      true);                                                                   \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(                                                                   \
          LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CULONGLONG, NULL),  \
        NEW(                                                                   \
          LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CULONGLONG, NULL),  \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UNIT, NULL)),     \
      true);                                                                   \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CFLOAT, NULL),    \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CFLOAT, NULL),    \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UNIT, NULL)),     \
      true);                                                                   \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CDOUBLE, NULL),   \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CDOUBLE, NULL),   \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UNIT, NULL)),     \
      true);                                                                   \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_FLOAT32, NULL),   \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_FLOAT32, NULL),   \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UNIT, NULL)),     \
      true);                                                                   \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_FLOAT64, NULL),   \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_FLOAT64, NULL),   \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UNIT, NULL)),     \
      true);                                                                   \
                                                                               \
    default_operators[idx++] =                                                 \
      NEW(LilyCheckedOperator,                                                 \
          from__String(op_name),                                               \
          init__Vec(                                                           \
            3,                                                                 \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_INT16, NULL), \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_INT16, NULL), \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UNIT, NULL)), \
          true);                                                               \
                                                                               \
    default_operators[idx++] =                                                 \
      NEW(LilyCheckedOperator,                                                 \
          from__String(op_name),                                               \
          init__Vec(                                                           \
            3,                                                                 \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_INT32, NULL), \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_INT32, NULL), \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UNIT, NULL)), \
          true);                                                               \
                                                                               \
    default_operators[idx++] =                                                 \
      NEW(LilyCheckedOperator,                                                 \
          from__String(op_name),                                               \
          init__Vec(                                                           \
            3,                                                                 \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_INT64, NULL), \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_INT64, NULL), \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UNIT, NULL)), \
          true);                                                               \
                                                                               \
    default_operators[idx++] =                                                 \
      NEW(LilyCheckedOperator,                                                 \
          from__String(op_name),                                               \
          init__Vec(                                                           \
            3,                                                                 \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_INT8, NULL),  \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_INT8, NULL),  \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UNIT, NULL)), \
          true);                                                               \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UINT16, NULL),    \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UINT16, NULL),    \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UNIT, NULL)),     \
      true);                                                                   \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UINT32, NULL),    \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UINT32, NULL),    \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UNIT, NULL)),     \
      true);                                                                   \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UINT64, NULL),    \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UINT64, NULL),    \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UNIT, NULL)),     \
      true);                                                                   \
                                                                               \
    default_operators[idx++] =                                                 \
      NEW(LilyCheckedOperator,                                                 \
          from__String(op_name),                                               \
          init__Vec(                                                           \
            3,                                                                 \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UINT8, NULL), \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UINT8, NULL), \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UNIT, NULL)), \
          true);

#define BASIC_BITWISE_ASSIGN_BINARY_OPERATOR(op_name)                          \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CSHORT, NULL),    \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CSHORT, NULL),    \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UNIT, NULL)),     \
      true);                                                                   \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CUSHORT, NULL),   \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CUSHORT, NULL),   \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UNIT, NULL)),     \
      true);                                                                   \
                                                                               \
    default_operators[idx++] =                                                 \
      NEW(LilyCheckedOperator,                                                 \
          from__String(op_name),                                               \
          init__Vec(                                                           \
            3,                                                                 \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CINT, NULL),  \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CINT, NULL),  \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UNIT, NULL)), \
          true);                                                               \
                                                                               \
    default_operators[idx++] =                                                 \
      NEW(LilyCheckedOperator,                                                 \
          from__String(op_name),                                               \
          init__Vec(                                                           \
            3,                                                                 \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CUINT, NULL), \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CUINT, NULL), \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UNIT, NULL)), \
          true);                                                               \
                                                                               \
    default_operators[idx++] =                                                 \
      NEW(LilyCheckedOperator,                                                 \
          from__String(op_name),                                               \
          init__Vec(                                                           \
            3,                                                                 \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CLONG, NULL), \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CLONG, NULL), \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UNIT, NULL)), \
          true);                                                               \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CULONG, NULL),    \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CULONG, NULL),    \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UNIT, NULL)),     \
      true);                                                                   \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CLONGLONG, NULL), \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CLONGLONG, NULL), \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UNIT, NULL)),     \
      true);                                                                   \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(                                                                   \
          LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CULONGLONG, NULL),  \
        NEW(                                                                   \
          LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CULONGLONG, NULL),  \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UNIT, NULL)),     \
      true);                                                                   \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CFLOAT, NULL),    \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CFLOAT, NULL),    \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UNIT, NULL)),     \
      true);                                                                   \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CDOUBLE, NULL),   \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CDOUBLE, NULL),   \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UNIT, NULL)),     \
      true);                                                                   \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_FLOAT32, NULL),   \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_FLOAT32, NULL),   \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UNIT, NULL)),     \
      true);                                                                   \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_FLOAT64, NULL),   \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_FLOAT64, NULL),   \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UNIT, NULL)),     \
      true);                                                                   \
                                                                               \
    default_operators[idx++] =                                                 \
      NEW(LilyCheckedOperator,                                                 \
          from__String(op_name),                                               \
          init__Vec(                                                           \
            3,                                                                 \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_INT8, NULL),  \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_INT8, NULL),  \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UNIT, NULL)), \
          true);                                                               \
                                                                               \
    default_operators[idx++] =                                                 \
      NEW(LilyCheckedOperator,                                                 \
          from__String(op_name),                                               \
          init__Vec(                                                           \
            3,                                                                 \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_INT16, NULL), \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_INT16, NULL), \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UNIT, NULL)), \
          true);                                                               \
                                                                               \
    default_operators[idx++] =                                                 \
      NEW(LilyCheckedOperator,                                                 \
          from__String(op_name),                                               \
          init__Vec(                                                           \
            3,                                                                 \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_INT32, NULL), \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_INT32, NULL), \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UNIT, NULL)), \
          true);                                                               \
                                                                               \
    default_operators[idx++] =                                                 \
      NEW(LilyCheckedOperator,                                                 \
          from__String(op_name),                                               \
          init__Vec(                                                           \
            3,                                                                 \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_INT64, NULL), \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_INT64, NULL), \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UNIT, NULL)), \
          true);                                                               \
                                                                               \
    default_operators[idx++] =                                                 \
      NEW(LilyCheckedOperator,                                                 \
          from__String(op_name),                                               \
          init__Vec(                                                           \
            3,                                                                 \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_ISIZE, NULL), \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_ISIZE, NULL), \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UNIT, NULL)), \
          true);                                                               \
                                                                               \
    default_operators[idx++] =                                                 \
      NEW(LilyCheckedOperator,                                                 \
          from__String(op_name),                                               \
          init__Vec(                                                           \
            3,                                                                 \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UINT8, NULL), \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UINT8, NULL), \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UNIT, NULL)), \
          true);                                                               \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UINT16, NULL),    \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UINT16, NULL),    \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UNIT, NULL)),     \
      true);                                                                   \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UINT32, NULL),    \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UINT32, NULL),    \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UNIT, NULL)),     \
      true);                                                                   \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UINT64, NULL),    \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UINT64, NULL),    \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UNIT, NULL)),     \
      true);                                                                   \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UINT64, NULL),    \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UINT64, NULL),    \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UNIT, NULL)),     \
      true);                                                                   \
                                                                               \
    default_operators[idx++] =                                                 \
      NEW(LilyCheckedOperator,                                                 \
          from__String(op_name),                                               \
          init__Vec(                                                           \
            3,                                                                 \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_USIZE, NULL), \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_USIZE, NULL), \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UNIT, NULL)), \
          true);

#define BASIC_LOGICAL_BINARY_OPERATOR(op_name)                                 \
    default_operators[idx++] =                                                 \
      NEW(LilyCheckedOperator,                                                 \
          from__String(op_name),                                               \
          init__Vec(                                                           \
            3,                                                                 \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_BOOL, NULL),  \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_BOOL, NULL),  \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_BOOL, NULL)), \
          true);

#define BASIC_COMPARISON_BINARY_OPERATOR(op_name)                              \
    default_operators[idx++] =                                                 \
      NEW(LilyCheckedOperator,                                                 \
          from__String(op_name),                                               \
          init__Vec(                                                           \
            3,                                                                 \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_BOOL, NULL),  \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_BOOL, NULL),  \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_BOOL, NULL)), \
          true);                                                               \
                                                                               \
    default_operators[idx++] =                                                 \
      NEW(LilyCheckedOperator,                                                 \
          from__String(op_name),                                               \
          init__Vec(                                                           \
            3,                                                                 \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_BYTE, NULL),  \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_BYTE, NULL),  \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_BOOL, NULL)), \
          true);                                                               \
                                                                               \
    default_operators[idx++] =                                                 \
      NEW(LilyCheckedOperator,                                                 \
          from__String(op_name),                                               \
          init__Vec(                                                           \
            3,                                                                 \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CHAR, NULL),  \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CHAR, NULL),  \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_BOOL, NULL)), \
          true);                                                               \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CSHORT, NULL),    \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CSHORT, NULL),    \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_BOOL, NULL)),     \
      true);                                                                   \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CUSHORT, NULL),   \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CUSHORT, NULL),   \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_BOOL, NULL)),     \
      true);                                                                   \
                                                                               \
    default_operators[idx++] =                                                 \
      NEW(LilyCheckedOperator,                                                 \
          from__String(op_name),                                               \
          init__Vec(                                                           \
            3,                                                                 \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CINT, NULL),  \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CINT, NULL),  \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_BOOL, NULL)), \
          true);                                                               \
                                                                               \
    default_operators[idx++] =                                                 \
      NEW(LilyCheckedOperator,                                                 \
          from__String(op_name),                                               \
          init__Vec(                                                           \
            3,                                                                 \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CUINT, NULL), \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CUINT, NULL), \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_BOOL, NULL)), \
          true);                                                               \
                                                                               \
    default_operators[idx++] =                                                 \
      NEW(LilyCheckedOperator,                                                 \
          from__String(op_name),                                               \
          init__Vec(                                                           \
            3,                                                                 \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CLONG, NULL), \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CLONG, NULL), \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_BOOL, NULL)), \
          true);                                                               \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CULONG, NULL),    \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CULONG, NULL),    \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_BOOL, NULL)),     \
      true);                                                                   \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CLONGLONG, NULL), \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CLONGLONG, NULL), \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_BOOL, NULL)),     \
      true);                                                                   \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(                                                                   \
          LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CULONGLONG, NULL),  \
        NEW(                                                                   \
          LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CULONGLONG, NULL),  \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_BOOL, NULL)),     \
      true);                                                                   \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CFLOAT, NULL),    \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CFLOAT, NULL),    \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_BOOL, NULL)),     \
      true);                                                                   \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CDOUBLE, NULL),   \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_CDOUBLE, NULL),   \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_BOOL, NULL)),     \
      true);                                                                   \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_FLOAT32, NULL),   \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_FLOAT32, NULL),   \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_BOOL, NULL)),     \
      true);                                                                   \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_FLOAT64, NULL),   \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_FLOAT64, NULL),   \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_BOOL, NULL)),     \
      true);                                                                   \
                                                                               \
    default_operators[idx++] =                                                 \
      NEW(LilyCheckedOperator,                                                 \
          from__String(op_name),                                               \
          init__Vec(                                                           \
            3,                                                                 \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_INT16, NULL), \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_INT16, NULL), \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_BOOL, NULL)), \
          true);                                                               \
                                                                               \
    default_operators[idx++] =                                                 \
      NEW(LilyCheckedOperator,                                                 \
          from__String(op_name),                                               \
          init__Vec(                                                           \
            3,                                                                 \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_INT32, NULL), \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_INT32, NULL), \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_BOOL, NULL)), \
          true);                                                               \
                                                                               \
    default_operators[idx++] =                                                 \
      NEW(LilyCheckedOperator,                                                 \
          from__String(op_name),                                               \
          init__Vec(                                                           \
            3,                                                                 \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_INT64, NULL), \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_INT64, NULL), \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_BOOL, NULL)), \
          true);                                                               \
                                                                               \
    default_operators[idx++] =                                                 \
      NEW(LilyCheckedOperator,                                                 \
          from__String(op_name),                                               \
          init__Vec(                                                           \
            3,                                                                 \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_INT8, NULL),  \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_INT8, NULL),  \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_BOOL, NULL)), \
          true);                                                               \
                                                                               \
    default_operators[idx++] =                                                 \
      NEW(LilyCheckedOperator,                                                 \
          from__String(op_name),                                               \
          init__Vec(                                                           \
            3,                                                                 \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_ISIZE, NULL), \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_ISIZE, NULL), \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_BOOL, NULL)), \
          true);                                                               \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UINT16, NULL),    \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UINT16, NULL),    \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_BOOL, NULL)),     \
      true);                                                                   \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UINT32, NULL),    \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UINT32, NULL),    \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_BOOL, NULL)),     \
      true);                                                                   \
                                                                               \
    default_operators[idx++] = NEW(                                            \
      LilyCheckedOperator,                                                     \
      from__String(op_name),                                                   \
      init__Vec(                                                               \
        3,                                                                     \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UINT64, NULL),    \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UINT64, NULL),    \
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_BOOL, NULL)),     \
      true);                                                                   \
                                                                               \
    default_operators[idx++] =                                                 \
      NEW(LilyCheckedOperator,                                                 \
          from__String(op_name),                                               \
          init__Vec(                                                           \
            3,                                                                 \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UINT8, NULL), \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UINT8, NULL), \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_BOOL, NULL)), \
          true);                                                               \
                                                                               \
    default_operators[idx++] =                                                 \
      NEW(LilyCheckedOperator,                                                 \
          from__String(op_name),                                               \
          init__Vec(                                                           \
            3,                                                                 \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_USIZE, NULL), \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_USIZE, NULL), \
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_BOOL, NULL)), \
          true);

LilyCheckedOperator **
load_default_operators__LilyCheckedOperator()
{
    LilyCheckedOperator **default_operators =
      lily_malloc(sizeof(PTR_SIZE) * DEFAULT_OPERATORS_COUNT);
    Usize idx = 0;

    BASIC_ARITHMETIC_BINARY_OPERATOR("+");
    BASIC_LOGICAL_BINARY_OPERATOR("and");
    BASIC_ASSIGN_BINARY_OPERATOR("+=");
    BASIC_BITWISE_ASSIGN_BINARY_OPERATOR("&=");
    BASIC_BITWISE_ASSIGN_BINARY_OPERATOR("<<=");
    BASIC_BITWISE_ASSIGN_BINARY_OPERATOR("|=");
    BASIC_BITWISE_ASSIGN_BINARY_OPERATOR(">>=");
    BASIC_ASSIGN_BINARY_OPERATOR("/=");
    BASIC_ASSIGN_BINARY_OPERATOR("**=");
    BASIC_ASSIGN_BINARY_OPERATOR("%=");
    BASIC_ASSIGN_BINARY_OPERATOR("*=");
    BASIC_ASSIGN_BINARY_OPERATOR("-=");
    BASIC_BITWISE_ASSIGN_BINARY_OPERATOR("xor=");
    BASIC_BITWISE_BINARY_OPERATOR("&");
    BASIC_BITWISE_BINARY_OPERATOR("|");
    BASIC_ARITHMETIC_BINARY_OPERATOR("/");
    BASIC_COMPARISON_BINARY_OPERATOR("==");
    BASIC_ARITHMETIC_BINARY_OPERATOR("**");
    BASIC_COMPARISON_BINARY_OPERATOR(">=");
    BASIC_COMPARISON_BINARY_OPERATOR(">");
    BASIC_BITWISE_BINARY_OPERATOR("<<");
    BASIC_COMPARISON_BINARY_OPERATOR("<=");
    BASIC_COMPARISON_BINARY_OPERATOR("<");
    BASIC_ARITHMETIC_BINARY_OPERATOR("%");
    BASIC_ARITHMETIC_BINARY_OPERATOR("*");
    BASIC_COMPARISON_BINARY_OPERATOR("not=");
    BASIC_LOGICAL_BINARY_OPERATOR("or");
    BASIC_BITWISE_BINARY_OPERATOR(">>");
    BASIC_ARITHMETIC_BINARY_OPERATOR("-");
    BASIC_BITWISE_BINARY_OPERATOR("xor");

    default_operators[idx++] =
      NEW(LilyCheckedOperator,
          from__String(".*"),
          init__Vec(
            2,
            NEW_VARIANT(
              LilyCheckedDataType,
              ptr,
              NULL,
              NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_ANY, NULL)),
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_ANY, NULL)),
          true);

    default_operators[idx++] =
      NEW(LilyCheckedOperator,
          from__String(".*"),
          init__Vec(
            2,
            NEW_VARIANT(
              LilyCheckedDataType,
              ref,
              NULL,
              NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_ANY, NULL)),
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_ANY, NULL)),
          true);

    default_operators[idx++] =
      NEW(LilyCheckedOperator,
          from__String("-"),
          init__Vec(
            2,
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_INT16, NULL),
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_INT16, NULL)),
          true);

    default_operators[idx++] =
      NEW(LilyCheckedOperator,
          from__String("-"),
          init__Vec(
            2,
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_INT32, NULL),
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_INT32, NULL)),
          true);

    default_operators[idx++] =
      NEW(LilyCheckedOperator,
          from__String("-"),
          init__Vec(
            2,
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_INT64, NULL),
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_INT64, NULL)),
          true);

    default_operators[idx++] =
      NEW(LilyCheckedOperator,
          from__String("-"),
          init__Vec(
            2,
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_INT8, NULL),
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_INT8, NULL)),
          true);

    default_operators[idx++] =
      NEW(LilyCheckedOperator,
          from__String("-"),
          init__Vec(
            2,
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_ISIZE, NULL),
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_ISIZE, NULL)),
          true);

    default_operators[idx++] = NEW(
      LilyCheckedOperator,
      from__String("-"),
      init__Vec(
        2,
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_FLOAT32, NULL),
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_FLOAT32, NULL)),
      true);

    default_operators[idx++] = NEW(
      LilyCheckedOperator,
      from__String("-"),
      init__Vec(
        2,
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_FLOAT64, NULL),
        NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_FLOAT64, NULL)),
      true);

    default_operators[idx++] =
      NEW(LilyCheckedOperator,
          from__String("not"),
          init__Vec(
            2,
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_BOOL, NULL),
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_BOOL, NULL)),
          true);

    default_operators[idx++] =
      NEW(LilyCheckedOperator,
          from__String("ref"),
          init__Vec(
            2,
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_ANY, NULL),
            NEW_VARIANT(
              LilyCheckedDataType,
              ptr,
              NULL,
              NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_ANY, NULL))),
          true);

    default_operators[idx++] =
      NEW(LilyCheckedOperator,
          from__String("ref"),
          init__Vec(
            2,
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_ANY, NULL),
            NEW_VARIANT(
              LilyCheckedDataType,
              ref,
              NULL,
              NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_ANY, NULL))),
          true);

    default_operators[idx++] =
      NEW(LilyCheckedOperator,
          from__String("ref mut"),
          init__Vec(
            2,
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_ANY, NULL),
            NEW_VARIANT(
              LilyCheckedDataType,
              ptr_mut,
              NULL,
              NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_ANY, NULL))),
          true);

    default_operators[idx++] =
      NEW(LilyCheckedOperator,
          from__String("ref mut"),
          init__Vec(
            2,
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_ANY, NULL),
            NEW_VARIANT(
              LilyCheckedDataType,
              ref_mut,
              NULL,
              NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_ANY, NULL))),
          true);

    default_operators[idx++] =
      NEW(LilyCheckedOperator,
          from__String("trace"),
          init__Vec(
            2,
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_ANY, NULL),
            NEW_VARIANT(
              LilyCheckedDataType,
              trace,
              NULL,
              NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_ANY, NULL))),
          true);

    default_operators[idx++] =
      NEW(LilyCheckedOperator,
          from__String("trace mut"),
          init__Vec(
            2,
            NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_ANY, NULL),
            NEW_VARIANT(
              LilyCheckedDataType,
              trace_mut,
              NULL,
              NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_ANY, NULL))),
          true);

    return default_operators;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyCheckedOperator, const LilyCheckedOperator *self)
{
    String *res =
      format__String("LilyCheckedOperator{{ name = {S}", self->name);

    DEBUG_VEC_STRING(self->signature, res, LilyCheckedDataType);
    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(LilyCheckedOperator, LilyCheckedOperator *self)
{
    if (self->ref_count > 0) {
        --self->ref_count;

        return;
    }

    if (self->is_default) {
        FREE(String, self->name);
        FREE_BUFFER_ITEMS(
          self->signature->buffer, self->signature->len, LilyCheckedDataType);
        FREE(Vec, self->signature);
    }

    lily_free(self);
}
