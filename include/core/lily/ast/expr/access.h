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

enum LilyAstAccessKind
{
    LILY_AST_ACCESS_KIND_GLOBAL,
    LILY_AST_ACCESS_KIND_HOOK,
    LILY_AST_ACCESS_KIND_OBJECT,
    LILY_AST_ACCESS_KIND_PATH,
    LILY_AST_ACCESS_KIND_PROPERTY_INIT,
    LILY_AST_ACCESS_KIND_SELF,
};

typedef struct LilyAstAccessHook
{
    LilyAstExpr *access;
    LilyAstExpr *id;
} LilyAstAccessHook;

/**
 *
 * @brief Construct LilyAstAccessHook type.
 */
inline CONSTRUCTOR(LilyAstAccessHook,
                   LilyAstAccessHook,
                   LilyAstExpr *access,
                   LilyAstExpr *id)
{
    return (LilyAstAccessHook){ .access = access, .id = id };
}

/**
 *
 * @brief Convert LilyAstAccessHook in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, LilyAstAccessHook, const LilyAstAccessHook *self);
#endif

typedef struct LilyAstAccess
{
    enum LilyAstAccessKind kind;
    union
    {
        LilyAstExpr *global;
        LilyAstAccessHook hook;
        Vec *object; // Vec<LilyAstDataType*>*
        Vec *path;   // Vec<LilyAstExpr*>*
        LilyAstExpr *property_init;
        LilyAstExpr *self;
    };
} LilyAstAccess;

/**
 *
 * @brief Construct LilyAstAccess (LILY_AST_ACCESS_KIND_GLOBAL).
 */
VARIANT_CONSTRUCTOR(LilyAstAccess *,
                    LilyAstAccess,
                    global,
                    LilyAstExpr *global);

/**
 *
 * @brief Construct LilyAstAccess (LILY_AST_ACCESS_KIND_HOOK).
 */
VARIANT_CONSTRUCTOR(LilyAstAccess *,
                    LilyAstAccess,
                    hook,
                    LilyAstAccessHook hook);

/**
 *
 * @brief Construct LilyAstAccess (LILY_AST_ACCESS_KIND_OBJECT).
 */
VARIANT_CONSTRUCTOR(LilyAstAccess *, LilyAstAccess, object, Vec *object);

/**
 *
 * @brief Construct LilyAstAccess (LILY_AST_ACCESS_KIND_PATH).
 */
VARIANT_CONSTRUCTOR(LilyAstAccess *, LilyAstAccess, path, Vec *path);

/**
 *
 * @brief Construct LilyAstAccess (LILY_AST_ACCESS_KIND_PROPERTY_INIT).
 */
VARIANT_CONSTRUCTOR(LilyAstAccess *,
                    LilyAstAccess,
                    property_init,
                    LilyAstExpr *property_init);

/**
 *
 * @brief Construct LilyAstAccess (LILY_AST_ACCESS_KIND_SELF).
 */
VARIANT_CONSTRUCTOR(LilyAstAccess *, LilyAstAccess, self, LilyAstExpr *self_);

/**
 *
 * @brief Convert LilyAstAccessKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, LilyAstAccessKind, enum LilyAstAccessKind self);
#endif

/**
 *
 * @brief Convert LilyAstAccess in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyAstAccess, const LilyAstAccess *self);
#endif

/**
 *
 * @brief Convert LilyAstAccess in String.
 */
String *
to_string__LilyAstAccess(const LilyAstAccess *self);

/**
 *
 * @brief Free LilyAstAccess type.
 */
DESTRUCTOR(LilyAstAccess, LilyAstAccess *self);

#endif // LILY_CORE_LILY_AST_EXPR_ACCESS_H