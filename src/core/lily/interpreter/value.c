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

#include <base/alloc.h>

#include <core/lily/interpreter/value.h>

#include <stdio.h>
#include <stdlib.h>

// Free LilyInterpreterValueObject type
// (LILY_INTERPRETER_VALUE_OBJECT_KIND_INSTANCE).
static inline VARIANT_DESTRUCTOR(LilyInterpreterValueObject,
                                 instance,
                                 const LilyInterpreterValueObject *self);

// Free LilyInterpreterValueObject type
// (LILY_INTERPRETER_VALUE_OBJECT_KIND_LIST).
static inline VARIANT_DESTRUCTOR(LilyInterpreterValueObject,
                                 list,
                                 const LilyInterpreterValueObject *self);

// Free LilyInterpreterValueObject type
// (LILY_INTERPRETER_VALUE_OBJECT_KIND_RESULT).
static inline VARIANT_DESTRUCTOR(LilyInterpreterValueObject,
                                 result,
                                 const LilyInterpreterValueObject *self);

// Free LilyInterpreterValueObject type
// (LILY_INTERPRETER_VALUE_OBJECT_KIND_STRUCT).
static inline VARIANT_DESTRUCTOR(LilyInterpreterValueObject,
                                 struct,
                                 const LilyInterpreterValueObject *self);

DESTRUCTOR(LilyInterpreterValueInstance,
           const LilyInterpreterValueInstance *self)
{
    FREE(LilyInterpreterValue, self->value);
    lily_free(self->value);
}

CONSTRUCTOR(LilyInterpreterValueListNode *,
            LilyInterpreterValueListNode,
            LilyInterpreterValue *value,
            struct LilyInterpreterValueListNode *next)
{
    LilyInterpreterValueListNode *self =
      lily_malloc(sizeof(LilyInterpreterValueListNode));

    self->value = value;
    self->next = next;

    return self;
}

DESTRUCTOR(LilyInterpreterValueListNode, LilyInterpreterValueListNode *self)
{
    FREE(LilyInterpreterValue, self->value);
    lily_free(self->value);

    LilyInterpreterValueListNode *current = self->next;

    while (current) {
        FREE(LilyInterpreterValue, current->value);
        lily_free(current->value);
    }
}

DESTRUCTOR(LilyInterpreterValueResult, const LilyInterpreterValueResult *self)
{
    switch (self->kind) {
        case LILY_INTERPRETER_VALUE_RESULT_KIND_OK:
            return lily_free(self->ok);
        case LILY_INTERPRETER_VALUE_RESULT_KIND_ERR:
            return lily_free(self->err);
        default:
            UNREACHABLE("unknown variant");
    }
}

CONSTRUCTOR(LilyInterpreterValueStruct,
            LilyInterpreterValueStruct,
            LilyInterpreterValue **values,
            Usize len)
{
    LilyInterpreterValueStruct self =
      (LilyInterpreterValueStruct){ .values = { 0 }, .len = len };

    for (Usize i = 0; i < len; ++i) {
        self.values[i] = values[i];
    }

    return self;
}

DESTRUCTOR(LilyInterpreterValueStruct, const LilyInterpreterValueStruct *self)
{
    for (Usize i = 0; i < self->len; ++i) {
        lily_free(self->values[i]);
    }
}

VARIANT_DESTRUCTOR(LilyInterpreterValueObject,
                   instance,
                   const LilyInterpreterValueObject *self)
{
    FREE(LilyInterpreterValueInstance, &self->instance);
}

VARIANT_DESTRUCTOR(LilyInterpreterValueObject,
                   list,
                   const LilyInterpreterValueObject *self)
{
    FREE(LilyInterpreterValueList, &self->list);
}

VARIANT_DESTRUCTOR(LilyInterpreterValueObject,
                   result,
                   const LilyInterpreterValueObject *self)
{
    FREE(LilyInterpreterValueResult, &self->result);
}

VARIANT_DESTRUCTOR(LilyInterpreterValueObject,
                   struct,
                   const LilyInterpreterValueObject *self)
{
    FREE(LilyInterpreterValueStruct, &self->struct_);
}

DESTRUCTOR(LilyInterpreterValueObject, const LilyInterpreterValueObject *self)
{
    switch (self->kind) {
        case LILY_INTERPRETER_VALUE_OBJECT_KIND_INSTANCE:
            return FREE_VARIANT(LilyInterpreterValueObject, instance, self);
        case LILY_INTERPRETER_VALUE_OBJECT_KIND_LIST:
            return FREE_VARIANT(LilyInterpreterValueObject, list, self);
        case LILY_INTERPRETER_VALUE_OBJECT_KIND_RESULT:
            return FREE_VARIANT(LilyInterpreterValueObject, result, self);
        case LILY_INTERPRETER_VALUE_OBJECT_KIND_STRUCT:
            return FREE_VARIANT(LilyInterpreterValueObject, struct, self);
        default:
            return;
    }
}

DESTRUCTOR(LilyInterpreterValue, const LilyInterpreterValue *self)
{
    switch (self->kind) {
        case LILY_INTERPRETER_VALUE_KIND_OBJECT:
            FREE(LilyInterpreterValueObject, &self->object);
            return;
        default:
            return;
    }
}
