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

#ifndef LILY_CORE_LILY_INTERPRETER_VALUE_H
#define LILY_CORE_LILY_INTERPRETER_VALUE_H

#include <base/alloc.h>
#include <base/macros.h>
#include <base/new.h>
#include <base/types.h>

#include <core/lily/analysis/checked/limits.h>

#include <stddef.h>

typedef struct LilyInterpreterValueBytes LilyInterpreterValueBytes;
typedef struct LilyInterpreterValueDynamicArray
  LilyInterpreterValueDynamicArray;
typedef struct LilyInterpreterValueList LilyInterpreterValueList;
typedef struct LilyInterpreterValueMultiPointersArray
  LilyInterpreterValueMultiPointersArray;
typedef struct LilyInterpreterValueResult LilyInterpreterValueResult;
typedef struct LilyInterpreterValueSizedArray LilyInterpreterValueSizedArray;
typedef struct LilyInterpreterValueStruct LilyInterpreterValueStruct;
typedef struct LilyInterpreterValueStr LilyInterpreterValueStr;
typedef struct LilyInterpreterValue LilyInterpreterValue;

// TODO: Remove LilyInterpreterValueBytes, LilyInterpreterValueDynamicArray,
// LilyInterpreterValueMultiPointersArray, LilyInterpreterValueList,
// LilyInterpreterValueResult, LilyInterpreterValueSizedArray,
// LilyInterpreterValueStruct, LilyInterpreterValueStr, when we have pseudo
// generic support to the base library.

enum LilyInterpreterValueKind
{
    LILY_INTERPRETER_VALUE_KIND_FALSE = 0,
    LILY_INTERPRETER_VALUE_KIND_TRUE = 1,
    LILY_INTERPRETER_VALUE_KIND_BYTES,
    LILY_INTERPRETER_VALUE_KIND_CSTR,
    LILY_INTERPRETER_VALUE_KIND_DESTROYED,
    LILY_INTERPRETER_VALUE_KIND_DYNAMIC_ARRAY,
    LILY_INTERPRETER_VALUE_KIND_INT8,
    LILY_INTERPRETER_VALUE_KIND_INT16,
    LILY_INTERPRETER_VALUE_KIND_INT32,
    LILY_INTERPRETER_VALUE_KIND_INT64,
    LILY_INTERPRETER_VALUE_KIND_ISIZE,
    LILY_INTERPRETER_VALUE_KIND_FLOAT,
    LILY_INTERPRETER_VALUE_KIND_LIST,
    LILY_INTERPRETER_VALUE_KIND_MULTI_POINTERS_ARRAY,
    LILY_INTERPRETER_VALUE_KIND_NIL,
    LILY_INTERPRETER_VALUE_KIND_PTR,
    LILY_INTERPRETER_VALUE_KIND_RESULT,
    LILY_INTERPRETER_VALUE_KIND_SIZED_ARRAY,
    LILY_INTERPRETER_VALUE_KIND_STR,
    LILY_INTERPRETER_VALUE_KIND_STRUCT,
    LILY_INTERPRETER_VALUE_KIND_UINT8,
    LILY_INTERPRETER_VALUE_KIND_UINT16,
    LILY_INTERPRETER_VALUE_KIND_UINT32,
    LILY_INTERPRETER_VALUE_KIND_UINT64,
    LILY_INTERPRETER_VALUE_KIND_USIZE,
    LILY_INTERPRETER_VALUE_KIND_UNDEF,
    LILY_INTERPRETER_VALUE_KIND_UNIT,
};

struct LilyInterpreterValue
{
    enum LilyInterpreterValueKind kind;
    Usize ref_count;
    union
    {
        struct LilyInterpreterValueBytes *bytes;
        char *cstr;
        struct LilyInterpreterValueDynamicArray *dynamic_array;
        Int8 int8;
        Int16 int16;
        Int32 int32;
        Int64 int64;
        Isize isize;
        Float64 float_;
        struct LilyInterpreterValueList *list;
        struct LilyInterpreterValueMultiPointersArray *multi_pointers_array;
        void *ptr;
        struct LilyInterpreterValueResult *result;
        struct LilyInterpreterValueSizedArray *sized_array;
        struct LilyInterpreterValueStr *str;
        struct LilyInterpreterValueStruct *struct_;
        Uint8 uint8;
        Uint16 uint16;
        Uint32 uint32;
        Uint64 uint64;
        Usize usize;
    };
};

