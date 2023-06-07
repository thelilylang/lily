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

#include <core/lily/mir/instruction.h>

#include <stdio.h>
#include <stdlib.h>

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
                                 loop,
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

DESTRUCTOR(LilyMirInstructionBlock, const LilyMirInstructionBlock *self)
{
    lily_free(self->name);
    FREE_BUFFER_ITEMS(
      self->insts->buffer, self->insts->len, LilyMirInstruction);
    FREE(Vec, self->insts);
}

DESTRUCTOR(LilyMirInstructionCall, const LilyMirInstructionCall *self)
{
    FREE_BUFFER_ITEMS(
      self->params->buffer, self->params->len, LilyMirInstruction);
    FREE(Vec, self->params);
}

DESTRUCTOR(LilyMirInstructionFun, const LilyMirInstructionFun *self)
{
    FREE_BUFFER_ITEMS(self->args->buffer, self->args->len, LilyMirInstruction);
    FREE(Vec, self->args);
    FREE_BUFFER_ITEMS(
      self->insts->buffer, self->insts->len, LilyMirInstruction);
    FREE(Vec, self->insts);
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

DESTRUCTOR(LilyMirInstructionSwitchCase, LilyMirInstructionSwitchCase *self)
{
    FREE(LilyMirInstructionVal, self->val);
    lily_free(self);
}

DESTRUCTOR(LilyMirInstructionSwitch, const LilyMirInstructionSwitch *self)
{
    FREE(LilyMirInstructionVal, self->val);
    FREE_BUFFER_ITEMS(
      self->cases->buffer, self->cases->len, LilyMirInstructionSwitch);
    FREE(Vec, self->cases);
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
                    loop,
                    LilyMirInstructionBlock loop)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_LOOP;
    self->loop = loop;

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
                    xor,
                    LilyMirInstructionSrcDest xor)
{
    LilyMirInstruction *self = lily_malloc(sizeof(LilyMirInstruction));

    self->kind = LILY_MIR_INSTRUCTION_KIND_XOR;
    self->xor = xor;

    return self;
}

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

VARIANT_DESTRUCTOR(LilyMirInstruction, loop, LilyMirInstruction *self)
{
    FREE(LilyMirInstructionBlock, &self->block);
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
        case LILY_MIR_INSTRUCTION_KIND_LOOP:
            FREE_VARIANT(LilyMirInstruction, loop, self);
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
        case LILY_MIR_INSTRUCTION_KIND_XOR:
            FREE_VARIANT(LilyMirInstruction, xor, self);
            break;
        default:
            UNREACHABLE("unknown variant");
    }
}
