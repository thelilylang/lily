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

#ifndef LILY_CORE_LILY_ANALYSIS_CHECKED_BODY_TRAIT_H
#define LILY_CORE_LILY_ANALYSIS_CHECKED_BODY_TRAIT_H

#include <base/string.h>

#include <core/lily/analysis/checked/decl/attribute.h>
#include <core/lily/analysis/checked/decl/prototype.h>

enum LilyCheckedBodyTraitItemKind
{
    LILY_CHECKED_BODY_TRAIT_ITEM_KIND_ATTRIBUTE,
    LILY_CHECKED_BODY_TRAIT_ITEM_KIND_PROTOTYPE,
};

/**
 *
 * @brief Convert LilyCheckedBodyTraitItemKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedBodyTraitItemKind,
               enum LilyCheckedBodyTraitItemKind self);
#endif

typedef struct LilyCheckedBodyTraitItem
{
    enum LilyCheckedBodyTraitItemKind kind;
    Location location;
    union
    {
        LilyCheckedDeclAttribute attribute;
        LilyCheckedDeclPrototype prototype;
    };
} LilyCheckedBodyTraitItem;

/**
 *
 * @brief Construct LilyCheckedBodyTraitItem type
 * (LILY_CHECKED_BODY_TRAIT_ITEM_KIND_ATTRIBUTE).
 */
VARIANT_CONSTRUCTOR(LilyCheckedBodyTraitItem *,
                    LilyCheckedBodyTraitItem,
                    attribute,
                    Location location,
                    LilyCheckedDeclAttribute attribute);

/**
 *
 * @brief Construct LilyCheckedBodyTraitItem type
 * (LILY_CHECKED_BODY_TRAIT_ITEM_KIND_PROTOTYPE).
 */
VARIANT_CONSTRUCTOR(LilyCheckedBodyTraitItem *,
                    LilyCheckedBodyTraitItem,
                    prototype,
                    Location location,
                    LilyCheckedDeclPrototype prototype);

/**
 *
 * @brief Convert LilyCheckedBodyTraitItem in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedBodyTraitItem,
               const LilyCheckedBodyTraitItem *self);
#endif

/**
 *
 * @brief Free LilyCheckedBodyTraitItem type.
 */
DESTRUCTOR(LilyCheckedBodyTraitItem, LilyCheckedBodyTraitItem *self);

#endif // LILY_CORE_LILY_ANALYSIS_CHECKED_BODY_TRAIT_H
