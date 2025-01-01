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

#include <base/format.h>
#include <base/string.h>

#include <core/lily/mir/instruction.h>
#include <core/lily/mir/mir.h>

#include <stdio.h>
#include <stdlib.h>

static VARIANT_DESTRUCTOR(LilyMirInstructionVal,
                          array,
                          LilyMirInstructionVal *self);

static VARIANT_DESTRUCTOR(LilyMirInstructionVal,
                          exception,
                          LilyMirInstructionVal *self);

static VARIANT_DESTRUCTOR(LilyMirInstructionVal,
                          reg,
                          LilyMirInstructionVal *self);

static VARIANT_DESTRUCTOR(LilyMirInstructionVal,
                          struct,
                          LilyMirInstructionVal *self);

static VARIANT_DESTRUCTOR(LilyMirInstructionVal,
                          tuple,
                          LilyMirInstructionVal *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction, alloc, LilyMirInstruction *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction, arg, LilyMirInstruction *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction, asm, LilyMirInstruction *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction,
                          bitcast,
                          LilyMirInstruction *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction, bitand, LilyMirInstruction *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction, bitnot, LilyMirInstruction *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction, bitor, LilyMirInstruction *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction, block, LilyMirInstruction *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction,
                          builtin_call,
                          LilyMirInstruction *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction, call, LilyMirInstruction *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction, const, LilyMirInstruction *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction, drop, LilyMirInstruction *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction, exp, LilyMirInstruction *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction, fadd, LilyMirInstruction *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction,
                          fcmp_eq,
                          LilyMirInstruction *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction,
                          fcmp_ne,
                          LilyMirInstruction *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction,
                          fcmp_le,
                          LilyMirInstruction *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction,
                          fcmp_lt,
                          LilyMirInstruction *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction,
                          fcmp_ge,
                          LilyMirInstruction *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction,
                          fcmp_gt,
                          LilyMirInstruction *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction, fdiv, LilyMirInstruction *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction, fmul, LilyMirInstruction *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction, fneg, LilyMirInstruction *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction, frem, LilyMirInstruction *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction, fsub, LilyMirInstruction *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction, fun, LilyMirInstruction *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction,
                          fun_prototype,
                          LilyMirInstruction *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction,
                          getarray,
                          LilyMirInstruction *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction, getarg, LilyMirInstruction *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction,
                          getfield,
                          LilyMirInstruction *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction,
                          getlist,
                          LilyMirInstruction *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction, getptr, LilyMirInstruction *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction,
                          getslice,
                          LilyMirInstruction *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction, iadd, LilyMirInstruction *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction,
                          icmp_eq,
                          LilyMirInstruction *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction,
                          icmp_ne,
                          LilyMirInstruction *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction,
                          icmp_le,
                          LilyMirInstruction *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction,
                          icmp_lt,
                          LilyMirInstruction *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction,
                          icmp_ge,
                          LilyMirInstruction *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction,
                          icmp_gt,
                          LilyMirInstruction *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction, idiv, LilyMirInstruction *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction, imul, LilyMirInstruction *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction,
                          inctrace,
                          LilyMirInstruction *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction, ineg, LilyMirInstruction *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction, irem, LilyMirInstruction *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction, isok, LilyMirInstruction *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction, iserr, LilyMirInstruction *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction, isub, LilyMirInstruction *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction, jmp, LilyMirInstruction *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction,
                          jmpcond,
                          LilyMirInstruction *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction, len, LilyMirInstruction *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction, load, LilyMirInstruction *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction,
                          makeref,
                          LilyMirInstruction *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction,
                          makeopt,
                          LilyMirInstruction *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction,
                          non_nil,
                          LilyMirInstruction *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction, not, LilyMirInstruction *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction, reg, LilyMirInstruction *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction,
                          ref_ptr,
                          LilyMirInstruction *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction, ret, LilyMirInstruction *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction, shl, LilyMirInstruction *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction, shr, LilyMirInstruction *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction, store, LilyMirInstruction *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction, struct, LilyMirInstruction *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction, switch, LilyMirInstruction *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction,
                          sys_call,
                          LilyMirInstruction *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction, trunc, LilyMirInstruction *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction, try, LilyMirInstruction *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction,
                          try_ptr,
                          LilyMirInstruction *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction,
                          unreachable,
                          LilyMirInstruction *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction, val, LilyMirInstruction *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction, var, LilyMirInstruction *self);

static VARIANT_DESTRUCTOR(LilyMirInstruction, xor, LilyMirInstruction *self);

#ifdef ENV_DEBUG
static threadlocal Usize tab_count = 0;
#endif

CONSTRUCTOR(LilyMirInstructionVal *,
            LilyMirInstructionVal,
            enum LilyMirInstructionValKind kind,
            LilyMirDt *dt)
{
    LilyMirInstructionVal *self = lily_malloc(sizeof(LilyMirInstructionVal));

    self->kind = kind;
    self->dt = dt;
    self->ref_count = 0;

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
    self->ref_count = 0;
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
    self->ref_count = 0;
    self->bytes = bytes;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstructionVal *,
                    LilyMirInstructionVal,
                    const,
                    LilyMirDt *dt,
                    const char *const_)
{
    LilyMirInstructionVal *self = lily_malloc(sizeof(LilyMirInstructionVal));

    self->kind = LILY_MIR_INSTRUCTION_VAL_KIND_CONST;
    self->dt = NEW_VARIANT(LilyMirDt, ptr, dt);
    self->ref_count = 0;
    self->const_ = const_;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstructionVal *,
                    LilyMirInstructionVal,
                    cstr,
                    LilyMirDt *dt,
                    const char *cstr)
{
    LilyMirInstructionVal *self = lily_malloc(sizeof(LilyMirInstructionVal));

    self->kind = LILY_MIR_INSTRUCTION_VAL_KIND_CSTR;
    self->dt = dt;
    self->ref_count = 0;
    self->cstr = cstr;

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
    self->ref_count = 0;
    self->exception[0] = ok;
    self->exception[1] = err;

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
    self->ref_count = 0;
    self->float_ = float_;

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
    self->ref_count = 0;
    self->int_ = int_;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstructionVal *,
                    LilyMirInstructionVal,
                    list,
                    LilyMirDt *dt,
                    Vec *list)
{
    LilyMirInstructionVal *self = lily_malloc(sizeof(LilyMirInstructionVal));

    self->kind = LILY_MIR_INSTRUCTION_VAL_KIND_LIST;
    self->dt = dt;
    self->ref_count = 0;
    self->list = list;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstructionVal *,
                    LilyMirInstructionVal,
                    param,
                    LilyMirDt *dt,
                    Usize param)
{
    LilyMirInstructionVal *self = lily_malloc(sizeof(LilyMirInstructionVal));

    self->kind = LILY_MIR_INSTRUCTION_VAL_KIND_PARAM;
    self->dt = dt;
    self->ref_count = 0;
    self->param = param;

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
    self->ref_count = 0;
    self->reg = reg;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstructionVal *,
                    LilyMirInstructionVal,
                    slice,
                    LilyMirDt *dt,
                    Vec *slice)
{
    LilyMirInstructionVal *self = lily_malloc(sizeof(LilyMirInstructionVal));

    self->kind = LILY_MIR_INSTRUCTION_VAL_KIND_SLICE;
    self->dt = dt;
    self->ref_count = 0;
    self->slice = slice;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstructionVal *,
                    LilyMirInstructionVal,
                    str,
                    LilyMirDt *dt,
                    String *str)
{
    LilyMirInstructionVal *self = lily_malloc(sizeof(LilyMirInstructionVal));

    self->kind = LILY_MIR_INSTRUCTION_VAL_KIND_STR;
    self->dt = dt;
    self->ref_count = 0;
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
    self->ref_count = 0;
    self->struct_ = struct_;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstructionVal *,
                    LilyMirInstructionVal,
                    trace,
                    LilyMirDt *dt,
                    LilyMirInstructionVal *trace)
{
    LilyMirInstructionVal *self = lily_malloc(sizeof(LilyMirInstructionVal));

    self->kind = LILY_MIR_INSTRUCTION_VAL_KIND_TRACE;
    self->dt = dt;
    self->ref_count = 0;
    self->trace = trace;

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
    self->ref_count = 0;
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
    self->ref_count = 0;
    self->uint = uint;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstructionVal *,
                    LilyMirInstructionVal,
                    var,
                    LilyMirDt *dt,
                    char *var)
{
    LilyMirInstructionVal *self = lily_malloc(sizeof(LilyMirInstructionVal));

    self->kind = LILY_MIR_INSTRUCTION_VAL_KIND_VAR;
    self->dt = NEW_VARIANT(LilyMirDt, ptr, dt);
    self->ref_count = 0;
    self->var = var;

    return self;
}

