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

// Stack-based VM

#ifdef ENV_DEBUG
#define LILY_FULL_ASSERT_VM
#endif

#if defined(CLANG_VERSION) || defined(GCC_VERSION)
#define LILY_USE_COMPUTED_GOTOS
#endif

#define VM_PUSH(stack, value) push__LilyInterpreterVMStack(stack, value)
#define VM_POP(stack) pop__LilyInterpreterVMStack(stack)

#ifdef LILY_USE_COMPUTED_GOTOS
#define VM_START(inst) \
    {                  \
        goto *inst_lookup[inst->kind];
#define VM_INST(name) label__##name:
#define VM_DEFAULT() \
    label__unknown:
#else
#define VM_START(inst) switch (inst->kind) {
#define VM_INST(name) case name:
#define VM_DEFAULT() default:
#endif

#define VM_NEXT() goto run_vm
#define VM_EXIT() goto exit_vm
#define VM_END() }
#define VM_NEXT_INST()                                                     \
    if ((current_block_inst = next__VecIter(&current_block_inst_iter))) {  \
        VM_NEXT();                                                         \
    } else {                                                               \
        {                                                                  \
            LilyMirInstruction *inst = next__VecIter(&current_block_iter); \
                                                                           \
            if (inst) {                                                    \
                current_block = &inst->block;                              \
            } else {                                                       \
                VM_EXIT();                                                 \
            }                                                              \
        }                                                                  \
                                                                           \
        current_block_inst_iter = NEW(VecIter, current_block->insts);      \
        current_block_inst = next__VecIter(&current_block_inst_iter);      \
                                                                           \
        ASSERT(current_block_inst);                                        \
                                                                           \
        VM_NEXT();                                                         \
    }

static void
run_inst__LilyInterpreterVM(LilyInterpreterVM *self);

static threadlocal VecIter current_block_iter;
static threadlocal LilyMirInstructionBlock *current_block = NULL;
static threadlocal VecIter current_block_inst_iter;
static threadlocal LilyMirInstruction *current_block_inst = NULL;
static threadlocal LilyInterpreterVMStack local_stack;
static threadlocal LilyInterpreterVMStackFrame *current_frame = NULL;

CONSTRUCTOR(LilyInterpreterVMStackFrame *,
            LilyInterpreterVMStackFrame,
            const char *name,
            LilyInterpreterValue *params,
            Usize params_len,
            Usize begin)
{
    LilyInterpreterVMStackFrame *self =
      lily_malloc(sizeof(LilyInterpreterVMStackFrame));

    self->name = name;
    self->params = params;
    self->params_len = params_len;
    self->begin = begin;
    self->end = 0;
    self->next = NULL;

    return self;
}

LilyInterpreterVMStackFrame *
attach_stack_frame__LilyInterpreterVMStackFrame(
  LilyInterpreterVMStackFrame *self,
  const char *name,
  LilyInterpreterValue *params,
  Usize params_len,
  Usize begin)
{
    self->end = begin;
    self->next =
      NEW(LilyInterpreterVMStackFrame, name, params, params_len, begin);

    return self->next;
}

DESTRUCTOR(LilyInterpreterVMStackFrame, LilyInterpreterVMStackFrame **self)
{
    // NOTE: Frame return mustn't be free at this point.
    ASSERT(!(*self)->next);

    for (Usize i = 0; i < (*self)->params_len; ++i) {
        FREE(LilyInterpreterValue, &(*self)->params[i]);
    }

    lily_free((*self)->params);
    lily_free(*self);

    *self = NULL;
}

void
push__LilyInterpreterVMStack(LilyInterpreterVMStack *self,
                             LilyInterpreterValue value)
{
    if (self->len > self->max_capacity) {
        RUNTIME_ERROR_STACK_OVERFLOW();
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
        RUNTIME_ERROR_STACK_EMPTY();
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

void
set_frame__LilyInterpreterVMStack(LilyInterpreterVMStack *self,
                                  LilyInterpreterVMStackFrame *frame)
{
    ASSERT(!self->current_frame);
    self->current_frame = frame;
}

LilyInterpreterVMStackFrameReturn
clean_frame__LilyInterpreterVMStack(LilyInterpreterVMStack *self)
{
    ASSERT(self->current_frame);

    LilyInterpreterVMStackFrameReturn return_ = self->current_frame->return_;

    FREE(LilyInterpreterVMStackFrame, &self->current_frame);
    self->current_frame = NULL;

    return return_;
}

DESTRUCTOR(LilyInterpreterVMStack, const LilyInterpreterVMStack *self)
{
    lily_free(self->buffer);
}

CONSTRUCTOR(LilyInterpreterVM,
            LilyInterpreterVM,
            Usize heap_capacity,
            Usize stack_capacity,
            const LilyMirModule *module,
            LilyInterpreterVMRessources ressources)
{
    LilyInterpreterMemory memory =
      heap_capacity
        ? NEW_VARIANT(LilyInterpreterMemory, capacity, heap_capacity)
        : NEW(LilyInterpreterMemory);
    LilyMirInstruction *entry_point =
      get__OrderedHashMap(module->insts, "main");

    ASSERT(entry_point);
    ASSERT(entry_point->kind == LILY_MIR_INSTRUCTION_KIND_FUN);
    ASSERT(entry_point->fun.insts->len >= 1);

    current_block_iter = NEW(VecIter, entry_point->fun.insts);

    {
        LilyMirInstruction *inst = next__VecIter(&current_block_iter);

        ASSERT(inst);

        current_block = &inst->block;
    }

    current_block_inst_iter = NEW(VecIter, current_block->insts);
    current_block_inst = next__VecIter(&current_block_inst_iter);
    local_stack = stack_capacity
                    ? NEW(LilyInterpreterVMStack, stack_capacity)
                    : NEW(LilyInterpreterVMStack, DEFAULT_MAX_STACK_CAPACITY);

    set_frame__LilyInterpreterVMStack(&local_stack,
                                      NEW(LilyInterpreterVMStackFrame,
                                          entry_point->fun.name,
                                          NULL,
                                          2,
                                          local_stack.len));

    current_frame = local_stack.current_frame;

    ASSERT(current_block);
    ASSERT(current_block_inst);

    return (LilyInterpreterVM){ .memory = memory,
                                .module = module,
                                .entry_point = entry_point,
                                .ressources = ressources };
}

LilyInterpreterValue *
load_const_value__LilyInterpreterVMStack(LilyInterpreterVMStack *self,
                                         const char *name)
{
    Usize begin = self->current_frame ? self->current_frame->begin : self->len;

    for (Usize i = 0; i < begin; ++i) {
        LilyInterpreterValue *current_value = &self->buffer[i];

        if (current_value->kind == LILY_INTERPRETER_VALUE_KIND_OBJECT &&
            current_value->object.kind ==
              LILY_INTERPRETER_VALUE_OBJECT_KIND_INSTANCE &&
            !strcmp(current_value->object.instance.name, name)) {
            LilyInterpreterValue *value = current_value->object.instance.value;

            return value;
        }
    }

    UNREACHABLE("impossible to load const value");
}

void
run_inst__LilyInterpreterVM(LilyInterpreterVM *self)
{
#ifdef LILY_USE_COMPUTED_GOTOS
    static void *inst_lookup[255] = {
        &&label__LILY_MIR_INSTRUCTION_KIND_ALLOC,
        &&label__LILY_MIR_INSTRUCTION_KIND_ARG,
        &&label__LILY_MIR_INSTRUCTION_KIND_ASM,
        &&label__LILY_MIR_INSTRUCTION_KIND_BITCAST,
        &&label__LILY_MIR_INSTRUCTION_KIND_BITAND,
        &&label__LILY_MIR_INSTRUCTION_KIND_BITNOT,
        &&label__LILY_MIR_INSTRUCTION_KIND_BITOR,
        &&label__LILY_MIR_INSTRUCTION_KIND_BLOCK,
        &&label__LILY_MIR_INSTRUCTION_KIND_BUILTIN_CALL,
        &&label__LILY_MIR_INSTRUCTION_KIND_CALL,
        &&label__LILY_MIR_INSTRUCTION_KIND_CONST,
        &&label__LILY_MIR_INSTRUCTION_KIND_DROP,
        &&label__LILY_MIR_INSTRUCTION_KIND_EXP,
        &&label__LILY_MIR_INSTRUCTION_KIND_FADD,
        &&label__LILY_MIR_INSTRUCTION_KIND_FCMP_EQ,
        &&label__LILY_MIR_INSTRUCTION_KIND_FCMP_NE,
        &&label__LILY_MIR_INSTRUCTION_KIND_FCMP_LE,
        &&label__LILY_MIR_INSTRUCTION_KIND_FCMP_LT,
        &&label__LILY_MIR_INSTRUCTION_KIND_FCMP_GE,
        &&label__LILY_MIR_INSTRUCTION_KIND_FCMP_GT,
        &&label__LILY_MIR_INSTRUCTION_KIND_FDIV,
        &&label__LILY_MIR_INSTRUCTION_KIND_FMUL,
        &&label__LILY_MIR_INSTRUCTION_KIND_FNEG,
        &&label__LILY_MIR_INSTRUCTION_KIND_FREM,
        &&label__LILY_MIR_INSTRUCTION_KIND_FSUB,
        &&label__LILY_MIR_INSTRUCTION_KIND_FUN,
        &&label__LILY_MIR_INSTRUCTION_KIND_FUN_PROTOTYPE,
        &&label__LILY_MIR_INSTRUCTION_KIND_GETARG,
        &&label__LILY_MIR_INSTRUCTION_KIND_GETARRAY,
        &&label__LILY_MIR_INSTRUCTION_KIND_GETLIST,
        &&label__LILY_MIR_INSTRUCTION_KIND_GETSLICE,
        &&label__LILY_MIR_INSTRUCTION_KIND_GETFIELD,
        &&label__LILY_MIR_INSTRUCTION_KIND_GETPTR,
        &&label__LILY_MIR_INSTRUCTION_KIND_IADD,
        &&label__LILY_MIR_INSTRUCTION_KIND_ICMP_EQ,
        &&label__LILY_MIR_INSTRUCTION_KIND_ICMP_NE,
        &&label__LILY_MIR_INSTRUCTION_KIND_ICMP_LE,
        &&label__LILY_MIR_INSTRUCTION_KIND_ICMP_LT,
        &&label__LILY_MIR_INSTRUCTION_KIND_ICMP_GE,
        &&label__LILY_MIR_INSTRUCTION_KIND_ICMP_GT,
        &&label__LILY_MIR_INSTRUCTION_KIND_IDIV,
        &&label__LILY_MIR_INSTRUCTION_KIND_IMUL,
        &&label__LILY_MIR_INSTRUCTION_KIND_INCTRACE,
        &&label__LILY_MIR_INSTRUCTION_KIND_INEG,
        &&label__LILY_MIR_INSTRUCTION_KIND_IREM,
        &&label__LILY_MIR_INSTRUCTION_KIND_ISOK,
        &&label__LILY_MIR_INSTRUCTION_KIND_ISERR,
        &&label__LILY_MIR_INSTRUCTION_KIND_ISUB,
        &&label__LILY_MIR_INSTRUCTION_KIND_JMP,
        &&label__LILY_MIR_INSTRUCTION_KIND_JMP_COND,
        &&label__LILY_MIR_INSTRUCTION_KIND_LEN,
        &&label__LILY_MIR_INSTRUCTION_KIND_LOAD,
        &&label__LILY_MIR_INSTRUCTION_KIND_MAKEREF,
        &&label__LILY_MIR_INSTRUCTION_KIND_MAKEOPT,
        &&label__LILY_MIR_INSTRUCTION_KIND_NON_NIL,
        &&label__LILY_MIR_INSTRUCTION_KIND_NOT,
        &&label__LILY_MIR_INSTRUCTION_KIND_REF_PTR,
        &&label__LILY_MIR_INSTRUCTION_KIND_REG,
        &&label__LILY_MIR_INSTRUCTION_KIND_RET,
        &&label__LILY_MIR_INSTRUCTION_KIND_SHL,
        &&label__LILY_MIR_INSTRUCTION_KIND_SHR,
        &&label__LILY_MIR_INSTRUCTION_KIND_STORE,
        &&label__LILY_MIR_INSTRUCTION_KIND_STRUCT,
        &&label__LILY_MIR_INSTRUCTION_KIND_SWITCH,
        &&label__LILY_MIR_INSTRUCTION_KIND_SYS_CALL,
        &&label__LILY_MIR_INSTRUCTION_KIND_TRUNC,
        &&label__LILY_MIR_INSTRUCTION_KIND_TRY,
        &&label__LILY_MIR_INSTRUCTION_KIND_TRY_PTR,
        &&label__LILY_MIR_INSTRUCTION_KIND_UNREACHABLE,
        &&label__LILY_MIR_INSTRUCTION_KIND_VAL,
        &&label__LILY_MIR_INSTRUCTION_KIND_VAR,
        &&label__LILY_MIR_INSTRUCTION_KIND_XOR,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
        &&label__unknown,
    };
#endif

#ifdef LILY_FULL_ASSERT_VM
#define SET_NEXT_LABEL(l) \
    ASSERT(!next_label);  \
    next_label = &&l
#else
#define SET_NEXT_LABEL(l) next_label = &&l
#endif

#define EAT_NEXT_LABEL()        \
    if (next_label) {           \
        void *tmp = next_label; \
        next_label = NULL;      \
        goto *tmp;              \
    }                           \
    return;

    void *next_label = NULL;
    register LilyInterpreterVMStack *stack = &local_stack;

    VM_START(current_block_inst);

    VM_INST(LILY_MIR_INSTRUCTION_KIND_ALLOC) {}

    VM_INST(LILY_MIR_INSTRUCTION_KIND_ARG) {}

    VM_INST(LILY_MIR_INSTRUCTION_KIND_ASM) {}

    VM_INST(LILY_MIR_INSTRUCTION_KIND_BITCAST) {}

    VM_INST(LILY_MIR_INSTRUCTION_KIND_BITAND) {}

    VM_INST(LILY_MIR_INSTRUCTION_KIND_BITNOT) {}

    VM_INST(LILY_MIR_INSTRUCTION_KIND_BITOR) {}

    VM_INST(LILY_MIR_INSTRUCTION_KIND_BLOCK) {}

    VM_INST(LILY_MIR_INSTRUCTION_KIND_BUILTIN_CALL) {}

    VM_INST(LILY_MIR_INSTRUCTION_KIND_CALL) {}

    VM_INST(LILY_MIR_INSTRUCTION_KIND_CONST) {}

    VM_INST(LILY_MIR_INSTRUCTION_KIND_DROP) {}

    VM_INST(LILY_MIR_INSTRUCTION_KIND_EXP) {}

    VM_INST(LILY_MIR_INSTRUCTION_KIND_FADD)
    {
        LilyInterpreterValue rhs = VM_POP(stack);
        LilyInterpreterValue lhs = VM_POP(stack);

#ifdef LILY_FULL_ASSERT_VM
        ASSERT(lhs.kind == LILY_INTERPRETER_VALUE_KIND_FLOAT);
        ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_FLOAT);
#endif

        VM_PUSH(
          stack,
          NEW_VARIANT(LilyInterpreterValue, float, lhs.float_ + rhs.float_));
        EAT_NEXT_LABEL();
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_FCMP_EQ)
    {
        LilyInterpreterValue rhs = VM_POP(stack);
        LilyInterpreterValue lhs = VM_POP(stack);

#ifdef LILY_FULL_ASSERT_VM
        ASSERT(lhs.kind == LILY_INTERPRETER_VALUE_KIND_FLOAT);
        ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_FLOAT);
#endif

        VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.float_ == rhs.float_));
        EAT_NEXT_LABEL();
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_FCMP_NE)
    {
        LilyInterpreterValue rhs = VM_POP(stack);
        LilyInterpreterValue lhs = VM_POP(stack);

#ifdef LILY_FULL_ASSERT_VM
        ASSERT(lhs.kind == LILY_INTERPRETER_VALUE_KIND_FLOAT);
        ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_FLOAT);
