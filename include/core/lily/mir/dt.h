/*
 * MIT License
 *
 * Copyright (c) 2022-2026 ArthurPV
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

#ifndef LILY_CORE_LILY_MIR_DT_H
#define LILY_CORE_LILY_MIR_DT_H

#include <base/new.h>
#include <base/types.h>
#include <base/vec.h>

#ifdef ENV_DEBUG
#include <base/string.h>
#endif

#include <core/lily/analysis/checked/data_type.h>

// TODO: add lambda data type
// TODO: add ref count

typedef struct LilyMirDt LilyMirDt;

enum LilyMirDtKind
{
    LILY_MIR_DT_KIND_ANY,
    LILY_MIR_DT_KIND_ARRAY,
    LILY_MIR_DT_KIND_BYTES,
    LILY_MIR_DT_KIND_CSTR,
    LILY_MIR_DT_KIND_RESULT,
    LILY_MIR_DT_KIND_I1,
    LILY_MIR_DT_KIND_I8,
    LILY_MIR_DT_KIND_I16,
    LILY_MIR_DT_KIND_I32,
    LILY_MIR_DT_KIND_I64,
    LILY_MIR_DT_KIND_ISIZE,
    LILY_MIR_DT_KIND_F32,
    LILY_MIR_DT_KIND_F64,
    LILY_MIR_DT_KIND_LIST,
    LILY_MIR_DT_KIND_PTR,
    LILY_MIR_DT_KIND_REF,
    LILY_MIR_DT_KIND_STR,
    LILY_MIR_DT_KIND_STRUCT,
    LILY_MIR_DT_KIND_STRUCT_NAME,
    LILY_MIR_DT_KIND_TRACE,
    LILY_MIR_DT_KIND_TUPLE,
    LILY_MIR_DT_KIND_U8,
    LILY_MIR_DT_KIND_U16,
    LILY_MIR_DT_KIND_U32,
    LILY_MIR_DT_KIND_U64,
    LILY_MIR_DT_KIND_UNIT,
    LILY_MIR_DT_KIND_USIZE,
    LILY_MIR_DT_KIND_C_VA_ARG
};

typedef struct LilyMirDtLen
{
    Usize len; // can be undef
    bool is_undef;
} LilyMirDtLen;

/**
 *
 * @brief Construct LilyMirDtLen type (undef variant).
 */
inline VARIANT_CONSTRUCTOR(LilyMirDtLen, LilyMirDtLen, undef)
{
    return (LilyMirDtLen){
        .is_undef = true,
    };
}

/**
 *
 * @brief Construct LilyMirDtLen type (def variant).
 */
inline VARIANT_CONSTRUCTOR(LilyMirDtLen, LilyMirDtLen, def, Usize len)
{
    return (LilyMirDtLen){ .is_undef = false, .len = len };
}

/**
 *
 * @brief Clone LilyMirDtLen type.
 */
inline LilyMirDtLen
clone__LilyMirDtLen(const LilyMirDtLen *self)
{
    return self->is_undef ? NEW_VARIANT(LilyMirDtLen, undef)
                          : NEW_VARIANT(LilyMirDtLen, def, self->len);
}

/**
 *
 * @brief Check if the both LilyMirDtLen are equal.
 */
inline bool
eq__LilyMirDtLen(const LilyMirDtLen *self, const LilyMirDtLen *other)
{
    return self->is_undef == other->is_undef ? true : self->len == other->len;
}

/**
 *
 * @brief Convert LilyCheckedDataTypeLen to LilyMirDtLen type.
 */
inline LilyMirDtLen
from_check_len__LilyMirDtLen(const LilyCheckedDataTypeLen *self)
{
    return self->is_undef ? NEW_VARIANT(LilyMirDtLen, undef)
                          : NEW_VARIANT(LilyMirDtLen, def, self->len);
}

typedef struct LilyMirDtArray
{
    LilyMirDtLen len;
    LilyMirDt *dt;
} LilyMirDtArray;

DESTRUCTOR(LilyMirDt, LilyMirDt *self);

/**
 *
 * @brief Construct LilyMirDtArray type.
 */
inline CONSTRUCTOR(LilyMirDtArray,
                   LilyMirDtArray,
                   LilyMirDtLen len,
                   LilyMirDt *dt)
{
    return (LilyMirDtArray){ .len = len, .dt = dt };
}

/**
 *
 * @brief Free LilyMirDtArray type.
 */
inline DESTRUCTOR(LilyMirDtArray, const LilyMirDtArray *self)
{
    FREE(LilyMirDt, self->dt);
}

typedef struct LilyMirDtResult
{
    LilyMirDt *ok;
    LilyMirDt *err;
} LilyMirDtResult;

/**
 *
 * @brief Construct LilyMirDtResult type.
 */
