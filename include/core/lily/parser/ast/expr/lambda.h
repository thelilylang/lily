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

#ifndef LILY_CORE_LILY_PARSER_AST_EXPR_LAMBDA_H
#define LILY_CORE_LILY_PARSER_AST_EXPR_LAMBDA_H

#include <base/string.h>

#include <core/lily/parser/ast/data_type.h>

typedef struct LilyAstExpr LilyAstExpr;
typedef struct LilyAstBodyFunItem LilyAstBodyFunItem;

enum LilyAstExprLambdaParamKind
{
    LILY_AST_EXPR_LAMBDA_PARAM_KIND_DEFAULT,
    LILY_AST_EXPR_LAMBDA_PARAM_KIND_NORMAL
};

/**
 *
 * @brief Convert LilyAstExprLambdaParamKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyAstExprLambdaParamKind,
               enum LilyAstExprLambdaParamKind self);
#endif

typedef struct LilyAstExprLambdaParam
{
    enum LilyAstExprLambdaParamKind kind;
    Location location;
    String *name;
    LilyAstDataType *data_type; // LilyAstDataType*?

    union
    {
        LilyAstExpr *optional;
    };
} LilyAstExprLambdaParam;

/**
 *
 * @brief Construct LilyAstExprLambdaParam type
 * (LILY_AST_EXPR_LAMBDA_PARAM_KIND_DEFAULT).
 */
VARIANT_CONSTRUCTOR(LilyAstExprLambdaParam *,
                    LilyAstExprLambdaParam,
                    default,
                    Location location,
                    String *name,
                    LilyAstDataType *data_type,
                    LilyAstExpr *optional);

/**
 *
 * @brief Construct LilyAstExprLambdaParam type
 * (LILY_AST_EXPR_LAMBDA_PARAM_KIND_NORMAL).
 */
VARIANT_CONSTRUCTOR(LilyAstExprLambdaParam *,
                    LilyAstExprLambdaParam,
                    normal,
                    Location location,
                    String *name,
                    LilyAstDataType *data_type);

/**
 *
 * @brief Convert LilyAstExprLambdaParam in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyAstExprLambdaParam,
               const LilyAstExprLambdaParam *self);
#endif

/**
 *
 * @brief Free LilyAstExprLambdaParam type.
 */
DESTRUCTOR(LilyAstExprLambdaParam, LilyAstExprLambdaParam *self);

enum LilyAstExprLambdaParamCallKind
{
    LILY_AST_EXPR_LAMBDA_PARAM_CALL_KIND_DEFAULT,
    LILY_AST_EXPR_LAMBDA_PARAM_CALL_KIND_NORMAL
};

/**
 *
 * @brief Convert LilyAstExprLambdaParamCallKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyAstExprLambdaParamCallKind,
               enum LilyAstExprLambdaParamCallKind self);
#endif

typedef struct LilyAstExprLambdaParamCall
{
    enum LilyAstExprLambdaParamCallKind kind;
    LilyAstExpr *expr;
    Location location;
    union
    {
        String *name;
    };
} LilyAstExprLambdaParamCall;

/**
 *
 * @brief Construct LilyAstExprLambdaParamCall type
 * (LILY_AST_EXPR_LAMBDA_PARAM_CALL_KIND_DEFAULT).
 */
VARIANT_CONSTRUCTOR(LilyAstExprLambdaParamCall *,
                    LilyAstExprLambdaParamCall,
                    default,
                    LilyAstExpr *expr,
                    Location location,
                    String *name);

/**
 *
 * @brief Construct LilyAstExprLambdaParamCall type
 * (LILY_AST_EXPR_LAMBDA_PARAM_CALL_KIND_NORMAL).
 */
VARIANT_CONSTRUCTOR(LilyAstExprLambdaParamCall *,
                    LilyAstExprLambdaParamCall,
                    normal,
                    LilyAstExpr *expr,
                    Location location);

/**
 *
 * @brief Convert LilyAstExprLambdaParamCall in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyAstExprLambdaParamCall,
               const LilyAstExprLambdaParamCall *self);
#endif

/**
 *
 * @brief Free LilyAstExprLambdaParamCall type.
 */
DESTRUCTOR(LilyAstExprLambdaParamCall, LilyAstExprLambdaParamCall *self);

typedef struct LilyAstExprLambda
{
    String *name;                      // String*?
    Vec *params;                       // Vec<LilyAstExprLambdaParam*>*?
    LilyAstDataType *return_data_type; // LilyAstDataType*?
    LilyAstBodyFunItem *item;
    Vec *params_call; // Vec<LilyAstExprLambdaParamCall*>*?
} LilyAstExprLambda;

/**
 *
 * @brief Construct LilyAstExprLambda type.
 */
inline CONSTRUCTOR(LilyAstExprLambda,
                   LilyAstExprLambda,
                   String *name,
                   Vec *params,
                   LilyAstDataType *return_data_type,
                   LilyAstBodyFunItem *item,
                   Vec *params_call)
{
    return (LilyAstExprLambda){ .name = name,
                                .params = params,
                                .return_data_type = return_data_type,
                                .item = item,
                                .params_call = params_call };
}

/**
 *
 * @brief Convert LilyAstExprLambda in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyAstExprLambda, const LilyAstExprLambda *self);
#endif

/**
 *
 * @brief Free LilyAstExprLambda type.
 */
DESTRUCTOR(LilyAstExprLambda, const LilyAstExprLambda *self);

#endif // LILY_CORE_LILY_PARSER_AST_EXPR_LAMBDA_H
