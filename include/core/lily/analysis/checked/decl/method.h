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

#ifndef LILY_CORE_LILY_ANALYSIS_CHECKED_DECL_METHOD_H
#define LILY_CORE_LILY_ANALYSIS_CHECKED_DECL_METHOD_H

#include <base/macros.h>
#include <base/string.h>

#include <core/lily/analysis/checked/access.h>
#include <core/lily/analysis/checked/body/fun.h>
#include <core/lily/analysis/checked/data_type.h>
#include <core/lily/analysis/checked/expr.h>
#include <core/lily/analysis/checked/generic_param.h>
#include <core/lily/analysis/checked/scope.h>
#include <core/lily/shared/visibility.h>
#include <core/shared/location.h>

enum LilyCheckedDeclMethodParamKind
{
    LILY_CHECKED_DECL_METHOD_PARAM_KIND_DEFAULT,
    LILY_CHECKED_DECL_METHOD_PARAM_KIND_NORMAL,
    LILY_CHECKED_DECL_METHOD_PARAM_KIND_MUT_SELF,
    LILY_CHECKED_DECL_METHOD_PARAM_KIND_REF_MUT_SELF,
    LILY_CHECKED_DECL_METHOD_PARAM_KIND_REF_SELF,
    LILY_CHECKED_DECL_METHOD_PARAM_KIND_SELF
};

/**
 *
 * @brief Convert LilyCheckedDeclMethodParamKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedDeclMethodParamKind,
               enum LilyCheckedDeclMethodParamKind self);
#endif

typedef struct LilyCheckedDeclMethodParam
{
    String *name; // String*? (&) only if kind is equal to
                  // LILY_CHECKED_DECL_FUN_PARAM_KIND_SELF,
                  // LILY_CHECKED_DECL_FUN_PARAM_KIND_MUT_SELF
                  // LILY_CHECKED_DECL_FUN_PARAM_KIND_REF_MUT_SELF or
                  // LILY_CHECKED_DECL_FUN_PARAM_KIND_REF_SELF
    LilyCheckedDataType *data_type; // LilyCheckedDataType*?
    enum LilyCheckedDeclMethodParamKind kind;
    Location location;
    union
    {
        LilyCheckedExpr *default_;
    };
} LilyCheckedDeclMethodParam;

/**
 *
 * @brief Construct LilyCheckedDeclMethodParam type
 * (LILY_CHECKED_DECL_METHOD_PARAM_KIND_DEFAULT).
 */
VARIANT_CONSTRUCTOR(LilyCheckedDeclMethodParam *,
                    LilyCheckedDeclMethodParam,
                    default,
                    String *name,
                    LilyCheckedDataType *data_type,
                    Location location,
                    LilyCheckedExpr *default_);

/**
 *
 * @brief Construct LilyCheckedDeclMethodParam type
 * (LILY_CHECKED_DECL_METHOD_PARAM_KIND_NORMAL).
 */
VARIANT_CONSTRUCTOR(LilyCheckedDeclMethodParam *,
                    LilyCheckedDeclMethodParam,
                    normal,
                    String *name,
                    LilyCheckedDataType *data_type,
                    Location location);

/**
 *
 * @brief Construct LilyCheckedDeclMethodParam type.
 */
CONSTRUCTOR(LilyCheckedDeclMethodParam *,
            LilyCheckedDeclMethodParam,
            enum LilyCheckedDeclMethodParamKind kind,
            Location location);

/**
 *
 * @brief Convert LilyCheckedDeclMethodParam in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedDeclMethodParam,
               const LilyCheckedDeclMethodParam *self);
#endif

/**
 *
 * @brief Free LilyCheckedDeclMethodParam type.
 */
DESTRUCTOR(LilyCheckedDeclMethodParam, LilyCheckedDeclMethodParam *self);

typedef struct LilyCheckedDeclMethod
{
    String *name; // String* (&)
    String *global_name;
    String *object_impl;                   // String*? (&)
    Vec *generic_params;                   // Vec<LilyCheckedGenericParam*>*?
    Vec *params;                           // Vec<LilyCheckedDeclMethodParam*>*?
    LilyCheckedDataType *return_data_type; // LilyCheckedDataType*?
    Vec *body;                             // Vec<LilyCheckedBodyFunItem*>*
    LilyCheckedScope *scope;
    LilyCheckedAccessFun *access;
    enum LilyVisibility visibility;
    bool is_async;
    bool is_operator;
    bool is_virtual;
    bool is_override;
    bool can_raise;
    bool can_inline;
    bool is_checked;
} LilyCheckedDeclMethod;

/**
 *
 * @brief Construct LilyCheckedDeclFun type.
 */
inline CONSTRUCTOR(LilyCheckedDeclMethod,
                   LilyCheckedDeclMethod,
                   String *name,
                   String *global_name,
                   String *object_impl,
                   Vec *generic_params,
                   Vec *params,
                   LilyCheckedDataType *return_data_type,
                   Vec *body,
                   LilyCheckedScope *scope,
                   LilyCheckedAccessFun *access,
                   enum LilyVisibility visibility,
                   bool is_async,
                   bool is_operator,
                   bool is_checked)
{
    return (LilyCheckedDeclMethod){ .name = name,
                                    .global_name = global_name,
                                    .object_impl = object_impl,
                                    .params = params,
                                    .return_data_type = return_data_type,
                                    .body = body,
                                    .scope = scope,
                                    .access = access,
                                    .visibility = visibility,
                                    .is_async = is_async,
                                    .is_operator = is_operator,
                                    .is_virtual = false,
                                    .is_override = false,
                                    .can_raise = false,
                                    .can_inline = false,
                                    .is_checked = false };
}

/**
 *
 * @brief Convert LilyCheckedDeclMethod in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedDeclMethod,
               const LilyCheckedDeclMethod *self);
#endif

/**
 *
 * @brief Free LilyCheckedDeclMethod type.
 */
DESTRUCTOR(LilyCheckedDeclMethod, const LilyCheckedDeclMethod *self);

#endif // LILY_CORE_LILY_ANALYSIS_CHECKED_DECL_METHOD_H
