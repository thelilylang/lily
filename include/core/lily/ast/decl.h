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

#ifndef LILY_CORE_LILY_AST_DECL_H
#define LILY_CORE_LILY_AST_DECL_H

#include <base/macros.h>

#include <core/lily/ast/decl/alias.h>
#include <core/lily/ast/decl/fun.h>
#include <core/lily/ast/decl/method.h>
#include <core/lily/ast/decl/object.h>
#include <core/lily/ast/decl/type.h>

enum LilyAstDeclKind
{
    LILY_AST_DECL_KIND_ALIAS,
    LILY_AST_DECL_KIND_CONSTANT,
    LILY_AST_DECL_KIND_FUN,
    LILY_AST_DECL_KIND_METHOD,
    LILY_AST_DECL_KIND_OBJECT,
    LILY_AST_DECL_KIND_TYPE
};

/**
 *
 * @brief Convert LilyAstDeclKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, LilyAstDeclKind, enum LilyAstDeclKind self);
#endif

typedef struct LilyAstDecl
{
    enum LilyAstDeclKind kind;
    Location location;
    union
    {
        LilyAstDeclAlias alias;
        LilyAstDeclConstant constant;
        LilyAstDeclFun fun;
        LilyAstDeclMethod method;
        LilyAstDeclObject object;
        LilyAstDeclType type;
    };
} LilyAstDecl;

/**
 *
 * @brief Construct LilyAstDecl type (LILY_AST_DECL_KIND_ALIAS).
 */
VARIANT_CONSTRUCTOR(LilyAstDecl *,
                    LilyAstDecl,
                    alias,
                    Location location,
                    LilyAstDeclAlias alias);

/**
 *
 * @brief Construct LilyAstDecl type (LILY_AST_DECL_KIND_CONSTANT).
 */
VARIANT_CONSTRUCTOR(LilyAstDecl *,
                    LilyAstDecl,
                    constant,
                    Location location,
                    LilyAstDeclConstant constant);

/**
 *
 * @brief Construct LilyAstDecl type (LILY_AST_DECL_KIND_FUN).
 */
VARIANT_CONSTRUCTOR(LilyAstDecl *,
                    LilyAstDecl,
                    fun,
                    Location location,
                    LilyAstDeclFun fun);

/**
 *
 * @brief Construct LilyAstDecl type (LILY_AST_DECL_KIND_METHOD).
 */
VARIANT_CONSTRUCTOR(LilyAstDecl *,
                    LilyAstDecl,
                    method,
                    Location location,
                    LilyAstDeclMethod method);

/**
 *
 * @brief Construct LilyAstDecl type (LILY_AST_DECL_KIND_OBJECT).
 */
VARIANT_CONSTRUCTOR(LilyAstDecl *,
                    LilyAstDecl,
                    object,
                    Location location,
                    LilyAstDeclObject object);

/**
 *
 * @brief Construct LilyAstDecl type (LILY_AST_DECL_KIND_TYPE).
 */
VARIANT_CONSTRUCTOR(LilyAstDecl *,
                    LilyAstDecl,
                    type,
                    Location location,
                    LilyAstDeclType type);

/**
 *
 * @brief Convert LilyAstDecl in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyAstDecl, const LilyAstDecl *self);
#endif

/**
 *
 * @brief Print debug LilyAstDecl struct.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
void
IMPL_FOR_DEBUG(debug, LilyAstDecl, const LilyAstDecl *self);
#endif

/**
 *
 * @brief Free LilyAstDecl type.
 */
DESTRUCTOR(LilyAstDecl, LilyAstDecl *self);

#endif // LILY_CORE_LILY_AST_DECL_H
