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

#ifndef LILY_BASE_OBJECT_SCHEMA_H
#define LILY_BASE_OBJECT_SCHEMA_H

#include <base/alloc.h>
#include <base/macros.h>
#include <base/object/value.h>
#include <base/ordered_hash_map.h>
#include <base/vec.h>

#define BOOL_OBJ_SCHEMA() NEW(ObjectSchema, OBJECT_SCHEMA_KIND_BOOL)
#define CHAR_OBJ_SCHEMA() NEW(ObjectSchema, OBJECT_SCHEMA_KIND_CHAR)
#define INT_OBJ_SCHEMA() NEW(ObjectSchema, OBJECT_SCHEMA_KIND_INT)
#define LIST_OBJ_SCHEMA(count, ...) \
    NEW_VARIANT(ObjectSchema, list, count, __VA_ARGS__)
#define NULL_OBJ_SCHEMA(o) make_null__ObjectSchema(o)
#define OBJECT_OBJ_SCHEMA(count, ...) \
    NEW_VARIANT(ObjectSchema, object, count, __VA_ARGS__)
#define HASH_OBJ_SCHEMA(k, v) \
    (OrderedHashMapInitPair)  \
    {                         \
        .key = k, .value = v  \
    }
#define STRING_OBJ_SCHEMA() NEW(ObjectSchema, OBJECT_SCHEMA_KIND_STRING)
#define UINT_OBJ_SCHEMA() NEW(ObjectSchema, OBJECT_SCHEMA_KIND_UINT)

#define REQUIRE_OBJ_SCHEMA(o) make_required__ObjectSchema(o)
#define ADD_PV_OBJ_SCHEMA(o) add_possible_values__ObjectSchema(o)

enum ObjectSchemaKind
{
    OBJECT_SCHEMA_KIND_BOOL = 1 << 0,
    OBJECT_SCHEMA_KIND_CHAR = 1 << 1,
    OBJECT_SCHEMA_KIND_INT = 1 << 2,
    OBJECT_SCHEMA_KIND_LIST = 1 << 3,
    OBJECT_SCHEMA_KIND_NULL = 1 << 4,
    OBJECT_SCHEMA_KIND_OBJECT = 1 << 5,
    OBJECT_SCHEMA_KIND_STRING = 1 << 6,
    OBJECT_SCHEMA_KIND_UINT = 1 << 7,
};

typedef struct ObjectSchema
{
    enum ObjectSchemaKind kind;
    bool is_required;
    Vec *possible_values; // Vec<ObjectValue*>*?
    union
    {
        Vec *list;              // Vec<ObjectSchema*>*
        OrderedHashMap *object; // HashMap<char*, ObjectSchema*>*
    };
} ObjectSchema;

/**
 *
 * @brief Construct ObjectSchema type.
 */
CONSTRUCTOR(ObjectSchema *, ObjectSchema, enum ObjectSchemaKind kind);

/**
 *
 * @brief Construct ObjectSchema type (OBJECT_SCHEMA_KIND_LIST).
 */
VARIANT_CONSTRUCTOR(ObjectSchema *, ObjectSchema, list, Usize count, ...);

/**
 *
 * @brief Construct ObjectSchema type (OBJECT_SCHEMA_KIND_OBJECT).
 */
VARIANT_CONSTRUCTOR(ObjectSchema *, ObjectSchema, object, Usize count, ...);

/**
 *
 * @brief Make object nullable.
 */
inline ObjectSchema *
make_null__ObjectSchema(ObjectSchema *self)
{
    self->kind |= OBJECT_SCHEMA_KIND_NULL;
    return self;
}

/**
 *
 * @brief Make object required.
 */
inline ObjectSchema *
make_required__ObjectSchema(ObjectSchema *self)
{
    self->is_required = true;
    return self;
}

/**
 *
 * @brief Add optiona values to object schema.
 */
ObjectSchema *
add_possible_values__ObjectSchema(ObjectSchema *self, Usize count, ...);

/**
 *
 * @brief Free ObjectSchema type.
 */
DESTRUCTOR(ObjectSchema, ObjectSchema *self);

#endif // LILY_BASE_OBJECT_SCHEMA_H
