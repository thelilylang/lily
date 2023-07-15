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
        case LILY_MIR_INSTRUCTION_VAL_KIND_ARRAY:
            TODO("Array");
        case LILY_MIR_INSTRUCTION_VAL_KIND_BYTES:
            TODO("Bytes");
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
        case LILY_MIR_INSTRUCTION_VAL_KIND_STR:
            TODO("str");
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
            TODO("var");
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

    ASSERT(res);

    add__LilyIrLlvmScope(Scope, (char *)Name, res);

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

            return LLVMArrayType(ElementType, DT->array.len);
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
