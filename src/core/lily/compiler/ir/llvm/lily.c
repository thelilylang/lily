/*
 * MIT License
 *
 * Copyright (c) 2022-2026 ArthurPV
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

#include <base/assert.h>

#include <core/lily/analysis/checked/limits.h>
#include <core/lily/compiler/ir/llvm/lily.h>
#include <core/lily/compiler/ir/llvm/primary.h>

#include <stdio.h>
#include <stdlib.h>

#define LILY_MAIN "lily.main"

static threadlocal LLVMTypeRef LilyMainFunType = NULL;
static threadlocal LLVMValueRef CurrentPtr = NULL;

LLVMBasicBlockRef
LilyLLVMCreateBasicBlock(const LilyIrLlvm *Self,
                         const LilyIrLlvmPending *Pending,
                         const char *Name)
{
    ASSERT(Pending->current_fun);

    LLVMBasicBlockRef match = get_block__LilyIrLlvmPending(Pending, Name);

    if (match) {
        return match;
    }

    return LLVMAppendBasicBlockInContext(
      Self->context, Pending->current_fun, Name);
}

LLVMValueRef
LilyLLVMGetNamedFunction(const LilyIrLlvm *Self, const char *Name)
{
    LLVMValueRef Fn = LLVMGetNamedFunction(Self->module, Name);

    ASSERT(Fn);

    return Fn;
}

LLVMTypeRef
LilyLLVMGetTypeByName(const LilyIrLlvm *Self, const char *Name)
{
    LLVMTypeRef Ty = LLVMGetTypeByName2(Self->context, Name);

    ASSERT(Ty);

    return Ty;
}

void
LilyLLVMSetLinkage(LLVMValueRef Global, const enum LilyMirLinkage Linkage)
{
    switch (Linkage) {
        case LILY_MIR_LINKAGE_PRIVATE:
            LLVMSetLinkage(Global, LLVMLinkerPrivateLinkage);
            break;
        case LILY_MIR_LINKAGE_EXTERNAL:
            LLVMSetLinkage(Global, LLVMExternalLinkage);
            break;
        case LILY_MIR_LINKAGE_PUBLIC:
            break;
        default:
            UNREACHABLE("unknown variant");
    }
}

LLVMValueRef
LilyLLVMBuildAlloc(const LilyIrLlvm *Self,
                   const LilyMirDt *DT,
                   const char *Name)
{
    LLVMTypeRef alloc_type = LilyLLVMGetType(Self, DT);

    ASSERT(alloc_type);

    return LLVMBuildAlloca(Self->builder, alloc_type, Name);
}

LLVMValueRef
LilyLLVMBuildAnd(const LilyIrLlvm *Self,
                 LilyIrLlvmScope *Scope,
                 const LilyIrLlvmPending *Pending,
                 const LilyMirInstructionVal *LHS,
                 const LilyMirInstructionVal *RHS,
                 const char *Name)
{
    LLVMValueRef left = LilyLLVMBuildVal(Self, Scope, Pending, LHS);
    LLVMValueRef right = LilyLLVMBuildVal(Self, Scope, Pending, RHS);

    ASSERT(left && right);

    return LLVMBuildAnd(Self->builder, left, right, Name);
}

LLVMValueRef
LilyLLVMBuildBitCast(const LilyIrLlvm *Self,
                     LilyIrLlvmScope *Scope,
                     const LilyIrLlvmPending *Pending,
                     const LilyMirInstructionVal *Val,
                     const LilyMirDt *DestDt,
                     const char *Name)
{
    LLVMValueRef val = LilyLLVMBuildVal(Self, Scope, Pending, Val);
    LLVMTypeRef dest_dt = LilyLLVMGetType(Self, DestDt);

    ASSERT(val && dest_dt);

    return LLVMBuildBitCast(Self->builder, val, dest_dt, Name);
}

LLVMValueRef
LilyLLVMBuildNot(const LilyIrLlvm *Self,
                 LilyIrLlvmScope *Scope,
                 const LilyIrLlvmPending *Pending,
                 const LilyMirInstructionVal *RHS,
                 const char *Name)
{
    LLVMValueRef right = LilyLLVMBuildVal(Self, Scope, Pending, RHS);

    ASSERT(right);

    return LLVMBuildNot(Self->builder, right, Name);
}

LLVMValueRef
LilyLLVMBuildOr(const LilyIrLlvm *Self,
                LilyIrLlvmScope *Scope,
                const LilyIrLlvmPending *Pending,
                const LilyMirInstructionVal *LHS,
                const LilyMirInstructionVal *RHS,
                const char *Name)
{
    LLVMValueRef left = LilyLLVMBuildVal(Self, Scope, Pending, LHS);
    LLVMValueRef right = LilyLLVMBuildVal(Self, Scope, Pending, RHS);

    ASSERT(left && right);

    return LLVMBuildOr(Self->builder, left, right, Name);
}

LLVMBasicBlockRef
LilyLLVMBuildBlock(const LilyIrLlvm *Self,
                   LilyIrLlvmScope *Scope,
                   const LilyIrLlvmPending *Pending,
                   const Vec *Insts,
                   const Usize limit_block_id,
                   const Usize id,
                   const char *Name)
{
    LLVMBasicBlockRef block = LilyLLVMCreateBasicBlock(Self, Pending, Name);

    LLVMPositionBuilderAtEnd(Self->builder, block);
    add_block__LilyIrLlvmPending(Pending, Name, block);
    add_scope_item__LilyIrLlvmScope(Scope, limit_block_id);

    for (Usize i = 0; i < Insts->len; ++i) {
        LilyLLVMBuildInst(Self, Scope, Pending, get__Vec(Insts, i), NULL);
    }

    if (id == limit_block_id) {
        destroy_scope_items__LilyIrLlvmScope(Scope, limit_block_id);
    }

    return block;
}

LLVMValueRef
LilyLLVMBuildBuiltinCall(const LilyIrLlvm *Self,
                         LilyIrLlvmScope *Scope,
                         const LilyIrLlvmPending *Pending,
                         const Vec *Params,
                         const LilyMirDt *ReturnDT,
                         const char *BuiltinName,
                         const char *Name)
{
    LLVMValueRef BuiltinFn = LLVMGetNamedFunction(Self->module, BuiltinName);

    ASSERT(BuiltinFn);

    LLVMValueRef Args[MAX_FUN_PARAMS] = { 0 };
    LLVMTypeRef ParamTypes[MAX_FUN_PARAMS] = { 0 };
    LLVMTypeRef ReturnType = LilyLLVMGetType(Self, ReturnDT);

    ASSERT(ReturnType);

    for (Usize i = 0; i < Params->len; ++i) {
        LLVMValueRef Arg =
          LilyLLVMBuildVal(Self, Scope, Pending, get__Vec(Params, i));

        ASSERT(Arg);

        Args[i] = Arg;
        ParamTypes[i] = LLVMTypeOf(Arg);
    }

    return LLVMBuildCall2(
      Self->builder,
      LLVMFunctionType(ReturnType, ParamTypes, Params->len, false),
      BuiltinFn,
      Args,
      Params->len,
      Name);
}

LLVMValueRef
LilyLLVMBuildCall(const LilyIrLlvm *Self,
                  LilyIrLlvmScope *Scope,
                  const LilyIrLlvmPending *Pending,
                  const Vec *Params,
                  const LilyMirDt *ReturnDT,
                  const char *FnName,
                  const char *Name)
{
    LLVMValueRef Fn = LilyLLVMGetNamedFunction(Self, FnName);
    LLVMValueRef Args[MAX_FUN_PARAMS] = { 0 };
    LLVMTypeRef ParamTypes[MAX_FUN_PARAMS] = { 0 };
    LLVMTypeRef ReturnType = LilyLLVMGetType(Self, ReturnDT);

    ASSERT(ReturnType);

    for (Usize i = 0; i < Params->len; ++i) {
        LLVMValueRef Arg =
          LilyLLVMBuildVal(Self, Scope, Pending, get__Vec(Params, i));

        ASSERT(Arg);

        Args[i] = Arg;
        ParamTypes[i] = LLVMTypeOf(Arg);
    }

    return LLVMBuildCall2(
      Self->builder,
      LLVMFunctionType(ReturnType, ParamTypes, Params->len, false),
      Fn,
      Args,
      Params->len,
      Name ? Name : "");
}

LLVMValueRef
LilyLLVMBuildAdd(const LilyIrLlvm *Self,
                 LilyIrLlvmScope *Scope,
                 const LilyIrLlvmPending *Pending,
                 const LilyMirInstructionVal *LHS,
                 const LilyMirInstructionVal *RHS,
                 bool IsFloat,
                 const char *Name)
{
    LLVMValueRef left = LilyLLVMBuildVal(Self, Scope, Pending, LHS);
    LLVMValueRef right = LilyLLVMBuildVal(Self, Scope, Pending, RHS);

    ASSERT(left && right);

    return IsFloat ? LLVMBuildFAdd(Self->builder, left, right, Name)
                   : LLVMBuildAdd(Self->builder, left, right, Name);
}

LLVMValueRef
LilyLLVMBuildCmpEq(const LilyIrLlvm *Self,
                   LilyIrLlvmScope *Scope,
                   const LilyIrLlvmPending *Pending,
                   const LilyMirInstructionVal *LHS,
                   const LilyMirInstructionVal *RHS,
                   bool IsFloat,
                   const char *Name)
{
    LLVMValueRef left = LilyLLVMBuildVal(Self, Scope, Pending, LHS);
    LLVMValueRef right = LilyLLVMBuildVal(Self, Scope, Pending, RHS);

    ASSERT(left && right);

    if (IsFloat) {
    }

    return IsFloat
             ? LLVMBuildFCmp(Self->builder, LLVMRealOEQ, left, right, Name)
             : LLVMBuildICmp(Self->builder, LLVMIntEQ, left, right, Name);
}

LLVMValueRef
LilyLLVMBuildCmpNe(const LilyIrLlvm *Self,
                   LilyIrLlvmScope *Scope,
                   const LilyIrLlvmPending *Pending,
                   const LilyMirInstructionVal *LHS,
                   const LilyMirInstructionVal *RHS,
                   bool IsFloat,
                   const char *Name)
{
    LLVMValueRef left = LilyLLVMBuildVal(Self, Scope, Pending, LHS);
    LLVMValueRef right = LilyLLVMBuildVal(Self, Scope, Pending, RHS);

    ASSERT(left && right);

    return IsFloat
             ? LLVMBuildFCmp(Self->builder, LLVMRealONE, left, right, Name)
             : LLVMBuildICmp(Self->builder, LLVMIntNE, left, right, Name);
}

LLVMValueRef
LilyLLVMBuildCmpLe(const LilyIrLlvm *Self,
                   LilyIrLlvmScope *Scope,
                   const LilyIrLlvmPending *Pending,
                   const LilyMirInstructionVal *LHS,
                   const LilyMirInstructionVal *RHS,
                   bool IsFloat,
                   bool IsSigned,
                   const char *Name)
{
    LLVMValueRef left = LilyLLVMBuildVal(Self, Scope, Pending, LHS);
    LLVMValueRef right = LilyLLVMBuildVal(Self, Scope, Pending, RHS);

    ASSERT(left && right);

    return IsFloat
             ? LLVMBuildFCmp(Self->builder, LLVMRealOLE, left, right, Name)
           : IsSigned
             ? LLVMBuildICmp(Self->builder, LLVMIntSLE, left, right, Name)
             : LLVMBuildICmp(Self->builder, LLVMIntULE, left, right, Name);
}

LLVMValueRef
LilyLLVMBuildCmpLt(const LilyIrLlvm *Self,
                   LilyIrLlvmScope *Scope,
                   const LilyIrLlvmPending *Pending,
                   const LilyMirInstructionVal *LHS,
                   const LilyMirInstructionVal *RHS,
                   bool IsFloat,
                   bool IsSigned,
                   const char *Name)
{
    LLVMValueRef left = LilyLLVMBuildVal(Self, Scope, Pending, LHS);
    LLVMValueRef right = LilyLLVMBuildVal(Self, Scope, Pending, RHS);

    ASSERT(left && right);

    return IsFloat
             ? LLVMBuildFCmp(Self->builder, LLVMRealOLT, left, right, Name)
           : IsSigned
             ? LLVMBuildICmp(Self->builder, LLVMIntSLT, left, right, Name)
             : LLVMBuildICmp(Self->builder, LLVMIntULT, left, right, Name);
}

LLVMValueRef
LilyLLVMBuildCmpGe(const LilyIrLlvm *Self,
                   LilyIrLlvmScope *Scope,
                   const LilyIrLlvmPending *Pending,
                   const LilyMirInstructionVal *LHS,
                   const LilyMirInstructionVal *RHS,
                   bool IsFloat,
                   bool IsSigned,
                   const char *Name)
{
    LLVMValueRef left = LilyLLVMBuildVal(Self, Scope, Pending, LHS);
    LLVMValueRef right = LilyLLVMBuildVal(Self, Scope, Pending, RHS);

    ASSERT(left && right);

    return IsFloat
             ? LLVMBuildFCmp(Self->builder, LLVMRealOGE, left, right, Name)
           : IsSigned
             ? LLVMBuildICmp(Self->builder, LLVMIntSGE, left, right, Name)
             : LLVMBuildICmp(Self->builder, LLVMIntUGE, left, right, Name);
}

LLVMValueRef
LilyLLVMBuildCmpGt(const LilyIrLlvm *Self,
                   LilyIrLlvmScope *Scope,
                   const LilyIrLlvmPending *Pending,
                   const LilyMirInstructionVal *LHS,
                   const LilyMirInstructionVal *RHS,
                   bool IsFloat,
                   bool IsSigned,
                   const char *Name)
{
    LLVMValueRef left = LilyLLVMBuildVal(Self, Scope, Pending, LHS);
    LLVMValueRef right = LilyLLVMBuildVal(Self, Scope, Pending, RHS);

    ASSERT(left && right);

    return IsFloat
             ? LLVMBuildFCmp(Self->builder, LLVMRealOGT, left, right, Name)
           : IsSigned
             ? LLVMBuildICmp(Self->builder, LLVMIntSGT, left, right, Name)
             : LLVMBuildICmp(Self->builder, LLVMIntUGT, left, right, Name);
}

LLVMValueRef
LilyLLVMBuildDiv(const LilyIrLlvm *Self,
                 LilyIrLlvmScope *Scope,
                 const LilyIrLlvmPending *Pending,
                 const LilyMirInstructionVal *LHS,
                 const LilyMirInstructionVal *RHS,
                 bool IsFloat,
                 bool IsSigned,
                 const char *Name)
{
    LLVMValueRef left = LilyLLVMBuildVal(Self, Scope, Pending, LHS);
    LLVMValueRef right = LilyLLVMBuildVal(Self, Scope, Pending, RHS);

    ASSERT(left && right);

    return IsFloat    ? LLVMBuildFDiv(Self->builder, left, right, Name)
           : IsSigned ? LLVMBuildSDiv(Self->builder, left, right, Name)
                      : LLVMBuildUDiv(Self->builder, left, right, Name);
}

LLVMValueRef
LilyLLVMBuildLen(const LilyIrLlvm *Self,
                 LilyIrLlvmScope *Scope,
                 const LilyIrLlvmPending *Pending,
                 const LilyMirInstructionVal *Val,
                 const char *Name)
{
    return LLVMBuildExtractValue(
      Self->builder, LilyLLVMBuildVal(Self, Scope, Pending, Val), 1, Name);
}

LLVMValueRef
LilyLLVMBuildMul(const LilyIrLlvm *Self,
                 LilyIrLlvmScope *Scope,
                 const LilyIrLlvmPending *Pending,
                 const LilyMirInstructionVal *LHS,
                 const LilyMirInstructionVal *RHS,
                 bool IsFloat,
                 const char *Name)
{
    LLVMValueRef left = LilyLLVMBuildVal(Self, Scope, Pending, LHS);
    LLVMValueRef right = LilyLLVMBuildVal(Self, Scope, Pending, RHS);

    ASSERT(left && right);

    return IsFloat ? LLVMBuildFMul(Self->builder, left, right, Name)
                   : LLVMBuildMul(Self->builder, left, right, Name);
}

LLVMValueRef
LilyLLVMBuildNeg(const LilyIrLlvm *Self,
                 LilyIrLlvmScope *Scope,
                 const LilyIrLlvmPending *Pending,
                 const LilyMirInstructionVal *RHS,
                 bool IsFloat,
                 const char *Name)
{
    LLVMValueRef V = LilyLLVMBuildVal(Self, Scope, Pending, RHS);

    ASSERT(V);

    return IsFloat ? LLVMBuildFNeg(Self->builder, V, Name)
                   : LLVMBuildNeg(Self->builder, V, Name);
}

LLVMValueRef
LilyLLVMBuildRem(const LilyIrLlvm *Self,
                 LilyIrLlvmScope *Scope,
                 const LilyIrLlvmPending *Pending,
                 const LilyMirInstructionVal *LHS,
                 const LilyMirInstructionVal *RHS,
                 bool IsFloat,
                 bool IsSigned,
                 const char *Name)
{
    LLVMValueRef left = LilyLLVMBuildVal(Self, Scope, Pending, LHS);
    LLVMValueRef right = LilyLLVMBuildVal(Self, Scope, Pending, RHS);

    ASSERT(left && right);

    return IsFloat    ? LLVMBuildFRem(Self->builder, left, right, Name)
           : IsSigned ? LLVMBuildSRem(Self->builder, left, right, Name)
                      : LLVMBuildURem(Self->builder, left, right, Name);
}

LLVMValueRef
LilyLLVMBuildSub(const LilyIrLlvm *Self,
                 LilyIrLlvmScope *Scope,
                 const LilyIrLlvmPending *Pending,
                 const LilyMirInstructionVal *LHS,
                 const LilyMirInstructionVal *RHS,
                 bool IsFloat,
                 const char *Name)
{
    LLVMValueRef left = LilyLLVMBuildVal(Self, Scope, Pending, LHS);
    LLVMValueRef right = LilyLLVMBuildVal(Self, Scope, Pending, RHS);

    ASSERT(left && right);

    return IsFloat ? LLVMBuildFSub(Self->builder, left, right, Name)
                   : LLVMBuildSub(Self->builder, left, right, Name);
}

LLVMValueRef
LilyLLVMBuildGetArray(const LilyIrLlvm *Self,
                      LilyIrLlvmScope *Scope,
                      const LilyIrLlvmPending *Pending,
                      const LilyMirInstructionVal *Val,
                      const LilyMirDt *DT,
                      const Vec *Indexes,
                      bool IsConst,
                      const char *Name)
{
    LLVMTypeRef ResultType = LilyLLVMGetType(Self, DT);

    ASSERT(ResultType);

    if (IsConst) {
        LLVMValueRef CurrentValue = LilyLLVMBuildVal(Self, Scope, Pending, Val);

        for (Usize i = 0; i < Indexes->len; ++i) {
            LilyMirInstructionVal *item = get__Vec(Indexes, i);
            Usize Index;

            switch (item->kind) {
                case LILY_MIR_INSTRUCTION_VAL_KIND_UINT:
                    Index = item->uint;
                    break;
                default:
                    UNREACHABLE("these values are not expected");
            }

            CurrentValue = LLVMBuildExtractValue(
              Self->builder, CurrentValue, Index, "extract.const.array");
        }

        return CurrentValue;
    }

    LLVMValueRef Pointer = LilyLLVMBuildVal(Self, Scope, Pending, Val);
    LLVMValueRef Layers[MAX_LAYERS] = { 0 };

    for (Usize i = 0; i < Indexes->len; ++i) {
        Layers[i] =
          LilyLLVMBuildVal(Self, Scope, Pending, get__Vec(Indexes, i));
    }

    return LLVMBuildGEP2(
      Self->builder, ResultType, Pointer, Layers, Indexes->len, Name);
}

LLVMValueRef
LilyLLVMBuildGetField(const LilyIrLlvm *Self,
                      LilyIrLlvmScope *Scope,
                      const LilyIrLlvmPending *Pending,
                      const LilyMirInstructionVal *Val,
                      const LilyMirDt *DT,
                      const Vec *Indexes,
                      const char *Name)
{
    LLVMTypeRef ResultType = LilyLLVMGetType(Self, DT);

    ASSERT(ResultType);

    LLVMValueRef Pointer = LilyLLVMBuildVal(Self, Scope, Pending, Val);
    LLVMValueRef Layers[MAX_RECORD_FIELDS] = { 0 };

    for (Usize i = 0; i < Indexes->len; ++i) {
        Layers[i] =
          LilyLLVMBuildVal(Self, Scope, Pending, get__Vec(Indexes, i));
    }

    return LLVMBuildGEP2(
      Self->builder, ResultType, Pointer, Layers, Indexes->len, Name);
}

LLVMValueRef
LilyLLVMBuildGetPtr(const LilyIrLlvm *Self,
                    LilyIrLlvmScope *Scope,
                    const LilyIrLlvmPending *Pending,
                    const LilyMirInstructionVal *Val,
                    const LilyMirDt *DT,
                    const char *Name)
{
    LLVMTypeRef Ty = LilyLLVMGetType(Self, DT);

    ASSERT(Ty);

    return LLVMBuildLoad2(
      Self->builder, Ty, LilyLLVMBuildVal(Self, Scope, Pending, Val), Name);
}

LLVMValueRef
LilyLLVMBuildJmp(const LilyIrLlvm *Self,
                 const LilyIrLlvmPending *Pending,
                 const char *Name)
{
    LLVMBasicBlockRef Dest = LilyLLVMCreateBasicBlock(Self, Pending, Name);
    LLVMValueRef jmp = LLVMBuildBr(Self->builder, Dest);

    add_block__LilyIrLlvmPending(Pending, Name, Dest);

    return jmp;
}

LLVMValueRef
LilyLLVMBuildJmpCond(const LilyIrLlvm *Self,
                     LilyIrLlvmScope *Scope,
                     const LilyIrLlvmPending *Pending,
                     const LilyMirInstructionVal *Cond,
                     const char *ThenName,
                     const char *ElseName)
{
    LLVMBasicBlockRef Then = LilyLLVMCreateBasicBlock(Self, Pending, ThenName);
    LLVMBasicBlockRef Else = LilyLLVMCreateBasicBlock(Self, Pending, ElseName);
    LLVMValueRef If = LilyLLVMBuildVal(Self, Scope, Pending, Cond);

    add_block__LilyIrLlvmPending(Pending, ThenName, Then);
    add_block__LilyIrLlvmPending(Pending, ElseName, Else);

    return LLVMBuildCondBr(Self->builder, If, Then, Else);
}

LLVMValueRef
LilyLLVMBuildLoad(const LilyIrLlvm *Self,
                  LilyIrLlvmScope *Scope,
                  const LilyIrLlvmPending *Pending,
                  const LilyMirInstructionVal *Val,
                  const LilyMirDt *DT,
                  const char *Name)
{
    LLVMTypeRef Ty = LilyLLVMGetType(Self, DT);

    ASSERT(Ty);

    return LLVMBuildLoad2(
      Self->builder, Ty, LilyLLVMBuildVal(Self, Scope, Pending, Val), Name);
}

LLVMValueRef
LilyLLVMBuildReg(const LilyIrLlvm *Self,
                 LilyIrLlvmScope *Scope,
                 const LilyIrLlvmPending *Pending,
                 const LilyMirInstruction *Inst,
                 const char *Name)
{
    LLVMValueRef res = LilyLLVMBuildInst(Self, Scope, Pending, Inst, Name);

    add__LilyIrLlvmScope(Scope, (char *)Name, res);

    return res;
}

LLVMValueRef
LilyLLVMBuildRet(const LilyIrLlvm *Self,
                 LilyIrLlvmScope *Scope,
                 const LilyIrLlvmPending *Pending,
                 const LilyMirInstruction *Inst)
{
    switch (Inst->kind) {
        case LILY_MIR_INSTRUCTION_KIND_VAL:
            switch (Inst->val->kind) {
                case LILY_MIR_INSTRUCTION_VAL_KIND_UNIT:
                    return LLVMBuildRetVoid(Self->builder);
                default:
                    break;
            }
        default:
            return LLVMBuildRet(
              Self->builder,
              LilyLLVMBuildInst(Self, Scope, Pending, Inst, NULL));
    }
}

LLVMValueRef
LilyLLVMBuildShl(const LilyIrLlvm *Self,
                 LilyIrLlvmScope *Scope,
                 const LilyIrLlvmPending *Pending,
                 const LilyMirInstructionVal *LHS,
                 const LilyMirInstructionVal *RHS,
                 const char *Name)
{
    LLVMValueRef left = LilyLLVMBuildVal(Self, Scope, Pending, LHS);
    LLVMValueRef right = LilyLLVMBuildVal(Self, Scope, Pending, RHS);

    ASSERT(left && right);

    return LLVMBuildShl(Self->builder, left, right, Name);
}

LLVMValueRef
LilyLLVMBuildShr(const LilyIrLlvm *Self,
                 LilyIrLlvmScope *Scope,
                 const LilyIrLlvmPending *Pending,
                 const LilyMirInstructionVal *LHS,
                 const LilyMirInstructionVal *RHS,
                 const char *Name)
{
    LLVMValueRef left = LilyLLVMBuildVal(Self, Scope, Pending, LHS);
    LLVMValueRef right = LilyLLVMBuildVal(Self, Scope, Pending, RHS);

    ASSERT(left && right);

    return LLVMBuildAShr(Self->builder, left, right, Name);
}

LLVMValueRef
LilyLLVMBuildStore(const LilyIrLlvm *Self,
                   LilyIrLlvmScope *Scope,
                   const LilyIrLlvmPending *Pending,
                   const LilyMirInstructionVal *Src,
                   const LilyMirInstructionVal *Dest,
                   const char *Name)
{
    LLVMValueRef dest = LilyLLVMBuildVal(Self, Scope, Pending, Dest);

    CurrentPtr = dest;

    LLVMValueRef src = LilyLLVMBuildVal(Self, Scope, Pending, Src);

    CurrentPtr = NULL;

    ASSERT(src && dest);

    if (src != dest) {
        return LLVMBuildStore(Self->builder, src, dest);
    }

    return dest;
}

LLVMValueRef
LilyLLVMBuildSwitch(const LilyIrLlvm *Self,
                    LilyIrLlvmScope *Scope,
                    const LilyIrLlvmPending *Pending,
                    const LilyMirInstructionVal *Val,
                    const Vec *Cases,
                    const LilyMirInstructionBlock *DefaultBlock)
{
    LLVMValueRef SwitchVal = LilyLLVMBuildVal(Self, Scope, Pending, Val);
    LLVMBasicBlockRef Else =
      LilyLLVMCreateBasicBlock(Self, Pending, DefaultBlock->name);
    LLVMValueRef Switch =
      LLVMBuildSwitch(Self->builder, SwitchVal, Else, Cases->len);

    add_block__LilyIrLlvmPending(Pending, DefaultBlock->name, Else);

    for (Usize i = 0; i < Cases->len; ++i) {
        LilyMirInstructionSwitchCase *case_ = get__Vec(Cases, i);
        LLVMValueRef CaseVal =
          LilyLLVMBuildVal(Self, Scope, Pending, case_->val);
        LLVMBasicBlockRef CaseBlock =
          LilyLLVMCreateBasicBlock(Self, Pending, case_->block_dest->name);

        LLVMAddCase(Switch, CaseVal, CaseBlock);
        add_block__LilyIrLlvmPending(
          Pending, case_->block_dest->name, CaseBlock);
    }

    return Switch;
}

LLVMValueRef
LilyLLVMBuildFunPrototype(const LilyIrLlvm *Self,
                          const Vec *Params,
                          const LilyMirDt *ReturnDT,
                          const char *SysName,
                          enum LilyMirLinkage Linkage)
{
    LLVMTypeRef FnSysParams[MAX_FUN_PARAMS] = { 0 };

    for (Usize i = 0; i < Params->len; ++i) {
        FnSysParams[i] = LilyLLVMGetType(Self, get__Vec(Params, i));
    }

    LLVMTypeRef FnSysType = LLVMFunctionType(
      LilyLLVMGetType(Self, ReturnDT), FnSysParams, Params->len, false);
    LLVMValueRef FnSys = LLVMAddFunction(Self->module, SysName, FnSysType);
    LilyLLVMSetLinkage(FnSys, Linkage);

    return FnSys;
}

LLVMValueRef
LilyLLVMBuildSysCall(const LilyIrLlvm *Self,
                     LilyIrLlvmScope *Scope,
                     const LilyIrLlvmPending *Pending,
                     const Vec *Params,
                     const LilyMirDt *ReturnDT,
                     const char *SysName,
                     const char *Name)
{
    LLVMValueRef SysFn = LLVMGetNamedFunction(Self->module, SysName);

    ASSERT(SysFn);

    LLVMValueRef Args[MAX_FUN_PARAMS] = { 0 };
    LLVMTypeRef ParamTypes[MAX_FUN_PARAMS] = { 0 };
    LLVMTypeRef ReturnType = LilyLLVMGetType(Self, ReturnDT);

    ASSERT(ReturnType);

    for (Usize i = 0; i < Params->len; ++i) {
        LLVMValueRef Arg =
          LilyLLVMBuildVal(Self, Scope, Pending, get__Vec(Params, i));

        ASSERT(Arg);

        Args[i] = Arg;
        ParamTypes[i] = LLVMTypeOf(Arg);
    }

    return LLVMBuildCall2(
      Self->builder,
      LLVMFunctionType(ReturnType, ParamTypes, Params->len, false),
      SysFn,
      Args,
      Params->len,
      Name);
}

LLVMValueRef
LilyLLVMBuildTrunc(const LilyIrLlvm *Self,
                   LilyIrLlvmScope *Scope,
                   const LilyIrLlvmPending *Pending,
                   const LilyMirInstructionVal *Src,
                   const LilyMirDt *DT,
                   const char *Name)
{
    LLVMValueRef Val = LilyLLVMBuildVal(Self, Scope, Pending, Src);
    LLVMTypeRef DestTy = LilyLLVMGetType(Self, DT);

    ASSERT(DestTy);

    return LLVMBuildTrunc(Self->builder, Val, DestTy, Name);
}

LLVMValueRef
LilyLLVMBuildVar(const LilyIrLlvm *Self,
                 LilyIrLlvmScope *Scope,
                 const LilyIrLlvmPending *Pending,
                 const LilyMirInstruction *Inst,
                 const char *Name)
{
    LLVMValueRef var = LilyLLVMBuildInst(Self, Scope, Pending, Inst, Name);

    add__LilyIrLlvmScope(Scope, (char *)Name, var);

    return var;
}

LLVMValueRef
LilyLLVMBuildXor(const LilyIrLlvm *Self,
                 LilyIrLlvmScope *Scope,
                 const LilyIrLlvmPending *Pending,
                 const LilyMirInstructionVal *LHS,
                 const LilyMirInstructionVal *RHS,
                 const char *Name)
{
    LLVMValueRef left = LilyLLVMBuildVal(Self, Scope, Pending, LHS);
    LLVMValueRef right = LilyLLVMBuildVal(Self, Scope, Pending, RHS);

    ASSERT(left && right);

    return LLVMBuildXor(Self->builder, left, right, Name);
}

LLVMValueRef
LilyLLVMBuildVal(const LilyIrLlvm *Self,
                 LilyIrLlvmScope *Scope,
                 const LilyIrLlvmPending *Pending,
                 const LilyMirInstructionVal *Val)
{
    switch (Val->kind) {
        // TODO: add const array
        case LILY_MIR_INSTRUCTION_VAL_KIND_ARRAY: {
            ASSERT(Val->dt->kind == LILY_MIR_DT_KIND_ARRAY);

            LLVMTypeRef ElementType = LilyLLVMGetType(Self, Val->dt->array.dt);
            // { [ ? x ElementType ], i64 }
            LLVMTypeRef array_type = LilyLLVMGetType(Self, Val->dt);
            LLVMValueRef array_ptr =
              LLVMBuildAlloca(Self->builder, array_type, "local.array");
            // [ ? x ElementType ]
            LLVMValueRef array_ptr_0 = LLVMBuildGEP2(
              Self->builder,
              ptr__LilyIrLlvm(Self, LLVMStructGetTypeAtIndex(array_type, 0)),
              array_ptr,
              (LLVMValueRef[]){ LLVMConstInt(i8__LilyIrLlvm(Self), 0, true) },
              1,
              "local.array.buffer");

            for (Usize i = 0; i < Val->array->len; ++i) {
                LLVMValueRef value_item = LilyLLVMBuildVal(
                  Self, Scope, Pending, get__Vec(Val->array, i));
                // ElementType
                LLVMValueRef array_ptr_item =
                  LLVMBuildGEP2(Self->builder,
                                ElementType,
                                array_ptr_0,
                                (LLVMValueRef[]){ LLVMConstInt(
                                  intptr__LilyIrLlvm(Self), i, true) },
                                1,
                                "local.array.item");

                ASSERT(array_ptr_item);

                LLVMBuildStore(Self->builder, value_item, array_ptr_item);
            }

            return array_ptr;
        }
        case LILY_MIR_INSTRUCTION_VAL_KIND_BYTES: {
            // [i8 x n]
            Usize bytes_buffer_len = strlen((const char *)Val->bytes);
            LLVMTypeRef bytes_buffer_type =
              LLVMArrayType(i8__LilyIrLlvm(Self), bytes_buffer_len + 1);
            LLVMValueRef global_bytes =
              LLVMAddGlobal(Self->module, bytes_buffer_type, "bytes");

            LLVMSetInitializer(
              global_bytes,
              LLVMConstStringInContext(Self->context,
                                       (const char *)Val->bytes,
                                       bytes_buffer_len + 1,
                                       true));
            LLVMSetUnnamedAddr(global_bytes, true);
            LilyLLVMSetLinkage(global_bytes, LILY_MIR_LINKAGE_PRIVATE);

            // Construct { i8*, iptr }
            LLVMTypeRef bytes_type = LilyLLVMGetType(Self, Val->dt);
            LLVMValueRef bytes_ptr =
              CurrentPtr
                ? CurrentPtr
                : LLVMBuildAlloca(Self->builder, bytes_type, "local.bytes");
            LLVMValueRef bytes_ptr_value = LLVMBuildLoad2(
              Self->builder, bytes_type, bytes_ptr, "local.bytes.load");

            LLVMBuildInsertValue(Self->builder,
                                 bytes_ptr_value,
                                 global_bytes,
                                 0,
                                 "local.bytes.buffer");
            LLVMBuildInsertValue(
              Self->builder,
              bytes_ptr_value,
              LLVMConstInt(intptr__LilyIrLlvm(Self), bytes_buffer_len, true),
              1,
              "local.bytes.len");

            return bytes_ptr;
        }
        case LILY_MIR_INSTRUCTION_VAL_KIND_CONST: {
            ASSERT(Scope);

            LLVMValueRef const_value =
              LLVMGetNamedGlobal(Self->module, Val->const_);

            ASSERT(const_value);

            return const_value;
        }
        case LILY_MIR_INSTRUCTION_VAL_KIND_CSTR: {
            // [i8 x n]
            Usize cstr_buffer_len = strlen(Val->cstr);
            LLVMTypeRef cstr_buffer_type =
              LLVMArrayType(i8__LilyIrLlvm(Self), cstr_buffer_len + 1);
            LLVMValueRef global_cstr =
              LLVMAddGlobal(Self->module, cstr_buffer_type, "cstr");

            LLVMSetInitializer(
              global_cstr,
              LLVMConstStringInContext(
                Self->context, Val->cstr, cstr_buffer_len + 1, true));
            LLVMSetUnnamedAddr(global_cstr, true);
            LilyLLVMSetLinkage(global_cstr, LILY_MIR_LINKAGE_PRIVATE);

            if (CurrentPtr) {
                LLVMBuildStore(Self->builder, global_cstr, CurrentPtr);

                return CurrentPtr;
            }

            return global_cstr;
        }
        case LILY_MIR_INSTRUCTION_VAL_KIND_EXCEPTION:
            TODO("Exception");
        case LILY_MIR_INSTRUCTION_VAL_KIND_FLOAT:
            return LLVMConstReal(LilyLLVMGetType(Self, Val->dt), Val->float_);
        case LILY_MIR_INSTRUCTION_VAL_KIND_INT:
            return LLVMConstInt(
              LilyLLVMGetType(Self, Val->dt), Val->int_, false);
        case LILY_MIR_INSTRUCTION_VAL_KIND_LIST:
            TODO("List");
        case LILY_MIR_INSTRUCTION_VAL_KIND_NIL:
            return LLVMConstNull(LilyLLVMGetType(Self, Val->dt));
        case LILY_MIR_INSTRUCTION_VAL_KIND_PARAM:
            ASSERT(Pending->current_fun);

            return LLVMGetParam(Pending->current_fun, Val->param);
        case LILY_MIR_INSTRUCTION_VAL_KIND_REG: {
            LLVMValueRef value = get__LilyIrLlvmScope(Scope, (char *)Val->reg);

            ASSERT(value);

            return value;
        }
        case LILY_MIR_INSTRUCTION_VAL_KIND_SLICE:
            TODO("slice");
        case LILY_MIR_INSTRUCTION_VAL_KIND_STR: {
            // [i8 x n]
            LLVMTypeRef str_buffer_type =
              LLVMArrayType(i8__LilyIrLlvm(Self), Val->str->len + 1);
            LLVMValueRef global_str =
              LLVMAddGlobal(Self->module, str_buffer_type, "str");

            LLVMSetInitializer(
              global_str,
              LLVMConstStringInContext(
                Self->context, Val->str->buffer, Val->str->len + 1, true));
            LLVMSetUnnamedAddr(global_str, true);
            LilyLLVMSetLinkage(global_str, LILY_MIR_LINKAGE_PRIVATE);
            LLVMSetGlobalConstant(global_str, true);

            // Construct { i8*, iptr }
            LLVMTypeRef str_type = LilyLLVMGetType(Self, Val->dt);
            LLVMValueRef str_ptr =
              CurrentPtr
                ? CurrentPtr
                : LLVMBuildAlloca(Self->builder, str_type, "local.str");
            LLVMValueRef str_buffer_ptr = LLVMBuildGEP2(
              Self->builder,
              str_type,
              str_ptr,
              (LLVMValueRef[]){ LLVMConstInt(i32__LilyIrLlvm(Self), 0, false),
                                LLVMConstInt(i32__LilyIrLlvm(Self), 0, false) },
              2,
              "local.str.buffer");
            LLVMValueRef str_len_ptr = LLVMBuildGEP2(
              Self->builder,
              str_type,
              str_ptr,
              (LLVMValueRef[]){ LLVMConstInt(i32__LilyIrLlvm(Self), 0, false),
                                LLVMConstInt(i32__LilyIrLlvm(Self), 1, false) },
              2,
              "local.str.len");

            LLVMSetIsInBounds(str_buffer_ptr, true);
            LLVMSetIsInBounds(str_len_ptr, true);

            LLVMBuildStore(Self->builder, global_str, str_buffer_ptr);
            LLVMBuildStore(
              Self->builder,
              LLVMConstInt(intptr__LilyIrLlvm(Self), Val->str->len, true),
              str_len_ptr);

            return str_ptr;
        }
        case LILY_MIR_INSTRUCTION_VAL_KIND_STRUCT:
            TODO("struct");
        case LILY_MIR_INSTRUCTION_VAL_KIND_TRACE:
            TODO("trace");
        case LILY_MIR_INSTRUCTION_VAL_KIND_TUPLE:
            TODO("tuple");
        case LILY_MIR_INSTRUCTION_VAL_KIND_UINT:
            return LLVMConstInt(
              LilyLLVMGetType(Self, Val->dt), Val->uint, true);
        case LILY_MIR_INSTRUCTION_VAL_KIND_UNDEF:
            return LLVMGetUndef(LilyLLVMGetType(Self, Val->dt));
        case LILY_MIR_INSTRUCTION_VAL_KIND_UNIT:
            UNREACHABLE("try to generate a return void instruction");
        case LILY_MIR_INSTRUCTION_VAL_KIND_VAR: {
            LLVMValueRef Res = get__LilyIrLlvmScope(Scope, (char *)Val->var);

            ASSERT(Res);

            return Res;
        }
        default:
            UNREACHABLE("unknown variant");
    }
}

LLVMValueRef
LilyLLVMBuildInst(const LilyIrLlvm *Self,
                  LilyIrLlvmScope *Scope,
                  const LilyIrLlvmPending *Pending,
                  const LilyMirInstruction *Inst,
                  const char *Name)
{
    LLVMValueRef res = NULL;

    switch (Inst->kind) {
        case LILY_MIR_INSTRUCTION_KIND_ALLOC:
            res = LilyLLVMBuildAlloc(Self, Inst->alloc.dt, Name);
            break;
        case LILY_MIR_INSTRUCTION_KIND_BITCAST:
            res = LilyLLVMBuildBitCast(
              Self, Scope, Pending, Inst->bitcast.val, Inst->bitcast.dt, Name);
            break;
        case LILY_MIR_INSTRUCTION_KIND_BITAND:
            res = LilyLLVMBuildAnd(
              Self, Scope, Pending, Inst->bitand.dest, Inst->bitand.src, Name);
            break;
        case LILY_MIR_INSTRUCTION_KIND_BITNOT:
            res =
              LilyLLVMBuildNot(Self, Scope, Pending, Inst->bitnot.src, Name);
            break;
        case LILY_MIR_INSTRUCTION_KIND_BITOR:
            res = LilyLLVMBuildOr(
              Self, Scope, Pending, Inst->bitor.dest, Inst->bitor.src, Name);
            break;
        case LILY_MIR_INSTRUCTION_KIND_BLOCK:
            LilyLLVMBuildBlock(Self,
                               Scope,
                               Pending,
                               Inst->block.insts,
                               Inst->block.limit->id,
                               Inst->block.id,
                               Inst->block.name);
            break;
        case LILY_MIR_INSTRUCTION_KIND_BUILTIN_CALL:
            res = LilyLLVMBuildBuiltinCall(Self,
                                           Scope,
                                           Pending,
                                           Inst->builtin_call.params,
                                           Inst->builtin_call.return_dt,
                                           Inst->builtin_call.name,
                                           Name);
            break;
        case LILY_MIR_INSTRUCTION_KIND_CALL:
            res = LilyLLVMBuildCall(Self,
                                    Scope,
                                    Pending,
                                    Inst->call.params,
                                    Inst->call.return_dt,
                                    Inst->call.name,
                                    Name);
            break;
        case LILY_MIR_INSTRUCTION_KIND_DROP:
            TODO("drop instruction");
        case LILY_MIR_INSTRUCTION_KIND_EXP:
            TODO("exp instruction");
        case LILY_MIR_INSTRUCTION_KIND_FADD:
            res = LilyLLVMBuildAdd(Self,
                                   Scope,
                                   Pending,
                                   Inst->fadd.dest,
                                   Inst->fadd.src,
                                   true,
                                   Name);
            break;
        case LILY_MIR_INSTRUCTION_KIND_FCMP_EQ:
            res = LilyLLVMBuildCmpEq(Self,
                                     Scope,
                                     Pending,
                                     Inst->fcmp_eq.dest,
                                     Inst->fcmp_eq.src,
                                     true,
                                     Name);
            break;
        case LILY_MIR_INSTRUCTION_KIND_FCMP_NE:
            res = LilyLLVMBuildCmpNe(Self,
                                     Scope,
                                     Pending,
                                     Inst->fcmp_ne.dest,
                                     Inst->fcmp_ne.src,
                                     true,
                                     Name);
            break;
        case LILY_MIR_INSTRUCTION_KIND_FCMP_LE:
            res = LilyLLVMBuildCmpLe(Self,
                                     Scope,
                                     Pending,
                                     Inst->fcmp_le.dest,
                                     Inst->fcmp_le.src,
                                     true,
                                     true,
                                     Name);
            break;
        case LILY_MIR_INSTRUCTION_KIND_FCMP_LT:
            res = LilyLLVMBuildCmpLt(Self,
                                     Scope,
                                     Pending,
                                     Inst->fcmp_lt.dest,
                                     Inst->fcmp_lt.src,
                                     true,
                                     true,
                                     Name);
            break;
        case LILY_MIR_INSTRUCTION_KIND_FCMP_GE:
            res = LilyLLVMBuildCmpGe(Self,
                                     Scope,
                                     Pending,
                                     Inst->fcmp_ge.dest,
                                     Inst->fcmp_ge.src,
                                     true,
                                     true,
                                     Name);
            break;
        case LILY_MIR_INSTRUCTION_KIND_FCMP_GT:
            res = LilyLLVMBuildCmpGt(Self,
                                     Scope,
                                     Pending,
                                     Inst->fcmp_gt.dest,
                                     Inst->fcmp_gt.src,
                                     true,
                                     true,
                                     Name);
            break;
        case LILY_MIR_INSTRUCTION_KIND_FDIV:
            res = LilyLLVMBuildDiv(Self,
                                   Scope,
                                   Pending,
                                   Inst->fdiv.dest,
                                   Inst->fdiv.src,
                                   true,
                                   true,
                                   Name);
            break;
        case LILY_MIR_INSTRUCTION_KIND_FMUL:
            res = LilyLLVMBuildMul(Self,
                                   Scope,
                                   Pending,
                                   Inst->fmul.dest,
                                   Inst->fmul.src,
                                   true,
                                   Name);
            break;
        case LILY_MIR_INSTRUCTION_KIND_FNEG:
            res = LilyLLVMBuildNeg(
              Self, Scope, Pending, Inst->fneg.src, true, Name);
            break;
        case LILY_MIR_INSTRUCTION_KIND_FREM:
            res = LilyLLVMBuildRem(Self,
                                   Scope,
                                   Pending,
                                   Inst->frem.dest,
                                   Inst->frem.src,
                                   true,
                                   true,
                                   Name);
            break;
        case LILY_MIR_INSTRUCTION_KIND_FSUB:
            res = LilyLLVMBuildSub(Self,
                                   Scope,
                                   Pending,
                                   Inst->fsub.dest,
                                   Inst->fsub.src,
                                   true,
                                   Name);
            break;
        case LILY_MIR_INSTRUCTION_KIND_GETARG:
            TODO("getarg");
        case LILY_MIR_INSTRUCTION_KIND_GETARRAY:
            res = LilyLLVMBuildGetArray(Self,
                                        Scope,
                                        Pending,
                                        Inst->getarray.val,
                                        Inst->getarray.dt,
                                        Inst->getarray.indexes,
                                        Inst->getarray.is_const,
                                        Name);
            break;
        case LILY_MIR_INSTRUCTION_KIND_GETLIST:
            TODO("getlist");
        case LILY_MIR_INSTRUCTION_KIND_GETSLICE:
            TODO("getslice");
        case LILY_MIR_INSTRUCTION_KIND_GETFIELD:
            res = LilyLLVMBuildGetField(Self,
                                        Scope,
                                        Pending,
                                        Inst->getfield.val,
                                        Inst->getfield.dt,
                                        Inst->getfield.indexes,
                                        Name);
            break;
        case LILY_MIR_INSTRUCTION_KIND_GETPTR:
            ASSERT(Inst->getptr.src->dt->kind == LILY_MIR_DT_KIND_PTR);

            res = LilyLLVMBuildGetPtr(Self,
                                      Scope,
                                      Pending,
                                      Inst->getptr.src,
                                      Inst->getptr.src->dt->ptr,
                                      Name);
            break;
        case LILY_MIR_INSTRUCTION_KIND_IADD:
            res = LilyLLVMBuildAdd(Self,
                                   Scope,
                                   Pending,
                                   Inst->iadd.dest,
                                   Inst->iadd.src,
                                   false,
                                   Name);
            break;
        case LILY_MIR_INSTRUCTION_KIND_ICMP_EQ:
            res = LilyLLVMBuildCmpEq(Self,
                                     Scope,
                                     Pending,
                                     Inst->icmp_eq.dest,
                                     Inst->icmp_eq.src,
                                     false,
                                     Name);
            break;
        case LILY_MIR_INSTRUCTION_KIND_ICMP_NE:
            res = LilyLLVMBuildCmpNe(Self,
                                     Scope,
                                     Pending,
                                     Inst->icmp_ne.dest,
                                     Inst->icmp_ne.src,
                                     false,
                                     Name);
            break;
        case LILY_MIR_INSTRUCTION_KIND_ICMP_LE:
        case LILY_MIR_INSTRUCTION_KIND_ICMP_LT:
        case LILY_MIR_INSTRUCTION_KIND_ICMP_GE:
        case LILY_MIR_INSTRUCTION_KIND_ICMP_GT:
        case LILY_MIR_INSTRUCTION_KIND_IDIV:
        case LILY_MIR_INSTRUCTION_KIND_IREM: {
            bool left_is_signed;
            bool right_is_signed;

            switch (Inst->kind) {
                case LILY_MIR_INSTRUCTION_KIND_ICMP_LE:
                    left_is_signed =
                      is_signed__LilyMirDt(Inst->icmp_le.dest->dt);
                    right_is_signed =
                      is_signed__LilyMirDt(Inst->icmp_le.src->dt);
                    break;
                case LILY_MIR_INSTRUCTION_KIND_ICMP_LT:
                    left_is_signed =
                      is_signed__LilyMirDt(Inst->icmp_lt.dest->dt);
                    right_is_signed =
                      is_signed__LilyMirDt(Inst->icmp_lt.src->dt);
                    break;
                case LILY_MIR_INSTRUCTION_KIND_ICMP_GE:
                    left_is_signed =
                      is_signed__LilyMirDt(Inst->icmp_ge.dest->dt);
                    right_is_signed =
                      is_signed__LilyMirDt(Inst->icmp_ge.src->dt);
                    break;
                case LILY_MIR_INSTRUCTION_KIND_ICMP_GT:
                    left_is_signed =
                      is_signed__LilyMirDt(Inst->icmp_gt.dest->dt);
                    right_is_signed =
                      is_signed__LilyMirDt(Inst->icmp_gt.src->dt);
                    break;
                case LILY_MIR_INSTRUCTION_KIND_IDIV:
                    left_is_signed = is_signed__LilyMirDt(Inst->idiv.dest->dt);
                    right_is_signed = is_signed__LilyMirDt(Inst->idiv.src->dt);
                    break;
                case LILY_MIR_INSTRUCTION_KIND_IREM:
                    left_is_signed = is_signed__LilyMirDt(Inst->irem.dest->dt);
                    right_is_signed = is_signed__LilyMirDt(Inst->irem.src->dt);
                    break;
                default:
                    UNREACHABLE("unknown variant in this context");
            }

            if (left_is_signed && right_is_signed) {
                switch (Inst->kind) {
                    case LILY_MIR_INSTRUCTION_KIND_ICMP_LE:
                        res = LilyLLVMBuildCmpLe(Self,
                                                 Scope,
                                                 Pending,
                                                 Inst->icmp_le.dest,
                                                 Inst->icmp_le.src,
                                                 false,
                                                 true,
                                                 Name);
                        break;
                    case LILY_MIR_INSTRUCTION_KIND_ICMP_LT:
                        res = LilyLLVMBuildCmpLt(Self,
                                                 Scope,
                                                 Pending,
                                                 Inst->icmp_lt.dest,
                                                 Inst->icmp_lt.src,
                                                 false,
                                                 true,
                                                 Name);
                        break;
                    case LILY_MIR_INSTRUCTION_KIND_ICMP_GE:
                        res = LilyLLVMBuildCmpGe(Self,
                                                 Scope,
                                                 Pending,
                                                 Inst->icmp_ge.dest,
                                                 Inst->icmp_ge.src,
                                                 false,
                                                 true,
                                                 Name);
                        break;
                    case LILY_MIR_INSTRUCTION_KIND_ICMP_GT:
                        res = LilyLLVMBuildCmpGt(Self,
                                                 Scope,
                                                 Pending,
                                                 Inst->icmp_gt.dest,
                                                 Inst->icmp_gt.src,
                                                 false,
                                                 true,
                                                 Name);
                        break;
                    case LILY_MIR_INSTRUCTION_KIND_IDIV:
                        res = LilyLLVMBuildDiv(Self,
                                               Scope,
                                               Pending,
                                               Inst->idiv.dest,
                                               Inst->idiv.src,
                                               false,
                                               true,
                                               Name);
                        break;
                    case LILY_MIR_INSTRUCTION_KIND_IREM:
                        res = LilyLLVMBuildRem(Self,
                                               Scope,
                                               Pending,
                                               Inst->irem.dest,
                                               Inst->irem.src,
                                               false,
                                               true,
                                               Name);
                        break;
                    default:
                        UNREACHABLE("this variant is not expected");
                }
            } else if (!left_is_signed && !right_is_signed) {
                switch (Inst->kind) {
                    case LILY_MIR_INSTRUCTION_KIND_ICMP_LE:
                        res = LilyLLVMBuildCmpLe(Self,
                                                 Scope,
                                                 Pending,
                                                 Inst->icmp_le.dest,
                                                 Inst->icmp_le.src,
                                                 false,
                                                 false,
                                                 Name);
                        break;
                    case LILY_MIR_INSTRUCTION_KIND_ICMP_LT:
                        res = LilyLLVMBuildCmpLt(Self,
                                                 Scope,
                                                 Pending,
                                                 Inst->icmp_lt.dest,
                                                 Inst->icmp_lt.src,
                                                 false,
                                                 false,
                                                 Name);
                        break;
                    case LILY_MIR_INSTRUCTION_KIND_ICMP_GE:
                        res = LilyLLVMBuildCmpGe(Self,
                                                 Scope,
                                                 Pending,
                                                 Inst->icmp_ge.dest,
                                                 Inst->icmp_ge.src,
                                                 false,
                                                 false,
                                                 Name);
                        break;
                    case LILY_MIR_INSTRUCTION_KIND_ICMP_GT:
                        res = LilyLLVMBuildCmpGt(Self,
                                                 Scope,
                                                 Pending,
                                                 Inst->icmp_gt.dest,
                                                 Inst->icmp_gt.src,
                                                 false,
                                                 false,
                                                 Name);
                        break;
                    case LILY_MIR_INSTRUCTION_KIND_IDIV:
                        res = LilyLLVMBuildDiv(Self,
                                               Scope,
                                               Pending,
                                               Inst->idiv.dest,
                                               Inst->idiv.src,
                                               false,
                                               false,
                                               Name);
                        break;
                    case LILY_MIR_INSTRUCTION_KIND_IREM:
                        res = LilyLLVMBuildRem(Self,
                                               Scope,
                                               Pending,
                                               Inst->irem.dest,
                                               Inst->irem.src,
                                               false,
                                               false,
                                               Name);
                        break;
                    default:
                        UNREACHABLE("this variant is not expected");
                }
            } else {
                UNREACHABLE("both sides of the expression are expected with "
                            "the same integer types (unsigned or signed).");
            }

            break;
        }
        case LILY_MIR_INSTRUCTION_KIND_IMUL:
            res = LilyLLVMBuildMul(Self,
                                   Scope,
                                   Pending,
                                   Inst->imul.dest,
                                   Inst->imul.src,
                                   false,
                                   Name);
            break;
        case LILY_MIR_INSTRUCTION_KIND_INCTRACE:
            TODO("inctrace");
        case LILY_MIR_INSTRUCTION_KIND_INEG:
            res = LilyLLVMBuildNeg(
              Self, Scope, Pending, Inst->ineg.src, false, Name);
            break;
        case LILY_MIR_INSTRUCTION_KIND_ISOK:
            TODO("isok");
        case LILY_MIR_INSTRUCTION_KIND_ISERR:
            TODO("iserr");
        case LILY_MIR_INSTRUCTION_KIND_ISUB:
            res = LilyLLVMBuildSub(Self,
                                   Scope,
                                   Pending,
                                   Inst->isub.dest,
                                   Inst->isub.src,
                                   false,
                                   Name);
            break;
        case LILY_MIR_INSTRUCTION_KIND_JMP:
            res = LilyLLVMBuildJmp(Self, Pending, Inst->jmp->name);
            break;
        case LILY_MIR_INSTRUCTION_KIND_JMPCOND:
            res = LilyLLVMBuildJmpCond(Self,
                                       Scope,
                                       Pending,
                                       Inst->jmpcond.cond,
                                       Inst->jmpcond.then_block->name,
                                       Inst->jmpcond.else_block->name);
            break;
        case LILY_MIR_INSTRUCTION_KIND_LEN:
            res = LilyLLVMBuildLen(Self, Scope, Pending, Inst->len.src, Name);
            break;
        case LILY_MIR_INSTRUCTION_KIND_LOAD:
            res = LilyLLVMBuildLoad(
              Self, Scope, Pending, Inst->load.src.src, Inst->load.dt, Name);
            break;
        case LILY_MIR_INSTRUCTION_KIND_MAKEREF:
            TODO("makeref");
        case LILY_MIR_INSTRUCTION_KIND_MAKEOPT:
            TODO("makeopt");
        case LILY_MIR_INSTRUCTION_KIND_NON_NIL:
            TODO("non nil");
        case LILY_MIR_INSTRUCTION_KIND_NOT:
            res = LilyLLVMBuildNot(Self, Scope, Pending, Inst->not.src, Name);
            break;
        case LILY_MIR_INSTRUCTION_KIND_REF_PTR:
            TODO("ref ptr");
        case LILY_MIR_INSTRUCTION_KIND_REG:
            res = LilyLLVMBuildReg(
              Self, Scope, Pending, Inst->reg.inst, Inst->reg.name);
            break;
        case LILY_MIR_INSTRUCTION_KIND_RET:
            res = LilyLLVMBuildRet(Self, Scope, Pending, Inst->ret);
            break;
        case LILY_MIR_INSTRUCTION_KIND_SHL:
            res = LilyLLVMBuildShl(
              Self, Scope, Pending, Inst->shl.dest, Inst->shl.src, Name);
            break;
        case LILY_MIR_INSTRUCTION_KIND_SHR:
            res = LilyLLVMBuildShr(
              Self, Scope, Pending, Inst->shr.dest, Inst->shr.src, Name);
            break;
        case LILY_MIR_INSTRUCTION_KIND_STORE:
            res = LilyLLVMBuildStore(
              Self, Scope, Pending, Inst->store.src, Inst->store.dest, Name);
            break;
        case LILY_MIR_INSTRUCTION_KIND_CONST:
        case LILY_MIR_INSTRUCTION_KIND_FUN:
        case LILY_MIR_INSTRUCTION_KIND_STRUCT:
            UNREACHABLE("not expected in this context");
        case LILY_MIR_INSTRUCTION_KIND_SWITCH:
            res = LilyLLVMBuildSwitch(Self,
                                      Scope,
                                      Pending,
                                      Inst->switch_.val,
                                      Inst->switch_.cases,
                                      Inst->switch_.default_block);
            break;
        case LILY_MIR_INSTRUCTION_KIND_SYS_CALL:
            res = LilyLLVMBuildSysCall(Self,
                                       Scope,
                                       Pending,
                                       Inst->sys_call.params,
                                       Inst->sys_call.return_dt,
                                       Inst->sys_call.name,
                                       Name);
            break;
        case LILY_MIR_INSTRUCTION_KIND_TRUNC:
            res = LilyLLVMBuildTrunc(
              Self, Scope, Pending, Inst->trunc.val, Inst->trunc.dt, Name);
            break;
        case LILY_MIR_INSTRUCTION_KIND_TRY:
            TODO("try");
        case LILY_MIR_INSTRUCTION_KIND_TRY_PTR:
            TODO("try ptr");
        case LILY_MIR_INSTRUCTION_KIND_UNREACHABLE:
            res = LLVMBuildUnreachable(Self->builder);
            break;
        case LILY_MIR_INSTRUCTION_KIND_VAL:
            res = LilyLLVMBuildVal(Self, Scope, Pending, Inst->val);
            break;
        case LILY_MIR_INSTRUCTION_KIND_VAR:
            res = LilyLLVMBuildVar(
              Self, Scope, Pending, Inst->var.inst, Inst->var.name);
            break;
        case LILY_MIR_INSTRUCTION_KIND_XOR:
            res = LilyLLVMBuildXor(
              Self, Scope, Pending, Inst->xor.dest, Inst->xor.src, Name);
            break;
        default:
            UNREACHABLE("unknown variant");
    }

    if (Inst->kind != LILY_MIR_INSTRUCTION_KIND_BLOCK) {
        ASSERT(res);
    }

    return res;
}

LLVMTypeRef
LilyLLVMGetType(const LilyIrLlvm *Self, const LilyMirDt *DT)
{
    switch (DT->kind) {
        case LILY_MIR_DT_KIND_ANY:
            return ptr__LilyIrLlvm(Self, i8__LilyIrLlvm(Self));
        case LILY_MIR_DT_KIND_ARRAY: {
            LLVMTypeRef ElementType = LilyLLVMGetType(Self, DT->array.dt);

            ASSERT(ElementType);

            if (DT->array.len.is_undef) {
                return LLVMStructType(
                  (LLVMTypeRef[]){ ptr__LilyIrLlvm(Self, ElementType),
                                   intptr__LilyIrLlvm(Self) },
                  2,
                  false);
            }

            return LLVMStructType(
              (LLVMTypeRef[]){ LLVMArrayType(ElementType, DT->array.len.len),
                               intptr__LilyIrLlvm(Self) },
              2,
              false);
        }
        case LILY_MIR_DT_KIND_BYTES:
            return LLVMStructType(
              (LLVMTypeRef[]){ ptr__LilyIrLlvm(Self, i8__LilyIrLlvm(Self)),
                               intptr__LilyIrLlvm(Self) },
              2,
              false);
        case LILY_MIR_DT_KIND_CSTR:
            return ptr__LilyIrLlvm(Self, i8__LilyIrLlvm(Self));
        case LILY_MIR_DT_KIND_RESULT: {
            LLVMTypeRef ok = LilyLLVMGetType(Self, DT->result.ok);
            LLVMTypeRef err = LilyLLVMGetType(Self, DT->result.err);

            ASSERT(ok && err);

            return LLVMStructType(
              (LLVMTypeRef[]){ i8__LilyIrLlvm(Self), ok, err }, 3, false);
        }
        case LILY_MIR_DT_KIND_I1:
            return i1__LilyIrLlvm(Self);
        case LILY_MIR_DT_KIND_I8:
        case LILY_MIR_DT_KIND_U8:
            return i8__LilyIrLlvm(Self);
        case LILY_MIR_DT_KIND_I16:
        case LILY_MIR_DT_KIND_U16:
            return i16__LilyIrLlvm(Self);
        case LILY_MIR_DT_KIND_I32:
        case LILY_MIR_DT_KIND_U32:
            return i32__LilyIrLlvm(Self);
        case LILY_MIR_DT_KIND_I64:
        case LILY_MIR_DT_KIND_U64:
            return i64__LilyIrLlvm(Self);
        case LILY_MIR_DT_KIND_ISIZE:
        case LILY_MIR_DT_KIND_USIZE:
            return intptr__LilyIrLlvm(Self);
        case LILY_MIR_DT_KIND_F32:
            return float__LilyIrLlvm(Self);
        case LILY_MIR_DT_KIND_F64:
            return double__LilyIrLlvm(Self);
        case LILY_MIR_DT_KIND_LIST:
            TODO("list");
        case LILY_MIR_DT_KIND_PTR: {
            LLVMTypeRef ptr_type = LilyLLVMGetType(Self, DT->ptr);

            ASSERT(ptr_type);

            return ptr__LilyIrLlvm(Self, ptr_type);
        }
        case LILY_MIR_DT_KIND_REF: {
            LLVMTypeRef ptr_type = LilyLLVMGetType(Self, DT->ref);

            ASSERT(ptr_type);

            return ptr__LilyIrLlvm(Self, ptr_type);
        }
        case LILY_MIR_DT_KIND_STR:
            return LLVMStructType(
              (LLVMTypeRef[]){ ptr__LilyIrLlvm(Self, i8__LilyIrLlvm(Self)),
                               intptr__LilyIrLlvm(Self) },
              2,
              false);
        case LILY_MIR_DT_KIND_STRUCT: {
            LLVMTypeRef ElementTypes[MAX_RECORD_FIELDS] = { 0 };

            for (Usize i = 0; i < DT->struct_->len; ++i) {
                LLVMTypeRef ElementType =
                  LilyLLVMGetType(Self, get__Vec(DT->struct_, i));

                ASSERT(ElementType);

                ElementTypes[i] = ElementType;
            }

            return LLVMStructType(ElementTypes, DT->struct_->len, false);
        }
        case LILY_MIR_DT_KIND_STRUCT_NAME: {
            LLVMTypeRef res =
              LLVMGetTypeByName2(Self->context, DT->struct_name);

            if (res) {
                return res;
            }

            res = LilyLLVMGetTypeByName(Self, DT->struct_name);

            ASSERT(res);

            return res;
        }
        case LILY_MIR_DT_KIND_TRACE: {
            LLVMTypeRef trace_type = LilyLLVMGetType(Self, DT->trace);

            ASSERT(trace_type);

            return LLVMStructType(
              (LLVMTypeRef[]){ trace_type, intptr__LilyIrLlvm(Self) },
              2,
              false);
        }
        case LILY_MIR_DT_KIND_TUPLE: {
            LLVMTypeRef ElementTypes[MAX_TUPLE_ITEMS] = { 0 };

            for (Usize i = 0; i < DT->tuple->len; ++i) {
                LLVMTypeRef ElementType =
                  LilyLLVMGetType(Self, get__Vec(DT->tuple, i));

                ASSERT(ElementType);

                ElementTypes[i] = ElementType;
            }

            return LLVMStructType(ElementTypes, DT->tuple->len, false);
        }
        case LILY_MIR_DT_KIND_UNIT:
            return void__LilyIrLlvm(Self);
        case LILY_MIR_DT_KIND_C_VA_ARG:
            TODO("va arg");
        default:
            UNREACHABLE("unknown variant");
    }
}

LLVMValueRef
LilyLLVMPrepareConst(const LilyIrLlvm *Self,
                     const enum LilyMirLinkage Linkage,
                     const LilyMirDt *DT,
                     const char *Name)
{
    LLVMTypeRef ConstTy = LilyLLVMGetType(Self, DT);

    ASSERT(ConstTy);

    LLVMValueRef Const = LLVMAddGlobal(Self->module, ConstTy, Name);

    LLVMSetGlobalConstant(Const, true);
    LilyLLVMSetLinkage(Const, Linkage);

    return Const;
}

LLVMValueRef
LilyLLVMPrepareFunction(const LilyIrLlvm *Self,
                        const enum LilyMirLinkage Linkage,
                        const Vec *Args,
                        const LilyMirDt *ReturnDT,
                        const char *Name)
{
    // TODO: manage attribute
    if (!strcmp(Name, "main")) {
        LilyMainFunType = LLVMFunctionType(
          LilyLLVMGetType(Self, ReturnDT),
          (LLVMTypeRef[]){ i32__LilyIrLlvm(Self),
                           ptr__LilyIrLlvm(Self, i8__LilyIrLlvm(Self)) },
          2,
          false);
        LLVMValueRef LilyMainFun =
          LLVMAddFunction(Self->module, LILY_MAIN, LilyMainFunType);

        LilyLLVMSetLinkage(LilyMainFun, LILY_MIR_LINKAGE_PRIVATE);

        return LilyMainFun;
    }

    LLVMTypeRef ReturnTy = LilyLLVMGetType(Self, ReturnDT);

    ASSERT(ReturnTy);

    LLVMTypeRef ParamTypes[MAX_FUN_PARAMS] = { 0 };

    for (Usize i = 0; i < Args->len; ++i) {
        const LilyMirInstruction *Arg =
          get_arg__LilyMirInstruction(get__Vec(Args, i));

        ASSERT(Arg);

        // TODO: add optimization on non-nil parameter
        LLVMTypeRef ParamType = LilyLLVMGetType(Self, Arg->arg.dt);

        ASSERT(ParamType);

        ParamTypes[i] = ParamType;
    }

    // TODO: add support for va_arg
    LLVMTypeRef FunType =
      LLVMFunctionType(ReturnTy, ParamTypes, Args->len, false);
    LLVMValueRef Fun = LLVMAddFunction(Self->module, Name, FunType);

    // Set param alignment
    for (Usize i = 0; i < Args->len; ++i) {
        const LilyMirInstruction *Arg =
          get_arg__LilyMirInstruction(get__Vec(Args, i));

        switch (Arg->arg.dt->kind) {
            case LILY_MIR_DT_KIND_REF: {
                LLVMValueRef Arg = LLVMGetParam(Fun, i);

                LLVMSetParamAlignment(
                  Arg,
                  LLVMABIAlignmentOfType(Self->target_data, LLVMTypeOf(Arg)));

                break;
            }
            default:
                break;
        }
    }

    LilyLLVMSetLinkage(Fun, Linkage);

    return Fun;
}

void
LilyLLVMPrepareModule(const LilyIrLlvm *Self, OrderedHashMap *Insts)
{
    OrderedHashMapIter iter = NEW(OrderedHashMapIter, Insts);
    LilyMirInstruction *Inst = NULL;

    while ((Inst = next__OrderedHashMapIter(&iter))) {
        switch (Inst->kind) {
            case LILY_MIR_INSTRUCTION_KIND_CONST:
                LilyLLVMPrepareConst(Self,
                                     Inst->const_.linkage,
                                     Inst->const_.val->dt,
                                     Inst->const_.name);
                break;
            case LILY_MIR_INSTRUCTION_KIND_FUN:
                LilyLLVMPrepareFunction(Self,
                                        Inst->fun.linkage,
                                        Inst->fun.args,
                                        Inst->fun.return_data_type,
                                        Inst->fun.name);
                break;
            case LILY_MIR_INSTRUCTION_KIND_FUN_PROTOTYPE:
                LilyLLVMBuildFunPrototype(Self,
                                          Inst->fun_prototype.params,
                                          Inst->fun_prototype.return_data_type,
                                          Inst->fun_prototype.name,
                                          Inst->fun_prototype.linkage);
                break;
            case LILY_MIR_INSTRUCTION_KIND_STRUCT:
                LilyLLVMPrepareStruct(Self, Inst->struct_.name);
                break;
            default:
                UNREACHABLE("this variant is not expected in global");
        }
    }
}

void
LilyLLVMFinishFunction(const LilyIrLlvm *Self,
                       LLVMValueRef Fn,
                       OrderedHashMap *Insts)
{
    LilyIrLlvmPending Pending = NEW(LilyIrLlvmPending);
    LilyIrLlvmScope Scope = NEW(LilyIrLlvmScope);

    Pending.current_fun = Fn;

    {
        OrderedHashMapIter iter = NEW(OrderedHashMapIter, Insts);
        LilyMirInstruction *Inst = NULL;

        while ((Inst = next__OrderedHashMapIter(&iter))) {
            LilyLLVMBuildInst(Self, &Scope, &Pending, Inst, NULL);
        }
    }

    FREE(LilyIrLlvmPending, &Pending);
    FREE(LilyIrLlvmScope, &Scope);
}

void
LilyLLVMFinishStruct(const LilyIrLlvm *Self,
                     LLVMTypeRef Struct,
                     const Vec *Types)
{
    LLVMTypeRef StructTypes[MAX_RECORD_FIELDS] = { 0 };

    for (Usize i = 0; i < Types->len; ++i) {
        LLVMTypeRef StructType = LilyLLVMGetType(Self, get__Vec(Types, i));

        ASSERT(StructType);

        StructTypes[i] = StructType;
    }

    LLVMStructSetBody(Struct, StructTypes, Types->len, false);
}

void
LilyLLVMRunModule(const LilyIrLlvm *Self, OrderedHashMap *Insts)
{
    OrderedHashMapIter iter = NEW(OrderedHashMapIter, Insts);
    LilyMirInstruction *Inst = NULL;

    while ((Inst = next__OrderedHashMapIter(&iter))) {
        switch (Inst->kind) {
            case LILY_MIR_INSTRUCTION_KIND_CONST: {
                LLVMValueRef Const =
                  LLVMGetNamedGlobal(Self->module, Inst->const_.name);

                ASSERT(Const);

                LilyLLVMFinishConst(Self, Const, Inst->const_.val);

                break;
            }
            case LILY_MIR_INSTRUCTION_KIND_FUN: {
                if (!strcmp(Inst->fun.name, "main")) {
                    LLVMValueRef Fn =
                      LLVMGetNamedFunction(Self->module, LILY_MAIN);

                    ASSERT(Fn);

                    LilyLLVMFinishFunction(Self, Fn, Inst->fun.insts);

                    LLVMTypeRef MainFnType = LLVMFunctionType(
                      i32__LilyIrLlvm(Self),
                      (LLVMTypeRef[]){
                        i32__LilyIrLlvm(Self),
                        ptr__LilyIrLlvm(Self, i8__LilyIrLlvm(Self)) },
                      2,
                      false);

                    ASSERT(LilyMainFunType);

                    LLVMValueRef MainFn =
                      LLVMAddFunction(Self->module, "main", MainFnType);

                    LLVMBasicBlockRef entry_main =
                      LLVMAppendBasicBlockInContext(
                        Self->context, MainFn, "entry");

                    LLVMPositionBuilderAtEnd(Self->builder, entry_main);

                    LLVMBuildCall2(Self->builder,
                                   LilyMainFunType,
                                   Fn,
                                   (LLVMValueRef[]){ LLVMGetParam(MainFn, 0),
                                                     LLVMGetParam(MainFn, 1) },
                                   2,
                                   "");

                    // TODO: Check the return value of the `lily.main` function.
                    // For the moment we return 0, in all cases.
                    LLVMBuildRet(Self->builder,
                                 LLVMConstInt(i32__LilyIrLlvm(Self), 0, false));
                } else {
                    LLVMValueRef Fn =
                      LLVMGetNamedFunction(Self->module, Inst->fun.name);

                    ASSERT(Fn);

                    LilyLLVMFinishFunction(Self, Fn, Inst->fun.insts);
                }

                break;
            }
            case LILY_MIR_INSTRUCTION_KIND_FUN_PROTOTYPE:
                continue;
            case LILY_MIR_INSTRUCTION_KIND_STRUCT: {
                LLVMTypeRef Struct =
                  LLVMGetTypeByName2(Self->context, Inst->struct_.name);

                ASSERT(Struct);

                LilyLLVMFinishStruct(Self, Struct, Inst->struct_.fields);

                break;
            }
            default:
                UNREACHABLE("this variant is not expected in global");
        }
    }
}
