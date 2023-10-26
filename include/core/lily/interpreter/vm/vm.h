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

#ifndef LILY_CORE_LILY_INTERPRETER_VM_H
#define LILY_CORE_LILY_INTERPRETER_VM_H

#include <base/alloc.h>
#include <base/macros.h>
#include <base/new.h>
#include <base/units.h>

#include <core/lily/interpreter/vm/memory.h>
#include <core/lily/interpreter/vm/value.h>
#include <core/lily/mir/mir.h>

#define DEFAULT_MAX_STACK_CAPACITY MiB // ~1 MB
#define DEFAULT_STACK_CAPACITY 8
#define DEFAULT_STACK_COEFF_INC 2

enum LilyInterpreterVMStackFrameReturnKind
{
    LILY_INTERPRETER_VM_STACK_FRAME_RETURN_KIND_NORMAL,
    LILY_INTERPRETER_VM_STACK_FRAME_RETURN_KIND_RAISE
};

typedef struct LilyInterpreterVMStackFrameReturn
{
    enum LilyInterpreterVMStackFrameReturnKind kind;
    LilyInterpreterValue *value;
} LilyInterpreterVMStackFrameReturn;

/**
 *
 * @brief Construct LilyInterpreterVMStackFrameReturn type
 * (LILY_INTERPRETER_VM_STACK_FRAME_RETURN_KIND_NORMAL).
 */
inline VARIANT_CONSTRUCTOR(LilyInterpreterVMStackFrameReturn,
                           LilyInterpreterVMStackFrameReturn,
                           normal,
                           LilyInterpreterValue *value)
{
    return (LilyInterpreterVMStackFrameReturn){
        .kind = LILY_INTERPRETER_VM_STACK_FRAME_RETURN_KIND_NORMAL,
        .value = value
    };
}

/**
 *
 * @brief Construct LilyInterpreterVMStackFrameReturn type
 * (LILY_INTERPRETER_VM_STACK_FRAME_RETURN_KIND_RAISE).
 */
inline VARIANT_CONSTRUCTOR(LilyInterpreterVMStackFrameReturn,
                           LilyInterpreterVMStackFrameReturn,
                           raise,
                           LilyInterpreterValue *value)
{
    return (LilyInterpreterVMStackFrameReturn){
        .kind = LILY_INTERPRETER_VM_STACK_FRAME_RETURN_KIND_RAISE,
        .value = value
    };
}

/**
 *
 * @brief Free LilyInterpreterVMStackFrameReturn type.
 */
inline DESTRUCTOR(LilyInterpreterVMStackFrameReturn,
                  LilyInterpreterVMStackFrameReturn *self)
{
    FREE(LilyInterpreterValue, &self->value);
}

typedef struct LilyInterpreterVMStackBlockFrame
{
    Vec *names;         // Vec<char* (&)>*
    HashMap *variables; // HashMap<char* (&), LilyInterpreterValue* (&)>*
    HashMap *regs;      // HashMap<char* (&), LilyInterpreterValue* (&)>*
    Usize begin; // index of the begin of the stack frame on the stack buffer
    Usize end;   // index of the end of the stack frame on the stack buffer, 0
                 // mean no next block stack frame
    struct LilyInterpreterVMStackBlockFrame
      *parent; // LilyInterpreterVMStackBlockFrame*? (&)
} LilyInterpreterVMStackBlockFrame;

/**
 *
 * @brief Construct LilyInterpreterVMStackBlockFrame type.
 */
CONSTRUCTOR(LilyInterpreterVMStackBlockFrame *,
            LilyInterpreterVMStackBlockFrame,
            char *name,
            Usize begin,
            Usize end);

/**
 *
 * @brief Add name to names Vec.
 */
inline void
add_name__LilyInterpreterVMStackBlockFrame(
  const LilyInterpreterVMStackBlockFrame *self,
  char *name)
{
    return push__Vec(self->names, name);
}

/**
 *
 * @brief Add reg.
 */
