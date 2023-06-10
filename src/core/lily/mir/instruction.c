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

#include <base/format.h>

#include <core/lily/mir/instruction.h>

#include <stdio.h>
#include <stdlib.h>

static Usize tab_count = 0;

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

static inline VARIANT_DESTRUCTOR(LilyMirInstruction,
                                 alloc,
                                 LilyMirInstruction *self);

static inline VARIANT_DESTRUCTOR(LilyMirInstruction,
                                 and,
                                 LilyMirInstruction *self);

static inline VARIANT_DESTRUCTOR(LilyMirInstruction,
                                 arg,
                                 LilyMirInstruction *self);

static inline VARIANT_DESTRUCTOR(LilyMirInstruction,
                                 asm,
                                 LilyMirInstruction *self);

static inline VARIANT_DESTRUCTOR(LilyMirInstruction,
                                 bitcast,
                                 LilyMirInstruction *self);

static inline VARIANT_DESTRUCTOR(LilyMirInstruction,
                                   bitand,
                                 LilyMirInstruction *self);

static inline VARIANT_DESTRUCTOR(LilyMirInstruction,
                                 bitnot,
                                 LilyMirInstruction *self);

static inline VARIANT_DESTRUCTOR(LilyMirInstruction,
                                 bitor
                                 ,
                                 LilyMirInstruction *self);

static inline VARIANT_DESTRUCTOR(LilyMirInstruction,
                                 block,
                                 LilyMirInstruction *self);

static inline VARIANT_DESTRUCTOR(LilyMirInstruction,
                                 builtin_call,
                                 LilyMirInstruction *self);

static inline VARIANT_DESTRUCTOR(LilyMirInstruction,
                                 call,
                                 LilyMirInstruction *self);

static inline VARIANT_DESTRUCTOR(LilyMirInstruction,
                                 const,
                                 LilyMirInstruction *self);

static inline VARIANT_DESTRUCTOR(LilyMirInstruction,
                                 drop,
                                 LilyMirInstruction *self);

static inline VARIANT_DESTRUCTOR(LilyMirInstruction,
                                 exp,
                                 LilyMirInstruction *self);

static inline VARIANT_DESTRUCTOR(LilyMirInstruction,
                                 fadd,
                                 LilyMirInstruction *self);

static inline VARIANT_DESTRUCTOR(LilyMirInstruction,
                                 fcmp_eq,
                                 LilyMirInstruction *self);

static inline VARIANT_DESTRUCTOR(LilyMirInstruction,
                                 fcmp_ne,
                                 LilyMirInstruction *self);

static inline VARIANT_DESTRUCTOR(LilyMirInstruction,
                                 fcmp_le,
                                 LilyMirInstruction *self);

static inline VARIANT_DESTRUCTOR(LilyMirInstruction,
                                 fcmp_lt,
                                 LilyMirInstruction *self);

static inline VARIANT_DESTRUCTOR(LilyMirInstruction,
                                 fcmp_ge,
                                 LilyMirInstruction *self);

static inline VARIANT_DESTRUCTOR(LilyMirInstruction,
                                 fcmp_gt,
                                 LilyMirInstruction *self);

static inline VARIANT_DESTRUCTOR(LilyMirInstruction,
                                 fdiv,
                                 LilyMirInstruction *self);

static inline VARIANT_DESTRUCTOR(LilyMirInstruction,
                                 fmul,
                                 LilyMirInstruction *self);

static inline VARIANT_DESTRUCTOR(LilyMirInstruction,
                                 fneg,
                                 LilyMirInstruction *self);

static inline VARIANT_DESTRUCTOR(LilyMirInstruction,
                                 frem,
                                 LilyMirInstruction *self);

static inline VARIANT_DESTRUCTOR(LilyMirInstruction,
                                 fun,
                                 LilyMirInstruction *self);

static inline VARIANT_DESTRUCTOR(LilyMirInstruction,
                                 getarray,
                                 LilyMirInstruction *self);

static inline VARIANT_DESTRUCTOR(LilyMirInstruction,
                                 getarg,
                                 LilyMirInstruction *self);

static inline VARIANT_DESTRUCTOR(LilyMirInstruction,
                                 getfield,
                                 LilyMirInstruction *self);

static inline VARIANT_DESTRUCTOR(LilyMirInstruction,
                                 getlist,
                                 LilyMirInstruction *self);

static inline VARIANT_DESTRUCTOR(LilyMirInstruction,
                                 getptr,
                                 LilyMirInstruction *self);

static inline VARIANT_DESTRUCTOR(LilyMirInstruction,
                                 getslice,
                                 LilyMirInstruction *self);

static inline VARIANT_DESTRUCTOR(LilyMirInstruction,
                                 iadd,
                                 LilyMirInstruction *self);

static inline VARIANT_DESTRUCTOR(LilyMirInstruction,
                                 icmp_eq,
                                 LilyMirInstruction *self);

static inline VARIANT_DESTRUCTOR(LilyMirInstruction,
                                 icmp_ne,
                                 LilyMirInstruction *self);

static inline VARIANT_DESTRUCTOR(LilyMirInstruction,
                                 icmp_le,
                                 LilyMirInstruction *self);

static inline VARIANT_DESTRUCTOR(LilyMirInstruction,
                                 icmp_lt,
                                 LilyMirInstruction *self);

