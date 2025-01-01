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

#ifndef LILY_CORE_LILY_ANALYSIS_CHECKED_BODY_FUN_H
#define LILY_CORE_LILY_ANALYSIS_CHECKED_BODY_FUN_H

#include <core/lily/analysis/checked/expr.h>
#include <core/lily/analysis/checked/stmt.h>

enum LilyCheckedBodyFunItemKind
{
    LILY_CHECKED_BODY_FUN_ITEM_KIND_EXPR,
    LILY_CHECKED_BODY_FUN_ITEM_KIND_STMT
};

/**
 *
 * @brief Convert LilyCheckedBodyFunItemKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedBodyFunItemKind,
               enum LilyCheckedBodyFunItemKind self);
#endif

typedef struct LilyCheckedBodyFunItem
{
    enum LilyCheckedBodyFunItemKind kind;
    Usize ref_count;
    union
    {
        LilyCheckedStmt stmt;
        LilyCheckedExpr *expr;
    };
} LilyCheckedBodyFunItem;

/**
 *
 * @brief Construct LilyCheckedBodyFunItem type
 * (LILY_CHECKED_BODY_FUN_ITEM_KIND_EXPR).
 */
VARIANT_CONSTRUCTOR(LilyCheckedBodyFunItem *,
                    LilyCheckedBodyFunItem,
                    expr,
                    LilyCheckedExpr *expr);

/**
 *
 * @brief Construct LilyCheckedBodyFunItem type
 * (LILY_CHECKED_BODY_FUN_ITEM_KIND_STMT).
 */
VARIANT_CONSTRUCTOR(LilyCheckedBodyFunItem *,
                    LilyCheckedBodyFunItem,
                    stmt,
                    LilyCheckedStmt stmt);

/**
 *
 * @brief Pass to ref a data type and increment the `ref_count`.
 * @return LilyCheckedBodyFunItem* (&)
 */
inline LilyCheckedBodyFunItem *
ref__LilyCheckedBodyFunItem(LilyCheckedBodyFunItem *self)
{
    ++self->ref_count;
    return self;
}

typedef struct LilyCheckedBodyFunInfo
{
    Vec *body; // Vec<LilyCheckedBodyFunItem*>*
    LilyCheckedScope *scope;
} LilyCheckedBodyFunInfo;

/**
 *
 * @brief Construct LilyCheckedBodyFunInfo type.
 */
inline CONSTRUCTOR(LilyCheckedBodyFunInfo,
                   LilyCheckedBodyFunInfo,
                   Vec *body,
                   LilyCheckedScope *scope)
{
    return (LilyCheckedBodyFunInfo){ .body = body, .scope = scope };
}

/**
 *
 * @brief Wrap item in body (Vec).
 * @note In some cases this function free somethings.
 */
LilyCheckedBodyFunInfo
wrap_item_in_body__LilyCheckedBodyFunItem(LilyCheckedBodyFunItem *self,
                                          LilyCheckedScope *parent);

/**
 *
 * @brief Convert LilyCheckedBodyFunItem in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedBodyFunItem,
               const LilyCheckedBodyFunItem *self);
#endif

/**
 *
 * @brief Free LilyCheckedBodyFunItem type.
 */
DESTRUCTOR(LilyCheckedBodyFunItem, LilyCheckedBodyFunItem *self);

#endif // LILY_CORE_LILY_ANALYSIS_CHECKED_BODY_FUN_H
