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
    LILY_MIR_INSTRUCTION_KIND_IREM,
    LILY_MIR_INSTRUCTION_KIND_ISOK,
    LILY_MIR_INSTRUCTION_KIND_ISERR,
    LILY_MIR_INSTRUCTION_KIND_ISUB,
    LILY_MIR_INSTRUCTION_KIND_JMP,
    LILY_MIR_INSTRUCTION_KIND_JMPCOND,
    LILY_MIR_INSTRUCTION_KIND_LEN,
    LILY_MIR_INSTRUCTION_KIND_LOAD,
    LILY_MIR_INSTRUCTION_KIND_MAKEREF,
    LILY_MIR_INSTRUCTION_KIND_MAKEOPT,
    LILY_MIR_INSTRUCTION_KIND_NON_NIL,
    LILY_MIR_INSTRUCTION_KIND_NOT,
    LILY_MIR_INSTRUCTION_KIND_OR,
    LILY_MIR_INSTRUCTION_KIND_REF_PTR,
    LILY_MIR_INSTRUCTION_KIND_REG,
    LILY_MIR_INSTRUCTION_KIND_RET,
    LILY_MIR_INSTRUCTION_KIND_SHL,
    LILY_MIR_INSTRUCTION_KIND_SHR,
    LILY_MIR_INSTRUCTION_KIND_STORE,
    LILY_MIR_INSTRUCTION_KIND_STRUCT,
    LILY_MIR_INSTRUCTION_KIND_SWITCH,
    LILY_MIR_INSTRUCTION_KIND_SYS_CALL,
    LILY_MIR_INSTRUCTION_KIND_TRUNC,
    LILY_MIR_INSTRUCTION_KIND_TRY,
    LILY_MIR_INSTRUCTION_KIND_TRY_PTR,
    LILY_MIR_INSTRUCTION_KIND_VAL,
    LILY_MIR_INSTRUCTION_KIND_XOR
};

typedef struct LilyMirInstruction LilyMirInstruction;

DESTRUCTOR(LilyMirInstruction, LilyMirInstruction *self);

enum LilyMirInstructionValKind
{
    LILY_MIR_INSTRUCTION_VAL_KIND_ARRAY,
    LILY_MIR_INSTRUCTION_VAL_KIND_BYTES,
    LILY_MIR_INSTRUCTION_VAL_KIND_EXCEPTION,
    LILY_MIR_INSTRUCTION_VAL_KIND_FLOAT,
    LILY_MIR_INSTRUCTION_VAL_KIND_INT,
    LILY_MIR_INSTRUCTION_VAL_KIND_LIST,
    LILY_MIR_INSTRUCTION_VAL_KIND_NIL,
    LILY_MIR_INSTRUCTION_VAL_KIND_REG,
    LILY_MIR_INSTRUCTION_VAL_KIND_SLICE,
    LILY_MIR_INSTRUCTION_VAL_KIND_STR,
    LILY_MIR_INSTRUCTION_VAL_KIND_STRUCT,
    LILY_MIR_INSTRUCTION_VAL_KIND_TRACE,
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
        struct LilyMirInstructionVal *exception[2]; // [ok?, err?]
        Float64 float_;
        Int64 int_;
        Vec *list;       // Vec<LilyMirInstructionVal*>*
        const char *reg; // const char* (&)
        Vec *slice;      // Vec<LilyMirInstructionVal*>*
        const char *str; // const char* (&)
        Vec *struct_;    // Vec<LilyMirInstructionVal*>*
        struct LilyMirInstructionVal *trace;
        Vec *tuple; // Vec<LilyMirInstructionVal*>*
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
 * (LILY_MIR_INSTRUCTION_VAL_KIND_LIST).
 */
VARIANT_CONSTRUCTOR(LilyMirInstructionVal *,
                    LilyMirInstructionVal,
                    list,
                    LilyMirDt *dt,
                    Vec *list);

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
 * (LILY_MIR_INSTRUCTION_VAL_KIND_SLICE).
 */
VARIANT_CONSTRUCTOR(LilyMirInstructionVal *,
                    LilyMirInstructionVal,
                    slice,
                    LilyMirDt *dt,
                    Vec *slice);

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
 * (LILY_MIR_INSTRUCTION_VAL_KIND_TRACE).
 */
VARIANT_CONSTRUCTOR(LilyMirInstructionVal *,
                    LilyMirInstructionVal,
                    trace,
                    LilyMirDt *dt,
                    LilyMirInstructionVal *trace);

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
 * @brief Convert LilyMirInstructionVal in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyMirInstructionVal,
               const LilyMirInstructionVal *self);
