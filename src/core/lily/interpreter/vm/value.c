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

#include <base/alloc.h>
#include <base/assert.h>

#include <core/lily/interpreter/vm.h>
#include <core/lily/interpreter/vm/runtime.h>
#include <core/lily/interpreter/vm/value.h>

#include <stdio.h>
#include <stdlib.h>

void
store__LilyInterpreterValue(LilyInterpreterValue *self,
                            const LilyInterpreterValue *src,
                            bool raw_value)
{
    // Free old values (values allocated by the runtime).
    switch (self->kind) {
        case LILY_INTERPRETER_VALUE_KIND_BYTES:
            FREE(LilyInterpreterValueBytes, self->bytes);
            break;
        case LILY_INTERPRETER_VALUE_KIND_DYNAMIC_ARRAY:
            FREE(LilyInterpreterValueDynamicArray, self->dynamic_array);
            break;
        case LILY_INTERPRETER_VALUE_KIND_LIST:
            FREE(LilyInterpreterValueList, self->list);
            break;
        case LILY_INTERPRETER_VALUE_KIND_MULTI_POINTERS_ARRAY:
            FREE(LilyInterpreterValueMultiPointersArray,
                 self->multi_pointers_array);
            break;
        case LILY_INTERPRETER_VALUE_KIND_RESULT:
            FREE(LilyInterpreterValueResult, self->result);
            break;
        case LILY_INTERPRETER_VALUE_KIND_SIZED_ARRAY:
            FREE(LilyInterpreterValueSizedArray, self->sized_array);
            break;
        case LILY_INTERPRETER_VALUE_KIND_STR:
            FREE(LilyInterpreterValueStr, self->str);
            break;
        case LILY_INTERPRETER_VALUE_KIND_STRUCT:
            FREE(LilyInterpreterValueStruct, self->struct_);
            break;
        default:
            break;
    }

    self->kind = src->kind;

    switch (self->kind) {
        case LILY_INTERPRETER_VALUE_KIND_FALSE:
        case LILY_INTERPRETER_VALUE_KIND_TRUE:
        case LILY_INTERPRETER_VALUE_KIND_NIL:
        case LILY_INTERPRETER_VALUE_KIND_UNDEF:
            break;
        case LILY_INTERPRETER_VALUE_KIND_BYTES:
            self->bytes = raw_value
                            ? src->bytes
                            : ref__LilyInterpreterValueBytes(src->bytes);
            break;
        case LILY_INTERPRETER_VALUE_KIND_CSTR:
            self->cstr = src->cstr;
            break;
        case LILY_INTERPRETER_VALUE_KIND_DYNAMIC_ARRAY:
            self->dynamic_array =
              raw_value
                ? src->dynamic_array
                : ref__LilyInterpreterValueDynamicArray(src->dynamic_array);
            break;
        case LILY_INTERPRETER_VALUE_KIND_INT8:
            self->int8 = src->int8;
            break;
        case LILY_INTERPRETER_VALUE_KIND_INT16:
            self->int16 = src->int16;
            break;
        case LILY_INTERPRETER_VALUE_KIND_INT32:
            self->int32 = src->int32;
            break;
        case LILY_INTERPRETER_VALUE_KIND_INT64:
            self->int64 = src->int64;
            break;
        case LILY_INTERPRETER_VALUE_KIND_ISIZE:
            self->isize = src->isize;
            break;
        case LILY_INTERPRETER_VALUE_KIND_FLOAT:
            self->float_ = src->float_;
            break;
        case LILY_INTERPRETER_VALUE_KIND_LIST:
            self->list =
              raw_value ? src->list : ref__LilyInterpreterValueList(src->list);
            break;
        case LILY_INTERPRETER_VALUE_KIND_MULTI_POINTERS_ARRAY:
            self->multi_pointers_array =
              raw_value ? src->multi_pointers_array
                        : ref__LilyInterpreterValueMultiPointersArray(
                            src->multi_pointers_array);
            break;
        case LILY_INTERPRETER_VALUE_KIND_PTR:
            self->ptr = src->ptr;
            break;
        case LILY_INTERPRETER_VALUE_KIND_RESULT:
            self->result = raw_value
                             ? src->result
                             : ref__LilyInterpreterValueResult(src->result);
            break;
        case LILY_INTERPRETER_VALUE_KIND_SIZED_ARRAY:
            self->sized_array =
              raw_value ? src->sized_array
                        : ref__LilyInterpreterValueSizedArray(src->sized_array);
            break;
        case LILY_INTERPRETER_VALUE_KIND_STR:
            self->str =
              raw_value ? src->str : ref__LilyInterpreterValueStr(src->str);
            break;
        case LILY_INTERPRETER_VALUE_KIND_STRUCT:
            self->struct_ = raw_value
                              ? src->struct_
                              : ref__LilyInterpreterValueStruct(src->struct_);
            break;
        case LILY_INTERPRETER_VALUE_KIND_UINT8:
            self->uint8 = src->uint8;
            break;
        case LILY_INTERPRETER_VALUE_KIND_UINT16:
            self->uint16 = src->uint16;
            break;
        case LILY_INTERPRETER_VALUE_KIND_UINT32:
            self->uint32 = src->uint32;
            break;
        case LILY_INTERPRETER_VALUE_KIND_UINT64:
            self->uint64 = src->uint64;
            break;
        case LILY_INTERPRETER_VALUE_KIND_USIZE:
            self->usize = src->usize;
            break;
        case LILY_INTERPRETER_VALUE_KIND_UNIT:
            UNREACHABLE("you cannot store a unit value");
        case LILY_INTERPRETER_VALUE_KIND_DESTROYED:
            RUNTIME_ERROR_UNREACHABLE("you cannot store a destroyed value");
        default:
            UNREACHABLE("unknown variant");
    }
}

