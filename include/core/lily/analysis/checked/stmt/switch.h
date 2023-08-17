/*
 * MIT License
 *
 * Copyright (c) 2022-2023 ArthurPV
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

#ifndef LILY_CORE_LILY_ANALYSIS_CHECKED_STMT_SWITCH_H
#define LILY_CORE_LILY_ANALYSIS_CHECKED_STMT_SWITCH_H

#include <base/alloc.h>
#include <base/macros.h>
#include <base/types.h>
#include <base/vec.h>

#include <core/lily/analysis/checked/expr.h>

typedef struct LilyCheckedBodyFunItem LilyCheckedBodyFunItem;

enum LilyCheckedStmtSwitchCaseValueKind
{
    LILY_CHECKED_STMT_SWITCH_CASE_VALUE_KIND_INT,
    LILY_CHECKED_STMT_SWITCH_CASE_VALUE_KIND_FLOAT,
    LILY_CHECKED_STMT_SWITCH_CASE_VALUE_KIND_UINT,
    LILY_CHECKED_STMT_SWITCH_CASE_VALUE_KIND_ELSE
};

/**
 *
 * @brief Convert LilyCheckedStmtSwitchCaseValueKind in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedStmtSwitchCaseValueKind,
               enum LilyCheckedStmtSwitchCaseValueKind self);
#endif

typedef struct LilyCheckedStmtSwitchCaseValue
{
    enum LilyCheckedStmtSwitchCaseValueKind kind;
    union
    {
        Int64 int_;
        Float64 float_;
        Uint64 uint;
    };
} LilyCheckedStmtSwitchCaseValue;

/**
 *
 * @brief Construct LilyCheckedStmtSwitchCaseValue type
 * (LILY_CHECKED_STMT_SWITCH_CASE_VALUE_KIND_INT).
 */
VARIANT_CONSTRUCTOR(LilyCheckedStmtSwitchCaseValue *,
                    LilyCheckedStmtSwitchCaseValue,
                    int,
                    Int64 int_);

/**
 *
 * @brief Construct LilyCheckedStmtSwitchCaseValue type
 * (LILY_CHECKED_STMT_SWITCH_CASE_VALUE_KIND_FLOAT).
 */
VARIANT_CONSTRUCTOR(LilyCheckedStmtSwitchCaseValue *,
                    LilyCheckedStmtSwitchCaseValue,
                    float,
                    Float64 float_);

/**
 *
 * @brief Construct LilyCheckedStmtSwitchCaseValue type
 * (LILY_CHECKED_STMT_SWITCH_CASE_VALUE_KIND_UINT).
 */
VARIANT_CONSTRUCTOR(LilyCheckedStmtSwitchCaseValue *,
                    LilyCheckedStmtSwitchCaseValue,
                    uint,
                    Uint64 uint);

/**
 *
 * @brief Construct LilyCheckedStmtSwitchCaseValue type
 * (LILY_CHECKED_STMT_SWITCH_CASE_VALUE_KIND_ELSE).
 */
VARIANT_CONSTRUCTOR(LilyCheckedStmtSwitchCaseValue *,
                    LilyCheckedStmtSwitchCaseValue,
                    else);

/**
 *
 * @brief Check if the both values are equal.
 */
bool
eq__LilyCheckedStmtSwitchCaseValue(const LilyCheckedStmtSwitchCaseValue *self,
                                   const LilyCheckedStmtSwitchCaseValue *other);

/**
 *
 * @brief Convert LilyCheckedStmtSwitchCaseValue in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedStmtSwitchCaseValue,
               const LilyCheckedStmtSwitchCaseValue *self);
#endif

/**
 *
 * @brief Free LilyCheckedStmtSwitchCaseValue type.
 */
inline DESTRUCTOR(LilyCheckedStmtSwitchCaseValue,
                  LilyCheckedStmtSwitchCaseValue *self)
{
    return lily_free(self);
}

typedef struct LilyCheckedStmtSwitchCase
{
    Vec *values; // Vec<LilyCheckedStmtSwitchCaseValue*>*
    LilyCheckedBodyFunItem *body_item;
} LilyCheckedStmtSwitchCase;

/**
 *
 * @brief Construct LilyCheckedStmtSwitchCase type.
 */
CONSTRUCTOR(LilyCheckedStmtSwitchCase *,
            LilyCheckedStmtSwitchCase,
            LilyCheckedStmtSwitchCaseValue *value,
            LilyCheckedExpr *cond,
            LilyCheckedBodyFunItem *body_item);

/**
 *
 * @brief Free LilyCheckedStmtSwitchCase type.
 */
DESTRUCTOR(LilyCheckedStmtSwitchCase, LilyCheckedStmtSwitchCase *self);

typedef struct LilyCheckedStmtSwitch
{
    LilyCheckedExpr *switched_expr;
    Vec *cases; // Vec<LilyCheckedStmtSwitchCase*>*
} LilyCheckedStmtSwitch;

/**
 *
 * @brief Construct LilyCheckedStmtSwitch type.
 */
inline CONSTRUCTOR(LilyCheckedStmtSwitch,
                   LilyCheckedStmtSwitch,
                   LilyCheckedExpr *switched_expr,
                   Vec *cases)
{
    return (LilyCheckedStmtSwitch){ .switched_expr = switched_expr,
                                    .cases = cases };
}

/**
 *
 * @brief Free LilyCheckedStmtSwitch type.
 */
DESTRUCTOR(LilyCheckedStmtSwitch, const LilyCheckedStmtSwitch *self);

#endif // LILY_CORE_LILY_ANALYSIS_CHECKED_STMT_SWITCH_H