inline CONSTRUCTOR(LilyMirDtResult,
                   LilyMirDtResult,
                   LilyMirDt *ok,
                   LilyMirDt *err)
{
    return (LilyMirDtResult){ .ok = ok, .err = err };
}

/**
 *
 * @brief Free LilyMirDtResult type.
 */
inline DESTRUCTOR(LilyMirDtResult, const LilyMirDtResult *self)
{
    FREE(LilyMirDt, self->ok);
    FREE(LilyMirDt, self->err);
}

typedef struct LilyMirDt
{
    enum LilyMirDtKind kind;
    union
    {
        LilyMirDtArray array;
        LilyMirDtLen bytes;
        LilyMirDtLen cstr;
        LilyMirDt *list;
        LilyMirDt *ptr;
        LilyMirDt *ref;
        LilyMirDtResult result;
        LilyMirDtLen str;
        Vec *struct_;            // Vec<LilyMirDt*>*
        const char *struct_name; // const char* (&)
        LilyMirDt *trace;
        Vec *tuple; // Vec<LilyMirDt*>*
    };
} LilyMirDt;

/**
 *
 * @brief Construct LilyMirDt type.
 */
CONSTRUCTOR(LilyMirDt *, LilyMirDt, enum LilyMirDtKind kind);

/**
 *
 * @brief Construct LilyMirDt type (LILY_MIR_DT_KIND_ARRAY).
 */
VARIANT_CONSTRUCTOR(LilyMirDt *, LilyMirDt, array, LilyMirDtArray array);

/**
 *
 * @brief Construct LilyMirDt type (LILY_MIR_DT_KIND_BYTES).
 */
VARIANT_CONSTRUCTOR(LilyMirDt *, LilyMirDt, bytes, LilyMirDtLen bytes);

/**
 *
 * @brief Construct LilyMirDt type (LILY_MIR_DT_KIND_CSTR).
 */
VARIANT_CONSTRUCTOR(LilyMirDt *, LilyMirDt, cstr, LilyMirDtLen cstr);

/**
 *
 * @brief Construct LilyMirDt type (LILY_MIR_DT_KIND_LIST).
 */
VARIANT_CONSTRUCTOR(LilyMirDt *, LilyMirDt, list, LilyMirDt *list);

/**
 *
 * @brief Construct LilyMirDt type (LILY_MIR_DT_KIND_PTR).
 */
VARIANT_CONSTRUCTOR(LilyMirDt *, LilyMirDt, ptr, LilyMirDt *ptr);

/**
 *
 * @brief Construct LilyMirDt type (LILY_MIR_DT_KIND_REF).
 */
VARIANT_CONSTRUCTOR(LilyMirDt *, LilyMirDt, ref, LilyMirDt *ref);

/**
 *
 * @brief Construct LilyMirDt type (LILY_MIR_DT_KIND_RESULT).
 */
VARIANT_CONSTRUCTOR(LilyMirDt *, LilyMirDt, result, LilyMirDtResult result);

/**
 *
 * @brief Construct LilyMirDt type (LILY_MIR_DT_KIND_STR).
 */
VARIANT_CONSTRUCTOR(LilyMirDt *, LilyMirDt, str, LilyMirDtLen str);

/**
 *
 * @brief Construct LilyMirDt type (LILY_MIR_DT_KIND_STRUCT).
 */
VARIANT_CONSTRUCTOR(LilyMirDt *, LilyMirDt, struct, Vec *struct_);

/**
 *
 * @brief Construct LilyMirDt type (LILY_MIR_DT_KIND_STRUCT_NAME).
 */
VARIANT_CONSTRUCTOR(LilyMirDt *,
                    LilyMirDt,
                    struct_name,
                    const char *struct_name);

/**
 *
 * @brief Construct LilyMirDt type (LILY_MIR_DT_KIND_TRACE).
 */
VARIANT_CONSTRUCTOR(LilyMirDt *, LilyMirDt, trace, LilyMirDt *trace);

/**
 *
 * @brief Construct LilyMirDt type (LILY_MIR_DT_KIND_STRUCT_TUPLE).
 */
VARIANT_CONSTRUCTOR(LilyMirDt *, LilyMirDt, tuple, Vec *tuple);

/**
 *
 * @brief Clone LilyMirDt type.
 */
LilyMirDt *
clone__LilyMirDt(LilyMirDt *self);

/**
 *
 * @brief Check if the two given LilyMirDt values are equal.
 */
bool
eq__LilyMirDt(LilyMirDt *self, LilyMirDt *other);

/**
 *
 * @brief Return true if the data type is signed, otherwise return false.
 */
bool
is_signed__LilyMirDt(LilyMirDt *self);

/**
 *
 * @brief Convert LilyMirDt in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyMirDt, const LilyMirDt *self);
#endif

/**
 *
 * @brief Free LilyMirDt type.
 */
DESTRUCTOR(LilyMirDt, LilyMirDt *self);

#endif // LILY_CORE_LILY_MIR_DT_H