void
minimal_free__LilyInterpreterValue(LilyInterpreterValue *self)
{
    // NOTE: expected raw value
#ifdef LILY_FULL_ASSERT_VM
    ASSERT(self->ref_count == 0);
#endif

    if (self->kind == LILY_INTERPRETER_VALUE_KIND_DESTROYED) {
        RUNTIME_ERROR_UNREACHABLE("value is already destroyed");
    }

    self->kind = LILY_INTERPRETER_VALUE_KIND_DESTROYED;
}

DESTRUCTOR(LilyInterpreterValue, LilyInterpreterValue *self)
{
    if (self->kind == LILY_INTERPRETER_VALUE_KIND_DESTROYED) {
        RUNTIME_ERROR_UNREACHABLE("value is already destroyed");
    }

    if (self->ref_count > 0) {
        --self->ref_count;

        return;
    }

    switch (self->kind) {
        case LILY_INTERPRETER_VALUE_KIND_BYTES:
            FREE(LilyInterpreterValueBytes, self->bytes);
            break;
        case LILY_INTERPRETER_VALUE_KIND_DYNAMIC_ARRAY:
            FREE(LilyInterpreterValueDynamicArray, self->dynamic_array);
            break;
        case LILY_INTERPRETER_VALUE_KIND_LIST:
            FREE(LilyInterpreterValueList, self->list);
            break;
        case LILY_INTERPRETER_VALUE_KIND_MULTI_POINTERS_ARRAY:
            FREE(LilyInterpreterValueMultiPointersArray,
                 self->multi_pointers_array);
            break;
        case LILY_INTERPRETER_VALUE_KIND_RESULT:
            FREE(LilyInterpreterValueResult, self->result);
            break;
        case LILY_INTERPRETER_VALUE_KIND_SIZED_ARRAY:
            FREE(LilyInterpreterValueSizedArray, self->sized_array);
            break;
        case LILY_INTERPRETER_VALUE_KIND_STR:
            FREE(LilyInterpreterValueStr, self->str);
            break;
        case LILY_INTERPRETER_VALUE_KIND_STRUCT:
            FREE(LilyInterpreterValueStruct, self->struct_);
            break;
        default:
            break;
    }

    self->kind = LILY_INTERPRETER_VALUE_KIND_DESTROYED;
}

CONSTRUCTOR(LilyInterpreterValueDynamicArray *,
            LilyInterpreterValueDynamicArray)
{
    LilyInterpreterValueDynamicArray *self =
      lily_malloc(sizeof(LilyInterpreterValueDynamicArray));

    self->ref_count = 0;
    self->buffer = NULL;
    self->len = 0;

    return self;
}

DESTRUCTOR(LilyInterpreterValueDynamicArray,
           LilyInterpreterValueDynamicArray *self)
{
    if (self->ref_count > 0) {
        --self->ref_count;
        return;
    }

    for (Usize i = 0; i < self->len; ++i) {
        FREE(LilyInterpreterValue, &self->buffer[i]);
    }

    lily_free(self);
}

CONSTRUCTOR(LilyInterpreterValueMultiPointersArray *,
            LilyInterpreterValueMultiPointersArray)
{
    LilyInterpreterValueMultiPointersArray *self =
      lily_malloc(sizeof(LilyInterpreterValueMultiPointersArray));

    self->ref_count = 0;
    self->buffer = NULL;
    self->len = 0;

    return self;
}

DESTRUCTOR(LilyInterpreterValueMultiPointersArray,
           LilyInterpreterValueMultiPointersArray *self)
{
    if (self->ref_count > 0) {
        --self->ref_count;
        return;
    }

    for (Usize i = 0; i < self->len; ++i) {
        FREE(LilyInterpreterValue, &self->buffer[i]);
    }

    lily_free(self);
}

CONSTRUCTOR(LilyInterpreterValueSizedArray *,
            LilyInterpreterValueSizedArray,
            LilyInterpreterValue *buffer,
            Usize len)
{
    LilyInterpreterValueSizedArray *self =
      lily_malloc(sizeof(LilyInterpreterValueSizedArray));

    self->ref_count = 0;
    self->buffer = buffer;
    self->len = len;

    return self;
}