#endif

        VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.float_ != rhs.float_));
        EAT_NEXT_LABEL();
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_FCMP_LE)
    {
        LilyInterpreterValue rhs = VM_POP(stack);
        LilyInterpreterValue lhs = VM_POP(stack);

#ifdef LILY_FULL_ASSERT_VM
        ASSERT(lhs.kind == LILY_INTERPRETER_VALUE_KIND_FLOAT);
        ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_FLOAT);
#endif

        VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.float_ <= rhs.float_));
        EAT_NEXT_LABEL();
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_FCMP_LT)
    {
        LilyInterpreterValue rhs = VM_POP(stack);
        LilyInterpreterValue lhs = VM_POP(stack);

#ifdef LILY_FULL_ASSERT_VM
        ASSERT(lhs.kind == LILY_INTERPRETER_VALUE_KIND_FLOAT);
        ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_FLOAT);
#endif

        VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.float_ <= rhs.float_));
        EAT_NEXT_LABEL();
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_FCMP_GE)
    {
        LilyInterpreterValue rhs = VM_POP(stack);
        LilyInterpreterValue lhs = VM_POP(stack);

#ifdef LILY_FULL_ASSERT_VM
        ASSERT(lhs.kind == LILY_INTERPRETER_VALUE_KIND_FLOAT);
        ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_FLOAT);
