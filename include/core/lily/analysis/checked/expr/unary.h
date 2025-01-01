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

#ifndef LILY_CORE_LILY_ANALYSIS_CHECKED_EXPR_UNARY_H
#define LILY_CORE_LILY_ANALYSIS_CHECKED_EXPR_UNARY_H

#include <base/macros.h>

#ifdef ENV_DEBUG
#include <base/string.h>
#endif

typedef struct LilyCheckedExpr LilyCheckedExpr;

enum LilyCheckedExprUnaryKind
{
    LILY_CHECKED_EXPR_UNARY_KIND_DEREFERENCE,
    LILY_CHECKED_EXPR_UNARY_KIND_NEG,
    LILY_CHECKED_EXPR_UNARY_KIND_NOT,
    LILY_CHECKED_EXPR_UNARY_KIND_REF,
    LILY_CHECKED_EXPR_UNARY_KIND_REF_MUT,
    LILY_CHECKED_EXPR_UNARY_KIND_TRACE,
    LILY_CHECKED_EXPR_UNARY_KIND_TRACE_MUT,
};

/**
 *
 * @brief Convert LilyCheckedExprUnaryKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprUnaryKind,
               enum LilyCheckedExprUnaryKind self);
#endif

typedef struct LilyCheckedExprUnary
{
    enum LilyCheckedExprUnaryKind kind;
    LilyCheckedExpr *right;
} LilyCheckedExprUnary;

/**
 *
 * @brief Construct LilyCheckedExprUnary type.
 */
inline CONSTRUCTOR(LilyCheckedExprUnary,
                   LilyCheckedExprUnary,
                   enum LilyCheckedExprUnaryKind kind,
                   LilyCheckedExpr *right)
{
    return (LilyCheckedExprUnary){ .kind = kind, .right = right };
}

/**
 *
 * @brief Convert LilyCheckedExprUnary in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprUnary,
               const LilyCheckedExprUnary *self);
#endif

/**
 *
 * @brief Convert LilyCheckedExprUnaryKind in string.
 */
char *
to_string__LilyCheckedExprUnaryKind(enum LilyCheckedExprUnaryKind kind);

/**
 *
 * @brief Check if the both unaries are equal.
 */
bool
eq__LilyCheckedExprUnary(const LilyCheckedExprUnary *self,
                         const LilyCheckedExprUnary *other);

/**
 *
 * @brief Free LilyCheckedExprUnary type.
 */
DESTRUCTOR(LilyCheckedExprUnary, const LilyCheckedExprUnary *self);

#endif // LILY_CORE_LILY_ANALYSIS_CHECKED_EXPR_UNARY_H
