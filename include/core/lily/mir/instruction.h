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

#ifndef LILY_CORE_LILY_MIR_INSTRUCTION_H
#define LILY_CORE_LILY_MIR_INSTRUCTION_H

#include <base/alloc.h>
#include <base/vec.h>

#include <core/lily/mir/dt.h>
#include <core/lily/mir/linkage.h>

enum LilyMirInstructionKind
{
    LILY_MIR_INSTRUCTION_KIND_ALLOC,
    LILY_MIR_INSTRUCTION_KIND_AND,
    LILY_MIR_INSTRUCTION_KIND_ARG,
    LILY_MIR_INSTRUCTION_KIND_ASM,
    LILY_MIR_INSTRUCTION_KIND_BITCAST,
    LILY_MIR_INSTRUCTION_KIND_BITAND,
    LILY_MIR_INSTRUCTION_KIND_BITNOT,
    LILY_MIR_INSTRUCTION_KIND_BITOR,
    LILY_MIR_INSTRUCTION_KIND_BLOCK,
    LILY_MIR_INSTRUCTION_KIND_BUILTIN_CALL,
    LILY_MIR_INSTRUCTION_KIND_CALL,
    LILY_MIR_INSTRUCTION_KIND_CONST,
    LILY_MIR_INSTRUCTION_KIND_DROP,
    LILY_MIR_INSTRUCTION_KIND_EXP,
    LILY_MIR_INSTRUCTION_KIND_FADD,
    LILY_MIR_INSTRUCTION_KIND_FCMP_EQ,
    LILY_MIR_INSTRUCTION_KIND_FCMP_NE,
    LILY_MIR_INSTRUCTION_KIND_FCMP_LE,
    LILY_MIR_INSTRUCTION_KIND_FCMP_LT,
    LILY_MIR_INSTRUCTION_KIND_FCMP_GE,
    LILY_MIR_INSTRUCTION_KIND_FCMP_GT,
    LILY_MIR_INSTRUCTION_KIND_FDIV,
    LILY_MIR_INSTRUCTION_KIND_FMUL,
    LILY_MIR_INSTRUCTION_KIND_FNEG,
    LILY_MIR_INSTRUCTION_KIND_FREM,
    LILY_MIR_INSTRUCTION_KIND_FUN,
    LILY_MIR_INSTRUCTION_KIND_GETARG,
    LILY_MIR_INSTRUCTION_KIND_GETARRAY,
    LILY_MIR_INSTRUCTION_KIND_GETLIST,
    LILY_MIR_INSTRUCTION_KIND_GETSLICE,
    LILY_MIR_INSTRUCTION_KIND_GETFIELD,
    LILY_MIR_INSTRUCTION_KIND_GETPTR,
    LILY_MIR_INSTRUCTION_KIND_IADD,
    LILY_MIR_INSTRUCTION_KIND_ICMP_EQ,
    LILY_MIR_INSTRUCTION_KIND_ICMP_NE,
    LILY_MIR_INSTRUCTION_KIND_ICMP_LE,
    LILY_MIR_INSTRUCTION_KIND_ICMP_LT,
    LILY_MIR_INSTRUCTION_KIND_ICMP_GE,
    LILY_MIR_INSTRUCTION_KIND_ICMP_GT,
    LILY_MIR_INSTRUCTION_KIND_IDIV,
    LILY_MIR_INSTRUCTION_KIND_IMUL,
    LILY_MIR_INSTRUCTION_KIND_INCTRACE,
    LILY_MIR_INSTRUCTION_KIND_INEG,
    LILY_MIR_INSTRUCTION_KIND_ISOK,
    LILY_MIR_INSTRUCTION_KIND_ISERR,
    LILY_MIR_INSTRUCTION_KIND_ISUB,
    LILY_MIR_INSTRUCTION_KIND_JMP,
    LILY_MIR_INSTRUCTION_KIND_JMPCOND,
    LILY_MIR_INSTRUCTION_KIND_LEN,
    LILY_MIR_INSTRUCTION_KIND_LOAD,
    LILY_MIR_INSTRUCTION_KIND_LOOP,
    LILY_MIR_INSTRUCTION_KIND_MAKEREF,
    LILY_MIR_INSTRUCTION_KIND_MAKEOPT,
    LILY_MIR_INSTRUCTION_KIND_NEW,
    LILY_MIR_INSTRUCTION_KIND_NEWARRAY,
    LILY_MIR_INSTRUCTION_KIND_NEWLIST,
    LILY_MIR_INSTRUCTION_KIND_NEWSLICE,
    LILY_MIR_INSTRUCTION_KIND_NEWTRACE,
    LILY_MIR_INSTRUCTION_KIND_NIL,
    LILY_MIR_INSTRUCTION_KIND_NON_NIL,
    LILY_MIR_INSTRUCTION_KIND_NOT,
    LILY_MIR_INSTRUCTION_KIND_OR,
    LILY_MIR_INSTRUCTION_KIND_REM,
    LILY_MIR_INSTRUCTION_KIND_REF_PTR,
    LILY_MIR_INSTRUCTION_KIND_RET,
    LILY_MIR_INSTRUCTION_KIND_SHL,
    LILY_MIR_INSTRUCTION_KIND_SHR,
    LILY_MIR_INSTRUCTION_KIND_STORE,
    LILY_MIR_INSTRUCTION_KIND_SWITCH,
    LILY_MIR_INSTRUCTION_KIND_SYS_CALL,
    LILY_MIR_INSTRUCTION_KIND_TRUNC,
    LILY_MIR_INSTRUCTION_KIND_TRY,
    LILY_MIR_INSTRUCTION_KIND_TRY_PTR,
    LILY_MIR_INSTRUCTION_KIND_VAL,
    LILY_MIR_INSTRUCTION_KIND_XOR
};

