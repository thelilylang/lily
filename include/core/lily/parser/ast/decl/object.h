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

#ifndef LILY_CORE_LILY_PARSER_AST_DECL_OBJECT_H
#define LILY_CORE_LILY_PARSER_AST_DECL_OBJECT_H

#include <base/macros.h>

#include <core/lily/parser/ast/decl/class.h>
#include <core/lily/parser/ast/decl/enum_object.h>
#include <core/lily/parser/ast/decl/record_object.h>
#include <core/lily/parser/ast/decl/trait.h>

enum LilyAstDeclObjectKind
{
    LILY_AST_DECL_OBJECT_KIND_CLASS,
    LILY_AST_DECL_OBJECT_KIND_ENUM,
    LILY_AST_DECL_OBJECT_KIND_RECORD,
    LILY_AST_DECL_OBJECT_KIND_TRAIT
};

/**
 *
 * @brief Convert LilyAstDeclObjectKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyAstDeclObjectKind,
               enum LilyAstDeclObjectKind self);
#endif

typedef struct LilyAstDeclObject
{
    enum LilyAstDeclObjectKind kind;
    union
    {
        LilyAstDeclClass class;
        LilyAstDeclEnumObject enum_;
        LilyAstDeclRecordObject record;
        LilyAstDeclTrait trait;
    };
} LilyAstDeclObject;

/**
 *
 * @brief Construct LilyAstDeclObject type (LILY_AST_DECL_OBJECT_KIND_CLASS).
 */
inline VARIANT_CONSTRUCTOR(LilyAstDeclObject,
                           LilyAstDeclObject,
                           class,
                           LilyAstDeclClass class)
{
    return (LilyAstDeclObject){ .kind = LILY_AST_DECL_OBJECT_KIND_CLASS,
                                .class = class };
}

/**
 *
 * @brief Construct LilyAstDeclObject type (LILY_AST_DECL_OBJECT_KIND_ENUM).
 */
inline VARIANT_CONSTRUCTOR(LilyAstDeclObject,
                           LilyAstDeclObject,
                           enum,
                           LilyAstDeclEnumObject enum_)
{
    return (LilyAstDeclObject){ .kind = LILY_AST_DECL_OBJECT_KIND_ENUM,
                                .enum_ = enum_ };
}

/**
 *
 * @brief Construct LilyAstDeclObject type (LILY_AST_DECL_OBJECT_KIND_RECORD).
 */
inline VARIANT_CONSTRUCTOR(LilyAstDeclObject,
                           LilyAstDeclObject,
                           record,
                           LilyAstDeclRecordObject record)
{
    return (LilyAstDeclObject){ .kind = LILY_AST_DECL_OBJECT_KIND_RECORD,
                                .record = record };
}

/**
 *
 * @brief Construct LilyAstDeclObject type (LILY_AST_DECL_OBJECT_KIND_TRAIT).
 */
inline VARIANT_CONSTRUCTOR(LilyAstDeclObject,
                           LilyAstDeclObject,
                           trait,
                           LilyAstDeclTrait trait)
{
    return (LilyAstDeclObject){ .kind = LILY_AST_DECL_OBJECT_KIND_TRAIT,
                                .trait = trait };
}

/**
 *
 * @brief Convert LilyAstDeclObject in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyAstDeclObject, const LilyAstDeclObject *self);
#endif

/**
 *
 * @brief Free LilyAstDeclObject type.
 */
DESTRUCTOR(LilyAstDeclObject, const LilyAstDeclObject *self);

#endif // LILY_CORE_LILY_PARSER_AST_DECL_OBJECT_H
