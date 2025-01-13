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

#ifndef LILY_CORE_LILY_MIR_INSTRUCTION_H
#define LILY_CORE_LILY_MIR_INSTRUCTION_H

#include <base/alloc.h>
#include <base/hash_map.h>
#include <base/ordered_hash_map.h>
#include <base/stack.h>
#include <base/vec.h>

#include <core/lily/mir/block_limit.h>
#include <core/lily/mir/debug_info.h>
#include <core/lily/mir/dt.h>
#include <core/lily/mir/linkage.h>
#include <core/lily/mir/name_manager.h>
#include <core/lily/mir/scope.h>

enum LilyMirInstructionKind : Uint8
{
    LILY_MIR_INSTRUCTION_KIND_ALLOC,
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
    LILY_MIR_INSTRUCTION_KIND_FSUB,
    LILY_MIR_INSTRUCTION_KIND_FUN,
    LILY_MIR_INSTRUCTION_KIND_FUN_PROTOTYPE,
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
    LILY_MIR_INSTRUCTION_KIND_UNREACHABLE,
    LILY_MIR_INSTRUCTION_KIND_VAL,
    LILY_MIR_INSTRUCTION_KIND_VAR,
    LILY_MIR_INSTRUCTION_KIND_XOR
};

typedef struct LilyMirInstruction LilyMirInstruction;

DESTRUCTOR(LilyMirInstruction, LilyMirInstruction *self);

enum LilyMirInstructionValKind
{
    LILY_MIR_INSTRUCTION_VAL_KIND_ARRAY,
    LILY_MIR_INSTRUCTION_VAL_KIND_BYTES,
    LILY_MIR_INSTRUCTION_VAL_KIND_CONST,
    LILY_MIR_INSTRUCTION_VAL_KIND_CSTR,
    LILY_MIR_INSTRUCTION_VAL_KIND_EXCEPTION, // TODO:
                                             // LILY_MIR_INSTRUCTION_VAL_KIND_EXCEPTION
                                             // ->
                                             // LILY_MIR_INSTRUCTION_VAL_KIND_RESULT
    LILY_MIR_INSTRUCTION_VAL_KIND_FLOAT,
    LILY_MIR_INSTRUCTION_VAL_KIND_INT,
    LILY_MIR_INSTRUCTION_VAL_KIND_LIST,
    LILY_MIR_INSTRUCTION_VAL_KIND_NIL,
    LILY_MIR_INSTRUCTION_VAL_KIND_PARAM,
    LILY_MIR_INSTRUCTION_VAL_KIND_REG,
    LILY_MIR_INSTRUCTION_VAL_KIND_SLICE,
    LILY_MIR_INSTRUCTION_VAL_KIND_STR,
    LILY_MIR_INSTRUCTION_VAL_KIND_STRUCT,
    LILY_MIR_INSTRUCTION_VAL_KIND_TRACE,
    LILY_MIR_INSTRUCTION_VAL_KIND_TUPLE,
    LILY_MIR_INSTRUCTION_VAL_KIND_UINT,
    LILY_MIR_INSTRUCTION_VAL_KIND_UNDEF,
    LILY_MIR_INSTRUCTION_VAL_KIND_UNIT,
    LILY_MIR_INSTRUCTION_VAL_KIND_VAR,
};