#endif

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
 * @brief Convert LilyMirInstructionSrcDest in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyMirInstructionSrcDest,
               const LilyMirInstructionSrcDest *self);
#endif

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
 * @brief Convert LilyMirInstructionSrc in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyMirInstructionSrc,
               const LilyMirInstructionSrc *self);
#endif

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
 * @brief Convert LilyMirInstructionAlloc in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyMirInstructionAlloc,
               const LilyMirInstructionAlloc *self);
#endif

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
 * @brief Convert LilyMirInstructionArg in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyMirInstructionArg,
               const LilyMirInstructionArg *self);
#endif

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

/**
 *
 * @brief Convert LilyMirInstructionAsm in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyMirInstructionAsm,
               const LilyMirInstructionAsm *self);
#endif

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
 * @brief Convert LilyMirInstructionValDt in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyMirInstructionValDt,
               const LilyMirInstructionValDt *self);
#endif

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
    const char *name; // const char* (&)
    Vec *insts;       // Vec<LilyMirInstruction*>*
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
 * @brief Convert LilyMirInstructionBlock in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyMirInstructionBlock,
               const LilyMirInstructionBlock *self);
#endif

/**
 *
 * @brief Free LilyMirInstructionBlock type.
 */
DESTRUCTOR(LilyMirInstructionBlock, const LilyMirInstructionBlock *self);

typedef struct LilyMirInstructionCall
{
    const char *name; // const char* (&)
    Vec *params;      // Vec<LilyMirInstructionVal*>*
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
 * @brief Convert LilyMirInstructionCall in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyMirInstructionCall,
               const LilyMirInstructionCall *self);
#endif

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
 * @brief Convert LilyMirInstructionConst in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyMirInstructionConst,
               const LilyMirInstructionConst *self);
#endif

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
 * @brief Convert LilyMirInstructionFun in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyMirInstructionFun,
               const LilyMirInstructionFun *self);
#endif

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
inline CONSTRUCTOR(LilyMirInstructionJmpCond,
                   LilyMirInstructionJmpCond,
                   LilyMirInstructionVal *cond,
                   LilyMirInstructionBlock *block)
{
    return (LilyMirInstructionJmpCond){ .cond = cond, .block = block };
}

/**
 *
 * @brief Convert LilyMirInstructionJmpCond in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyMirInstructionJmpCond,
               const LilyMirInstructionJmpCond *self);
#endif

/**
 *
 * @brief Free LilyMirInstructionJmpCond type.
 */
inline DESTRUCTOR(LilyMirInstructionJmpCond,
                  const LilyMirInstructionJmpCond *self)
{
    FREE(LilyMirInstructionVal, self->cond);
}

typedef struct LilyMirInstructionReg
{
    char *name;
    LilyMirInstruction *inst;
} LilyMirInstructionReg;

/**
 *
 * @brief Construct LilyMirInstructionReg type.
 */
inline CONSTRUCTOR(LilyMirInstructionReg,
                   LilyMirInstructionReg,
                   char *name,
                   LilyMirInstruction *inst)
{
    return (LilyMirInstructionReg){ .name = name, .inst = inst };
}

/**
 *
 * @brief Convert LilyMirInstructionReg in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyMirInstructionReg,
               const LilyMirInstructionReg *self);
#endif

/**
 *
 * @brief Free LilyMirInstructionReg type.
 */
inline DESTRUCTOR(LilyMirInstructionReg, const LilyMirInstructionReg *self)
{
    lily_free(self->name);
    FREE(LilyMirInstruction, self->inst);
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
 * @brief Convert LilyMirInstructionSwitchCase in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyMirInstructionSwitchCase,
               const LilyMirInstructionSwitchCase *self);
#endif

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
 * @brief Convert LilyMirInstructionSwitch in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyMirInstructionSwitch,
               const LilyMirInstructionSwitch *self);
#endif

/**
 *
 * @brief Construct LilyMirInstructionSwitch type.
 */
DESTRUCTOR(LilyMirInstructionSwitch, const LilyMirInstructionSwitch *self);

typedef struct LilyMirInstructionTry
{
    LilyMirInstructionVal *val;
    LilyMirInstructionBlock *try_block; // LilyMirInstructionBlock* (&)
    LilyMirInstructionVal *catch_val;
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
            LilyMirInstructionVal *catch_val,
            LilyMirInstructionBlock *catch_block)
{
    return (LilyMirInstructionTry){ .val = val,
                                    .try_block = try_block,
                                    .catch_val = catch_val,
                                    .catch_block = catch_block };
}

