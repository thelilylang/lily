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

#ifndef LILY_CORE_LILY_PARSER_AST_DECL_FUN_H
#define LILY_CORE_LILY_PARSER_AST_DECL_FUN_H

#include <base/string.h>

#include <core/lily/parser/ast/body/fun.h>
#include <core/lily/parser/ast/data_type.h>
#include <core/lily/parser/ast/decl/method.h>
#include <core/lily/parser/ast/expr.h>
#include <core/lily/parser/ast/generic_param.h>
#include <core/lily/shared/visibility.h>
#include <core/shared/location.h>

enum LilyAstDeclFunParamKind
{
    LILY_AST_DECL_FUN_PARAM_KIND_DEFAULT,
    LILY_AST_DECL_FUN_PARAM_KIND_NORMAL,
};

/**
 *
 * @brief Convert LilyAstDeclFunParamKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyAstDeclFunParamKind,
               enum LilyAstDeclFunParamKind self);
#endif

typedef struct LilyAstDeclFunParam
{
    String *name;
    LilyAstDataType *data_type; // LilyAstDataType*?
    enum LilyAstDeclFunParamKind kind;
    Location location;
    union
    {
        LilyAstExpr *default_;
    };
} LilyAstDeclFunParam;

/**
 *
 * @brief Construct LilyAstDeclFunParam type
 * (LILY_AST_DECL_FUN_PARAM_KIND_DEFAULT).
 */
VARIANT_CONSTRUCTOR(LilyAstDeclFunParam *,
                    LilyAstDeclFunParam,
                    default,
                    String *name,
                    LilyAstDataType *data_type,
                    Location location,
                    LilyAstExpr *default_);

/**
 *
 * @brief Construct LilyAstDeclFunParam type
 * (LILY_AST_DECL_FUN_PARAM_KIND_NORMAL).
 */
VARIANT_CONSTRUCTOR(LilyAstDeclFunParam *,
                    LilyAstDeclFunParam,
                    normal,
                    String *name,
                    LilyAstDataType *data_type,
                    Location location);

/**
 *
 * @brief Convert LilyAstDeclFunParam in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyAstDeclFunParam, const LilyAstDeclFunParam *self);
#endif

/**
 *
 * @brief Free LilyAstDeclFunParam type.
 */
DESTRUCTOR(LilyAstDeclFunParam, LilyAstDeclFunParam *self);

typedef struct LilyAstDeclFun
{
    String *name;
    String *object_impl; // String*?
    Vec *generic_params; // Vec<LilyAstGenericParam>*?
    Vec *params;         // Vec<LilyAstDeclFunParam*|LilyAstDeclMethodParam*>*?
    LilyAstDataType *return_data_type; // LilyAstDataType*?
    Vec *body;                         // Vec<LilyAstBodyFunItem*>*
    Vec *req;                          // Vec<LilyAstExpr*>*?
    Vec *when;                         // Vec<LilyAstExpr*>*?
    enum LilyVisibility visibility;
    bool is_async;
    bool is_operator;
    bool req_is_comptime;
    bool when_is_comptime;
} LilyAstDeclFun;

/**
 *
 * @brief Construct LilyAstDeclFun type.
 */
inline CONSTRUCTOR(LilyAstDeclFun,
                   LilyAstDeclFun,
                   String *name,
                   String *object_impl,
                   Vec *generic_params,
                   Vec *params,
                   LilyAstDataType *return_data_type,
                   Vec *body,
                   Vec *req,
                   Vec *when,
                   enum LilyVisibility visibility,
                   bool is_async,
                   bool is_operator,
                   bool req_is_comptime,
                   bool when_is_comptime)
{
    return (LilyAstDeclFun){ .name = name,
                             .object_impl = object_impl,
                             .generic_params = generic_params,
                             .params = params,
                             .return_data_type = return_data_type,
                             .body = body,
                             .req = req,
                             .when = when,
                             .visibility = visibility,
                             .is_async = is_async,
                             .is_operator = is_operator,
                             .req_is_comptime = req_is_comptime,
                             .when_is_comptime = when_is_comptime };
}

/**
 *
 * @brief Convert LilyAstDeclFun in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyAstDeclFun, const LilyAstDeclFun *self);
#endif

/**
 *
 * @brief Free LilyAstDeclFun type.
 */
DESTRUCTOR(LilyAstDeclFun, const LilyAstDeclFun *self);

#endif // LILY_CORE_LILY_PARSER_AST_DECL_FUN_H