// val(<dt>) <value>
typedef struct LilyMirInstructionVal
{
    enum LilyMirInstructionValKind kind;
    LilyMirDt *dt;
    Usize ref_count;
    union
    {
        Vec *array;         // Vec<LilyMirInstructionVal*>*
        const Uint8 *bytes; // const Uint8* (&)
        const char *const_; // const char* (&)
        const char *cstr;   // const char* (&)
        struct LilyMirInstructionVal *exception[2]; // [ok?, err?]
        Float64 float_;
        Int64 int_;
        Vec *list;   // Vec<LilyMirInstructionVal*>*
        Usize param; // id of the param
        const char *reg;
        Vec *slice;   // Vec<LilyMirInstructionVal*>*
        String *str;  // String* (&)
        Vec *struct_; // Vec<LilyMirInstructionVal*>*
        struct LilyMirInstructionVal *trace;
        Vec *tuple; // Vec<LilyMirInstructionVal*>*
        Uint64 uint;
        const char *var; // const char* (&)
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
 * (LILY_MIR_INSTRUCTION_VAL_KIND_CONST).
 */
VARIANT_CONSTRUCTOR(LilyMirInstructionVal *,
                    LilyMirInstructionVal,
                    const,
                    LilyMirDt *dt,
                    const char *const_);

/**
 *
 * @brief Construct LilyMirInstructionVal type
 * (LILY_MIR_INSTRUCTION_VAL_KIND_CSTR).
 */
VARIANT_CONSTRUCTOR(LilyMirInstructionVal *,
                    LilyMirInstructionVal,
                    cstr,
                    LilyMirDt *dt,
                    const char *cstr);

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
 * (LILY_MIR_INSTRUCTION_VAL_KIND_PARAM).
 */
VARIANT_CONSTRUCTOR(LilyMirInstructionVal *,
                    LilyMirInstructionVal,
                    param,
                    LilyMirDt *dt,
                    Usize param);

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
                    String *str);

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
 * @brief Construct LilyMirInstructionVal type
 * (LILY_MIR_INSTRUCTION_VAL_KIND_VAR).
 */
VARIANT_CONSTRUCTOR(LilyMirInstructionVal *,
                    LilyMirInstructionVal,
                    var,
                    LilyMirDt *dt,
                    char *var);

/**
 *
 * @brief Pass to ref a pointer of `LilyMirInstructionVal` and increment
 * the `ref_count`.
 * @return LilyMirInstructionVal* (&)
 */
inline LilyMirInstructionVal *
ref__LilyMirInstructionVal(LilyMirInstructionVal *self)
{
    ++self->ref_count;
    return self;
}

/**
 *
 * @brief Return true if the both vals are equal otherwise return false.
 */
bool
eq__LilyMirInstructionVal(const LilyMirInstructionVal *self,
                          const LilyMirInstructionVal *other);

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
typedef struct LilyMirInstructionDestSrc
{
    LilyMirInstructionVal *dest;
    LilyMirInstructionVal *src;
} LilyMirInstructionDestSrc;

/**
 *
 * @brief Construct LilyMirInstructionDestSrc type.
 */
inline CONSTRUCTOR(LilyMirInstructionDestSrc,
                   LilyMirInstructionDestSrc,
                   LilyMirInstructionVal *dest,
                   LilyMirInstructionVal *src)
{
    return (LilyMirInstructionDestSrc){ .dest = dest, .src = src };
}

/**
 *
 * @brief Convert LilyMirInstructionDestSrc in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyMirInstructionDestSrc,
               const LilyMirInstructionDestSrc *self);
#endif

/**
 *
 * @brief Free LilyMirInstructionDestSrc type.
 */
inline DESTRUCTOR(LilyMirInstructionDestSrc,
                  const LilyMirInstructionDestSrc *self)
{
    FREE(LilyMirInstructionVal, self->dest);
    FREE(LilyMirInstructionVal, self->src);
}

// drop <val>
// fneg <val>
// getarg <val>
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
    Usize id;
} LilyMirInstructionArg;

/**
 *
 * @brief Construct LilyMirInstructionArg type.
 */
inline CONSTRUCTOR(LilyMirInstructionArg,
                   LilyMirInstructionArg,
                   LilyMirDt *dt,
                   Usize id)
{
    return (LilyMirInstructionArg){ .dt = dt, .id = id };
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
    const char *name;
    LilyMirBlockLimit *limit;
    Vec *insts; // Vec<LilyMirInstruction*>*
    Usize id;
} LilyMirInstructionBlock;

/**
 *
 * @brief Construct LilyMirInstructionBlock type.
 */
