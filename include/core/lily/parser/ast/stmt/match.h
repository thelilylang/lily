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

#ifndef LILY_CORE_LILY_PARSER_AST_STMT_MATCH_H
#define LILY_CORE_LILY_PARSER_AST_STMT_MATCH_H

#include <core/lily/parser/ast/expr.h>
#include <core/lily/parser/ast/pattern.h>

typedef struct LilyAstBodyFunItem LilyAstBodyFunItem;

typedef struct LilyAstStmtMatchCase
{
    LilyAstPattern *pattern;
    LilyAstExpr *cond; // LilyAstExpr*?
    LilyAstBodyFunItem *body_item;
} LilyAstStmtMatchCase;

/**
 *
 * @brief Construct LilyAstStmtMatchCase type.
 */
CONSTRUCTOR(LilyAstStmtMatchCase *,
            LilyAstStmtMatchCase,
            LilyAstPattern *pattern,
            LilyAstExpr *cond,
            LilyAstBodyFunItem *body_item);

/**
 *
 * @brief Convert LilyAstStmtMatchCase in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyAstStmtMatchCase,
               const LilyAstStmtMatchCase *self);
#endif

/**
 *
 * @brief Free LilyAstStmtMatchCase type.
 */
DESTRUCTOR(LilyAstStmtMatchCase, LilyAstStmtMatchCase *self);

typedef struct LilyAstStmtMatch
{
    LilyAstExpr *expr;
    Vec *cases; // Vec<LilyAstStmtMatchCase*>*
} LilyAstStmtMatch;

/**
 *
 * @brief Construct LilyAstStmtMatch type.
 */
inline CONSTRUCTOR(LilyAstStmtMatch,
                   LilyAstStmtMatch,
                   LilyAstExpr *expr,
                   Vec *cases)
{
    return (LilyAstStmtMatch){ .expr = expr, .cases = cases };
}

/**
 *
 * @brief Convert LilyAstStmtMatch in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyAstStmtMatch, const LilyAstStmtMatch *self);
#endif

/**
 *
 * @brief Free LilyAstStmtMatch type.
 */
DESTRUCTOR(LilyAstStmtMatch, const LilyAstStmtMatch *self);

#endif // LILY_CORE_LILY_PARSER_AST_STMT_MATCH_H