/**
 *
 * @brief Convert LilyMirInstructionTry in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyMirInstructionTry,
               const LilyMirInstructionTry *self);
#endif

/**
 *
 * @brief Free LilyMirInstructionTry type.
 */
inline DESTRUCTOR(LilyMirInstructionTry, const LilyMirInstructionTry *self)
{
    FREE(LilyMirInstructionVal, self->val);
}

typedef struct LilyMirInstructionStruct
{
    enum LilyMirLinkage linkage;
    const char *name;
    Vec *fields; // Vec<LilyMirDt*>*
} LilyMirInstructionStruct;

/**
 *
 * @brief Construct LilyMirInstructionStruct type.
 */
inline CONSTRUCTOR(LilyMirInstructionStruct,
                   LilyMirInstructionStruct,
                   enum LilyMirLinkage linkage,
                   const char *name,
                   Vec *fields)
{
    return (LilyMirInstructionStruct){ .linkage = linkage,
                                       .name = name,
                                       .fields = fields };
}

/**
 *
 * @brief Convert LilyMirInstructionStruct in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyMirInstructionStruct,
               const LilyMirInstructionStruct *self);
#endif

/**
 *
 * @brief Free LilyMirInstructionStruct type.
 */
DESTRUCTOR(LilyMirInstructionStruct, const LilyMirInstructionStruct *self);

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
        LilyMirInstructionSrc bitnot;
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
        LilyMirInstructionSrc getarray;
        LilyMirInstructionSrc getarg;
        LilyMirInstructionSrc getfield;
        LilyMirInstructionSrc getlist;
        LilyMirInstructionSrc getptr;
        LilyMirInstructionSrc getslice;
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
        LilyMirInstructionSrcDest irem;
        LilyMirInstructionSrc isok;
        LilyMirInstructionSrc iserr;
        LilyMirInstructionSrcDest isub;
        LilyMirInstructionBlock *jmp; // LilyMirInstructionBlock* (&)
        LilyMirInstructionJmpCond jmpcond;
        LilyMirInstructionSrc len;
        LilyMirInstructionSrc load;
        LilyMirInstructionSrc makeref;
        LilyMirInstructionSrc makeopt;
        LilyMirInstruction *non_nil;
        LilyMirInstructionSrc not ;
        LilyMirInstructionSrcDest or ;
        LilyMirInstructionReg reg;
        LilyMirInstructionSrc ref_ptr;
        LilyMirInstruction *ret;
        LilyMirInstructionSrcDest shl;
        LilyMirInstructionSrcDest shr;
        LilyMirInstructionSrcDest store;
        LilyMirInstructionStruct struct_;
        LilyMirInstructionSwitch switch_;
        LilyMirInstructionCall sys_call;
        LilyMirInstructionValDt trunc;
        LilyMirInstructionTry try;
        LilyMirInstructionTry try_ptr;
        LilyMirInstructionVal *val;
        LilyMirInstructionSrcDest xor ;
    };
} LilyMirInstruction;