bool
eq__LilyMirInstructionVal(const LilyMirInstructionVal *self,
                          const LilyMirInstructionVal *other)
{
    if (self->kind != other->kind || !eq__LilyMirDt(self->dt, other->dt)) {
        return false;
    }

    switch (self->kind) {
        case LILY_MIR_INSTRUCTION_VAL_KIND_ARRAY: {
            if (self->array->len != other->array->len) {
                return false;
            }

            for (Usize i = 0; i < self->array->len; ++i) {
                if (!eq__LilyMirInstructionVal(get__Vec(self->array, i),
                                               get__Vec(other->array, i))) {
                    return false;
                }
            }

            return true;
        }
        case LILY_MIR_INSTRUCTION_VAL_KIND_BYTES:
            return !strcmp((const char *)self->bytes,
                           (const char *)other->bytes);
        case LILY_MIR_INSTRUCTION_VAL_KIND_CONST:
            return !strcmp(self->const_, other->const_);
        case LILY_MIR_INSTRUCTION_VAL_KIND_CSTR:
            return !strcmp(self->cstr, other->cstr);
        case LILY_MIR_INSTRUCTION_VAL_KIND_EXCEPTION:
            if (self->exception[0] && other->exception[0]) {
                return eq__LilyMirInstructionVal(self->exception[0],
                                                 other->exception[0]);
            } else if (self->exception[1] && other->exception[1]) {
                return eq__LilyMirInstructionVal(self->exception[1],
                                                 other->exception[1]);
            }

            return false;
        case LILY_MIR_INSTRUCTION_VAL_KIND_FLOAT:
            return self->float_ == other->float_;
        case LILY_MIR_INSTRUCTION_VAL_KIND_INT:
            return self->int_ == other->int_;
        case LILY_MIR_INSTRUCTION_VAL_KIND_LIST:
            if (self->list->len != other->list->len) {
                return false;
            }

            for (Usize i = 0; i < self->list->len; ++i) {
                if (!eq__LilyMirInstructionVal(get__Vec(self->list, i),
                                               get__Vec(other->list, i))) {
                    return false;
                }
            }

            return true;
        case LILY_MIR_INSTRUCTION_VAL_KIND_NIL:
        case LILY_MIR_INSTRUCTION_VAL_KIND_UNDEF:
        case LILY_MIR_INSTRUCTION_VAL_KIND_UNIT:
            return true;
        case LILY_MIR_INSTRUCTION_VAL_KIND_PARAM:
            return self->param == other->param;
        case LILY_MIR_INSTRUCTION_VAL_KIND_REG:
            return !strcmp(self->reg, other->reg);
        case LILY_MIR_INSTRUCTION_VAL_KIND_SLICE:
            if (self->slice->len != other->slice->len) {
                return false;
            }

            for (Usize i = 0; i < self->slice->len; ++i) {
                if (!eq__LilyMirInstructionVal(get__Vec(self->slice, i),
                                               get__Vec(other->slice, i))) {
                    return false;
                }
            }

            return true;
        case LILY_MIR_INSTRUCTION_VAL_KIND_STR:
            return !strcmp(self->str->buffer, other->str->buffer);
        case LILY_MIR_INSTRUCTION_VAL_KIND_STRUCT:
            if (self->struct_->len != other->struct_->len) {
                return false;
            }

            for (Usize i = 0; i < self->struct_->len; ++i) {
                if (!eq__LilyMirInstructionVal(get__Vec(self->struct_, i),
                                               get__Vec(other->struct_, i))) {
                    return false;
                }
            }

            return true;
        case LILY_MIR_INSTRUCTION_VAL_KIND_TRACE:
            return eq__LilyMirInstructionVal(self->trace, other->trace);
        case LILY_MIR_INSTRUCTION_VAL_KIND_TUPLE:
            if (self->tuple->len != other->tuple->len) {
                return false;
            }

            for (Usize i = 0; i < self->tuple->len; ++i) {
                if (!eq__LilyMirInstructionVal(get__Vec(self->tuple, i),
                                               get__Vec(other->tuple, i))) {
                    return false;
                }
            }

            return true;
        case LILY_MIR_INSTRUCTION_VAL_KIND_UINT:
            return self->uint == other->uint;
        case LILY_MIR_INSTRUCTION_VAL_KIND_VAR:
            return !strcmp(self->var, other->var);
        default:
            UNREACHABLE("unknown variant");
    }
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyMirInstructionVal,
               const LilyMirInstructionVal *self)
{
    String *res = format__String("\x1b[31mval\x1b[0m({Sr}) ",
                                 to_string__Debug__LilyMirDt(self->dt));

    switch (self->kind) {
        case LILY_MIR_INSTRUCTION_VAL_KIND_ARRAY:
            push_str__String(res, "[");

            for (Usize i = 0; i < self->array->len; ++i) {
                String *item = to_string__Debug__LilyMirInstructionVal(
                  get__Vec(self->array, i));

                APPEND_AND_FREE(res, item);
            }

            push_str__String(res, "]");

            return res;
        case LILY_MIR_INSTRUCTION_VAL_KIND_BYTES:
            push_str__String(res, "b\"");
            push_str__String(res, (char *)self->bytes);
            push_str__String(res, "\"");

            return res;
        case LILY_MIR_INSTRUCTION_VAL_KIND_CONST:
            push__String(res, '@');
            push_str__String(res, (char *)self->const_);

            return res;
        case LILY_MIR_INSTRUCTION_VAL_KIND_CSTR:
            push_str__String(res, "c\"");
            push_str__String(res, (char *)self->cstr);
            push_str__String(res, "\"");

            return res;
        case LILY_MIR_INSTRUCTION_VAL_KIND_EXCEPTION:
            if (self->exception[0]) {
                push_str__String(res, "\x1b[33m@Ok\x1b[0m(");

                String *ok_string =
                  to_string__Debug__LilyMirInstructionVal(self->exception[0]);
                APPEND_AND_FREE(res, ok_string);

                push_str__String(res, ")");
            } else {
                push_str__String(res, "\x1b[33m@Err\x1b[0m(");

                String *err_string =
                  to_string__Debug__LilyMirInstructionVal(self->exception[1]);
                APPEND_AND_FREE(res, err_string);

                push_str__String(res, ")");
            }

            return res;
        case LILY_MIR_INSTRUCTION_VAL_KIND_FLOAT: {
            char *s = format("\x1b[33m{f}\x1b[0m", self->float_);

            PUSH_STR_AND_FREE(res, s);

            return res;
        }
        case LILY_MIR_INSTRUCTION_VAL_KIND_INT: {
            char *s = format("\x1b[33m{d}\x1b[0m", self->int_);

            PUSH_STR_AND_FREE(res, s);

            return res;
        }
        case LILY_MIR_INSTRUCTION_VAL_KIND_LIST:
            push_str__String(res, "{");

            for (Usize i = 0; i < self->list->len; ++i) {
                String *item = to_string__Debug__LilyMirInstructionVal(
                  get__Vec(self->list, i));

                APPEND_AND_FREE(res, item);
            }

            push_str__String(res, "}");

            return res;
        case LILY_MIR_INSTRUCTION_VAL_KIND_NIL:
            push_str__String(res, "\x1b[33mnil\x1b[0m");

            return res;
        case LILY_MIR_INSTRUCTION_VAL_KIND_PARAM: {
            char *s = format("\x1b[33m${d}\x1b[0m", self->param);

            PUSH_STR_AND_FREE(res, s);

            return res;
        }
        case LILY_MIR_INSTRUCTION_VAL_KIND_REG: {
            char *s = format("\x1b[33m%{s}\x1b[0m", self->reg);

            PUSH_STR_AND_FREE(res, s);

            return res;
        }
        case LILY_MIR_INSTRUCTION_VAL_KIND_SLICE:
            push_str__String(res, "&[");

            for (Usize i = 0; i < self->slice->len; ++i) {
                String *item = to_string__Debug__LilyMirInstructionVal(
                  get__Vec(self->slice, i));

                APPEND_AND_FREE(res, item);
            }

            push_str__String(res, "]");

            return res;
        case LILY_MIR_INSTRUCTION_VAL_KIND_STR: {
            char *s = format(
              "\x1b[33mStr(\"{S}\", {d})\x1b[0m", self->str, self->str->len);

            PUSH_STR_AND_FREE(res, s);

            return res;
        }
        case LILY_MIR_INSTRUCTION_VAL_KIND_STRUCT:
            push_str__String(res, "\x1b[33mstruct\x1b[0m {");

            for (Usize i = 0; i < self->struct_->len; ++i) {
                char *item = format("{d} = {Sr}",
                                    i,
                                    to_string__Debug__LilyMirInstructionVal(
                                      get__Vec(self->struct_, i)));

                PUSH_STR_AND_FREE(res, item);

                if (i + 1 != self->struct_->len) {
                    push_str__String(res, ", ");
                }
            }

            push_str__String(res, "}");

            return res;
        case LILY_MIR_INSTRUCTION_VAL_KIND_TRACE: {
            char *s =
              format("\x1b[33mtrace\x1b[0m {Sr}",
                     to_string__Debug__LilyMirInstructionVal(self->trace));

            PUSH_STR_AND_FREE(res, s);

            return res;
        }
        case LILY_MIR_INSTRUCTION_VAL_KIND_TUPLE:
            push_str__String(res, "(");

            for (Usize i = 0; i < self->tuple->len; ++i) {
                char *item = format("{Sr}",
                                    to_string__Debug__LilyMirInstructionVal(
                                      get__Vec(self->tuple, i)));

                PUSH_STR_AND_FREE(res, item);

                if (i + 1 != self->tuple->len) {
                    push_str__String(res, ", ");
                }
            }

            push_str__String(res, ")");

            return res;
        case LILY_MIR_INSTRUCTION_VAL_KIND_UINT: {
            char *s = format("\x1b[33m{d}\x1b[0m", self->uint);

            PUSH_STR_AND_FREE(res, s);

            return res;
        }
        case LILY_MIR_INSTRUCTION_VAL_KIND_UNDEF:
            push_str__String(res, "\x1b[33mundef\x1b[0m");

            return res;
        case LILY_MIR_INSTRUCTION_VAL_KIND_UNIT:
            push_str__String(res, "\x1b[33m()\x1b[0m");

            return res;
        case LILY_MIR_INSTRUCTION_VAL_KIND_VAR:
            push_str__String(res, "\x1b[33mvar ");
            push_str__String(res, (char *)self->var);
            push_str__String(res, "\x1b[0m");

            return res;
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

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

VARIANT_DESTRUCTOR(LilyMirInstructionVal, reg, LilyMirInstructionVal *self)
{
    FREE(LilyMirDt, self->dt);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstructionVal, struct, LilyMirInstructionVal *self)
{
    FREE(LilyMirDt, self->dt);
    FREE_BUFFER_ITEMS(
      self->struct_->buffer, self->struct_->len, LilyMirInstructionVal);
    FREE(Vec, self->struct_);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstructionVal, tuple, LilyMirInstructionVal *self)
{
    FREE(LilyMirDt, self->dt);
    FREE_BUFFER_ITEMS(
      self->tuple->buffer, self->tuple->len, LilyMirInstructionVal);
    FREE(Vec, self->tuple);
    lily_free(self);
}

DESTRUCTOR(LilyMirInstructionVal, LilyMirInstructionVal *self)
{
    if (self->ref_count > 0) {
        --self->ref_count;
        return;
    }

    switch (self->kind) {
        case LILY_MIR_INSTRUCTION_VAL_KIND_ARRAY:
            FREE_VARIANT(LilyMirInstructionVal, array, self);
            break;
        case LILY_MIR_INSTRUCTION_VAL_KIND_EXCEPTION:
            FREE_VARIANT(LilyMirInstructionVal, exception, self);
            break;
        case LILY_MIR_INSTRUCTION_VAL_KIND_REG:
            FREE_VARIANT(LilyMirInstructionVal, reg, self);
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

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyMirInstructionDestSrc,
               const LilyMirInstructionDestSrc *self)
{
    return format__String("{Sr}, {Sr}",
                          to_string__Debug__LilyMirInstructionVal(self->dest),
                          to_string__Debug__LilyMirInstructionVal(self->src));
}
#endif

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyMirInstructionSrc,
               const LilyMirInstructionSrc *self)
{
    return format__String("{Sr}",
                          to_string__Debug__LilyMirInstructionVal(self->src));
}
#endif

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyMirInstructionAlloc,
               const LilyMirInstructionAlloc *self)
{
    return format__String("\x1b[34malloc\x1b[0m {Sr}",
                          to_string__Debug__LilyMirDt(self->dt));
}
#endif

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyMirInstructionArg,
               const LilyMirInstructionArg *self)
{
    return format__String(
      "{Sr} ${d}", to_string__Debug__LilyMirDt(self->dt), self->id);
}
#endif

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyMirInstructionAsm,
               const LilyMirInstructionAsm *self)
{
    return format__String("asm({s})", self->content);
}
#endif

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyMirInstructionValDt,
               const LilyMirInstructionValDt *self)
{
    return format__String("{Sr} -> {Sr}",
                          to_string__Debug__LilyMirInstructionVal(self->val),
                          to_string__Debug__LilyMirDt(self->dt));
}
#endif

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyMirInstructionBlock,
               const LilyMirInstructionBlock *self)
{
    String *res = format__String("{s}:\n", self->name);

    ++tab_count;

    String *tab = repeat__String(" ", tab_count);

    for (Usize i = 0; i < self->insts->len; ++i) {
        String *item = format__String(
          "{S}{Sr}\n",
          tab,
          to_string__Debug__LilyMirInstruction(get__Vec(self->insts, i)));

        APPEND_AND_FREE(res, item);
    }

    FREE(String, tab);

    --tab_count;

    return res;
}
#endif

