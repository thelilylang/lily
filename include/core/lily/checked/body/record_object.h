/*
 * MIT License
 *
 * Copyright (c) 2022-2023 ArthurPV
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

#ifndef LILY_CORE_LILY_CHECKED_BODY_RECORD_OBJECT_H
#define LILY_CORE_LILY_CHECKED_BODY_RECORD_OBJECT_H

#include <core/lily/checked/decl/constant.h>
#include <core/lily/checked/decl/method.h>
#include <core/lily/checked/field_object.h>
#include <core/shared/location.h>

enum LilyCheckedBodyRecordObjectItemKind
{
    LILY_CHECKED_BODY_RECORD_OBJECT_ITEM_KIND_CONSTANT,
    LILY_CHECKED_BODY_RECORD_OBJECT_ITEM_KIND_FIELD,
    LILY_CHECKED_BODY_RECORD_OBJECT_ITEM_KIND_METHOD
};

/**
 *
 * @brief Convert LilyCheckedBodyRecordbjectItemKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedBodyRecordObjectItemKind,
               enum LilyCheckedBodyRecordObjectItemKind self);
#endif

typedef struct LilyCheckedBodyRecordObjectItem
{
    enum LilyCheckedBodyRecordObjectItemKind kind;
    Location location;
    union
    {
        LilyCheckedDeclConstant constant;
        LilyCheckedFieldObject field;
        LilyCheckedDeclMethod method;
    };
} LilyCheckedBodyRecordObjectItem;

/**
 *
 * @brief Construct LilyCheckedBodyRecordObjectItem type
 * (LILY_CHECKED_BODY_RECORD_OBJECT_ITEM_KIND_CONSTANT).
 */
VARIANT_CONSTRUCTOR(LilyCheckedBodyRecordObjectItem *,
                    LilyCheckedBodyRecordObjectItem,
                    constant,
                    Location location,
                    LilyCheckedDeclConstant constant);

/**
 *
 * @brief Construct LilyCheckedBodyRecordObjectItem type
 * (LILY_CHECKED_BODY_RECORD_OBJECT_ITEM_KIND_FIELD).
 */
VARIANT_CONSTRUCTOR(LilyCheckedBodyRecordObjectItem *,
                    LilyCheckedBodyRecordObjectItem,
                    field,
                    Location location,
                    LilyCheckedFieldObject field);

/**
 *
 * @brief Construct LilyCheckedBodyRecordObjectItem type
 * (LILY_CHECKED_BODY_RECORD_OBJECT_ITEM_KIND_METHOD).
 */
VARIANT_CONSTRUCTOR(LilyCheckedBodyRecordObjectItem *,
                    LilyCheckedBodyRecordObjectItem,
                    method,
                    Location location,
                    LilyCheckedDeclMethod method);

/**
 *
 * @brief Convert LilyCheckedBodyRecordObjectItem in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedBodyRecordObjectItem,
               const LilyCheckedBodyRecordObjectItem *self);
#endif

/**
 *
 * @brief Free LilyCheckedBodyRecordObjectItem type.
 */
DESTRUCTOR(LilyCheckedBodyRecordObjectItem, LilyCheckedBodyRecordObjectItem *self);

#endif // LILY_CORE_LILY_CHECKED_BODY_RECORD_OBJECT_H
