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

#ifndef LILY_CORE_LILY_PARSER_AST_DATA_TYPE_H
#define LILY_CORE_LILY_PARSER_AST_DATA_TYPE_H

#include <base/macros.h>
#include <base/new.h>
#include <base/string.h>
#include <base/vec.h>

#include <core/shared/location.h>

enum LilyAstDataTypeKind
{
    LILY_AST_DATA_TYPE_KIND_ANY,
    LILY_AST_DATA_TYPE_KIND_ARRAY,
    LILY_AST_DATA_TYPE_KIND_BOOL,
    LILY_AST_DATA_TYPE_KIND_BYTE,
    LILY_AST_DATA_TYPE_KIND_BYTES,
    LILY_AST_DATA_TYPE_KIND_CHAR,
    LILY_AST_DATA_TYPE_KIND_CSHORT,
    LILY_AST_DATA_TYPE_KIND_CUSHORT,
    LILY_AST_DATA_TYPE_KIND_CINT,
    LILY_AST_DATA_TYPE_KIND_CUINT,
    LILY_AST_DATA_TYPE_KIND_CLONG,
    LILY_AST_DATA_TYPE_KIND_CULONG,
    LILY_AST_DATA_TYPE_KIND_CLONGLONG,
    LILY_AST_DATA_TYPE_KIND_CULONGLONG,
    LILY_AST_DATA_TYPE_KIND_CFLOAT,
    LILY_AST_DATA_TYPE_KIND_CDOUBLE,
    LILY_AST_DATA_TYPE_KIND_CSTR,
    LILY_AST_DATA_TYPE_KIND_CVOID,
    LILY_AST_DATA_TYPE_KIND_CUSTOM,
    LILY_AST_DATA_TYPE_KIND_FLOAT32,
    LILY_AST_DATA_TYPE_KIND_FLOAT64,
    LILY_AST_DATA_TYPE_KIND_INT16,
    LILY_AST_DATA_TYPE_KIND_INT32,
    LILY_AST_DATA_TYPE_KIND_INT64,
    LILY_AST_DATA_TYPE_KIND_INT8,
    LILY_AST_DATA_TYPE_KIND_ISIZE,
    LILY_AST_DATA_TYPE_KIND_LAMBDA,
    LILY_AST_DATA_TYPE_KIND_LIST,
    LILY_AST_DATA_TYPE_KIND_MUT,
    LILY_AST_DATA_TYPE_KIND_NEVER,
    LILY_AST_DATA_TYPE_KIND_OBJECT,
    LILY_AST_DATA_TYPE_KIND_OPTIONAL,
    LILY_AST_DATA_TYPE_KIND_PTR,
    LILY_AST_DATA_TYPE_KIND_REF,
    LILY_AST_DATA_TYPE_KIND_RESULT,
    LILY_AST_DATA_TYPE_KIND_SELF,
    LILY_AST_DATA_TYPE_KIND_STR,
    LILY_AST_DATA_TYPE_KIND_TRACE,
    LILY_AST_DATA_TYPE_KIND_TUPLE,
    LILY_AST_DATA_TYPE_KIND_UINT16,
    LILY_AST_DATA_TYPE_KIND_UINT32,
    LILY_AST_DATA_TYPE_KIND_UINT64,
    LILY_AST_DATA_TYPE_KIND_UINT8,
    LILY_AST_DATA_TYPE_KIND_UNIT,
    LILY_AST_DATA_TYPE_KIND_USIZE,
};

typedef struct LilyAstDataType LilyAstDataType;

typedef struct LilyAstDataTypeLambda
{
    Vec *params;                  // Vec<LilyAstDataType*>*?
    LilyAstDataType *return_type; // LilyAstDatatType*?
} LilyAstDataTypeLambda;

/**
 *
 * @brief Construct LilyAstDataTypeLambda type.
 */
inline CONSTRUCTOR(LilyAstDataTypeLambda,
                   LilyAstDataTypeLambda,
                   Vec *params,
                   LilyAstDataType *return_type)
{
    return (LilyAstDataTypeLambda){ .params = params,
                                    .return_type = return_type };
}

