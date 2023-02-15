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

#ifndef LILY_CORE_LILY_AST_EXPR_H
#define LILY_CORE_LILY_AST_EXPR_H

#include <core/lily/ast/expr/access.h>
#include <core/lily/ast/expr/array.h>
#include <core/lily/ast/expr/binary.h>
#include <core/lily/ast/expr/call.h>
#include <core/lily/ast/expr/cast.h>
#include <core/lily/ast/expr/identifier.h>
#include <core/lily/ast/expr/identifier_macro.h>
#include <core/lily/ast/expr/lambda.h>
#include <core/lily/ast/expr/literal.h>
#include <core/lily/ast/expr/tuple.h>
#include <core/lily/ast/expr/unary.h>

enum LilyAstExprKind
{
    LILY_AST_EXPR_KIND_ACCESS,
    LILY_AST_EXPR_KIND_ARRAY,
    LILY_AST_EXPR_KIND_BINARY,
    LILY_AST_EXPR_KIND_CALL,
    LILY_AST_EXPR_KIND_IDENTIFIER,
    LILY_AST_EXPR_KIND_IDENTIFIER_MACRO,
    LILY_AST_EXPR_KIND_LAMBDA,
    LILY_AST_EXPR_KIND_LITERAL,
    LILY_AST_EXPR_KIND_SELF,
    LILY_AST_EXPR_KIND_TUPLE,
    LILY_AST_EXPR_KIND_UNARY,
    LILY_AST_EXPR_KIND_GROUPING,
    LILY_AST_EXPR_KIND_WILDCARD
};

typedef struct LilyAstExpr
{
    enum LilyAstExprKind kind;
    union
    {
        LilyAstAccess access;
    };
} LilyAstExpr;

/**
 *
 * @brief Free LilyAstExpr type.
 */
DESTRUCTOR(LilyAstExpr, LilyAstExpr *self);

#endif // LILY_CORE_LILY_AST_EXPR_H