typedef struct LilyMirInstruction LilyMirInstruction;

enum LilyMirInstructionValKind
{
    LILY_MIR_INSTRUCTION_VAL_KIND_ARRAY,
    LILY_MIR_INSTRUCTION_VAL_KIND_BYTES,
    LILY_MIR_INSTRUCTION_VAL_KIND_EXCEPTION,
    LILY_MIR_INSTRUCTION_VAL_KIND_INT,
    LILY_MIR_INSTRUCTION_VAL_KIND_FLOAT,
    LILY_MIR_INSTRUCTION_VAL_KIND_REG,
    LILY_MIR_INSTRUCTION_VAL_KIND_STR,
    LILY_MIR_INSTRUCTION_VAL_KIND_STRUCT,
    LILY_MIR_INSTRUCTION_VAL_KIND_TUPLE,
    LILY_MIR_INSTRUCTION_VAL_KIND_UINT,
    LILY_MIR_INSTRUCTION_VAL_KIND_UNDEF,
    LILY_MIR_INSTRUCTION_VAL_KIND_UNIT
};

// val(<dt>) <value>
typedef struct LilyMirInstructionVal
{
    enum LilyMirInstructionValKind kind;
    LilyMirDt *dt;
    union
    {
        Vec *array;         // Vec<LilyMirInstructionVal*>*
        const Uint8 *bytes; // const Uint8* (&)
        struct LilyMirInstructionVal *exception[2]; // [ok, err]
        Int64 int_;
        Float64 float_;
        const char *reg; // const char* (&)
        const char *str; // const char* (&)
        Vec *struct_;    // Vec<LilyMirInstructionVal*>*
        Vec *tuple;      // Vec<LilyMirInstructionVal*>*
        Uint64 uint;
    };
} LilyMirInstructionVal;

/**
 *
 * @brief Construct LilyMirInstructionVal type.
 */
CONSTRUCTOR(LilyMirInstructionVal *,
            LilyMirInstructionVal,
            enum LilyMirInstructionValKind kind,
            LilyMirDt *dt);

