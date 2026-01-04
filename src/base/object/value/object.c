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

#include <base/object/value.h>
#include <base/object/value/object.h>

ObjectValueObject
init__ObjectValueObject(Usize count, ...)
{
    va_list arg;

    va_start(arg, count);

    ObjectValueObject res = vinit__ObjectValueObject(count, arg);

    va_end(arg);

    return res;
}

ObjectValueObject
vinit__ObjectValueObject(Usize count, va_list arg)
{
    ObjectValueObject self = NEW(ObjectValueObject);

    self.content = vinit__OrderedHashMap(count, arg);

    return self;
}

DESTRUCTOR(ObjectValueObject, const ObjectValueObject *self)
{
    FREE_ORD_HASHMAP_VALUES(self->content, ObjectValue);
    FREE(OrderedHashMap, self->content);
}