static inline VARIANT_DESTRUCTOR(LilyMirInstruction,
                                 icmp_ge,
                                 LilyMirInstruction *self);

static inline VARIANT_DESTRUCTOR(LilyMirInstruction,
                                 icmp_gt,
                                 LilyMirInstruction *self);

static inline VARIANT_DESTRUCTOR(LilyMirInstruction,
                                 idiv,
                                 LilyMirInstruction *self);

static inline VARIANT_DESTRUCTOR(LilyMirInstruction,
                                 imul,
                                 LilyMirInstruction *self);

static inline VARIANT_DESTRUCTOR(LilyMirInstruction,
                                 inctrace,
                                 LilyMirInstruction *self);

static inline VARIANT_DESTRUCTOR(LilyMirInstruction,
                                 ineg,
                                 LilyMirInstruction *self);

static inline VARIANT_DESTRUCTOR(LilyMirInstruction,
                                 irem,
                                 LilyMirInstruction *self);

static inline VARIANT_DESTRUCTOR(LilyMirInstruction,
                                 isok,
                                 LilyMirInstruction *self);

static inline VARIANT_DESTRUCTOR(LilyMirInstruction,
                                 iserr,
                                 LilyMirInstruction *self);

static inline VARIANT_DESTRUCTOR(LilyMirInstruction,
                                 isub,
                                 LilyMirInstruction *self);

static inline VARIANT_DESTRUCTOR(LilyMirInstruction,
                                 jmp,
                                 LilyMirInstruction *self);

static inline VARIANT_DESTRUCTOR(LilyMirInstruction,
                                 jmpcond,
                                 LilyMirInstruction *self);

static inline VARIANT_DESTRUCTOR(LilyMirInstruction,
                                 len,
                                 LilyMirInstruction *self);

static inline VARIANT_DESTRUCTOR(LilyMirInstruction,
                                 load,
                                 LilyMirInstruction *self);

static inline VARIANT_DESTRUCTOR(LilyMirInstruction,
                                 makeref,
                                 LilyMirInstruction *self);

static inline VARIANT_DESTRUCTOR(LilyMirInstruction,
                                 makeopt,
                                 LilyMirInstruction *self);

static inline VARIANT_DESTRUCTOR(LilyMirInstruction,
                                 non_nil,
                                 LilyMirInstruction *self);

static inline VARIANT_DESTRUCTOR(LilyMirInstruction,
                                 not,
                                 LilyMirInstruction *self);

static inline VARIANT_DESTRUCTOR(LilyMirInstruction,
                                 or
                                 ,
                                 LilyMirInstruction *self);

static inline VARIANT_DESTRUCTOR(LilyMirInstruction,
                                 reg,
                                 LilyMirInstruction *self);

static inline VARIANT_DESTRUCTOR(LilyMirInstruction,
                                 ref_ptr,
                                 LilyMirInstruction *self);

static inline VARIANT_DESTRUCTOR(LilyMirInstruction,
                                 ret,
                                 LilyMirInstruction *self);

static inline VARIANT_DESTRUCTOR(LilyMirInstruction,
                                 shl,
                                 LilyMirInstruction *self);

static inline VARIANT_DESTRUCTOR(LilyMirInstruction,
                                 shr,
                                 LilyMirInstruction *self);

static inline VARIANT_DESTRUCTOR(LilyMirInstruction,
                                 store,
                                 LilyMirInstruction *self);

static inline VARIANT_DESTRUCTOR(LilyMirInstruction,
                                 struct,
                                 LilyMirInstruction *self);

static inline VARIANT_DESTRUCTOR(LilyMirInstruction,
                                 switch,
                                 LilyMirInstruction *self);

static inline VARIANT_DESTRUCTOR(LilyMirInstruction,
                                 sys_call,
                                 LilyMirInstruction *self);

static inline VARIANT_DESTRUCTOR(LilyMirInstruction,
                                 trunc,
                                 LilyMirInstruction *self);

static inline VARIANT_DESTRUCTOR(LilyMirInstruction,
                                 try,
                                 LilyMirInstruction *self);

static inline VARIANT_DESTRUCTOR(LilyMirInstruction,
                                 try_ptr,
                                 LilyMirInstruction *self);

static inline VARIANT_DESTRUCTOR(LilyMirInstruction,
                                 val,
                                 LilyMirInstruction *self);

static inline VARIANT_DESTRUCTOR(LilyMirInstruction,
                                 var,
                                 LilyMirInstruction *self);

