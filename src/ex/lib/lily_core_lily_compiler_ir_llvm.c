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

#ifndef LILY_EX_LIB_LILY_CORE_LILY_COMPILER_IR_LLVM_C
#define LILY_EX_LIB_LILY_CORE_LILY_COMPILER_IR_LLVM_C

#include <core/lily/compiler/ir/llvm/attr.h>
#include <core/lily/compiler/ir/llvm/dump.h>
#include <core/lily/compiler/ir/llvm/lily.h>
#include <core/lily/compiler/ir/llvm/pending.h>
#include <core/lily/compiler/ir/llvm/primary.h>
#include <core/lily/compiler/ir/llvm/scope.h>

#include "lily_base.c"
#include "lily_core_lily_compiler_driver.c"
#include "lily_core_lily_mir.c"

// <core/lily/compiler/ir/llvm/attr.h>
extern inline LLVMAttributeRef
align_attr__LilyIrLlvm(const LilyIrLlvm *self, Uint64 align);

extern inline LLVMAttributeRef
alwaysinline_attr__LilyIrLlvm(const LilyIrLlvm *self);

extern inline LLVMAttributeRef
builtin_attr__LilyIrLlvm(const LilyIrLlvm *self);

extern inline LLVMAttributeRef
cold_attr__LilyIrLlvm(const LilyIrLlvm *self);

extern inline LLVMAttributeRef
hot_attr__LilyIrLlvm(const LilyIrLlvm *self);

extern inline LLVMAttributeRef
immarg_attr__LilyIrLlvm(const LilyIrLlvm *self);

extern inline LLVMAttributeRef
noalias_attr__LilyIrLlvm(const LilyIrLlvm *self);

extern inline LLVMAttributeRef
nobuiltin_attr__LilyIrLlvm(const LilyIrLlvm *self);

extern inline LLVMAttributeRef
nocallback_attr__LilyIrLlvm(const LilyIrLlvm *self);

extern inline LLVMAttributeRef
nofree_attr__LilyIrLlvm(const LilyIrLlvm *self);

extern inline LLVMAttributeRef
noinline_attr__LilyIrLlvm(const LilyIrLlvm *self);

extern inline LLVMAttributeRef
nonnull_attr__LilyIrLlvm(const LilyIrLlvm *self);

extern inline LLVMAttributeRef
norecurse_attr__LilyIrLlvm(const LilyIrLlvm *self);

extern inline LLVMAttributeRef
noreturn_attr__LilyIrLlvm(const LilyIrLlvm *self);

extern inline LLVMAttributeRef
nosync_attr__LilyIrLlvm(const LilyIrLlvm *self);

extern inline LLVMAttributeRef
nounwind_attr__LilyIrLlvm(const LilyIrLlvm *self);

extern inline LLVMAttributeRef
optnone_attr__LilyIrLlvm(const LilyIrLlvm *self);

extern inline LLVMAttributeRef
readonly_attr__LilyIrLlvm(const LilyIrLlvm *self);

extern inline LLVMAttributeRef
readnone_attr__LilyIrLlvm(const LilyIrLlvm *self);

extern inline LLVMAttributeRef
speculatable_attr__LilyIrLlvm(const LilyIrLlvm *self);

extern inline LLVMAttributeRef
uwtable_attr__LilyIrLlvm(const LilyIrLlvm *self);

extern inline LLVMAttributeRef
writeonly_attr__LilyIrLlvm(const LilyIrLlvm *self);

extern inline LLVMAttributeRef
willreturn_attr__LilyIrLlvm(const LilyIrLlvm *self);

// <core/lily/compiler/ir/llvm/dump.h>
extern inline void
dump__LilyIrLlvm(const LilyIrLlvm *self);

// <core/lily/compiler/ir/llvm/lily.h>
extern inline LLVMTypeRef
LilyLLVMPrepareStruct(const LilyIrLlvm *Self, const char *Name);

extern inline void
LilyLLVMFinishConst(const LilyIrLlvm *Self,
                    LLVMValueRef Const,
                    const LilyMirInstructionVal *Val);

// <core/lily/compiler/ir/llvm/pending.h>
extern inline CONSTRUCTOR(LilyIrLlvmPending, LilyIrLlvmPending);

extern inline LLVMBasicBlockRef
get_block__LilyIrLlvmPending(const LilyIrLlvmPending *self, const char *name);

extern inline DESTRUCTOR(LilyIrLlvmPending, const LilyIrLlvmPending *self);

// <core/lily/compiler/ir/llvm/primary.h>
extern inline LLVMTypeRef
i1__LilyIrLlvm(const LilyIrLlvm *self);

extern inline LLVMTypeRef
i8__LilyIrLlvm(const LilyIrLlvm *self);

extern inline LLVMTypeRef
i16__LilyIrLlvm(const LilyIrLlvm *self);

extern inline LLVMTypeRef
i32__LilyIrLlvm(const LilyIrLlvm *self);

extern inline LLVMTypeRef
i64__LilyIrLlvm(const LilyIrLlvm *self);

extern inline LLVMTypeRef
i128__LilyIrLlvm(const LilyIrLlvm *self);

extern inline LLVMTypeRef
intptr__LilyIrLlvm(const LilyIrLlvm *self);

extern inline LLVMTypeRef
float__LilyIrLlvm(const LilyIrLlvm *self);

extern inline LLVMTypeRef
double__LilyIrLlvm(const LilyIrLlvm *self);

extern inline LLVMTypeRef
void__LilyIrLlvm(const LilyIrLlvm *self);

extern inline LLVMTypeRef
ptr__LilyIrLlvm(const LilyIrLlvm *self, LLVMTypeRef ptr_type);

// <core/lily/compiler/ir/llvm/scope.h>
extern inline LLVMValueRef
get__LilyIrLlvmScopeItem(const LilyIrLlvmScopeItem *self, const char *name);

extern inline CONSTRUCTOR(LilyIrLlvmScope, LilyIrLlvmScope);

extern inline void
add_scope_item__LilyIrLlvmScope(const LilyIrLlvmScope *self,
                                Usize limit_block_id);

#endif // LILY_EX_LIB_LILY_CORE_LILY_COMPILER_IR_LLVM_C