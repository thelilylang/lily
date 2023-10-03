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

#include <base/macros.h>
#include <base/new.h>
#include <base/types.h>

#include <core/lily/analysis/checked/limits.h>

#include <stddef.h>

typedef struct LilyInterpreterValue LilyInterpreterValue;

// TODO: Remove all objects definitions, when we have pseudo generic support to
// the base library.

typedef struct LilyInterpreterValueDynamicArray
{
    LilyInterpreterValue *buffer;
    Usize len;
} LilyInterpreterValueDynamicArray;

/**
 *
 * @brief Construct LilyInterpreterValueDynamicArray type.
 */
inline CONSTRUCTOR(LilyInterpreterValueDynamicArray,
                   LilyInterpreterValueDynamicArray)
{
    return (LilyInterpreterValueDynamicArray){
        .buffer = NULL,
        .len = 0,
    };
}

typedef struct LilyInterpreterValueMultiPointersArray
{
    LilyInterpreterValue *buffer;
    Usize len;
} LilyInterpreterValueMultiPointersArray;

/**
 *
 * @brief Construct LilyInterpreterValueMultiPointersArray type.
 */
inline CONSTRUCTOR(LilyInterpreterValueMultiPointersArray,
                   LilyInterpreterValueMultiPointersArray)
{
    return (LilyInterpreterValueMultiPointersArray){ .buffer = NULL, .len = 0 };
}

typedef struct LilyInterpreterValueSizedArray
{
    const LilyInterpreterValue *buffer;
    Usize len;
} LilyInterpreterValueSizedArray;

/**
 *
 * @brief Construct LilyInterpreterValueSizedArray type.
 */
inline CONSTRUCTOR(LilyInterpreterValueSizedArray,
                   LilyInterpreterValueSizedArray,
                   const LilyInterpreterValue *buffer,
                   Usize len)
{
    return (LilyInterpreterValueSizedArray){ .buffer = buffer, .len = len };
}

typedef struct LilyInterpreterValueBytes
{
    Uint8 *buffer;
    Usize len;
} LilyInterpreterValueBytes;

/**
 *
 * @brief Construct LilyInterpreterValueBytes type.
 */
inline CONSTRUCTOR(LilyInterpreterValueBytes,
                   LilyInterpreterValueBytes,
                   Uint8 *buffer,
                   Usize len)
{
    return (LilyInterpreterValueBytes){ .buffer = buffer, .len = len };
}

typedef struct LilyInterpreterValueInstance
{
    const char *name; // const char* (&)
    struct LilyInterpreterValue *value;
} LilyInterpreterValueInstance;

/**
 *
 * @brief Construct LilyInterpreterValueInstance type.
 */
inline CONSTRUCTOR(LilyInterpreterValueInstance,
                   LilyInterpreterValueInstance,
                   const char *name,
                   struct LilyInterpreterValue *value)
{
    return (LilyInterpreterValueInstance){ .name = name, .value = value };
}

/**
 *
 * @brief Free LilyInterpreterValueInstance type.
 */
DESTRUCTOR(LilyInterpreterValueInstance,
           const LilyInterpreterValueInstance *self);

typedef struct LilyInterpreterValueListNode
{
    LilyInterpreterValue *value;
    struct LilyInterpreterValueListNode *next;
} LilyInterpreterValueListNode;

/**
 *
 * @brief Construct LilyInterpreterValueListNode type.
 */
CONSTRUCTOR(LilyInterpreterValueListNode *,
            LilyInterpreterValueListNode,
            LilyInterpreterValue *value,
            struct LilyInterpreterValueListNode *next);

/**
 *
 * @brief Free LilyInterpreterValueListNode type.
 */
DESTRUCTOR(LilyInterpreterValueListNode, LilyInterpreterValueListNode *self);

typedef struct LilyInterpreterValueList
{
    LilyInterpreterValueListNode *first;
    struct LilyInterpreterValueListNode *last;
} LilyInterpreterValueList;

/**
 *
 * @brief Construct LilyInterpreterValueList type.
 */