/**
 *
 * @brief Construct LilyInterpreterValue (LILY_INTERPRETER_VALUE_KIND_BYTES).
 */
inline VARIANT_CONSTRUCTOR(LilyInterpreterValue,
                           LilyInterpreterValue,
                           bytes,
                           LilyInterpreterValueBytes *bytes)
{
    return (LilyInterpreterValue){ .kind = LILY_INTERPRETER_VALUE_KIND_BYTES,
                                   .ref_count = 0,
                                   .bytes = bytes };
}

/**
 *
 * @brief Construct LilyInterpreterValue (LILY_INTERPRETER_VALUE_KIND_CSTR).
 */
inline VARIANT_CONSTRUCTOR(LilyInterpreterValue,
                           LilyInterpreterValue,
                           cstr,
                           char *cstr)
{
    return (LilyInterpreterValue){ .kind = LILY_INTERPRETER_VALUE_KIND_CSTR,
                                   .ref_count = 0,
                                   .cstr = cstr };
}

/**
 *
 * @brief Construct LilyInterpreterValue
 * (LILY_INTERPRETER_VALUE_KIND_DYNAMIC_ARRAY).
 */
inline VARIANT_CONSTRUCTOR(LilyInterpreterValue,
                           LilyInterpreterValue,
                           dynamic_array,
                           LilyInterpreterValueDynamicArray *dynamic_array)
{
    return (LilyInterpreterValue){ .kind =
                                     LILY_INTERPRETER_VALUE_KIND_DYNAMIC_ARRAY,
                                   .ref_count = 0,
                                   .dynamic_array = dynamic_array };
}

/**
 *
 * @brief Construct LilyInterpreterValue (LILY_INTERPRETER_VALUE_KIND_INT8).
 */
inline VARIANT_CONSTRUCTOR(LilyInterpreterValue,
                           LilyInterpreterValue,
                           int8,
                           Int8 int8)
{
    return (LilyInterpreterValue){ .kind = LILY_INTERPRETER_VALUE_KIND_INT8,
                                   .ref_count = 0,
                                   .int8 = int8 };
}

/**
 *
 * @brief Construct LilyInterpreterValue (LILY_INTERPRETER_VALUE_KIND_INT16).
 */
inline VARIANT_CONSTRUCTOR(LilyInterpreterValue,
                           LilyInterpreterValue,
                           int16,
                           Int16 int16)
{
    return (LilyInterpreterValue){ .kind = LILY_INTERPRETER_VALUE_KIND_INT16,
                                   .ref_count = 0,
                                   .int16 = int16 };
}

/**
 *
 * @brief Construct LilyInterpreterValue (LILY_INTERPRETER_VALUE_KIND_INT32).
 */
inline VARIANT_CONSTRUCTOR(LilyInterpreterValue,
                           LilyInterpreterValue,
                           int32,
                           Int32 int32)
{
    return (LilyInterpreterValue){ .kind = LILY_INTERPRETER_VALUE_KIND_INT32,
                                   .ref_count = 0,
                                   .int32 = int32 };
}

/**
 *
 * @brief Construct LilyInterpreterValue (LILY_INTERPRETER_VALUE_KIND_INT64).
 */
inline VARIANT_CONSTRUCTOR(LilyInterpreterValue,
                           LilyInterpreterValue,
                           int64,
                           Int64 int64)
{
    return (LilyInterpreterValue){ .kind = LILY_INTERPRETER_VALUE_KIND_INT64,
                                   .ref_count = 0,
                                   .int64 = int64 };
}

/**
 *
 * @brief Construct LilyInterpreterValue (LILY_INTERPRETER_VALUE_KIND_ISIZE).
 */
inline VARIANT_CONSTRUCTOR(LilyInterpreterValue,
                           LilyInterpreterValue,
                           isize,
                           Isize isize)
{
    return (LilyInterpreterValue){ .kind = LILY_INTERPRETER_VALUE_KIND_ISIZE,
                                   .ref_count = 0,
                                   .isize = isize };
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
                                   .ref_count = 0,
                                   .float_ = float_ };
}

/**
 *
 * @brief Construct LilyInterpreterValue
 * (LILY_INTERPRETER_VALUE_KIND_LIST).
 */
