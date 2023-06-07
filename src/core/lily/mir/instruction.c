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

#include <core/lily/mir/instruction.h>

static VARIANT_DESTRUCTOR(LilyMirInstructionVal,
                          array,
                          LilyMirInstructionVal *self);

static VARIANT_DESTRUCTOR(LilyMirInstructionVal,
                          exception,
                          LilyMirInstructionVal *self);

static VARIANT_DESTRUCTOR(LilyMirInstructionVal,
                          struct,
                          LilyMirInstructionVal *self);

static VARIANT_DESTRUCTOR(LilyMirInstructionVal,
                          tuple,
                          LilyMirInstructionVal *self);

CONSTRUCTOR(LilyMirInstructionVal *,
            LilyMirInstructionVal,
            enum LilyMirInstructionValKind kind,
            LilyMirDt *dt)
{
    LilyMirInstructionVal *self = lily_malloc(sizeof(LilyMirInstructionVal));

    self->kind = kind;
    self->dt = dt;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstructionVal *,
                    LilyMirInstructionVal,
                    array,
                    LilyMirDt *dt,
                    Vec *array)
{
    LilyMirInstructionVal *self = lily_malloc(sizeof(LilyMirInstructionVal));

    self->kind = LILY_MIR_INSTRUCTION_VAL_KIND_ARRAY;
    self->dt = dt;
    self->array = array;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstructionVal *,
                    LilyMirInstructionVal,
                    bytes,
                    LilyMirDt *dt,
                    const Uint8 *bytes)
{
    LilyMirInstructionVal *self = lily_malloc(sizeof(LilyMirInstructionVal));

    self->kind = LILY_MIR_INSTRUCTION_VAL_KIND_BYTES;
    self->dt = dt;
    self->bytes = bytes;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstructionVal *,
                    LilyMirInstructionVal,
                    exception,
                    LilyMirDt *dt,
                    struct LilyMirInstructionVal *ok,
                    struct LilyMirInstructionVal *err)
{
    LilyMirInstructionVal *self = lily_malloc(sizeof(LilyMirInstructionVal));

    self->kind = LILY_MIR_INSTRUCTION_VAL_KIND_EXCEPTION;
    self->dt = dt;
    self->exception[0] = ok;
    self->exception[1] = err;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstructionVal *,
                    LilyMirInstructionVal,
                    int,
                    LilyMirDt *dt,
                    Int64 int_)
{
    LilyMirInstructionVal *self = lily_malloc(sizeof(LilyMirInstructionVal));

    self->kind = LILY_MIR_INSTRUCTION_VAL_KIND_INT;
    self->dt = dt;
    self->int_ = int_;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstructionVal *,
                    LilyMirInstructionVal,
                    float,
                    LilyMirDt *dt,
                    Float64 float_)
{
    LilyMirInstructionVal *self = lily_malloc(sizeof(LilyMirInstructionVal));

    self->kind = LILY_MIR_INSTRUCTION_VAL_KIND_FLOAT;
    self->dt = dt;
    self->float_ = float_;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstructionVal *,
                    LilyMirInstructionVal,
                    reg,
                    LilyMirDt *dt,
                    const char *reg)
{
    LilyMirInstructionVal *self = lily_malloc(sizeof(LilyMirInstructionVal));

    self->kind = LILY_MIR_INSTRUCTION_VAL_KIND_REG;
    self->dt = dt;
    self->reg = reg;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstructionVal *,
                    LilyMirInstructionVal,
                    str,
                    LilyMirDt *dt,
                    const char *str)
{
    LilyMirInstructionVal *self = lily_malloc(sizeof(LilyMirInstructionVal));

    self->kind = LILY_MIR_INSTRUCTION_VAL_KIND_STR;
    self->dt = dt;
    self->str = str;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstructionVal *,
                    LilyMirInstructionVal,
                    struct,
                    LilyMirDt *dt,
                    Vec *struct_)
{
    LilyMirInstructionVal *self = lily_malloc(sizeof(LilyMirInstructionVal));

    self->kind = LILY_MIR_INSTRUCTION_VAL_KIND_STRUCT;
    self->dt = dt;
    self->struct_ = struct_;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstructionVal *,
                    LilyMirInstructionVal,
                    tuple,
                    LilyMirDt *dt,
                    Vec *tuple)
{
    LilyMirInstructionVal *self = lily_malloc(sizeof(LilyMirInstructionVal));

    self->kind = LILY_MIR_INSTRUCTION_VAL_KIND_TUPLE;
    self->dt = dt;
    self->tuple = tuple;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstructionVal *,
                    LilyMirInstructionVal,
                    uint,
                    LilyMirDt *dt,
                    Uint64 uint)
{
    LilyMirInstructionVal *self = lily_malloc(sizeof(LilyMirInstructionVal));

    self->kind = LILY_MIR_INSTRUCTION_VAL_KIND_UINT;
    self->dt = dt;
    self->uint = uint;

    return self;
}

VARIANT_DESTRUCTOR(LilyMirInstructionVal, array, LilyMirInstructionVal *self)
{
    FREE(LilyMirDt, self->dt);
    FREE_BUFFER_ITEMS(
      self->array->buffer, self->array->len, LilyMirInstructionVal);
    FREE(Vec, self->array);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstructionVal,
                   exception,
                   LilyMirInstructionVal *self)
{
    FREE(LilyMirDt, self->dt);
    FREE(LilyMirInstructionVal, self->exception[0]);
    FREE(LilyMirInstructionVal, self->exception[1]);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstructionVal, struct, LilyMirInstructionVal *self)
{
    FREE_BUFFER_ITEMS(
      self->struct_->buffer, self->struct_->len, LilyMirInstructionVal);
    FREE(Vec, self->struct_);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstructionVal, tuple, LilyMirInstructionVal *self)
{
    FREE_BUFFER_ITEMS(
      self->tuple->buffer, self->tuple->len, LilyMirInstructionVal);
    FREE(Vec, self->tuple);
    lily_free(self);
}

DESTRUCTOR(LilyMirInstructionVal, LilyMirInstructionVal *self)
{
    switch (self->kind) {
        case LILY_MIR_INSTRUCTION_VAL_KIND_ARRAY:
            FREE_VARIANT(LilyMirInstructionVal, array, self);
            break;
        case LILY_MIR_INSTRUCTION_VAL_KIND_EXCEPTION:
            FREE_VARIANT(LilyMirInstructionVal, exception, self);
            break;
        case LILY_MIR_INSTRUCTION_VAL_KIND_STRUCT:
            FREE_VARIANT(LilyMirInstructionVal, struct, self);
            break;
        case LILY_MIR_INSTRUCTION_VAL_KIND_TUPLE:
            FREE_VARIANT(LilyMirInstructionVal, tuple, self);
            break;
        default:
            FREE(LilyMirDt, self->dt);
            lily_free(self);
    }
}