/**
 *
 * @brief Convert LilyAstDataTypeLambda in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyAstDataTypeLambda,
               const LilyAstDataTypeLambda *self);
#endif

enum LilyAstDataTypeArrayKind
{
    LILY_AST_DATA_TYPE_ARRAY_KIND_DYNAMIC,
    LILY_AST_DATA_TYPE_ARRAY_KIND_MULTI_POINTERS,
    LILY_AST_DATA_TYPE_ARRAY_KIND_SIZED,
    LILY_AST_DATA_TYPE_ARRAY_KIND_UNDETERMINED,
};

typedef struct LilyAstDataTypeArray
{
    enum LilyAstDataTypeArrayKind kind;
    LilyAstDataType *data_type; // LilyAstDataType*?
    union
    {
        Usize size;
    };
} LilyAstDataTypeArray;

/**
 *
 * @brief Construct LilyAstDataTypeArray type.
 */
inline CONSTRUCTOR(LilyAstDataTypeArray,
                   LilyAstDataTypeArray,
                   enum LilyAstDataTypeArrayKind kind,
                   LilyAstDataType *data_type)
{
    return (LilyAstDataTypeArray){ .kind = kind, .data_type = data_type };
}

/**
 *
 * @brief Construct LilyAstDataTypeArray type
 * (LILY_AST_DATA_TYPE_ARRAY_KIND_SIZED).
 */
inline VARIANT_CONSTRUCTOR(LilyAstDataTypeArray,
                           LilyAstDataTypeArray,
                           sized,
                           LilyAstDataType *data_type,
                           Usize size)
{
    return (LilyAstDataTypeArray){ .kind = LILY_AST_DATA_TYPE_ARRAY_KIND_SIZED,
                                   .data_type = data_type,
                                   .size = size };
}

/**
 *
 * @brief Convert LilyAstDataTypeArrayKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyAstDataTypeArrayKind,
               enum LilyAstDataTypeArrayKind self);
#endif

/**
 *
 * @brief Convert LilyAstDataTypeArray in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyAstDataTypeArray,
               const LilyAstDataTypeArray *self);
#endif

typedef struct LilyAstDataTypeCustom
{
    String *name;
    Vec *generics; // Vec<LilyAstDataType*>*?
} LilyAstDataTypeCustom;

/**
 *
 * @brief Construct LilyAstDataTypeCustom type.
 */
inline CONSTRUCTOR(LilyAstDataTypeCustom,
                   LilyAstDataTypeCustom,
                   String *name,
                   Vec *generics)
{
    return (LilyAstDataTypeCustom){ .name = name, .generics = generics };
}

/**
 *
 * @brief Convert LilyAstDataTypeCustom in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyAstDataTypeCustom,
               const LilyAstDataTypeCustom *self);
#endif

typedef struct LilyAstDataTypeResult
{
    LilyAstDataType *ok;
    Vec *errs; // Vec<LilyAstDataType*>*?
} LilyAstDataTypeResult;

/**
 *
 * @brief Construct LilyAstDataTypeResult type.
 */
inline CONSTRUCTOR(LilyAstDataTypeResult,
                   LilyAstDataTypeResult,
                   LilyAstDataType *ok,
                   Vec *errs)
{
    return (LilyAstDataTypeResult){ .ok = ok, .errs = errs };
}

/**
 *
 * @brief Convert LilyAstDataTypeResult in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyAstDataTypeResult,
               const LilyAstDataTypeResult *self);
#endif

/**
 *
 * @brief Free LilyAstDataTypeResult type.
 */
DESTRUCTOR(LilyAstDataTypeResult, const LilyAstDataTypeResult *self);

struct LilyAstDataType
{
    enum LilyAstDataTypeKind kind;
    Location location;
    union
    {
        LilyAstDataTypeArray array;
        LilyAstDataTypeCustom custom;
        LilyAstDataTypeLambda lambda;
        LilyAstDataType *list;
        LilyAstDataType *mut;
        LilyAstDataType *optional;
        LilyAstDataType *ptr;
        LilyAstDataType *ref;
        LilyAstDataTypeResult result;
        LilyAstDataType *trace;
        Vec *tuple; // Vec<LilyAstDataType*>*
    };
};

/**
 *
 * @brief Construct LilyAstDataType type.
 */
CONSTRUCTOR(LilyAstDataType *,
            LilyAstDataType,
            enum LilyAstDataTypeKind kind,
            Location location);