#endif

        VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.float_ >= rhs.float_));
        EAT_NEXT_LABEL();
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_FCMP_GT)
    {
        LilyInterpreterValue rhs = VM_POP(stack);
        LilyInterpreterValue lhs = VM_POP(stack);

#ifdef LILY_FULL_ASSERT_VM
        ASSERT(lhs.kind == LILY_INTERPRETER_VALUE_KIND_FLOAT);
        ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_FLOAT);
#endif

        VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.float_ > rhs.float_));
        EAT_NEXT_LABEL();
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_FDIV)
    {
        LilyInterpreterValue rhs = VM_POP(stack);
        LilyInterpreterValue lhs = VM_POP(stack);

#ifdef LILY_FULL_ASSERT_VM
        ASSERT(lhs.kind == LILY_INTERPRETER_VALUE_KIND_FLOAT);
        ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_FLOAT);
#endif

        VM_PUSH(
          stack,
          NEW_VARIANT(LilyInterpreterValue, float, lhs.float_ / rhs.float_));
        EAT_NEXT_LABEL();
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_FMUL)
    {
        LilyInterpreterValue rhs = VM_POP(stack);
        LilyInterpreterValue lhs = VM_POP(stack);

#ifdef LILY_FULL_ASSERT_VM
        ASSERT(lhs.kind == LILY_INTERPRETER_VALUE_KIND_FLOAT);
        ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_FLOAT);
