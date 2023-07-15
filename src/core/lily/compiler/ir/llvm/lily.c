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

#include <base/assert.h>

#include <core/lily/analysis/checked/limits.h>
#include <core/lily/compiler/ir/llvm/lily.h>
#include <core/lily/compiler/ir/llvm/primary.h>

#include <stdio.h>
#include <stdlib.h>

LLVMValueRef
LilyLLVMBuildAlloc(const LilyIrLlvm *Self,
                   const LilyIrLlvmPending *Pending,
                   const LilyMirInstruction *Inst,
                   const char *Name)
{
    LLVMTypeRef alloc_type = LilyLLVMGetType(Self, Pending, Inst->alloc.dt);

    ASSERT(alloc_type);

    return LLVMBuildAlloca(Self->builder, alloc_type, Name);
}

LLVMValueRef
LilyLLVMBuildAnd(const LilyIrLlvm *Self,
                 const LilyIrLlvmScope *Scope,
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
                     const LilyIrLlvmScope *Scope,
                     const LilyIrLlvmPending *Pending,
                     const LilyMirInstructionVal *Val,
                     const LilyMirDt *DestDt,
                     const char *Name)
{
    LLVMValueRef val = LilyLLVMBuildVal(Self, Scope, Pending, Val);
    LLVMTypeRef dest_dt = LilyLLVMGetType(Self, Pending, DestDt);

    ASSERT(val && dest_dt);

    return LLVMBuildBitCast(Self->builder, val, dest_dt, Name);
}

LLVMValueRef
LilyLLVMBuildNot(const LilyIrLlvm *Self,
                 const LilyIrLlvmScope *Scope,
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
                const LilyIrLlvmScope *Scope,
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
                   const LilyIrLlvmScope *Scope,
                   const LilyIrLlvmPending *Pending,
                   Vec *Insts,
                   const char *Name)
{
    LLVMBasicBlockRef block =
      LLVMCreateBasicBlockInContext(Self->context, Name);

    LLVMPositionBuilderAtEnd(Self->builder, block);

    for (Usize i = 0; i < Insts->len; ++i) {
        LilyLLVMBuildInst(Self, Scope, Pending, get__Vec(Insts, i), NULL);
    }

    return block;
}

