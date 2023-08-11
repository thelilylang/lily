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

#ifndef LILY_CORE_LILY_ANALYSIS_CHECKED_STMT_MATCH_H
#define LILY_CORE_LILY_ANALYSIS_CHECKED_STMT_MATCH_H

#include <core/lily/analysis/checked/expr.h>
#include <core/lily/analysis/checked/pattern.h>

#include <base/ordered_hash_map.h>

typedef struct LilyCheckedBodyFunItem LilyCheckedBodyFunItem;

enum LilyCheckedStmtMatchCaseValueKind
{
    LILY_CHECKED_STMT_MATCH_CASE_VALUE_KIND_INT,
    LILY_CHECKED_STMT_MATCH_CASE_VALUE_KIND_FLOAT,
    LILY_CHECKED_STMT_MATCH_CASE_VALUE_KIND_UINT,
};

typedef struct LilyCheckedStmtMatchCaseValue
{
    enum LilyCheckedStmtMatchCaseValueKind kind;
    union
    {
        Int64 int_;
        Float64 float_;
        Uint64 uint;
    };
} LilyCheckedStmtMatchCaseValue;

/**
 *
 * @brief Construct LilyCheckedStmtMatchCaseValue type
 * (LILY_CHECKED_STMT_MATCH_CASE_VALUE_KIND_INT).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedStmtMatchCaseValue,
                           LilyCheckedStmtMatchCaseValue,
                           int,
                           Int64 int_)
{
    return (LilyCheckedStmtMatchCaseValue){
        .kind = LILY_CHECKED_STMT_MATCH_CASE_VALUE_KIND_INT, .int_ = int_
    };
}

/**
 *
 * @brief Construct LilyCheckedStmtMatchCaseValue type
 * (LILY_CHECKED_STMT_MATCH_CASE_VALUE_KIND_FLOAT).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedStmtMatchCaseValue,
                           LilyCheckedStmtMatchCaseValue,
                           float,
                           Float64 float_)
{
    return (LilyCheckedStmtMatchCaseValue){
        .kind = LILY_CHECKED_STMT_MATCH_CASE_VALUE_KIND_FLOAT, .float_ = float_
    };
}

/**
 *
 * @brief Construct LilyCheckedStmtMatchCaseValue type
 * (LILY_CHECKED_STMT_MATCH_CASE_VALUE_KIND_UINT).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedStmtMatchCaseValue,
                           LilyCheckedStmtMatchCaseValue,
                           uint,
                           Uint64 uint)
{
    return (LilyCheckedStmtMatchCaseValue){
        .kind = LILY_CHECKED_STMT_MATCH_CASE_VALUE_KIND_UINT, .uint = uint
    };
}

/**
 *
 * @brief Check if the both values are equal.
 */
bool
eq__LilyCheckedStmtMatchCaseValue(const LilyCheckedStmtMatchCaseValue *self,
                                  const LilyCheckedStmtMatchCaseValue *other);

typedef struct LilyCheckedStmtMatchCase
{
    Vec *
      captured_variables; // Vec<OrderedHashMap<LilyCheckedCapturedVariable*>*>*
    LilyCheckedStmtMatchCaseValue value;
    LilyCheckedBodyFunItem *body_item;
    Usize subcase_count;
} LilyCheckedStmtMatchCase;

/**
 *
 * @brief Construct LilyCheckedStmtMatchCase type.
 */
CONSTRUCTOR(LilyCheckedStmtMatchCase *,
            LilyCheckedStmtMatchCase,
            LilyCheckedExpr *cond,
            LilyCheckedStmtMatchCaseValue value,
            LilyCheckedBodyFunItem *body_item,
            LilyCheckedScope *parent);

/**
 *
 * @brief Convert LilyCheckedStmtMatchCase in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedStmtMatchCase,
               const LilyCheckedStmtMatchCase *self);
#endif

/**
 *
 * @brief Add captured_variables content to statement match case.
 */
inline void
add_captured_variables__LilyCheckedStmtMatchCase(
  const LilyCheckedStmtMatchCase *self,
  OrderedHashMap *captured_variables)
{
    return push__Vec(self->captured_variables, captured_variables);
}

/**
 *
 * @brief Add a new subcase to statement match case.
 * The compiler will generate a if statement if is not already created otherwise
 * it's add a elif branch or else branch.
 */
void
add_subcase__LilyCheckedStmtMatchCase(LilyCheckedStmtMatchCase *self,
                                      LilyCheckedExpr *cond,
                                      LilyCheckedBodyFunItem *body_item,
                                      LilyCheckedScope *parent);

/**
 *
 * Add `captured_variables`, `cond` and `body_item` to statement match case.
 */
void
add__LilyCheckedStmtMatchCase(const LilyCheckedStmtMatchCase *self,
                              OrderedHashMap *captured_variables,
                              LilyCheckedExpr *cond,
                              LilyCheckedBodyFunItem *body_item);

/**
 *
 * @brief Free LilyCheckedStmtMatchCase type.
 */
DESTRUCTOR(LilyCheckedStmtMatchCase, LilyCheckedStmtMatchCase *self);

typedef struct LilyCheckedStmtMatch
{
    LilyCheckedExpr *expr;
    Vec *cases; // Vec<LilyCheckedStmtMatchCase*>*
    bool use_switch;
    bool has_else;
} LilyCheckedStmtMatch;

/**
 *
 * @brief Construct LilyCheckedStmtMatch type.
 */
inline CONSTRUCTOR(LilyCheckedStmtMatch,
                   LilyCheckedStmtMatch,
                   LilyCheckedExpr *expr,
                   Vec *cases,
                   bool use_switch,
                   bool has_else)
{
    return (LilyCheckedStmtMatch){ .expr = expr,
                                   .cases = cases,
                                   .use_switch = use_switch,
                                   .has_else = has_else };
}

/**
 *
 * @brief Add a new case or use an already pushed case to add
 * `captured_variables`, `cond` and `body_item` to the case.
 * @note If the case if already pushed, we will free pattern parametter
 */
void
add_case__LilyCheckedStmtMatch(const LilyCheckedStmtMatch *self,
                               LilyCheckedPattern *pattern,
                               OrderedHashMap *captured_variables,
                               LilyCheckedExpr *cond,
                               LilyCheckedBodyFunItem *body_item);

/**
 *
 * @return 0 Ok.
 * @return 1 Unused case.
 * @return 2 duplicate case.
 */
int
look_for_case_error__LilyCheckedStmtMatch(const LilyCheckedStmtMatch *self,
                                          LilyCheckedPattern *pattern,
                                          LilyCheckedExpr *cond);

/**
 *
 * @brief Convert LilyCheckedStmtMatch in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedStmtMatch,
               const LilyCheckedStmtMatch *self);
#endif

/**
 *
 * @brief Free LilyCheckedStmtMatch type.
 */
DESTRUCTOR(LilyCheckedStmtMatch, const LilyCheckedStmtMatch *self);

#endif // LILY_CORE_LILY_ANALYSIS_CHECKED_STMT_MATCH_H