#endif

        VM_PUSH(
          stack,
          NEW_VARIANT(LilyInterpreterValue, float, lhs.float_ *rhs.float_));
        EAT_NEXT_LABEL();
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_FNEG)
    {
        LilyInterpreterValue rhs = VM_POP(stack);

#ifdef LILY_FULL_ASSERT_VM
        ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_FLOAT);
#endif

        VM_PUSH(stack, NEW_VARIANT(LilyInterpreterValue, float, -rhs.float_));
        EAT_NEXT_LABEL();
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_FREM)
    {
        LilyInterpreterValue rhs = VM_POP(stack);
        LilyInterpreterValue lhs = VM_POP(stack);

#ifdef LILY_FULL_ASSERT_VM
        ASSERT(lhs.kind == LILY_INTERPRETER_VALUE_KIND_FLOAT);
        ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_FLOAT);
#endif

        VM_PUSH(stack,
                NEW_VARIANT(LilyInterpreterValue,
                            float,
                            mod__Float64(lhs.float_, rhs.float_)));
        EAT_NEXT_LABEL();
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_FSUB)
    {
        LilyInterpreterValue rhs = VM_POP(stack);
        LilyInterpreterValue lhs = VM_POP(stack);

#ifdef LILY_FULL_ASSERT_VM
        ASSERT(lhs.kind == LILY_INTERPRETER_VALUE_KIND_FLOAT);
        ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_FLOAT);
