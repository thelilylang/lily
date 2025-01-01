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

#ifndef LILY_CORE_LILY_PARSER_AST_EXPR_BINARY_H
#define LILY_CORE_LILY_PARSER_AST_EXPR_BINARY_H

#include <base/macros.h>
#include <base/string.h>

#include <core/lily/scanner/token.h>

#define MAX_LILY_AST_EXPR_BINARY_PRECEDENCE 100

typedef struct LilyAstExpr LilyAstExpr;

enum LilyAstExprBinaryKind
{
    LILY_AST_EXPR_BINARY_KIND_ADD,                // +
    LILY_AST_EXPR_BINARY_KIND_AND,                // and
    LILY_AST_EXPR_BINARY_KIND_ASSIGN_ADD,         // +=
    LILY_AST_EXPR_BINARY_KIND_ASSIGN_BIT_AND,     // &=
    LILY_AST_EXPR_BINARY_KIND_ASSIGN_BIT_L_SHIFT, // <<=
    LILY_AST_EXPR_BINARY_KIND_ASSIGN_BIT_OR,      // |=
    LILY_AST_EXPR_BINARY_KIND_ASSIGN_BIT_R_SHIFT, // >>=
    LILY_AST_EXPR_BINARY_KIND_ASSIGN_DIV,         // /=
    LILY_AST_EXPR_BINARY_KIND_ASSIGN_EXP,         // **=
    LILY_AST_EXPR_BINARY_KIND_ASSIGN_MOD,         // %=
    LILY_AST_EXPR_BINARY_KIND_ASSIGN_MUL,         // *=
    LILY_AST_EXPR_BINARY_KIND_ASSIGN_SUB,         // -=
    LILY_AST_EXPR_BINARY_KIND_ASSIGN_XOR,         // xor=
    LILY_AST_EXPR_BINARY_KIND_ASSIGN,             // =
    LILY_AST_EXPR_BINARY_KIND_BIT_AND,            // &
    LILY_AST_EXPR_BINARY_KIND_BIT_OR,             // |
    LILY_AST_EXPR_BINARY_KIND_CHAIN,              // |>
    LILY_AST_EXPR_BINARY_KIND_DIV,                // /
    LILY_AST_EXPR_BINARY_KIND_EQ,                 // ==
    LILY_AST_EXPR_BINARY_KIND_EXP,                // **
    LILY_AST_EXPR_BINARY_KIND_GREATER_EQ,         // >=
    LILY_AST_EXPR_BINARY_KIND_GREATER,            // >
    LILY_AST_EXPR_BINARY_KIND_BIT_L_SHIFT,        // <<
    LILY_AST_EXPR_BINARY_KIND_LESS_EQ,            // <=
    LILY_AST_EXPR_BINARY_KIND_LESS,               // <
    LILY_AST_EXPR_BINARY_KIND_MOD,                // %
    LILY_AST_EXPR_BINARY_KIND_MUL,                // *
    LILY_AST_EXPR_BINARY_KIND_NOT_EQ,             // not=
    LILY_AST_EXPR_BINARY_KIND_OR,                 // or
    LILY_AST_EXPR_BINARY_KIND_BIT_R_SHIFT,        // >>
    LILY_AST_EXPR_BINARY_KIND_RANGE,              // ..
    LILY_AST_EXPR_BINARY_KIND_SUB,                // -
    LILY_AST_EXPR_BINARY_KIND_XOR,                // xor
    LILY_AST_EXPR_BINARY_KIND_LIST_HEAD,          // ->
    LILY_AST_EXPR_BINARY_KIND_LIST_TAIL,          // <-
};

typedef struct LilyAstExprBinary
{
    enum LilyAstExprBinaryKind kind;
    LilyAstExpr *left;
    LilyAstExpr *right;
} LilyAstExprBinary;

/**
 *
 * @brief Construct LilyAstExprBinary type.
 */
inline CONSTRUCTOR(LilyAstExprBinary,
                   LilyAstExprBinary,
                   enum LilyAstExprBinaryKind kind,
                   LilyAstExpr *left,
                   LilyAstExpr *right)
{
    return (LilyAstExprBinary){ .kind = kind, .left = left, .right = right };
}

/**
 *
 * @brief Convert LilyAstExprBinaryKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyAstExprBinaryKind,
               enum LilyAstExprBinaryKind self);
#endif

/**
 *
 * @brief Convert LilyAstExprBinary in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, LilyAstExprBinary, const LilyAstExprBinary *self);
#endif

/**
 *
 * @brief Convert LilyAstExprBinary in String.
 */
String *
to_string__LilyAstExprBinary(const LilyAstExprBinary *self);

/**
 *
 * @brief Convert LilyAstExprBinaryKind in precedence (Uint8).
 */
Uint8
to_precedence__LilyAstExprBinaryKind(enum LilyAstExprBinaryKind self);

/**
 *
 * @brief Convert LilyToken in LilyAstExprBinaryKind.
 */
enum LilyAstExprBinaryKind
from_token__LilyAstExprBinary(const LilyToken *token);

/**
 *
 * @brief Free LilyAstExprBinary type.
 */
DESTRUCTOR(LilyAstExprBinary, const LilyAstExprBinary *self);

#endif // LILY_CORE_LILY_PARSER_AST_EXPR_BINARY_H
