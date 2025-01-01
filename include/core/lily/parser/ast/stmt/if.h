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

#ifndef LILY_CORE_LILY_PARSER_AST_STMT_IF_H
#define LILY_CORE_LILY_PARSER_AST_STMT_IF_H

#include <core/lily/parser/ast/capture.h>
#include <core/lily/parser/ast/expr.h>

typedef struct LilyAstStmtIf
{
    LilyAstExpr *if_expr;
    LilyAstCapture *if_capture; // LilyAstCapture*?
    Vec *if_body;               // Vec<LilyAstBodyFunItem*>*
    Vec *elif_exprs;            // Vec<LilyAstExpr*>*?
    Vec *elif_captures;         // Vec<LilyAstCapture*?>*?
    Vec *elif_bodies;           // Vec<Vec<LilyAstBodyFunItem*>*>*?
    Vec *else_body;             // Vec<LilyAstBodyFunItem*>*?
} LilyAstStmtIf;

/**
 *
 * @brief Construct LilyAstStmtIf type.
 */
inline CONSTRUCTOR(LilyAstStmtIf,
                   LilyAstStmtIf,
                   LilyAstExpr *if_expr,
                   LilyAstCapture *if_capture,
                   Vec *if_body,
                   Vec *elif_exprs,
                   Vec *elif_captures,
                   Vec *elif_bodies,
                   Vec *else_body)
{
    return (LilyAstStmtIf){ .if_expr = if_expr,
                            .if_capture = if_capture,
                            .if_body = if_body,
                            .elif_exprs = elif_exprs,
                            .elif_captures = elif_captures,
                            .elif_bodies = elif_bodies,
                            .else_body = else_body };
}

/**
 *
 * @brief Convert LilyAstStmtIf in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyAstStmtIf, const LilyAstStmtIf *self);
#endif

/**
 *
 * @brief Free LilyAstStmtIf type.
 */
DESTRUCTOR(LilyAstStmtIf, const LilyAstStmtIf *self);

#endif // LILY_CORE_LILY_PARSER_AST_STMT_IF_H
