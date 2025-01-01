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

#ifndef LILY_CORE_LILY_ANALYSIS_CHECKED_DATA_TYPE_H
#define LILY_CORE_LILY_ANALYSIS_CHECKED_DATA_TYPE_H

#include <base/hash_map.h>
#include <base/macros.h>
#include <base/new.h>
#include <base/ordered_hash_map.h>
#include <base/string.h>
#include <base/vec.h>

#include <core/shared/location.h>

#include <core/lily/analysis/checked/access.h>

typedef struct LilyCheckedScope LilyCheckedScope;

enum LilyCheckedDataTypeKind
{
    LILY_CHECKED_DATA_TYPE_KIND_ANY,
    LILY_CHECKED_DATA_TYPE_KIND_ARRAY,
    LILY_CHECKED_DATA_TYPE_KIND_BOOL,
    LILY_CHECKED_DATA_TYPE_KIND_BYTE,
    LILY_CHECKED_DATA_TYPE_KIND_BYTES,
    LILY_CHECKED_DATA_TYPE_KIND_CHAR,
    LILY_CHECKED_DATA_TYPE_KIND_CSHORT,
    LILY_CHECKED_DATA_TYPE_KIND_CUSHORT,
    LILY_CHECKED_DATA_TYPE_KIND_CINT,
    LILY_CHECKED_DATA_TYPE_KIND_CUINT,
    LILY_CHECKED_DATA_TYPE_KIND_CLONG,
    LILY_CHECKED_DATA_TYPE_KIND_CULONG,
    LILY_CHECKED_DATA_TYPE_KIND_CLONGLONG,
    LILY_CHECKED_DATA_TYPE_KIND_CULONGLONG,
    LILY_CHECKED_DATA_TYPE_KIND_CFLOAT,
    LILY_CHECKED_DATA_TYPE_KIND_CDOUBLE,
    LILY_CHECKED_DATA_TYPE_KIND_CSTR,
    LILY_CHECKED_DATA_TYPE_KIND_CVOID,
    LILY_CHECKED_DATA_TYPE_KIND_CUSTOM,
    LILY_CHECKED_DATA_TYPE_KIND_RESULT,
    LILY_CHECKED_DATA_TYPE_KIND_FLOAT32,
    LILY_CHECKED_DATA_TYPE_KIND_FLOAT64,
    LILY_CHECKED_DATA_TYPE_KIND_INT16,
    LILY_CHECKED_DATA_TYPE_KIND_INT32,
    LILY_CHECKED_DATA_TYPE_KIND_INT64,
    LILY_CHECKED_DATA_TYPE_KIND_INT8,
    LILY_CHECKED_DATA_TYPE_KIND_ISIZE,
    LILY_CHECKED_DATA_TYPE_KIND_LAMBDA,
    LILY_CHECKED_DATA_TYPE_KIND_LIST,
    LILY_CHECKED_DATA_TYPE_KIND_MUT,
    LILY_CHECKED_DATA_TYPE_KIND_NEVER,
    LILY_CHECKED_DATA_TYPE_KIND_OPTIONAL,
    LILY_CHECKED_DATA_TYPE_KIND_PTR,
    LILY_CHECKED_DATA_TYPE_KIND_PTR_MUT,
    LILY_CHECKED_DATA_TYPE_KIND_REF,
    LILY_CHECKED_DATA_TYPE_KIND_REF_MUT,
    LILY_CHECKED_DATA_TYPE_KIND_STR,
    LILY_CHECKED_DATA_TYPE_KIND_TRACE,
    LILY_CHECKED_DATA_TYPE_KIND_TRACE_MUT,
    LILY_CHECKED_DATA_TYPE_KIND_TUPLE,
    LILY_CHECKED_DATA_TYPE_KIND_UINT16,
    LILY_CHECKED_DATA_TYPE_KIND_UINT32,
    LILY_CHECKED_DATA_TYPE_KIND_UINT64,
    LILY_CHECKED_DATA_TYPE_KIND_UINT8,
    LILY_CHECKED_DATA_TYPE_KIND_UNIT,
    LILY_CHECKED_DATA_TYPE_KIND_USIZE,