#endif

        VM_PUSH(
          stack,
          NEW_VARIANT(LilyInterpreterValue, float, lhs.float_ - rhs.float_));
        EAT_NEXT_LABEL();
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_FUN) {}

    VM_INST(LILY_MIR_INSTRUCTION_KIND_FUN_PROTOTYPE) {}

    VM_INST(LILY_MIR_INSTRUCTION_KIND_GETARG) {}

    VM_INST(LILY_MIR_INSTRUCTION_KIND_GETARRAY) {}

    VM_INST(LILY_MIR_INSTRUCTION_KIND_GETLIST) {}

    VM_INST(LILY_MIR_INSTRUCTION_KIND_GETSLICE) {}

    VM_INST(LILY_MIR_INSTRUCTION_KIND_GETFIELD) {}

    VM_INST(LILY_MIR_INSTRUCTION_KIND_GETPTR) {}

    VM_INST(LILY_MIR_INSTRUCTION_KIND_IADD)
    {
        LilyInterpreterValue rhs = VM_POP(stack);
        LilyInterpreterValue lhs = VM_POP(stack);

        if (lhs.kind == LILY_INTERPRETER_VALUE_KIND_INT) {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue, uint, lhs.int_ + rhs.int_));
            EAT_NEXT_LABEL();
        } else if (lhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT) {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue, uint, lhs.uint + rhs.uint));
            EAT_NEXT_LABEL();
        }

        RUNTIME_ERROR_UNREACHABLE("expected Int... or Uint...");
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_ICMP_EQ)
    {
        LilyInterpreterValue rhs = VM_POP(stack);
        LilyInterpreterValue lhs = VM_POP(stack);

        if (lhs.kind == LILY_INTERPRETER_VALUE_KIND_INT) {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.int_ == rhs.int_));
            EAT_NEXT_LABEL();
        } else if (lhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT) {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.uint == rhs.uint));
            EAT_NEXT_LABEL();
        }

        RUNTIME_ERROR_UNREACHABLE("expected Int... or Uint...");
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_ICMP_NE)
    {
        LilyInterpreterValue rhs = VM_POP(stack);
        LilyInterpreterValue lhs = VM_POP(stack);

        if (lhs.kind == LILY_INTERPRETER_VALUE_KIND_INT) {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.int_ != rhs.int_));
            EAT_NEXT_LABEL();
        } else if (lhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT) {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.uint != rhs.uint));
            EAT_NEXT_LABEL();
        }

        RUNTIME_ERROR_UNREACHABLE("expected Int... or Uint...");
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_ICMP_LE)
    {
        LilyInterpreterValue rhs = VM_POP(stack);
        LilyInterpreterValue lhs = VM_POP(stack);

        if (lhs.kind == LILY_INTERPRETER_VALUE_KIND_INT) {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.int_ <= rhs.int_));
            EAT_NEXT_LABEL();
        } else if (lhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT) {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.uint <= rhs.uint));
            EAT_NEXT_LABEL();
        }

        RUNTIME_ERROR_UNREACHABLE("expected Int... or Uint...");
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_ICMP_LT)
    {
        LilyInterpreterValue rhs = VM_POP(stack);
        LilyInterpreterValue lhs = VM_POP(stack);

        if (lhs.kind == LILY_INTERPRETER_VALUE_KIND_INT) {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.int_ < rhs.int_));
            EAT_NEXT_LABEL();
        } else if (lhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT) {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.uint < rhs.uint));
            EAT_NEXT_LABEL();
        }

        RUNTIME_ERROR_UNREACHABLE("expected Int... or Uint...");
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_ICMP_GE)
    {
        LilyInterpreterValue rhs = VM_POP(stack);
        LilyInterpreterValue lhs = VM_POP(stack);

        if (lhs.kind == LILY_INTERPRETER_VALUE_KIND_INT) {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.int_ >= rhs.int_));
            EAT_NEXT_LABEL();
        } else if (lhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT) {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.uint >= rhs.uint));
            EAT_NEXT_LABEL();
        }

        RUNTIME_ERROR_UNREACHABLE("expected Int... or Uint...");
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_ICMP_GT)
    {
        LilyInterpreterValue rhs = VM_POP(stack);
        LilyInterpreterValue lhs = VM_POP(stack);

        if (lhs.kind == LILY_INTERPRETER_VALUE_KIND_INT) {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.int_ > rhs.int_));
            EAT_NEXT_LABEL();
        } else if (lhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT) {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.uint > rhs.uint));
            EAT_NEXT_LABEL();
        }

        RUNTIME_ERROR_UNREACHABLE("expected Int... or Uint...");
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_IDIV)
    {
        LilyInterpreterValue rhs = VM_POP(stack);
        LilyInterpreterValue lhs = VM_POP(stack);

        if (lhs.kind == LILY_INTERPRETER_VALUE_KIND_INT) {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue, int, lhs.int_ / rhs.int_));
            EAT_NEXT_LABEL();
        } else if (lhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT) {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue, uint, lhs.uint / rhs.uint));
            EAT_NEXT_LABEL();
        }

        RUNTIME_ERROR_UNREACHABLE("expected Int... or Uint...");
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_IMUL)
    {
        LilyInterpreterValue rhs = VM_POP(stack);
        LilyInterpreterValue lhs = VM_POP(stack);

        if (lhs.kind == LILY_INTERPRETER_VALUE_KIND_INT) {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(LilyInterpreterValue, int, lhs.int_ *rhs.int_));
            EAT_NEXT_LABEL();
        } else if (lhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT) {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue, uint, lhs.uint * rhs.uint));
            EAT_NEXT_LABEL();
        }

        RUNTIME_ERROR_UNREACHABLE("expected Int... or Uint...");
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_INCTRACE) {}

    VM_INST(LILY_MIR_INSTRUCTION_KIND_INEG)
    {
        LilyInterpreterValue rhs = VM_POP(stack);

#ifdef LILY_FULL_ASSERT_VM
        ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT);
