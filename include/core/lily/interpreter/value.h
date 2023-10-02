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

#ifndef LILY_CORE_LILY_INTERPRETER_VALUE_H
#define LILY_CORE_LILY_INTERPRETER_VALUE_H

#include <base/types.h>

#include <core/lily/analysis/checked/limits.h>

typedef struct LilyInterpreterValue LilyInterpreterValue;

// TODO: Remove all objects definitions, when we have pseudo generic support to
// the base library.

typedef struct LilyInterpreterValueDynamicArray
{
    LilyInterpreterValue *buffer;
    Usize len;
    Usize capacity;
} LilyInterpreterValueDynamicArray;

typedef struct LilyInterpreterValueMultiPointersArray
{
    LilyInterpreterValue *buffer;
    Usize len;
} LilyInterpreterValueMultiPointersArray;

typedef struct LilyInterpreterValueSizedArray
{
    const LilyInterpreterValue *buffer;
    Usize len;
} LilyInterpreterValueSizedArray;

typedef struct LilyInterpreterValueBytes
{
    Uint8 *buffer;
    Usize len;
} LilyInterpreterValueBytes;

typedef struct LilyInterpreterValueList
{
    LilyInterpreterValue *value;
    struct LilyInterpreterValueList *next;
    struct LilyInterpreterValueList *last;
} LilyInterpreterValueList;

enum LilyInterpreterValueResultKind
{
    LILY_INTERPRETER_VALUE_RESULT_KIND_OK,
    LILY_INTERPRETER_VALUE_RESULT_KIND_ERR,
};

typedef struct LilyInterpreterValueResult
{
    enum LilyInterpreterValueResultKind kind;
    union
    {
        LilyInterpreterValue *ok;
        LilyInterpreterValue *err;
    };
} LilyInterpreterValueResult;

typedef struct LilyInterpreterValueStr
{
    char *s;
    Usize len;
} LilyInterpreterValueStr;

typedef struct LilyInterpreterValueStruct
{
    LilyInterpreterValue *values[MAX_RECORD_FIELDS];
    Usize len;
} LilyInterpreterValueStruct;

enum LilyInterpreterValueObjectKind
{
    LILY_INTERPRETER_VALUE_OBJECT_KIND_DYNAMIC_ARRAY,
    LILY_INTERPRETER_VALUE_OBJECT_KIND_MULTI_POINTERS_ARRAY,
    LILY_INTERPRETER_VALUE_OBJECT_KIND_SIZED_ARRAY,
    LILY_INTERPRETER_VALUE_OBJECT_KIND_BYTES,
    LILY_INTERPRETER_VALUE_OBJECT_KIND_CSTR,
    LILY_INTERPRETER_VALUE_OBJECT_KIND_INSTANCE,
    LILY_INTERPRETER_VALUE_OBJECT_KIND_LIST,
    LILY_INTERPRETER_VALUE_OBJECT_KIND_PTR,
    LILY_INTERPRETER_VALUE_OBJECT_KIND_REF,
    LILY_INTERPRETER_VALUE_OBJECT_KIND_RESULT,
    LILY_INTERPRETER_VALUE_OBJECT_KIND_STR,
    LILY_INTERPRETER_VALUE_OBJECT_KIND_STRUCT,
};

typedef struct LilyInterpreterValueObject
{
    enum LilyInterpreterValueObjectKind kind;
    union
    {
        LilyInterpreterValueDynamicArray dynamic_array;
        LilyInterpreterValueMultiPointersArray multi_pointers_array;
        LilyInterpreterValueSizedArray sized_array;
        LilyInterpreterValueBytes bytes;
        char *cstr;
        struct LilyInterpreterValueObject *instance;
        LilyInterpreterValueList list;
        struct LilyInterpreterValue *ptr;
        struct LilyInterpreterValue *ref; // struct LilyInterpreterValue* (&)
        LilyInterpreterValueResult result;
        LilyInterpreterValueStr str;
        LilyInterpreterValueStruct struct_;
    };
} LilyInterpreterValueObject;

enum LilyInterpreterValueKind
{
    LILY_INTERPRETER_VALUE_KIND_INT,
    LILY_INTERPRETER_VALUE_KIND_FALSE,
    LILY_INTERPRETER_VALUE_KIND_FLOAT,
    LILY_INTERPRETER_VALUE_KIND_NIL,
    LILY_INTERPRETER_VALUE_KIND_OBJ,
    LILY_INTERPRETER_VALUE_KIND_TRUE,
    LILY_INTERPRETER_VALUE_KIND_UNDEF,
    LILY_INTERPRETER_VALUE_KIND_UINT,
    LILY_INTERPRETER_VALUE_KIND_UNIT,
};

struct LilyInterpreterValue
{
    enum LilyInterpreterValueKind kind;
    union
    {
        Int64 int_;
        Float64 float_;
        LilyInterpreterValueObject *object;
        Uint64 uint;
    };
};

#endif // LILY_CORE_LILY_INTERPRETER_VALUE_H