    // These data types cannot be defined by the user
    LILY_CHECKED_DATA_TYPE_KIND_UNKNOWN,
    LILY_CHECKED_DATA_TYPE_KIND_CONDITIONAL_COMPILER_CHOICE,
    LILY_CHECKED_DATA_TYPE_KIND_COMPILER_CHOICE,
    LILY_CHECKED_DATA_TYPE_KIND_COMPILER_GENERIC
};

typedef struct LilyCheckedDataType LilyCheckedDataType;

typedef struct LilyCheckedDataTypeLambda
{
    Vec *params; // Vec<LilyCheckedDataType*>*?
    LilyCheckedDataType *return_type;
} LilyCheckedDataTypeLambda;

/**
 *
 * @brief Construct LilyCheckedDataTypeLambda type.
 */
inline CONSTRUCTOR(LilyCheckedDataTypeLambda,
                   LilyCheckedDataTypeLambda,
                   Vec *params,
                   LilyCheckedDataType *return_type)
{
    return (LilyCheckedDataTypeLambda){ .params = params,
                                        .return_type = return_type };
}

/**
 *
 * @brief Convert LilyCheckedDataTypeLambda in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedDataTypeLambda,
               const LilyCheckedDataTypeLambda *self);
#endif

enum LilyCheckedDataTypeArrayKind
{
    LILY_CHECKED_DATA_TYPE_ARRAY_KIND_DYNAMIC,
    LILY_CHECKED_DATA_TYPE_ARRAY_KIND_MULTI_POINTERS,
    LILY_CHECKED_DATA_TYPE_ARRAY_KIND_SIZED,
    LILY_CHECKED_DATA_TYPE_ARRAY_KIND_UNDETERMINED,
    LILY_CHECKED_DATA_TYPE_ARRAY_KIND_UNKNOWN
};

typedef struct LilyCheckedDataTypeArray
{
    enum LilyCheckedDataTypeArrayKind kind;
    LilyCheckedDataType *data_type;
    union
    {
        Usize sized;
        Usize unknown; // Size of the unknown array
    };
} LilyCheckedDataTypeArray;

/**
 *
 * @brief Construct LilyCheckedDataTypeArray type.
 */
inline CONSTRUCTOR(LilyCheckedDataTypeArray,
                   LilyCheckedDataTypeArray,
                   enum LilyCheckedDataTypeArrayKind kind,
                   LilyCheckedDataType *data_type)
{
    return (LilyCheckedDataTypeArray){ .kind = kind, .data_type = data_type };
}

/**
 *
 * @brief Construct LilyCheckedDataTypeArray type
 * (LILY_CHECKED_DATA_TYPE_ARRAY_KIND_SIZED).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedDataTypeArray,
                           LilyCheckedDataTypeArray,
                           sized,
                           LilyCheckedDataType *data_type,
                           Usize sized)
{
    return (LilyCheckedDataTypeArray){
        .kind = LILY_CHECKED_DATA_TYPE_ARRAY_KIND_SIZED,
        .data_type = data_type,
        .sized = sized
    };
}

/**
 *
 * @brief Construct LilyCheckedDataTypeArray type
 * (LILY_CHECKED_DATA_TYPE_ARRAY_KIND_UNKNOWN).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedDataTypeArray,
                           LilyCheckedDataTypeArray,
                           unknown,
                           LilyCheckedDataType *data_type,
                           Usize unknown)
{
    return (LilyCheckedDataTypeArray){
        .kind = LILY_CHECKED_DATA_TYPE_ARRAY_KIND_UNKNOWN,
        .data_type = data_type,
        .unknown = unknown
    };
}

/**
 *
 * @brief Convert LilyCheckedDataTypeArrayKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedDataTypeArrayKind,
               enum LilyCheckedDataTypeArrayKind self);
#endif

/**
 *
 * @brief Convert LilyCheckedDataTypeArray in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedDataTypeArray,
               const LilyCheckedDataTypeArray *self);
#endif

enum LilyCheckedDataTypeCustomKind
{
    LILY_CHECKED_DATA_TYPE_CUSTOM_KIND_CLASS,
    LILY_CHECKED_DATA_TYPE_CUSTOM_KIND_ENUM,
    LILY_CHECKED_DATA_TYPE_CUSTOM_KIND_ENUM_OBJECT,
    LILY_CHECKED_DATA_TYPE_CUSTOM_KIND_ERROR,
    LILY_CHECKED_DATA_TYPE_CUSTOM_KIND_GENERIC,
    LILY_CHECKED_DATA_TYPE_CUSTOM_KIND_RECORD,
    LILY_CHECKED_DATA_TYPE_CUSTOM_KIND_RECORD_OBJECT,
    LILY_CHECKED_DATA_TYPE_CUSTOM_KIND_TRAIT,
};

/**
 *
 * @brief Convert LilyCheckedDataTypeCustomKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedDataTypeCustomKind,
               enum LilyCheckedDataTypeCustomKind self);
#endif

typedef struct LilyCheckedDataTypeCustom
{
    Usize scope_id;
    LilyCheckedAccessScope scope;
    String *name;        // String* (&)
    String *global_name; // String* (&)
    Vec *generics;       // Vec<LilyCheckedDataType*>*?
    enum LilyCheckedDataTypeCustomKind kind;
    bool is_recursive;
} LilyCheckedDataTypeCustom;

/**
 *
 * @brief Construct LilyCheckedDataTypeCustom type.
 */
