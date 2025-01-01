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

#ifndef LILY_CORE_LILY_ANALYSIS_CHECKED_DECL_TYPE_H
#define LILY_CORE_LILY_ANALYSIS_CHECKED_DECL_TYPE_H

#include <core/lily/analysis/checked/decl/alias.h>
#include <core/lily/analysis/checked/decl/enum.h>
#include <core/lily/analysis/checked/decl/record.h>

enum LilyCheckedDeclTypeKind
{
    LILY_CHECKED_DECL_TYPE_KIND_ALIAS,
    LILY_CHECKED_DECL_TYPE_KIND_ENUM,
    LILY_CHECKED_DECL_TYPE_KIND_RECORD
};

/**
 *
 * @brief Convert LilyCheckedDeclTypeKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedDeclTypeKind,
               enum LilyCheckedDeclTypeKind self);
#endif

typedef struct LilyCheckedDeclType
{
    enum LilyCheckedDeclTypeKind kind;
    // Type dependencies
    // TODO: maybe make deps NULL for alias
    Vec *deps; // Vec<LilyCheckedDecl* (&)>*
    union
    {
        LilyCheckedDeclAlias alias;
        LilyCheckedDeclEnum enum_;
        LilyCheckedDeclRecord record;
    };
} LilyCheckedDeclType;

/**
 *
 * @brief Construct LilyCheckedDeclType type
 * (LILY_CHECKED_DECL_TYPE_KIND_ALIAS).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedDeclType,
                           LilyCheckedDeclType,
                           alias,
                           LilyCheckedDeclAlias alias)
{
    return (LilyCheckedDeclType){ .kind = LILY_CHECKED_DECL_TYPE_KIND_ALIAS,
                                  .deps = NEW(Vec),
                                  .alias = alias };
}

/**
 *
 * @brief Construct LilyCheckedDeclType type (LILY_CHECKED_DECL_TYPE_KIND_ENUM).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedDeclType,
                           LilyCheckedDeclType,
                           enum,
                           LilyCheckedDeclEnum enum_)
{
    return (LilyCheckedDeclType){ .kind = LILY_CHECKED_DECL_TYPE_KIND_ENUM,
                                  .deps = NEW(Vec),
                                  .enum_ = enum_ };
}

/**
 *
 * @brief Construct LilyCheckedDeclType type
 * (LILY_CHECKED_DECL_TYPE_KIND_RECORD).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedDeclType,
                           LilyCheckedDeclType,
                           record,
                           LilyCheckedDeclRecord record)
{
    return (LilyCheckedDeclType){ .kind = LILY_CHECKED_DECL_TYPE_KIND_RECORD,
                                  .deps = NEW(Vec),
                                  .record = record };
}

/**
 *
 * @brief Convert LilyCheckedDeclType in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyCheckedDeclType, const LilyCheckedDeclType *self);
#endif

/**
 *
 * @brief Free LilyCheckedDeclType type.
 */
DESTRUCTOR(LilyCheckedDeclType, const LilyCheckedDeclType *self);

#endif // LILY_CORE_LILY_ANALYSIS_CHECKED_DECL_TYPE_H
