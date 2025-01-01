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

#ifndef LILY_CORE_LILY_ANALYSIS_CHECKED_EXPR_LAMBDA_H
#define LILY_CORE_LILY_ANALYSIS_CHECKED_EXPR_LAMBDA_H

#include <base/string.h>

#include <core/lily/analysis/checked/data_type.h>

typedef struct LilyCheckedExpr LilyCheckedExpr;

enum LilyCheckedExprLambdaParamKind
{
    LILY_CHECKED_EXPR_LAMBDA_PARAM_KIND_DEFAULT,
    LILY_CHECKED_EXPR_LAMBDA_PARAM_KIND_NORMAL
};

/**
 *
 * @brief Convert LilyCheckedExprLambdaParamKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprLambdaParamKind,
               enum LilyCheckedExprLambdaParamKind self);
#endif

typedef struct LilyCheckedExprLambdaParam
{
    enum LilyCheckedExprLambdaParamKind kind;
    Location location;
    String *name;
    LilyCheckedDataType *data_type; // LilyCheckedDataType*?

    union
    {
        LilyCheckedExpr *optional;
    };
} LilyCheckedExprLambdaParam;

/**
 *
 * @brief Construct LilyCheckedExprLambdaParam type
 * (LILY_CHECKED_EXPR_LAMBDA_PARAM_KIND_DEFAULT).
 */
VARIANT_CONSTRUCTOR(LilyCheckedExprLambdaParam *,
                    LilyCheckedExprLambdaParam,
                    default_,
                    Location location,
                    String *name,
                    LilyCheckedDataType *data_type,
                    LilyCheckedExpr *optional);

/**
 *
 * @brief Construct LilyCheckedExprLambdaParam type
 * (LILY_CHECKED_EXPR_LAMBDA_PARAM_KIND_NORMAL).
 */
VARIANT_CONSTRUCTOR(LilyCheckedExprLambdaParam *,
                    LilyCheckedExprLambdaParam,
                    normal,
                    Location location,
                    String *name,
                    LilyCheckedDataType *data_type);

/**
 *
 * @brief Convert LilyCheckedExprLambdaParam in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprLambdaParam,
               const LilyCheckedExprLambdaParam *self);
#endif

/**
 *
 * @brief Free LilyCheckedExprLambdaParam type.
 */
DESTRUCTOR(LilyCheckedExprLambdaParam, LilyCheckedExprLambdaParam *self);

typedef struct LilyCheckedExprLambda
{
    Vec *params;                           // Vec<LilyCheckedExprLambdaParam*>*?
    LilyCheckedDataType *return_data_type; // LilyCheckedDataType*?
    Vec *body;                             // Vec<LilyCheckedBodyFunItem*>*
} LilyCheckedExprLambda;

/**
 *
 * @brief Construct LilyCheckedExprLambda type.
 */
inline CONSTRUCTOR(LilyCheckedExprLambda,
                   LilyCheckedExprLambda,
                   Vec *params,
                   LilyCheckedDataType *return_data_type,
                   Vec *body)
{
    return (LilyCheckedExprLambda){ .params = params,
                                    .return_data_type = return_data_type,
                                    .body = body };
}

/**
 *
 * @brief Convert LilyCheckedExprLambda in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprLambda,
               const LilyCheckedExprLambda *self);
#endif

/**
 *
 * @brief Free LilyCheckedExprLambda type.
 */
DESTRUCTOR(LilyCheckedExprLambda, const LilyCheckedExprLambda *self);

#endif // LILY_CORE_LILY_ANALYSIS_CHECKED_EXPR_LAMBDA_H
