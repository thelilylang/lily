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

#ifndef LILY_CORE_LILY_PARSER_AST_EXPR_ACCESS_H
#define LILY_CORE_LILY_PARSER_AST_EXPR_ACCESS_H

#include <base/macros.h>
#include <base/string.h>
#include <base/vec.h>

typedef struct LilyAstExpr LilyAstExpr;

enum LilyAstExprAccessKind
{
    LILY_AST_EXPR_ACCESS_KIND_GLOBAL_PATH,
    LILY_AST_EXPR_ACCESS_KIND_HOOK,
    LILY_AST_EXPR_ACCESS_KIND_OBJECT,
    LILY_AST_EXPR_ACCESS_KIND_PATH,
    LILY_AST_EXPR_ACCESS_KIND_PROPERTY_INIT,
    LILY_AST_EXPR_ACCESS_KIND_SELF_PATH,
    LILY_AST_EXPR_ACCESS_KIND_self_PATH,
};

typedef struct LilyAstExprAccessHook
{
    LilyAstExpr *access;
    LilyAstExpr *expr;
} LilyAstExprAccessHook;

/**
 *
 * @brief Construct LilyAstExprAccessHook type.
 */
inline CONSTRUCTOR(LilyAstExprAccessHook,
                   LilyAstExprAccessHook,
                   LilyAstExpr *access,
                   LilyAstExpr *expr)
{
    return (LilyAstExprAccessHook){ .access = access, .expr = expr };
}

/**
 *
 * @brief Convert LilyAstExprAccessHook in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyAstExprAccessHook,
               const LilyAstExprAccessHook *self);
#endif

// <access><object>
typedef struct LilyAstExprAccessObject
{
    LilyAstExpr *access; // LilyAstExpr*?
                         // e.g. A.B.C
    Vec *object;         // Vec<LilyAstDataType*>*
                         // e.g. @Object@Object2
} LilyAstExprAccessObject;

/**
 *
 * @brief Construct LilyAstExprAccessObject type.
 */
inline CONSTRUCTOR(LilyAstExprAccessObject,
                   LilyAstExprAccessObject,
                   LilyAstExpr *access,
                   Vec *object)
{
    return (LilyAstExprAccessObject){ .access = access, .object = object };
}

/**
 *
 * @brief Convert LilyAstExprAccessObject in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyAstExprAccessObject,
               const LilyAstExprAccessObject *self);
#endif

typedef struct LilyAstExprAccess
{
    enum LilyAstExprAccessKind kind;
    union
    {
        LilyAstExprAccessHook global_hook;
        Vec *global_path; // Vec<LilyAstExpr*>*
        LilyAstExprAccessHook hook;
        LilyAstExprAccessObject object;
        Vec *path;          // Vec<LilyAstExpr*>*
        Vec *property_init; // Vec<LilyAstExpr*>*
        LilyAstExprAccessHook self_hook;
        Vec *self_path; // Vec<LilyAstExpr*>*
        LilyAstExprAccessHook Self_hook;
        Vec *Self_path; // Vec<LilyAstExpr*>*
    };
} LilyAstExprAccess;

/**
 *
 * @brief Construct LilyAstExprAccess type
 * (LILY_AST_EXPR_ACCESS_KIND_GLOBAL_PATH).
 */
inline VARIANT_CONSTRUCTOR(LilyAstExprAccess,
                           LilyAstExprAccess,
                           global_path,
                           Vec *global_path)
{
    return (LilyAstExprAccess){ .kind = LILY_AST_EXPR_ACCESS_KIND_GLOBAL_PATH,
                                .global_path = global_path };
}

/**
 *
 * @brief Construct LilyAstExprAccess type (LILY_AST_EXPR_ACCESS_KIND_HOOK).
 */
inline VARIANT_CONSTRUCTOR(LilyAstExprAccess,
                           LilyAstExprAccess,
                           hook,
                           LilyAstExprAccessHook hook)
{
    return (LilyAstExprAccess){ .kind = LILY_AST_EXPR_ACCESS_KIND_HOOK,
                                .hook = hook };
}

/**
 *
 * @brief Construct LilyAstExprAccess type (LILY_AST_EXPR_ACCESS_KIND_OBJECT).
 */
inline VARIANT_CONSTRUCTOR(LilyAstExprAccess,
                           LilyAstExprAccess,
                           object,
                           LilyAstExprAccessObject object)
{
    return (LilyAstExprAccess){ .kind = LILY_AST_EXPR_ACCESS_KIND_OBJECT,
                                .object = object };
}

/**
 *
 * @brief Construct LilyAstExprAccess type (LILY_AST_EXPR_ACCESS_KIND_PATH).
 */
inline VARIANT_CONSTRUCTOR(LilyAstExprAccess,
                           LilyAstExprAccess,
                           path,
                           Vec *path)
{
    return (LilyAstExprAccess){ .kind = LILY_AST_EXPR_ACCESS_KIND_PATH,
                                .path = path };
}

/**
 *
 * @brief Construct LilyAstExprAccess type
 * (LILY_AST_EXPR_ACCESS_KIND_PROPERTY_INIT).
 */
inline VARIANT_CONSTRUCTOR(LilyAstExprAccess,
                           LilyAstExprAccess,
                           property_init,
                           Vec *property_init)
{
    return (LilyAstExprAccess){ .kind = LILY_AST_EXPR_ACCESS_KIND_PROPERTY_INIT,
                                .property_init = property_init };
}

/**
 *
 * @brief Construct LilyAstExprAccess type
 * (LILY_AST_EXPR_ACCESS_KIND_SELF_PATH).
 */
inline VARIANT_CONSTRUCTOR(LilyAstExprAccess,
                           LilyAstExprAccess,
                           Self_path,
                           Vec *Self_path)
{
    return (LilyAstExprAccess){ .kind = LILY_AST_EXPR_ACCESS_KIND_SELF_PATH,
                                .Self_path = Self_path };
}

/**
 *
 * @brief Construct LilyAstExprAccess type
 * (LILY_AST_EXPR_ACCESS_KIND_self_PATH).
 */
inline VARIANT_CONSTRUCTOR(LilyAstExprAccess,
                           LilyAstExprAccess,
                           self_path,
                           Vec *self_path)
{
    return (LilyAstExprAccess){ .kind = LILY_AST_EXPR_ACCESS_KIND_self_PATH,
                                .self_path = self_path };
}

/**
 *
 * @brief Convert LilyAstExprAccessKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyAstExprAccessKind,
               enum LilyAstExprAccessKind self);
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
 * @brief Get path from access expression.
 */
Vec *
get_path__LilyAstExprAccess(const LilyAstExprAccess *self);

/**
 *
 * @brief Add item to path access.
 * @return Return 1 if is failing to add an item to the path otherwise return 0.
 */
int
add_item_to_path__LilyAstExprAccess(LilyAstExpr **self, LilyAstExpr **item);

/**
 *
 * @brief Free LilyAstExprAccess type.
 */
DESTRUCTOR(LilyAstExprAccess, const LilyAstExprAccess *self);

#endif // LILY_CORE_LILY_PARSER_AST_EXPR_ACCESS_H