inline CONSTRUCTOR(LilyInterpreterValueList,
                   LilyInterpreterValueList,
                   LilyInterpreterValueListNode *first,
                   LilyInterpreterValueListNode *last)
{
    return (LilyInterpreterValueList){
        .first = first,
        .last = last,
    };
}

/**
 *
 * @brief Free LilyInterpreterValueList type.
 */
inline DESTRUCTOR(LilyInterpreterValueList,
                  const LilyInterpreterValueList *self)
{
    FREE(LilyInterpreterValueListNode, self->first);
}

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

/**
 *
 * @brief Construct LilyInterpreterValueResult type
 * (LILY_INTERPRETER_VALUE_RESULT_KIND_OK).
 */
VARIANT_CONSTRUCTOR(LilyInterpreterValueResult,
                    LilyInterpreterValueResult,
                    ok,
                    LilyInterpreterValue *ok)
{
    return (LilyInterpreterValueResult){
        .kind = LILY_INTERPRETER_VALUE_RESULT_KIND_OK, .ok = ok
    };
}

/**
 *
 * @brief Construct LilyInterpreterValueResult type
 * (LILY_INTERPRETER_VALUE_RESULT_KIND_ERR).
 */
VARIANT_CONSTRUCTOR(LilyInterpreterValueResult,
                    LilyInterpreterValueResult,
                    err,
                    LilyInterpreterValue *err)
{
    return (LilyInterpreterValueResult){
        .kind = LILY_INTERPRETER_VALUE_RESULT_KIND_ERR, .err = err
    };
}

/**
 *
 * @brief Free LilyInterpreterValueResult type.
 */
DESTRUCTOR(LilyInterpreterValueResult, const LilyInterpreterValueResult *self);

typedef struct LilyInterpreterValueStr
{
    char *s;
    Usize len;
} LilyInterpreterValueStr;

/**
 *
 * @brief Construct LilyInterpreterValueStr type
 */
CONSTRUCTOR(LilyInterpreterValueStr,
            LilyInterpreterValueStr,
            char *s,
            Usize len)
{
    return (LilyInterpreterValueStr){ .s = s, .len = len };
}

typedef struct LilyInterpreterValueStruct
{
    LilyInterpreterValue *values[MAX_RECORD_FIELDS];
    Usize len;
} LilyInterpreterValueStruct;

/**
 *
 * @brief Construct LilyInterpreterValueStruct type.
 */
CONSTRUCTOR(LilyInterpreterValueStruct,
            LilyInterpreterValueStruct,
            LilyInterpreterValue **values,
            Usize len);

/**
 *
 * @brief Free LilyInterpreterValueStruct type.
 */
DESTRUCTOR(LilyInterpreterValueStruct, const LilyInterpreterValueStruct *self);

enum LilyInterpreterValueObjectKind
{
    LILY_INTERPRETER_VALUE_OBJECT_KIND_DYNAMIC_ARRAY,
    LILY_INTERPRETER_VALUE_OBJECT_KIND_MULTI_POINTERS_ARRAY,
    LILY_INTERPRETER_VALUE_OBJECT_KIND_SIZED_ARRAY,
    LILY_INTERPRETER_VALUE_OBJECT_KIND_BYTES,
    LILY_INTERPRETER_VALUE_OBJECT_KIND_CSTR,
    LILY_INTERPRETER_VALUE_OBJECT_KIND_INSTANCE,
    LILY_INTERPRETER_VALUE_OBJECT_KIND_LIST,
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
        LilyInterpreterValueInstance instance;
        LilyInterpreterValueList list;
        LilyInterpreterValueResult result;
        LilyInterpreterValueStr str;
        LilyInterpreterValueStruct struct_;
    };
} LilyInterpreterValueObject;

/**
 *
 * @brief Construct LilyInterpreterValueObject type
 * (LILY_INTERPRETER_VALUE_OBJECT_KIND_DYNAMIC_ARRAY).
 */
