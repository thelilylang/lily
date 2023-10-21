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

#include <core/lily/interpreter/vm/value.h>

#include <stdio.h>
#include <stdlib.h>

// Free LilyInterpreterValueObject type
// (LILY_INTERPRETER_VALUE_OBJECT_KIND_INSTANCE).
static inline VARIANT_DESTRUCTOR(LilyInterpreterValueObject,
                                 instance,
                                 LilyInterpreterValueObject *self);

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

DESTRUCTOR(LilyInterpreterValueInstance, LilyInterpreterValueInstance *self)
{
    FREE(LilyInterpreterValue, &self->value);
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
    FREE(LilyInterpreterValue, &self->value);
    lily_free(self->value);

    LilyInterpreterValueListNode *current = self->next;

    while (current) {
        FREE(LilyInterpreterValue, &current->value);
        lily_free(current->value);
    }
}

DESTRUCTOR(LilyInterpreterValueList, const LilyInterpreterValueList *self)
{
    FREE(LilyInterpreterValueListNode, self->first);
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
                   LilyInterpreterValueObject *self)
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

DESTRUCTOR(LilyInterpreterValueObject, LilyInterpreterValueObject *self)
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

VARIANT_CONSTRUCTOR(LilyInterpreterValue *,
                    LilyInterpreterValue,
                    int8,
                    Int8 int8)
{
    LilyInterpreterValue *self = lily_malloc(sizeof(LilyInterpreterValue));

    self->kind = LILY_INTERPRETER_VALUE_KIND_INT8;
    self->ref_count = 0;
    self->int8 = int8;

    return self;
}

VARIANT_CONSTRUCTOR(LilyInterpreterValue *,
                    LilyInterpreterValue,
                    int16,
                    Int16 int16)
{
    LilyInterpreterValue *self = lily_malloc(sizeof(LilyInterpreterValue));

    self->kind = LILY_INTERPRETER_VALUE_KIND_INT16;
    self->ref_count = 0;
    self->int16 = int16;

    return self;
}

VARIANT_CONSTRUCTOR(LilyInterpreterValue *,
                    LilyInterpreterValue,
                    int32,
                    Int32 int32)
{
    LilyInterpreterValue *self = lily_malloc(sizeof(LilyInterpreterValue));

    self->kind = LILY_INTERPRETER_VALUE_KIND_INT32;
    self->ref_count = 0;
    self->int32 = int32;

    return self;
}

VARIANT_CONSTRUCTOR(LilyInterpreterValue *,
                    LilyInterpreterValue,
                    int64,
                    Int64 int64)
{
    LilyInterpreterValue *self = lily_malloc(sizeof(LilyInterpreterValue));

    self->kind = LILY_INTERPRETER_VALUE_KIND_INT64;
    self->ref_count = 0;
    self->int64 = int64;

    return self;
}

VARIANT_CONSTRUCTOR(LilyInterpreterValue *,
                    LilyInterpreterValue,
                    isize,
                    Isize isize)
{
    LilyInterpreterValue *self = lily_malloc(sizeof(LilyInterpreterValue));

    self->kind = LILY_INTERPRETER_VALUE_KIND_ISIZE;
    self->ref_count = 0;
    self->isize = isize;

    return self;
}

VARIANT_CONSTRUCTOR(LilyInterpreterValue *,
                    LilyInterpreterValue,
                    float,
                    Float64 float_)
{
    LilyInterpreterValue *self = lily_malloc(sizeof(LilyInterpreterValue));

    self->kind = LILY_INTERPRETER_VALUE_KIND_FLOAT;
    self->ref_count = 0;
    self->float_ = float_;

    return self;
}

VARIANT_CONSTRUCTOR(LilyInterpreterValue *,
                    LilyInterpreterValue,
                    object,
                    LilyInterpreterValueObject object)
{
    LilyInterpreterValue *self = lily_malloc(sizeof(LilyInterpreterValue));

    self->kind = LILY_INTERPRETER_VALUE_KIND_OBJECT;
    self->ref_count = 0;
    self->object = object;

    return self;
}

VARIANT_CONSTRUCTOR(LilyInterpreterValue *,
                    LilyInterpreterValue,
                    uint8,
                    Uint8 uint8)
{
    LilyInterpreterValue *self = lily_malloc(sizeof(LilyInterpreterValue));

    self->kind = LILY_INTERPRETER_VALUE_KIND_UINT8;
    self->ref_count = 0;
    self->uint8 = uint8;

    return self;
}

VARIANT_CONSTRUCTOR(LilyInterpreterValue *,
                    LilyInterpreterValue,
                    uint16,
                    Uint16 uint16)
{
    LilyInterpreterValue *self = lily_malloc(sizeof(LilyInterpreterValue));

    self->kind = LILY_INTERPRETER_VALUE_KIND_UINT16;
    self->ref_count = 0;
    self->uint16 = uint16;

    return self;
}

VARIANT_CONSTRUCTOR(LilyInterpreterValue *,
                    LilyInterpreterValue,
                    uint32,
                    Uint32 uint32)
{
    LilyInterpreterValue *self = lily_malloc(sizeof(LilyInterpreterValue));

    self->kind = LILY_INTERPRETER_VALUE_KIND_UINT32;
    self->ref_count = 0;
    self->uint32 = uint32;

    return self;
}

VARIANT_CONSTRUCTOR(LilyInterpreterValue *,
                    LilyInterpreterValue,
                    uint64,
                    Uint64 uint64)
{
    LilyInterpreterValue *self = lily_malloc(sizeof(LilyInterpreterValue));

    self->kind = LILY_INTERPRETER_VALUE_KIND_UINT64;
    self->ref_count = 0;
    self->uint64 = uint64;

    return self;
}

VARIANT_CONSTRUCTOR(LilyInterpreterValue *,
                    LilyInterpreterValue,
                    usize,
                    Usize usize)
{
    LilyInterpreterValue *self = lily_malloc(sizeof(LilyInterpreterValue));

    self->kind = LILY_INTERPRETER_VALUE_KIND_USIZE;
    self->ref_count = 0;
    self->usize = usize;

    return self;
}

CONSTRUCTOR(LilyInterpreterValue *,
            LilyInterpreterValue,
            enum LilyInterpreterValueKind kind)
{
    LilyInterpreterValue *self = lily_malloc(sizeof(LilyInterpreterValue));

    self->kind = kind;
    self->ref_count = 0;

    return self;
}

DESTRUCTOR(LilyInterpreterValue, LilyInterpreterValue **self)
{
    if ((*self)->ref_count > 0) {
        --(*self)->ref_count;
        return;
    }

    switch ((*self)->kind) {
        case LILY_INTERPRETER_VALUE_KIND_OBJECT:
            FREE(LilyInterpreterValueObject, &(*self)->object);
            lily_free(*self);
            *self = NULL;
            return;
        default:
            lily_free(*self);
            *self = NULL;
            return;
    }
}
