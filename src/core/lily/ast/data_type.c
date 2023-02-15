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

#include <core/lily/ast/data_type.h>

// Free LilyAstDataTypeLambda type.
static DESTRUCTOR(LilyAstDataTypeLambda, const LilyAstDataTypeLambda *self);

// Free LilyAstDataTypeArray type.
static DESTRUCTOR(LilyAstDataTypeArray, const LilyAstDataTypeArray *self);

// Free LilyAstDataTypeCustom type.
static DESTRUCTOR(LilyAstDataTypeCustom, const LilyAstDataTypeCustom *self);

// Free LilyAstDataType type (LILY_AST_DATA_TYPE_KIND_ARRAY).
static VARIANT_DESTRUCTOR(LilyAstDataType, array, LilyAstDataType *self);

// Free LilyAstDataType type (LILY_AST_DATA_TYPE_KIND_CUSTOM).
static VARIANT_DESTRUCTOR(LilyAstDataType, custom, LilyAstDataType *self);

// Free LilyAstDataType type (LILY_AST_DATA_TYPE_KIND_EXCEPTION).
static VARIANT_DESTRUCTOR(LilyAstDataType, exception, LilyAstDataType *self);

// Free LilyAstDataType type (LILY_AST_DATA_TYPE_KIND_LAMBDA).
static VARIANT_DESTRUCTOR(LilyAstDataType, lambda, LilyAstDataType *self);

// Free LilyAstDataType type (LILY_AST_DATA_TYPE_KIND_MUT).
static VARIANT_DESTRUCTOR(LilyAstDataType, mut, LilyAstDataType *self);

// Free LilyAstDataType type (LILY_AST_DATA_TYPE_KIND_OPTIONAL).
static VARIANT_DESTRUCTOR(LilyAstDataType, optional, LilyAstDataType *self);

// Free LilyAstDataType type (LILY_AST_DATA_TYPE_KIND_PTR).
static VARIANT_DESTRUCTOR(LilyAstDataType, ptr, LilyAstDataType *self);

// Free LilyAstDataType type (LILY_AST_DATA_TYPE_KIND_REF).
static VARIANT_DESTRUCTOR(LilyAstDataType, ref, LilyAstDataType *self);

// Free LilyAstDataType type (LILY_AST_DATA_TYPE_KIND_TRACE).
static VARIANT_DESTRUCTOR(LilyAstDataType, trace, LilyAstDataType *self);

// Free LilyAstDataType type (LILY_AST_DATA_TYPE_KIND_TUPLE).
static VARIANT_DESTRUCTOR(LilyAstDataType, tuple, LilyAstDataType *self);

DESTRUCTOR(LilyAstDataTypeLambda, const LilyAstDataTypeLambda *self)
{
    FREE_BUFFER_ITEMS(self->params->buffer, self->params->len, LilyAstDataType);
    FREE(Vec, self->params);
    FREE(LilyAstDataType, self->ret_type);
}

DESTRUCTOR(LilyAstDataTypeArray, const LilyAstDataTypeArray *self)
{
    FREE(LilyAstDataType, self->data_type);
}

DESTRUCTOR(LilyAstDataTypeCustom, const LilyAstDataTypeCustom *self)
{
    FREE_BUFFER_ITEMS(
      self->generics->buffer, self->generics->len, LilyAstDataType);
    FREE(Vec, self->generics);
    FREE(String, self->name);
}

CONSTRUCTOR(LilyAstDataType *,
            LilyAstDataType,
            enum LilyAstDataTypeKind kind,
            Location location)
{
    LilyAstDataType *self = lily_malloc(sizeof(LilyAstDataType));

    self->kind = kind;
    self->location = location;

    return self;
}

VARIANT_CONSTRUCTOR(LilyAstDataType *,
                    LilyAstDataType,
                    array,
                    Location location,
                    LilyAstDataTypeArray array)
{
    LilyAstDataType *self = lily_malloc(sizeof(LilyAstDataType));

    self->kind = LILY_AST_DATA_TYPE_KIND_ARRAY;
    self->location = location;
    self->array = array;

    return self;
}

VARIANT_CONSTRUCTOR(LilyAstDataType *,
                    LilyAstDataType,
                    custom,
                    Location location,
                    LilyAstDataTypeCustom custom)
{
    LilyAstDataType *self = lily_malloc(sizeof(LilyAstDataType));

    self->kind = LILY_AST_DATA_TYPE_KIND_CUSTOM;
    self->location = location;
    self->custom = custom;

    return self;
}

VARIANT_CONSTRUCTOR(LilyAstDataType *,
                    LilyAstDataType,
                    exception,
                    Location location,
                    LilyAstDataType *exception)
{
    LilyAstDataType *self = lily_malloc(sizeof(LilyAstDataType));

    self->kind = LILY_AST_DATA_TYPE_KIND_EXCEPTION;
    self->location = location;
    self->exception = exception;

    return self;
}

VARIANT_CONSTRUCTOR(LilyAstDataType *,
                    LilyAstDataType,
                    lambda,
                    Location location,
                    LilyAstDataTypeLambda lambda)
{
    LilyAstDataType *self = lily_malloc(sizeof(LilyAstDataType));

    self->kind = LILY_AST_DATA_TYPE_KIND_LAMBDA;
    self->location = location;
    self->lambda = lambda;

    return self;
}

