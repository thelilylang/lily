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

#ifndef LILY_CORE_LILY_ANALYSIS_CHECKED_DECL_OBJECT_H
#define LILY_CORE_LILY_ANALYSIS_CHECKED_DECL_OBJECT_H

#include <base/macros.h>

#include <core/lily/analysis/checked/decl/class.h>
#include <core/lily/analysis/checked/decl/enum_object.h>
#include <core/lily/analysis/checked/decl/record_object.h>
#include <core/lily/analysis/checked/decl/trait.h>

enum LilyCheckedDeclObjectKind
{
    LILY_CHECKED_DECL_OBJECT_KIND_CLASS,
    LILY_CHECKED_DECL_OBJECT_KIND_ENUM,
    LILY_CHECKED_DECL_OBJECT_KIND_RECORD,
    LILY_CHECKED_DECL_OBJECT_KIND_TRAIT
};

/**
 *
 * @brief Convert LilyCheckedDeclObjectKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedDeclObjectKind,
               enum LilyCheckedDeclObjectKind self);
#endif

typedef struct LilyCheckedDeclObject
{
    enum LilyCheckedDeclObjectKind kind;
    // Type dependencies
    Vec *deps; // Vec<LilyCheckedDataType* (&)>*
    union
    {
        LilyCheckedDeclClass class;
        LilyCheckedDeclEnumObject enum_;
        LilyCheckedDeclRecordObject record;
        LilyCheckedDeclTrait trait;
    };
} LilyCheckedDeclObject;

/**
 *
 * @brief Construct LilyCheckedDeclObject type
 * (LILY_CHECKED_DECL_OBJECT_KIND_CLASS).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedDeclObject,
                           LilyCheckedDeclObject,
                           class,
                           LilyCheckedDeclClass class)
{
    return (LilyCheckedDeclObject){ .kind = LILY_CHECKED_DECL_OBJECT_KIND_CLASS,
                                    .deps = NEW(Vec),
                                    .class = class };
}

/**
 *
 * @brief Construct LilyCheckedDeclObject type
 * (LILY_CHECKED_DECL_OBJECT_KIND_ENUM).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedDeclObject,
                           LilyCheckedDeclObject,
                           enum,
                           LilyCheckedDeclEnumObject enum_)
{
    return (LilyCheckedDeclObject){ .kind = LILY_CHECKED_DECL_OBJECT_KIND_ENUM,
                                    .deps = NEW(Vec),
                                    .enum_ = enum_ };
}

/**
 *
 * @brief Construct LilyCheckedDeclObject type
 * (LILY_CHECKED_DECL_OBJECT_KIND_RECORD).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedDeclObject,
                           LilyCheckedDeclObject,
                           record,
                           LilyCheckedDeclRecordObject record)
{
    return (LilyCheckedDeclObject){ .kind =
                                      LILY_CHECKED_DECL_OBJECT_KIND_RECORD,
                                    .deps = NEW(Vec),
                                    .record = record };
}

/**
 *
 * @brief Construct LilyCheckedDeclObject type
 * (LILY_CHECKED_DECL_OBJECT_KIND_TRAIT).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedDeclObject,
                           LilyCheckedDeclObject,
                           trait,
                           LilyCheckedDeclTrait trait)
{
    return (LilyCheckedDeclObject){ .kind = LILY_CHECKED_DECL_OBJECT_KIND_TRAIT,
                                    .deps = NEW(Vec),
                                    .trait = trait };
}

/**
 *
 * @brief Convert LilyCheckedDeclObject in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedDeclObject,
               const LilyCheckedDeclObject *self);
#endif

/**
 *
 * @brief Free LilyCheckedDeclObject type.
 */
DESTRUCTOR(LilyCheckedDeclObject, const LilyCheckedDeclObject *self);

#endif // LILY_CORE_LILY_ANALYSIS_CHECKED_DECL_OBJECT_H