DESTRUCTOR(LilyMirInstructionBlock, const LilyMirInstructionBlock *self)
{
    FREE(LilyMirBlockLimit, self->limit);
    FREE_BUFFER_ITEMS(
      self->insts->buffer, self->insts->len, LilyMirInstruction);
    FREE(Vec, self->insts);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyMirInstructionCall,
               const LilyMirInstructionCall *self)
{
    String *res = format__String("\x1b[34mcall({Sr})\x1b[0m {s}(",
                                 to_string__Debug__LilyMirDt(self->return_dt),
                                 self->name);

    for (Usize i = 0; i < self->params->len; ++i) {
        String *param =
          to_string__Debug__LilyMirInstructionVal(get__Vec(self->params, i));

        APPEND_AND_FREE(res, param);

        if (i + 1 != self->params->len) {
            push_str__String(res, ", ");
        }
    }

    push_str__String(res, ")");

    return res;
}
#endif

DESTRUCTOR(LilyMirInstructionCall, const LilyMirInstructionCall *self)
{
    FREE(LilyMirDt, self->return_dt);
    FREE_BUFFER_ITEMS(
      self->params->buffer, self->params->len, LilyMirInstructionVal);
    FREE(Vec, self->params);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyMirInstructionConst,
               const LilyMirInstructionConst *self)
{
    return format__String("{s} \x1b[34mconst\x1b[0m {s} = {Sr}",
                          to_string__Debug__LilyMirLinkage(self->linkage),
                          self->name,
                          to_string__Debug__LilyMirInstructionVal(self->val));
}
#endif

bool
eq__LilyMirInstructionFunLoadName(const LilyMirInstructionFunLoadName *self,
                                  const LilyMirInstructionFunLoadName *other)
{
    if (self->kind != other->kind) {
        return false;
    }

    switch (self->kind) {
        case LILY_MIR_INSTRUCTION_FUN_LOAD_NAME_KIND_CONST:
            return !strcmp(self->const_, other->const_);
        case LILY_MIR_INSTRUCTION_FUN_LOAD_NAME_KIND_PARAM:
            return self->param == other->param;
        case LILY_MIR_INSTRUCTION_FUN_LOAD_NAME_KIND_REG:
            return !strcmp(self->reg, other->reg);
        case LILY_MIR_INSTRUCTION_FUN_LOAD_NAME_KIND_VAR:
            return !strcmp(self->var, other->var);
        default:
            UNREACHABLE("unknown variant");
    }
}

CONSTRUCTOR(LilyMirInstructionFunLoad *,
            LilyMirInstructionFunLoad,
            LilyMirInstructionFunLoadName value_name,
            LilyMirInstruction *inst,
            Usize block_id)
{
    LilyMirInstructionFunLoad *self =
      lily_malloc(sizeof(LilyMirInstructionFunLoad));

    self->value_name = value_name;
    self->inst = inst;
    self->block_id = block_id;

    return self;
}

CONSTRUCTOR(LilyMirInstructionFun,
            LilyMirInstructionFun,
            enum LilyMirLinkage linkage,
            const char *name,
            const char *base_name,
            Vec *args,
            HashMap *generic_params,
            LilyMirDt *return_data_type,
            LilyMirBlockLimit *limit)
{
    Stack *block_stack = NEW(Stack, 1024);
    OrderedHashMap *insts = NEW(OrderedHashMap);

    LilyMirInstruction *block =
      NEW_VARIANT(LilyMirInstruction,
                  block,
                  NEW(LilyMirInstructionBlock, "entry", limit, 0));

    insert__OrderedHashMap(insts, (char *)block->block.name, block);
    push__Stack(block_stack, &block->block);

    LilyMirScope *scope =
      NEW(LilyMirScope, LILY_MIR_SCOPE_KIND_ROOT, limit, NULL);

    return (LilyMirInstructionFun){
        .linkage = linkage,
        .name = name,
        .base_name = base_name,
        .args = args,
        .generic_params = generic_params,
        .insts = insts,
        .block_stack = block_stack,
        .return_data_type = return_data_type,
        .reg_manager = NEW(LilyMirNameManager, "r."),
        .block_manager = NEW(LilyMirNameManager, "bb"),
        .virtual_variable_manager = NEW(LilyMirNameManager, "."),
        .scope = scope,
        .root_scope = scope,
        .block_count = 1
    };
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyMirInstructionFun,
               const LilyMirInstructionFun *self)
{
    String *res =
      format__String("{s} \x1b[36mfun\x1b[0m {s}(",
                     to_string__Debug__LilyMirLinkage(self->linkage),
                     self->name);

    for (Usize i = 0; i < self->args->len; ++i) {
        String *item =
          to_string__Debug__LilyMirInstruction(get__Vec(self->args, i));

        APPEND_AND_FREE(res, item);

        if (i + 1 != self->args->len) {
            push_str__String(res, ", ");
        }
    }

    {
        char *s = format(") {Sr} {{\n",
                         to_string__Debug__LilyMirDt(self->return_data_type));

        PUSH_STR_AND_FREE(res, s);
    }

    String *tab = repeat__String(" ", tab_count);

    ++tab_count;

    {
        OrderedHashMapIter iter = NEW(OrderedHashMapIter, self->insts);
        LilyMirInstruction *inst = NULL;

        while ((inst = next__OrderedHashMapIter(&iter))) {
            String *item = format__String(
              "{Sr}\n", to_string__Debug__LilyMirInstruction(inst));

            APPEND_AND_FREE(res, item);
        }
    }

    pop__String(res);

    FREE(String, tab);

    --tab_count;

    push_str__String(res, "}");

    return res;
}
#endif

