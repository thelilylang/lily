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

#ifndef LILY_CORE_LILY_CHECKED_EXPR_ACCESS_H
#define LILY_CORE_LILY_CHECKED_EXPR_ACCESS_H

#include <base/macros.h>
#include <base/string.h>
#include <base/vec.h>

#include <core/lily/checked/scope.h>

typedef struct LilyCheckedExpr LilyCheckedExpr;

enum LilyCheckedExprAccessKind
{
    LILY_CHECKED_EXPR_ACCESS_KIND_GLOBAL,
    LILY_CHECKED_EXPR_ACCESS_KIND_HOOK,
    LILY_CHECKED_EXPR_ACCESS_KIND_OBJECT,
    LILY_CHECKED_EXPR_ACCESS_KIND_PATH,
    LILY_CHECKED_EXPR_ACCESS_KIND_PROPERTY_INIT,
    LILY_CHECKED_EXPR_ACCESS_KIND_SELF,
};

typedef struct LilyCheckedExprAccessHook
{
    LilyCheckedExpr *access;
    LilyCheckedExpr *expr;
} LilyCheckedExprAccessHook;

/**
 *
 * @brief Construct LilyCheckedExprAccessHook type.
 */
inline CONSTRUCTOR(LilyCheckedExprAccessHook,
                   LilyCheckedExprAccessHook,
                   LilyCheckedExpr *access,
                   LilyCheckedExpr *expr)
{
    return (LilyCheckedExprAccessHook){ .access = access, .expr = expr };
}

/**
 *
 * @brief Convert LilyCheckedExprAccessHook in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprAccessHook,
               const LilyCheckedExprAccessHook *self);
#endif

// <access><object>
typedef struct LilyCheckedExprAccessObject
{
    LilyCheckedExpr *access; // LilyCheckedExpr*?
                             // e.g. A.B.C
    Vec *object;             // Vec<LilyCheckedDataType*>*
                             // e.g. @Object@Object2
} LilyCheckedExprAccessObject;

/**
 *
 * @brief Construct LilyCheckedExprAccessObject type.
 */
inline CONSTRUCTOR(LilyCheckedExprAccessObject,
                   LilyCheckedExprAccessObject,
                   LilyCheckedExpr *access,
                   Vec *object)
{
    return (LilyCheckedExprAccessObject){ .access = access, .object = object };
}

/**
 *
 * @brief Convert LilyCheckedExprAccessObject in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprAccessObject,
               const LilyCheckedExprAccessObject *self);
#endif

typedef struct LilyCheckedExprAccess
{
    enum LilyCheckedExprAccessKind kind;
    LilyCheckedScope scope;
    union
    {
        LilyCheckedExpr *global;
        LilyCheckedExprAccessHook hook;
        LilyCheckedExprAccessObject object;
        Vec *path; // Vec<LilyCheckedExpr*>*
        LilyCheckedExpr *property_init;
        LilyCheckedExpr *self;
    };
} LilyCheckedExprAccess;

/**
 *
 * @brief Construct LilyCheckedExprAccess type
 * (LILY_CHECKED_EXPR_ACCESS_KIND_GLOBAL).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedExprAccess,
                           LilyCheckedExprAccess,
                           global,
                           LilyCheckedScope scope,
                           LilyCheckedExpr *global)
{
    return (LilyCheckedExprAccess){ .kind =
                                      LILY_CHECKED_EXPR_ACCESS_KIND_GLOBAL,
                                    .scope = scope,
                                    .global = global };
}

/**
 *
 * @brief Construct LilyCheckedExprAccess type
 * (LILY_CHECKED_EXPR_ACCESS_KIND_HOOK).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedExprAccess,
                           LilyCheckedExprAccess,
                           hook,
                           LilyCheckedScope scope,
                           LilyCheckedExprAccessHook hook)
{
    return (LilyCheckedExprAccess){ .kind = LILY_CHECKED_EXPR_ACCESS_KIND_HOOK,
                                    .scope = scope,
                                    .hook = hook };
}

/**
 *
 * @brief Construct LilyCheckedExprAccess type
 * (LILY_CHECKED_EXPR_ACCESS_KIND_OBJECT).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedExprAccess,
                           LilyCheckedExprAccess,
                           object,
                           LilyCheckedScope scope,
                           LilyCheckedExprAccessObject object)
{
    return (LilyCheckedExprAccess){ .kind =
                                      LILY_CHECKED_EXPR_ACCESS_KIND_OBJECT,
                                    .scope = scope,
                                    .object = object };
}

/**
 *
 * @brief Construct LilyCheckedExprAccess type
 * (LILY_CHECKED_EXPR_ACCESS_KIND_PATH).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedExprAccess,
                           LilyCheckedExprAccess,
                           path,
                           LilyCheckedScope scope,
                           Vec *path)
{
    return (LilyCheckedExprAccess){ .kind = LILY_CHECKED_EXPR_ACCESS_KIND_PATH,
                                    .scope = scope,
                                    .path = path };
}

/**
 *
 * @brief Construct LilyCheckedExprAccess type
 * (LILY_CHECKED_EXPR_ACCESS_KIND_PROPERTY_INIT).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedExprAccess,
                           LilyCheckedExprAccess,
                           property_init,
                           LilyCheckedScope scope,
                           LilyCheckedExpr *property_init)
{
    return (LilyCheckedExprAccess){
        .kind = LILY_CHECKED_EXPR_ACCESS_KIND_PROPERTY_INIT,
        .scope = scope,
        .property_init = property_init
    };
}

/**
 *
 * @brief Construct LilyCheckedExprAccess type (LILY_AST_EXPR_ACCESS_KIND_SELF).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedExprAccess,
                           LilyCheckedExprAccess,
                           self,
                           LilyCheckedScope scope,
                           LilyCheckedExpr *self)
{
    return (LilyCheckedExprAccess){ .kind = LILY_CHECKED_EXPR_ACCESS_KIND_SELF,
                                    .scope = scope,
                                    .self = self };
}

/**
 *
 * @brief Convert LilyCheckedExprAccessKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprAccessKind,
               enum LilyCheckedExprAccessKind self);
#endif

/**
 *
 * @brief Convert LilyCheckedExprAccess in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprAccess,
               const LilyCheckedExprAccess *self);
#endif

/**
 *
 * @brief Convert LilyCheckedExprAccess in String.
 */
String *
to_string__LilyCheckedExprAccess(const LilyCheckedExprAccess *self);

/**
 *
 * @brief Free LilyCheckedExprAccess type.
 */
DESTRUCTOR(LilyCheckedExprAccess, const LilyCheckedExprAccess *self);

#endif // LILY_CORE_LILY_CHECKED_EXPR_ACCESS_H