/**
 *
 * @brief Construct LilyMirInstructionVal type
 * (LILY_MIR_INSTRUCTION_VAL_KIND_ARRAY).
 */
VARIANT_CONSTRUCTOR(LilyMirInstructionVal *,
                    LilyMirInstructionVal,
                    array,
                    LilyMirDt *dt,
                    Vec *array);

/**
 *
 * @brief Construct LilyMirInstructionVal type
 * (LILY_MIR_INSTRUCTION_VAL_KIND_BYTES).
 */
VARIANT_CONSTRUCTOR(LilyMirInstructionVal *,
                    LilyMirInstructionVal,
                    bytes,
                    LilyMirDt *dt,
                    const Uint8 *bytes);

/**
 *
 * @brief Construct LilyMirInstructionVal type
 * (LILY_MIR_INSTRUCTION_VAL_KIND_BYTES).
 */
VARIANT_CONSTRUCTOR(LilyMirInstructionVal *,
                    LilyMirInstructionVal,
                    exception,
                    LilyMirDt *dt,
                    struct LilyMirInstructionVal *ok,
                    struct LilyMirInstructionVal *err);

/**
 *
 * @brief Construct LilyMirInstructionVal type
 * (LILY_MIR_INSTRUCTION_VAL_KIND_INT).
 */
VARIANT_CONSTRUCTOR(LilyMirInstructionVal *,
                    LilyMirInstructionVal,
                    int,
                    LilyMirDt *dt,
                    Int64 int_);

/**
 *
 * @brief Construct LilyMirInstructionVal type
 * (LILY_MIR_INSTRUCTION_VAL_KIND_FLOAT).
 */
VARIANT_CONSTRUCTOR(LilyMirInstructionVal *,
                    LilyMirInstructionVal,
                    float,
                    LilyMirDt *dt,
                    Float64 float_);

/**
 *
 * @brief Construct LilyMirInstructionVal type
 * (LILY_MIR_INSTRUCTION_VAL_KIND_REG).
 */
VARIANT_CONSTRUCTOR(LilyMirInstructionVal *,
                    LilyMirInstructionVal,
                    reg,
                    LilyMirDt *dt,
                    const char *reg);

/**
 *
 * @brief Construct LilyMirInstructionVal type
 * (LILY_MIR_INSTRUCTION_VAL_KIND_STR).
 */
VARIANT_CONSTRUCTOR(LilyMirInstructionVal *,
                    LilyMirInstructionVal,
                    str,
                    LilyMirDt *dt,
                    const char *str);

/**
 *
 * @brief Construct LilyMirInstructionVal type
 * (LILY_MIR_INSTRUCTION_VAL_KIND_STRUCT).
 */
VARIANT_CONSTRUCTOR(LilyMirInstructionVal *,
                    LilyMirInstructionVal,
                    struct,
                    LilyMirDt *dt,
                    Vec *struct_);

/**
 *
 * @brief Construct LilyMirInstructionVal type
 * (LILY_MIR_INSTRUCTION_VAL_KIND_TUPLE).
 */
VARIANT_CONSTRUCTOR(LilyMirInstructionVal *,
                    LilyMirInstructionVal,
                    tuple,
                    LilyMirDt *dt,
                    Vec *tuple);

/**
 *
 * @brief Construct LilyMirInstructionVal type
 * (LILY_MIR_INSTRUCTION_VAL_KIND_UINT).
 */
VARIANT_CONSTRUCTOR(LilyMirInstructionVal *,
                    LilyMirInstructionVal,
                    uint,
                    LilyMirDt *dt,
                    Uint64 uint);

/**
 *
 * @brief Free LilyMirInstructionVal type.
 */
DESTRUCTOR(LilyMirInstructionVal, LilyMirInstructionVal *self);

