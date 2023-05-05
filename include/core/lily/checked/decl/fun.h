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

#ifndef LILY_CORE_LILY_CHECKED_DECL_FUN_H
#define LILY_CORE_LILY_CHECKED_DECL_FUN_H

#include <base/string.h>

#include <core/lily/checked/access.h>
#include <core/lily/checked/body/fun.h>
#include <core/lily/checked/data_type.h>
#include <core/lily/checked/decl/method.h>
#include <core/lily/checked/expr.h>
#include <core/lily/checked/generic_param.h>
#include <core/lily/checked/scope.h>
#include <core/lily/visibility.h>

#include <core/shared/location.h>

enum LilyCheckedDeclFunParamKind
{
    LILY_CHECKED_DECL_FUN_PARAM_KIND_DEFAULT,
    LILY_CHECKED_DECL_FUN_PARAM_KIND_NORMAL,
};

/**
 *
 * @brief Convert LilyCheckedDeclFunParamKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedDeclFunParamKind,
               enum LilyCheckedDeclFunParamKind self);
#endif

typedef struct LilyCheckedDeclFunParam
{
    String *name;
    LilyCheckedDataType *data_type; // LilyCheckedDataType*?
    enum LilyCheckedDeclFunParamKind kind;
    Location location;
    union
    {
        LilyCheckedExpr *default_;
    };
} LilyCheckedDeclFunParam;

/**
 *
 * @brief Construct LilyCheckedDeclFunParam type
 * (LILY_CHECKED_DECL_FUN_PARAM_KIND_DEFAULT).
 */
VARIANT_CONSTRUCTOR(LilyCheckedDeclFunParam *,
                    LilyCheckedDeclFunParam,
                    default,
                    String *name,
                    LilyCheckedDataType *data_type,
                    Location location,
                    LilyCheckedExpr *default_);

/**
 *
 * @brief Construct LilyCheckedDeclFunParam type
 * (LILY_CHECKED_DECL_FUN_PARAM_KIND_NORMAL).
 */
VARIANT_CONSTRUCTOR(LilyCheckedDeclFunParam *,
                    LilyCheckedDeclFunParam,
                    normal,
                    String *name,
                    LilyCheckedDataType *data_type,
                    Location location);

/**
 *
 * @brief Convert LilyCheckedDeclFunParam in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedDeclFunParam,
               const LilyCheckedDeclFunParam *self);
#endif

/**
 *
 * @brief Free LilyCheckedDeclFunParam type.
 */
DESTRUCTOR(LilyCheckedDeclFunParam, LilyCheckedDeclFunParam *self);

typedef struct LilyCheckedDeclFun
{
    String *name;
    Vec *generic_params;                   // Vec<LilyCheckedGenericParam*>*?
    Vec *params;                           // Vec<LilyCheckedDeclFunParam*>*?
    LilyCheckedDataType *return_data_type; // LilyCheckedDataType*?
    Vec *body;                             // Vec<LilyCheckedBodyFunItem*>*
    LilyCheckedScope *scope;
    LilyCheckedAccessFun *access;
    enum LilyVisibility visibility;
    bool is_async;
    bool is_operator;
    bool can_raise;
    bool can_inline;
} LilyCheckedDeclFun;

/**
 *
 * @brief Construct LilyCheckedDeclFun type.
 */
inline CONSTRUCTOR(LilyCheckedDeclFun,
                   LilyCheckedDeclFun,
                   String *name,
                   Vec *generic_params,
                   Vec *params,
                   LilyCheckedDataType *return_data_type,
                   Vec *body,
                   LilyCheckedScope *scope,
                   enum LilyVisibility visibility,
                   bool is_async,
                   bool is_operator)
{
    return (LilyCheckedDeclFun){ .name = name,
                                 .params = params,
                                 .return_data_type = return_data_type,
                                 .body = body,
                                 .visibility = visibility,
                                 .is_async = is_async,
                                 .is_operator = is_operator,
                                 .can_raise = false,
                                 .can_inline = false };
}

/**
 *
 * @brief Convert LilyCheckedDeclFun in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyCheckedDeclFun, const LilyCheckedDeclFun *self);
#endif

/**
 *
 * @brief Free LilyCheckedDeclFun type.
 */
DESTRUCTOR(LilyCheckedDeclFun, const LilyCheckedDeclFun *self);

#endif // LILY_CORE_LILY_CHECKED_DECL_FUN_H