DESTRUCTOR(LilyMirInstructionFun, const LilyMirInstructionFun *self)
{
    FREE_BUFFER_ITEMS(self->args->buffer, self->args->len, LilyMirInstruction);
    FREE(Vec, self->args);
    FREE_ORD_HASHMAP_VALUES(self->insts, LilyMirInstruction);
    FREE(OrderedHashMap, self->insts);

    {
        LilyMirScope *current_scope = self->scope;

        while (current_scope) {
            LilyMirScope *parent = current_scope->parent;

            FREE(LilyMirScope, current_scope);

            current_scope = parent;
        }
    }

    FREE(Stack, self->block_stack);
    FREE(LilyMirNameManager, &self->reg_manager);
    FREE(LilyMirNameManager, &self->block_manager);
    FREE(LilyMirNameManager, &self->virtual_variable_manager);
    FREE(LilyMirDt, self->return_data_type);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyMirInstructionFunPrototype,
               const LilyMirInstructionFunPrototype *self)
{
    String *res =
      format__String("{s} \x1b[36mfun\x1b[0m {s}(",
                     to_string__Debug__LilyMirLinkage(self->linkage),
                     self->name);

    for (Usize i = 0; i < self->params->len; ++i) {
        String *param = to_string__Debug__LilyMirDt(get__Vec(self->params, i));

        APPEND_AND_FREE(res, param);

        if (i + 1 != self->params->len) {
            push_str__String(res, ", ");
        }
    }

    {
        char *s =
          format(") {Sr}", to_string__Debug__LilyMirDt(self->return_data_type));

        PUSH_STR_AND_FREE(res, s);
    }

    return res;
}
#endif

DESTRUCTOR(LilyMirInstructionFunPrototype,
           const LilyMirInstructionFunPrototype *self)
{
    FREE_BUFFER_ITEMS(self->params->buffer, self->params->len, LilyMirDt);
    FREE(Vec, self->params);
    FREE(LilyMirDt, self->return_data_type);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyMirInstructionGetArray,
               const LilyMirInstructionGetArray *self)
{
    String *res =
      format__String("\x1b[34{s} mgetarray({Sr})\x1b[0m {Sr}, [",
                     self->is_const ? "const" : "",
                     to_string__Debug__LilyMirDt(self->dt),
                     to_string__Debug__LilyMirInstructionVal(self->val));

    for (Usize i = 0; i < self->indexes->len; ++i) {
        String *index =
          to_string__Debug__LilyMirInstructionVal(get__Vec(self->indexes, i));

        APPEND_AND_FREE(res, index);
    }

    push_str__String(res, "]");

    return res;
}
#endif

DESTRUCTOR(LilyMirInstructionGetArray, const LilyMirInstructionGetArray *self)
{
    FREE(LilyMirDt, self->dt);
    FREE(LilyMirInstructionVal, self->val);
    FREE_BUFFER_ITEMS(
      self->indexes->buffer, self->indexes->len, LilyMirInstructionVal);
    FREE(Vec, self->indexes);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyMirInstructionGetField,
               const LilyMirInstructionGetField *self)
{
    String *res =
      format__String("\x1b[34mgetfield({Sr})\x1b[0m {Sr}, [",
                     to_string__Debug__LilyMirDt(self->dt),
                     to_string__Debug__LilyMirInstructionVal(self->val));

    for (Usize i = 0; i < self->indexes->len; ++i) {
        String *index =
          to_string__Debug__LilyMirInstructionVal(get__Vec(self->indexes, i));

        APPEND_AND_FREE(res, index);
    }

    push_str__String(res, "]");

    return res;
}
#endif

DESTRUCTOR(LilyMirInstructionGetField, const LilyMirInstructionGetField *self)
{
    FREE(LilyMirDt, self->dt);
    FREE(LilyMirInstructionVal, self->val);
    FREE_BUFFER_ITEMS(
      self->indexes->buffer, self->indexes->len, LilyMirInstructionVal);
    FREE(Vec, self->indexes);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyMirInstructionLoad,
               const LilyMirInstructionLoad *self)
{
    return format__String("\x1b[34mload\x1b[0m({Sr}) {Sr}",
                          to_string__Debug__LilyMirDt(self->dt),
                          to_string__Debug__LilyMirInstructionSrc(&self->src));
}
#endif

DESTRUCTOR(LilyMirInstructionLoad, const LilyMirInstructionLoad *self)
{
    FREE(LilyMirInstructionSrc, &self->src);
    FREE(LilyMirDt, self->dt);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyMirInstructionJmpCond,
               const LilyMirInstructionJmpCond *self)
{
    return format__String(
      "\x1b[34mjmpcond\x1b[0m {Sr} then \x1b[33mblock\x1b[0m {s} else "
      "\x1b[33mblock\x1b[0m {s}",
      to_string__Debug__LilyMirInstructionVal(self->cond),
      self->then_block->name,
      self->else_block->name);
}
#endif

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyMirInstructionReg,
               const LilyMirInstructionReg *self)
{
    return format__String("%{s} = {Sr}",
                          self->name,
                          to_string__Debug__LilyMirInstruction(self->inst));
}
#endif

DESTRUCTOR(LilyMirInstructionReg, const LilyMirInstructionReg *self)
{
    FREE(LilyMirInstruction, self->inst);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyMirInstructionVar,
               const LilyMirInstructionVar *self)
{
    return format__String("\x1b[35mvar\x1b[0m {s} = {Sr}",
                          self->name,
                          to_string__Debug__LilyMirInstruction(self->inst));
}
#endif

DESTRUCTOR(LilyMirInstructionVar, const LilyMirInstructionVar *self)
{
    FREE(LilyMirInstruction, self->inst);
}

CONSTRUCTOR(LilyMirInstructionSwitchCase *,
            LilyMirInstructionSwitchCase,
            LilyMirInstructionVal *val,
            LilyMirInstructionBlock *block_dest)
{
    LilyMirInstructionSwitchCase *self =
      lily_malloc(sizeof(LilyMirInstructionSwitchCase));

    self->val = val;
    self->block_dest = block_dest;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyMirInstructionSwitchCase,
               const LilyMirInstructionSwitchCase *self)
{
    return format__String("case {Sr}, block {S}",
                          to_string__Debug__LilyMirInstructionVal(self->val),
                          self->block_dest->name);
}
#endif

DESTRUCTOR(LilyMirInstructionSwitchCase, LilyMirInstructionSwitchCase *self)
{
    FREE(LilyMirInstructionVal, self->val);
    lily_free(self);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyMirInstructionSwitch,
               const LilyMirInstructionSwitch *self)
{
    String *res =
      format__String("switch {Sr}, block {S} {{\n",
                     to_string__Debug__LilyMirInstructionVal(self->val),
                     self->default_block->name);

    ++tab_count;

    String *tab = repeat__String(" ", tab_count);

    for (Usize i = 0; i < self->cases->len; ++i) {
        String *case_ =
          format__String("{S}{Sr}\n",
                         tab,
                         to_string__Debug__LilyMirInstructionSwitchCase(
                           get__Vec(self->cases, i)));

        APPEND_AND_FREE(res, case_);
    }

    --tab_count;

    push_str__String(res, "  }");

    FREE(String, tab);

    return res;
}
#endif

DESTRUCTOR(LilyMirInstructionSwitch, const LilyMirInstructionSwitch *self)
{
    FREE(LilyMirInstructionVal, self->val);
    FREE_BUFFER_ITEMS(
      self->cases->buffer, self->cases->len, LilyMirInstructionSwitchCase);
    FREE(Vec, self->cases);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyMirInstructionTry,
               const LilyMirInstructionTry *self)
{
    return format__String(
      "try {Sr}, block {s}\ncatch -> {Sr}, block {s}",
      to_string__Debug__LilyMirInstructionVal(self->val),
      self->try_block->name,
      to_string__Debug__LilyMirInstructionVal(self->catch_val),
      self->catch_block->name);
}
#endif

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyMirInstructionStruct,
               const LilyMirInstructionStruct *self)
{
    String *res =
      format__String("{s} \x1b[36mstruct\x1b[0m {s} {{",
                     to_string__Debug__LilyMirLinkage(self->linkage),
                     self->name);

    for (Usize i = 0; i < self->fields->len; ++i) {
        String *field = to_string__Debug__LilyMirDt(get__Vec(self->fields, i));

        APPEND_AND_FREE(res, field);

        if (i + 1 != self->fields->len) {
            push_str__String(res, ", ");
        }
    }

    push_str__String(res, "}");

    return res;
}
#endif

