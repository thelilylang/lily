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

#ifndef LILY_BASE_OBJECT_VALUE_LIST_H
#define LILY_BASE_OBJECT_VALUE_LIST_H

#include <base/macros.h>
#include <base/new.h>
#include <base/types.h>
#include <base/vec.h>

#include <stddef.h>

typedef struct ObjectValue ObjectValue;

typedef struct ObjectValueList
{
    Vec *content; // Vec<struct ObjectValue*>*
} ObjectValueList;

/**
 *
 * @brief Construct ObjectValueList type.
 */
inline CONSTRUCTOR(ObjectValueList, ObjectValueList)
{
    return (ObjectValueList){ .content = NEW(Vec) };
}

/**
 *
 * @brief Build a list of object.
 */
ObjectValueList
init__ObjectValueList(Usize count, ...);

/**
 *
 * @brief Alternative version of the `init__ObjectValueList` function, to pass
 * the argument directly.
 */
ObjectValueList
vinit__ObjectValueList(Usize count, va_list arg);

/**
 *
 * @brief Push object value to list.
 */
inline void
push__ObjectValueList(ObjectValueList *self, struct ObjectValue *object_value)
{
    push__Vec(self->content, object_value);
}

/**
 *
 * @brief Free ObjectValueList type.
 */
DESTRUCTOR(ObjectValueList, const ObjectValueList *self);

#endif // LILY_BASE_OBJECT_VALUE_LIST_H