inline CONSTRUCTOR(LilyCheckedDataTypeCustom,
                   LilyCheckedDataTypeCustom,
                   Usize scope_id,
                   LilyCheckedAccessScope scope,
                   String *name,
                   String *global_name,
                   Vec *generics,
                   enum LilyCheckedDataTypeCustomKind kind,
                   bool is_recursive)
{
    return (LilyCheckedDataTypeCustom){ .scope_id = scope_id,
                                        .scope = scope,
                                        .name = name,
                                        .global_name = global_name,
                                        .generics = generics,
                                        .kind = kind,
                                        .is_recursive = is_recursive };
}

/**
 *
 * @brief Convert LilyCheckedDataTypeCustom in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedDataTypeCustom,
               const LilyCheckedDataTypeCustom *self);
#endif

typedef struct LilyCheckedDataTypeCondition
{
    Vec *params; // Vec<LilyCheckedDataType* (&)>*
    Usize return_data_type_id;
    Usize ref_count;
} LilyCheckedDataTypeCondition;

/**
 *
 * @brief Construct LilyCheckedDataTypeCondition type.
 */
CONSTRUCTOR(LilyCheckedDataTypeCondition *,
            LilyCheckedDataTypeCondition,
            Vec *params,
            Usize return_data_type_id);

/**
 *
 * @brief Pass to ref a pointer of `LilyCheckedDataTypeCondition` and increment
 * the `ref_count`.
 * @return LilyCheckedDataTypeCondition* (&)
 */
inline LilyCheckedDataTypeCondition *
ref__LilyCheckedDataTypeCondition(LilyCheckedDataTypeCondition *self)
{
    ++self->ref_count;
    return self;
}

/**
 *
 * @brief Convert LilyCheckedDataTypeCondition in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedDataTypeCondition,
               const LilyCheckedDataTypeCondition *self);
#endif

/**
 *
 * @brief Free LilyCheckedDataTypeCondition type.
 */
DESTRUCTOR(LilyCheckedDataTypeCondition, LilyCheckedDataTypeCondition *self);

typedef struct LilyCheckedDataTypeConditionalCompilerChoice
{
    Vec *conds;   // Vec<LilyCheckedDataTypeCondition*>* => params
    Vec *choices; // Vec<LilyCheckedDataType* (&)>* => return data types
} LilyCheckedDataTypeConditionalCompilerChoice;

/**
 *
 * @brief Construct LilyCheckedDataTypeConditionalCompilerChoice type.
 */
inline CONSTRUCTOR(LilyCheckedDataTypeConditionalCompilerChoice,
                   LilyCheckedDataTypeConditionalCompilerChoice,
                   Vec *choices,
                   Vec *conds)
{
    return (LilyCheckedDataTypeConditionalCompilerChoice){ .conds = conds,
                                                           .choices = choices };
}

/**
 *
 * @brief Check if the choice is not duplicate then add it.
 * @return Return index of the pushed choice.
 * @param LilyCheckedDataType* (&)
 */
Usize
add_choice__LilyCheckedDataTypeConditionalCompilerChoice(
  const LilyCheckedDataTypeConditionalCompilerChoice *self,
  LilyCheckedDataType *choice);

/**
 *
 * @brief Convert LilyCheckedDataTypeConditionalCompilerChoice in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedDataTypeConditionalCompilerChoice,
               const LilyCheckedDataTypeConditionalCompilerChoice *self);
#endif

/**
 *
 * @brief Free LilyCheckedDataTypeCondtionalCompilerChoice type.
 */