DESTRUCTOR(LilyMirInstructionStruct, const LilyMirInstructionStruct *self)
{
    FREE_BUFFER_ITEMS(self->fields->buffer, self->fields->len, LilyMirDt);
    FREE(Vec, self->fields);
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    alloc,
                    LilyMirInstructionAlloc alloc)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_ALLOC;
    self->debug_info = NULL;
    self->alloc = alloc;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    arg,
                    LilyMirInstructionArg arg)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_ARG;
    self->debug_info = NULL;
    self->arg = arg;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    asm,
                    LilyMirInstructionAsm asm)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_ASM;
    self->debug_info = NULL;
    self->asm = asm;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    bitcast,
                    LilyMirInstructionValDt bitcast)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_BITCAST;
    self->debug_info = NULL;
    self->bitcast = bitcast;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                      bitand,
                    LilyMirInstructionDestSrc bitand)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_BITAND;
    self->debug_info = NULL;
    self->bitand = bitand;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    bitnot,
                    LilyMirInstructionSrc bitnot)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_BITNOT;
    self->debug_info = NULL;
    self->bitnot = bitnot;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    bitor
                    ,
                    LilyMirInstructionDestSrc bitor)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_BITOR;
    self->debug_info = NULL;
    self->bitor = bitor ;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    block,
                    LilyMirInstructionBlock block)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_BLOCK;
    self->debug_info = NULL;
    self->block = block;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    builtin_call,
                    LilyMirInstructionCall builtin_call)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_BUILTIN_CALL;
    self->debug_info = NULL;
    self->builtin_call = builtin_call;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    call,
                    LilyMirInstructionCall call)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_CALL;
    self->debug_info = NULL;
    self->call = call;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    const,
                    LilyMirInstructionConst const_)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_CONST;
    self->debug_info = NULL;
    self->const_ = const_;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    drop,
                    LilyMirInstructionSrc drop)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_DROP;
    self->debug_info = NULL;
    self->drop = drop;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    exp,
                    LilyMirInstructionDestSrc exp)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_EXP;
    self->debug_info = NULL;
    self->exp = exp;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    fadd,
                    LilyMirInstructionDestSrc fadd)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_FADD;
    self->debug_info = NULL;
    self->fadd = fadd;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    fcmp_eq,
                    LilyMirInstructionDestSrc fcmp_eq)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_FCMP_EQ;
    self->debug_info = NULL;
    self->fcmp_eq = fcmp_eq;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    fcmp_ne,
                    LilyMirInstructionDestSrc fcmp_ne)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_FCMP_NE;
    self->debug_info = NULL;
    self->fcmp_ne = fcmp_ne;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    fcmp_le,
                    LilyMirInstructionDestSrc fcmp_le)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_FCMP_LE;
    self->debug_info = NULL;
    self->fcmp_le = fcmp_le;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    fcmp_lt,
                    LilyMirInstructionDestSrc fcmp_lt)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_FCMP_LT;
    self->debug_info = NULL;
    self->fcmp_lt = fcmp_lt;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    fcmp_ge,
                    LilyMirInstructionDestSrc fcmp_ge)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_FCMP_GE;
    self->debug_info = NULL;
    self->fcmp_ge = fcmp_ge;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    fcmp_gt,
                    LilyMirInstructionDestSrc fcmp_gt)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_FCMP_GT;
    self->debug_info = NULL;
    self->fcmp_gt = fcmp_gt;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    fdiv,
                    LilyMirInstructionDestSrc fdiv)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_FDIV;
    self->debug_info = NULL;
    self->fdiv = fdiv;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    fmul,
                    LilyMirInstructionDestSrc fmul)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_FMUL;
    self->debug_info = NULL;
    self->fmul = fmul;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    fneg,
                    LilyMirInstructionSrc fneg)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_FNEG;
    self->debug_info = NULL;
    self->fneg = fneg;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    frem,
                    LilyMirInstructionDestSrc frem)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_FREM;
    self->debug_info = NULL;
    self->frem = frem;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    fsub,
                    LilyMirInstructionDestSrc fsub)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_FSUB;
    self->debug_info = NULL;
    self->fsub = fsub;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    fun,
                    LilyMirInstructionFun fun)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_FUN;
    self->debug_info = NULL;
    self->fun = fun;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    fun_prototype,
                    LilyMirInstructionFunPrototype fun_prototype)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_FUN_PROTOTYPE;
    self->debug_info = NULL;
    self->fun_prototype = fun_prototype;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    getarray,
                    LilyMirInstructionGetArray getarray)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_GETARRAY;
    self->debug_info = NULL;
    self->getarray = getarray;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    getarg,
                    LilyMirInstructionSrc getarg)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_GETARG;
    self->debug_info = NULL;
    self->getarg = getarg;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    getfield,
                    LilyMirInstructionGetField getfield)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_GETFIELD;
    self->debug_info = NULL;
    self->getfield = getfield;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    getlist,
                    LilyMirInstructionSrc getlist)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_GETLIST;
    self->debug_info = NULL;
    self->getlist = getlist;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    getptr,
                    LilyMirInstructionSrc getptr)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_GETPTR;
    self->debug_info = NULL;
    self->getptr = getptr;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    getslice,
                    LilyMirInstructionSrc getslice)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_GETSLICE;
    self->debug_info = NULL;
    self->getslice = getslice;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    iadd,
                    LilyMirInstructionDestSrc iadd)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_IADD;
    self->debug_info = NULL;
    self->iadd = iadd;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    icmp_eq,
                    LilyMirInstructionDestSrc icmp_eq)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_ICMP_EQ;
    self->debug_info = NULL;
    self->icmp_eq = icmp_eq;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    icmp_ne,
                    LilyMirInstructionDestSrc icmp_ne)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_ICMP_NE;
    self->debug_info = NULL;
    self->icmp_ne = icmp_ne;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    icmp_le,
                    LilyMirInstructionDestSrc icmp_le)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_ICMP_LE;
    self->debug_info = NULL;
    self->icmp_le = icmp_le;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    icmp_lt,
                    LilyMirInstructionDestSrc icmp_lt)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_ICMP_LT;
    self->debug_info = NULL;
    self->icmp_lt = icmp_lt;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    icmp_ge,
                    LilyMirInstructionDestSrc icmp_ge)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_ICMP_GE;
    self->debug_info = NULL;
    self->icmp_ge = icmp_ge;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    icmp_gt,
                    LilyMirInstructionDestSrc icmp_gt)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_ICMP_GT;
    self->debug_info = NULL;
    self->icmp_gt = icmp_gt;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    idiv,
                    LilyMirInstructionDestSrc idiv)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_IDIV;
    self->debug_info = NULL;
    self->idiv = idiv;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    imul,
                    LilyMirInstructionDestSrc imul)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_IMUL;
    self->debug_info = NULL;
    self->imul = imul;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    inctrace,
                    LilyMirInstructionDestSrc inctrace)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_INCTRACE;
    self->debug_info = NULL;
    self->imul = inctrace;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    ineg,
                    LilyMirInstructionSrc ineg)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_INEG;
    self->debug_info = NULL;
    self->ineg = ineg;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    irem,
                    LilyMirInstructionDestSrc irem)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_IREM;
    self->debug_info = NULL;
    self->irem = irem;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    isok,
                    LilyMirInstructionSrc isok)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_ISOK;
    self->debug_info = NULL;
    self->isok = isok;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    iserr,
                    LilyMirInstructionSrc iserr)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_ISERR;
    self->debug_info = NULL;
    self->iserr = iserr;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    isub,
                    LilyMirInstructionDestSrc isub)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_ISUB;
    self->debug_info = NULL;
    self->isub = isub;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    jmp,
                    LilyMirInstructionBlock *jmp)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_JMP;
    self->debug_info = NULL;
    self->jmp = jmp;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    jmpcond,
                    LilyMirInstructionJmpCond jmpcond)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_JMPCOND;
    self->debug_info = NULL;
    self->jmpcond = jmpcond;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    len,
                    LilyMirInstructionSrc len)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_LEN;
    self->debug_info = NULL;
    self->len = len;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    load,
                    LilyMirInstructionLoad load)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_LOAD;
    self->debug_info = NULL;
    self->load = load;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    makeref,
                    LilyMirInstructionSrc makeref)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_MAKEREF;
    self->debug_info = NULL;
    self->makeref = makeref;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    makeopt,
                    LilyMirInstructionSrc makeopt)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_MAKEOPT;
    self->debug_info = NULL;
    self->makeopt = makeopt;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    non_nil,
                    LilyMirInstruction *non_nil)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_NON_NIL;
    self->debug_info = NULL;
    self->non_nil = non_nil;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    not,
                    LilyMirInstructionSrc not )
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_NOT;
    self->debug_info = NULL;
    self->not = not ;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    reg,
                    LilyMirInstructionReg reg)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_REG;
    self->debug_info = NULL;
    self->reg = reg;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    ref_ptr,
                    LilyMirInstructionSrc ref_ptr)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_REF_PTR;
    self->debug_info = NULL;
    self->ref_ptr = ref_ptr;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    ret,
                    LilyMirInstruction *ret)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_RET;
    self->debug_info = NULL;
    self->ret = ret;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    shl,
                    LilyMirInstructionDestSrc shl)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_SHL;
    self->debug_info = NULL;
    self->shl = shl;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    shr,
                    LilyMirInstructionDestSrc shr)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_SHR;
    self->debug_info = NULL;
    self->shr = shr;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    store,
                    LilyMirInstructionDestSrc store)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_STORE;
    self->debug_info = NULL;
    self->store = store;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    struct,
                    LilyMirInstructionStruct struct_)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_STRUCT;
    self->debug_info = NULL;
    self->struct_ = struct_;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    switch,
                    LilyMirInstructionSwitch switch_)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_SWITCH;
    self->debug_info = NULL;
    self->switch_ = switch_;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    sys_call,
                    LilyMirInstructionCall sys_call)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_SYS_CALL;
    self->debug_info = NULL;
    self->sys_call = sys_call;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    trunc,
                    LilyMirInstructionValDt trunc)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_TRUNC;
    self->debug_info = NULL;
    self->trunc = trunc;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    try,
                    LilyMirInstructionTry try)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_TRY;
    self->debug_info = NULL;
    self->try = try;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    try_ptr,
                    LilyMirInstructionTry try_ptr)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_TRY_PTR;
    self->debug_info = NULL;
    self->try_ptr = try_ptr;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *, LilyMirInstruction, unreachable)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_UNREACHABLE;
    self->debug_info = NULL;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    val,
                    LilyMirInstructionVal *val)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_VAL;
    self->debug_info = NULL;
    self->val = val;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    var,
                    LilyMirInstructionVar var)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_VAR;
    self->debug_info = NULL;
    self->var = var;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    xor,
                    LilyMirInstructionDestSrc xor)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_XOR;
    self->debug_info = NULL;
    self->xor = xor;

    return self;
}

const LilyMirInstruction *
get_arg__LilyMirInstruction(const LilyMirInstruction *self)
{
    switch (self->kind) {
        case LILY_MIR_INSTRUCTION_KIND_ARG:
            return self;
        case LILY_MIR_INSTRUCTION_KIND_NON_NIL:
            return get_arg__LilyMirInstruction(self->non_nil);
        default:
            return NULL;
    }
}

