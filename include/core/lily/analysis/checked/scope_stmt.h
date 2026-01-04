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

#ifndef LILY_CORE_LILY_ANALYSIS_CHECKED_SCOPE_STMT_H
#define LILY_CORE_LILY_ANALYSIS_CHECKED_SCOPE_STMT_H

#include <base/macros.h>
#include <base/ordered_hash_map.h>
#include <base/string.h>

typedef struct LilyCheckedStmtElseBranch LilyCheckedStmtElseBranch;
typedef struct LilyCheckedStmtFor LilyCheckedStmtFor;
typedef struct LilyCheckedStmtIfBranch LilyCheckedStmtIfBranch;
typedef struct LilyCheckedStmtMatchCase LilyCheckedStmtMatchCase;
typedef struct LilyCheckedStmtWhile LilyCheckedStmtWhile;

enum LilyCheckedScopeStmtKind
{
    LILY_CHECKED_SCOPE_STMT_KIND_ELSE_BRANCH,
    LILY_CHECKED_SCOPE_STMT_KIND_FOR,
    LILY_CHECKED_SCOPE_STMT_KIND_IF_BRANCH,
    LILY_CHECKED_SCOPE_STMT_KIND_MATCH_CASE,
    LILY_CHECKED_SCOPE_STMT_KIND_WHILE
};

/**
 *
 * @brief Convert LilyCheckedScopeStmtKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedScopeStmtKind,
               enum LilyCheckedScopeStmtKind self);
#endif

typedef struct LilyCheckedScopeStmt
{
    enum LilyCheckedScopeStmtKind kind;
    union
    {
        const LilyCheckedStmtElseBranch
          *else_branch;                 // const LilyCheckedStmtElseBranch* (&)
        const LilyCheckedStmtFor *for_; // const LilyCheckedStmtFor* (&)
        const LilyCheckedStmtIfBranch
          *if_branch; // const LilyCheckedStmtIfBranch* (&)
        const LilyCheckedStmtMatchCase
          *match_case; // const LilyCheckedStmtMatchCase* (&)
        const LilyCheckedStmtWhile *while_; // const LilyCheckedStmtWhile* (&)
    };
} LilyCheckedScopeStmt;

/**
 *
 * @brief Construct LilyCheckedScopeStmt type
 * (LILY_CHECKED_SCOPE_STMT_KIND_ELSE_BRANCH).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedScopeStmt,
                           LilyCheckedScopeStmt,
                           else_branch,
                           const LilyCheckedStmtElseBranch *else_branch)
{
    return (LilyCheckedScopeStmt){ .kind =
                                     LILY_CHECKED_SCOPE_STMT_KIND_ELSE_BRANCH,
                                   .else_branch = else_branch };
}

/**
 *
 * @brief Construct LilyCheckedScopeStmt type
 * (LILY_CHECKED_SCOPE_STMT_KIND_FOR).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedScopeStmt,
                           LilyCheckedScopeStmt,
                           for,
                           const LilyCheckedStmtFor *for_)
{
    return (LilyCheckedScopeStmt){ .kind = LILY_CHECKED_SCOPE_STMT_KIND_FOR,
                                   .for_ = for_ };
}

/**
 *
 * @brief Construct LilyCheckedScopeStmt type
 * (LILY_CHECKED_SCOPE_STMT_KIND_IF_BRANCH).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedScopeStmt,
                           LilyCheckedScopeStmt,
                           if_branch,
                           const LilyCheckedStmtIfBranch *if_branch)
{
    return (LilyCheckedScopeStmt){ .kind =
                                     LILY_CHECKED_SCOPE_STMT_KIND_IF_BRANCH,
                                   .if_branch = if_branch };
}

/**
 *
 * @brief Construct LilyCheckedScopeStmt type
 * (LILY_CHECKED_SCOPE_STMT_KIND_MATCH_CASE).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedScopeStmt,
                           LilyCheckedScopeStmt,
                           match_case,
                           const LilyCheckedStmtMatchCase *match_case)
{
    return (LilyCheckedScopeStmt){ .kind =
                                     LILY_CHECKED_SCOPE_STMT_KIND_MATCH_CASE,
                                   .match_case = match_case };
}

/**
 *
 * @brief Construct LilyCheckedScopeStmt type
 * (LILY_CHECKED_SCOPE_STMT_KIND_WHILE).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedScopeStmt,
                           LilyCheckedScopeStmt,
                           while,
                           const LilyCheckedStmtWhile *while_)
{
    return (LilyCheckedScopeStmt){ .kind = LILY_CHECKED_SCOPE_STMT_KIND_WHILE,
                                   .while_ = while_ };
}

/**
 *
 * @brief Get `captured_variables`.
 */
OrderedHashMap *
get_captured_variables__LilyCheckedScopeStmt(const LilyCheckedScopeStmt *self);

/**
 *
 * @brief Convert LilyCheckedScopeStmt in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedScopeStmt,
               const LilyCheckedScopeStmt *self);
#endif

#endif // LILY_CORE_LILY_ANALYSIS_CHECKED_SCOPE_STMT_H