// and <val>, <val>
// bitand <val>, <val>
// bitnot <val>, <val>
// bitor <val>, <val>
// iadd <val>, <val>
// icmp eq <val>, <val>
// icmp ne <val>, <val>
// icmp le <val>, <val>
// icmp lt <val>, <val>
// icmp ge <val>, <val>
// icmp gt <val>, <val>
// idiv <val>, <val>
// isub <val>, <val>
// fcmp eq <val>, <val>
// fcmp ne <val>, <val>
// fcmp le <val>, <val>
// fcmp lt <val>, <val>
// fcmp ge <val>, <val>
// fcmp gt <val>, <val>
// exp <val>, <val>
// fdiv <val>, <val>
// fmul <val>, <val>
// frem <val>, <val>
// <inst> <dest>, <src>
typedef struct LilyMirInstructionSrcDest
{
    LilyMirInstructionVal *dest;
    LilyMirInstructionVal *src;
} LilyMirInstructionSrcDest;

/**
 *
 * @brief Construct LilyMirInstructionSrcDest type.
 */
inline CONSTRUCTOR(LilyMirInstructionSrcDest,
                   LilyMirInstructionSrcDest,
                   LilyMirInstructionVal *dest,
                   LilyMirInstructionVal *src)
{
    return (LilyMirInstructionSrcDest){ .dest = dest, .src = src };
}

/**
 *
 * @brief Free LilyMirInstructionSrcDest type.
 */
inline DESTRUCTOR(LilyMirInstructionSrcDest,
                  const LilyMirInstructionSrcDest *self)
{
    FREE(LilyMirInstructionVal, self->dest);
    FREE(LilyMirInstructionVal, self->src);
}

// drop <val>
// fneg <val>
// getarg <val>
// getarray <val>
// getlist <val>
// getslice <val>
// getfield <val>
// getptr <val>
// inctrace <val>
// <inst> <src>
typedef struct LilyMirInstructionSrc
{
    LilyMirInstructionVal *src;
} LilyMirInstructionSrc;

/**
 *
 * @brief Construct LilyMirInstructionSrc type.
 */
inline CONSTRUCTOR(LilyMirInstructionSrc,
                   LilyMirInstructionSrc,
                   LilyMirInstructionVal *src)
{
    return (LilyMirInstructionSrc){ .src = src };
}

/**
 *
 * @brief Free LilyMirInstructionSrc type.
 */
inline DESTRUCTOR(LilyMirInstructionSrc, const LilyMirInstructionSrc *self)
{
    FREE(LilyMirInstructionVal, self->src);
}

// alloc <dt>
typedef struct LilyMirInstructionAlloc
{
    LilyMirDt *dt;
} LilyMirInstructionAlloc;

/**
 *
 * @brief Construct LilyMirInstructionAlloc type.
 */
inline CONSTRUCTOR(LilyMirInstructionAlloc,
                   LilyMirInstructionAlloc,
                   LilyMirDt *dt)
{
    return (LilyMirInstructionAlloc){ .dt = dt };
}

/**
 *
 * @brief Free LilyMirInstructionAlloc type.
 */
inline DESTRUCTOR(LilyMirInstructionAlloc, const LilyMirInstructionAlloc *self)
{
    FREE(LilyMirDt, self->dt);
}

// arg(<dt>) <name>
typedef struct LilyMirInstructionArg
{
    LilyMirDt *dt;
    const char *name; // const char* (&)
} LilyMirInstructionArg;

/**
 *
 * @brief Construct LilyMirInstructionArg type.
 */
inline CONSTRUCTOR(LilyMirInstructionArg,
                   LilyMirInstructionArg,
                   LilyMirDt *dt,
                   const char *name)
{
    return (LilyMirInstructionArg){ .dt = dt, .name = name };
}

/**
 *
 * @brief Free LilyMirInstructionArg type.
 */
inline DESTRUCTOR(LilyMirInstructionArg, const LilyMirInstructionArg *self)
{
    FREE(LilyMirDt, self->dt);
}

// asm("xor eax, eax")
typedef struct LilyMirInstructionAsm
{
    const char *content; // const char* (&)
} LilyMirInstructionAsm;