DESTRUCTOR(LilyCheckedDataTypeConditionalCompilerChoice,
           const LilyCheckedDataTypeConditionalCompilerChoice *self);

typedef struct LilyCheckedDataTypeCompilerGeneric
{
    String *name; // String* (&)
} LilyCheckedDataTypeCompilerGeneric;

/**
 *
 * @brief Construct LilyCheckedDataTypeCompilerGeneric type.
 */
inline CONSTRUCTOR(LilyCheckedDataTypeCompilerGeneric,
                   LilyCheckedDataTypeCompilerGeneric,
                   String *name)
{
    return (LilyCheckedDataTypeCompilerGeneric){ .name = name };
}

/**
 *
 * @brief Convert LilyCheckedDataTypeCompilerGeneric in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedDataTypeCompilerGeneric,
               const LilyCheckedDataTypeCompilerGeneric *self);
#endif

typedef struct LilyCheckedDataTypeResult
{
    LilyCheckedDataType *ok;
    Vec *errs; // Vec<LilyCheckedDataType*>*?
    Usize ref_count;
} LilyCheckedDataTypeResult;

/**
 *
 * @brief Construct LilyCheckedDataTypeResult type.
 */
CONSTRUCTOR(LilyCheckedDataTypeResult *,
            LilyCheckedDataTypeResult,
            LilyCheckedDataType *ok,
            Vec *errs);

/**
 *
 * @brief Pass to ref a data type and increment the `ref_count`.
 * @return LilyCheckedDataTypeResult* (&)
 */
inline LilyCheckedDataTypeResult *
ref__LilyCheckedDataTypeResult(LilyCheckedDataTypeResult *self)
{
    ++self->ref_count;
    return self;
}

/**
 *
 * @brief Convert LilyCheckedDataTypeResult in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedDataTypeResult,
               const LilyCheckedDataTypeResult *self);
#endif

/**
 *
 * @brief Free LilyCheckedDataTypeResult type.
 */
DESTRUCTOR(LilyCheckedDataTypeResult, LilyCheckedDataTypeResult *self);

typedef struct LilyCheckedDataTypeLen
{
    Usize len; // can be undef
    bool is_undef;
} LilyCheckedDataTypeLen;

/**
 *
 * @brief Construct LilyCheckedDataTypeLen type (undef variant).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedDataTypeLen,
                           LilyCheckedDataTypeLen,
                           undef)
{
    return (LilyCheckedDataTypeLen){
        .is_undef = true,
    };
}

/**
 *
 * @brief Construct LilyCheckedDataTypeLen type (def variant).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedDataTypeLen,
                           LilyCheckedDataTypeLen,
                           def,
                           Usize len)
{
    return (LilyCheckedDataTypeLen){ .is_undef = false, .len = len };
}

/**
 *
 * @brief Clone LilyCheckedDataTypeLen type.
 */
inline LilyCheckedDataTypeLen
clone__LilyCheckedDataTypeLen(const LilyCheckedDataTypeLen *self)
{
    return self->is_undef ? NEW_VARIANT(LilyCheckedDataTypeLen, undef)
                          : NEW_VARIANT(LilyCheckedDataTypeLen, def, self->len);
}

/**
 *
 * @brief Check if the both LilyCheckedDataTypeLen are equal.
 */
inline bool
eq__LilyCheckedDataTypeLen(const LilyCheckedDataTypeLen *self,
                           const LilyCheckedDataTypeLen *other)
{
    return self->is_undef == other->is_undef ? true : self->len == other->len;
}