inline VARIANT_CONSTRUCTOR(LilyInterpreterValue,
                           LilyInterpreterValue,
                           list,
                           LilyInterpreterValueList *list)
{
    return (LilyInterpreterValue){ .kind = LILY_INTERPRETER_VALUE_KIND_LIST,
                                   .ref_count = 0,
                                   .list = list };
}

/**
 *
 * @brief Construct LilyInterpreterValue
 * (LILY_INTERPRETER_VALUE_KIND_MULTI_POINTERS_ARRAY).
 */
inline VARIANT_CONSTRUCTOR(
  LilyInterpreterValue,
  LilyInterpreterValue,
  multi_pointers_array,
  LilyInterpreterValueMultiPointersArray *multi_pointers_array)
{
    return (LilyInterpreterValue){
        .kind = LILY_INTERPRETER_VALUE_KIND_MULTI_POINTERS_ARRAY,
        .ref_count = 0,
        .multi_pointers_array = multi_pointers_array
    };
}

/**
 *
 * @brief Construct LilyInterpreterValue (LILY_INTERPRETER_VALUE_KIND_PTR).
 */
inline VARIANT_CONSTRUCTOR(LilyInterpreterValue,
                           LilyInterpreterValue,
                           ptr,
                           void *ptr)
{
    return (LilyInterpreterValue){ .kind = LILY_INTERPRETER_VALUE_KIND_PTR,
                                   .ref_count = 0,
                                   .ptr = ptr };
}

/**
 *
 * @brief Construct LilyInterpreterValue (LILY_INTERPRETER_VALUE_KIND_RESULT).
 */
inline VARIANT_CONSTRUCTOR(LilyInterpreterValue,
                           LilyInterpreterValue,
                           result,
                           LilyInterpreterValueResult *result)
{
    return (LilyInterpreterValue){ .kind = LILY_INTERPRETER_VALUE_KIND_RESULT,
                                   .ref_count = 0,
                                   .result = result };
}

/**
 *
 * @brief Construct LilyInterpreterValue
 * (LILY_INTERPRETER_VALUE_KIND_SIZED_ARRAY).
 */
inline VARIANT_CONSTRUCTOR(LilyInterpreterValue,
                           LilyInterpreterValue,
                           sized_array,
                           LilyInterpreterValueSizedArray *sized_array)
{
    return (LilyInterpreterValue){ .kind =
                                     LILY_INTERPRETER_VALUE_KIND_SIZED_ARRAY,
                                   .ref_count = 0,
                                   .sized_array = sized_array };
}

/**
 *
 * @brief Construct LilyInterpreterValue (LILY_INTERPRETER_VALUE_KIND_STR).
 */
inline VARIANT_CONSTRUCTOR(LilyInterpreterValue,
                           LilyInterpreterValue,
                           str,
                           LilyInterpreterValueStr *str)
{
    return (LilyInterpreterValue){ .kind = LILY_INTERPRETER_VALUE_KIND_STR,
                                   .ref_count = 0,
                                   .str = str };
}

/**
 *
 * @brief Construct LilyInterpreterValue (LILY_INTERPRETER_VALUE_KIND_STRUCT).
 */
inline VARIANT_CONSTRUCTOR(LilyInterpreterValue,
                           LilyInterpreterValue,
                           struct,
                           LilyInterpreterValueStruct *struct_)
{
    return (LilyInterpreterValue){ .kind = LILY_INTERPRETER_VALUE_KIND_STRUCT,
                                   .ref_count = 0,
                                   .struct_ = struct_ };
}

/**
 *
 * @brief Construct LilyInterpreterValue (LILY_INTERPRETER_VALUE_KIND_UINT8).
 */
inline VARIANT_CONSTRUCTOR(LilyInterpreterValue,
                           LilyInterpreterValue,
                           uint8,
                           Uint8 uint8)
{
    return (LilyInterpreterValue){ .kind = LILY_INTERPRETER_VALUE_KIND_UINT8,
                                   .ref_count = 0,
                                   .uint8 = uint8 };
}

/**
 *
 * @brief Construct LilyInterpreterValue (LILY_INTERPRETER_VALUE_KIND_UINT16).
 */
inline VARIANT_CONSTRUCTOR(LilyInterpreterValue,
                           LilyInterpreterValue,
                           uint16,
                           Uint16 uint16)
{
    return (LilyInterpreterValue){ .kind = LILY_INTERPRETER_VALUE_KIND_UINT16,
                                   .ref_count = 0,
                                   .uint16 = uint16 };
}

