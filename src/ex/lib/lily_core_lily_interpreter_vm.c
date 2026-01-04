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

#ifndef LILY_EX_LIB_LILY_CORE_LILY_INTERPRETER_VM_C
#define LILY_EX_LIB_LILY_CORE_LILY_INTERPRETER_VM_C

#include <core/lily/interpreter/vm/memory.h>
#include <core/lily/interpreter/vm/runtime/operator.h>
#include <core/lily/interpreter/vm/value.h>
#include <core/lily/interpreter/vm/vm.h>

#include "lily_base.c"
#include "lily_builtin.c"
#include "lily_sys.c"

// <core/lily/interpreter/vm/memory.h>
extern inline VARIANT_CONSTRUCTOR(LilyInterpreterMemory,
                                  LilyInterpreterMemory,
                                  capacity,
                                  Usize capacity);

extern inline CONSTRUCTOR(LilyInterpreterMemory, LilyInterpreterMemory);

// <core/lily/interpreter/vm/value.h>
extern inline VARIANT_CONSTRUCTOR(LilyInterpreterValue,
                                  LilyInterpreterValue,
                                  bytes,
                                  LilyInterpreterValueBytes *bytes);

extern inline VARIANT_CONSTRUCTOR(LilyInterpreterValue,
                                  LilyInterpreterValue,
                                  cstr,
                                  char *cstr);

extern inline VARIANT_CONSTRUCTOR(
  LilyInterpreterValue,
  LilyInterpreterValue,
  dynamic_array,
  LilyInterpreterValueDynamicArray *dynamic_array);

extern inline VARIANT_CONSTRUCTOR(LilyInterpreterValue,
                                  LilyInterpreterValue,
                                  int8,
                                  Int8 int8);

extern inline VARIANT_CONSTRUCTOR(LilyInterpreterValue,
                                  LilyInterpreterValue,
                                  int16,
                                  Int16 int16);

extern inline VARIANT_CONSTRUCTOR(LilyInterpreterValue,
                                  LilyInterpreterValue,
                                  int32,
                                  Int32 int32);

extern inline VARIANT_CONSTRUCTOR(LilyInterpreterValue,
                                  LilyInterpreterValue,
                                  int64,
                                  Int64 int64);

extern inline VARIANT_CONSTRUCTOR(LilyInterpreterValue,
                                  LilyInterpreterValue,
                                  isize,
                                  Isize isize);

extern inline VARIANT_CONSTRUCTOR(LilyInterpreterValue,
                                  LilyInterpreterValue,
                                  float,
                                  Float64 float_);

extern inline VARIANT_CONSTRUCTOR(LilyInterpreterValue,
                                  LilyInterpreterValue,
                                  list,
                                  LilyInterpreterValueList *list);

extern inline VARIANT_CONSTRUCTOR(
  LilyInterpreterValue,
  LilyInterpreterValue,
  multi_pointers_array,
  LilyInterpreterValueMultiPointersArray *multi_pointers_array);

extern inline VARIANT_CONSTRUCTOR(LilyInterpreterValue,
                                  LilyInterpreterValue,
                                  ptr,
                                  void *ptr);

extern inline VARIANT_CONSTRUCTOR(LilyInterpreterValue,
                                  LilyInterpreterValue,
                                  result,
                                  LilyInterpreterValueResult *result);

extern inline VARIANT_CONSTRUCTOR(LilyInterpreterValue,
                                  LilyInterpreterValue,
                                  sized_array,
                                  LilyInterpreterValueSizedArray *sized_array);

extern inline VARIANT_CONSTRUCTOR(LilyInterpreterValue,
                                  LilyInterpreterValue,
                                  str,
                                  LilyInterpreterValueStr *str);

extern inline VARIANT_CONSTRUCTOR(LilyInterpreterValue,
                                  LilyInterpreterValue,
                                  struct,
                                  LilyInterpreterValueStruct *struct_);

extern inline VARIANT_CONSTRUCTOR(LilyInterpreterValue,
                                  LilyInterpreterValue,
                                  uint8,
                                  Uint8 uint8);

