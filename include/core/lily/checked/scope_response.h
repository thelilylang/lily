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

#ifndef LILY_CORE_LILY_CHECKED_SCOPE_RESPONSE_H
#define LILY_CORE_LILY_CHECKED_SCOPE_RESPONSE_H

#include <core/shared/location.h>

#include <stddef.h>

typedef struct LilyCheckedDeclModule LilyCheckedDeclModule;
typedef struct LilyCheckedDeclConstant LilyCheckedDeclConstant;
typedef struct LilyCheckedDeclEnum LilyCheckedDeclEnum;
typedef struct LilyCheckedDeclRecord LilyCheckedDeclRecord;
typedef struct LilyCheckedDeclAlias LilyCheckedDeclAlias;
typedef struct LilyCheckedDeclError LilyCheckedDeclError;
typedef struct LilyCheckedDeclEnumObject LilyCheckedDeclEnumObject;
typedef struct LilyCheckedDeclRecordObject LilyCheckedDeclRecordObject;
typedef struct LilyCheckedDeclClass LilyCheckedDeclClass;
typedef struct LilyCheckedDeclTrait LilyCheckedDeclTrait;
typedef struct LilyCheckedDeclFun LilyCheckedDeclFun;
typedef struct LilyCheckedStmtVariable LilyCheckedStmtVariable;
typedef struct LilyCheckedDeclFunParam LilyCheckedDeclFunParam;
typedef struct LilyCheckedDeclMethodParam LilyCheckedDeclMethodParam;
typedef struct LilyCheckedGenericParam LilyCheckedGenericParam;

// TODO: add debug for LilyCheckedScopeResponse

enum LilyCheckedScopeResponseKind
{
    LILY_CHECKED_SCOPE_RESPONSE_KIND_MODULE,
    LILY_CHECKED_SCOPE_RESPONSE_KIND_CONSTANT,
    LILY_CHECKED_SCOPE_RESPONSE_KIND_ENUM,
    LILY_CHECKED_SCOPE_RESPONSE_KIND_RECORD,
    LILY_CHECKED_SCOPE_RESPONSE_KIND_ALIAS,
    LILY_CHECKED_SCOPE_RESPONSE_KIND_ERROR,
    LILY_CHECKED_SCOPE_RESPONSE_KIND_ENUM_OBJECT,
    LILY_CHECKED_SCOPE_RESPONSE_KIND_RECORD_OBJECT,
    LILY_CHECKED_SCOPE_RESPONSE_KIND_CLASS,
    LILY_CHECKED_SCOPE_RESPONSE_KIND_TRAIT,
    LILY_CHECKED_SCOPE_RESPONSE_KIND_FUN,
    LILY_CHECKED_SCOPE_RESPONSE_KIND_LABEL,
    LILY_CHECKED_SCOPE_RESPONSE_KIND_VARIABLE,
    LILY_CHECKED_SCOPE_RESPONSE_KIND_FUN_PARAM,
    LILY_CHECKED_SCOPE_RESPONSE_KIND_METHOD_PARAM,
    LILY_CHECKED_SCOPE_RESPONSE_KIND_GENERIC,
    LILY_CHECKED_SCOPE_RESPONSE_KIND_NOT_FOUND
};

typedef struct LilyCheckedScopeResponse
{
    enum LilyCheckedScopeResponseKind kind;
    const Location *location; // const Location* (&)
    union
    {
        LilyCheckedDeclModule *module;
        LilyCheckedDeclConstant *constant;
        LilyCheckedDeclEnum *enum_;
        LilyCheckedDeclRecord *record;
        LilyCheckedDeclAlias *alias;
        LilyCheckedDeclError *error;
        LilyCheckedDeclEnumObject *enum_object;
        LilyCheckedDeclRecordObject *record_object;
        LilyCheckedDeclClass *class;
        LilyCheckedDeclTrait *trait;
        LilyCheckedDeclFun *fun;
        LilyCheckedStmtVariable *variable;
        LilyCheckedDeclFunParam *fun_param;
        LilyCheckedDeclMethodParam *method_param;
        LilyCheckedGenericParam *generic;
    };
} LilyCheckedScopeResponse;

/**
 *
 * @brief Construct LilyCheckedScopeResponse type.
 */