#endif

        VM_PUSH(stack, NEW_VARIANT(LilyInterpreterValue, int, -rhs.int_));
        EAT_NEXT_LABEL();
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_IREM)
    {
        LilyInterpreterValue rhs = VM_POP(stack);
        LilyInterpreterValue lhs = VM_POP(stack);

        if (lhs.kind == LILY_INTERPRETER_VALUE_KIND_INT) {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue, int, lhs.int_ % rhs.int_));
            EAT_NEXT_LABEL();
        } else if (lhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT) {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue, uint, lhs.uint % rhs.uint));
            EAT_NEXT_LABEL();
        }

        RUNTIME_ERROR_UNREACHABLE("expected Int... or Uint...");
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_ISOK) {}

    VM_INST(LILY_MIR_INSTRUCTION_KIND_ISERR) {}

    VM_INST(LILY_MIR_INSTRUCTION_KIND_ISUB)
    {
        LilyInterpreterValue rhs = VM_POP(stack);
        LilyInterpreterValue lhs = VM_POP(stack);

        if (lhs.kind == LILY_INTERPRETER_VALUE_KIND_INT) {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue, int, lhs.int_ - rhs.int_));
            EAT_NEXT_LABEL();
        } else if (lhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT) {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue, uint, lhs.uint - rhs.uint));
            EAT_NEXT_LABEL();
        }

        RUNTIME_ERROR_UNREACHABLE("expected Int... or Uint...");
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_JMP) {}

    VM_INST(LILY_MIR_INSTRUCTION_KIND_JMP_COND) {}

    VM_INST(LILY_MIR_INSTRUCTION_KIND_LEN) {}

    VM_INST(LILY_MIR_INSTRUCTION_KIND_LOAD) {}

    VM_INST(LILY_MIR_INSTRUCTION_KIND_MAKEREF) {}

    VM_INST(LILY_MIR_INSTRUCTION_KIND_MAKEOPT) {}

    VM_INST(LILY_MIR_INSTRUCTION_KIND_NON_NIL) {}

    VM_INST(LILY_MIR_INSTRUCTION_KIND_NOT)
    {
        LilyInterpreterValue rhs = VM_POP(stack);

#ifdef LILY_FULL_ASSERT_VM
        ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_TRUE ||
               rhs.kind == LILY_INTERPRETER_VALUE_KIND_FALSE);
