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

#ifndef LILY_CORE_LILY_AST_EXPR_ACCESS_H
#define LILY_CORE_LILY_AST_EXPR_ACCESS_H

#include <base/macros.h>
#include <base/string.h>
#include <base/vec.h>

typedef struct LilyAstExpr LilyAstExpr;

enum LilyAstExprAccessKind
{
    LILY_AST_ACCESS_EXPR_KIND_GLOBAL,
    LILY_AST_ACCESS_EXPR_KIND_HOOK,
    LILY_AST_ACCESS_EXPR_KIND_OBJECT,
    LILY_AST_ACCESS_EXPR_KIND_PATH,
    LILY_AST_ACCESS_EXPR_KIND_PROPERTY_INIT,
    LILY_AST_ACCESS_EXPR_KIND_SELF,
};

typedef struct LilyAstExprAccessHook
{
    LilyAstExpr *access;
    LilyAstExpr *id;
} LilyAstExprAccessHook;

/**
 *
 * @brief Construct LilyAstExprAccessHook type.
 */
inline CONSTRUCTOR(LilyAstExprAccessHook,
                   LilyAstExprAccessHook,
                   LilyAstExpr *access,
                   LilyAstExpr *id)
{
    return (LilyAstExprAccessHook){ .access = access, .id = id };
}

/**
 *
 * @brief Convert LilyAstExprAccessHook in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, LilyAstExprAccessHook, const LilyAstExprAccessHook *self);
#endif

typedef struct LilyAstExprAccess
{
    enum LilyAstExprAccessKind kind;
    union
    {
        LilyAstExpr *global;
        LilyAstExprAccessHook hook;
        Vec *object; // Vec<LilyAstDataType*>*
        Vec *path;   // Vec<LilyAstExpr*>*
        LilyAstExpr *property_init;
        LilyAstExpr *self;
    };
} LilyAstExprAccess;

/**
 *
 * @brief Construct LilyAstExprAccess (LILY_AST_ACCESS_EXPR_KIND_GLOBAL).
 */
VARIANT_CONSTRUCTOR(LilyAstExprAccess *,
                    LilyAstExprAccess,
                    global,
                    LilyAstExpr *global);

/**
 *
 * @brief Construct LilyAstExprAccess (LILY_AST_ACCESS_EXPR_KIND_HOOK).
 */
VARIANT_CONSTRUCTOR(LilyAstExprAccess *,
                    LilyAstExprAccess,
                    hook,
                    LilyAstExprAccessHook hook);

/**
 *
 * @brief Construct LilyAstExprAccess (LILY_AST_ACCESS_EXPR_KIND_OBJECT).
 */
VARIANT_CONSTRUCTOR(LilyAstExprAccess *, LilyAstExprAccess, object, Vec *object);

/**
 *
 * @brief Construct LilyAstExprAccess (LILY_AST_ACCESS_EXPR_KIND_PATH).
 */
VARIANT_CONSTRUCTOR(LilyAstExprAccess *, LilyAstExprAccess, path, Vec *path);

/**
 *
 * @brief Construct LilyAstExprAccess (LILY_AST_ACCESS_EXPR_KIND_PROPERTY_INIT).
 */
VARIANT_CONSTRUCTOR(LilyAstExprAccess *,
                    LilyAstExprAccess,
                    property_init,
                    LilyAstExpr *property_init);

/**
 *
 * @brief Construct LilyAstExprAccess (LILY_AST_ACCESS_EXPR_KIND_SELF).
 */
VARIANT_CONSTRUCTOR(LilyAstExprAccess *, LilyAstExprAccess, self, LilyAstExpr *self_);

/**
 *
 * @brief Convert LilyAstExprAccessKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, LilyAstExprAccessKind, enum LilyAstExprAccessKind self);
#endif

/**
 *
 * @brief Convert LilyAstExprAccess in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyAstExprAccess, const LilyAstExprAccess *self);
#endif

/**
 *
 * @brief Convert LilyAstExprAccess in String.
 */
String *
to_string__LilyAstExprAccess(const LilyAstExprAccess *self);

/**
 *
 * @brief Free LilyAstExprAccess type.
 */
DESTRUCTOR(LilyAstExprAccess, LilyAstExprAccess *self);

#endif // LILY_CORE_LILY_AST_EXPR_ACCESS_H