extern inline VARIANT_CONSTRUCTOR(LilyInterpreterValue,
                                  LilyInterpreterValue,
                                  uint16,
                                  Uint16 uint16);

extern inline VARIANT_CONSTRUCTOR(LilyInterpreterValue,
                                  LilyInterpreterValue,
                                  uint32,
                                  Uint32 uint32);

extern inline VARIANT_CONSTRUCTOR(LilyInterpreterValue,
                                  LilyInterpreterValue,
                                  uint64,
                                  Uint64 uint64);

extern inline VARIANT_CONSTRUCTOR(LilyInterpreterValue,
                                  LilyInterpreterValue,
                                  usize,
                                  Usize usize);

extern inline CONSTRUCTOR(LilyInterpreterValue,
                          LilyInterpreterValue,
                          enum LilyInterpreterValueKind kind);

extern inline LilyInterpreterValue *
ref__LilyInterpreterValue(LilyInterpreterValue *self);

extern inline LilyInterpreterValue
get__LilyInterpreterValueDynamicArray(
  const LilyInterpreterValueDynamicArray *self,
  Usize n);

extern inline LilyInterpreterValueDynamicArray *
ref__LilyInterpreterValueDynamicArray(LilyInterpreterValueDynamicArray *self);

extern inline LilyInterpreterValue *
get__LilyInterpreterValueMultiPointersArray(
  const LilyInterpreterValueMultiPointersArray *self,
  Usize n);

extern inline LilyInterpreterValueMultiPointersArray *
ref__LilyInterpreterValueMultiPointersArray(
  LilyInterpreterValueMultiPointersArray *self);

extern inline LilyInterpreterValue *
get__LilyInterpreterValueSizedArray(const LilyInterpreterValueSizedArray *self,
                                    Usize n);

extern inline LilyInterpreterValueSizedArray *
ref__LilyInterpreterValueSizedArray(LilyInterpreterValueSizedArray *self);

extern inline Uint8
get__LilyInterpreterValueBytes(const LilyInterpreterValueBytes *self, Usize n);

extern inline LilyInterpreterValueBytes *
ref__LilyInterpreterValueBytes(LilyInterpreterValueBytes *self);

extern inline LilyInterpreterValueList *
ref__LilyInterpreterValueList(LilyInterpreterValueList *self);

extern inline LilyInterpreterValueResult *
ref__LilyInterpreterValueResult(LilyInterpreterValueResult *self);

extern inline char
get__LilyInterpreterValueStr(const LilyInterpreterValueStr *self, Usize n);

extern inline LilyInterpreterValueStr *
ref__LilyInterpreterValueStr(LilyInterpreterValueStr *self);

extern inline LilyInterpreterValueStruct *
ref__LilyInterpreterValueStruct(LilyInterpreterValueStruct *self);

// <core/lily/interpreter/vm/vm.h>
extern inline VARIANT_CONSTRUCTOR(LilyInterpreterVMStackFrameReturn,
                                  LilyInterpreterVMStackFrameReturn,
                                  normal,
                                  LilyInterpreterValue value);

extern inline VARIANT_CONSTRUCTOR(LilyInterpreterVMStackFrameReturn,
                                  LilyInterpreterVMStackFrameReturn,
                                  raise,
                                  LilyInterpreterValue value);

extern inline DESTRUCTOR(LilyInterpreterVMStackFrameReturn,
                         LilyInterpreterVMStackFrameReturn *self);

extern inline CONSTRUCTOR(LilyInterpreterVMResources,
                          LilyInterpreterVMResources,
                          Vec *args);

extern inline DESTRUCTOR(LilyInterpreterVMResources,
                         const LilyInterpreterVMResources *self);

extern inline void
set_max_heap__LilyInterpreterVM(LilyInterpreterVM *self, Usize max_heap);

// <core/lily/interpreter/vm/runtime/operator.h>
extern inline Float64
mod__Float64(Float64 lhs, Float64 rhs);

#endif // LILY_EX_LIB_LILY_CORE_LILY_INTERPRETER_VM_C