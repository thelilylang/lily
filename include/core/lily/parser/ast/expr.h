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

#ifndef LILY_CORE_LILY_PARSER_AST_EXPR_H
#define LILY_CORE_LILY_PARSER_AST_EXPR_H

#include <base/macros.h>
#include <base/string.h>

#include <core/lily/parser/ast/expr/access.h>
#include <core/lily/parser/ast/expr/array.h>
#include <core/lily/parser/ast/expr/binary.h>
#include <core/lily/parser/ast/expr/call.h>
#include <core/lily/parser/ast/expr/cast.h>
#include <core/lily/parser/ast/expr/identifier.h>
#include <core/lily/parser/ast/expr/lambda.h>
#include <core/lily/parser/ast/expr/list.h>
#include <core/lily/parser/ast/expr/literal.h>
#include <core/lily/parser/ast/expr/try.h>
#include <core/lily/parser/ast/expr/tuple.h>
#include <core/lily/parser/ast/expr/unary.h>

#include <core/shared/location.h>

enum LilyAstExprKind
{
    LILY_AST_EXPR_KIND_ACCESS,
    LILY_AST_EXPR_KIND_ARRAY,
    LILY_AST_EXPR_KIND_BINARY,
    LILY_AST_EXPR_KIND_CALL,
    LILY_AST_EXPR_KIND_CAST,
    LILY_AST_EXPR_KIND_GROUPING,
    LILY_AST_EXPR_KIND_IDENTIFIER,
    LILY_AST_EXPR_KIND_IDENTIFIER_DOLLAR,
    LILY_AST_EXPR_KIND_LAMBDA,
    LILY_AST_EXPR_KIND_LIST,
    LILY_AST_EXPR_KIND_LITERAL,
    LILY_AST_EXPR_KIND_SELF,
    LILY_AST_EXPR_KIND_TRY,
    LILY_AST_EXPR_KIND_TUPLE,
    LILY_AST_EXPR_KIND_UNARY,
    LILY_AST_EXPR_KIND_WILDCARD
};

typedef struct LilyAstExpr
{
    enum LilyAstExprKind kind;
    Location location;
    union
    {
        LilyAstExprAccess access;
        LilyAstExprArray array;
        LilyAstExprBinary binary;
        LilyAstExprCall call;
        LilyAstExprCast cast;
        LilyAstExpr *grouping;
        LilyAstExprIdentifier identifier;
        LilyAstExprIdentifierDollar identifier_dollar;
        LilyAstExprLambda lambda;
        LilyAstExprList list;
        LilyAstExprLiteral literal;
        LilyAstExprTry try;
        LilyAstExprTuple tuple;
        LilyAstExprUnary unary;
    };
} LilyAstExpr;

/**
 *
 * @brief Construct LilyAstExpr type (LILY_AST_EXPR_KIND_ACCESS).
 */
VARIANT_CONSTRUCTOR(LilyAstExpr *,
                    LilyAstExpr,
                    access,
                    Location location,
                    LilyAstExprAccess access);

/**
 *
 * @brief Construct LilyAstExpr type (LILY_AST_EXPR_KIND_ARRAY).
 */
VARIANT_CONSTRUCTOR(LilyAstExpr *,
                    LilyAstExpr,
                    array,
                    Location location,
                    LilyAstExprArray array);

/**
 *
 * @brief Construct LilyAstExpr type (LILY_AST_EXPR_KIND_BINARY).
 */
VARIANT_CONSTRUCTOR(LilyAstExpr *,
                    LilyAstExpr,
                    binary,
                    Location location,
                    LilyAstExprBinary binary);

/**
 *
 * @brief Construct LilyAstExpr type (LILY_AST_EXPR_KIND_CALL).
 */
VARIANT_CONSTRUCTOR(LilyAstExpr *,
                    LilyAstExpr,
                    call,
                    Location location,
                    LilyAstExprCall call);

/**
 *
 * @brief Construct LilyAstExpr type (LILY_AST_EXPR_KIND_CAST).
 */
VARIANT_CONSTRUCTOR(LilyAstExpr *,
                    LilyAstExpr,
                    cast,
                    Location location,
                    LilyAstExprCast cast);