/**
 *
 * @brief Construct LilyMirInstructionAsm type.
 */
inline CONSTRUCTOR(LilyMirInstructionAsm,
                   LilyMirInstructionAsm,
                   const char *content)
{
    return (LilyMirInstructionAsm){ .content = content };
}

// bitcast <val> -> <dt>
// <inst> <val> -> <dt>
typedef struct LilyMirInstructionValDt
{
    LilyMirInstructionVal *val;
    LilyMirDt *dt;
} LilyMirInstructionValDt;

/**
 *
 * @brief Construct LilyMirInstructionValDt type.
 */
inline CONSTRUCTOR(LilyMirInstructionValDt,
                   LilyMirInstructionValDt,
                   LilyMirInstructionVal *val,
                   LilyMirDt *dt)
{
    return (LilyMirInstructionValDt){ .val = val, .dt = dt };
}

/**
 *
 * @brief Free LilyMirInstructionValDt type.
 */
inline DESTRUCTOR(LilyMirInstructionValDt, const LilyMirInstructionValDt *self)
{
    FREE(LilyMirInstructionVal, self->val);
    FREE(LilyMirDt, self->dt);
}

typedef struct LilyMirInstructionBlock
{
    char *name;
    Vec *insts; // Vec<LilyMirInstruction*>*
} LilyMirInstructionBlock;

/**
 *
 * @brief Construct LilyMirInstructionBlock type.
 */
inline CONSTRUCTOR(LilyMirInstructionBlock,
                   LilyMirInstructionBlock,
                   char *name,
                   Vec *insts)
{
    return (LilyMirInstructionBlock){ .name = name, .insts = insts };
}

/**
 *
 * @brief Free LilyMirInstructionBlock type.
 */
DESTRUCTOR(LilyMirInstructionBlock, const LilyMirInstructionBlock *self);

typedef struct LilyMirInstructionCall
{
    const char *name; // const char* (&)
    Vec *params;      // Vec<LilyMirInstruction*>*
} LilyMirInstructionCall;

/**
 *
 * @brief Construct LilyMirInstructionCall type.
 */
inline CONSTRUCTOR(LilyMirInstructionCall,
                   LilyMirInstructionCall,
                   const char *name,
                   Vec *params)
{
    return (LilyMirInstructionCall){ .name = name, .params = params };
}

/**
 *
 * @brief Free LilyMirInstructionCall type.
 */
DESTRUCTOR(LilyMirInstructionCall, const LilyMirInstructionCall *self);

// <priv|pub> const <name> = <val>
typedef struct LilyMirInstructionConst
{
    enum LilyMirLinkage linkage;
    const char *name; // const char* (&)
    LilyMirInstructionVal *val;
} LilyMirInstructionConst;

/**
 *
 * @brief Construct LilyMirInstructionConst type.
 */
inline CONSTRUCTOR(LilyMirInstructionConst,
                   LilyMirInstructionConst,
                   enum LilyMirLinkage linkage,
                   const char *name,
                   LilyMirInstructionVal *val)
{
    return (
      LilyMirInstructionConst){ .linkage = linkage, .name = name, .val = val };
}

/**
 *
 * @brief Free LilyMirInstructionConst type.
 */
inline DESTRUCTOR(LilyMirInstructionConst, const LilyMirInstructionConst *self)
{
    FREE(LilyMirInstructionVal, self->val);
}

typedef struct LilyMirInstructionFun
{
    enum LilyMirLinkage linkage;
    const char *name; // const char* (&)
    Vec *args;        // Vec<LilyMirInstruction*>*
    Vec *insts;       // Vec<LilyMirInstruction*>*
} LilyMirInstructionFun;

/**
 *
 * @brief Construct LilyMirInstructionFun type.
 */
