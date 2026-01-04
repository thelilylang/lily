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

#ifndef LILY_CORE_CC_CI_RESOLVER_EXPR_H
#define LILY_CORE_CC_CI_RESOLVER_EXPR_H

#include <core/cc/ci/ast.h>
#include <core/cc/ci/parser.h>

typedef struct CIResolverExpr
{
    const CIParser *parser; // const CIParser*? (&)
    const CIScope *scope;   // const CIScope*? (&)
    bool is_at_preprocessor_time;
} CIResolverExpr;

/**
 *
 * @brief Construct CIResolverExpr type.
 */
inline CONSTRUCTOR(CIResolverExpr,
                   CIResolverExpr,
                   const CIParser *parser,
                   const CIScope *scope,
                   bool is_at_preprocessor_time)
{
    return (CIResolverExpr){ .parser = parser,
                             .scope = scope,
                             .is_at_preprocessor_time =
                               is_at_preprocessor_time };
}

/**
 *
 * @brief Check if the expression is true.
 */
bool
is_true__CIResolverExpr(CIExpr *expr);

/**
 *
 * @brief Check if the expression is NULL.
 */
bool
is_null__CIResolverExpr(CIExpr *expr);

/**
 *
 * @brief Convert resolved expression to literal integer value.
 */
Isize
to_literal_integer_value__CIResolverExpr(CIExpr *expr);

/**
 *
 * @brief Resolve preprocessor condition.
 */
CIExpr *
run__CIResolverExpr(const CIResolverExpr *self, CIExpr *expr);

#endif // LILY_CORE_CC_CI_RESOLVER_EXPR_H