/**
 *
 * @brief Construct LilyMirInstruction type (LILY_MIR_INSTRUCTION_KIND_ALLOC).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    alloc,
                    LilyMirInstructionAlloc alloc);

/**
 *
 * @brief Construct LilyMirInstruction type (LILY_MIR_INSTRUCTION_KIND_AND).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    and,
                    LilyMirInstructionSrcDest and);

/**
 *
 * @brief Construct LilyMirInstruction type (LILY_MIR_INSTRUCTION_KIND_ARG).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    arg,
                    LilyMirInstructionArg arg);

/**
 *
 * @brief Construct LilyMirInstruction type (LILY_MIR_INSTRUCTION_KIND_ASM).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    asm,
                    LilyMirInstructionAsm asm);

/**
 *
 * @brief Construct LilyMirInstruction type (LILY_MIR_INSTRUCTION_KIND_BITCAST).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    bitcast,
                    LilyMirInstructionValDt bitcast);

/**
 *
 * @brief Construct LilyMirInstruction type (LILY_MIR_INSTRUCTION_KIND_BITAND).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                      bitand,
                    LilyMirInstructionSrcDest bitand);

/**
 *
 * @brief Construct LilyMirInstruction type (LILY_MIR_INSTRUCTION_KIND_BITNOT).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    bitnot,
                    LilyMirInstructionSrc bitnot);

/**
 *
 * @brief Construct LilyMirInstruction type (LILY_MIR_INSTRUCTION_KIND_BITOR).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    bitor
                    ,
                    LilyMirInstructionSrcDest bitor);

/**
 *
 * @brief Construct LilyMirInstruction type (LILY_MIR_INSTRUCTION_KIND_BLOCK).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    block,
                    LilyMirInstructionBlock block);

/**
 *
 * @brief Construct LilyMirInstruction type
 * (LILY_MIR_INSTRUCTION_KIND_BUILTIN_CALL).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    builtin_call,
                    LilyMirInstructionCall builtin_call);

/**
 *
 * @brief Construct LilyMirInstruction type (LILY_MIR_INSTRUCTION_KIND_CALL).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    call,
                    LilyMirInstructionCall call);

/**
 *
 * @brief Construct LilyMirInstruction type (LILY_MIR_INSTRUCTION_KIND_CONST).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    const,
                    LilyMirInstructionConst const_);

/**
 *
 * @brief Construct LilyMirInstruction type (LILY_MIR_INSTRUCTION_KIND_DROP).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    drop,
                    LilyMirInstructionSrc drop);

/**
 *
 * @brief Construct LilyMirInstruction type (LILY_MIR_INSTRUCTION_KIND_EXP).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    exp,
                    LilyMirInstructionSrcDest exp);

/**
 *
 * @brief Construct LilyMirInstruction type (LILY_MIR_INSTRUCTION_KIND_FADD).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    fadd,
                    LilyMirInstructionSrcDest fadd);

/**
 *
 * @brief Construct LilyMirInstruction type (LILY_MIR_INSTRUCTION_KIND_FCMP_EQ).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    fcmp_eq,
                    LilyMirInstructionSrcDest fcmp_eq);

/**
 *
 * @brief Construct LilyMirInstruction type (LILY_MIR_INSTRUCTION_KIND_FCMP_NE).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    fcmp_ne,
                    LilyMirInstructionSrcDest fcmp_ne);

/**
 *
 * @brief Construct LilyMirInstruction type (LILY_MIR_INSTRUCTION_KIND_FCMP_LE).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    fcmp_le,
                    LilyMirInstructionSrcDest fcmp_le);

/**
 *
 * @brief Construct LilyMirInstruction type (LILY_MIR_INSTRUCTION_KIND_FCMP_LT).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    fcmp_lt,
                    LilyMirInstructionSrcDest fcmp_lt);

/**
 *
 * @brief Construct LilyMirInstruction type (LILY_MIR_INSTRUCTION_KIND_FCMP_GE).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    fcmp_ge,
                    LilyMirInstructionSrcDest fcmp_ge);

/**
 *
 * @brief Construct LilyMirInstruction type (LILY_MIR_INSTRUCTION_KIND_FCMP_GT).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    fcmp_gt,
                    LilyMirInstructionSrcDest fcmp_gt);

/**
 *
 * @brief Construct LilyMirInstruction type (LILY_MIR_INSTRUCTION_KIND_FDIV).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    fdiv,
                    LilyMirInstructionSrcDest fdiv);

/**
 *
 * @brief Construct LilyMirInstruction type (LILY_MIR_INSTRUCTION_KIND_FMUL).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    fmul,
                    LilyMirInstructionSrcDest fmul);

/**
 *
 * @brief Construct LilyMirInstruction type (LILY_MIR_INSTRUCTION_KIND_FNEG).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    fneg,
                    LilyMirInstructionSrc fneg);

/**
 *
 * @brief Construct LilyMirInstruction type (LILY_MIR_INSTRUCTION_KIND_FREM).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    frem,
                    LilyMirInstructionSrcDest frem);

/**
 *
 * @brief Construct LilyMirInstruction type (LILY_MIR_INSTRUCTION_KIND_FUN).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    fun,
                    LilyMirInstructionFun fun);

/**
 *
 * @brief Construct LilyMirInstruction type
 * (LILY_MIR_INSTRUCTION_KIND_GETARRAY).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    getarray,
                    LilyMirInstructionSrc getarray);

/**
 *
 * @brief Construct LilyMirInstruction type (LILY_MIR_INSTRUCTION_KIND_GETARG).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    getarg,
                    LilyMirInstructionSrc getarg);

/**
 *
 * @brief Construct LilyMirInstruction type
 * (LILY_MIR_INSTRUCTION_KIND_GETFIELD).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    getfield,
                    LilyMirInstructionSrc getfield);

/**
 *
 * @brief Construct LilyMirInstruction type
 * (LILY_MIR_INSTRUCTION_KIND_GETLIST).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    getlist,
                    LilyMirInstructionSrc getlist);

/**
 *
 * @brief Construct LilyMirInstruction type
 * (LILY_MIR_INSTRUCTION_KIND_GETPTR).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    getptr,
                    LilyMirInstructionSrc getptr);

/**
 *
 * @brief Construct LilyMirInstruction type
 * (LILY_MIR_INSTRUCTION_KIND_GETSLICE).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    getslice,
                    LilyMirInstructionSrc getslice);

/**
 *
 * @brief Construct LilyMirInstruction type
 * (LILY_MIR_INSTRUCTION_KIND_IADD).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    iadd,
                    LilyMirInstructionSrcDest iadd);

/**
 *
 * @brief Construct LilyMirInstruction type
 * (LILY_MIR_INSTRUCTION_KIND_ICMP_EQ).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    icmp_eq,
                    LilyMirInstructionSrcDest icmp_eq);

/**
 *
 * @brief Construct LilyMirInstruction type
 * (LILY_MIR_INSTRUCTION_KIND_ICMP_NE).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    icmp_ne,
                    LilyMirInstructionSrcDest icmp_ne);

/**
 *
 * @brief Construct LilyMirInstruction type
 * (LILY_MIR_INSTRUCTION_KIND_ICMP_LE).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    icmp_le,
                    LilyMirInstructionSrcDest icmp_le);

/**
 *
 * @brief Construct LilyMirInstruction type
 * (LILY_MIR_INSTRUCTION_KIND_ICMP_LT).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    icmp_lt,
                    LilyMirInstructionSrcDest icmp_lt);

/**
 *
 * @brief Construct LilyMirInstruction type
 * (LILY_MIR_INSTRUCTION_KIND_ICMP_GE).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    icmp_ge,
                    LilyMirInstructionSrcDest icmp_ge);

/**
 *
 * @brief Construct LilyMirInstruction type
 * (LILY_MIR_INSTRUCTION_KIND_ICMP_GT).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    icmp_gt,
                    LilyMirInstructionSrcDest icmp_gt);

/**
 *
 * @brief Construct LilyMirInstruction type
 * (LILY_MIR_INSTRUCTION_KIND_IDIV).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    idiv,
                    LilyMirInstructionSrcDest idiv);

/**
 *
 * @brief Construct LilyMirInstruction type
 * (LILY_MIR_INSTRUCTION_KIND_IMUL).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    imul,
                    LilyMirInstructionSrcDest imul);

/**
 *
 * @brief Construct LilyMirInstruction type
 * (LILY_MIR_INSTRUCTION_KIND_INCTRACE).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    inctrace,
                    LilyMirInstructionSrcDest inctrace);

/**
 *
 * @brief Construct LilyMirInstruction type
 * (LILY_MIR_INSTRUCTION_KIND_INEG).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    ineg,
                    LilyMirInstructionSrc ineg);

/**
 *
 * @brief Construct LilyMirInstruction type
 * (LILY_MIR_INSTRUCTION_KIND_IREM).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    irem,
                    LilyMirInstructionSrcDest irem);

/**
 *
 * @brief Construct LilyMirInstruction type
 * (LILY_MIR_INSTRUCTION_KIND_ISOK).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    isok,
                    LilyMirInstructionSrc isok);

/**
 *
 * @brief Construct LilyMirInstruction type
 * (LILY_MIR_INSTRUCTION_KIND_ISERR).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    iserr,
                    LilyMirInstructionSrc iserr);

/**
 *
 * @brief Construct LilyMirInstruction type
 * (LILY_MIR_INSTRUCTION_KIND_ISUB).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    isub,
                    LilyMirInstructionSrcDest isub);

/**
 *
 * @brief Construct LilyMirInstruction type
 * (LILY_MIR_INSTRUCTION_KIND_JMP).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    jmp,
                    LilyMirInstructionBlock *jmp);

/**
 *
 * @brief Construct LilyMirInstruction type
 * (LILY_MIR_INSTRUCTION_KIND_JMPCOND).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    jmpcond,
                    LilyMirInstructionJmpCond jmpcond);

/**
 *
 * @brief Construct LilyMirInstruction type
 * (LILY_MIR_INSTRUCTION_KIND_LEN).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    len,
                    LilyMirInstructionSrc len);

/**
 *
 * @brief Construct LilyMirInstruction type
 * (LILY_MIR_INSTRUCTION_KIND_LOAD).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    load,
                    LilyMirInstructionSrc load);

/**
 *
 * @brief Construct LilyMirInstruction type
 * (LILY_MIR_INSTRUCTION_KIND_LOOP).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    loop,
                    LilyMirInstructionBlock loop);

/**
 *
 * @brief Construct LilyMirInstruction type
 * (LILY_MIR_INSTRUCTION_KIND_MAKEREF).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    makeref,
                    LilyMirInstructionSrc makeref);

/**
 *
 * @brief Construct LilyMirInstruction type
 * (LILY_MIR_INSTRUCTION_KIND_MAKEOPT).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    makeopt,
                    LilyMirInstructionSrc makeopt);

/**
 *
 * @brief Construct LilyMirInstruction type
 * (LILY_MIR_INSTRUCTION_KIND_NON_NIL).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    non_nil,
                    LilyMirInstruction *non_nil);

/**
 *
 * @brief Construct LilyMirInstruction type
 * (LILY_MIR_INSTRUCTION_KIND_NOT).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    not,
                    LilyMirInstructionSrc not );

/**
 *
 * @brief Construct LilyMirInstruction type
 * (LILY_MIR_INSTRUCTION_KIND_OR).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    or
                    ,
                    LilyMirInstructionSrcDest or);

/**
 *
 * @brief Construct LilyMirInstruction type
 * (LILY_MIR_INSTRUCTION_KIND_REG).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    reg,
                    LilyMirInstructionReg reg);

/**
 *
 * @brief Construct LilyMirInstruction type
 * (LILY_MIR_INSTRUCTION_KIND_REF_PTR).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    ref_ptr,
                    LilyMirInstructionSrc ref_ptr);

/**
 *
 * @brief Construct LilyMirInstruction type
 * (LILY_MIR_INSTRUCTION_KIND_RET).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    ret,
                    LilyMirInstruction *ret);

/**
 *
 * @brief Construct LilyMirInstruction type
 * (LILY_MIR_INSTRUCTION_KIND_SHL).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    shl,
                    LilyMirInstructionSrcDest shl);

/**
 *
 * @brief Construct LilyMirInstruction type
 * (LILY_MIR_INSTRUCTION_KIND_SHR).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    shr,
                    LilyMirInstructionSrcDest shr);

/**
 *
 * @brief Construct LilyMirInstruction type
 * (LILY_MIR_INSTRUCTION_KIND_STORE).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    store,
                    LilyMirInstructionSrcDest store);

/**
 *
 * @brief Construct LilyMirInstruction type
 * (LILY_MIR_INSTRUCTION_KIND_STRUCT).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    struct,
                    LilyMirInstructionStruct struct_);

/**
 *
 * @brief Construct LilyMirInstruction type
 * (LILY_MIR_INSTRUCTION_KIND_SWITCH).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    switch,
                    LilyMirInstructionSwitch switch_);

/**
 *
 * @brief Construct LilyMirInstruction type
 * (LILY_MIR_INSTRUCTION_KIND_SYS_CALL).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    sys_call,
                    LilyMirInstructionCall sys_call);

/**
 *
 * @brief Construct LilyMirInstruction type
 * (LILY_MIR_INSTRUCTION_KIND_TRUNC).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    trunc,
                    LilyMirInstructionValDt trunc);

/**
 *
 * @brief Construct LilyMirInstruction type
 * (LILY_MIR_INSTRUCTION_KIND_TRY).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    try,
                    LilyMirInstructionTry try);

/**
 *
 * @brief Construct LilyMirInstruction type
 * (LILY_MIR_INSTRUCTION_KIND_TRY_PTR).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    try_ptr,
                    LilyMirInstructionTry try_ptr);

/**
 *
 * @brief Construct LilyMirInstruction type
 * (LILY_MIR_INSTRUCTION_KIND_VAL).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    val,
                    LilyMirInstructionVal *val);

/**
 *
 * @brief Construct LilyMirInstruction type
 * (LILY_MIR_INSTRUCTION_KIND_XOR).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    xor,
                    LilyMirInstructionSrcDest xor);

/**
 *
 * @brief Convert LilyMirInstruction in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyMirInstruction, const LilyMirInstruction *self);
#endif

/**
 *
 * @brief Free LilyMirInstruction type.
 */
DESTRUCTOR(LilyMirInstruction, LilyMirInstruction *self);

#endif // LILY_CORE_LILY_IR_INSTRUCTION_H
