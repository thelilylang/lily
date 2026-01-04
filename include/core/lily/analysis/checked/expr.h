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

#ifndef LILY_CORE_LILY_ANALYSIS_CHECKED_EXPR_H
#define LILY_CORE_LILY_ANALYSIS_CHECKED_EXPR_H

#include <base/string.h>

#include <core/lily/analysis/checked/data_type.h>
#include <core/lily/analysis/checked/expr/access.h>
#include <core/lily/analysis/checked/expr/array.h>
#include <core/lily/analysis/checked/expr/binary.h>
#include <core/lily/analysis/checked/expr/call.h>
#include <core/lily/analysis/checked/expr/cast.h>
#include <core/lily/analysis/checked/expr/compiler_fun.h>
#include <core/lily/analysis/checked/expr/lambda.h>
#include <core/lily/analysis/checked/expr/list.h>
#include <core/lily/analysis/checked/expr/literal.h>
#include <core/lily/analysis/checked/expr/tuple.h>
#include <core/lily/analysis/checked/expr/unary.h>
#include <core/lily/parser/ast/expr.h>

#include <core/shared/location.h>

// TODO: Maybe delete lambda expression

enum LilyCheckedExprKind
{
    LILY_CHECKED_EXPR_KIND_ACCESS,
    LILY_CHECKED_EXPR_KIND_ARRAY,
    LILY_CHECKED_EXPR_KIND_BINARY,
    LILY_CHECKED_EXPR_KIND_CALL,
    LILY_CHECKED_EXPR_KIND_CAST,
    LILY_CHECKED_EXPR_KIND_COMPILER_FUN, // only usable by the compiler
    LILY_CHECKED_EXPR_KIND_GROUPING,
    LILY_CHECKED_EXPR_KIND_LAMBDA,
    LILY_CHECKED_EXPR_KIND_LIST,
    LILY_CHECKED_EXPR_KIND_LITERAL,
    LILY_CHECKED_EXPR_KIND_SELF,
    LILY_CHECKED_EXPR_KIND_TUPLE,
    LILY_CHECKED_EXPR_KIND_UNARY,
    LILY_CHECKED_EXPR_KIND_UNITER,
    LILY_CHECKED_EXPR_KIND_UNKNOWN
};

typedef struct LilyCheckedExpr
{
    enum LilyCheckedExprKind kind;
    const Location *location; // const Location* (&)
                              // This is the result data type of the expression.
    LilyCheckedDataType *data_type; // LilyCheckedDataType*
    const LilyAstExpr *ast_expr;    // const LilyAstExpr*? (&)
    Usize ref_count;
    union
    {
        LilyCheckedExprAccess access;
        LilyCheckedExprArray array;
        LilyCheckedExprBinary binary;
        LilyCheckedExprCall call;
        LilyCheckedExprCast cast;
        LilyCheckedExprCompilerFun compiler_fun;
        LilyCheckedExpr *grouping;
        LilyCheckedExprLambda lambda;
        LilyCheckedExprList list;
        LilyCheckedExprLiteral literal;
        LilyCheckedExprTuple tuple;
        LilyCheckedExprUnary unary;
        LilyCheckedExpr *uniter;
    };
} LilyCheckedExpr;

/**
 *
 * @brief Construct LilyCheckedExpr type (LILY_CHECKED_EXPR_KIND_ACCESS).
 */
VARIANT_CONSTRUCTOR(LilyCheckedExpr *,
                    LilyCheckedExpr,
                    access,
                    const Location *location,
                    LilyCheckedDataType *data_type,
                    const LilyAstExpr *ast_expr,
                    LilyCheckedExprAccess access);

/**
 *
 * @brief Construct LilyCheckedExpr type (LILY_CHECKED_EXPR_KIND_ARRAY).
 */
VARIANT_CONSTRUCTOR(LilyCheckedExpr *,
                    LilyCheckedExpr,
                    array,
                    const Location *location,
                    LilyCheckedDataType *data_type,
                    const LilyAstExpr *ast_expr,
                    LilyCheckedExprArray array);

/**
 *
 * @brief Construct LilyCheckedExpr type (LILY_CHECKED_EXPR_KIND_BINARY).
 */
VARIANT_CONSTRUCTOR(LilyCheckedExpr *,
                    LilyCheckedExpr,
                    binary,
                    const Location *location,
                    LilyCheckedDataType *data_type,
                    const LilyAstExpr *ast_expr,
                    LilyCheckedExprBinary binary);

/**
 *
 * @brief Construct LilyCheckedExpr type (LILY_CHECKED_EXPR_KIND_CALL).
 */
VARIANT_CONSTRUCTOR(LilyCheckedExpr *,
                    LilyCheckedExpr,
                    call,
                    const Location *location,
                    LilyCheckedDataType *data_type,
                    const LilyAstExpr *ast_expr,
                    LilyCheckedExprCall call);

/**
 *
 * @brief Construct LilyCheckedExpr type (LILY_CHECKED_EXPR_KIND_CAST).
 */
VARIANT_CONSTRUCTOR(LilyCheckedExpr *,
                    LilyCheckedExpr,
                    cast,
                    const Location *location,
                    LilyCheckedDataType *data_type,
                    const LilyAstExpr *ast_expr,
                    LilyCheckedExprCast cast);

/**
 *
 * @brief Construct LilyCheckedExpr type (LILY_CHECKED_EXPR_KIND_COMPILER_FUN).
 */