inline CONSTRUCTOR(LilyCheckedScopeResponse, LilyCheckedScopeResponse)
{
    return (LilyCheckedScopeResponse){
        .kind = LILY_CHECKED_SCOPE_RESPONSE_KIND_NOT_FOUND, .location = NULL
    };
}

/**
 *
 * @brief Construct LilyCheckedScopeResponse type
 * (LILY_CHECKED_SCOPE_RESPONSE_KIND_MODULE).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedScopeResponse,
                           LilyCheckedScopeResponse,
                           module,
                           const Location *location,
                           LilyCheckedDeclModule *module)
{
    return (LilyCheckedScopeResponse){
        .kind = LILY_CHECKED_SCOPE_RESPONSE_KIND_MODULE,
        .location = location,
        .module = module
    };
}

/**
 *
 * @brief Construct LilyCheckedScopeResponse type
 * (LILY_CHECKED_SCOPE_RESPONSE_KIND_CONSTANT).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedScopeResponse,
                           LilyCheckedScopeResponse,
                           constant,
                           const Location *location,
                           LilyCheckedDeclConstant *constant)
{
    return (LilyCheckedScopeResponse){
        .kind = LILY_CHECKED_SCOPE_RESPONSE_KIND_CONSTANT,
        .location = location,
        .constant = constant
    };
}

/**
 *
 * @brief Construct LilyCheckedScopeResponse type
 * (LILY_CHECKED_SCOPE_RESPONSE_KIND_ENUM).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedScopeResponse,
                           LilyCheckedScopeResponse,
                           enum_,
                           const Location *location,
                           LilyCheckedDeclEnum *enum_)
{
    return (LilyCheckedScopeResponse){ .kind =
                                         LILY_CHECKED_SCOPE_RESPONSE_KIND_ENUM,
                                       .location = location,
                                       .enum_ = enum_ };
}

/**
 *
 * @brief Construct LilyCheckedScopeResponse type
 * (LILY_CHECKED_SCOPE_RESPONSE_KIND_RECORD).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedScopeResponse,
                           LilyCheckedScopeResponse,
                           record,
                           const Location *location,
                           LilyCheckedDeclRecord *record)
{
    return (LilyCheckedScopeResponse){
        .kind = LILY_CHECKED_SCOPE_RESPONSE_KIND_RECORD,
        .location = location,
        .record = record
    };
}

/**
 *
 * @brief Construct LilyCheckedScopeResponse type
 * (LILY_CHECKED_SCOPE_RESPONSE_KIND_ALIAS).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedScopeResponse,
                           LilyCheckedScopeResponse,
                           alias,
                           const Location *location,
                           LilyCheckedDeclAlias *alias)
{
    return (LilyCheckedScopeResponse){ .kind =
                                         LILY_CHECKED_SCOPE_RESPONSE_KIND_ALIAS,
                                       .location = location,
                                       .alias = alias };
}

/**
 *
 * @brief Construct LilyCheckedScopeResponse type
 * (LILY_CHECKED_SCOPE_RESPONSE_KIND_ERROR).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedScopeResponse,
                           LilyCheckedScopeResponse,
                           error,
                           const Location *location,
                           LilyCheckedDeclError *error)
{
    return (LilyCheckedScopeResponse){ .kind =
                                         LILY_CHECKED_SCOPE_RESPONSE_KIND_ERROR,
                                       .location = location,
                                       .error = error };
}

/**
 *
 * @brief Construct LilyCheckedScopeResponse type
 * (LILY_CHECKED_SCOPE_RESPONSE_KIND_ENUM_OBJECT).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedScopeResponse,
                           LilyCheckedScopeResponse,
                           enum_object,
                           const Location *location,
                           LilyCheckedDeclEnumObject *enum_object)
{
    return (LilyCheckedScopeResponse){
        .kind = LILY_CHECKED_SCOPE_RESPONSE_KIND_ENUM_OBJECT,
        .location = location,
        .enum_object = enum_object
    };
}

/**
 *
 * @brief Construct LilyCheckedScopeResponse type
 * (LILY_CHECKED_SCOPE_RESPONSE_KIND_RECORD_OBJECT).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedScopeResponse,
                           LilyCheckedScopeResponse,
                           record_object,
                           const Location *location,
                           LilyCheckedDeclRecordObject *record_object)
{
    return (LilyCheckedScopeResponse){
        .kind = LILY_CHECKED_SCOPE_RESPONSE_KIND_RECORD_OBJECT,
        .location = location,
        .record_object = record_object
    };
}

/**
 *
 * @brief Construct LilyCheckedScopeResponse type
 * (LILY_CHECKED_SCOPE_RESPONSE_KIND_CLASS).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedScopeResponse,
                           LilyCheckedScopeResponse,
                           class,
                           const Location *location,
                           LilyCheckedDeclClass *class)
{
    return (LilyCheckedScopeResponse){ .kind =
                                         LILY_CHECKED_SCOPE_RESPONSE_KIND_CLASS,
                                       .location = location,
                                       .class = class };
}

/**
 *
 * @brief Construct LilyCheckedScopeResponse type
 * (LILY_CHECKED_SCOPE_RESPONSE_KIND_TRAIT).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedScopeResponse,
                           LilyCheckedScopeResponse,
                           trait,
                           const Location *location,
                           LilyCheckedDeclTrait *trait)
{
    return (LilyCheckedScopeResponse){ .kind =
                                         LILY_CHECKED_SCOPE_RESPONSE_KIND_TRAIT,
                                       .location = location,
                                       .trait = trait };
}

/**
 *
 * @brief Construct LilyCheckedScopeResponse type
 * (LILY_CHECKED_SCOPE_RESPONSE_KIND_FUN).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedScopeResponse,
                           LilyCheckedScopeResponse,
                           fun,
                           const Location *location,
                           LilyCheckedDeclFun *fun)
{
    return (LilyCheckedScopeResponse){ .kind =
                                         LILY_CHECKED_SCOPE_RESPONSE_KIND_FUN,
                                       .location = location,
                                       .fun = fun };
}

/**
 *
 * @brief Construct LilyCheckedScopeResponse type
 * (LILY_CHECKED_SCOPE_RESPONSE_KIND_LABEL).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedScopeResponse,
                           LilyCheckedScopeResponse,
                           label,
                           const Location *location)
{
    return (LilyCheckedScopeResponse){
        .kind = LILY_CHECKED_SCOPE_RESPONSE_KIND_LABEL,
        .location = location,
    };
}

/**
 *
 * @brief Construct LilyCheckedScopeResponse type
 * (LILY_CHECKED_SCOPE_RESPONSE_KIND_VARIABLE).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedScopeResponse,
                           LilyCheckedScopeResponse,
                           variable,
                           const Location *location,
                           LilyCheckedStmtVariable *variable)
{
    return (LilyCheckedScopeResponse){
        .kind = LILY_CHECKED_SCOPE_RESPONSE_KIND_VARIABLE,
        .location = location,
        .variable = variable
    };
}

/**
 *
 * @brief Construct LilyCheckedScopeResponse type
 * (LILY_CHECKED_SCOPE_RESPONSE_KIND_FUN_PARAM).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedScopeResponse,
                           LilyCheckedScopeResponse,
                           fun_param,
                           const Location *location,
                           LilyCheckedDeclFunParam *fun_param)
{
    return (LilyCheckedScopeResponse){
        .kind = LILY_CHECKED_SCOPE_RESPONSE_KIND_FUN_PARAM,
        .location = location,
        .fun_param = fun_param
    };
}

/**
 *
 * @brief Construct LilyCheckedScopeResponse type
 * (LILY_CHECKED_SCOPE_RESPONSE_KIND_METHOD_PARAM).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedScopeResponse,
                           LilyCheckedScopeResponse,
                           method_param,
                           const Location *location,
                           LilyCheckedDeclMethodParam *method_param)
{
    return (LilyCheckedScopeResponse){
        .kind = LILY_CHECKED_SCOPE_RESPONSE_KIND_METHOD_PARAM,
        .location = location,
        .method_param = method_param
    };
}

/**
 *
 * @brief Construct LilyCheckedScopeResponse type
 * (LILY_CHECKED_SCOPE_RESPONSE_KIND_GENERIC).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedScopeResponse,
                           LilyCheckedScopeResponse,
                           generic,
                           const Location *location,
                           LilyCheckedGenericParam *generic)
{
    return (LilyCheckedScopeResponse){
        .kind = LILY_CHECKED_SCOPE_RESPONSE_KIND_GENERIC,
        .location = location,
        .generic = generic
    };
}

#endif // LILY_CORE_LILY_CHECKED_SCOPE_RESPONSE_H