DESTRUCTOR(LilyInterpreterValueSizedArray, LilyInterpreterValueSizedArray *self)
{
    if (self->ref_count > 0) {
        --self->ref_count;
        return;
    }

    for (Usize i = 0; i < self->len; ++i) {
        FREE(LilyInterpreterValue, &self->buffer[i]);
    }

    lily_free(self->buffer);
    lily_free(self);
}

CONSTRUCTOR(LilyInterpreterValueBytes *,
            LilyInterpreterValueBytes,
            Uint8 *buffer,
            Usize len)
{
    LilyInterpreterValueBytes *self =
      lily_malloc(sizeof(LilyInterpreterValueBytes));

    self->ref_count = 0;
    self->buffer = buffer;
    self->len = len;

    return self;
}

DESTRUCTOR(LilyInterpreterValueBytes, LilyInterpreterValueBytes *self)
{
    if (self->ref_count > 0) {
        --self->ref_count;
        return;
    }

    lily_free(self);
}

CONSTRUCTOR(LilyInterpreterValueListNode *,
            LilyInterpreterValueListNode,
            LilyInterpreterValue value,
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
    LilyInterpreterValueListNode *current = self;

    while (current) {
        LilyInterpreterValueListNode *next = self->next;

        FREE(LilyInterpreterValue, &self->value);
        lily_free(current);

        current = next;
    }
}

CONSTRUCTOR(LilyInterpreterValueList *,
            LilyInterpreterValueList,
            LilyInterpreterValueListNode *head,
            LilyInterpreterValueListNode *tail)
{
    LilyInterpreterValueList *self =
      lily_malloc(sizeof(LilyInterpreterValueList));

    self->ref_count = 0;
    self->head = head;
    self->tail = tail;

    return self;
}

DESTRUCTOR(LilyInterpreterValueList, LilyInterpreterValueList *self)
{
    if (self->ref_count > 0) {
        --self->ref_count;
        return;
    }

    FREE(LilyInterpreterValueListNode, self->head);

    lily_free(self);
}

VARIANT_CONSTRUCTOR(LilyInterpreterValueResult *,
                    LilyInterpreterValueResult,
                    ok,
                    LilyInterpreterValue ok)
{
    LilyInterpreterValueResult *self =
      lily_malloc(sizeof(LilyInterpreterValueResult));

    self->kind = LILY_INTERPRETER_VALUE_RESULT_KIND_OK;
    self->ref_count = 0;
    self->ok = ok;

    return self;
}

VARIANT_CONSTRUCTOR(LilyInterpreterValueResult *,
                    LilyInterpreterValueResult,
                    err,
                    LilyInterpreterValue err)
{
    LilyInterpreterValueResult *self =
      lily_malloc(sizeof(LilyInterpreterValueResult));

    self->kind = LILY_INTERPRETER_VALUE_RESULT_KIND_ERR;
    self->ref_count = 0;
    self->err = err;

    return self;
}

DESTRUCTOR(LilyInterpreterValueResult, LilyInterpreterValueResult *self)
{
    if (self->ref_count > 0) {
        --self->ref_count;
        return;
    }

    switch (self->kind) {
        case LILY_INTERPRETER_VALUE_RESULT_KIND_OK:
            FREE(LilyInterpreterValue, &self->ok);
            break;
        case LILY_INTERPRETER_VALUE_RESULT_KIND_ERR:
            FREE(LilyInterpreterValue, &self->err);
            break;
        default:
            UNREACHABLE("unknown variant");
    }

    lily_free(self);
}

CONSTRUCTOR(LilyInterpreterValueStr *,
            LilyInterpreterValueStr,
            char *s,
            Usize len)
{
    LilyInterpreterValueStr *self =
      lily_malloc(sizeof(LilyInterpreterValueStr));

    self->ref_count = 0;
    self->s = s;
    self->len = len;

    return self;
}

DESTRUCTOR(LilyInterpreterValueStr, LilyInterpreterValueStr *self)
{
    if (self->ref_count > 0) {
        --self->ref_count;
        return;
    }

    lily_free(self);
}

CONSTRUCTOR(LilyInterpreterValueStruct *,
            LilyInterpreterValueStruct,
            LilyInterpreterValue *values,
            Usize len)
{
    LilyInterpreterValueStruct *self =
      lily_malloc(sizeof(LilyInterpreterValueStruct));

    self->ref_count = 0;
    self->len = len;

    for (Usize i = 0; i < len; ++i) {
        self->values[i] = values[i];
    }

    return self;
}

LilyInterpreterValue *
get__LilyInterpreterValueStruct(LilyInterpreterValueStruct *self, Usize n)
{
    return &self->values[n];
}

DESTRUCTOR(LilyInterpreterValueStruct, LilyInterpreterValueStruct *self)
{
    if (self->ref_count > 0) {
        --self->ref_count;
        return;
    }

    for (Usize i = 0; i < self->len; ++i) {
        FREE(LilyInterpreterValue, &self->values[i]);
    }

    lily_free(self);
}
