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

#ifndef LILY_BASE_OBJECT_VALUE_OBJECT_H
#define LILY_BASE_OBJECT_VALUE_OBJECT_H

#include <base/macros.h>
#include <base/new.h>
#include <base/ordered_hash_map.h>

typedef struct ObjectValue ObjectValue;

typedef struct ObjectValueObject
{
    OrderedHashMap *content; // OrderedHashMap<char* (&), struct ObjectValue*>*
} ObjectValueObject;

/**
 *
 * @brief Construct ObjectValueObject type.
 */
inline CONSTRUCTOR(ObjectValueObject, ObjectValueObject)
{
    return (ObjectValueObject){ .content = NEW(OrderedHashMap) };
}

/**
 *
 * @brief Build a custom object.
 */
ObjectValueObject
init__ObjectValueObject(Usize count, ...);

/**
 *
 * @brief Alternative version of the `init__ObjectValueObject` function, to pass
 * the argument directly.
 */
ObjectValueObject
vinit__ObjectValueObject(Usize count, va_list arg);

/**
 *
 * @brief Add object value to custom object.
 */
inline void
add__ObjectValueObject(ObjectValueObject *self,
                       char *object_name,
                       struct ObjectValue *object_value)
{
    insert__OrderedHashMap(self->content, object_name, object_value);
}

/**
 *
 * @brief Free ObjectValueObject type.
 */
DESTRUCTOR(ObjectValueObject, const ObjectValueObject *self);

#endif // LILY_BASE_OBJECT_VALUE_OBJECT_H