/**
 *
 * @brief Convert LilyCheckedDataTypeLen in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedDataTypeLen,
               const LilyCheckedDataTypeLen *self);
#endif

struct LilyCheckedDataType
{
    enum LilyCheckedDataTypeKind kind;
    const Location *location; // const Location*? (&)
    Usize ref_count;
    // NOTE: only using for compiler choice data type
    // (`LILY_CHECKED_DATA_TYPE_KIND_COMPILER_CHOICE` and
    // `LILY_CHECKED_DATA_TYPE_KIND_CONDITIONAL_COMPILER_CHOICE`). By default,
    // is set on true on all data types except for compiler choice defined data
    // type. Moreover, this value indicates if the compiler can eleminate a type
    // of the choice. This value is set on true when the function is finish to
    // analysis. In addition, all data types from other functions or methods
    // cannot be modified when they are called (type elimination is not possible
    // after function or method analysis).
    bool is_lock;
    union
    {
        LilyCheckedDataTypeArray array;
        LilyCheckedDataTypeLen bytes;
        LilyCheckedDataTypeCustom custom;
        LilyCheckedDataTypeLambda lambda;
        LilyCheckedDataType *list;
        LilyCheckedDataType *mut;
        LilyCheckedDataType *optional;
        LilyCheckedDataType *ptr;
        LilyCheckedDataType *ptr_mut;
        LilyCheckedDataType *ref;
        LilyCheckedDataType *ref_mut;
        LilyCheckedDataTypeResult *result;
        LilyCheckedDataTypeLen str;
        LilyCheckedDataType *trace;
        LilyCheckedDataType *trace_mut;
        Vec *tuple; // Vec<LilyCheckedDataType*>*
        LilyCheckedDataTypeConditionalCompilerChoice
          conditional_compiler_choice;
        Vec *compiler_choice; // Vec<LilyCheckedDataType* (&)>*
        LilyCheckedDataTypeCompilerGeneric compiler_generic;
    };
};

/**
 *
 * @brief Construct LilyCheckedDataType type.
 */
CONSTRUCTOR(LilyCheckedDataType *,
            LilyCheckedDataType,
            enum LilyCheckedDataTypeKind kind,
            const Location *location);

/**
 *
 * @brief Construct LilyCheckedDataType type
 * (LILY_CHECKED_DATA_TYPE_KIND_ARRAY).
 */
VARIANT_CONSTRUCTOR(LilyCheckedDataType *,
                    LilyCheckedDataType,
                    array,
                    const Location *location,
                    LilyCheckedDataTypeArray array);

/**
 *
 * @brief Construct LilyCheckedDataType type
 * (LILY_CHECKED_DATA_TYPE_KIND_BYTES).
 */
VARIANT_CONSTRUCTOR(LilyCheckedDataType *,
                    LilyCheckedDataType,
                    bytes,
                    const Location *location,
                    LilyCheckedDataTypeLen bytes);

/**
 *
 * @brief Construct LilyCheckedDataType type
 * (LILY_CHECKED_DATA_TYPE_KIND_CUSTOM).
 */
VARIANT_CONSTRUCTOR(LilyCheckedDataType *,
                    LilyCheckedDataType,
                    custom,
                    const Location *location,
                    LilyCheckedDataTypeCustom custom);

/**
 *
 * @brief Construct LilyCheckedDataType type
 * (LILY_CHECKED_DATA_TYPE_KIND_LAMBDA).
 */
VARIANT_CONSTRUCTOR(LilyCheckedDataType *,
                    LilyCheckedDataType,
                    lambda,
                    const Location *location,
                    LilyCheckedDataTypeLambda lambda);

/**
 *
 * @brief Construct LilyCheckedDataType type
 * (LILY_CHECKED_DATA_TYPE_KIND_LIST).
 */
VARIANT_CONSTRUCTOR(LilyCheckedDataType *,
                    LilyCheckedDataType,
                    list,
                    const Location *location,
                    LilyCheckedDataType *list);

/**
 *
 * @brief Construct LilyCheckedDataType type (LILY_CHECKED_DATA_TYPE_KIND_MUT).
 */
VARIANT_CONSTRUCTOR(LilyCheckedDataType *,
                    LilyCheckedDataType,
                    mut,
                    const Location *location,
                    LilyCheckedDataType *mut);

/**
 *
 * @brief Construct LilyCheckedDataType type
 * (LILY_CHECKED_DATA_TYPE_KIND_OPTIONAL).
 */
VARIANT_CONSTRUCTOR(LilyCheckedDataType *,
                    LilyCheckedDataType,
                    optional,
                    const Location *location,
                    LilyCheckedDataType *optional);

/**
 *
 * @brief Construct LilyCheckedDataType type (LILY_CHECKED_DATA_TYPE_KIND_PTR).
 */
VARIANT_CONSTRUCTOR(LilyCheckedDataType *,
                    LilyCheckedDataType,
                    ptr,
                    const Location *location,
                    LilyCheckedDataType *ptr);

