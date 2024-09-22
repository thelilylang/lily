/*
 * MIT License
 *
 * Copyright (c) 2022-2024 ArthurPV
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

#ifndef LILY_BASE_OBJECT_VALUE_H
#define LILY_BASE_OBJECT_VALUE_H

#include <base/new.h>
#include <base/object/value/list.h>
#include <base/object/value/object.h>
#include <base/string.h>
#include <base/types.h>

#define BOOL_OBJ(v) NEW_VARIANT(ObjectValue, bool, v)
#define CHAR_OBJ(v) NEW_VARIANT(ObjectValue, char, v)
#define INT_OBJ(v) NEW_VARIANT(ObjectValue, int, v)
#define NULL_OBJ(k) NEW_VARIANT(ObjectValue, null, k)
#define LIST_OBJ(count, ...) NEW_VARIANT(ObjectValue, list, count, __VA_ARGS__)
#define STRING_OBJ(v) NEW_VARIANT(ObjectValue, string, from__String(v))
#define STRING_FMT_OBJ(v, ...) \
    NEW_VARIANT(ObjectValue, string, format__String(v, __VA_ARGS__))
#define UINT_OBJ(v) NEW_VARIANT(ObjectValue, uint, v)

enum ObjectValueKind
{
    OBJECT_VALUE_KIND_BOOL,
    OBJECT_VALUE_KIND_CHAR,
    OBJECT_VALUE_KIND_INT,
    OBJECT_VALUE_KIND_LIST,
    OBJECT_VALUE_KIND_NULL,
    OBJECT_VALUE_KIND_OBJECT,
    OBJECT_VALUE_KIND_STRING,
    OBJECT_VALUE_KIND_UINT,
};

typedef struct ObjectValue
{
    enum ObjectValueKind kind;
    Usize ref_count;
    union
    {
        bool bool_;
        char char_;
        Isize int_;
        ObjectValueList list;
        ObjectValueObject object;
        String *string; // String*
        Usize uint;
    };
} ObjectValue;

/**
 *
 * @brief Construct ObjectValue type (OBJECT_VALUE_KIND_BOOL).
 */
VARIANT_CONSTRUCTOR(ObjectValue *, ObjectValue, bool, bool bool_);

/**
 *
 * @brief Construct ObjectValue type (OBJECT_VALUE_KIND_CHAR).
 */
VARIANT_CONSTRUCTOR(ObjectValue *, ObjectValue, char, char char_);

/**
 *
 * @brief Construct ObjectValue type (OBJECT_VALUE_KIND_INT).
 */
VARIANT_CONSTRUCTOR(ObjectValue *, ObjectValue, int, Isize int_);

/**
 *
 * @brief Construct ObjectValue type (OBJECT_VALUE_KIND_LIST).
 */
VARIANT_CONSTRUCTOR(ObjectValue *, ObjectValue, list, Usize count, ...);

/**
 *
 * @brief Construct ObjectValue type (OBJECT_VALUE_KIND_NULL).
 */
VARIANT_CONSTRUCTOR(ObjectValue *,
                    ObjectValue,
                    null,
                    enum ObjectValueKind kind);

/**
 *
 * @brief Construct ObjectValue type (OBJECT_VALUE_KIND_OBJECT).
 */
VARIANT_CONSTRUCTOR(ObjectValue *, ObjectValue, object, Usize count, ...);

/**
 *
 * @brief Construct ObjectValue type (OBJECT_VALUE_KIND_STRING).
 */
VARIANT_CONSTRUCTOR(ObjectValue *, ObjectValue, string, String *string);

/**
 *
 * @brief Construct ObjectValue type (OBJECT_VALUE_KIND_UINT).
 */
VARIANT_CONSTRUCTOR(ObjectValue *, ObjectValue, uint, Usize uint);

/**
 *
 * @brief Free ObjectValue type.
 */
DESTRUCTOR(ObjectValue, ObjectValue *self);

#endif // LILY_BASE_OBJECT_VALUE_H
