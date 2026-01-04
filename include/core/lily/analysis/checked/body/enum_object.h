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

#ifndef LILY_CORE_LILY_ANALYSIS_CHECKED_BODY_ENUM_OBJECT_H
#define LILY_CORE_LILY_ANALYSIS_CHECKED_BODY_ENUM_OBJECT_H

#include <base/macros.h>

#include <core/lily/analysis/checked/decl/constant.h>
#include <core/lily/analysis/checked/decl/method.h>
#include <core/lily/analysis/checked/variant.h>
#include <core/shared/location.h>

#ifdef ENV_DEBUG
#include <base/string.h>
#endif

enum LilyCheckedBodyEnumObjectItemKind
{
    LILY_CHECKED_BODY_ENUM_OBJECT_ITEM_KIND_CONSTANT,
    LILY_CHECKED_BODY_ENUM_OBJECT_ITEM_KIND_METHOD,
    LILY_CHECKED_BODY_ENUM_OBJECT_ITEM_KIND_VARIANT
};

/**
 *
 * @brief Convert LilyCheckedBodyEnumObjectItemKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedBodyEnumObjectItemKind,
               enum LilyCheckedBodyEnumObjectItemKind self);
#endif

typedef struct LilyCheckedBodyEnumObjectItem
{
    enum LilyCheckedBodyEnumObjectItemKind kind;
    const Location *location;
    union
    {
        LilyCheckedDeclConstant constant;
        LilyCheckedDeclMethod method;
        LilyCheckedVariant *variant;
    };
} LilyCheckedBodyEnumObjectItem;

/**
 *
 * @brief Construct LilyCheckedBodyEnumObjectItem type
 * (LILY_CHECKED_BODY_ENUM_OBJECT_ITEM_KIND_CONSTANT).
 */
VARIANT_CONSTRUCTOR(LilyCheckedBodyEnumObjectItem *,
                    LilyCheckedBodyEnumObjectItem,
                    constant,
                    const Location *location,
                    LilyCheckedDeclConstant constant);

/**
 *
 * @brief Construct LilyCheckedBodyEnumObjectItem type
 * (LILY_CHECKED_BODY_ENUM_OBJECT_ITEM_KIND_METHOD).
 */
VARIANT_CONSTRUCTOR(LilyCheckedBodyEnumObjectItem *,
                    LilyCheckedBodyEnumObjectItem,
                    method,
                    const Location *location,
                    LilyCheckedDeclMethod method);

/**
 *
 * @brief Construct LilyCheckedBodyEnumObjectItem type
 * (LILY_CHECKED_BODY_ENUM_OBJECT_ITEM_KIND_VARIANT).
 */
VARIANT_CONSTRUCTOR(LilyCheckedBodyEnumObjectItem *,
                    LilyCheckedBodyEnumObjectItem,
                    variant,
                    const Location *location,
                    LilyCheckedVariant *variant);

/**
 *
 * @brief Convert LilyCheckedBodyEnumObjectItem in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedBodyEnumObjectItem,
               const LilyCheckedBodyEnumObjectItem *self);
#endif

/**
 *
 * @brief Free LilyCheckedBodyEnumObjectItem type.
 */
DESTRUCTOR(LilyCheckedBodyEnumObjectItem, LilyCheckedBodyEnumObjectItem *self);

#endif // LILY_CORE_LILY_ANALYSIS_CHECKED_BODY_ENUM_OBJECT_H