/**
 *
 * @brief Construct LilyCheckedDataType type
 * (LILY_CHECKED_DATA_TYPE_KIND_PTR_MUT).
 */
VARIANT_CONSTRUCTOR(LilyCheckedDataType *,
                    LilyCheckedDataType,
                    ptr_mut,
                    const Location *location,
                    LilyCheckedDataType *ptr_mut);

/**
 *
 * @brief Construct LilyCheckedDataType type (LILY_CHECKED_DATA_TYPE_KIND_REF).
 */
VARIANT_CONSTRUCTOR(LilyCheckedDataType *,
                    LilyCheckedDataType,
                    ref,
                    const Location *location,
                    LilyCheckedDataType *ref);

/**
 *
 * @brief Construct LilyCheckedDataType type
 * (LILY_CHECKED_DATA_TYPE_KIND_REF_MUT).
 */
VARIANT_CONSTRUCTOR(LilyCheckedDataType *,
                    LilyCheckedDataType,
                    ref_mut,
                    const Location *location,
                    LilyCheckedDataType *ref_mut);

/**
 *
 * @brief Construct LilyCheckedDataType type
 * (LILY_CHECKED_DATA_TYPE_KIND_RESULT).
 */
VARIANT_CONSTRUCTOR(LilyCheckedDataType *,
                    LilyCheckedDataType,
                    result,
                    const Location *location,
                    LilyCheckedDataTypeResult *result);

/**
 *
 * @brief Construct LilyCheckedDataType type (LILY_CHECKED_DATA_TYPE_KIND_STR).
 */
VARIANT_CONSTRUCTOR(LilyCheckedDataType *,
                    LilyCheckedDataType,
                    str,
                    const Location *location,
                    LilyCheckedDataTypeLen str);

/**
 *
 * @brief Construct LilyCheckedDataType type
 * (LILY_CHECKED_DATA_TYPE_KIND_TRACE).
 */
VARIANT_CONSTRUCTOR(LilyCheckedDataType *,
                    LilyCheckedDataType,
                    trace,
                    const Location *location,
                    LilyCheckedDataType *trace);

/**
 *
 * @brief Construct LilyCheckedDataType type
 * (LILY_CHECKED_DATA_TYPE_KIND_TRACE_MUT).
 */
VARIANT_CONSTRUCTOR(LilyCheckedDataType *,
                    LilyCheckedDataType,
                    trace_mut,
                    const Location *location,
                    LilyCheckedDataType *trace_mut);

/**
 *
 * @brief Construct LilyCheckedDataType type
 * (LILY_CHECKED_DATA_TYPE_KIND_TUPLE).
 */
VARIANT_CONSTRUCTOR(LilyCheckedDataType *,
                    LilyCheckedDataType,
                    tuple,
                    const Location *location,
                    Vec *tuple);

/**
 *
 * @brief Construct LilyCheckedDataType type
 * (LILY_CHECKED_DATA_TYPE_KIND_CONDITIONAL_COMPILER_CHOICE).
 */
VARIANT_CONSTRUCTOR(
  LilyCheckedDataType *,
  LilyCheckedDataType,
  conditional_compiler_choice,
  const Location *location,
  LilyCheckedDataTypeConditionalCompilerChoice conditional_compiler_choice);

/**
 *
 * @brief Construct LilyCheckedDataType type
 * (LILY_CHECKED_DATA_TYPE_KIND_COMPILER_CHOICE).
 */
VARIANT_CONSTRUCTOR(LilyCheckedDataType *,
                    LilyCheckedDataType,
                    compiler_choice,
                    const Location *location,
                    Vec *compiler_choice);

/**
 *
 * @brief Construct LilyCheckedDataType type
 * (LILY_CHECKED_DATA_TYPE_KIND_COMPILER_GENERIC).
 */
VARIANT_CONSTRUCTOR(LilyCheckedDataType *,
                    LilyCheckedDataType,
                    compiler_generic,
                    const Location *location,
                    LilyCheckedDataTypeCompilerGeneric compiler_generic);

/**
 *
 * @brief Return true if the both data types are equal otherwise return false.
 */
bool
eq__LilyCheckedDataType(LilyCheckedDataType *self, LilyCheckedDataType *other);

/**
 *
 * @brief Return true if the both data types are equal otherwise return false.
 */
