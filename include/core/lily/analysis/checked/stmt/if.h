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

#ifndef LILY_CORE_LILY_ANALYSIS_CHECKED_STMT_IF_H
#define LILY_CORE_LILY_ANALYSIS_CHECKED_STMT_IF_H

#include <core/lily/analysis/checked/expr.h>
#include <core/lily/analysis/checked/scope.h>

typedef struct LilyCheckedStmtIfBranch
{
    LilyCheckedExpr *cond;
    Vec *body; // Vec<LilyCheckedBodyFunItem*>*
    LilyCheckedScope *scope;
} LilyCheckedStmtIfBranch;

/**
 *
 * @brief Construct LilyCheckedStmtIfBranch type.
 */
CONSTRUCTOR(LilyCheckedStmtIfBranch *,
            LilyCheckedStmtIfBranch,
            LilyCheckedExpr *cond,
            Vec *body,
            LilyCheckedScope *scope);

/**
 *
 * @brief Convert LilyCheckedStmtIfBranch in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedStmtIfBranch,
               const LilyCheckedStmtIfBranch *self);
#endif

/**
 *
 * @brief Free LilyCheckedStmtIfBranch type.
 */
DESTRUCTOR(LilyCheckedStmtIfBranch, LilyCheckedStmtIfBranch *self);

typedef struct LilyCheckedStmtElseBranch
{
    Vec *body; // Vec<LilyCheckedBodyFunItem*>*
    LilyCheckedScope *scope;
} LilyCheckedStmtElseBranch;

/**
 *
 * @brief Construct LilyCheckedStmtElseBranch type.
 */
CONSTRUCTOR(LilyCheckedStmtElseBranch *,
            LilyCheckedStmtElseBranch,
            Vec *body,
            LilyCheckedScope *scope);

/**
 *
 * @brief Convert LilyCheckedStmtElseBranch in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedStmtElseBranch,
               const LilyCheckedStmtElseBranch *self);
#endif

/**
 *
 * @brief Free LilyCheckedStmtElseBranch type.
 */
DESTRUCTOR(LilyCheckedStmtElseBranch, LilyCheckedStmtElseBranch *self);

typedef struct LilyCheckedStmtIf
{
    LilyCheckedStmtIfBranch *if_;
    Vec *elifs;                       // Vec<LilyCheckedStmtIfBranch*>*?
    LilyCheckedStmtElseBranch *else_; // LilyCheckedStmtElseBranch*?
} LilyCheckedStmtIf;

/**
 *
 * @brief Construct LilyCheckedStmtIf type.
 */
inline CONSTRUCTOR(LilyCheckedStmtIf,
                   LilyCheckedStmtIf,
                   LilyCheckedStmtIfBranch *if_,
                   Vec *elifs,
                   LilyCheckedStmtElseBranch *else_)
{
    return (LilyCheckedStmtIf){ .if_ = if_, .elifs = elifs, .else_ = else_ };
}

/**
 *
 * @brief Convert LilyCheckedStmtIf in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyCheckedStmtIf, const LilyCheckedStmtIf *self);
#endif

/**
 *
 * @brief Free LilyCheckedStmtIf type.
 */
DESTRUCTOR(LilyCheckedStmtIf, const LilyCheckedStmtIf *self);

#endif // LILY_CORE_LILY_ANALYSIS_CHECKED_STMT_IF_H