void
add_reg__LilyInterpreterVMStackBlockFrame(
  const LilyInterpreterVMStackBlockFrame *self,
  char *name,
  LilyInterpreterValue *value);

/**
 *
 * @brief Add variable.
 */
void
add_variable__LilyInterpreterVMStackBlockFrame(
  const LilyInterpreterVMStackBlockFrame *self,
  char *name,
  LilyInterpreterValue *value);

/**
 *
 * @brief Search reg and return the associated value.
 */
LilyInterpreterValue *
search_reg__LilyInterpreterVMStackBlockFrame(
  const LilyInterpreterVMStackBlockFrame *self,
  char *name);

/**
 *
 * @brief Search variable (in the block frame) and return the associated value.
 * @note This function cannot find global variables, only local ones.
 */
LilyInterpreterValue *
search_variable__LilyInterpreterVMStackBlockFrame(
  const LilyInterpreterVMStackBlockFrame *self,
  char *name);

/**
 *
 * @brief Free LilyInterpreterVMStackBlockFrame type.
 */
DESTRUCTOR(LilyInterpreterVMStackBlockFrame,
           LilyInterpreterVMStackBlockFrame **self);

typedef struct LilyInterpreterVMStackFrame
{
    const char *name; // const char* (&)
    LilyInterpreterValue *params[MAX_FUN_PARAMS];
    Usize params_len;
    LilyInterpreterVMStackFrameReturn return_;
    Usize begin; // index of the begin of the stack frame on the stack buffer
    Usize end;   // index of the end of the stack frame on the stack buffer, 0
                 // mean no next stack frame
    Usize current_block_frame_limit_id;
    Usize block_frames_len;
    LilyInterpreterVMStackBlockFrame **block_frames;
    struct LilyInterpreterVMStackFrame *next; // LilyInterpreterVMStackFrame*?
} LilyInterpreterVMStackFrame;

/**
 *
 * @brief Construct LilyInterpreterVMStackFrame type.
 */
CONSTRUCTOR(LilyInterpreterVMStackFrame *,
            LilyInterpreterVMStackFrame,
            const char *name,
            LilyInterpreterValue **params,
            Usize params_len,
            Usize begin,
            Usize current_block_frame_limit_id,
            Usize block_frames_len);

/**
 *
 * @breif Attache a new stack frame to an other stack frame.
 */
LilyInterpreterVMStackFrame *
attach_stack_frame__LilyInterpreterVMStackFrame(
  LilyInterpreterVMStackFrame *self,
  const char *name,
  LilyInterpreterValue **params,
  Usize params_len,
  Usize begin,
  Usize current_block_frame_limit_id,
  Usize block_frames_len);

/**
 *
 * @brief Add block frame to `block_frames` field.
 */
void
add_block_frame__LilyInterpreterVMStackFrame(LilyInterpreterVMStackFrame *self,
                                             Usize current_block_frame_limit_id,
                                             char *name,
                                             Usize begin);

/**
 *
 * @brief Free LilyInterpreterVMStackFrame type.
 */
DESTRUCTOR(LilyInterpreterVMStackFrame, LilyInterpreterVMStackFrame **self);

typedef struct LilyInterpreterVMStack
{
    // The bottom of the stack (before the first call frame) is used to store
    // constant value.
    LilyInterpreterValue *top; // LilyInterpreterValue* (&)
    LilyInterpreterValue **buffer;
    LilyInterpreterVMStackFrame *current_frame; // LilyInterpreterVMStackFrame*?
    Usize len;
    Usize capacity;
    Usize max_capacity;
} LilyInterpreterVMStack;

/**
 *
 * @brief Construct LilyInterpreterVMStack type.
 */
inline CONSTRUCTOR(LilyInterpreterVMStack,
                   LilyInterpreterVMStack,
                   Usize max_capacity)
{
    return (LilyInterpreterVMStack){
        .top = NULL,
        .buffer = lily_calloc(DEFAULT_STACK_CAPACITY, PTR_SIZE),
        .current_frame = NULL,
        .capacity = DEFAULT_STACK_CAPACITY,
        .max_capacity =
          max_capacity == 0 ? DEFAULT_MAX_STACK_CAPACITY : max_capacity,
        .len = 0
    };
}