/**
 *
 * @brief Construct LilyInterpreterValue (LILY_INTERPRETER_VALUE_KIND_UINT32).
 */
inline VARIANT_CONSTRUCTOR(LilyInterpreterValue,
                           LilyInterpreterValue,
                           uint32,
                           Uint32 uint32)
{
    return (LilyInterpreterValue){ .kind = LILY_INTERPRETER_VALUE_KIND_UINT32,
                                   .ref_count = 0,
                                   .uint32 = uint32 };
}

/**
 *
 * @brief Construct LilyInterpreterValue (LILY_INTERPRETER_VALUE_KIND_UINT64).
 */
inline VARIANT_CONSTRUCTOR(LilyInterpreterValue,
                           LilyInterpreterValue,
                           uint64,
                           Uint64 uint64)
{
    return (LilyInterpreterValue){ .kind = LILY_INTERPRETER_VALUE_KIND_UINT64,
                                   .ref_count = 0,
                                   .uint64 = uint64 };
}

/**
 *
 * @brief Construct LilyInterpreterValue (LILY_INTERPRETER_VALUE_KIND_USIZE).
 */
inline VARIANT_CONSTRUCTOR(LilyInterpreterValue,
                           LilyInterpreterValue,
                           usize,
                           Usize usize)
{
    return (LilyInterpreterValue){ .kind = LILY_INTERPRETER_VALUE_KIND_USIZE,
                                   .ref_count = 0,
                                   .usize = usize };
}

/**
 *
 * @brief Construct LilyInterpreterValue.
 */
inline CONSTRUCTOR(LilyInterpreterValue,
                   LilyInterpreterValue,
                   enum LilyInterpreterValueKind kind)
{
    return (LilyInterpreterValue){ .kind = kind, .ref_count = 0 };
}

/**
 *
 *
 * @brief Pass to ref a pointer of `LilyInterpreterValue` and increment
 * the `ref_count`.
 * @return LilyInterpreterValue*
 */
inline LilyInterpreterValue *
ref__LilyInterpreterValue(LilyInterpreterValue *self)
{
    ++self->ref_count;
    return self;
}

/**
 *
 * @brief Store src to self.
 * @param self dest
 */
void
store__LilyInterpreterValue(LilyInterpreterValue *self,
                            const LilyInterpreterValue *src,
                            bool raw_value);

/**
 *
 * @brief Minimal free (doesn't free union values).
 */
void
minimal_free__LilyInterpreterValue(LilyInterpreterValue *self);

/**
 *
 * @brief Free LilyInterpreterValue type.
 */
DESTRUCTOR(LilyInterpreterValue, LilyInterpreterValue *self);

struct LilyInterpreterValueDynamicArray
{
    Usize ref_count;
    LilyInterpreterValue *buffer;
    Usize len;
};

/**
 *
 * @brief Construct LilyInterpreterValueDynamicArray type.
 */
CONSTRUCTOR(LilyInterpreterValueDynamicArray *,
            LilyInterpreterValueDynamicArray);

/**
 *
 * @brief Get element at n from the dynamic array.
 */
inline LilyInterpreterValue
get__LilyInterpreterValueDynamicArray(
  const LilyInterpreterValueDynamicArray *self,
  Usize n)
{
    return self->buffer[n];
}

/**
 *
 *
 * @brief Pass to ref a pointer of `LilyInterpreterValueDynamicArray` and
 * increment the `ref_count`.
 * @return LilyInterpreterValueDynamicArray*
 */
inline LilyInterpreterValueDynamicArray *
ref__LilyInterpreterValueDynamicArray(LilyInterpreterValueDynamicArray *self)
{
    ++self->ref_count;
    return self;
}

/**
 *
 * @brief Free LilyInterpreterValueDynamicArray type.
 */
DESTRUCTOR(LilyInterpreterValueDynamicArray,
           LilyInterpreterValueDynamicArray *self);

struct LilyInterpreterValueMultiPointersArray
{
    Usize ref_count;
    LilyInterpreterValue *buffer;
    Usize len;
};

/**
 *
 * @brief Construct LilyInterpreterValueMultiPointersArray type.
 */
CONSTRUCTOR(LilyInterpreterValueMultiPointersArray *,
            LilyInterpreterValueMultiPointersArray);

/**
 *
 * @brief Get element at n from the multi pointers array.
 */
inline LilyInterpreterValue *
get__LilyInterpreterValueMultiPointersArray(
  const LilyInterpreterValueMultiPointersArray *self,
  Usize n)
{
    return &self->buffer[n];
}