VARIANT_CONSTRUCTOR(LilyCheckedExpr *,
                    LilyCheckedExpr,
                    compiler_fun,
                    const Location *location,
                    LilyCheckedDataType *data_type,
                    const LilyAstExpr *ast_expr,
                    LilyCheckedExprCompilerFun compiler_fun);

/**
 *
 * @brief Construct LilyCheckedExpr type (LILY_CHECKED_EXPR_KIND_GROUPING).
 */
VARIANT_CONSTRUCTOR(LilyCheckedExpr *,
                    LilyCheckedExpr,
                    grouping,
                    const Location *location,
                    LilyCheckedDataType *data_type,
                    const LilyAstExpr *ast_expr,
                    LilyCheckedExpr *grouping);

/**
 *
 * @brief Construct LilyCheckedExpr type (LILY_CHECKED_EXPR_KIND_LAMBDA).
 */
VARIANT_CONSTRUCTOR(LilyCheckedExpr *,
                    LilyCheckedExpr,
                    lambda,
                    const Location *location,
                    LilyCheckedDataType *data_type,
                    const LilyAstExpr *ast_expr,
                    LilyCheckedExprLambda lambda);

/**
 *
 * @brief Construct LilyCheckedExpr type (LILY_CHECKED_EXPR_KIND_LIST).
 */
VARIANT_CONSTRUCTOR(LilyCheckedExpr *,
                    LilyCheckedExpr,
                    list,
                    const Location *location,
                    LilyCheckedDataType *data_type,
                    const LilyAstExpr *ast_expr,
                    LilyCheckedExprList list);

/**
 *
 * @brief Construct LilyCheckedExpr type (LILY_CHECKED_EXPR_KIND_LITERAL).
 */
VARIANT_CONSTRUCTOR(LilyCheckedExpr *,
                    LilyCheckedExpr,
                    literal,
                    const Location *location,
                    LilyCheckedDataType *data_type,
                    const LilyAstExpr *ast_expr,
                    LilyCheckedExprLiteral literal);

/**
 *
 * @brief Construct LilyCheckedExpr type (LILY_CHECKED_EXPR_KIND_TUPLE).
 */
VARIANT_CONSTRUCTOR(LilyCheckedExpr *,
                    LilyCheckedExpr,
                    tuple,
                    const Location *location,
                    LilyCheckedDataType *data_type,
                    const LilyAstExpr *ast_expr,
                    LilyCheckedExprTuple tuple);

/**
 *
 * @brief Construct LilyCheckedExpr type (LILY_CHECKED_EXPR_KIND_UNARY).
 */
VARIANT_CONSTRUCTOR(LilyCheckedExpr *,
                    LilyCheckedExpr,
                    unary,
                    const Location *location,
                    LilyCheckedDataType *data_type,
                    const LilyAstExpr *ast_expr,
                    LilyCheckedExprUnary unary);

/**
 *
 * @brief Construct LilyCheckedExpr type (LILY_CHECKED_EXPR_KIND_UNITER).
 */
VARIANT_CONSTRUCTOR(LilyCheckedExpr *,
                    LilyCheckedExpr,
                    uniter,
                    const LilyAstExpr *ast_expr,
                    LilyCheckedExpr *uniter);

/**
 *
 * @brief Construct LilyCheckedExpr type (LILY_CHECKED_EXPR_KIND_UNKNOWN).
 */
VARIANT_CONSTRUCTOR(LilyCheckedExpr *,
                    LilyCheckedExpr,
                    unknown,
                    const Location *location,
                    const LilyAstExpr *ast_expr);

/**
 *
 * @brief Construct LilyCheckedExpr type.
 */
CONSTRUCTOR(LilyCheckedExpr *,
            LilyCheckedExpr,
            const Location *location,
            LilyCheckedDataType *data_type,
            const LilyAstExpr *ast_expr,
            enum LilyCheckedExprKind kind);

/**
 *
 * @brief Check if the both expressions are equal.
 */
bool
eq__LilyCheckedExpr(const LilyCheckedExpr *self, const LilyCheckedExpr *other);

/**
 *
 * @brief Pass to ref a pointer of `LilyCheckedExpr` and increment
 * the `ref_count`.
 * @return LilyCheckedExpr* (&)
 */
inline LilyCheckedExpr *
ref__LilyCheckedExpr(LilyCheckedExpr *self)
{
    ++self->ref_count;
    return self;
}

/**
 *
 * @brief Check if the expression is a node (e.g. binary expression, unary
 * expression).
 */
inline bool
is_node__LilyCheckedExpr(const LilyCheckedExpr *self)
{
    return self->kind == LILY_CHECKED_EXPR_KIND_BINARY ||
           self->kind == LILY_CHECKED_EXPR_KIND_CAST ||
           self->kind == LILY_CHECKED_EXPR_KIND_GROUPING ||
           self->kind == LILY_CHECKED_EXPR_KIND_UNARY;
}

/**
 *
 * @brief Convert LilyCheckedExprKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, LilyCheckedExprKind, enum LilyCheckedExprKind self);
#endif

/**
 *
 * @brief Convert LilyCheckedExpr in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyCheckedExpr, const LilyCheckedExpr *self);
#endif

/**
 *
 * @brief Free LilyCheckedExpr type.
 */
DESTRUCTOR(LilyCheckedExpr, LilyCheckedExpr *self);

#endif // LILY_CORE_LILY_ANALYSIS_CHECKED_EXPR_H