void
partial_free__LilyMirInstruction(LilyMirInstruction *self)
{
    if (self->debug_info) {
        FREE(LilyMirDebugInfo, self->debug_info);
    }

    lily_free(self);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyMirInstruction, const LilyMirInstruction *self)
{
    String *res = NULL;

    switch (self->kind) {
        case LILY_MIR_INSTRUCTION_KIND_ALLOC:
            res = format__String(
              "{Sr}", to_string__Debug__LilyMirInstructionAlloc(&self->alloc));
            break;
        case LILY_MIR_INSTRUCTION_KIND_ARG:
            res = format__String(
              "{Sr}", to_string__Debug__LilyMirInstructionArg(&self->arg));
            break;
        case LILY_MIR_INSTRUCTION_KIND_ASM:
            res = format__String(
              "{Sr}", to_string__Debug__LilyMirInstructionAsm(&self->asm));
            break;
        case LILY_MIR_INSTRUCTION_KIND_BITCAST:
            res = format__String(
              "\x1b[34mbitcast\x1b[0m {Sr}",
              to_string__Debug__LilyMirInstructionValDt(&self->bitcast));
            break;
        case LILY_MIR_INSTRUCTION_KIND_BITAND:
            res = format__String(
              "\x1b[34mbitand\x1b[0m {Sr}",
              to_string__Debug__LilyMirInstructionDestSrc(&self->bitand));
            break;
        case LILY_MIR_INSTRUCTION_KIND_BITNOT:
            res = format__String(
              "\x1b[34mbitnot\x1b[0m {Sr}",
              to_string__Debug__LilyMirInstructionSrc(&self->bitnot));
            break;
        case LILY_MIR_INSTRUCTION_KIND_BITOR:
            res = format__String(
              "\x1b[34mbitor\x1b[0m {Sr}",
              to_string__Debug__LilyMirInstructionDestSrc(&self->bitor));
            break;
        case LILY_MIR_INSTRUCTION_KIND_BLOCK:
            res = format__String(
              "{Sr}", to_string__Debug__LilyMirInstructionBlock(&self->block));
            break;
        case LILY_MIR_INSTRUCTION_KIND_BUILTIN_CALL:
            res = format__String(
              "\x1b[34mcall\x1b[0m @builtin.{Sr}",
              to_string__Debug__LilyMirInstructionCall(&self->builtin_call));
            break;
        case LILY_MIR_INSTRUCTION_KIND_CALL:
            res = to_string__Debug__LilyMirInstructionCall(&self->call);
            break;
        case LILY_MIR_INSTRUCTION_KIND_CONST:
            res = format__String(
              "{Sr}", to_string__Debug__LilyMirInstructionConst(&self->const_));
            break;
        case LILY_MIR_INSTRUCTION_KIND_DROP:
            res = format__String(
              "\x1b[34mdrop\x1b[0m {Sr}",
              to_string__Debug__LilyMirInstructionSrc(&self->drop));
            break;
        case LILY_MIR_INSTRUCTION_KIND_EXP:
            res = format__String(
              "\x1b[34mexp\x1b[0m {Sr}",
              to_string__Debug__LilyMirInstructionDestSrc(&self->exp));
            break;
        case LILY_MIR_INSTRUCTION_KIND_FADD:
            res = format__String(
              "\x1b[34mfadd\x1b[0m {Sr}",
              to_string__Debug__LilyMirInstructionDestSrc(&self->fadd));
            break;
        case LILY_MIR_INSTRUCTION_KIND_FCMP_EQ:
            res = format__String(
              "\x1b[34mfcmp eq\x1b[0m {Sr}",
              to_string__Debug__LilyMirInstructionDestSrc(&self->fcmp_eq));
            break;
        case LILY_MIR_INSTRUCTION_KIND_FCMP_NE:
            res = format__String(
              "\x1b[34mfcmp ne\x1b[0m {Sr}",
              to_string__Debug__LilyMirInstructionDestSrc(&self->fcmp_ne));
            break;
        case LILY_MIR_INSTRUCTION_KIND_FCMP_LE:
            res = format__String(
              "\x1b[34mfcmp le\x1b[0m {Sr}",
              to_string__Debug__LilyMirInstructionDestSrc(&self->fcmp_le));
            break;
        case LILY_MIR_INSTRUCTION_KIND_FCMP_LT:
            res = format__String(
              "\x1b[34mfcmp lt\x1b[0m {Sr}",
              to_string__Debug__LilyMirInstructionDestSrc(&self->fcmp_lt));
            break;
        case LILY_MIR_INSTRUCTION_KIND_FCMP_GE:
            res = format__String(
              "\x1b[34mfcmp ge\x1b[0m {Sr}",
              to_string__Debug__LilyMirInstructionDestSrc(&self->fcmp_ge));
            break;
        case LILY_MIR_INSTRUCTION_KIND_FCMP_GT:
            res = format__String(
              "\x1b[34mfcmp gt\x1b[0m {Sr}",
              to_string__Debug__LilyMirInstructionDestSrc(&self->fcmp_gt));
            break;
        case LILY_MIR_INSTRUCTION_KIND_FDIV:
            res = format__String(
              "\x1b[34mfdiv\x1b[0m {Sr}",
              to_string__Debug__LilyMirInstructionDestSrc(&self->fdiv));
            break;
        case LILY_MIR_INSTRUCTION_KIND_FMUL:
            res = format__String(
              "\x1b[34mfmul\x1b[0m {Sr}",
              to_string__Debug__LilyMirInstructionDestSrc(&self->fmul));
            break;
        case LILY_MIR_INSTRUCTION_KIND_FNEG:
            res = format__String(
              "\x1b[34mfneg\x1b[0m {Sr}",
              to_string__Debug__LilyMirInstructionSrc(&self->fneg));
            break;
        case LILY_MIR_INSTRUCTION_KIND_FREM:
            res = format__String(
              "\x1b[34mfrem\x1b[0m {Sr}",
              to_string__Debug__LilyMirInstructionDestSrc(&self->frem));
            break;
        case LILY_MIR_INSTRUCTION_KIND_FSUB:
            res = format__String(
              "\x1b[34mfsub\x1b[0m {Sr}",
              to_string__Debug__LilyMirInstructionDestSrc(&self->fsub));
            break;
        case LILY_MIR_INSTRUCTION_KIND_FUN:
            res = format__String(
              "\n{Sr}", to_string__Debug__LilyMirInstructionFun(&self->fun));
            break;
        case LILY_MIR_INSTRUCTION_KIND_FUN_PROTOTYPE:
            res =
              format__String("\n{Sr}",
                             to_string__Debug__LilyMirInstructionFunPrototype(
                               &self->fun_prototype));
            break;
        case LILY_MIR_INSTRUCTION_KIND_GETARG:
            res = format__String(
              "\x1b[34mgetarg\x1b[0m {Sr}",
              to_string__Debug__LilyMirInstructionSrc(&self->getarg));
            break;
        case LILY_MIR_INSTRUCTION_KIND_GETARRAY:
            res = to_string__Debug__LilyMirInstructionGetArray(&self->getarray);
            break;
        case LILY_MIR_INSTRUCTION_KIND_GETLIST:
            res = format__String(
              "\x1b[34mgetlist\x1b[0m {Sr}",
              to_string__Debug__LilyMirInstructionSrc(&self->getlist));
            break;
        case LILY_MIR_INSTRUCTION_KIND_GETSLICE:
            res = format__String(
              "\x1b[34mgetslice\x1b[0m {Sr}",
              to_string__Debug__LilyMirInstructionSrc(&self->getslice));
            break;
        case LILY_MIR_INSTRUCTION_KIND_GETFIELD:
            res = to_string__Debug__LilyMirInstructionGetField(&self->getfield);
            break;
        case LILY_MIR_INSTRUCTION_KIND_GETPTR:
            res = format__String(
              "\x1b[34mgetptr\x1b[0m {Sr}",
              to_string__Debug__LilyMirInstructionSrc(&self->getptr));
            break;
        case LILY_MIR_INSTRUCTION_KIND_IADD:
            res = format__String(
              "\x1b[34miadd\x1b[0m {Sr}",
              to_string__Debug__LilyMirInstructionDestSrc(&self->iadd));
            break;
        case LILY_MIR_INSTRUCTION_KIND_ICMP_EQ:
            res = format__String(
              "\x1b[34micmp eq\x1b[0m {Sr}",
              to_string__Debug__LilyMirInstructionDestSrc(&self->icmp_eq));
            break;
        case LILY_MIR_INSTRUCTION_KIND_ICMP_NE:
            res = format__String(
              "\x1b[34micmp ne\x1b[0m {Sr}",
              to_string__Debug__LilyMirInstructionDestSrc(&self->icmp_ne));
            break;
        case LILY_MIR_INSTRUCTION_KIND_ICMP_LE:
            res = format__String(
              "\x1b[34micmp le\x1b[0m {Sr}",
              to_string__Debug__LilyMirInstructionDestSrc(&self->icmp_le));
            break;
        case LILY_MIR_INSTRUCTION_KIND_ICMP_LT:
            res = format__String(
              "\x1b[34micmp lt\x1b[0m {Sr}",
              to_string__Debug__LilyMirInstructionDestSrc(&self->icmp_lt));
            break;
        case LILY_MIR_INSTRUCTION_KIND_ICMP_GE:
            res = format__String(
              "\x1b[34micmp ge\x1b[0m {Sr}",
              to_string__Debug__LilyMirInstructionDestSrc(&self->icmp_ge));
            break;
        case LILY_MIR_INSTRUCTION_KIND_ICMP_GT:
            res = format__String(
              "\x1b[34micmp gt\x1b[0m {Sr}",
              to_string__Debug__LilyMirInstructionDestSrc(&self->icmp_gt));
            break;
        case LILY_MIR_INSTRUCTION_KIND_IDIV:
            res = format__String(
              "\x1b[34midiv\x1b[0m {Sr}",
              to_string__Debug__LilyMirInstructionDestSrc(&self->idiv));
            break;
        case LILY_MIR_INSTRUCTION_KIND_IMUL:
            res = format__String(
              "\x1b[34mimul\x1b[0m {Sr}",
              to_string__Debug__LilyMirInstructionDestSrc(&self->imul));
            break;
        case LILY_MIR_INSTRUCTION_KIND_INCTRACE:
            res = format__String(
              "\x1b[34minctrace\x1b[0m {Sr}",
              to_string__Debug__LilyMirInstructionSrc(&self->inctrace));
            break;
        case LILY_MIR_INSTRUCTION_KIND_INEG:
            res = format__String(
              "\x1b[34mineg\x1b[0m {Sr}",
              to_string__Debug__LilyMirInstructionSrc(&self->ineg));
            break;
        case LILY_MIR_INSTRUCTION_KIND_IREM:
            res = format__String(
              "\x1b[34mirem\x1b[0m {Sr}",
              to_string__Debug__LilyMirInstructionDestSrc(&self->irem));
            break;
        case LILY_MIR_INSTRUCTION_KIND_ISOK:
            res = format__String(
              "\x1b[34misok\x1b[0m {Sr}",
              to_string__Debug__LilyMirInstructionSrc(&self->isok));
            break;
        case LILY_MIR_INSTRUCTION_KIND_ISERR:
            res = format__String(
              "\x1b[34miserr\x1b[0m {Sr}",
              to_string__Debug__LilyMirInstructionSrc(&self->iserr));
            break;
        case LILY_MIR_INSTRUCTION_KIND_ISUB:
            res = format__String(
              "\x1b[34misub\x1b[0m {Sr}",
              to_string__Debug__LilyMirInstructionDestSrc(&self->isub));
            break;
        case LILY_MIR_INSTRUCTION_KIND_JMP:
            res = format__String("\x1b[34mjmp\x1b[34m \x1b[33mblock\x1b[0m {s}",
                                 self->jmp->name);
            break;
        case LILY_MIR_INSTRUCTION_KIND_JMPCOND:
            res = format__String(
              "{Sr}",
              to_string__Debug__LilyMirInstructionJmpCond(&self->jmpcond));
            break;
        case LILY_MIR_INSTRUCTION_KIND_LEN:
            res = format__String(
              "\x1b[34mlen\x1b[0m {Sr}",
              to_string__Debug__LilyMirInstructionSrc(&self->len));
            break;
        case LILY_MIR_INSTRUCTION_KIND_LOAD:
            res = to_string__Debug__LilyMirInstructionLoad(&self->load);
            break;
        case LILY_MIR_INSTRUCTION_KIND_MAKEREF:
            res = format__String(
              "\x1b[34mmakeref\x1b[0m {Sr}",
              to_string__Debug__LilyMirInstructionSrc(&self->makeref));
            break;
        case LILY_MIR_INSTRUCTION_KIND_MAKEOPT:
            res = format__String(
              "\x1b[34mmakeopt\x1b[0m {Sr}",
              to_string__Debug__LilyMirInstructionSrc(&self->makeopt));
            break;
        case LILY_MIR_INSTRUCTION_KIND_NON_NIL:
            res = format__String(
              "\x1b[34mnon_nil\x1b[0m {Sr}",
              to_string__Debug__LilyMirInstruction(self->non_nil));
            break;
        case LILY_MIR_INSTRUCTION_KIND_NOT:
            res = format__String(
              "\x1b[34mnot\x1b[0m {Sr}",
              to_string__Debug__LilyMirInstructionSrc(&self->not ));
            break;
        case LILY_MIR_INSTRUCTION_KIND_REF_PTR:
            res = format__String(
              "\x1b[34mref_ptr\x1b[0m {Sr}",
              to_string__Debug__LilyMirInstructionSrc(&self->ref_ptr));
            break;
        case LILY_MIR_INSTRUCTION_KIND_REG:
            res = format__String(
              "{Sr}", to_string__Debug__LilyMirInstructionReg(&self->reg));
            break;
        case LILY_MIR_INSTRUCTION_KIND_RET:
            res =
              format__String("\x1b[34mret\x1b[0m {Sr}",
                             to_string__Debug__LilyMirInstruction(self->ret));
            break;
        case LILY_MIR_INSTRUCTION_KIND_SHL:
            res = format__String(
              "\x1b[34mshl\x1b[0m {Sr}",
              to_string__Debug__LilyMirInstructionDestSrc(&self->shl));
            break;
        case LILY_MIR_INSTRUCTION_KIND_SHR:
            res = format__String(
              "\x1b[34mshr\x1b[0m {Sr}",
              to_string__Debug__LilyMirInstructionDestSrc(&self->shr));
            break;
        case LILY_MIR_INSTRUCTION_KIND_STORE:
            res = format__String(
              "\x1b[34mstore\x1b[0m {Sr}",
              to_string__Debug__LilyMirInstructionDestSrc(&self->store));
            break;
        case LILY_MIR_INSTRUCTION_KIND_STRUCT:
            res = to_string__Debug__LilyMirInstructionStruct(&self->struct_);
            break;
        case LILY_MIR_INSTRUCTION_KIND_SWITCH:
            res = format__String(
              "{Sr}",
              to_string__Debug__LilyMirInstructionSwitch(&self->switch_));
            break;
        case LILY_MIR_INSTRUCTION_KIND_SYS_CALL:
            res = format__String(
              "\x1b[34mcall\x1b[0m @sys.{Sr}",
              to_string__Debug__LilyMirInstructionCall(&self->sys_call));
            break;
        case LILY_MIR_INSTRUCTION_KIND_TRUNC:
            res = format__String(
              "\x1b[34mtrunc\x1b[0m {Sr}",
              to_string__Debug__LilyMirInstructionValDt(&self->trunc));
            break;
        case LILY_MIR_INSTRUCTION_KIND_TRY:
            res = format__String(
              "{Sr}", to_string__Debug__LilyMirInstructionTry(&self->try));
            break;
        case LILY_MIR_INSTRUCTION_KIND_TRY_PTR:
            res = format__String(
              "{Sr}", to_string__Debug__LilyMirInstructionTry(&self->try_ptr));
            break;
        case LILY_MIR_INSTRUCTION_KIND_UNREACHABLE:
            res = from__String("\x1b[34munreachable\x1b[0m");
            break;
        case LILY_MIR_INSTRUCTION_KIND_VAL:
            res = format__String(
              "{Sr}", to_string__Debug__LilyMirInstructionVal(self->val));
            break;
        case LILY_MIR_INSTRUCTION_KIND_VAR:
            res = format__String(
              "{Sr}", to_string__Debug__LilyMirInstructionVar(&self->var));
            break;
        case LILY_MIR_INSTRUCTION_KIND_XOR:
            res = format__String(
              "\x1b[34mxor\x1b[0m {Sr}",
              to_string__Debug__LilyMirInstructionDestSrc(&self->xor));
            break;
        default:
            UNREACHABLE("unknown variant");
    }

    return res;
}
#endif

