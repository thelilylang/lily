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

#include <core/lily/interpreter/runtime.h>
#include <core/lily/interpreter/vm.h>

#define VM_PUSH(stack, value) push__LilyInterpreterVMStack(self, value)
#define VM_POP(stack, value) pop__LilyInterpreterVMStack(self, value)

#define VM_INST(name) inst__##name:
#define VM_CALL_INST(name) goto inst__##name;

static threadlocal LilyMirInstructionBlock *current_block = NULL;
static threadlocal LilyMirInstruction *current_inst = NULL;

void
push__LilyInterpreterVMStack(LilyInterpreterVMStack *self,
                             LilyInterpreterValue value)
{
    if (self->len > self->max_capacity) {
        RUNTIME_ERROR("Stack overflow");
    }

    if (self->len == self->capacity) {
        self->capacity *= DEFAULT_STACK_COEFF_INC;
        self->buffer =
          realloc(self->buffer, sizeof(LilyInterpreterValue) * self->capacity);
    }

    self->buffer[self->len] = value;
    self->top = &self->buffer[self->len];
    ++self->len;
}

LilyInterpreterValue
pop__LilyInterpreterVMStack(LilyInterpreterVMStack *self)
{
    if (self->len == 0) {
        RUNTIME_ERROR("Empty stack");
    }

    LilyInterpreterValue top_value = self->buffer[--self->len];
    // Usize down_capacity = self->capacity / DEFAULT_STACK_COEFF_INC;

    // if (self->len < down_capacity) {
    // 	self->capacity = down_capacity;
    // 	self->buffer = realloc(self->buffer, sizeof(LilyInterpreterValue) *
    // self->capacity);
    // }

    if (self->len > 0) {
        self->top = &self->buffer[self->len - 1];
    } else {
        self->top = NULL;
    }

    return top_value;
}

DESTRUCTOR(LilyInterpreterVMStack, const LilyInterpreterVMStack *self)
{
    lily_free(self->buffer);
}

CONSTRUCTOR(LilyInterpreterVM,
            LilyInterpreterVM,
            Usize heap_capacity,
            Usize stack_capacity,
            const LilyMirModule *module)
{
    LilyInterpreterMemory memory =
      heap_capacity
        ? NEW_VARIANT(LilyInterpreterMemory, capacity, heap_capacity)
        : NEW(LilyInterpreterMemory);
    LilyInterpreterVMStack stack =
      stack_capacity ? NEW(LilyInterpreterVMStack, stack_capacity)
                     : NEW(LilyInterpreterVMStack, DEFAULT_MAX_STACK_CAPACITY);
    LilyMirInstruction *entry_point =
      get__OrderedHashMap(module->insts, "main");

    ASSERT(entry_point);
    ASSERT(entry_point->kind == LILY_MIR_INSTRUCTION_KIND_FUN);
    ASSERT(entry_point->fun.insts->len >= 1);

	current_block =
      &CAST(LilyMirInstruction *, get__Vec(entry_point->fun.insts, 0))
         ->block;
	current_inst = get__Vec(current_block->insts, 0);

	ASSERT(current_block);
	ASSERT(current_inst);

    return (LilyInterpreterVM){ .memory = memory,
                                .stack = stack,
                                .module = module,
                                .entry_point = entry_point };
}

void
run__LilyInterpreterVM(LilyInterpreterVM *self)
{
run_vm : {
	switch (current_inst->kind) {
	}
}
}
