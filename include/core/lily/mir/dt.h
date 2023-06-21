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

#ifndef LILY_CORE_LILY_MIR_DT_H
#define LILY_CORE_LILY_MIR_DT_H

#include <base/new.h>
#include <base/types.h>
#include <base/vec.h>

#ifdef ENV_DEBUG
#include <base/string.h>
#endif

// TODO: add lambda data type

typedef struct LilyMirDt LilyMirDt;

enum LilyMirDtKind
{
    LILY_MIR_DT_KIND_ANY,
    LILY_MIR_DT_KIND_ARRAY,
    LILY_MIR_DT_KIND_BYTES,
    LILY_MIR_DT_KIND_EXCEPTION,
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

typedef struct LilyMirDtArray
{
    Usize len;
    LilyMirDt *dt;
} LilyMirDtArray;

DESTRUCTOR(LilyMirDt, LilyMirDt *self);

/**
 *
 * @brief Construct LilyMirDtArray type.
 */
inline CONSTRUCTOR(LilyMirDtArray, LilyMirDtArray, Usize len, LilyMirDt *dt)
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

typedef struct LilyMirDtException
{
    LilyMirDt *ok;
    LilyMirDt *err;
} LilyMirDtException;

/**
 *
 * @brief Construct LilyMirDtException type.
 */
inline CONSTRUCTOR(LilyMirDtException,
                   LilyMirDtException,
                   LilyMirDt *ok,
                   LilyMirDt *err)
{
    return (LilyMirDtException){ .ok = ok, .err = err };
}

/**
 *
 * @brief Free LilyMirDtException type.
 */
inline DESTRUCTOR(LilyMirDtException, const LilyMirDtException *self)
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
        Usize bytes;
        LilyMirDtException exception;
        LilyMirDt *list;
        LilyMirDt *ptr;
        LilyMirDt *ref;
        Usize str;
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
VARIANT_CONSTRUCTOR(LilyMirDt *, LilyMirDt, bytes, Usize bytes);

/**
 *
 * @brief Construct LilyMirDt type (LILY_MIR_DT_KIND_EXCEPTION).
 */
VARIANT_CONSTRUCTOR(LilyMirDt *,
                    LilyMirDt,
                    exception,
                    LilyMirDtException exception);

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
 * @brief Construct LilyMirDt type (LILY_MIR_DT_KIND_STR).
 */
VARIANT_CONSTRUCTOR(LilyMirDt *, LilyMirDt, str, Usize str);

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