bool
eq_return_data_type__LilyCheckedDataType(LilyCheckedDataType *self,
                                         LilyCheckedDataType *other);

/**
 *
 * @brief Get the return data type according with the given condition.
 * @param cond Vec<LilyCheckedDataType* (&)>*
 * @return LilyCheckedDataType* (&)?
 */
LilyCheckedDataType *
get_return_data_type_of_conditional_compiler_choice(
  const LilyCheckedDataType *self,
  const Vec *cond);

/**
 *
 * @brief Clone LilyCheckedDataType type.
 */
LilyCheckedDataType *
clone__LilyCheckedDataType(LilyCheckedDataType *self);

/**
 *
 * @brief Return true if is a literal data type.
 */
inline bool
is_literal_data_type__LilyCheckedDataType(LilyCheckedDataType *self)
{
    return self->kind != LILY_CHECKED_DATA_TYPE_KIND_CUSTOM;
}

/**
 *
 * @brief Get the integer size of the data type.
 */
Usize
get_integer_size__LilyCheckedDataType(LilyCheckedDataType *self);

/**
 *
 * @brief Check if the data type is an integer.
 */
bool
is_integer_data_type__LilyCheckedDataType(LilyCheckedDataType *self);

/**
 *
 * @brief Check if the data type is a signed integer.
 */
bool
is_signed_integer_data_type__LilyCheckedDataType(LilyCheckedDataType *self);

/**
 *
 * @brief Check if the data type is a float.
 */
bool
is_float_data_type__LilyCheckedDataType(LilyCheckedDataType *self);

/**
 *
 * @brief Check if there is a custom data type in the node.
 * @note A direct data type does not include arrays, lists, tuples.
 */
bool
contains_direct_custom_data_type__LilyCheckedDataType(
  LilyCheckedDataType *self);

/**
 *
 * @brief Check if there is a custom data type.
 * @note A direct data type does not include arrays, lists, tuples.
 */
LilyCheckedDataType *
get_direct_custom_data_type__LilyCheckedDataType(LilyCheckedDataType *self);

/**
 *
 * @brief Update data type to a known data type.
 * @param other LilyCheckedDataType* (&)
 */
void
update_data_type__LilyCheckedDataType(LilyCheckedDataType *self,
                                      LilyCheckedDataType *other);

/**
 *
 * @brief Return true if a string kind (Bytes, Str, CStr).
 */
bool
is_string_data_type__LilyCheckedDataType(LilyCheckedDataType *self);

/**
 *
 * @brief Serialize a data type.
 */
void
serialize__LilyCheckedDataType(LilyCheckedDataType *self, String *ser);

/**
 *
 * @brief Return true if a known data type otherwise return false.
 */
inline bool
is_known_data_type__LilyCheckedDataType(LilyCheckedDataType *self)
{
    return self->kind != LILY_CHECKED_DATA_TYPE_KIND_UNKNOWN;
}

/**
 *
 * @brief Return true if a known data type and a compiler defined data type
 * otherwise return false.
 */
inline bool
is_compiler_defined_and_known_dt__LilyCheckedDataType(LilyCheckedDataType *self)
{
    return is_known_data_type__LilyCheckedDataType(self) &&
           (self->kind ==
              LILY_CHECKED_DATA_TYPE_KIND_CONDITIONAL_COMPILER_CHOICE ||
            self->kind == LILY_CHECKED_DATA_TYPE_KIND_COMPILER_CHOICE ||
            self->kind == LILY_CHECKED_DATA_TYPE_KIND_COMPILER_GENERIC);
}

/**
 *
 * @brief Check if is a compiler defined data type.
 */
inline bool
is_compiler_defined__LilyCheckedDataType(LilyCheckedDataType *self)
{
    return self->kind ==
             LILY_CHECKED_DATA_TYPE_KIND_CONDITIONAL_COMPILER_CHOICE ||
           self->kind == LILY_CHECKED_DATA_TYPE_KIND_COMPILER_CHOICE ||
           self->kind == LILY_CHECKED_DATA_TYPE_KIND_COMPILER_GENERIC ||
           self->kind == LILY_CHECKED_DATA_TYPE_KIND_UNKNOWN;
}

/**
 *
 * @brief Check if is a unknown data type.
 */
inline bool
is_unknown_data_type__LilyCheckedDataType(LilyCheckedDataType *self)
{
    return self->kind == LILY_CHECKED_DATA_TYPE_KIND_UNKNOWN;
}

