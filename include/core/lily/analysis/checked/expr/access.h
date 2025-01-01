/*
 * MIT License
 *
 * Copyright (c) 2022-2025 ArthurPV
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

#ifndef LILY_CORE_LILY_ANALYSIS_CHECKED_EXPR_ACCESS_H
#define LILY_CORE_LILY_ANALYSIS_CHECKED_EXPR_ACCESS_H

#include <base/macros.h>

typedef struct LilyCheckedExpr LilyCheckedExpr;

enum LilyCheckedExprAccessKind
{
    LILY_CHECKED_EXPR_ACCESS_KIND_HOOK,
    LILY_CHECKED_EXPR_ACCESS_KIND_TUPLE,
};

/**
 *
 * @brief Convert LilyCheckedExprAccessKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprAccessKind,
               enum LilyCheckedExprAccessKind kind);
#endif

// left[index]
typedef struct LilyCheckedExprAccessHook
{
    LilyCheckedExpr *left;
    LilyCheckedExpr *index;
} LilyCheckedExprAccessHook;

/**
 *
 * @brief Construct LilyCheckedExprAccessHook type.
 */
inline CONSTRUCTOR(LilyCheckedExprAccessHook,
                   LilyCheckedExprAccessHook,
                   LilyCheckedExpr *left,
                   LilyCheckedExpr *index)
{
    return (LilyCheckedExprAccessHook){ .left = left, .index = index };
}

/**
 *
 * @brief Check if the both accceses hook are equal.
 */
bool
eq__LilyCheckedExprAccessHook(const LilyCheckedExprAccessHook *self,
                              const LilyCheckedExprAccessHook *other);

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

/**
 *
 * @brief Free LilyCheckedExprAccessHook type.
 */
DESTRUCTOR(LilyCheckedExprAccessHook, const LilyCheckedExprAccessHook *self);

// left#n
typedef struct LilyCheckedExprAccessTuple
{
    LilyCheckedExpr *left;
    LilyCheckedExpr *n; // expected a literal
} LilyCheckedExprAccessTuple;

/**
 *
 * @brief Construct LilyCheckedExprAccessTuple type.
 */
inline CONSTRUCTOR(LilyCheckedExprAccessTuple,
                   LilyCheckedExprAccessTuple,
                   LilyCheckedExpr *left,
                   LilyCheckedExpr *n)
{
    return (LilyCheckedExprAccessTuple){ .left = left, .n = n };
}

/**
 *
 * @brief Check if the both accceses tuple are equal.
 */
bool
eq__LilyCheckedExprAccessTuple(const LilyCheckedExprAccessTuple *self,
                               const LilyCheckedExprAccessTuple *other);

/**
 *
 * @brief Convert LilyCheckedExprAccessTuple in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprAccessTuple,
               const LilyCheckedExprAccessTuple *self);
#endif

/**
 *
 * @brief Free LilyCheckedExprAccessTuple type.
 */
DESTRUCTOR(LilyCheckedExprAccessTuple, const LilyCheckedExprAccessTuple *self);

typedef struct LilyCheckedExprAccess
{
    enum LilyCheckedExprAccessKind kind;
    union
    {
        LilyCheckedExprAccessHook hook;
        LilyCheckedExprAccessTuple tuple;
    };
} LilyCheckedExprAccess;

/**
 *
 * @brief Construct LilyCheckedExprAccess type
 * (LILY_CHECKED_EXPR_ACCESS_KIND_HOOK).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedExprAccess,
                           LilyCheckedExprAccess,
                           hook,
                           LilyCheckedExprAccessHook hook)
{
    return (LilyCheckedExprAccess){ .kind = LILY_CHECKED_EXPR_ACCESS_KIND_HOOK,
                                    .hook = hook };
}

/**
 *
 * @brief Construct LilyCheckedExprAccess type
 * (LILY_CHECKED_EXPR_ACCESS_KIND_TUPLE).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedExprAccess,
                           LilyCheckedExprAccess,
                           tuple,
                           LilyCheckedExprAccessTuple tuple)
{
    return (LilyCheckedExprAccess){ .kind = LILY_CHECKED_EXPR_ACCESS_KIND_TUPLE,
                                    .tuple = tuple };
}

/**
 *
 * @brief Check if the both accesses are equal.
 */
bool
eq__LilyCheckedExprAccess(const LilyCheckedExprAccess *self,
                          const LilyCheckedExprAccess *other);

/**
 *
 * @brief Convert LilyCheckedExprAccess in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprAccess,
               const LilyCheckedExprAccess *self);
#endif

/**
 *
 * @brief Free LilyCheckedExprAccess type.
 */
DESTRUCTOR(LilyCheckedExprAccess, const LilyCheckedExprAccess *self);

#endif // LILY_CORE_LILY_ANALYSIS_CHECKED_EXPR_ACCESS_H