/**
 *
 * @brief Construct LilyAstExpr type (LILY_AST_EXPR_KIND_GROUPING).
 */
VARIANT_CONSTRUCTOR(LilyAstExpr *,
                    LilyAstExpr,
                    grouping,
                    Location location,
                    LilyAstExpr *grouping);

/**
 *
 * @brief Construct LilyAstExpr type (LILY_AST_EXPR_KIND_IDENTIFIER).
 */
VARIANT_CONSTRUCTOR(LilyAstExpr *,
                    LilyAstExpr,
                    identifier,
                    Location location,
                    LilyAstExprIdentifier identifier);

/**
 *
 * @brief Construct LilyAstExpr type (LILY_AST_EXPR_KIND_IDENTIFIER_DOLLAR).
 */
VARIANT_CONSTRUCTOR(LilyAstExpr *,
                    LilyAstExpr,
                    identifier_dollar,
                    Location location,
                    LilyAstExprIdentifierDollar identifier_dollar);

/**
 *
 * @brief Construct LilyAstExpr type (LILY_AST_EXPR_KIND_LAMBDA).
 */
VARIANT_CONSTRUCTOR(LilyAstExpr *,
                    LilyAstExpr,
                    lambda,
                    Location location,
                    LilyAstExprLambda lambda);

/**
 *
 * @brief Construct LilyAstExpr type (LILY_AST_EXPR_KIND_LIST).
 */
VARIANT_CONSTRUCTOR(LilyAstExpr *,
                    LilyAstExpr,
                    list,
                    Location location,
                    LilyAstExprList list);

/**
 *
 * @brief Construct LilyAstExpr type (LILY_AST_EXPR_KIND_LITERAL).
 */
VARIANT_CONSTRUCTOR(LilyAstExpr *,
                    LilyAstExpr,
                    literal,
                    Location location,
                    LilyAstExprLiteral literal);

/**
 *
 * @brief Construct LilyAstExpr type (LILY_AST_EXPR_KIND_TRY).
 */
VARIANT_CONSTRUCTOR(LilyAstExpr *,
                    LilyAstExpr,
                    try,
                    Location location,
                    LilyAstExprTry try);

/**
 *
 * @brief Construct LilyAstExpr type (LILY_AST_EXPR_KIND_TUPLE).
 */
VARIANT_CONSTRUCTOR(LilyAstExpr *,
                    LilyAstExpr,
                    tuple,
                    Location location,
                    LilyAstExprTuple tuple);

/**
 *
 * @brief Construct LilyAstExpr type (LILY_AST_EXPR_KIND_UNARY).
 */
VARIANT_CONSTRUCTOR(LilyAstExpr *,
                    LilyAstExpr,
                    unary,
                    Location location,
                    LilyAstExprUnary unary);

/**
 *
 * @brief Construct LilyAstExpr type.
 */
CONSTRUCTOR(LilyAstExpr *,
            LilyAstExpr,
            Location location,
            enum LilyAstExprKind kind);

/**
 *
 * @brief Convert LilyAstExprKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, LilyAstExprKind, enum LilyAstExprKind self);
#endif

/**
 *
 * @brief Convert LilyAstExpr in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyAstExpr, const LilyAstExpr *self);
#endif

/**
 *
 * @brief Convert LilyAstExpr in String.
 */
String *
to_string__LilyAstExpr(const LilyAstExpr *self);

/**
 *
 * @brief Get generic params from expression.
 */
Vec *
get_generic_params__LilyAstExpr(LilyAstExpr *self);

/**
 *
 * @breif Get generic params from a variant call.
 */
Vec *
get_generic_params_from_variant_call__LilyAstExpr(LilyAstExpr *self);

/**
 *
 * @brief Get last name of expression.
 * @return String*? (&)
 */
String *
get_last_name__LilyAstExpr(const LilyAstExpr *self);

/**
 *
 * @brief Free LilyAstExpr type.
 */
DESTRUCTOR(LilyAstExpr, LilyAstExpr *self);

#endif // LILY_CORE_LILY_PARSER_AST_EXPR_H