/**
 *
 * @brief Push value on the stack.
 */
void
push__LilyInterpreterVMStack(LilyInterpreterVMStack *self,
                             LilyInterpreterValue *value);

/**
 *
 * @brief Remove and return the value on the top.
 */
LilyInterpreterValue *
pop__LilyInterpreterVMStack(LilyInterpreterVMStack *self);

/**
 *
 * @brief Return the value on the top.
 */
LilyInterpreterValue *
peek__LilyInterpreterVMStack(LilyInterpreterVMStack *self);

/**
 *
 * @brief Set frame on the stack.
 */
void
set_frame__LilyInterpreterVMStack(LilyInterpreterVMStack *self,
                                  LilyInterpreterVMStackFrame *frame);

/**
 *
 * @brief Clean stack of the current block.
 */
void
clean_block_stack__LilyInterpreterVMStack(LilyInterpreterVMStack *self);

/**
 *
 * @brief Clean frame on the stack, and return `frame return`.
 */
LilyInterpreterVMStackFrameReturn
clean_frame__LilyInterpreterVMStack(LilyInterpreterVMStack *self);

/**
 *
 * @brief Load const from the stack.
 */
LilyInterpreterValue *
load_const_value__LilyInterpreterVMStack(LilyInterpreterVMStack *self,
                                         const char *name);

/**
 *
 * @brief Free LilyInterpreterVMStack type.
 */
DESTRUCTOR(LilyInterpreterVMStack, LilyInterpreterVMStack *self);

typedef struct LilyInterpreterVMResources
{
    Vec *args; // Vec<char*>*
} LilyInterpreterVMResources;

/**
 *
 * @brief Construct LilyInterpreterVMResources type.
 */
inline CONSTRUCTOR(LilyInterpreterVMResources,
                   LilyInterpreterVMResources,
                   Vec *args)
{
    return (LilyInterpreterVMResources){ .args = args };
}

/**
 *
 * @brief Free LilyInterpreterVMResources type.
 */
inline DESTRUCTOR(LilyInterpreterVMResources,
                  const LilyInterpreterVMResources *self)
{
    FREE(Vec, self->args);
}

typedef struct LilyInterpreterVM
{
    LilyInterpreterMemory memory;
    const LilyMirModule *module;           // const LilyMirModule* (&)
    const LilyMirInstruction *entry_point; // const LilyMirInstruction* (&)
                                           // main function
    LilyInterpreterVMResources resources;
    // TODO: Maybe add VM config type
    bool check_overflow; // also check underflow
} LilyInterpreterVM;

/**
 *
 * @brief Construct LilyInterpreterVM type.
 * @param heap_capacity Default heap capacity if is set to 0.
 * @param stack_capacity Default stack capacity if is set to 0.
 */
CONSTRUCTOR(LilyInterpreterVM,
            LilyInterpreterVM,
            Usize heap_capacity,
            Usize stack_capacity,
            const LilyMirModule *module,
            LilyInterpreterVMResources resources,
            bool check_overflow);

/**
 *
 * @brief Set max stack capacity.
 */
void
set_max_stack__LilyInterpreterVM(Usize max_stack);

/**
 *
 * @brief Set max heap capacity.
 */
inline void
set_max_heap__LilyInterpreterVM(LilyInterpreterVM *self, Usize max_heap)
{
    self->memory.capacity = max_heap == 0 ? __max_capacity__$Alloc() : max_heap;
}

/**
 *
 * @brief Run VM.
 */
void
run__LilyInterpreterVM(LilyInterpreterVM *self);

/**
 *
 * @brief Free LilyInterpreterVM type.
 */
DESTRUCTOR(LilyInterpreterVM, const LilyInterpreterVM *self);

#endif // LILY_CORE_LILY_INTERPRETER_VM_H