inline CONSTRUCTOR(LilyMirInstructionFun,
                   LilyMirInstructionFun,
                   enum LilyMirLinkage linkage,
                   const char *name,
                   Vec *args,
                   Vec *insts)
{
    return (LilyMirInstructionFun){
        .linkage = linkage, .name = name, .args = args, .insts = insts
    };
}

/**
 *
 * @brief Free LilyMirInstructionFun type.
 */
DESTRUCTOR(LilyMirInstructionFun, const LilyMirInstructionFun *self);

typedef struct LilyMirInstructionJmpCond
{
    LilyMirInstructionVal *cond;
    LilyMirInstructionBlock *block; // LilyMirInstructionBlock* (&)
} LilyMirInstructionJmpCond;

/**
 *
 * @brief Construct LilyMirInstructionJmpCond type.
 */
CONSTRUCTOR(LilyMirInstructionJmpCond,
            LilyMirInstructionJmpCond,
            LilyMirInstructionVal *cond,
            LilyMirInstructionBlock *block)
{
    return (LilyMirInstructionJmpCond){ .cond = cond, .block = block };
}

/**
 *
 * @brief Free LilyMirInstructionJmpCond type.
 */
inline DESTRUCTOR(LilyMirInstructionJmpCond,
                  const LilyMirInstructionJmpCond *self)
{
    FREE(LilyMirInstructionVal, self->cond);
}

typedef struct LilyMirInstructionSwitchCase
{
    LilyMirInstructionVal *val;
    LilyMirInstructionBlock *block_dest; // LilyMirInstructionBlock* (&)
} LilyMirInstructionSwitchCase;

/**
 *
 * @brief Construct LilyMirInstructionSwitchCase type.
 */
CONSTRUCTOR(LilyMirInstructionSwitchCase *,
            LilyMirInstructionSwitchCase,
            LilyMirInstructionVal *val,
            LilyMirInstructionBlock *block_dest);

/**
 *
 * @brief Free LilyMirInstructionSwitchCase type.
 */
DESTRUCTOR(LilyMirInstructionSwitchCase, LilyMirInstructionSwitchCase *self);

typedef struct LilyMirInstructionSwitch
{
    LilyMirInstructionVal *val;
    LilyMirInstructionBlock *default_block;
    Vec *cases; // Vec<LilyMirInstructionSwitchCase*>*
} LilyMirInstructionSwitch;

/**
 *
 * @brief Construct LilyMirInstructionSwitch type.
 */
inline CONSTRUCTOR(LilyMirInstructionSwitch,
                   LilyMirInstructionSwitch,
                   LilyMirInstructionVal *val,
                   LilyMirInstructionBlock *default_block,
                   Vec *cases)
{
    return (LilyMirInstructionSwitch){ .val = val,
                                       .default_block = default_block,
                                       .cases = cases };
}

/**
 *
 * @brief Construct LilyMirInstructionSwitch type.
 */
DESTRUCTOR(LilyMirInstructionSwitch, const LilyMirInstructionSwitch *self);

typedef struct LilyMirInstructionTry
{
    LilyMirInstructionVal *val;
    LilyMirInstructionBlock *try_block;   // LilyMirInstructionBlock* (&)
    LilyMirInstructionBlock *catch_block; // LilyMirInstructionBlock* (&)
} LilyMirInstructionTry;

/**
 *
 * @brief Construct LilyMirInstructionTry type.
 */
CONSTRUCTOR(LilyMirInstructionTry,
            LilyMirInstructionTry,
            LilyMirInstructionVal *val,
            LilyMirInstructionBlock *try_block,
            LilyMirInstructionBlock *catch_block)
{
    return (LilyMirInstructionTry){ .val = val,
                                    .try_block = try_block,
                                    .catch_block = catch_block };
}

/**
 *
 * @brief Free LilyMirInstructionTry type.
 */
inline DESTRUCTOR(LilyMirInstructionTry, const LilyMirInstructionTry *self)
{
    FREE(LilyMirInstructionVal, self->val);
}

