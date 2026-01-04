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

#ifndef LILY_CORE_LILY_PARSER_AST_EXPR_UNARY_H
#define LILY_CORE_LILY_PARSER_AST_EXPR_UNARY_H

#include <base/macros.h>

#ifdef ENV_DEBUG
#include <base/string.h>
#endif

typedef struct LilyAstExpr LilyAstExpr;

enum LilyAstExprUnaryKind
{
    LILY_AST_EXPR_UNARY_KIND_DEREFERENCE,
    LILY_AST_EXPR_UNARY_KIND_NEG,
    LILY_AST_EXPR_UNARY_KIND_NOT,
    LILY_AST_EXPR_UNARY_KIND_OPTIONAL,
    LILY_AST_EXPR_UNARY_KIND_REF,
    LILY_AST_EXPR_UNARY_KIND_REF_MUT,
    LILY_AST_EXPR_UNARY_KIND_TRACE,
    LILY_AST_EXPR_UNARY_KIND_TRACE_MUT,
};

/**
 *
 * @brief Convert LilyAstExprUnaryKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, LilyAstExprUnaryKind, enum LilyAstExprUnaryKind self);
#endif

typedef struct LilyAstExprUnary
{
    enum LilyAstExprUnaryKind kind;
    LilyAstExpr *right;
} LilyAstExprUnary;

/**
 *
 * @brief Construct LilyAstExprUnary type.
 */
inline CONSTRUCTOR(LilyAstExprUnary,
                   LilyAstExprUnary,
                   enum LilyAstExprUnaryKind kind,
                   LilyAstExpr *right)
{
    return (LilyAstExprUnary){ .kind = kind, .right = right };
}

/**
 *
 * @brief Convert LilyAstExprUnary in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyAstExprUnary, const LilyAstExprUnary *self);
#endif

/**
 *
 * @brief Free LilyAstExprUnary type.
 */
DESTRUCTOR(LilyAstExprUnary, const LilyAstExprUnary *self);

#endif // LILY_CORE_LILY_PARSER_AST_EXPR_UNARY_H
