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

#ifndef LILY_CORE_LILY_PARSER_AST_BODY_TRAIT_H
#define LILY_CORE_LILY_PARSER_AST_BODY_TRAIT_H

#include <base/string.h>

#include <core/lily/parser/ast/decl/attribute.h>
#include <core/lily/parser/ast/decl/prototype.h>

enum LilyAstBodyTraitItemKind
{
    LILY_AST_BODY_TRAIT_ITEM_KIND_ATTRIBUTE,
    LILY_AST_BODY_TRAIT_ITEM_KIND_PROTOTYPE,
};

/**
 *
 * @brief Convert LilyAstBodyTraitItemKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyAstBodyTraitItemKind,
               enum LilyAstBodyTraitItemKind self);
#endif

typedef struct LilyAstBodyTraitItem
{
    enum LilyAstBodyTraitItemKind kind;
    Location location;
    union
    {
        LilyAstDeclAttribute attribute;
        LilyAstDeclPrototype prototype;
    };
} LilyAstBodyTraitItem;

/**
 *
 * @brief Construct LilyAstBodyTraitItem type
 * (LILY_AST_BODY_TRAIT_ITEM_KIND_ATTRIBUTE).
 */
VARIANT_CONSTRUCTOR(LilyAstBodyTraitItem *,
                    LilyAstBodyTraitItem,
                    attribute,
                    Location location,
                    LilyAstDeclAttribute attribute);

/**
 *
 * @brief Construct LilyAstBodyTraitItem type
 * (LILY_AST_BODY_TRAIT_ITEM_KIND_PROTOTYPE).
 */
VARIANT_CONSTRUCTOR(LilyAstBodyTraitItem *,
                    LilyAstBodyTraitItem,
                    prototype,
                    Location location,
                    LilyAstDeclPrototype prototype);

/**
 *
 * @brief Convert LilyAstBodyTraitItem in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyAstBodyTraitItem,
               const LilyAstBodyTraitItem *self);
#endif

/**
 *
 * @brief Free LilyAstBodyTraitItem type.
 */
DESTRUCTOR(LilyAstBodyTraitItem, LilyAstBodyTraitItem *self);

#endif // LILY_CORE_LILY_PARSER_AST_BODY_TRAIT_H