/**
 *
 * @brief Construct LilyAstDataType type (LILY_AST_DATA_TYPE_KIND_ARRAY).
 */
VARIANT_CONSTRUCTOR(LilyAstDataType *,
                    LilyAstDataType,
                    array,
                    Location location,
                    LilyAstDataTypeArray array);

/**
 *
 * @brief Construct LilyAstDataType type (LILY_AST_DATA_TYPE_KIND_CUSTOM).
 */
VARIANT_CONSTRUCTOR(LilyAstDataType *,
                    LilyAstDataType,
                    custom,
                    Location location,
                    LilyAstDataTypeCustom custom);

/**
 *
 * @brief Construct LilyAstDataType type (LILY_AST_DATA_TYPE_KIND_LAMBDA).
 */
VARIANT_CONSTRUCTOR(LilyAstDataType *,
                    LilyAstDataType,
                    lambda,
                    Location location,
                    LilyAstDataTypeLambda lambda);

/**
 *
 * @brief Construct LilyAstDataType type (LILY_AST_DATA_TYPE_KIND_LIST).
 */
VARIANT_CONSTRUCTOR(LilyAstDataType *,
                    LilyAstDataType,
                    list,
                    Location location,
                    LilyAstDataType *list);

/**
 *
 * @brief Construct LilyAstDataType type (LILY_AST_DATA_TYPE_KIND_MUT).
 */
VARIANT_CONSTRUCTOR(LilyAstDataType *,
                    LilyAstDataType,
                    mut,
                    Location location,
                    LilyAstDataType *mut);

/**
 *
 * @brief Construct LilyAstDataType type (LILY_AST_DATA_TYPE_KIND_OPTIONAL).
 */
VARIANT_CONSTRUCTOR(LilyAstDataType *,
                    LilyAstDataType,
                    optional,
                    Location location,
                    LilyAstDataType *optional);

/**
 *
 * @brief Construct LilyAstDataType type (LILY_AST_DATA_TYPE_KIND_PTR).
 */
VARIANT_CONSTRUCTOR(LilyAstDataType *,
                    LilyAstDataType,
                    ptr,
                    Location location,
                    LilyAstDataType *ptr);

/**
 *
 * @brief Construct LilyAstDataType type (LILY_AST_DATA_TYPE_KIND_REF).
 */
VARIANT_CONSTRUCTOR(LilyAstDataType *,
                    LilyAstDataType,
                    ref,
                    Location location,
                    LilyAstDataType *ref);

/**
 *
 * @brief Construct LilyAstDataType type (LILY_AST_DATA_TYPE_KIND_RESULT).
 */
VARIANT_CONSTRUCTOR(LilyAstDataType *,
                    LilyAstDataType,
                    result,
                    Location location,
                    LilyAstDataTypeResult result);

/**
 *
 * @brief Construct LilyAstDataType type (LILY_AST_DATA_TYPE_KIND_TRACE).
 */
VARIANT_CONSTRUCTOR(LilyAstDataType *,
                    LilyAstDataType,
                    trace,
                    Location location,
                    LilyAstDataType *trace);

/**
 *
 * @brief Construct LilyAstDataType type (LILY_AST_DATA_TYPE_KIND_TUPLE).
 */
VARIANT_CONSTRUCTOR(LilyAstDataType *,
                    LilyAstDataType,
                    tuple,
                    Location location,
                    Vec *tuple);

/**
 *
 * @brief Convert LilyAstDataTypeKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, LilyAstDataTypeKind, enum LilyAstDataTypeKind self);
#endif

/**
 *
 * @brief Convert LilyAstDataType in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyAstDataType, const LilyAstDataType *self);
#endif

/**
 *
 * @brief Print debug LilyAstDataType struct.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
void
IMPL_FOR_DEBUG(debug, LilyAstDataType, const LilyAstDataType *self);
#endif

/**
 *
 * @brief Convert LilyAstDataType in String.
 */
String *
to_string__LilyAstDataType(const LilyAstDataType *self);

/**
 *
 * @brief Free LilyAstDataType.
 */
DESTRUCTOR(LilyAstDataType, LilyAstDataType *self);

#endif // LILY_CORE_LILY_PARSER_AST_DATA_TYPE_H
