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

#ifndef LILY_CORE_LILY_ANALYSIS_CHECKED_EXPR_CAST_H
#define LILY_CORE_LILY_ANALYSIS_CHECKED_EXPR_CAST_H

#include <core/lily/analysis/checked/data_type.h>

typedef struct LilyCheckedExpr LilyCheckedExpr;

enum LilyCheckedExprCastKind
{
    LILY_CHECKED_EXPR_CAST_KIND_DYNAMIC,
    LILY_CHECKED_EXPR_CAST_KIND_LITERAL,
    LILY_CHECKED_EXPR_CAST_KIND_STRING
};

/**
 *
 * @brief Convert LilyCheckedExprCastKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprCastKind,
               enum LilyCheckedExprCastKind self);
#endif

typedef struct LilyCheckedExprCast
{
    enum LilyCheckedExprCastKind kind;
    LilyCheckedExpr *expr;
    LilyCheckedDataType *dest_data_type;
} LilyCheckedExprCast;

/**
 *
 * @brief Construct LilyCheckedExprCast type.
 */
inline CONSTRUCTOR(LilyCheckedExprCast,
                   LilyCheckedExprCast,
                   enum LilyCheckedExprCastKind kind,
                   LilyCheckedExpr *expr,
                   LilyCheckedDataType *dest_data_type)
{
    return (LilyCheckedExprCast){ .kind = kind,
                                  .expr = expr,
                                  .dest_data_type = dest_data_type };
}

/**
 *
 * @brief Convert LilyCheckedExprCast in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyCheckedExprCast, const LilyCheckedExprCast *self);
#endif

/**
 *
 * @brief Check if a LLVM bitcast.
 */
bool
is_llvm_bitcast__LilyCheckedExprCast(const LilyCheckedExprCast *self);

/**
 *
 * @brief Check if a LLVM trunc cast.
 */
bool
is_llvm_trunc__LilyCheckedExprCast(const LilyCheckedExprCast *self);

/**
 *
 * @brief Check if a LLVM sext cast.
 */
bool
is_llvm_sext__LilyCheckedExprCast(const LilyCheckedExprCast *self);

/**
 *
 * @brief Check if the both casts are equal.
 */
bool
eq__LilyCheckedExprCast(const LilyCheckedExprCast *self,
                        const LilyCheckedExprCast *other);

/**
 *
 * @brief Free LilyCheckedExprCast type.
 */
DESTRUCTOR(LilyCheckedExprCast, const LilyCheckedExprCast *self);

#endif // LILY_CORE_LILY_ANALYSIS_CHECKED_EXPR_CAST_H