#define FREE_DEBUG_INFO(self)                     \
    if (self->debug_info) {                       \
        FREE(LilyMirDebugInfo, self->debug_info); \
    }

VARIANT_DESTRUCTOR(LilyMirInstruction, alloc, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    FREE(LilyMirInstructionAlloc, &self->alloc);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, arg, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    FREE(LilyMirInstructionArg, &self->arg);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, asm, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, bitcast, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    FREE(LilyMirInstructionValDt, &self->bitcast);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, bitand, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    FREE(LilyMirInstructionDestSrc, &self->bitand);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, bitnot, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    FREE(LilyMirInstructionSrc, &self->bitnot);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, bitor, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    FREE(LilyMirInstructionDestSrc, &self->bitor);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, block, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    FREE(LilyMirInstructionBlock, &self->block);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, builtin_call, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    FREE(LilyMirInstructionCall, &self->builtin_call);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, call, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    FREE(LilyMirInstructionCall, &self->call);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, const, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    FREE(LilyMirInstructionConst, &self->const_);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, drop, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    FREE(LilyMirInstructionSrc, &self->drop);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, exp, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    FREE(LilyMirInstructionDestSrc, &self->exp);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, fadd, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    FREE(LilyMirInstructionDestSrc, &self->fadd);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, fcmp_eq, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    FREE(LilyMirInstructionDestSrc, &self->fcmp_eq);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, fcmp_ne, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    FREE(LilyMirInstructionDestSrc, &self->fcmp_ne);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, fcmp_le, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    FREE(LilyMirInstructionDestSrc, &self->fcmp_le);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, fcmp_lt, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    FREE(LilyMirInstructionDestSrc, &self->fcmp_lt);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, fcmp_ge, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    FREE(LilyMirInstructionDestSrc, &self->fcmp_ge);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, fcmp_gt, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    FREE(LilyMirInstructionDestSrc, &self->fcmp_gt);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, fdiv, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    FREE(LilyMirInstructionDestSrc, &self->fdiv);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, fmul, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    FREE(LilyMirInstructionDestSrc, &self->fmul);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, fneg, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    FREE(LilyMirInstructionSrc, &self->fneg);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, frem, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    FREE(LilyMirInstructionDestSrc, &self->frem);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, fsub, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    FREE(LilyMirInstructionDestSrc, &self->fsub);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, fun, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    FREE(LilyMirInstructionFun, &self->fun);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, fun_prototype, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    FREE(LilyMirInstructionFunPrototype, &self->fun_prototype);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, getarray, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    FREE(LilyMirInstructionGetArray, &self->getarray);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, getarg, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    FREE(LilyMirInstructionSrc, &self->getarg);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, getfield, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    FREE(LilyMirInstructionGetField, &self->getfield);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, getlist, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    FREE(LilyMirInstructionSrc, &self->getlist);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, getptr, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    FREE(LilyMirInstructionSrc, &self->getptr);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, getslice, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    FREE(LilyMirInstructionSrc, &self->getslice);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, iadd, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    FREE(LilyMirInstructionDestSrc, &self->iadd);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, icmp_eq, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    FREE(LilyMirInstructionDestSrc, &self->icmp_eq);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, icmp_ne, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    FREE(LilyMirInstructionDestSrc, &self->icmp_ne);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, icmp_le, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    FREE(LilyMirInstructionDestSrc, &self->icmp_le);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, icmp_lt, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    FREE(LilyMirInstructionDestSrc, &self->icmp_lt);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, icmp_ge, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    FREE(LilyMirInstructionDestSrc, &self->icmp_ge);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, icmp_gt, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    FREE(LilyMirInstructionDestSrc, &self->icmp_gt);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, idiv, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    FREE(LilyMirInstructionDestSrc, &self->idiv);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, imul, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    FREE(LilyMirInstructionDestSrc, &self->imul);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, inctrace, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    FREE(LilyMirInstructionSrc, &self->inctrace);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, ineg, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    FREE(LilyMirInstructionSrc, &self->ineg);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, irem, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    FREE(LilyMirInstructionDestSrc, &self->irem);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, isok, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    FREE(LilyMirInstructionSrc, &self->isok);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, iserr, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    FREE(LilyMirInstructionSrc, &self->iserr);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, isub, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    FREE(LilyMirInstructionDestSrc, &self->isub);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, jmp, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, jmpcond, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    FREE(LilyMirInstructionJmpCond, &self->jmpcond);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, len, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    FREE(LilyMirInstructionSrc, &self->len);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, load, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    FREE(LilyMirInstructionLoad, &self->load);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, makeref, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    FREE(LilyMirInstructionSrc, &self->makeref);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, makeopt, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    FREE(LilyMirInstructionSrc, &self->makeopt);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, non_nil, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    FREE(LilyMirInstruction, self->non_nil);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, not, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    FREE(LilyMirInstructionSrc, &self->not );
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, reg, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    FREE(LilyMirInstructionReg, &self->reg);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, ref_ptr, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    FREE(LilyMirInstructionSrc, &self->ref_ptr);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, ret, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    FREE(LilyMirInstruction, self->ret);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, shl, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    FREE(LilyMirInstructionDestSrc, &self->shl);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, shr, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    FREE(LilyMirInstructionDestSrc, &self->shr);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, store, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    FREE(LilyMirInstructionDestSrc, &self->store);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, struct, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    FREE(LilyMirInstructionStruct, &self->struct_);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, switch, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    FREE(LilyMirInstructionSwitch, &self->switch_);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, sys_call, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    FREE(LilyMirInstructionCall, &self->sys_call);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, trunc, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    FREE(LilyMirInstructionValDt, &self->trunc);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, try, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    FREE(LilyMirInstructionTry, &self->try);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, try_ptr, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    FREE(LilyMirInstructionTry, &self->try_ptr);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, unreachable, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, val, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    FREE(LilyMirInstructionVal, self->val);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, var, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    FREE(LilyMirInstructionVar, &self->var);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, xor, LilyMirInstruction *self)
{
    FREE_DEBUG_INFO(self);
    FREE(LilyMirInstructionDestSrc, &self->xor);
    lily_free(self);
}