static inline VARIANT_DESTRUCTOR(LilyMirInstruction,
                                 xor,
                                 LilyMirInstruction *self);

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
                    list,
                    LilyMirDt *dt,
                    Vec *list)
{
    LilyMirInstructionVal *self = lily_malloc(sizeof(LilyMirInstructionVal));

    self->kind = LILY_MIR_INSTRUCTION_VAL_KIND_LIST;
    self->dt = dt;
    self->list = list;

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
                    slice,
                    LilyMirDt *dt,
                    Vec *slice)
{
    LilyMirInstructionVal *self = lily_malloc(sizeof(LilyMirInstructionVal));

    self->kind = LILY_MIR_INSTRUCTION_VAL_KIND_SLICE;
    self->dt = dt;
    self->slice = slice;

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
                    trace,
                    LilyMirDt *dt,
                    LilyMirInstructionVal *trace)
{
    LilyMirInstructionVal *self = lily_malloc(sizeof(LilyMirInstructionVal));

    self->kind = LILY_MIR_INSTRUCTION_VAL_KIND_TRACE;
    self->dt = dt;
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

VARIANT_CONSTRUCTOR(LilyMirInstructionVal *,
                    LilyMirInstructionVal,
                    var,
                    LilyMirDt *dt,
                    const char *var)
{
    LilyMirInstructionVal *self = lily_malloc(sizeof(LilyMirInstructionVal));

    self->kind = LILY_MIR_INSTRUCTION_VAL_KIND_VAR;
    self->dt = dt;
    self->var = var;

    return self;
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
        case LILY_MIR_INSTRUCTION_VAL_KIND_REG:
            push_str__String(res, "\x1b[33m%");
            push_str__String(res, (char *)self->reg);
            push_str__String(res, "\x1b[0m");

            return res;
        case LILY_MIR_INSTRUCTION_VAL_KIND_SLICE:
            push_str__String(res, "&[");

            for (Usize i = 0; i < self->slice->len; ++i) {
                String *item = to_string__Debug__LilyMirInstructionVal(
                  get__Vec(self->slice, i));

                APPEND_AND_FREE(res, item);
            }

            push_str__String(res, "]");

            return res;
        case LILY_MIR_INSTRUCTION_VAL_KIND_STR:
            push_str__String(res, "\x1b[32m\"");
            push_str__String(res, (char *)self->bytes);
            push_str__String(res, "\"\x1b[0m");

            return res;
        case LILY_MIR_INSTRUCTION_VAL_KIND_STRUCT:
            push_str__String(res, "\x1b[33mstruct\x1b[0m {");

            for (Usize i = 0; i < self->struct_->len; ++i) {
                char *item = format("{d} = {Sr}",
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

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyMirInstructionSrcDest,
               const LilyMirInstructionSrcDest *self)
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
    return format__String("alloc {Sr}", to_string__Debug__LilyMirDt(self->dt));
}
#endif

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyMirInstructionArg,
               const LilyMirInstructionArg *self)
{
    return format__String(
      "{Sr} {s}", to_string__Debug__LilyMirDt(self->dt), self->name);
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
    String *res =
      format__String("{Sr}{s}:", repeat__String("\t", tab_count), self->name);

    ++tab_count;

    String *tab = repeat__String("\t", tab_count);

    for (Usize i = 0; i < self->insts->len; ++i) {
        String *item = format__String(
          "{S}{Sr}",
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
    String *res = format__String("call {s}(", self->name);

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

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyMirInstructionFun,
               const LilyMirInstructionFun *self)
{
    String *res = format__String("{s} \x1b[36mfun\x1b[0m {s}(");

    for (Usize i = 0; i < self->args->len; ++i) {
        String *item =
          to_string__Debug__LilyMirInstruction(get__Vec(self->args, i));

        APPEND_AND_FREE(res, item);

        if (i + 1 != self->args->len) {
            push_str__String(res, ", ");
        }
    }

    push_str__String(res, ") {\n");

    ++tab_count;

    String *tab = repeat__String("\t", tab_count);

    for (Usize i = 0; i < self->insts->len; ++i) {
        String *item = format__String(
          "{Sr}\n",
          to_string__Debug__LilyMirInstruction(get__Vec(self->insts, i)));

        APPEND_AND_FREE(res, item);
    }

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
    FREE_BUFFER_ITEMS(
      self->insts->buffer, self->insts->len, LilyMirInstruction);
    FREE(Vec, self->insts);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyMirInstructionJmpCond,
               const LilyMirInstructionJmpCond *self)
{
    return format__String(
      "\x1b[34mjmpcond\x1b[0m {Sr}, \x1b[33mblock\x1b[0m {s}",
      to_string__Debug__LilyMirInstructionVal(self->cond),
      self->block->name);
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
    lily_free(self->name);
    FREE(LilyMirInstruction, self->inst);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyMirInstructionVar,
               const LilyMirInstructionVar *self)
{
    return format__String("var {s} = {Sr}",
                          self->name,
                          to_string__Debug__LilyMirInstruction(self->inst));
}
#endif

DESTRUCTOR(LilyMirInstructionVar, const LilyMirInstructionVar *self)
{
    lily_free(self->name);
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
    return format__String("case {Sr}, block {s}",
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
      format__String("switch {Sr}, block {s} {{",
                     to_string__Debug__LilyMirInstructionVal(self->val),
                     self->default_block->name);

    ++tab_count;

    String *tab = repeat__String("\t", tab_count);

    for (Usize i = 0; i < self->cases->len; ++i) {
        String *case_ =
          format__String("{S}{Sr}\n",
                         to_string__Debug__LilyMirInstructionSwitchCase(
                           get__Vec(self->cases, i)));

        APPEND_AND_FREE(res, case_);
    }

    --tab_count;

    append__String(res, tab);
    push_str__String(res, "}");

    FREE(String, tab);

    return res;
}
#endif

DESTRUCTOR(LilyMirInstructionSwitch, const LilyMirInstructionSwitch *self)
{
    FREE(LilyMirInstructionVal, self->val);
    FREE_BUFFER_ITEMS(
      self->cases->buffer, self->cases->len, LilyMirInstructionSwitch);
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
      format__String("{s} struct {s} {{",
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
    self->alloc = alloc;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    and,
                    LilyMirInstructionSrcDest and)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_AND;
    self->and = and;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    arg,
                    LilyMirInstructionArg arg)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_ARG;
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
    self->bitcast = bitcast;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                      bitand,
                    LilyMirInstructionSrcDest bitand)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_BITAND;
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
    self->bitnot = bitnot;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    bitor
                    ,
                    LilyMirInstructionSrcDest bitor)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_BITOR;
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
    self->drop = drop;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    exp,
                    LilyMirInstructionSrcDest exp)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_EXP;
    self->exp = exp;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    fadd,
                    LilyMirInstructionSrcDest fadd)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_FADD;
    self->fadd = fadd;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    fcmp_eq,
                    LilyMirInstructionSrcDest fcmp_eq)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_FCMP_EQ;
    self->fcmp_eq = fcmp_eq;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    fcmp_ne,
                    LilyMirInstructionSrcDest fcmp_ne)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_FCMP_NE;
    self->fcmp_ne = fcmp_ne;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    fcmp_le,
                    LilyMirInstructionSrcDest fcmp_le)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_FCMP_LE;
    self->fcmp_le = fcmp_le;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    fcmp_lt,
                    LilyMirInstructionSrcDest fcmp_lt)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_FCMP_LT;
    self->fcmp_lt = fcmp_lt;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    fcmp_ge,
                    LilyMirInstructionSrcDest fcmp_ge)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_FCMP_GE;
    self->fcmp_ge = fcmp_ge;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    fcmp_gt,
                    LilyMirInstructionSrcDest fcmp_gt)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_FCMP_GT;
    self->fcmp_gt = fcmp_gt;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    fdiv,
                    LilyMirInstructionSrcDest fdiv)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_FDIV;
    self->fdiv = fdiv;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    fmul,
                    LilyMirInstructionSrcDest fmul)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_FMUL;
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
    self->fneg = fneg;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    frem,
                    LilyMirInstructionSrcDest frem)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_FREM;
    self->frem = frem;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    fun,
                    LilyMirInstructionFun fun)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_FUN;
    self->fun = fun;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    getarray,
                    LilyMirInstructionSrc getarray)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_GETARRAY;
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
    self->getarg = getarg;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    getfield,
                    LilyMirInstructionSrc getfield)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_GETFIELD;
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
    self->getslice = getslice;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    iadd,
                    LilyMirInstructionSrcDest iadd)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_IADD;
    self->iadd = iadd;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    icmp_eq,
                    LilyMirInstructionSrcDest icmp_eq)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_ICMP_EQ;
    self->icmp_eq = icmp_eq;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    icmp_ne,
                    LilyMirInstructionSrcDest icmp_ne)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_ICMP_NE;
    self->icmp_ne = icmp_ne;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    icmp_le,
                    LilyMirInstructionSrcDest icmp_le)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_ICMP_LE;
    self->icmp_le = icmp_le;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    icmp_lt,
                    LilyMirInstructionSrcDest icmp_lt)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_ICMP_LT;
    self->icmp_lt = icmp_lt;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    icmp_ge,
                    LilyMirInstructionSrcDest icmp_ge)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_ICMP_GE;
    self->icmp_ge = icmp_ge;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    icmp_gt,
                    LilyMirInstructionSrcDest icmp_gt)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_ICMP_GT;
    self->icmp_gt = icmp_gt;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    idiv,
                    LilyMirInstructionSrcDest idiv)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_IDIV;
    self->idiv = idiv;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    imul,
                    LilyMirInstructionSrcDest imul)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_IMUL;
    self->imul = imul;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    inctrace,
                    LilyMirInstructionSrcDest inctrace)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_INCTRACE;
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
    self->ineg = ineg;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    irem,
                    LilyMirInstructionSrcDest irem)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_IREM;
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
    self->iserr = iserr;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    isub,
                    LilyMirInstructionSrcDest isub)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_ISUB;
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
    self->len = len;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    load,
                    LilyMirInstructionSrc load)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_LOAD;
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
    self->not = not ;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    or
                    ,
                    LilyMirInstructionSrcDest or)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_OR;
    self->or = or ;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    reg,
                    LilyMirInstructionReg reg)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_REG;
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
    self->ret = ret;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    shl,
                    LilyMirInstructionSrcDest shl)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_SHL;
    self->shl = shl;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    shr,
                    LilyMirInstructionSrcDest shr)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_SHR;
    self->shr = shr;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    store,
                    LilyMirInstructionSrcDest store)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_STORE;
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
    self->try_ptr = try_ptr;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    val,
                    LilyMirInstructionVal *val)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_VAL;
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
    self->var = var;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    xor,
                    LilyMirInstructionSrcDest xor)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_XOR;
    self->xor = xor;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyMirInstruction, const LilyMirInstruction *self)
{
    String *res = NULL;

    switch (self->kind) {
        case LILY_MIR_INSTRUCTION_KIND_ALLOC:
            res = format__String(
              "alloc {Sr}",
              to_string__Debug__LilyMirInstructionAlloc(&self->alloc));
            break;
        case LILY_MIR_INSTRUCTION_KIND_AND:
            res = format__String(
              "and {Sr}",
              to_string__Debug__LilyMirInstructionSrcDest(&self->and));
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
              "bitcast {Sr}",
              to_string__Debug__LilyMirInstructionValDt(&self->bitcast));
            break;
        case LILY_MIR_INSTRUCTION_KIND_BITAND:
            res = format__String(
              "bitand {Sr}",
              to_string__Debug__LilyMirInstructionSrcDest(&self->bitand));
            break;
        case LILY_MIR_INSTRUCTION_KIND_BITNOT:
            res = format__String(
              "bitnot {Sr}",
              to_string__Debug__LilyMirInstructionSrc(&self->bitnot));
            break;
        case LILY_MIR_INSTRUCTION_KIND_BITOR:
            res = format__String(
              "bitor {Sr}",
              to_string__Debug__LilyMirInstructionSrcDest(&self->bitor));
            break;
        case LILY_MIR_INSTRUCTION_KIND_BLOCK:
            res = format__String(
              "{Sr}", to_string__Debug__LilyMirInstructionBlock(&self->block));
            break;
        case LILY_MIR_INSTRUCTION_KIND_BUILTIN_CALL:
            res = format__String(
              "call @builtin.{Sr}",
              to_string__Debug__LilyMirInstructionCall(&self->builtin_call));
            break;
        case LILY_MIR_INSTRUCTION_KIND_CALL:
            res = format__String(
              "call {Sr}",
              to_string__Debug__LilyMirInstructionCall(&self->call));
            break;
        case LILY_MIR_INSTRUCTION_KIND_CONST:
            res = format__String(
              "{Sr}", to_string__Debug__LilyMirInstructionConst(&self->const_));
            break;
        case LILY_MIR_INSTRUCTION_KIND_DROP:
            res = format__String(
              "drop {Sr}",
              to_string__Debug__LilyMirInstructionSrc(&self->drop));
            break;
        case LILY_MIR_INSTRUCTION_KIND_EXP:
            res = format__String(
              "exp {Sr}",
              to_string__Debug__LilyMirInstructionSrcDest(&self->exp));
            break;
        case LILY_MIR_INSTRUCTION_KIND_FADD:
            res = format__String(
              "fadd {Sr}",
              to_string__Debug__LilyMirInstructionSrcDest(&self->fadd));
            break;
        case LILY_MIR_INSTRUCTION_KIND_FCMP_EQ:
            res = format__String(
              "fcmp eq {Sr}",
              to_string__Debug__LilyMirInstructionSrcDest(&self->fcmp_eq));
            break;
        case LILY_MIR_INSTRUCTION_KIND_FCMP_NE:
            res = format__String(
              "fcmp ne {Sr}",
              to_string__Debug__LilyMirInstructionSrcDest(&self->fcmp_ne));
            break;
        case LILY_MIR_INSTRUCTION_KIND_FCMP_LE:
            res = format__String(
              "fcmp le {Sr}",
              to_string__Debug__LilyMirInstructionSrcDest(&self->fcmp_le));
            break;
        case LILY_MIR_INSTRUCTION_KIND_FCMP_LT:
            res = format__String(
              "fcmp lt {Sr}",
              to_string__Debug__LilyMirInstructionSrcDest(&self->fcmp_lt));
            break;
        case LILY_MIR_INSTRUCTION_KIND_FCMP_GE:
            res = format__String(
              "fcmp ge {Sr}",
              to_string__Debug__LilyMirInstructionSrcDest(&self->fcmp_ge));
            break;
        case LILY_MIR_INSTRUCTION_KIND_FCMP_GT:
            res = format__String(
              "fcmp gt {Sr}",
              to_string__Debug__LilyMirInstructionSrcDest(&self->fcmp_gt));
            break;
        case LILY_MIR_INSTRUCTION_KIND_FDIV:
            res = format__String(
              "fdiv {Sr}",
              to_string__Debug__LilyMirInstructionSrcDest(&self->fdiv));
            break;
        case LILY_MIR_INSTRUCTION_KIND_FMUL:
            res = format__String(
              "fmul {Sr}",
              to_string__Debug__LilyMirInstructionSrcDest(&self->fmul));
            break;
        case LILY_MIR_INSTRUCTION_KIND_FNEG:
            res = format__String(
              "fneg {Sr}",
              to_string__Debug__LilyMirInstructionSrc(&self->fneg));
            break;
        case LILY_MIR_INSTRUCTION_KIND_FREM:
            res = format__String(
              "frem {Sr}",
              to_string__Debug__LilyMirInstructionSrcDest(&self->frem));
            break;
        case LILY_MIR_INSTRUCTION_KIND_FUN:
            res = format__String(
              "{Sr}", to_string__Debug__LilyMirInstructionFun(&self->fun));
            break;
        case LILY_MIR_INSTRUCTION_KIND_GETARG:
            res = format__String(
              "getarg {Sr}",
              to_string__Debug__LilyMirInstructionSrc(&self->getarg));
            break;
        case LILY_MIR_INSTRUCTION_KIND_GETARRAY:
            res = format__String(
              "getarray {Sr}",
              to_string__Debug__LilyMirInstructionSrc(&self->getarray));
            break;
        case LILY_MIR_INSTRUCTION_KIND_GETLIST:
            res = format__String(
              "getlist {Sr}",
              to_string__Debug__LilyMirInstructionSrc(&self->getlist));
            break;
        case LILY_MIR_INSTRUCTION_KIND_GETSLICE:
            res = format__String(
              "getslice {Sr}",
              to_string__Debug__LilyMirInstructionSrc(&self->getslice));
            break;
        case LILY_MIR_INSTRUCTION_KIND_GETFIELD:
            res = format__String(
              "getfield {Sr}",
              to_string__Debug__LilyMirInstructionSrc(&self->getfield));
            break;
        case LILY_MIR_INSTRUCTION_KIND_GETPTR:
            res = format__String(
              "getptr {Sr}",
              to_string__Debug__LilyMirInstructionSrc(&self->getptr));
            break;
        case LILY_MIR_INSTRUCTION_KIND_IADD:
            res = format__String(
              "iadd {Sr}",
              to_string__Debug__LilyMirInstructionSrcDest(&self->iadd));
            break;
        case LILY_MIR_INSTRUCTION_KIND_ICMP_EQ:
            res = format__String(
              "icmp eq {Sr}",
              to_string__Debug__LilyMirInstructionSrcDest(&self->icmp_eq));
            break;
        case LILY_MIR_INSTRUCTION_KIND_ICMP_NE:
            res = format__String(
              "icmp ne {Sr}",
              to_string__Debug__LilyMirInstructionSrcDest(&self->icmp_ne));
            break;
        case LILY_MIR_INSTRUCTION_KIND_ICMP_LE:
            res = format__String(
              "icmp le {Sr}",
              to_string__Debug__LilyMirInstructionSrcDest(&self->icmp_le));
            break;
        case LILY_MIR_INSTRUCTION_KIND_ICMP_LT:
            res = format__String(
              "icmp lt {Sr}",
              to_string__Debug__LilyMirInstructionSrcDest(&self->icmp_lt));
            break;
        case LILY_MIR_INSTRUCTION_KIND_ICMP_GE:
            res = format__String(
              "icmp ge {Sr}",
              to_string__Debug__LilyMirInstructionSrcDest(&self->icmp_ge));
            break;
        case LILY_MIR_INSTRUCTION_KIND_ICMP_GT:
            res = format__String(
              "icmp gt {Sr}",
              to_string__Debug__LilyMirInstructionSrcDest(&self->icmp_gt));
            break;
        case LILY_MIR_INSTRUCTION_KIND_IDIV:
            res = format__String(
              "idiv {Sr}",
              to_string__Debug__LilyMirInstructionSrcDest(&self->idiv));
            break;
        case LILY_MIR_INSTRUCTION_KIND_IMUL:
            res = format__String(
              "imul {Sr}",
              to_string__Debug__LilyMirInstructionSrcDest(&self->imul));
            break;
        case LILY_MIR_INSTRUCTION_KIND_INCTRACE:
            res = format__String(
              "inctrace {Sr}",
              to_string__Debug__LilyMirInstructionSrc(&self->inctrace));
            break;
        case LILY_MIR_INSTRUCTION_KIND_INEG:
            res = format__String(
              "ineg {Sr}",
              to_string__Debug__LilyMirInstructionSrc(&self->ineg));
            break;
        case LILY_MIR_INSTRUCTION_KIND_IREM:
            res = format__String(
              "irem {Sr}",
              to_string__Debug__LilyMirInstructionSrcDest(&self->irem));
            break;
        case LILY_MIR_INSTRUCTION_KIND_ISOK:
            res = format__String(
              "isok {Sr}",
              to_string__Debug__LilyMirInstructionSrc(&self->isok));
            break;
        case LILY_MIR_INSTRUCTION_KIND_ISERR:
            res = format__String(
              "iserr {Sr}",
              to_string__Debug__LilyMirInstructionSrc(&self->iserr));
            break;
        case LILY_MIR_INSTRUCTION_KIND_ISUB:
            res = format__String(
              "isub {Sr}",
              to_string__Debug__LilyMirInstructionSrcDest(&self->isub));
            break;
        case LILY_MIR_INSTRUCTION_KIND_JMP:
            res = format__String("jmp block {s}", self->jmp->name);
            break;
        case LILY_MIR_INSTRUCTION_KIND_JMPCOND:
            res = format__String(
              "{Sr}",
              to_string__Debug__LilyMirInstructionJmpCond(&self->jmpcond));
            break;
        case LILY_MIR_INSTRUCTION_KIND_LEN:
            res = format__String(
              "len {Sr}", to_string__Debug__LilyMirInstructionSrc(&self->len));
            break;
        case LILY_MIR_INSTRUCTION_KIND_LOAD:
            res = format__String(
              "load {Sr}",
              to_string__Debug__LilyMirInstructionSrc(&self->load));
            break;
        case LILY_MIR_INSTRUCTION_KIND_MAKEREF:
            res = format__String(
              "makeref {Sr}",
              to_string__Debug__LilyMirInstructionSrc(&self->makeref));
            break;
        case LILY_MIR_INSTRUCTION_KIND_MAKEOPT:
            res = format__String(
              "makeopt {Sr}",
              to_string__Debug__LilyMirInstructionSrc(&self->makeopt));
            break;
        case LILY_MIR_INSTRUCTION_KIND_NON_NIL:
            res = format__String(
              "non_nil {Sr}",
              to_string__Debug__LilyMirInstruction(self->non_nil));
            break;
        case LILY_MIR_INSTRUCTION_KIND_NOT:
            res = format__String(
              "not {Sr}", to_string__Debug__LilyMirInstructionSrc(&self->not ));
            break;
        case LILY_MIR_INSTRUCTION_KIND_OR:
            res = format__String(
              "or {Sr}",
              to_string__Debug__LilyMirInstructionSrcDest(&self->or));
            break;
        case LILY_MIR_INSTRUCTION_KIND_REF_PTR:
            res = format__String(
              "ref_ptr {Sr}",
              to_string__Debug__LilyMirInstructionSrc(&self->ref_ptr));
            break;
        case LILY_MIR_INSTRUCTION_KIND_REG:
            res = format__String(
              "{Sr}", to_string__Debug__LilyMirInstructionReg(&self->reg));
            break;
        case LILY_MIR_INSTRUCTION_KIND_RET:
            res = format__String(
              "ret {Sr}", to_string__Debug__LilyMirInstruction(self->ret));
            break;
        case LILY_MIR_INSTRUCTION_KIND_SHL:
            res = format__String(
              "shl {Sr}",
              to_string__Debug__LilyMirInstructionSrcDest(&self->shl));
            break;
        case LILY_MIR_INSTRUCTION_KIND_SHR:
            res = format__String(
              "shr {Sr}",
              to_string__Debug__LilyMirInstructionSrcDest(&self->shr));
            break;
        case LILY_MIR_INSTRUCTION_KIND_STORE:
            res = format__String(
              "store {Sr}",
              to_string__Debug__LilyMirInstructionSrcDest(&self->store));
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
              "call @sys.{Sr}",
              to_string__Debug__LilyMirInstructionCall(&self->sys_call));
            break;
        case LILY_MIR_INSTRUCTION_KIND_TRUNC:
            res = format__String(
              "trunc {Sr}",
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
              "xor {Sr}",
              to_string__Debug__LilyMirInstructionSrcDest(&self->xor));
            break;
        default:
            UNREACHABLE("unknown variant");
    }

    return res;
}
#endif

