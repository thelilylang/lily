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

#include <core/lily/interpreter/memory.h>
#include <core/lily/interpreter/value.h>
#include <core/lily/mir/mir.h>

#define DEFAULT_MAX_STACK_CAPACITY 1024 * 1024
#define DEFAULT_STACK_CAPACITY 8
#define DEFAULT_STACK_COEFF_INC 2

typedef struct LilyInterpreterVMStack
{
    LilyInterpreterValue *top; // LilyInterpreterValue* (&)
    LilyInterpreterValue *buffer;
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
    return (LilyInterpreterVMStack){ .top = NULL,
                                     .buffer = lily_calloc(
                                       DEFAULT_STACK_CAPACITY,
                                       sizeof(LilyInterpreterValue)),
                                     .capacity = DEFAULT_STACK_CAPACITY,
                                     .max_capacity = max_capacity,
                                     .len = 0 };
}

/**
 *
 * @brief Push value on the stack.
 */
void
push__LilyInterpreterVMStack(LilyInterpreterVMStack *self,
                             LilyInterpreterValue value);

/**
 *
 * @brief Remove and return the value on the top.
 */
LilyInterpreterValue
pop__LilyInterpreterVMStack(LilyInterpreterVMStack *self);

/**
 *
 * @brief Free LilyInterpreterVMStack type.
 */
DESTRUCTOR(LilyInterpreterVMStack, const LilyInterpreterVMStack *self);

typedef struct LilyInterpreterVM
{
    LilyInterpreterMemory memory;
    LilyInterpreterVMStack stack;
    const LilyMirModule *module;           // const LilyMirModule* (&)
    const LilyMirInstruction *entry_point; // const LilyMirInstruction* (&)
                                           // main function
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
            const LilyMirModule *module);

/**
 *
 * @brief Run VM.
 */
void
run__LilyInterpreterVM(LilyInterpreterVM *self);

#endif // LILY_CORE_LILY_INTERPRETER_VM_H
