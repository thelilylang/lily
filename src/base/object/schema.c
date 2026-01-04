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

#include <base/object/schema.h>

/// @brief Free ObjectSchema type (OBJECT_SCHEMA_KIND_LIST).
static VARIANT_DESTRUCTOR(ObjectSchema, list, ObjectSchema *self);

/// @brief Free ObjectSchema type (OBJECT_SCHEMA_KIND_OBJECT).
static VARIANT_DESTRUCTOR(ObjectSchema, object, ObjectSchema *self);

CONSTRUCTOR(ObjectSchema *, ObjectSchema, enum ObjectSchemaKind kind)
{
    ObjectSchema *self = lily_malloc(sizeof(ObjectSchema));

    self->kind = kind;
    self->is_required = false;
    self->possible_values = NULL;

    return self;
}

VARIANT_CONSTRUCTOR(ObjectSchema *, ObjectSchema, list, Usize count, ...)
{
    ObjectSchema *self = lily_malloc(sizeof(ObjectSchema));
    va_list arg;

    va_start(arg, count);

    self->kind = OBJECT_SCHEMA_KIND_LIST;
    self->is_required = false;
    self->possible_values = NULL;
    self->list = vinit__Vec(count, arg);

    va_end(arg);

    return self;
}

VARIANT_CONSTRUCTOR(ObjectSchema *, ObjectSchema, object, Usize count, ...)
{
    ObjectSchema *self = lily_malloc(sizeof(ObjectSchema));
    va_list arg;

    va_start(arg, count);

    self->kind = OBJECT_SCHEMA_KIND_OBJECT;
    self->is_required = false;
    self->possible_values = NULL;
    self->object = vinit__OrderedHashMap(count, arg);

    va_end(arg);

    return self;
}

ObjectSchema *
add_possible_values__ObjectSchema(ObjectSchema *self, Usize count, ...)
{
    va_list arg;

    va_start(arg, count);

    self->possible_values = vinit__Vec(count, arg);

    va_end(arg);

    return self;
}

VARIANT_DESTRUCTOR(ObjectSchema, list, ObjectSchema *self)
{
    FREE_BUFFER_ITEMS(self->list->buffer, self->list->len, ObjectSchema);
    FREE(Vec, self->list);
    lily_free(self);
}

VARIANT_DESTRUCTOR(ObjectSchema, object, ObjectSchema *self)
{
    FREE_ORD_HASHMAP_VALUES(self->object, ObjectSchema);
    FREE(OrderedHashMap, self->object);
    lily_free(self);
}

DESTRUCTOR(ObjectSchema, ObjectSchema *self)
{
    if (self->possible_values) {
        FREE_BUFFER_ITEMS(self->possible_values->buffer,
                          self->possible_values->len,
                          ObjectValue);
        FREE(Vec, self->possible_values);
    }

    switch (self->kind) {
        case OBJECT_SCHEMA_KIND_LIST:
            FREE_VARIANT(ObjectSchema, list, self);

            break;
        case OBJECT_SCHEMA_KIND_OBJECT:
            FREE_VARIANT(ObjectSchema, object, self);

            break;
        default:
            lily_free(self);
    }
}
