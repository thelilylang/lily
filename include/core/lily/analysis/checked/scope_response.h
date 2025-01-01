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

#ifndef LILY_CORE_LILY_ANALYSIS_CHECKED_SCOPE_RESPONSE_H
#define LILY_CORE_LILY_ANALYSIS_CHECKED_SCOPE_RESPONSE_H

#include <base/hash_map.h>
#include <base/new.h>

#include <core/shared/location.h>

#include <core/lily/analysis/checked/scope_container.h>

#include <stddef.h>

typedef struct LilyCheckedDecl LilyCheckedDecl;
typedef struct LilyCheckedCapturedVariable LilyCheckedCapturedVariable;
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
typedef struct LilyCheckedField LilyCheckedField;
typedef struct LilyCheckedFieldObject LilyCheckedFieldObject;
typedef struct LilyCheckedVariant LilyCheckedVariant;
typedef struct LilyCheckedGenericParam LilyCheckedGenericParam;

// TODO: add debug for LilyCheckedScopeResponse

enum LilyCheckedScopeResponseKind
{
    LILY_CHECKED_SCOPE_RESPONSE_KIND_MODULE,
    LILY_CHECKED_SCOPE_RESPONSE_KIND_CONSTANT,
    LILY_CHECKED_SCOPE_RESPONSE_KIND_CATCH_VARIABLE,
    LILY_CHECKED_SCOPE_RESPONSE_KIND_CAPTURED_VARIABLE,
    LILY_CHECKED_SCOPE_RESPONSE_KIND_ENUM,
    LILY_CHECKED_SCOPE_RESPONSE_KIND_ENUM_VARIANT,
    LILY_CHECKED_SCOPE_RESPONSE_KIND_ENUM_VARIANT_OBJECT,
    LILY_CHECKED_SCOPE_RESPONSE_KIND_RECORD,
    LILY_CHECKED_SCOPE_RESPONSE_KIND_RECORD_FIELD,
    LILY_CHECKED_SCOPE_RESPONSE_KIND_RECORD_FIELD_OBJECT,
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
    // NOTE: Location is always NULL when kind is equal to
    // LILY_CHECKED_SCOPE_RESPONSE_KIND_FUN.
    const Location *location; // const Location*? (&)
    LilyCheckedScopeContainer
      scope_container; // NOTE: scope_container is undef when kind is equal to
                       // LILY_CHECKED_SCOPE_RESPONSE_KIND_NOT_FOUND or
                       // LILY_CHECKED_SCOPE_RESPONSE_KIND_CATCH
    // NOTE: NULL when it's a function and is not an object or a type or an
    // error declaration.
    LilyCheckedDecl *decl; // LilyCheckedDecl*? (&)
    union
    {
        HashMap *catch_variable; // HashMap<LilyCheckedDataType*>* (&)
        LilyCheckedCapturedVariable
          *captured_variable;              // LilyCheckedCapturedVariable* (&)
        LilyCheckedDeclModule *module;     // LilyCheckedDeclModule* (&)
        LilyCheckedDeclConstant *constant; // LilyCheckedDeclConstant* (&)
        LilyCheckedDeclEnum *enum_;        // LilyCheckedDeclEnum* (&)
        LilyCheckedVariant *enum_variant;  // LilyCheckedVariant* (&)
        LilyCheckedVariant *enum_variant_object; // LilyCheckedVariant* (&)
        LilyCheckedDeclRecord *record;           // LilyCheckedDeclRecord* (&)
        LilyCheckedField *record_field;          // LilyCheckedField* (&)
        LilyCheckedFieldObject
          *record_field_object;      // LilyCheckedFieldObject* (&)
        LilyCheckedDeclAlias *alias; // LilyCheckedDeclAlias* (&)
        LilyCheckedDeclError *error; // LilyCheckedDeclError* (&)
        LilyCheckedDeclEnumObject
          *enum_object; // LilyCheckedDeclEnumObject* (&)
        LilyCheckedDeclRecordObject
          *record_object;                   // LilyCheckedDeclRecordObject* (&)
        LilyCheckedDeclClass *class;        // LilyCheckedDeclClass* (&)
        LilyCheckedDeclTrait *trait;        // LilyCheckedDeclTrait* (&)
        Vec *fun;                           // Vec<LilyCheckedDecl* (&)>*
        LilyCheckedStmtVariable *variable;  // LilyCheckedStmtVariable* (&)
        LilyCheckedDeclFunParam *fun_param; // LilyCheckedDeclFunParam* (&)
        LilyCheckedDeclMethodParam
          *method_param;                  // LilyCheckedDeclMethodParam* (&)
        LilyCheckedGenericParam *generic; // LilyCheckedGenericParam* (&)
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
 * (LILY_CHECKED_SCOPE_RESPONSE_KIND_CATCH).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedScopeResponse,
                           LilyCheckedScopeResponse,
                           catch_variable,
                           const Location *location,
                           HashMap *catch_variable)
{
    return (LilyCheckedScopeResponse){
        .kind = LILY_CHECKED_SCOPE_RESPONSE_KIND_CATCH_VARIABLE,
        .location = location,
        .catch_variable = catch_variable
    };
}

/**
 *
 * @brief Construct LilyCheckedScopeResponse type
 * (LILY_CHECKED_SCOPE_RESPONSE_KIND_CAPTURED_VARIABLE).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedScopeResponse,
                           LilyCheckedScopeResponse,
                           captured_variable,
                           const Location *location,
                           LilyCheckedScopeContainer scope_container,
                           LilyCheckedCapturedVariable *captured_variable)
{
    return (LilyCheckedScopeResponse){
        .kind = LILY_CHECKED_SCOPE_RESPONSE_KIND_CAPTURED_VARIABLE,
        .location = location,
        .scope_container = scope_container,
        .captured_variable = captured_variable
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
                           LilyCheckedScopeContainer scope_container,
                           LilyCheckedDeclModule *module)
{
    return (LilyCheckedScopeResponse){
        .kind = LILY_CHECKED_SCOPE_RESPONSE_KIND_MODULE,
        .location = location,
        .scope_container = scope_container,
        .decl = NULL,
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
                           LilyCheckedScopeContainer scope_container,
                           LilyCheckedDeclConstant *constant)
{
    return (LilyCheckedScopeResponse){
        .kind = LILY_CHECKED_SCOPE_RESPONSE_KIND_CONSTANT,
        .location = location,
        .scope_container = scope_container,
        .decl = NULL,
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
                           LilyCheckedScopeContainer scope_container,
                           LilyCheckedDecl *decl,
                           LilyCheckedDeclEnum *enum_)
{
    return (LilyCheckedScopeResponse){ .kind =
                                         LILY_CHECKED_SCOPE_RESPONSE_KIND_ENUM,
                                       .location = location,
                                       .scope_container = scope_container,
                                       .decl = decl,
                                       .enum_ = enum_ };
}

/**
 *
 * @brief Construct LilyCheckedScopeResponse type
 * (LILY_CHECKED_SCOPE_RESPONSE_KIND_ENUM_VARIANT).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedScopeResponse,
                           LilyCheckedScopeResponse,
                           enum_variant,
                           const Location *location,
                           LilyCheckedScopeContainer scope_container,
                           LilyCheckedVariant *enum_variant)
{
    return (LilyCheckedScopeResponse){
        .kind = LILY_CHECKED_SCOPE_RESPONSE_KIND_ENUM_VARIANT,
        .location = location,
        .scope_container = scope_container,
        .decl = NULL,
        .enum_variant = enum_variant
    };
}

/**
 *
 * @brief Construct LilyCheckedScopeResponse type
 * (LILY_CHECKED_SCOPE_RESPONSE_KIND_ENUM_VARIANT_OBJECT).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedScopeResponse,
                           LilyCheckedScopeResponse,
                           enum_variant_object,
                           const Location *location,
                           LilyCheckedScopeContainer scope_container,
                           LilyCheckedVariant *enum_variant_object)
{
    return (LilyCheckedScopeResponse){
        .kind = LILY_CHECKED_SCOPE_RESPONSE_KIND_ENUM_VARIANT_OBJECT,
        .location = location,
        .scope_container = scope_container,
        .decl = NULL,
        .enum_variant_object = enum_variant_object
    };
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
                           LilyCheckedScopeContainer scope_container,
                           LilyCheckedDecl *decl,
                           LilyCheckedDeclRecord *record)
{
    return (LilyCheckedScopeResponse){
        .kind = LILY_CHECKED_SCOPE_RESPONSE_KIND_RECORD,
        .location = location,
        .scope_container = scope_container,
        .decl = decl,
        .record = record
    };
}

/**
 *
 * @brief Construct LilyCheckedScopeResponse type
 * (LILY_CHECKED_SCOPE_RESPONSE_KIND_RECORD_FIELD).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedScopeResponse,
                           LilyCheckedScopeResponse,
                           record_field,
                           const Location *location,
                           LilyCheckedScopeContainer scope_container,
                           LilyCheckedField *record_field)
{
    return (LilyCheckedScopeResponse){
        .kind = LILY_CHECKED_SCOPE_RESPONSE_KIND_RECORD_FIELD,
        .location = location,
        .scope_container = scope_container,
        .decl = NULL,
        .record_field = record_field
    };
}

/**
 *
 * @brief Construct LilyCheckedScopeResponse type
 * (LILY_CHECKED_SCOPE_RESPONSE_KIND_RECORD_FIELD_OBJECT).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedScopeResponse,
                           LilyCheckedScopeResponse,
                           record_field_object,
                           const Location *location,
                           LilyCheckedScopeContainer scope_container,
                           LilyCheckedFieldObject *record_field_object)
{
    return (LilyCheckedScopeResponse){
        .kind = LILY_CHECKED_SCOPE_RESPONSE_KIND_RECORD_FIELD_OBJECT,
        .location = location,
        .scope_container = scope_container,
        .decl = NULL,
        .record_field_object = record_field_object
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
                           LilyCheckedScopeContainer scope_container,
                           LilyCheckedDecl *decl,
                           LilyCheckedDeclAlias *alias)
{
    return (LilyCheckedScopeResponse){ .kind =
                                         LILY_CHECKED_SCOPE_RESPONSE_KIND_ALIAS,
                                       .location = location,
                                       .scope_container = scope_container,
                                       .decl = decl,
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
                           LilyCheckedScopeContainer scope_container,
                           LilyCheckedDecl *decl,
                           LilyCheckedDeclError *error)
{
    return (LilyCheckedScopeResponse){ .kind =
                                         LILY_CHECKED_SCOPE_RESPONSE_KIND_ERROR,
                                       .location = location,
                                       .scope_container = scope_container,
                                       .decl = decl,
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
                           LilyCheckedScopeContainer scope_container,
                           LilyCheckedDecl *decl,
                           LilyCheckedDeclEnumObject *enum_object)
{
    return (LilyCheckedScopeResponse){
        .kind = LILY_CHECKED_SCOPE_RESPONSE_KIND_ENUM_OBJECT,
        .location = location,
        .scope_container = scope_container,
        .decl = decl,
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
                           LilyCheckedScopeContainer scope_container,
                           LilyCheckedDecl *decl,
                           LilyCheckedDeclRecordObject *record_object)
{
    return (LilyCheckedScopeResponse){
        .kind = LILY_CHECKED_SCOPE_RESPONSE_KIND_RECORD_OBJECT,
        .location = location,
        .scope_container = scope_container,
        .decl = decl,
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
                           LilyCheckedScopeContainer scope_container,
                           LilyCheckedDecl *decl,
                           LilyCheckedDeclClass *class)
{
    return (LilyCheckedScopeResponse){ .kind =
                                         LILY_CHECKED_SCOPE_RESPONSE_KIND_CLASS,
                                       .location = location,
                                       .scope_container = scope_container,
                                       .decl = decl,
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
                           LilyCheckedScopeContainer scope_container,
                           LilyCheckedDecl *decl,
                           LilyCheckedDeclTrait *trait)
{
    return (LilyCheckedScopeResponse){ .kind =
                                         LILY_CHECKED_SCOPE_RESPONSE_KIND_TRAIT,
                                       .location = location,
                                       .scope_container = scope_container,
                                       .decl = decl,
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
                           LilyCheckedScopeContainer scope_container,
                           Vec *fun)
{
    return (LilyCheckedScopeResponse){ .kind =
                                         LILY_CHECKED_SCOPE_RESPONSE_KIND_FUN,
                                       .location = location,
                                       .scope_container = scope_container,
                                       .decl = NULL,
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
                           const Location *location,
                           LilyCheckedScopeContainer scope_container)
{
    return (LilyCheckedScopeResponse){
        .kind = LILY_CHECKED_SCOPE_RESPONSE_KIND_LABEL,
        .location = location,
        .decl = NULL,
        .scope_container = scope_container,
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
                           LilyCheckedScopeContainer scope_container,
                           LilyCheckedStmtVariable *variable)
{
    return (LilyCheckedScopeResponse){
        .kind = LILY_CHECKED_SCOPE_RESPONSE_KIND_VARIABLE,
        .location = location,
        .scope_container = scope_container,
        .decl = NULL,
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
                           LilyCheckedScopeContainer scope_container,
                           LilyCheckedDeclFunParam *fun_param)
{
    return (LilyCheckedScopeResponse){
        .kind = LILY_CHECKED_SCOPE_RESPONSE_KIND_FUN_PARAM,
        .location = location,
        .scope_container = scope_container,
        .decl = NULL,
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
                           LilyCheckedScopeContainer scope_container,
                           LilyCheckedDeclMethodParam *method_param)
{
    return (LilyCheckedScopeResponse){
        .kind = LILY_CHECKED_SCOPE_RESPONSE_KIND_METHOD_PARAM,
        .location = location,
        .scope_container = scope_container,
        .decl = NULL,
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
                           LilyCheckedScopeContainer scope_container,
                           LilyCheckedGenericParam *generic)
{
    return (LilyCheckedScopeResponse){
        .kind = LILY_CHECKED_SCOPE_RESPONSE_KIND_GENERIC,
        .location = location,
        .scope_container = scope_container,
        .decl = NULL,
        .generic = generic
    };
}

/**
 *
 * @brief Free LilyCheckedScopeResponse type.
 * @note The destructor is only needed to call when kind is equal to
 * LILY_CHECKED_SCOPE_RESPONSE_KIND_FUN.
 */
inline DESTRUCTOR(LilyCheckedScopeResponse,
                  const LilyCheckedScopeResponse *self)
{
    if (self->kind == LILY_CHECKED_SCOPE_RESPONSE_KIND_FUN) {
        FREE(Vec, self->fun);
    }
}

#endif // LILY_CORE_LILY_ANALYSIS_CHECKED_SCOPE_RESPONSE_H