inline VARIANT_CONSTRUCTOR(LilyInterpreterValueObject,
                           LilyInterpreterValueObject,
                           dynamic_array,
                           LilyInterpreterValueDynamicArray dynamic_array)
{
    return (LilyInterpreterValueObject){
        .kind = LILY_INTERPRETER_VALUE_OBJECT_KIND_DYNAMIC_ARRAY,
        .dynamic_array = dynamic_array
    };
}

/**
 *
 * @brief Construct LilyInterpreterValueObject type
 * (LILY_INTERPRETER_VALUE_OBJECT_KIND_MULTI_POINTERS_ARRAY).
 */
inline VARIANT_CONSTRUCTOR(
  LilyInterpreterValueObject,
  LilyInterpreterValueObject,
  multi_pointers_array,
  LilyInterpreterValueMultiPointersArray multi_pointers_array)
{
    return (LilyInterpreterValueObject){
        .kind = LILY_INTERPRETER_VALUE_OBJECT_KIND_MULTI_POINTERS_ARRAY,
        .multi_pointers_array = multi_pointers_array
    };
}

/**
 *
 * @brief Construct LilyInterpreterValueObject type
 * (LILY_INTERPRETER_VALUE_OBJECT_KIND_SIZED_ARRAY).
 */
inline VARIANT_CONSTRUCTOR(LilyInterpreterValueObject,
                           LilyInterpreterValueObject,
                           sized_array,
                           LilyInterpreterValueSizedArray sized_array)
{
    return (LilyInterpreterValueObject){
        .kind = LILY_INTERPRETER_VALUE_OBJECT_KIND_SIZED_ARRAY,
        .sized_array = sized_array
    };
}

/**
 *
 * @brief Construct LilyInterpreterValueObject type
 * (LILY_INTERPRETER_VALUE_OBJECT_KIND_BYTES).
 */
inline VARIANT_CONSTRUCTOR(LilyInterpreterValueObject,
                           LilyInterpreterValueObject,
                           bytes,
                           LilyInterpreterValueBytes bytes)
{
    return (LilyInterpreterValueObject){
        .kind = LILY_INTERPRETER_VALUE_OBJECT_KIND_BYTES, .bytes = bytes
    };
}

/**
 *
 * @brief Construct LilyInterpreterValueObject type
 * (LILY_INTERPRETER_VALUE_OBJECT_KIND_CSTR).
 */
inline VARIANT_CONSTRUCTOR(LilyInterpreterValueObject,
                           LilyInterpreterValueObject,
                           cstr,
                           char *cstr)
{
    return (LilyInterpreterValueObject){
        .kind = LILY_INTERPRETER_VALUE_OBJECT_KIND_CSTR, .cstr = cstr
    };
}

/**
 *
 * @brief Construct LilyInterpreterValueObject type
 * (LILY_INTERPRETER_VALUE_OBJECT_KIND_INSTANCE).
 */
inline VARIANT_CONSTRUCTOR(LilyInterpreterValueObject,
                           LilyInterpreterValueObject,
                           instance,
                           LilyInterpreterValueInstance instance)
{
    return (LilyInterpreterValueObject){
        .kind = LILY_INTERPRETER_VALUE_OBJECT_KIND_INSTANCE,
        .instance = instance
    };
}

/**
 *
 * @brief Construct LilyInterpreterValueObject type
 * (LILY_INTERPRETER_VALUE_OBJECT_KIND_LIST).
 */
inline VARIANT_CONSTRUCTOR(LilyInterpreterValueObject,
                           LilyInterpreterValueObject,
                           list,
                           LilyInterpreterValueList list)
{
    return (LilyInterpreterValueObject){
        .kind = LILY_INTERPRETER_VALUE_OBJECT_KIND_LIST, .list = list
    };
}

/**
 *
 * @brief Construct LilyInterpreterValueObject type
 * (LILY_INTERPRETER_VALUE_OBJECT_KIND_RESULT).
 */
inline VARIANT_CONSTRUCTOR(LilyInterpreterValueObject,
                           LilyInterpreterValueObject,
                           result,
                           LilyInterpreterValueResult result)
{
    return (LilyInterpreterValueObject){
        .kind = LILY_INTERPRETER_VALUE_OBJECT_KIND_RESULT, .result = result
    };
}

