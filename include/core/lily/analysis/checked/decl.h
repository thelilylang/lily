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

#ifndef LILY_CORE_LILY_ANALYSIS_CHECKED_DECL_H
#define LILY_CORE_LILY_ANALYSIS_CHECKED_DECL_H

#include <base/macros.h>

#include <core/lily/analysis/checked/decl/error.h>
#include <core/lily/analysis/checked/decl/fun.h>
#include <core/lily/analysis/checked/decl/method.h>
#include <core/lily/analysis/checked/decl/module.h>
#include <core/lily/analysis/checked/decl/object.h>
#include <core/lily/analysis/checked/decl/type.h>
#include <core/lily/parser/ast/decl.h>

enum LilyCheckedDeclKind
{
    LILY_CHECKED_DECL_KIND_CONSTANT,
    LILY_CHECKED_DECL_KIND_ERROR,
    LILY_CHECKED_DECL_KIND_FUN,
    LILY_CHECKED_DECL_KIND_METHOD,
    LILY_CHECKED_DECL_KIND_MODULE,
    LILY_CHECKED_DECL_KIND_OBJECT,
    LILY_CHECKED_DECL_KIND_TYPE
};

/**
 *
 * @brief Convert LilyCheckedDeclKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, LilyCheckedDeclKind, enum LilyCheckedDeclKind self);
#endif

typedef struct LilyCheckedDecl
{
    enum LilyCheckedDeclKind kind;
    const Location *location;    // const Location* (&)
    const LilyAstDecl *ast_decl; // const LilyAstDecl* (&)
    union
    {
        LilyCheckedDeclConstant constant;
        LilyCheckedDeclError error;
        LilyCheckedDeclFun fun;
        LilyCheckedDeclMethod method;
        LilyCheckedDeclModule module;
        LilyCheckedDeclObject object;
        LilyCheckedDeclType type;
    };
} LilyCheckedDecl;

/**
 *
 * @brief Construct LilyCheckedDecl type (LILY_CHECKED_DECL_KIND_CONSTANT).
 */
VARIANT_CONSTRUCTOR(LilyCheckedDecl *,
                    LilyCheckedDecl,
                    constant,
                    const Location *location,
                    const LilyAstDecl *ast_decl,
                    LilyCheckedDeclConstant constant);

/**
 *
 * @brief Construct LilyCheckedDecl type (LILY_CHECKED_DECL_KIND_ERROR).
 */
VARIANT_CONSTRUCTOR(LilyCheckedDecl *,
                    LilyCheckedDecl,
                    error,
                    const Location *location,
                    const LilyAstDecl *ast_decl,
                    LilyCheckedDeclError error);

/**
 *
 * @brief Construct LilyCheckedDecl type (LILY_CHECKED_DECL_KIND_FUN).
 */
VARIANT_CONSTRUCTOR(LilyCheckedDecl *,
                    LilyCheckedDecl,
                    fun,
                    const Location *location,
                    const LilyAstDecl *ast_decl,
                    LilyCheckedDeclFun fun);

/**
 *
 * @brief Construct LilyCheckedDecl type (LILY_CHECKED_DECL_KIND_METHOD).
 */
VARIANT_CONSTRUCTOR(LilyCheckedDecl *,
                    LilyCheckedDecl,
                    method,
                    const Location *location,
                    const LilyAstDecl *ast_decl,
                    LilyCheckedDeclMethod method);

/**
 *
 * @brief Construct LilyCheckedDecl type (LILY_CHECKED_DECL_KIND_MODULE).
 */
VARIANT_CONSTRUCTOR(LilyCheckedDecl *,
                    LilyCheckedDecl,
                    module,
                    const Location *location,
                    const LilyAstDecl *ast_decl,
                    LilyCheckedDeclModule module);

/**
 *
 * @brief Construct LilyCheckedDecl type (LILY_CHECKED_DECL_KIND_OBJECT).
 */
VARIANT_CONSTRUCTOR(LilyCheckedDecl *,
                    LilyCheckedDecl,
                    object,
                    const Location *location,
                    const LilyAstDecl *ast_decl,
                    LilyCheckedDeclObject object);

/**
 *
 * @brief Construct LilyCheckedDecl type (LILY_CHECKED_DECL_KIND_TYPE).
 */
VARIANT_CONSTRUCTOR(LilyCheckedDecl *,
                    LilyCheckedDecl,
                    type,
                    const Location *location,
                    const LilyAstDecl *ast_decl,
                    LilyCheckedDeclType type);

/**
 *
 * @brief Get the global name of the declaration.
 * @return String* (&)
 */
String *
get_global_name__LilyCheckedDecl(const LilyCheckedDecl *self);

/**
 *
 * @brief Get signatures from declaration.
 * @return Vec<T*>* (&)
 * T = LilyCheckedSignatureFun* | LilyCheckedSignatureType*
 */
Vec *
get_signatures__LilyCheckedDecl(const LilyCheckedDecl *self);

/**
 *
 * @brief Get scope from declaration.
 * @return LilyCheckedScope*? (&)
 */
LilyCheckedScope *
get_scope__LilyCheckedDecl(const LilyCheckedDecl *self);

/**
 *
 * @brief Get the generic params from declaration.
 * @return const Vec<LilyCheckedGenericParam*>*? (&)
 */
const Vec *
get_generic_params__LilyCheckedDecl(const LilyCheckedDecl *self);

/**
 *
 * @brief Check the lenght of the generic params.
 * @param called_generic_params Vec<LilyAstDataType*>*? (&)
 * @return If the lenght of the generic params is not the same as the called
 * one, return 1, otherwise return 0.
 */
int
verify_generic_params__LilyCheckedDecl(const LilyCheckedDecl *self,
                                       Vec *called_generic_params);

/**
 *
 * @brief Return if the declaration is checked.
 */
bool
is_checked__LilyCheckedDecl(const LilyCheckedDecl *self);

/**
 *
 * @brief Convert LilyCheckedDecl in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyCheckedDecl, const LilyCheckedDecl *self);
#endif

/**
 *
 * @brief Print debug LilyCheckedDecl struct.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
void
IMPL_FOR_DEBUG(debug, LilyCheckedDecl, const LilyCheckedDecl *self);
#endif

/**
 *
 * @brief Free LilyCheckedDecl type.
 */
DESTRUCTOR(LilyCheckedDecl, LilyCheckedDecl *self);

#endif // LILY_CORE_LILY_ANALYSIS_CHECKED_DECL_H