DESTRUCTOR(LilyMirInstruction, LilyMirInstruction *self)
{
    switch (self->kind) {
        case LILY_MIR_INSTRUCTION_KIND_ALLOC:
            FREE_VARIANT(LilyMirInstruction, alloc, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_ARG:
            FREE_VARIANT(LilyMirInstruction, arg, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_ASM:
            FREE_VARIANT(LilyMirInstruction, asm, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_BITCAST:
            FREE_VARIANT(LilyMirInstruction, bitcast, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_BITAND:
            FREE_VARIANT(LilyMirInstruction, bitand, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_BITNOT:
            FREE_VARIANT(LilyMirInstruction, bitnot, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_BITOR:
            FREE_VARIANT(LilyMirInstruction, bitor, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_BLOCK:
            FREE_VARIANT(LilyMirInstruction, block, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_BUILTIN_CALL:
            FREE_VARIANT(LilyMirInstruction, builtin_call, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_CALL:
            FREE_VARIANT(LilyMirInstruction, call, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_CONST:
            FREE_VARIANT(LilyMirInstruction, const, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_DROP:
            FREE_VARIANT(LilyMirInstruction, drop, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_EXP:
            FREE_VARIANT(LilyMirInstruction, exp, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_FADD:
            FREE_VARIANT(LilyMirInstruction, fadd, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_FCMP_EQ:
            FREE_VARIANT(LilyMirInstruction, fcmp_eq, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_FCMP_NE:
            FREE_VARIANT(LilyMirInstruction, fcmp_ne, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_FCMP_LE:
            FREE_VARIANT(LilyMirInstruction, fcmp_le, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_FCMP_LT:
            FREE_VARIANT(LilyMirInstruction, fcmp_lt, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_FCMP_GE:
            FREE_VARIANT(LilyMirInstruction, fcmp_ge, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_FCMP_GT:
            FREE_VARIANT(LilyMirInstruction, fcmp_gt, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_FDIV:
            FREE_VARIANT(LilyMirInstruction, fdiv, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_FMUL:
            FREE_VARIANT(LilyMirInstruction, fmul, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_FNEG:
            FREE_VARIANT(LilyMirInstruction, fneg, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_FREM:
            FREE_VARIANT(LilyMirInstruction, frem, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_FSUB:
            FREE_VARIANT(LilyMirInstruction, fsub, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_FUN:
            FREE_VARIANT(LilyMirInstruction, fun, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_FUN_PROTOTYPE:
            FREE_VARIANT(LilyMirInstruction, fun_prototype, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_GETARG:
            FREE_VARIANT(LilyMirInstruction, getarg, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_GETARRAY:
            FREE_VARIANT(LilyMirInstruction, getarray, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_GETLIST:
            FREE_VARIANT(LilyMirInstruction, getlist, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_GETSLICE:
            FREE_VARIANT(LilyMirInstruction, getslice, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_GETFIELD:
            FREE_VARIANT(LilyMirInstruction, getfield, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_GETPTR:
            FREE_VARIANT(LilyMirInstruction, getptr, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_IADD:
            FREE_VARIANT(LilyMirInstruction, iadd, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_ICMP_EQ:
            FREE_VARIANT(LilyMirInstruction, icmp_eq, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_ICMP_NE:
            FREE_VARIANT(LilyMirInstruction, icmp_ne, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_ICMP_LE:
            FREE_VARIANT(LilyMirInstruction, icmp_le, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_ICMP_LT:
            FREE_VARIANT(LilyMirInstruction, icmp_lt, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_ICMP_GE:
            FREE_VARIANT(LilyMirInstruction, icmp_ge, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_ICMP_GT:
            FREE_VARIANT(LilyMirInstruction, icmp_gt, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_IDIV:
            FREE_VARIANT(LilyMirInstruction, idiv, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_IMUL:
            FREE_VARIANT(LilyMirInstruction, imul, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_INCTRACE:
            FREE_VARIANT(LilyMirInstruction, inctrace, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_INEG:
            FREE_VARIANT(LilyMirInstruction, ineg, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_IREM:
            FREE_VARIANT(LilyMirInstruction, irem, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_ISOK:
            FREE_VARIANT(LilyMirInstruction, isok, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_ISERR:
            FREE_VARIANT(LilyMirInstruction, iserr, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_ISUB:
            FREE_VARIANT(LilyMirInstruction, isub, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_JMP:
            FREE_VARIANT(LilyMirInstruction, jmp, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_JMPCOND:
            FREE_VARIANT(LilyMirInstruction, jmpcond, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_LEN:
            FREE_VARIANT(LilyMirInstruction, len, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_LOAD:
            FREE_VARIANT(LilyMirInstruction, load, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_MAKEREF:
            FREE_VARIANT(LilyMirInstruction, makeref, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_MAKEOPT:
            FREE_VARIANT(LilyMirInstruction, makeopt, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_NON_NIL:
            FREE_VARIANT(LilyMirInstruction, non_nil, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_NOT:
            FREE_VARIANT(LilyMirInstruction, not, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_REF_PTR:
            FREE_VARIANT(LilyMirInstruction, ref_ptr, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_REG:
            FREE_VARIANT(LilyMirInstruction, reg, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_RET:
            FREE_VARIANT(LilyMirInstruction, ret, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_SHL:
            FREE_VARIANT(LilyMirInstruction, shl, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_SHR:
            FREE_VARIANT(LilyMirInstruction, shr, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_STORE:
            FREE_VARIANT(LilyMirInstruction, store, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_STRUCT:
            FREE_VARIANT(LilyMirInstruction, struct, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_SWITCH:
            FREE_VARIANT(LilyMirInstruction, switch, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_SYS_CALL:
            FREE_VARIANT(LilyMirInstruction, sys_call, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_TRUNC:
            FREE_VARIANT(LilyMirInstruction, trunc, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_TRY:
            FREE_VARIANT(LilyMirInstruction, try, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_TRY_PTR:
            FREE_VARIANT(LilyMirInstruction, try_ptr, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_UNREACHABLE:
            FREE_VARIANT(LilyMirInstruction, unreachable, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_VAL:
            FREE_VARIANT(LilyMirInstruction, val, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_VAR:
            FREE_VARIANT(LilyMirInstruction, var, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_XOR:
            FREE_VARIANT(LilyMirInstruction, xor, self);
            break;
        default:
            UNREACHABLE("unknown variant");
    }
}
