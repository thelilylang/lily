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

#include <base/alloc.h>
#include <base/object/value.h>

#include <stdio.h>
#include <stdlib.h>

/// @brief Free ObjectValue type (OBJECT_VALUE_KIND_BOOL).
static inline VARIANT_DESTRUCTOR(ObjectValue, bool, ObjectValue *self);

/// @brief Free ObjectValue type (OBJECT_VALUE_KIND_CHAR).
static inline VARIANT_DESTRUCTOR(ObjectValue, char, ObjectValue *self);

/// @brief Free ObjectValue type (OBJECT_VALUE_KIND_INT).
static inline VARIANT_DESTRUCTOR(ObjectValue, int, ObjectValue *self);

/// @brief Free ObjectValue type (OBJECT_VALUE_KIND_LIST).
static VARIANT_DESTRUCTOR(ObjectValue, list, ObjectValue *self);

/// @brief Free ObjectValue type (OBJECT_VALUE_KIND_NULL).
static inline VARIANT_DESTRUCTOR(ObjectValue, null, ObjectValue *self);

/// @brief Free ObjectValue type (OBJECT_VALUE_KIND_OBJECT).
static VARIANT_DESTRUCTOR(ObjectValue, object, ObjectValue *self);

/// @brief Free ObjectValue type (OBJECT_VALUE_KIND_STRING).
static VARIANT_DESTRUCTOR(ObjectValue, string, ObjectValue *self);

/// @brief Free ObjectValue type (OBJECT_VALUE_KIND_UINT).
static inline VARIANT_DESTRUCTOR(ObjectValue, uint, ObjectValue *self);

VARIANT_CONSTRUCTOR(ObjectValue *, ObjectValue, bool, bool bool_)
{
    ObjectValue *self = lily_malloc(sizeof(ObjectValue));

    self->kind = OBJECT_VALUE_KIND_BOOL;
    self->ref_count = 0;
    self->bool_ = bool_;

    return self;
}

VARIANT_CONSTRUCTOR(ObjectValue *, ObjectValue, char, char char_)
{
    ObjectValue *self = lily_malloc(sizeof(ObjectValue));

    self->kind = OBJECT_VALUE_KIND_CHAR;
    self->ref_count = 0;
    self->char_ = char_;

    return self;
}

VARIANT_CONSTRUCTOR(ObjectValue *, ObjectValue, int, Isize int_)
{
    ObjectValue *self = lily_malloc(sizeof(ObjectValue));

    self->kind = OBJECT_VALUE_KIND_INT;
    self->ref_count = 0;
    self->int_ = int_;

    return self;
}

VARIANT_CONSTRUCTOR(ObjectValue *, ObjectValue, list, Usize count, ...)
{
    ObjectValue *self = lily_malloc(sizeof(ObjectValue));
    va_list arg;

    va_start(arg, count);

    self->kind = OBJECT_VALUE_KIND_LIST;
    self->ref_count = 0;
    self->list = vinit__ObjectValueList(count, arg);

    va_end(arg);

    return self;
}

VARIANT_CONSTRUCTOR(ObjectValue *, ObjectValue, null, enum ObjectValueKind kind)
{
    ObjectValue *self = lily_malloc(sizeof(ObjectValue));

    self->kind = kind | OBJECT_VALUE_KIND_NULL;
    self->ref_count = 0;

    return self;
}

VARIANT_CONSTRUCTOR(ObjectValue *, ObjectValue, object, Usize count, ...)
{
    ObjectValue *self = lily_malloc(sizeof(ObjectValue));
    va_list arg;

    va_start(arg, count);

    self->kind = OBJECT_VALUE_KIND_OBJECT;
    self->ref_count = 0;
    self->object = vinit__ObjectValueObject(count, arg);

    va_end(arg);

    return self;
}

VARIANT_CONSTRUCTOR(ObjectValue *, ObjectValue, string, String *string)
{
    ObjectValue *self = lily_malloc(sizeof(ObjectValue));

    self->kind = OBJECT_VALUE_KIND_STRING;
    self->ref_count = 0;
    self->string = string;

    return self;
}

VARIANT_CONSTRUCTOR(ObjectValue *, ObjectValue, uint, Usize uint)
{
    ObjectValue *self = lily_malloc(sizeof(ObjectValue));

    self->kind = OBJECT_VALUE_KIND_UINT;
    self->ref_count = 0;
    self->uint = uint;

    return self;
}

VARIANT_DESTRUCTOR(ObjectValue, bool, ObjectValue *self)
{
    lily_free(self);
}

VARIANT_DESTRUCTOR(ObjectValue, char, ObjectValue *self)
{
    lily_free(self);
}

VARIANT_DESTRUCTOR(ObjectValue, int, ObjectValue *self)
{
    lily_free(self);
}

VARIANT_DESTRUCTOR(ObjectValue, list, ObjectValue *self)
{
    FREE(ObjectValueList, &self->list);
    lily_free(self);
}

VARIANT_DESTRUCTOR(ObjectValue, null, ObjectValue *self)
{
    lily_free(self);
}

VARIANT_DESTRUCTOR(ObjectValue, object, ObjectValue *self)
{
    FREE(ObjectValueObject, &self->object);
    lily_free(self);
}

VARIANT_DESTRUCTOR(ObjectValue, string, ObjectValue *self)
{
    FREE(String, self->string);
    lily_free(self);
}

VARIANT_DESTRUCTOR(ObjectValue, uint, ObjectValue *self)
{
    lily_free(self);
}

DESTRUCTOR(ObjectValue, ObjectValue *self)
{
    if (self->ref_count > 0) {
        --self->ref_count;
        return;
    }

    switch (self->kind) {
        case OBJECT_VALUE_KIND_BOOL:
            FREE_VARIANT(ObjectValue, bool, self);

            break;
        case OBJECT_VALUE_KIND_CHAR:
            FREE_VARIANT(ObjectValue, char, self);

            break;
        case OBJECT_VALUE_KIND_INT:
            FREE_VARIANT(ObjectValue, int, self);

            break;
        case OBJECT_VALUE_KIND_LIST:
            FREE_VARIANT(ObjectValue, list, self);

            break;
        case OBJECT_VALUE_KIND_NULL:
            FREE_VARIANT(ObjectValue, null, self);

            break;
        case OBJECT_VALUE_KIND_OBJECT:
            FREE_VARIANT(ObjectValue, object, self);

            break;
        case OBJECT_VALUE_KIND_STRING:
            FREE_VARIANT(ObjectValue, string, self);

            break;
        case OBJECT_VALUE_KIND_UINT:
            FREE_VARIANT(ObjectValue, uint, self);

            break;
        default:
            UNREACHABLE("unknown variant");
    }
}
