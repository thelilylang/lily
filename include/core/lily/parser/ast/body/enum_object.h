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

#ifndef LILY_CORE_LILY_PARSER_AST_BODY_ENUM_OBJECT_H
#define LILY_CORE_LILY_PARSER_AST_BODY_ENUM_OBJECT_H

#include <base/macros.h>

#include <core/lily/parser/ast/decl/constant.h>
#include <core/lily/parser/ast/decl/method.h>
#include <core/lily/parser/ast/variant.h>
#include <core/shared/location.h>

#ifdef ENV_DEBUG
#include <base/string.h>
#endif

enum LilyAstBodyEnumObjectItemKind
{
    LILY_AST_BODY_ENUM_OBJECT_ITEM_KIND_CONSTANT,
    LILY_AST_BODY_ENUM_OBJECT_ITEM_KIND_METHOD,
    LILY_AST_BODY_ENUM_OBJECT_ITEM_KIND_VARIANT
};

/**
 *
 * @brief Convert LilyAstBodyEnumObjectItemKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyAstBodyEnumObjectItemKind,
               enum LilyAstBodyEnumObjectItemKind self);
#endif

typedef struct LilyAstBodyEnumObjectItem
{
    enum LilyAstBodyEnumObjectItemKind kind;
    Location location;
    union
    {
        LilyAstDeclConstant constant;
        LilyAstDeclMethod method;
        LilyAstVariant *variant;
    };
} LilyAstBodyEnumObjectItem;

/**
 *
 * @brief Construct LilyAstBodyEnumObjectItem type
 * (LILY_AST_BODY_ENUM_OBJECT_ITEM_KIND_CONSTANT).
 */
VARIANT_CONSTRUCTOR(LilyAstBodyEnumObjectItem *,
                    LilyAstBodyEnumObjectItem,
                    constant,
                    Location location,
                    LilyAstDeclConstant constant);

/**
 *
 * @brief Construct LilyAstBodyEnumObjectItem type
 * (LILY_AST_BODY_ENUM_OBJECT_ITEM_KIND_METHOD).
 */
VARIANT_CONSTRUCTOR(LilyAstBodyEnumObjectItem *,
                    LilyAstBodyEnumObjectItem,
                    method,
                    Location location,
                    LilyAstDeclMethod method);

/**
 *
 * @brief Construct LilyAstBodyEnumObjectItem type
 * (LILY_AST_BODY_ENUM_OBJECT_ITEM_KIND_VARIANT).
 */
VARIANT_CONSTRUCTOR(LilyAstBodyEnumObjectItem *,
                    LilyAstBodyEnumObjectItem,
                    variant,
                    Location location,
                    LilyAstVariant *variant);

/**
 *
 * @brief Convert LilyAstBodyEnumObjectItem in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyAstBodyEnumObjectItem,
               const LilyAstBodyEnumObjectItem *self);
#endif

/**
 *
 * @brief Free LilyAstBodyEnumObjectItem type.
 */
DESTRUCTOR(LilyAstBodyEnumObjectItem, LilyAstBodyEnumObjectItem *self);

#endif // LILY_CORE_LILY_PARSER_AST_BODY_ENUM_OBJECT_H