/**
 *
 *
 * @brief Pass to ref a pointer of `LilyInterpreterValueMultiPointersArray` and
 * increment the `ref_count`.
 * @return LilyInterpreterValueMultiPointersArray*
 */
inline LilyInterpreterValueMultiPointersArray *
ref__LilyInterpreterValueMultiPointersArray(
  LilyInterpreterValueMultiPointersArray *self)
{
    ++self->ref_count;
    return self;
}

/**
 *
 * @brief Free LilyInterpreterValueMultiPointersArray type.
 */
DESTRUCTOR(LilyInterpreterValueMultiPointersArray,
           LilyInterpreterValueMultiPointersArray *self);

struct LilyInterpreterValueSizedArray
{
    Usize ref_count;
    LilyInterpreterValue *buffer;
    Usize len;
};

/**
 *
 * @brief Construct LilyInterpreterValueSizedArray type.
 */
CONSTRUCTOR(LilyInterpreterValueSizedArray *,
            LilyInterpreterValueSizedArray,
            LilyInterpreterValue *buffer,
            Usize len);

/**
 *
 * @brief Get element at n from the sized array.
 */
inline LilyInterpreterValue *
get__LilyInterpreterValueSizedArray(const LilyInterpreterValueSizedArray *self,
                                    Usize n)
{
    return &self->buffer[n];
}

/**
 *
 *
 * @brief Pass to ref a pointer of `LilyInterpreterValueSizedArray` and
 * increment the `ref_count`.
 * @return LilyInterpreterValueSizedArray*
 */
inline LilyInterpreterValueSizedArray *
ref__LilyInterpreterValueSizedArray(LilyInterpreterValueSizedArray *self)
{
    ++self->ref_count;
    return self;
}

/**
 *
 * @brief Free LilyInterpreterValueSizedArray type.
 */
DESTRUCTOR(LilyInterpreterValueSizedArray,
           LilyInterpreterValueSizedArray *self);

struct LilyInterpreterValueBytes
{
    Usize ref_count;
    Uint8 *buffer;
    Usize len;
};

/**
 *
 * @brief Construct LilyInterpreterValueBytes type.
 */
CONSTRUCTOR(LilyInterpreterValueBytes *,
            LilyInterpreterValueBytes,
            Uint8 *buffer,
            Usize len);

/**
 *
 * @brief Get element at n from the bytes.
 */
inline Uint8
get__LilyInterpreterValueBytes(const LilyInterpreterValueBytes *self, Usize n)
{
    return self->buffer[n];
}

/**
 *
 *
 * @brief Pass to ref a pointer of `LilyInterpreterValueBytes` and increment
 * the `ref_count`.
 * @return LilyInterpreterValueBytes*
 */
inline LilyInterpreterValueBytes *
ref__LilyInterpreterValueBytes(LilyInterpreterValueBytes *self)
{
    ++self->ref_count;
    return self;
}

/**
 *
 * @brief Free LilyInterpreterValueBytes type.
 */
DESTRUCTOR(LilyInterpreterValueBytes, LilyInterpreterValueBytes *self);

typedef struct LilyInterpreterValueListNode
{
    LilyInterpreterValue value;
    struct LilyInterpreterValueListNode *next;
} LilyInterpreterValueListNode;

/**
 *
 * @brief Construct LilyInterpreterValueListNode type.
 */
CONSTRUCTOR(LilyInterpreterValueListNode *,
            LilyInterpreterValueListNode,
            LilyInterpreterValue value,
            struct LilyInterpreterValueListNode *next);

/**
 *
 * @brief Free LilyInterpreterValueListNode type.
 */
DESTRUCTOR(LilyInterpreterValueListNode, LilyInterpreterValueListNode *self);

struct LilyInterpreterValueList
{
    Usize ref_count;
    LilyInterpreterValueListNode *head;
    LilyInterpreterValueListNode *tail;
};

/**
 *
 * @brief Construct LilyInterpreterValueList type.
 */
CONSTRUCTOR(LilyInterpreterValueList *,
            LilyInterpreterValueList,
            LilyInterpreterValueListNode *head,
            LilyInterpreterValueListNode *tail);

/**
 *
 *
 * @brief Pass to ref a pointer of `LilyInterpreterValueList` and increment
 * the `ref_count`.
 * @return LilyInterpreterValueList*
 */