typedef struct LilyMirInstruction
{
    enum LilyMirInstructionKind kind;
    union
    {
        LilyMirInstructionAlloc alloc;
        LilyMirInstructionSrcDest and;
        LilyMirInstructionArg arg;
        LilyMirInstructionAsm asm;
        LilyMirInstructionValDt bitcast;
        LilyMirInstructionSrcDest bitand;
        LilyMirInstructionSrcDest bitnot;
        LilyMirInstructionSrcDest bitor ;
        LilyMirInstructionBlock block;
        LilyMirInstructionCall builtin_call;
        LilyMirInstructionCall call;
        LilyMirInstructionConst const_;
        LilyMirInstructionSrc drop;
        LilyMirInstructionSrcDest exp;
        LilyMirInstructionSrcDest fadd;
        LilyMirInstructionSrcDest fcmp_eq;
        LilyMirInstructionSrcDest fcmp_ne;
        LilyMirInstructionSrcDest fcmp_le;
        LilyMirInstructionSrcDest fcmp_lt;
        LilyMirInstructionSrcDest fcmp_ge;
        LilyMirInstructionSrcDest fcmp_gt;
        LilyMirInstructionSrcDest fdiv;
        LilyMirInstructionSrcDest fmul;
        LilyMirInstructionSrc fneg;
        LilyMirInstructionSrcDest frem;
        LilyMirInstructionFun fun;
        LilyMirInstructionSrc getarg;
        LilyMirInstructionSrc getarray;
        LilyMirInstructionSrc getlist;
        LilyMirInstructionSrc getslice;
        LilyMirInstructionSrc getfield;
        LilyMirInstructionSrc getptr;
        LilyMirInstructionSrcDest iadd;
        LilyMirInstructionSrcDest icmp_eq;
        LilyMirInstructionSrcDest icmp_ne;
        LilyMirInstructionSrcDest icmp_le;
        LilyMirInstructionSrcDest icmp_lt;
        LilyMirInstructionSrcDest icmp_ge;
        LilyMirInstructionSrcDest icmp_gt;
        LilyMirInstructionSrcDest idiv;
        LilyMirInstructionSrcDest imul;
        LilyMirInstructionSrc inctrace;
        LilyMirInstructionSrc ineg;
        LilyMirInstructionSrc isok;
        LilyMirInstructionSrc iserr;
        LilyMirInstructionSrcDest isub;
        LilyMirInstructionBlock *jmp;
        LilyMirInstructionJmpCond jmpcond;
        LilyMirInstructionSrc len;
        LilyMirInstructionSrc load;
        LilyMirInstructionBlock loop;
        LilyMirInstructionSrc makeref;
        LilyMirInstructionSrc makeopt;
        Vec *new;      // Vec<LilyMirInstructionVal*>*
        Vec *newarray; // Vec<LilyMirInstructionVal*>*
        Vec *newlist;  // Vec<LilyMirInstructionVal*>*
        LilyMirInstructionSrc newtrace;
        LilyMirInstruction *non_nil;
        LilyMirInstructionSrc not ;
        LilyMirInstructionSrcDest or ;
        LilyMirInstructionSrcDest rem;
        LilyMirInstructionSrc ref_ptr;
        LilyMirInstructionSrc ret;
        LilyMirInstructionSrcDest shl;
        LilyMirInstructionSrcDest shr;
        LilyMirInstructionSrcDest store;
        LilyMirInstructionSwitch switch_;
        LilyMirInstructionCall sys_call;
        LilyMirInstructionValDt trunc;
        LilyMirInstructionTry try;
        LilyMirInstructionTry try_ptr;
        LilyMirInstructionVal val;
        LilyMirInstructionSrcDest xor ;
    };
} LilyMirInstruction;

/**
 *
 * @brief Free LilyMirInstruction type.
 */
DESTRUCTOR(LilyMirInstruction, LilyMirInstruction *self);

#endif // LILY_CORE_LILY_IR_INSTRUCTION_H
