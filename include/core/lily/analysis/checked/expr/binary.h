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

#ifndef LILY_CORE_LILY_ANALYSIS_CHECKED_EXPR_BINARY_H
#define LILY_CORE_LILY_ANALYSIS_CHECKED_EXPR_BINARY_H

#include <base/macros.h>
#include <base/string.h>

#include <core/lily/scanner/token.h>

#define MAX_LILY_CHECKED_EXPR_BINARY_PRECEDENCE 100

typedef struct LilyCheckedExpr LilyCheckedExpr;

// The binary representation is only used when the operator is considered
// default. See `src/core/lily/checked/operator.c` for more information on
// default operators. NOTE: User-defined operators are represented by a function
// call.
enum LilyCheckedExprBinaryKind
{
    LILY_CHECKED_EXPR_BINARY_KIND_ADD,                // +
    LILY_CHECKED_EXPR_BINARY_KIND_AND,                // and
    LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_ADD,         // +=
    LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_BIT_AND,     // &=
    LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_BIT_L_SHIFT, // <<=
    LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_BIT_OR,      // |=
    LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_BIT_R_SHIFT, // >>=
    LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_DIV,         // /=
    LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_EXP,         // **=
    LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_MOD,         // %=
    LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_MUL,         // *=
    LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_SUB,         // -=
    LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN_XOR,         // xor=
    LILY_CHECKED_EXPR_BINARY_KIND_ASSIGN,             // =
    LILY_CHECKED_EXPR_BINARY_KIND_BIT_AND,            // &
    LILY_CHECKED_EXPR_BINARY_KIND_BIT_OR,             // |
    LILY_CHECKED_EXPR_BINARY_KIND_CHAIN,              // |>
    LILY_CHECKED_EXPR_BINARY_KIND_DIV,                // /
    LILY_CHECKED_EXPR_BINARY_KIND_EQ,                 // ==
    LILY_CHECKED_EXPR_BINARY_KIND_EXP,                // **
    LILY_CHECKED_EXPR_BINARY_KIND_GREATER_EQ,         // >=
    LILY_CHECKED_EXPR_BINARY_KIND_GREATER,            // >
    LILY_CHECKED_EXPR_BINARY_KIND_BIT_L_SHIFT,        // <<
    LILY_CHECKED_EXPR_BINARY_KIND_LESS_EQ,            // <=
    LILY_CHECKED_EXPR_BINARY_KIND_LESS,               // <
    LILY_CHECKED_EXPR_BINARY_KIND_MOD,                // %
    LILY_CHECKED_EXPR_BINARY_KIND_MUL,                // *
    LILY_CHECKED_EXPR_BINARY_KIND_NOT_EQ,             // not=
    LILY_CHECKED_EXPR_BINARY_KIND_OR,                 // or
    LILY_CHECKED_EXPR_BINARY_KIND_BIT_R_SHIFT,        // >>
    LILY_CHECKED_EXPR_BINARY_KIND_RANGE,              // ..
    LILY_CHECKED_EXPR_BINARY_KIND_SUB,                // -
    LILY_CHECKED_EXPR_BINARY_KIND_XOR,                // xor
    LILY_CHECKED_EXPR_BINARY_KIND_LIST_HEAD,          // ->
    LILY_CHECKED_EXPR_BINARY_KIND_LIST_TAIL,          // <-

};

typedef struct LilyCheckedExprBinary
{
    enum LilyCheckedExprBinaryKind kind;
    LilyCheckedExpr *left;
    LilyCheckedExpr *right;
} LilyCheckedExprBinary;

/**
 *
 * @brief Construct LilyCheckedExprBinary type.
 */
inline CONSTRUCTOR(LilyCheckedExprBinary,
                   LilyCheckedExprBinary,
                   enum LilyCheckedExprBinaryKind kind,
                   LilyCheckedExpr *left,
                   LilyCheckedExpr *right)
{
    return (
      LilyCheckedExprBinary){ .kind = kind, .left = left, .right = right };
}

/**
 *
 * @brief Convert LilyCheckedExprBinaryKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprBinaryKind,
               enum LilyCheckedExprBinaryKind self);
#endif

/**
 *
 * @brief Convert LilyCheckedExprBinary in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprBinary,
               const LilyCheckedExprBinary *self);
#endif

/**
 *
 * @brief Convert LilyCheckedExprBinaryKind in string.
 */
char *
to_string__LilyCheckedExprBinaryKind(enum LilyCheckedExprBinaryKind kind);

/**
 *
 * @brief Return true if it's an assign operator.
 */
bool
is_assign__LilyCheckedExprBinaryKind(enum LilyCheckedExprBinaryKind kind);

/**
 *
 * @brief Check if the both binaries expression are equal.
 */
bool
eq__LilyCheckedExprBinary(const LilyCheckedExprBinary *self,
                          const LilyCheckedExprBinary *other);

/**
 *
 * @brief Free LilyCheckedExprBinary type.
 */
DESTRUCTOR(LilyCheckedExprBinary, const LilyCheckedExprBinary *self);

#endif // LILY_CORE_LILY_ANALYSIS_CHECKED_EXPR_BINARY_H
