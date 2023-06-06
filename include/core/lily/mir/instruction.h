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

#include <base/vec.h>

#include <core/lily/mir/dt.h>

enum LilyMirInstructionKind
{
    LILY_MIR_INSTRUCTION_KIND_ADD,
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
    LILY_MIR_INSTRUCTION_KIND_DIV,
    LILY_MIR_INSTRUCTION_KIND_DROP,
    LILY_MIR_INSTRUCTION_KIND_ICMP_EQ,
    LILY_MIR_INSTRUCTION_KIND_ICMP_NE,
    LILY_MIR_INSTRUCTION_KIND_ICMP_LE,
    LILY_MIR_INSTRUCTION_KIND_ICMP_LT,
    LILY_MIR_INSTRUCTION_KIND_ICMP_GE,
    LILY_MIR_INSTRUCTION_KIND_ICMP_GT,
    LILY_MIR_INSTRUCTION_KIND_EXP,
    LILY_MIR_INSTRUCTION_KIND_FASTLOAD,
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
    LILY_MIR_INSTRUCTION_KIND_INCTRACE,
    LILY_MIR_INSTRUCTION_KIND_JMP,
    LILY_MIR_INSTRUCTION_KIND_JMPMATCH,
    LILY_MIR_INSTRUCTION_KIND_JMPCOND,
    LILY_MIR_INSTRUCTION_KIND_LEN,
    LILY_MIR_INSTRUCTION_KIND_LOAD,
    LILY_MIR_INSTRUCTION_KIND_LOOP,
    LILY_MIR_INSTRUCTION_KIND_MAKEREF,
    LILY_MIR_INSTRUCTION_KIND_MAKEOPT,
    LILY_MIR_INSTRUCTION_KIND_MUL,
    LILY_MIR_INSTRUCTION_KIND_NEG,
    LILY_MIR_INSTRUCTION_KIND_NEW,
    LILY_MIR_INSTRUCTION_KIND_NEWARRAY,
    LILY_MIR_INSTRUCTION_KIND_NEWLIST,
    LILY_MIR_INSTRUCTION_KIND_NEWSLICE,
    LILY_MIR_INSTRUCTION_KIND_NEWTRACE,
    LILY_MIR_INSTRUCTION_KIND_NIL,
    LILY_MIR_INSTRUCTION_KIND_NON_NIL,
    LILY_MIR_INSTRUCTION_KIND_NOT,
    LILY_MIR_INSTRUCTION_KIND_OR,
    LILY_MIR_INSTRUCTION_KIND_REG,
    LILY_MIR_INSTRUCTION_KIND_REM,
    LILY_MIR_INSTRUCTION_KIND_REF_PTR,
    LILY_MIR_INSTRUCTION_KIND_RET,
    LILY_MIR_INSTRUCTION_KIND_SHL,
    LILY_MIR_INSTRUCTION_KIND_SHR,
    LILY_MIR_INSTRUCTION_KIND_STORE,
    LILY_MIR_INSTRUCTION_KIND_STRUCT,
    LILY_MIR_INSTRUCTION_KIND_SUB,
    LILY_MIR_INSTRUCTION_KIND_SYS_CALL,
    LILY_MIR_INSTRUCTION_KIND_TRUNC,
    LILY_MIR_INSTRUCTION_KIND_TRY,
    LILY_MIR_INSTRUCTION_KIND_TRY_PTR,
    LILY_MIR_INSTRUCTION_KIND_VAL,
    LILY_MIR_INSTRUCTION_KIND_XOR
};

typedef struct LilyMirInstruction LilyMirInstruction;

// val(<dt>) <inst>
typedef struct LilyMirInstructionVal
{
    LilyMirDt dt;
    LilyMirInstruction *inst;
} LilyMirInstructionVal;

// add <val>, <val>
// and <val>, <val>
// bitand <val>, <val>
// bitnot <val>, <val>
// bitor <val>, <val>
// <inst> <dest>, <src>
typedef struct LilyMirInstructionSrcDest
{
    LilyMirInstructionVal dest;
    LilyMirInstructionVal src;
} LilyMirInstructionSrcDest;

// alloc <dt>
typedef struct LilyMirInstructionAlloc
{
    LilyMirDt dt;
} LilyMirInstructionAlloc;

// arg(<dt>) <name>
typedef struct LilyMirInstructionArg
{
    LilyMirDt dt;
    const char *name; // const char* (&)
} LilyMirInstructionArg;

// asm("xor eax, eax")
typedef struct LilyMirInstructionAsm
{
    const char *content; // const char* (&)
} LilyMirInstructionAsm;

// bitcast <val> -> <dt>
// <inst> <val> -> <dt>
typedef struct LilyMirInstructionValDt
{
    LilyMirInstructionVal val;
    LilyMirDt dt;
} LilyMirInstructionValDt;

typedef struct LilyMirInstructionBlock
{
    Vec *insts; // Vec<LilyMirInstruction*>*
} LilyMirInstructionBlock;

typedef struct LilyMirInstructionCall
{
    const char *name; // const char* (&)
    Vec *params;      // Vec<LilyMirInstruction*>*
} LilyMirInstructionCall;

typedef struct LilyMirInstructionConst
{
    const char *name; // const char* (&)
} LilyMirInstructionConst;

typedef struct LilyMirInstruction
{
    enum LilyMirInstructionKind kind;
    union
    {
        LilyMirInstructionSrcDest add;
        LilyMirInstructionAlloc alloc;
        LilyMirInstructionSrcDest and;
        LilyMirInstructionArg arg;
        LilyMirInstructionAsm asm;
        LilyMirInstructionValDt bitcast;
        LilyMirInstructionSrcDest bitand;
        LilyMirInstructionSrcDest bitnot;
        LilyMirInstructionSrcDest bitor ;
        LilyMirInstructionBlock block;
        LilyMirInstructionVal val;
        LilyMirInstructionCall builtin_call;
        LilyMirInstructionCall call;
    };
} LilyMirInstruction;

#endif // LILY_CORE_LILY_IR_INSTRUCTION_H