#endif

        VM_PUSH(stack, NEW(LilyInterpreterValue, !rhs.kind));
        EAT_NEXT_LABEL();
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_REF_PTR) {}

    VM_INST(LILY_MIR_INSTRUCTION_KIND_REG) {}

    VM_INST(LILY_MIR_INSTRUCTION_KIND_RET) {}

    VM_INST(LILY_MIR_INSTRUCTION_KIND_SHL)
    {
        LilyInterpreterValue rhs = VM_POP(stack);
        LilyInterpreterValue lhs = VM_POP(stack);

        if (lhs.kind == LILY_INTERPRETER_VALUE_KIND_INT) {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue, int, lhs.int_ << rhs.int_));
            EAT_NEXT_LABEL();
        } else if (lhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT) {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue, uint, lhs.uint << rhs.uint));
            EAT_NEXT_LABEL();
        }

        RUNTIME_ERROR_UNREACHABLE("expected Int... or Uint...");
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_SHR)
    {
        LilyInterpreterValue rhs = VM_POP(stack);
        LilyInterpreterValue lhs = VM_POP(stack);

        if (lhs.kind == LILY_INTERPRETER_VALUE_KIND_INT) {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue, int, lhs.int_ >> rhs.int_));
            EAT_NEXT_LABEL();
        } else if (lhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT) {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue, uint, lhs.uint >> rhs.uint));
            EAT_NEXT_LABEL();
        }

        RUNTIME_ERROR_UNREACHABLE("expected Int... or Uint...");
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_STORE) {}

    VM_INST(LILY_MIR_INSTRUCTION_KIND_STRUCT) {}

    VM_INST(LILY_MIR_INSTRUCTION_KIND_SWITCH) {}

    VM_INST(LILY_MIR_INSTRUCTION_KIND_SYS_CALL) {}

    VM_INST(LILY_MIR_INSTRUCTION_KIND_TRUNC) {}

    VM_INST(LILY_MIR_INSTRUCTION_KIND_TRY) {}

    VM_INST(LILY_MIR_INSTRUCTION_KIND_TRY_PTR) {}

    VM_INST(LILY_MIR_INSTRUCTION_KIND_UNREACHABLE) {}

    VM_INST(LILY_MIR_INSTRUCTION_KIND_VAL) {}

    VM_INST(LILY_MIR_INSTRUCTION_KIND_VAR) {}

    VM_INST(LILY_MIR_INSTRUCTION_KIND_XOR)
    {
        LilyInterpreterValue rhs = VM_POP(stack);
        LilyInterpreterValue lhs = VM_POP(stack);

        if (lhs.kind == LILY_INTERPRETER_VALUE_KIND_INT) {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue, int, lhs.int_ ^ rhs.int_));
            EAT_NEXT_LABEL();
        } else if (lhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT) {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue, uint, lhs.uint ^ rhs.uint));
            EAT_NEXT_LABEL();
        }

        RUNTIME_ERROR_UNREACHABLE("expected Int... or Uint...");
    }

    VM_DEFAULT()
    {
        UNREACHABLE("unknown instruction");
    }

    VM_END();
}

void
run__LilyInterpreterVM(LilyInterpreterVM *self)
{
run_vm : {
    run_inst__LilyInterpreterVM(self);
    VM_NEXT_INST();
}

exit_vm : {
    // Clean up

    FREE(LilyInterpreterVMStack, &local_stack);
}
}