/**
 *
 * @brief Pass to ref a data type and increment the `ref_count`.
 * @return LilyCheckedDataType* (&)
 */
inline LilyCheckedDataType *
ref__LilyCheckedDataType(LilyCheckedDataType *self)
{
    ++self->ref_count;
    return self;
}

/**
 *
 * @brief Resolve generic data type. If the resolve of the generic data type
 * failed, return NULL.
 * @return LilyCheckedDataType*?
 */
LilyCheckedDataType *
resolve_generic_data_type_with_ordered_hash_map__LilyCheckedDataType(
  LilyCheckedDataType *self,
  OrderedHashMap *generic_params);

/**
 *
 * @brief Resolve generic data type. If the resolve of the generic data type
 * failed, return NULL.
 * @return LilyCheckedDataType*?
 */
LilyCheckedDataType *
resolve_generic_data_type_with_hash_map__LilyCheckedDataType(
  LilyCheckedDataType *self,
  HashMap *generic_params);

/**
 *
 * @brief Check if the contains generic data type.
 */
bool
contains_generic_data_type__LilyCheckedDataType(LilyCheckedDataType *self);

/**
 *
 * @brief Generate generic params from resolved generic data type.
 * @param generic_params Vec<LilyCheckedGenericParam*>* (&)
 */
OrderedHashMap *
generate_generic_params_from_resolved_data_type__LilyCheckedDataType(
  LilyCheckedDataType *self,
  Vec *generic_params,
  LilyCheckedDataType *original);

/**
 *
 * @brief Generate generic params from resolved fields.
 * @param params Vec<LilyCheckedExprCallRecordParam*>* (&)
 * @param generic_params Vec<LilyCheckedGenericParam*>* (&)
 * @param fields Vec<LilyCheckedField*>* (&)
 * @return OrderedHashMap<LilyCheckedDataType*>*
 */
OrderedHashMap *
generate_generic_params_from_resolved_fields__LilyCheckedDataType(
  Vec *params,
  Vec *generic_params,
  Vec *fields);

/**
 *
 * @brief Checks whether the given data type is guaranteed by this type.
 * @param guarantee It cannot equal to `LILY_CHECKED_DATA_TYPE_KIND_CUSTOM` and
 * `LILY_CHECKED_DATA_TYPE_KIND_LAMBDA` and
 * `LILY_CHECKED_DATA_TYPE_KIND_TUPLE` and `LILY_CHECKED_DATA_TYPE_KIND_ARRAY`.
 * @note If the data type corresponds to an unknown data type or a compiler
 * generic, the data type is replaced by the given guaranteed type.
 */
bool
is_guarantee__LilyCheckedDataType(LilyCheckedDataType *self,
                                  enum LilyCheckedDataTypeKind guarantee);

/**
 *
 * @brief Get choice from data type.
 * @return Vec<LilyCheckedDataType*>*? (&)
 */
Vec *
get_choices__LilyCheckedDataType(const LilyCheckedDataType *self);

/**
 *
 * @brief Add a choice to a choices (without duplication).
 */
void
add_choice__LilyCheckedDataType(Vec *choices, LilyCheckedDataType *choice);

/**
 *
 * @brief Check if the data type is updatable.
 */
bool
can_update__LilyCheckedDataType(LilyCheckedDataType *self);

/**
 *
 * @brief Check if is contains the right data type kind.
 */
bool
is_contains_kind__LilyCheckedDataType(LilyCheckedDataType *self,
                                      enum LilyCheckedDataTypeKind kind);

/**
 *
 * @brief Convert LilyCheckedDataTypeKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedDataTypeKind,
               enum LilyCheckedDataTypeKind self);
#endif

/**
 *
 * @brief Convert LilyCheckedDataType in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyCheckedDataType, const LilyCheckedDataType *self);
#endif

/**
 *
 * @brief Print debug LilyCheckedDataType struct.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
void
IMPL_FOR_DEBUG(debug, LilyCheckedDataType, const LilyCheckedDataType *self);
#endif

/**
 *
 * @brief Free LilyCheckedDataType.
 */
DESTRUCTOR(LilyCheckedDataType, LilyCheckedDataType *self);

#endif // LILY_CORE_LILY_ANALYSIS_CHECKED_DATA_TYPE_H