LLVMValueRef
LilyLLVMBuildAdd(const LilyIrLlvm *Self,
                 const LilyIrLlvmScope *Scope,
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
LilyLLVMBuildVal(const LilyIrLlvm *Self,
                 const LilyIrLlvmScope *Scope,
                 const LilyIrLlvmPending *Pending,
                 const LilyMirInstructionVal *Val)
{
    switch (Val->kind) {
        // TODO: add const array
        case LILY_MIR_INSTRUCTION_VAL_KIND_ARRAY: {
            ASSERT(Val->dt->kind == LILY_MIR_DT_KIND_ARRAY);

            LLVMTypeRef ElementType =
              LilyLLVMGetType(Self, Pending, Val->dt->array.dt);
            LLVMTypeRef array_type = LilyLLVMGetType(Self, Pending, Val->dt);
            LLVMValueRef array_ptr =
              LLVMBuildAlloca(Self->builder, array_type, "local.array");

            for (Usize i = 0; i < Val->array->len; ++i) {
                LLVMValueRef value_item = LilyLLVMBuildVal(
                  Self, Scope, Pending, get__Vec(Val->array, i));
                LLVMValueRef array_ptr_item = LLVMBuildGEP2(
                  Self->builder,
                  ElementType,
                  array_ptr,
                  (LLVMValueRef[]){
                    LLVMConstInt(i8__LilyIrLlvm(Self), 0, true),
                    LLVMConstInt(i8__LilyIrLlvm(Self), 0, true),
                    LLVMConstInt(intptr__LilyIrLlvm(Self), i, true) },
                  3,
                  "local.array.item");

                ASSERT(array_ptr_item);

                LLVMBuildStore(Self->builder, array_ptr_item, value_item);
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

            LLVMSetInitializer(global_bytes,
                               LLVMConstString((const char *)Val->bytes,
                                               bytes_buffer_len,
                                               false));
            LLVMSetUnnamedAddr(global_bytes, true);

            // Construct { i8*, iptr }
            LLVMTypeRef bytes_type = LilyLLVMGetType(Self, Pending, Val->dt);
            LLVMValueRef bytes_ptr =
              LLVMBuildAlloca(Self->builder, bytes_type, "local.bytes");
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
        case LILY_MIR_INSTRUCTION_VAL_KIND_EXCEPTION:
            TODO("Exception");
        case LILY_MIR_INSTRUCTION_VAL_KIND_FLOAT:
            return LLVMConstReal(LilyLLVMGetType(Self, Pending, Val->dt),
                                 Val->float_);
        case LILY_MIR_INSTRUCTION_VAL_KIND_INT:
            return LLVMConstInt(
              LilyLLVMGetType(Self, Pending, Val->dt), Val->uint, false);
        case LILY_MIR_INSTRUCTION_VAL_KIND_LIST:
            TODO("List");
        case LILY_MIR_INSTRUCTION_VAL_KIND_NIL:
            return LLVMConstNull(LilyLLVMGetType(Self, Pending, Val->dt));
        case LILY_MIR_INSTRUCTION_VAL_KIND_PARAM:
            ASSERT(Pending->current_fun);

            return LLVMGetParam(Pending->current_fun, Val->param);
        case LILY_MIR_INSTRUCTION_VAL_KIND_REG:
            return get__LilyIrLlvmScope(Scope, Val->reg->buffer);
        case LILY_MIR_INSTRUCTION_VAL_KIND_SLICE:
            TODO("slice");
        case LILY_MIR_INSTRUCTION_VAL_KIND_STR: {
            // [i32 x n]
            LLVMTypeRef str_buffer_type =
              LLVMArrayType(i32__LilyIrLlvm(Self), Val->str->len + 1);
            LLVMValueRef global_str =
              LLVMAddGlobal(Self->module, str_buffer_type, "str");

            LLVMSetInitializer(
              global_str,
              LLVMConstString(Val->str->buffer, Val->str->len, false));
            LLVMSetUnnamedAddr(global_str, true);

            // Construct { i32*, iptr }
            LLVMTypeRef str_type = LilyLLVMGetType(Self, Pending, Val->dt);
            LLVMValueRef str_ptr =
              LLVMBuildAlloca(Self->builder, str_type, "local.str");
            LLVMValueRef str_ptr_value = LLVMBuildLoad2(
              Self->builder, str_type, str_ptr, "local.str.load");

            LLVMBuildInsertValue(
              Self->builder, str_ptr_value, global_str, 0, "local.str.buffer");
            LLVMBuildInsertValue(
              Self->builder,
              str_ptr_value,
              LLVMConstInt(intptr__LilyIrLlvm(Self), Val->str->len, true),
              1,
              "local.str.len");

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
              LilyLLVMGetType(Self, Pending, Val->dt), Val->uint, true);
        case LILY_MIR_INSTRUCTION_VAL_KIND_UNDEF:
            return LLVMGetUndef(LilyLLVMGetType(Self, Pending, Val->dt));
        case LILY_MIR_INSTRUCTION_VAL_KIND_UNIT:
            UNREACHABLE("try to generate a return void instruction");
        case LILY_MIR_INSTRUCTION_VAL_KIND_VAR:
            return get__LilyIrLlvmScope(Scope, Val->reg->buffer);
        default:
            UNREACHABLE("unknown variant");
    }
}

LLVMValueRef
LilyLLVMBuildInst(const LilyIrLlvm *Self,
                  const LilyIrLlvmScope *Scope,
                  const LilyIrLlvmPending *Pending,
                  const LilyMirInstruction *Inst,
                  const char *Name)
{
    LLVMValueRef res = NULL;

    switch (Inst->kind) {
        case LILY_MIR_INSTRUCTION_KIND_ALLOC:
            res = LilyLLVMBuildAlloc(Self, Pending, Inst, Name);
            break;
        case LILY_MIR_INSTRUCTION_KIND_AND:
            res = LilyLLVMBuildAnd(
              Self, Scope, Pending, Inst->and.dest, Inst->and.src, Name);
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
            LilyLLVMBuildBlock(Self, Scope, Pending, Inst->block.insts, Name);
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
        case LILY_MIR_INSTRUCTION_KIND_FADD:
            res = LilyLLVMBuildAdd(Self,
                                   Scope,
                                   Pending,
                                   Inst->fadd.dest,
                                   Inst->fadd.src,
                                   true,
                                   Name);
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
LilyLLVMGetType(const LilyIrLlvm *Self,
                const LilyIrLlvmPending *Pending,
                const LilyMirDt *DT)
{
    switch (DT->kind) {
        case LILY_MIR_DT_KIND_ANY:
            return ptr__LilyIrLlvm(Self, i8__LilyIrLlvm(Self));
        case LILY_MIR_DT_KIND_ARRAY: {
            LLVMTypeRef ElementType =
              LilyLLVMGetType(Self, Pending, DT->array.dt);

            ASSERT(ElementType);

            return LLVMStructType(
              (LLVMTypeRef[]){ LLVMArrayType(ElementType, DT->array.len),
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
        case LILY_MIR_DT_KIND_RESULT: {
            LLVMTypeRef ok = LilyLLVMGetType(Self, Pending, DT->result.ok);
            LLVMTypeRef err = LilyLLVMGetType(Self, Pending, DT->result.err);

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
        case LILY_MIR_DT_KIND_PTR:
        case LILY_MIR_DT_KIND_REF: {
            LLVMTypeRef ptr_type = LilyLLVMGetType(Self, Pending, DT->ptr);

            ASSERT(ptr_type);

            return ptr__LilyIrLlvm(Self, ptr_type);
        }
        case LILY_MIR_DT_KIND_STR:
            return LLVMStructType(
              (LLVMTypeRef[]){ ptr__LilyIrLlvm(Self, i32__LilyIrLlvm(Self)),
                               intptr__LilyIrLlvm(Self) },
              2,
              false);
        case LILY_MIR_DT_KIND_STRUCT: {
            LLVMTypeRef ElementTypes[MAX_RECORD_FIELDS] = { 0 };

            for (Usize i = 0; i < DT->struct_->len; ++i) {
                LLVMTypeRef ElementType =
                  LilyLLVMGetType(Self, Pending, get__Vec(DT->struct_, i));

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

            res = get_struct__LilyIrLlvmPending(Pending, DT->struct_name);

            ASSERT(res);

            return res;
        }
        case LILY_MIR_DT_KIND_TRACE: {
            LLVMTypeRef trace_type = LilyLLVMGetType(Self, Pending, DT->trace);

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
                  LilyLLVMGetType(Self, Pending, get__Vec(DT->tuple, i));

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