VARIANT_CONSTRUCTOR(LilyAstDataType *,
                    LilyAstDataType,
                    mut,
                    Location location,
                    LilyAstDataType *mut)
{
    LilyAstDataType *self = lily_malloc(sizeof(LilyAstDataType));

    self->kind = LILY_AST_DATA_TYPE_KIND_MUT;
    self->location = location;
    self->mut = mut;

    return self;
}

VARIANT_CONSTRUCTOR(LilyAstDataType *,
                    LilyAstDataType,
                    optional,
                    Location location,
                    LilyAstDataType *optional)
{
    LilyAstDataType *self = lily_malloc(sizeof(LilyAstDataType));

    self->kind = LILY_AST_DATA_TYPE_KIND_OPTIONAL;
    self->location = location;
    self->optional = optional;

    return self;
}

VARIANT_CONSTRUCTOR(LilyAstDataType *,
                    LilyAstDataType,
                    ptr,
                    Location location,
                    LilyAstDataType *ptr)
{
    LilyAstDataType *self = lily_malloc(sizeof(LilyAstDataType));

    self->kind = LILY_AST_DATA_TYPE_KIND_PTR;
    self->location = location;
    self->ptr = ptr;

    return self;
}

VARIANT_CONSTRUCTOR(LilyAstDataType *,
                    LilyAstDataType,
                    ref,
                    Location location,
                    LilyAstDataType *ref)
{
    LilyAstDataType *self = lily_malloc(sizeof(LilyAstDataType));

    self->kind = LILY_AST_DATA_TYPE_KIND_REF;
    self->location = location;
    self->ref = ref;

    return self;
}

VARIANT_CONSTRUCTOR(LilyAstDataType *,
                    LilyAstDataType,
                    trace,
                    Location location,
                    LilyAstDataType *trace)
{
    LilyAstDataType *self = lily_malloc(sizeof(LilyAstDataType));

    self->kind = LILY_AST_DATA_TYPE_KIND_TRACE;
    self->location = location;
    self->trace = trace;

    return self;
}

VARIANT_CONSTRUCTOR(LilyAstDataType *,
                    LilyAstDataType,
                    tuple,
                    Location location,
                    Vec *tuple)
{
    LilyAstDataType *self = lily_malloc(sizeof(LilyAstDataType));

    self->kind = LILY_AST_DATA_TYPE_KIND_TUPLE;
    self->location = location;
    self->tuple = tuple;

    return self;
}

VARIANT_DESTRUCTOR(LilyAstDataType, array, LilyAstDataType *self)
{
    FREE(LilyAstDataTypeArray, &self->array);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyAstDataType, custom, LilyAstDataType *self)
{
    FREE(LilyAstDataTypeCustom, &self->custom);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyAstDataType, exception, LilyAstDataType *self)
{
    FREE(LilyAstDataType, self->exception);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyAstDataType, lambda, LilyAstDataType *self)
{
    FREE(LilyAstDataTypeLambda, &self->lambda);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyAstDataType, mut, LilyAstDataType *self)
{
    FREE(LilyAstDataType, self->mut);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyAstDataType, optional, LilyAstDataType *self)
{
    FREE(LilyAstDataType, self->optional);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyAstDataType, ptr, LilyAstDataType *self)
{
    FREE(LilyAstDataType, self->ptr);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyAstDataType, ref, LilyAstDataType *self)
{
    FREE(LilyAstDataType, self->ref);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyAstDataType, trace, LilyAstDataType *self)
{
    FREE(LilyAstDataType, self->trace);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyAstDataType, tuple, LilyAstDataType *self)
{
    FREE_BUFFER_ITEMS(self->tuple->buffer, self->tuple->len, LilyAstDataType);
    FREE(Vec, self->tuple);
    lily_free(self);
}

DESTRUCTOR(LilyAstDataType, LilyAstDataType *self)
{
    switch (self->kind) {
        case LILY_AST_DATA_TYPE_KIND_ARRAY:
            FREE_VARIANT(LilyAstDataType, array, self);
            break;
        case LILY_AST_DATA_TYPE_KIND_CUSTOM:
            FREE_VARIANT(LilyAstDataType, custom, self);
            break;
        case LILY_AST_DATA_TYPE_KIND_EXCEPTION:
            FREE_VARIANT(LilyAstDataType, exception, self);
            break;
        case LILY_AST_DATA_TYPE_KIND_LAMBDA:
            FREE_VARIANT(LilyAstDataType, lambda, self);
            break;
        case LILY_AST_DATA_TYPE_KIND_MUT:
            FREE_VARIANT(LilyAstDataType, mut, self);
            break;
        case LILY_AST_DATA_TYPE_KIND_OPTIONAL:
            FREE_VARIANT(LilyAstDataType, optional, self);
            break;
        case LILY_AST_DATA_TYPE_KIND_PTR:
            FREE_VARIANT(LilyAstDataType, ptr, self);
            break;
        case LILY_AST_DATA_TYPE_KIND_REF:
            FREE_VARIANT(LilyAstDataType, ref, self);
            break;
        case LILY_AST_DATA_TYPE_KIND_TRACE:
            FREE_VARIANT(LilyAstDataType, trace, self);
            break;
        case LILY_AST_DATA_TYPE_KIND_TUPLE:
            FREE_VARIANT(LilyAstDataType, tuple, self);
            break;
        default:
            free(self);
    }
}