/**
 *
 * @brief Construct LilyInterpreterValueObject type
 * (LILY_INTERPRETER_VALUE_OBJECT_KIND_STR).
 */
inline VARIANT_CONSTRUCTOR(LilyInterpreterValueObject,
                           LilyInterpreterValueObject,
                           str,
                           LilyInterpreterValueStr str)
{
    return (LilyInterpreterValueObject){
        .kind = LILY_INTERPRETER_VALUE_OBJECT_KIND_STR, .str = str
    };
}

/**
 *
 * @brief Construct LilyInterpreterValueObject type
 * (LILY_INTERPRETER_VALUE_OBJECT_KIND_STRUCT).
 */
inline VARIANT_CONSTRUCTOR(LilyInterpreterValueObject,
                           LilyInterpreterValueObject,
                           struct,
                           LilyInterpreterValueStruct struct_)
{
    return (LilyInterpreterValueObject){
        .kind = LILY_INTERPRETER_VALUE_OBJECT_KIND_STRUCT, .struct_ = struct_
    };
}

/**
 *
 * @brief Free LilyInterpreterValueObject type.
 */
DESTRUCTOR(LilyInterpreterValueObject, const LilyInterpreterValueObject *self);

enum LilyInterpreterValueKind
{
    LILY_INTERPRETER_VALUE_KIND_INT,
    LILY_INTERPRETER_VALUE_KIND_FALSE,
    LILY_INTERPRETER_VALUE_KIND_FLOAT,
    LILY_INTERPRETER_VALUE_KIND_NIL,
    LILY_INTERPRETER_VALUE_KIND_OBJECT,
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
        LilyInterpreterValueObject object;
        Uint64 uint;
    };
};

/**
 *
 * @brief Construct LilyInterpreterValue (LILY_INTERPRETER_VALUE_KIND_INT).
 */
inline VARIANT_CONSTRUCTOR(LilyInterpreterValue,
                           LilyInterpreterValue,
                           int,
                           Int64 int_)
{
    return (LilyInterpreterValue){ .kind = LILY_INTERPRETER_VALUE_KIND_INT,
                                   .int_ = int_ };
}

/**
 *
 * @brief Construct LilyInterpreterValue (LILY_INTERPRETER_VALUE_KIND_FLOAT).
 */
inline VARIANT_CONSTRUCTOR(LilyInterpreterValue,
                           LilyInterpreterValue,
                           float,
                           Float64 float_)
{
    return (LilyInterpreterValue){ .kind = LILY_INTERPRETER_VALUE_KIND_FLOAT,
                                   .float_ = float_ };
}

/**
 *
 * @brief Construct LilyInterpreterValue (LILY_INTERPRETER_VALUE_KIND_OBJECT).
 */
inline VARIANT_CONSTRUCTOR(LilyInterpreterValue,
                           LilyInterpreterValue,
                           object,
                           LilyInterpreterValueObject object)
{
    return (LilyInterpreterValue){ .kind = LILY_INTERPRETER_VALUE_KIND_OBJECT,
                                   .object = object };
}

/**
 *
 * @brief Construct LilyInterpreterValue (LILY_INTERPRETER_VALUE_KIND_UINT).
 */
inline VARIANT_CONSTRUCTOR(LilyInterpreterValue,
                           LilyInterpreterValue,
                           uint,
                           Uint64 uint)
{
    return (LilyInterpreterValue){ .kind = LILY_INTERPRETER_VALUE_KIND_UINT,
                                   .uint = uint };
}

/**
 *
 * @brief Construct LilyInterpreterValue.
 */
inline CONSTRUCTOR(LilyInterpreterValue,
                   LilyInterpreterValue,
                   enum LilyInterpreterValueKind kind)
{
    return (LilyInterpreterValue){
        .kind = kind,
    };
}

/**
 *
 * @brief Free LilyInterpreterValue type.
 */
DESTRUCTOR(LilyInterpreterValue, const LilyInterpreterValue *self);

#endif // LILY_CORE_LILY_INTERPRETER_VALUE_H