VARIANT_DESTRUCTOR(LilyMirInstruction, alloc, LilyMirInstruction *self)
{
    FREE(LilyMirInstructionAlloc, &self->alloc);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, and, LilyMirInstruction *self)
{
    FREE(LilyMirInstructionSrcDest, &self->and);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, arg, LilyMirInstruction *self)
{
    FREE(LilyMirInstructionArg, &self->arg);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, asm, LilyMirInstruction *self)
{
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, bitcast, LilyMirInstruction *self)
{
    FREE(LilyMirInstructionValDt, &self->bitcast);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, bitand, LilyMirInstruction *self)
{
    FREE(LilyMirInstructionSrcDest, &self->bitand);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, bitnot, LilyMirInstruction *self)
{
    FREE(LilyMirInstructionSrc, &self->bitnot);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, bitor, LilyMirInstruction *self)
{
    FREE(LilyMirInstructionSrcDest, &self->bitor);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, block, LilyMirInstruction *self)
{
    FREE(LilyMirInstructionBlock, &self->block);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, builtin_call, LilyMirInstruction *self)
{
    FREE(LilyMirInstructionCall, &self->builtin_call);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, call, LilyMirInstruction *self)
{
    FREE(LilyMirInstructionCall, &self->call);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, const, LilyMirInstruction *self)
{
    FREE(LilyMirInstructionConst, &self->const_);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, drop, LilyMirInstruction *self)
{
    FREE(LilyMirInstructionSrc, &self->drop);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, exp, LilyMirInstruction *self)
{
    FREE(LilyMirInstructionSrcDest, &self->exp);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, fadd, LilyMirInstruction *self)
{
    FREE(LilyMirInstructionSrcDest, &self->fadd);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, fcmp_eq, LilyMirInstruction *self)
{
    FREE(LilyMirInstructionSrcDest, &self->fcmp_eq);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, fcmp_ne, LilyMirInstruction *self)
{
    FREE(LilyMirInstructionSrcDest, &self->fcmp_ne);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, fcmp_le, LilyMirInstruction *self)
{
    FREE(LilyMirInstructionSrcDest, &self->fcmp_le);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, fcmp_lt, LilyMirInstruction *self)
{
    FREE(LilyMirInstructionSrcDest, &self->fcmp_lt);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, fcmp_ge, LilyMirInstruction *self)
{
    FREE(LilyMirInstructionSrcDest, &self->fcmp_ge);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, fcmp_gt, LilyMirInstruction *self)
{
    FREE(LilyMirInstructionSrcDest, &self->fcmp_gt);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, fdiv, LilyMirInstruction *self)
{
    FREE(LilyMirInstructionSrcDest, &self->fdiv);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, fmul, LilyMirInstruction *self)
{
    FREE(LilyMirInstructionSrcDest, &self->fmul);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, fneg, LilyMirInstruction *self)
{
    FREE(LilyMirInstructionSrc, &self->fneg);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, frem, LilyMirInstruction *self)
{
    FREE(LilyMirInstructionSrcDest, &self->frem);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, fun, LilyMirInstruction *self)
{
    FREE(LilyMirInstructionFun, &self->fun);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, getarray, LilyMirInstruction *self)
{
    FREE(LilyMirInstructionSrc, &self->getarray);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, getarg, LilyMirInstruction *self)
{
    FREE(LilyMirInstructionSrc, &self->getarg);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, getfield, LilyMirInstruction *self)
{
    FREE(LilyMirInstructionSrc, &self->getfield);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, getlist, LilyMirInstruction *self)
{
    FREE(LilyMirInstructionSrc, &self->getlist);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, getptr, LilyMirInstruction *self)
{
    FREE(LilyMirInstructionSrc, &self->getptr);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, getslice, LilyMirInstruction *self)
{
    FREE(LilyMirInstructionSrc, &self->getslice);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, iadd, LilyMirInstruction *self)
{
    FREE(LilyMirInstructionSrcDest, &self->iadd);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, icmp_eq, LilyMirInstruction *self)
{
    FREE(LilyMirInstructionSrcDest, &self->icmp_eq);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, icmp_ne, LilyMirInstruction *self)
{
    FREE(LilyMirInstructionSrcDest, &self->icmp_ne);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, icmp_le, LilyMirInstruction *self)
{
    FREE(LilyMirInstructionSrcDest, &self->icmp_le);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, icmp_lt, LilyMirInstruction *self)
{
    FREE(LilyMirInstructionSrcDest, &self->icmp_lt);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, icmp_ge, LilyMirInstruction *self)
{
    FREE(LilyMirInstructionSrcDest, &self->icmp_ge);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, icmp_gt, LilyMirInstruction *self)
{
    FREE(LilyMirInstructionSrcDest, &self->icmp_gt);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, idiv, LilyMirInstruction *self)
{
    FREE(LilyMirInstructionSrcDest, &self->idiv);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, imul, LilyMirInstruction *self)
{
    FREE(LilyMirInstructionSrcDest, &self->imul);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, inctrace, LilyMirInstruction *self)
{
    FREE(LilyMirInstructionSrc, &self->inctrace);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, ineg, LilyMirInstruction *self)
{
    FREE(LilyMirInstructionSrc, &self->ineg);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, irem, LilyMirInstruction *self)
{
    FREE(LilyMirInstructionSrcDest, &self->irem);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, isok, LilyMirInstruction *self)
{
    FREE(LilyMirInstructionSrc, &self->isok);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, iserr, LilyMirInstruction *self)
{
    FREE(LilyMirInstructionSrc, &self->iserr);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, isub, LilyMirInstruction *self)
{
    FREE(LilyMirInstructionSrcDest, &self->isub);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, jmp, LilyMirInstruction *self)
{
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, jmpcond, LilyMirInstruction *self)
{
    FREE(LilyMirInstructionJmpCond, &self->jmpcond);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, len, LilyMirInstruction *self)
{
    FREE(LilyMirInstructionSrc, &self->len);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, load, LilyMirInstruction *self)
{
    FREE(LilyMirInstructionSrc, &self->load);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, makeref, LilyMirInstruction *self)
{
    FREE(LilyMirInstructionSrc, &self->makeref);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, makeopt, LilyMirInstruction *self)
{
    FREE(LilyMirInstructionSrc, &self->makeopt);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, non_nil, LilyMirInstruction *self)
{
    FREE(LilyMirInstruction, self->non_nil);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, not, LilyMirInstruction *self)
{
    FREE(LilyMirInstructionSrc, &self->not );
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, or, LilyMirInstruction *self)
{
    FREE(LilyMirInstructionSrcDest, &self->or);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, reg, LilyMirInstruction *self)
{
    FREE(LilyMirInstructionReg, &self->reg);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, ref_ptr, LilyMirInstruction *self)
{
    FREE(LilyMirInstructionSrc, &self->ref_ptr);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, ret, LilyMirInstruction *self)
{
    FREE(LilyMirInstruction, self->ret);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, shl, LilyMirInstruction *self)
{
    FREE(LilyMirInstructionSrcDest, &self->shl);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, shr, LilyMirInstruction *self)
{
    FREE(LilyMirInstructionSrcDest, &self->shr);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, store, LilyMirInstruction *self)
{
    FREE(LilyMirInstructionSrcDest, &self->store);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, struct, LilyMirInstruction *self)
{
    FREE(LilyMirInstructionStruct, &self->struct_);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, switch, LilyMirInstruction *self)
{
    FREE(LilyMirInstructionSwitch, &self->switch_);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, sys_call, LilyMirInstruction *self)
{
    FREE(LilyMirInstructionCall, &self->sys_call);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, trunc, LilyMirInstruction *self)
{
    FREE(LilyMirInstructionValDt, &self->trunc);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, try, LilyMirInstruction *self)
{
    FREE(LilyMirInstructionTry, &self->try);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, try_ptr, LilyMirInstruction *self)
{
    FREE(LilyMirInstructionTry, &self->try_ptr);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, val, LilyMirInstruction *self)
{
    FREE(LilyMirInstructionVal, self->val);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, var, LilyMirInstruction *self)
{
    FREE(LilyMirInstructionVar, &self->var);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirInstruction, xor, LilyMirInstruction *self)
{
    FREE(LilyMirInstructionSrcDest, &self->xor);
    lily_free(self);
}

DESTRUCTOR(LilyMirInstruction, LilyMirInstruction *self)
{
    switch (self->kind) {
        case LILY_MIR_INSTRUCTION_KIND_ALLOC:
            FREE_VARIANT(LilyMirInstruction, alloc, self);
            break;
        case LILY_MIR_INSTRUCTION_KIND_AND:
            FREE_VARIANT(LilyMirInstruction, and, self);
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
        case LILY_MIR_INSTRUCTION_KIND_FUN:
            FREE_VARIANT(LilyMirInstruction, fun, self);
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
        case LILY_MIR_INSTRUCTION_KIND_OR:
            FREE_VARIANT(LilyMirInstruction, or, self);
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
