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

#ifndef LILY_CORE_LILY_PARSER_AST_DECL_TYPE_H
#define LILY_CORE_LILY_PARSER_AST_DECL_TYPE_H

#include <core/lily/parser/ast/decl/alias.h>
#include <core/lily/parser/ast/decl/enum.h>
#include <core/lily/parser/ast/decl/record.h>

enum LilyAstDeclTypeKind
{
    LILY_AST_DECL_TYPE_KIND_ALIAS,
    LILY_AST_DECL_TYPE_KIND_ENUM,
    LILY_AST_DECL_TYPE_KIND_RECORD
};

/**
 *
 * @brief Convert LilyAstDeclTypeKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, LilyAstDeclTypeKind, enum LilyAstDeclTypeKind self);
#endif

typedef struct LilyAstDeclType
{
    enum LilyAstDeclTypeKind kind;
    union
    {
        LilyAstDeclAlias alias;
        LilyAstDeclEnum enum_;
        LilyAstDeclRecord record;
    };
} LilyAstDeclType;

/**
 *
 * @brief Construct LilyAstDeclType type (LILY_AST_DECL_TYPE_KIND_ALIAS).
 */
inline VARIANT_CONSTRUCTOR(LilyAstDeclType,
                           LilyAstDeclType,
                           alias,
                           LilyAstDeclAlias alias)
{
    return (LilyAstDeclType){ .kind = LILY_AST_DECL_TYPE_KIND_ALIAS,
                              .alias = alias };
}

/**
 *
 * @brief Construct LilyAstDeclType type (LILY_AST_DECL_TYPE_KIND_ENUM).
 */
inline VARIANT_CONSTRUCTOR(LilyAstDeclType,
                           LilyAstDeclType,
                           enum,
                           LilyAstDeclEnum enum_)
{
    return (LilyAstDeclType){ .kind = LILY_AST_DECL_TYPE_KIND_ENUM,
                              .enum_ = enum_ };
}

/**
 *
 * @brief Construct LilyAstDeclType type (LILY_AST_DECL_TYPE_KIND_RECORD).
 */
inline VARIANT_CONSTRUCTOR(LilyAstDeclType,
                           LilyAstDeclType,
                           record,
                           LilyAstDeclRecord record)
{
    return (LilyAstDeclType){ .kind = LILY_AST_DECL_TYPE_KIND_RECORD,
                              .record = record };
}

/**
 *
 * @brief Convert LilyAstDeclType in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyAstDeclType, const LilyAstDeclType *self);
#endif

/**
 *
 * @brief Free LilyAstDeclType type.
 */
DESTRUCTOR(LilyAstDeclType, const LilyAstDeclType *self);

#endif // LILY_CORE_LILY_PARSER_AST_DECL_TYPE_H
