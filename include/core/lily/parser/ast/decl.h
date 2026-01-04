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

#ifndef LILY_CORE_LILY_PARSER_AST_DECL_H
#define LILY_CORE_LILY_PARSER_AST_DECL_H

#include <base/macros.h>

#include <core/lily/parser/ast/decl/error.h>
#include <core/lily/parser/ast/decl/fun.h>
#include <core/lily/parser/ast/decl/include.h>
#include <core/lily/parser/ast/decl/method.h>
#include <core/lily/parser/ast/decl/module.h>
#include <core/lily/parser/ast/decl/object.h>
#include <core/lily/parser/ast/decl/type.h>
#include <core/lily/parser/ast/decl/use.h>

enum LilyAstDeclKind
{
    LILY_AST_DECL_KIND_CONSTANT,
    LILY_AST_DECL_KIND_ERROR,
    LILY_AST_DECL_KIND_FUN,
    LILY_AST_DECL_KIND_INCLUDE,
    LILY_AST_DECL_KIND_METHOD,
    LILY_AST_DECL_KIND_MODULE,
    LILY_AST_DECL_KIND_OBJECT,
    LILY_AST_DECL_KIND_TYPE,
    LILY_AST_DECL_KIND_USE
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
        LilyAstDeclConstant constant;
        LilyAstDeclError error;
        LilyAstDeclFun fun;
        LilyAstDeclInclude include;
        LilyAstDeclMethod method;
        LilyAstDeclModule module;
        LilyAstDeclObject object;
        LilyAstDeclType type;
        LilyAstDeclUse use;
    };
} LilyAstDecl;

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
 * @brief Construct LilyAstDecl type (LILY_AST_DECL_KIND_ERROR).
 */
VARIANT_CONSTRUCTOR(LilyAstDecl *,
                    LilyAstDecl,
                    error,
                    Location location,
                    LilyAstDeclError error);

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
 * @brief Construct LilyAstDecl type (LILY_AST_DECL_KIND_INCLUDE).
 */
VARIANT_CONSTRUCTOR(LilyAstDecl *,
                    LilyAstDecl,
                    include,
                    Location location,
                    LilyAstDeclInclude include);

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
 * @brief Construct LilyAstDecl type (LILY_AST_DECL_KIND_MODULE).
 */
VARIANT_CONSTRUCTOR(LilyAstDecl *,
                    LilyAstDecl,
                    module,
                    Location location,
                    LilyAstDeclModule module);

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
 * @brief Construct LilyAstDecl type (LILY_AST_DECL_KIND_USE).
 */
VARIANT_CONSTRUCTOR(LilyAstDecl *,
                    LilyAstDecl,
                    use,
                    Location location,
                    LilyAstDeclUse use);

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

#endif // LILY_CORE_LILY_PARSER_AST_DECL_H
