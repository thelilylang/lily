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

#ifndef LILY_CORE_LILY_ANALYSIS_CHECKED_STMT_TRY_H
#define LILY_CORE_LILY_ANALYSIS_CHECKED_STMT_TRY_H

#include <base/macros.h>

#include <core/lily/analysis/checked/expr.h>

#ifdef ENV_DEBUG
#include <base/string.h>
#endif

typedef struct LilyCheckedStmtTry
{
    Vec *try_body; // Vec<LilyCheckedBodyFunItem*>*
    LilyCheckedScope *try_scope;
    Vec *catch_body;               // Vec<LilyCheckedBodyFunItem*>*?
    LilyCheckedScope *catch_scope; // LilyCheckedScope*?
} LilyCheckedStmtTry;

/**
 *
 * @brief Construct LilyCheckedStmtTry type.
 */
inline CONSTRUCTOR(LilyCheckedStmtTry,
                   LilyCheckedStmtTry,
                   Vec *try_body,
                   LilyCheckedScope *try_scope,
                   Vec *catch_body,
                   LilyCheckedScope *catch_scope)
{
    return (LilyCheckedStmtTry){ .try_body = try_body,
                                 .try_scope = try_scope,
                                 .catch_body = catch_body,
                                 .catch_scope = catch_scope };
}

/**
 *
 * @brief Convert LilyCheckedStmtTry in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyCheckedStmtTry, const LilyCheckedStmtTry *self);
#endif

/**
 *
 * @brief Free LilyCheckedStmtTry type.
 */
DESTRUCTOR(LilyCheckedStmtTry, const LilyCheckedStmtTry *self);

#endif // LILY_CORE_LILY_ANALYSIS_CHECKED_STMT_TRY_H
