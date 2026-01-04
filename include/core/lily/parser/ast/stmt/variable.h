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

#ifndef LILY_CORE_LILY_PARSER_AST_STMT_VARIABLE_H
#define LILY_CORE_LILY_PARSER_AST_STMT_VARIABLE_H

#include <base/macros.h>
#include <base/string.h>

#include <core/lily/parser/ast/data_type.h>
#include <core/lily/parser/ast/expr.h>

typedef struct LilyAstStmtVariable
{
    String *name;
    LilyAstDataType *data_type; // LilyAstDataType*?
    LilyAstExpr *expr;          // LilyAstExpr*
    bool is_mut;
    bool is_trace;
    bool is_ref;
    bool is_drop;
} LilyAstStmtVariable;

/**
 *
 * @brief Construct LilyAstStmtVariable type.
 */
inline CONSTRUCTOR(LilyAstStmtVariable,
                   LilyAstStmtVariable,
                   String *name,
                   LilyAstDataType *data_type,
                   LilyAstExpr *expr,
                   bool is_mut,
                   bool is_trace,
                   bool is_ref,
                   bool is_drop)
{
    return (LilyAstStmtVariable){ .name = name,
                                  .data_type = data_type,
                                  .expr = expr,
                                  .is_mut = is_mut,
                                  .is_trace = is_trace,
                                  .is_ref = is_ref,
                                  .is_drop = is_drop };
}

/**
 *
 * @brief Convert LilyAstStmtVariable in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyAstStmtVariable, const LilyAstStmtVariable *self);
#endif

/**
 *
 * @brief Free LilyAstStmtVariable type.
 */
DESTRUCTOR(LilyAstStmtVariable, const LilyAstStmtVariable *self);

#endif