inline CONSTRUCTOR(LilyMirInstructionBlock,
                   LilyMirInstructionBlock,
                   const char *name,
                   LilyMirBlockLimit *limit,
                   Usize id)
{
    return (LilyMirInstructionBlock){
        .name = name, .limit = limit, .insts = NEW(Vec), .id = id
    };
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
    LilyMirDt *return_dt;
    const char *name; // const char* (&)
    Vec *params;      // Vec<LilyMirInstructionVal*>*
} LilyMirInstructionCall;

/**
 *
 * @brief Construct LilyMirInstructionCall type.
 */
inline CONSTRUCTOR(LilyMirInstructionCall,
                   LilyMirInstructionCall,
                   LilyMirDt *return_dt,
                   const char *name,
                   Vec *params)
{
    return (LilyMirInstructionCall){ .return_dt = return_dt,
                                     .name = name,
                                     .params = params };
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

// typedef struct LilyMirInstructionFunLoadNameField
// {
//     const char *instance_name;
//     Vec *indexes; // Vec<LilyMirInstructionVal*>* (&)
// } LilyMirInstructionFunLoadNameField;
//
// /**
//  *
//  * @brief Construct LilyMirInstructionFunLoadNameField type.
//  */
// inline CONSTRUCTOR(LilyMirInstructionFunLoadNameField,
//                    LilyMirInstructionFunLoadNameField,
//                    const char *instance_name,
//                    Vec *indexes)
// {
//     return (LilyMirInstructionFunLoadNameField){ .instance_name =
//     instance_name,
//                                                  .indexes = indexes };
// }

enum LilyMirInstructionFunLoadNameKind
{
    LILY_MIR_INSTRUCTION_FUN_LOAD_NAME_KIND_CONST,
    LILY_MIR_INSTRUCTION_FUN_LOAD_NAME_KIND_PARAM,
    LILY_MIR_INSTRUCTION_FUN_LOAD_NAME_KIND_REG,
    LILY_MIR_INSTRUCTION_FUN_LOAD_NAME_KIND_VAR,
};

typedef struct LilyMirInstructionFunLoadName
{
    enum LilyMirInstructionFunLoadNameKind kind;
    union
    {
        const char *const_;
        Usize param;
        const char *reg;
        const char *var;
    };
} LilyMirInstructionFunLoadName;

/**
 *
 * @brief Construct LilyMirInstructionFunLoadName type
 * (LILY_MIR_INSTRUCTION_FUN_LOAD_NAME_KIND_CONST).
 */
inline VARIANT_CONSTRUCTOR(LilyMirInstructionFunLoadName,
                           LilyMirInstructionFunLoadName,
                           const,
                           const char *const_)
{
    return (LilyMirInstructionFunLoadName){
        .kind = LILY_MIR_INSTRUCTION_FUN_LOAD_NAME_KIND_CONST, .const_ = const_
    };
}

/**
 *
 * @brief Construct LilyMirInstructionFunLoadName type
 * (LILY_MIR_INSTRUCTION_FUN_LOAD_NAME_KIND_PARAM).
 */
inline VARIANT_CONSTRUCTOR(LilyMirInstructionFunLoadName,
                           LilyMirInstructionFunLoadName,
                           param,
                           Usize param)
{
    return (LilyMirInstructionFunLoadName){
        .kind = LILY_MIR_INSTRUCTION_FUN_LOAD_NAME_KIND_PARAM, .param = param
    };
}

/**
 *
 * @brief Construct LilyMirInstructionFunLoadName type
 * (LILY_MIR_INSTRUCTION_FUN_LOAD_NAME_KIND_REG).
 */
inline VARIANT_CONSTRUCTOR(LilyMirInstructionFunLoadName,
                           LilyMirInstructionFunLoadName,
                           reg,
                           const char *reg)
{
    return (LilyMirInstructionFunLoadName){
        .kind = LILY_MIR_INSTRUCTION_FUN_LOAD_NAME_KIND_REG, .reg = reg
    };
}

/**
 *
 * @brief Construct LilyMirInstructionFunLoadName type
 * (LILY_MIR_INSTRUCTION_FUN_LOAD_NAME_KIND_VAR).
 */
inline VARIANT_CONSTRUCTOR(LilyMirInstructionFunLoadName,
                           LilyMirInstructionFunLoadName,
                           var,
                           const char *var)
{
    return (LilyMirInstructionFunLoadName){
        .kind = LILY_MIR_INSTRUCTION_FUN_LOAD_NAME_KIND_VAR, .var = var
    };
}

/**
 *
 * @brief Check if both names are equal.
 */
bool
eq__LilyMirInstructionFunLoadName(const LilyMirInstructionFunLoadName *self,
                                  const LilyMirInstructionFunLoadName *other);

typedef struct LilyMirInstructionFunLoad
{
    // %<reg> = load ... <value_name> ...
    LilyMirInstructionFunLoadName value_name;
    LilyMirInstruction *inst; // LilyMirInstruction* (&)
    Usize block_id;
} LilyMirInstructionFunLoad;

/**
 *
 * @brief Construct LilyMirInstructionFunLoad type.
 */
CONSTRUCTOR(LilyMirInstructionFunLoad *,
            LilyMirInstructionFunLoad,
            LilyMirInstructionFunLoadName value_name,
            LilyMirInstruction *inst,
            Usize block_id);

/**
 *
 * @brief Free LilyMirInstructionFunLoad type.
 */
inline DESTRUCTOR(LilyMirInstructionFunLoad, LilyMirInstructionFunLoad *self)
{
    lily_free(self);
}

typedef struct LilyMirInstructionFun
{
    enum LilyMirLinkage linkage;
    const char *name; // const char* (&)
    // without serialization
    const char *base_name;   // const char* (&)
    Vec *args;               // Vec<LilyMirInstruction*>*
    OrderedHashMap *insts;   // OrderedHashMap<LilyMirInstruction*>*
    Stack *block_stack;      // Stack<LilyMirInstructionBlock*>*
    HashMap *generic_params; // HashMap<LilyCheckedDataType*>* (&)
    LilyMirDt *return_data_type;
    LilyMirNameManager reg_manager;
    LilyMirNameManager block_manager;
    LilyMirNameManager virtual_variable_manager;
    LilyMirScope *root_scope; // LilyMirScope* (&)
    LilyMirScope *scope;
    Usize block_count;
} LilyMirInstructionFun;

/**
 *
 * @brief Construct LilyMirInstructionFun type.
 */
CONSTRUCTOR(LilyMirInstructionFun,
            LilyMirInstructionFun,
            enum LilyMirLinkage linkage,
            const char *name,
            const char *base_name,
            Vec *args,
            HashMap *generic_params,
            LilyMirDt *return_data_type,
            LilyMirBlockLimit *limit);

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

typedef struct LilyMirInstructionFunPrototype
{
    const char *name; // const char* (&)
    Vec *params;      // Vec<LilyMirDt*>*
    LilyMirDt *return_data_type;
    enum LilyMirLinkage linkage;
} LilyMirInstructionFunPrototype;

/**
 *
 * @brief Construct LilyMirInstructionFunPrototype type.
 */
inline CONSTRUCTOR(LilyMirInstructionFunPrototype,
                   LilyMirInstructionFunPrototype,
                   const char *name,
                   Vec *params,
                   LilyMirDt *return_data_type,
                   enum LilyMirLinkage linkage)
{
    return (LilyMirInstructionFunPrototype){ .name = name,
                                             .params = params,
                                             .return_data_type =
                                               return_data_type,
                                             .linkage = linkage };
}

/**
 *
 * @brief Convert LilyMirInstructionFunPrototype in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyMirInstructionFunPrototype,
               const LilyMirInstructionFunPrototype *self);
#endif

/**
 *
 * @brief Free LilyMirInstructionFunPrototype type.
 */
DESTRUCTOR(LilyMirInstructionFunPrototype,
           const LilyMirInstructionFunPrototype *self);

typedef struct LilyMirInstructionGetArray
{
    LilyMirDt *dt;              // data type of the final item
    LilyMirInstructionVal *val; // variable, param, ...
    Vec *indexes;               // Vec<LilyMirInstructionVal*>*
    bool is_const; // true if the array and the indexes are constant.
} LilyMirInstructionGetArray;

/**
 *
 * @brief Construct LilyMirInstructionGetArray type.
 */
inline CONSTRUCTOR(LilyMirInstructionGetArray,
                   LilyMirInstructionGetArray,
                   LilyMirDt *dt,
                   LilyMirInstructionVal *val,
                   Vec *indexes,
                   bool is_const)
{
    return (LilyMirInstructionGetArray){
        .dt = dt, .val = val, .indexes = indexes, .is_const = is_const
    };
}

/**
 *
 * @brief Convert LilyMirInstructionGetArray in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyMirInstructionGetArray,
               const LilyMirInstructionGetArray *self);
#endif

/**
 *
 * @brief Free LilyMirInstructionGetArray type.
 */
DESTRUCTOR(LilyMirInstructionGetArray, const LilyMirInstructionGetArray *self);

typedef struct LilyMirInstructionGetField
{
    LilyMirDt *dt;              // data type of the final field
    LilyMirInstructionVal *val; // variable, param, ...
    Vec *indexes;               // Vec<LilyMirInstructionVal*>*
} LilyMirInstructionGetField;

/**
 *
 * @brief Construct LilyMirInstructionGetField type.
 */
inline CONSTRUCTOR(LilyMirInstructionGetField,
                   LilyMirInstructionGetField,
                   LilyMirDt *dt,
                   LilyMirInstructionVal *val,
                   Vec *indexes)
{
    return (
      LilyMirInstructionGetField){ .dt = dt, .val = val, .indexes = indexes };
}

/**
 *
 * @brief Convert LilyMirInstructionGetField in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyMirInstructionGetField,
               const LilyMirInstructionGetField *self);
#endif

/**
 *
 * @brief Free LilyMirInstructionGetField type.
 */
DESTRUCTOR(LilyMirInstructionGetField, const LilyMirInstructionGetField *self);

typedef struct LilyMirInstructionLoad
{
    LilyMirInstructionSrc src;
    LilyMirDt *dt;
} LilyMirInstructionLoad;

/**
 *
 * @brief Construct LilyMirInstructionLoad type.
 */
inline CONSTRUCTOR(LilyMirInstructionLoad,
                   LilyMirInstructionLoad,
                   LilyMirInstructionSrc src,
                   LilyMirDt *dt)
{
    return (LilyMirInstructionLoad){ .src = src, .dt = dt };
}

/**
 *
 * @brief Convert LilyMirInstructionLoad in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyMirInstructionLoad,
               const LilyMirInstructionLoad *self);
#endif

/**
 *
 * @brief Free LilyMirInstructionLoad type.
 */
DESTRUCTOR(LilyMirInstructionLoad, const LilyMirInstructionLoad *self);

typedef struct LilyMirInstructionJmpCond
{
    LilyMirInstructionVal *cond;
    LilyMirInstructionBlock *then_block; // LilyMirInstructionBlock* (&)
    LilyMirInstructionBlock *else_block; // LilyMirInstructionBlock* (&)
} LilyMirInstructionJmpCond;

/**
 *
 * @brief Construct LilyMirInstructionJmpCond type.
 */
inline CONSTRUCTOR(LilyMirInstructionJmpCond,
                   LilyMirInstructionJmpCond,
                   LilyMirInstructionVal *cond,
                   LilyMirInstructionBlock *then_block,
                   LilyMirInstructionBlock *else_block)
{
    return (LilyMirInstructionJmpCond){ .cond = cond,
                                        .then_block = then_block,
                                        .else_block = else_block };
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
    const char *name;
    LilyMirInstruction *inst;
} LilyMirInstructionReg;

/**
 *
 * @brief Construct LilyMirInstructionReg type.
 */
inline CONSTRUCTOR(LilyMirInstructionReg,
                   LilyMirInstructionReg,
                   const char *name,
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
DESTRUCTOR(LilyMirInstructionReg, const LilyMirInstructionReg *self);

typedef struct LilyMirInstructionVar
{
    char *name;
    LilyMirInstruction *inst;
} LilyMirInstructionVar;

/**
 *
 * @brief Construct LilyMirInstructionVar type.
 */
inline CONSTRUCTOR(LilyMirInstructionVar,
                   LilyMirInstructionVar,
                   char *name,
                   LilyMirInstruction *inst)
{
    return (LilyMirInstructionVar){ .name = name, .inst = inst };
}

/**
 *
 * @brief Convert LilyMirInstructionVar in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyMirInstructionVar,
               const LilyMirInstructionVar *self);
#endif

/**
 *
 * @brief Free LilyMirInstructionVar type.
 */
DESTRUCTOR(LilyMirInstructionVar, const LilyMirInstructionVar *self);

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
inline CONSTRUCTOR(LilyMirInstructionTry,
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
    const char *name;               // const char* (&)
    Vec *fields;                    // Vec<LilyMirDt*>*
    OrderedHashMap *generic_params; // OrderedHashMap<LilyCheckedDataType*>* (&)
} LilyMirInstructionStruct;

/**
 *
 * @brief Construct LilyMirInstructionStruct type.
 */
inline CONSTRUCTOR(LilyMirInstructionStruct,
                   LilyMirInstructionStruct,
                   enum LilyMirLinkage linkage,
                   const char *name,
                   Vec *fields,
                   OrderedHashMap *generic_params)
{
    return (LilyMirInstructionStruct){ .linkage = linkage,
                                       .name = name,
                                       .fields = fields,
                                       .generic_params = generic_params };
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
    LilyMirDebugInfo *debug_info; // LilyMirDebugInfo*?
    union
    {
        LilyMirInstructionAlloc alloc;
        LilyMirInstructionArg arg;
        LilyMirInstructionAsm asm;
        LilyMirInstructionValDt bitcast;
        LilyMirInstructionDestSrc bitand;
        LilyMirInstructionSrc bitnot;
        LilyMirInstructionDestSrc bitor ;
        LilyMirInstructionBlock block;
        LilyMirInstructionCall builtin_call;
        LilyMirInstructionCall call;
        LilyMirInstructionConst const_;
        LilyMirInstructionSrc drop;
        LilyMirInstructionDestSrc exp;
        LilyMirInstructionDestSrc fadd;
        LilyMirInstructionDestSrc fcmp_eq;
        LilyMirInstructionDestSrc fcmp_ne;
        LilyMirInstructionDestSrc fcmp_le;
        LilyMirInstructionDestSrc fcmp_lt;
        LilyMirInstructionDestSrc fcmp_ge;
        LilyMirInstructionDestSrc fcmp_gt;
        LilyMirInstructionDestSrc fdiv;
        LilyMirInstructionDestSrc fmul;
        LilyMirInstructionSrc fneg;
        LilyMirInstructionDestSrc frem;
        LilyMirInstructionDestSrc fsub;
        LilyMirInstructionFun fun;
        LilyMirInstructionFunPrototype fun_prototype;
        LilyMirInstructionGetArray getarray;
        LilyMirInstructionSrc getarg;
        LilyMirInstructionGetField getfield;
        LilyMirInstructionSrc getlist;
        LilyMirInstructionSrc getptr;
        LilyMirInstructionSrc getslice;
        LilyMirInstructionDestSrc iadd;
        LilyMirInstructionDestSrc icmp_eq;
        LilyMirInstructionDestSrc icmp_ne;
        LilyMirInstructionDestSrc icmp_le;
        LilyMirInstructionDestSrc icmp_lt;
        LilyMirInstructionDestSrc icmp_ge;
        LilyMirInstructionDestSrc icmp_gt;
        LilyMirInstructionDestSrc idiv;
        LilyMirInstructionDestSrc imul;
        LilyMirInstructionSrc inctrace;
        LilyMirInstructionSrc ineg;
        LilyMirInstructionDestSrc irem;
        LilyMirInstructionSrc isok;
        LilyMirInstructionSrc iserr;
        LilyMirInstructionDestSrc isub;
        LilyMirInstructionBlock *jmp; // LilyMirInstructionBlock* (&)
        LilyMirInstructionJmpCond jmpcond;
        LilyMirInstructionSrc len;
        LilyMirInstructionLoad load;
        LilyMirInstructionSrc makeref;
        LilyMirInstructionSrc makeopt;
        LilyMirInstruction *non_nil;
        LilyMirInstructionSrc not;
        LilyMirInstructionReg reg;
        LilyMirInstructionSrc ref_ptr;
        LilyMirInstruction *ret;
        LilyMirInstructionDestSrc shl;
        LilyMirInstructionDestSrc shr;
        LilyMirInstructionDestSrc store;
        LilyMirInstructionStruct struct_;
        LilyMirInstructionSwitch switch_;
        LilyMirInstructionCall sys_call;
        LilyMirInstructionValDt trunc;
        LilyMirInstructionTry try;
        LilyMirInstructionTry try_ptr;
        LilyMirInstructionVal *val;
        LilyMirInstructionVar var;
        LilyMirInstructionDestSrc xor ;
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
                    LilyMirInstructionDestSrc bitand);

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
                    LilyMirInstructionDestSrc bitor);

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
                    LilyMirInstructionDestSrc exp);

/**
 *
 * @brief Construct LilyMirInstruction type (LILY_MIR_INSTRUCTION_KIND_FADD).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    fadd,
                    LilyMirInstructionDestSrc fadd);

/**
 *
 * @brief Construct LilyMirInstruction type (LILY_MIR_INSTRUCTION_KIND_FCMP_EQ).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    fcmp_eq,
                    LilyMirInstructionDestSrc fcmp_eq);

/**
 *
 * @brief Construct LilyMirInstruction type (LILY_MIR_INSTRUCTION_KIND_FCMP_NE).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    fcmp_ne,
                    LilyMirInstructionDestSrc fcmp_ne);

/**
 *
 * @brief Construct LilyMirInstruction type (LILY_MIR_INSTRUCTION_KIND_FCMP_LE).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    fcmp_le,
                    LilyMirInstructionDestSrc fcmp_le);

/**
 *
 * @brief Construct LilyMirInstruction type (LILY_MIR_INSTRUCTION_KIND_FCMP_LT).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    fcmp_lt,
                    LilyMirInstructionDestSrc fcmp_lt);

/**
 *
 * @brief Construct LilyMirInstruction type (LILY_MIR_INSTRUCTION_KIND_FCMP_GE).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    fcmp_ge,
                    LilyMirInstructionDestSrc fcmp_ge);

/**
 *
 * @brief Construct LilyMirInstruction type (LILY_MIR_INSTRUCTION_KIND_FCMP_GT).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    fcmp_gt,
                    LilyMirInstructionDestSrc fcmp_gt);

/**
 *
 * @brief Construct LilyMirInstruction type (LILY_MIR_INSTRUCTION_KIND_FDIV).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    fdiv,
                    LilyMirInstructionDestSrc fdiv);

/**
 *
 * @brief Construct LilyMirInstruction type (LILY_MIR_INSTRUCTION_KIND_FMUL).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    fmul,
                    LilyMirInstructionDestSrc fmul);

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
                    LilyMirInstructionDestSrc frem);

/**
 *
 * @brief Construct LilyMirInstruction type (LILY_MIR_INSTRUCTION_KIND_FSUB).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    fsub,
                    LilyMirInstructionDestSrc fsub);

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
 * (LILY_MIR_INSTRUCTION_KIND_FUN_PROTOTYPE).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    fun_prototype,
                    LilyMirInstructionFunPrototype fun_prototype);

/**
 *
 * @brief Construct LilyMirInstruction type
 * (LILY_MIR_INSTRUCTION_KIND_GETARRAY).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    getarray,
                    LilyMirInstructionGetArray getarray);

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
                    LilyMirInstructionGetField getfield);

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
                    LilyMirInstructionDestSrc iadd);

/**
 *
 * @brief Construct LilyMirInstruction type
 * (LILY_MIR_INSTRUCTION_KIND_ICMP_EQ).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    icmp_eq,
                    LilyMirInstructionDestSrc icmp_eq);

/**
 *
 * @brief Construct LilyMirInstruction type
 * (LILY_MIR_INSTRUCTION_KIND_ICMP_NE).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    icmp_ne,
                    LilyMirInstructionDestSrc icmp_ne);

/**
 *
 * @brief Construct LilyMirInstruction type
 * (LILY_MIR_INSTRUCTION_KIND_ICMP_LE).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    icmp_le,
                    LilyMirInstructionDestSrc icmp_le);

/**
 *
 * @brief Construct LilyMirInstruction type
 * (LILY_MIR_INSTRUCTION_KIND_ICMP_LT).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    icmp_lt,
                    LilyMirInstructionDestSrc icmp_lt);

/**
 *
 * @brief Construct LilyMirInstruction type
 * (LILY_MIR_INSTRUCTION_KIND_ICMP_GE).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    icmp_ge,
                    LilyMirInstructionDestSrc icmp_ge);

/**
 *
 * @brief Construct LilyMirInstruction type
 * (LILY_MIR_INSTRUCTION_KIND_ICMP_GT).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    icmp_gt,
                    LilyMirInstructionDestSrc icmp_gt);

/**
 *
 * @brief Construct LilyMirInstruction type
 * (LILY_MIR_INSTRUCTION_KIND_IDIV).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    idiv,
                    LilyMirInstructionDestSrc idiv);

/**
 *
 * @brief Construct LilyMirInstruction type
 * (LILY_MIR_INSTRUCTION_KIND_IMUL).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    imul,
                    LilyMirInstructionDestSrc imul);

/**
 *
 * @brief Construct LilyMirInstruction type
 * (LILY_MIR_INSTRUCTION_KIND_INCTRACE).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    inctrace,
                    LilyMirInstructionDestSrc inctrace);

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
                    LilyMirInstructionDestSrc irem);

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
                    LilyMirInstructionDestSrc isub);

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
                    LilyMirInstructionLoad load);

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
                    LilyMirInstructionSrc not);

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
                    LilyMirInstructionDestSrc shl);

/**
 *
 * @brief Construct LilyMirInstruction type
 * (LILY_MIR_INSTRUCTION_KIND_SHR).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    shr,
                    LilyMirInstructionDestSrc shr);

/**
 *
 * @brief Construct LilyMirInstruction type
 * (LILY_MIR_INSTRUCTION_KIND_STORE).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    store,
                    LilyMirInstructionDestSrc store);

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
 * (LILY_MIR_INSTRUCTION_KIND_UNREACHABLE).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *, LilyMirInstruction, unreachable);

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
 * (LILY_MIR_INSTRUCTION_KIND_VAR).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    var,
                    LilyMirInstructionVar var);

/**
 *
 * @brief Construct LilyMirInstruction type
 * (LILY_MIR_INSTRUCTION_KIND_XOR).
 */
VARIANT_CONSTRUCTOR(LilyMirInstruction *,
                    LilyMirInstruction,
                    xor,
                    LilyMirInstructionDestSrc xor);

/**
 *
 * @brief Get arg from the given instruction.
 */
const LilyMirInstruction *
get_arg__LilyMirInstruction(const LilyMirInstruction *self);

/**
 *
 * @brief Free `self` ptr and `self->debug_info` ptr.
 */
void
partial_free__LilyMirInstruction(LilyMirInstruction *self);

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

#endif // LILY_CORE_LILY_MIR_INSTRUCTION_H