inline LilyInterpreterValueList *
ref__LilyInterpreterValueList(LilyInterpreterValueList *self)
{
    ++self->ref_count;
    return self;
}

/**
 *
 * @brief Free LilyInterpreterValueList type.
 */
DESTRUCTOR(LilyInterpreterValueList, LilyInterpreterValueList *self);

enum LilyInterpreterValueResultKind
{
    LILY_INTERPRETER_VALUE_RESULT_KIND_OK,
    LILY_INTERPRETER_VALUE_RESULT_KIND_ERR,
};

typedef struct LilyInterpreterValueResult
{
    enum LilyInterpreterValueResultKind kind;
    Usize ref_count;
    union
    {
        LilyInterpreterValue ok;
        LilyInterpreterValue err;
    };
} LilyInterpreterValueResult;

/**
 *
 * @brief Construct LilyInterpreterValueResult type
 * (LILY_INTERPRETER_VALUE_RESULT_KIND_OK).
 */
VARIANT_CONSTRUCTOR(LilyInterpreterValueResult *,
                    LilyInterpreterValueResult,
                    ok,
                    LilyInterpreterValue ok);

/**
 *
 * @brief Construct LilyInterpreterValueResult type
 * (LILY_INTERPRETER_VALUE_RESULT_KIND_ERR).
 */
VARIANT_CONSTRUCTOR(LilyInterpreterValueResult *,
                    LilyInterpreterValueResult,
                    err,
                    LilyInterpreterValue err);

/**
 *
 *
 * @brief Pass to ref a pointer of `LilyInterpreterValueResult` and increment
 * the `ref_count`.
 * @return LilyInterpreterValueResult*
 */
inline LilyInterpreterValueResult *
ref__LilyInterpreterValueResult(LilyInterpreterValueResult *self)
{
    ++self->ref_count;
    return self;
}

/**
 *
 * @brief Free LilyInterpreterValueResult type.
 */
DESTRUCTOR(LilyInterpreterValueResult, LilyInterpreterValueResult *self);

typedef struct LilyInterpreterValueStr
{
    Usize ref_count;
    char *s;
    Usize len;
} LilyInterpreterValueStr;

/**
 *
 * @brief Construct LilyInterpreterValueStr type.
 */
CONSTRUCTOR(LilyInterpreterValueStr *,
            LilyInterpreterValueStr,
            char *s,
            Usize len);

/**
 *
 * @brief Get element at n from the str.
 */
inline char
get__LilyInterpreterValueStr(const LilyInterpreterValueStr *self, Usize n)
{
    return self->s[n];
}

/**
 *
 *
 * @brief Pass to ref a pointer of `LilyInterpreterValueStr` and increment
 * the `ref_count`.
 * @return LilyInterpreterValueStr*
 */
inline LilyInterpreterValueStr *
ref__LilyInterpreterValueStr(LilyInterpreterValueStr *self)
{
    ++self->ref_count;
    return self;
}

/**
 *
 * @brief Free LilyInterpreterValueStr type.
 */
DESTRUCTOR(LilyInterpreterValueStr, LilyInterpreterValueStr *self);

typedef struct LilyInterpreterValueStruct
{
    Usize ref_count;
    LilyInterpreterValue values[MAX_RECORD_FIELDS];
    Usize len;
} LilyInterpreterValueStruct;

/**
 *
 * @brief Construct LilyInterpreterValueStruct type.
 */
CONSTRUCTOR(LilyInterpreterValueStruct *,
            LilyInterpreterValueStruct,
            LilyInterpreterValue *values,
            Usize len);

/**
 *
 * @brief Get field at n from the struct.
 */
LilyInterpreterValue *
get__LilyInterpreterValueStruct(LilyInterpreterValueStruct *self, Usize n);

/**
 *
 *
 * @brief Pass to ref a pointer of `LilyInterpreterValueStruct` and increment
 * the `ref_count`.
 * @return LilyInterpreterValueStruct*
 */
inline LilyInterpreterValueStruct *
ref__LilyInterpreterValueStruct(LilyInterpreterValueStruct *self)
{
    ++self->ref_count;
    return self;
}

/**
 *
 * @brief Free LilyInterpreterValueStruct type.
 */
DESTRUCTOR(LilyInterpreterValueStruct, LilyInterpreterValueStruct *self);

#endif // LILY_CORE_LILY_INTERPRETER_VALUE_H
