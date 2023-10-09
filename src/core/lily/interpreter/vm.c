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

#define VM_SET_CURRENT_BLOCK_ITER(iter) current_block_iter = iter;
#define VM_SET_CURRENT_BLOCK(block) current_block = block;
#define VM_SET_CURRENT_BLOCK_INST_ITER(iter) current_block_inst_iter = iter;
#define VM_SET_CURRENT_BLOCK_INST(new_inst) current_block_inst = new_inst;
#define VM_GOTO_INST(inst) goto *inst_lookup[inst->kind];
#define VM_NEXT(run) goto run
#define VM_EXIT(exit) goto exit
#define VM_END() }
#define VM_NEXT_INST(run, exit)                                           \
    if ((current_block_inst = next__VecIter(&current_block_inst_iter))) { \
        VM_NEXT(run);                                                     \
    } else {                                                              \
        {                                                                 \
            LilyMirInstruction *inst =                                    \
              next__OrderedHashMapIter(&current_block_iter);              \
                                                                          \
            if (inst) {                                                   \
                current_block = &inst->block;                             \
            } else {                                                      \
                VM_EXIT(exit);                                            \
            }                                                             \
        }                                                                 \
                                                                          \
        current_block_inst_iter = NEW(VecIter, current_block->insts);     \
        current_block_inst = next__VecIter(&current_block_inst_iter);     \
                                                                          \
        ASSERT(current_block_inst);                                       \
                                                                          \
        VM_NEXT(run);                                                     \
    }

#define CLEAN_BLOCK_STACK(is_ret)                                             \
    /* Clean stack of the last block. */                                      \
    if (current_block->limit->id ==                                           \
          current_frame->current_block_frame_limit_id ||                      \
        is_ret) {                                                             \
        clean_block_stack__LilyInterpreterVMStack(&local_stack);              \
        FREE(LilyInterpreterVMStackBlockFrame,                                \
             &current_frame                                                   \
                ->block_frames[current_frame->current_block_frame_limit_id]); \
    }

static inline void
set_return__LilyInterpreterVMStackFrame(
  LilyInterpreterVMStackFrame *self,
  LilyInterpreterVMStackFrameReturn return_);

static void
push_value__LilyInterpreterVM(LilyInterpreterVM *self,
                              const LilyMirInstructionVal *val);

static void
run_inst__LilyInterpreterVM(LilyInterpreterVM *self);

static void
run_insts__LilyInterpreterVM(LilyInterpreterVM *self);

static threadlocal OrderedHashMapIter current_block_iter;
static threadlocal LilyMirInstructionBlock *current_block = NULL;
static threadlocal VecIter current_block_inst_iter;
static threadlocal LilyMirInstruction *current_block_inst = NULL;
static threadlocal LilyInterpreterVMStack local_stack;
static threadlocal LilyInterpreterVMStackFrame *current_frame = NULL;

CONSTRUCTOR(LilyInterpreterVMStackBlockFrame *,
            LilyInterpreterVMStackBlockFrame,
            char *name,
            Usize begin,
            Usize end)
{
    LilyInterpreterVMStackBlockFrame *self =
      lily_malloc(sizeof(LilyInterpreterVMStackBlockFrame));

    self->names = init__Vec(1, name);
    self->variables = NEW(HashMap);
    self->regs = NEW(HashMap);
    self->begin = begin;
    self->end = end;

    return self;
}

LilyInterpreterValue *
search_reg__LilyInterpreterVMStackBlockFrame(
  const LilyInterpreterVMStackBlockFrame *self,
  char *name)
{
    LilyInterpreterValue *reg_value = get__HashMap(self->regs, name);
    LilyInterpreterValue *poped_value = VM_POP(&local_stack);

#ifdef LILY_FULL_ASSERT_VM
    ASSERT(reg_value);
    // Verify if the poped value on stack have the same address than reg_value
    ASSERT(reg_value == poped_value);
#endif

    return reg_value;
}

LilyInterpreterValue *
search_variable__LilyInterpreterVMStackBlockFrame(
  const LilyInterpreterVMStackBlockFrame *self,
  char *name)
{
    LilyInterpreterValue *var_value = get__HashMap(self->variables, name);

#ifdef LILY_FULL_ASSERT_VM
    ASSERT(var_value);
#endif

    return ref__LilyInterpreterValue(var_value);
}

DESTRUCTOR(LilyInterpreterVMStackBlockFrame,
           LilyInterpreterVMStackBlockFrame **self)
{
    FREE(Vec, (*self)->names);
    FREE(HashMap, (*self)->variables);
    FREE(HashMap, (*self)->regs);
    lily_free(*self);

    *self = NULL;
}

CONSTRUCTOR(LilyInterpreterVMStackFrame *,
            LilyInterpreterVMStackFrame,
            const char *name,
            LilyInterpreterValue **params,
            Usize params_len,
            Usize begin,
            Usize current_block_frame_limit_id,
            Usize block_frames_len)
{
    LilyInterpreterVMStackFrame *self =
      lily_malloc(sizeof(LilyInterpreterVMStackFrame));

    self->name = name;
    self->params = params;
    self->params_len = params_len;
    self->begin = begin;
    self->end = 0;
    self->current_block_frame_limit_id = current_block_frame_limit_id;
    self->block_frames = calloc(block_frames_len, PTR_SIZE);
    self->next = NULL;

    return self;
}

void
set_return__LilyInterpreterVMStackFrame(
  LilyInterpreterVMStackFrame *self,
  LilyInterpreterVMStackFrameReturn return_)
{
    self->return_ = return_;
}

void
add_block_frame__LilyInterpreterVMStackFrame(LilyInterpreterVMStackFrame *self,
                                             Usize current_block_frame_limit_id,
                                             char *name,
                                             Usize begin)
{
    LilyInterpreterVMStackBlockFrame **block_frame =
      &self->block_frames[current_block_frame_limit_id];

    // Set end value to the last current block frame.
    self->block_frames[self->current_block_frame_limit_id]->end = begin;

    if (!(*block_frame)) {
        // Set a new block frame.
        *block_frame = NEW(LilyInterpreterVMStackBlockFrame, name, begin, 0);
    } else {
        // Add name to the already created block frame.
        add_name__LilyInterpreterVMStackBlockFrame(*block_frame, name);
        (*block_frame)->end = 0;
    }

    self->current_block_frame_limit_id = current_block_frame_limit_id;
}

LilyInterpreterVMStackFrame *
attach_stack_frame__LilyInterpreterVMStackFrame(
  LilyInterpreterVMStackFrame *self,
  const char *name,
  LilyInterpreterValue **params,
  Usize params_len,
  Usize begin,
  Usize current_block_frame_limit_id,
  Usize block_frames_len)
{
    self->end = begin;
    self->next = NEW(LilyInterpreterVMStackFrame,
                     name,
                     params,
                     params_len,
                     begin,
                     current_block_frame_limit_id,
                     block_frames_len);

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

    for (Usize i = 0; i < (*self)->block_frames_len; ++i) {
        if ((*self)->block_frames[i]) {
            FREE(LilyInterpreterVMStackBlockFrame, &(*self)->block_frames[i]);
        }
    }

    lily_free((*self)->block_frames);
    lily_free(*self);

    *self = NULL;
}

void
push__LilyInterpreterVMStack(LilyInterpreterVMStack *self,
                             LilyInterpreterValue *value)
{
    if (self->len > self->max_capacity) {
        RUNTIME_ERROR_STACK_OVERFLOW();
    }

    if (self->len == self->capacity) {
        self->capacity *= DEFAULT_STACK_COEFF_INC;
        self->buffer = realloc(self->buffer, PTR_SIZE * self->capacity);
    }

    self->buffer[self->len] = value;
    self->top = self->buffer[self->len];
    ++self->len;
}

LilyInterpreterValue *
pop__LilyInterpreterVMStack(LilyInterpreterVMStack *self)
{
    if (self->len == 0) {
        RUNTIME_ERROR_STACK_EMPTY();
    }

    LilyInterpreterValue *top_value = self->buffer[--self->len];
    // Usize down_capacity = self->capacity / DEFAULT_STACK_COEFF_INC;

    // if (self->len < down_capacity) {
    // 	self->capacity = down_capacity;
    // 	self->buffer = realloc(self->buffer, sizeof(LilyInterpreterValue) *
    // self->capacity);
    // }

    if (self->len > 0) {
        self->top = self->buffer[self->len - 1];
    } else {
        self->top = NULL;
    }

    return top_value;
}

LilyInterpreterValue *
peek__LilyInterpreterVMStack(LilyInterpreterVMStack *self)
{
    if (self->len == 0) {
        RUNTIME_ERROR_STACK_EMPTY();
    }

    return self->buffer[self->len - 1];
}

void
set_frame__LilyInterpreterVMStack(LilyInterpreterVMStack *self,
                                  LilyInterpreterVMStackFrame *frame)
{
    ASSERT(!self->current_frame);
    self->current_frame = frame;
}

void
clean_block_stack__LilyInterpreterVMStack(LilyInterpreterVMStack *self)
{
    LilyInterpreterVMStackBlockFrame *current_block_frame =
      current_frame->block_frames[current_frame->current_block_frame_limit_id];

    // NOTE: No block frame in front of it is expected.
#ifdef LILY_FULL_ASSERT_VM
    ASSERT(current_block_frame->end == 0);
#endif

    while (self->len >= current_block_frame->begin - 1) {
        LilyInterpreterValue *value = VM_POP(self);

        FREE(LilyInterpreterValue, &value);
    }
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
            LilyInterpreterVMRessources ressources,
            bool check_overflow)
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

    current_block_iter = NEW(OrderedHashMapIter, entry_point->fun.insts);

    {
        LilyMirInstruction *inst =
          next__OrderedHashMapIter(&current_block_iter);

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
                                          local_stack.len,
                                          current_block->limit->id,
                                          entry_point->fun.insts->len));

    current_frame = local_stack.current_frame;

    ASSERT(current_block);
    ASSERT(current_block_inst);

    return (LilyInterpreterVM){ .memory = memory,
                                .module = module,
                                .entry_point = entry_point,
                                .ressources = ressources,
                                .check_overflow = check_overflow };
}

LilyInterpreterValue *
load_const_value__LilyInterpreterVMStack(LilyInterpreterVMStack *self,
                                         const char *name)
{
    Usize begin = self->current_frame ? self->current_frame->begin : self->len;

    for (Usize i = 0; i < begin; ++i) {
        LilyInterpreterValue *current_value = self->buffer[i];

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
push_value__LilyInterpreterVM(LilyInterpreterVM *self,
                              const LilyMirInstructionVal *val)
{
    switch (val->kind) {
        case LILY_MIR_INSTRUCTION_VAL_KIND_ARRAY:
            TODO("push array");
        case LILY_MIR_INSTRUCTION_VAL_KIND_BYTES:
            return VM_PUSH(
              &local_stack,
              NEW_VARIANT(LilyInterpreterValue,
                          object,
                          NEW_VARIANT(LilyInterpreterValueObject,
                                      bytes,
                                      NEW(LilyInterpreterValueBytes,
                                          (Uint8 *)val->bytes,
                                          val->dt->bytes.len))));
        case LILY_MIR_INSTRUCTION_VAL_KIND_CONST:
            TODO("push const");
        case LILY_MIR_INSTRUCTION_VAL_KIND_CSTR:
            return VM_PUSH(&local_stack,
                           NEW_VARIANT(LilyInterpreterValue,
                                       object,
                                       NEW_VARIANT(LilyInterpreterValueObject,
                                                   cstr,
                                                   (char *)val->cstr)));
        case LILY_MIR_INSTRUCTION_VAL_KIND_EXCEPTION:
            TODO("push exception");
        case LILY_MIR_INSTRUCTION_VAL_KIND_FLOAT:
            return VM_PUSH(
              &local_stack,
              NEW_VARIANT(LilyInterpreterValue, float, val->float_));
        case LILY_MIR_INSTRUCTION_VAL_KIND_INT:
            switch (val->dt->kind) {
                case LILY_MIR_DT_KIND_I1:
                    return VM_PUSH(&local_stack,
                                   NEW(LilyInterpreterValue, val->int_));
                case LILY_MIR_DT_KIND_I8:
                    return VM_PUSH(
                      &local_stack,
                      NEW_VARIANT(LilyInterpreterValue, int8, (Int8)val->int_));
                case LILY_MIR_DT_KIND_I16:
                    return VM_PUSH(&local_stack,
                                   NEW_VARIANT(LilyInterpreterValue,
                                               int16,
                                               (Int16)val->int_));
                case LILY_MIR_DT_KIND_I32:
                    return VM_PUSH(&local_stack,
                                   NEW_VARIANT(LilyInterpreterValue,
                                               int32,
                                               (Int32)val->int_));
                case LILY_MIR_DT_KIND_I64:
                    return VM_PUSH(
                      &local_stack,
                      NEW_VARIANT(LilyInterpreterValue, int64, val->int_));
                case LILY_MIR_DT_KIND_ISIZE:
                    return VM_PUSH(&local_stack,
                                   NEW_VARIANT(LilyInterpreterValue,
                                               isize,
                                               (Isize)val->int_));
                default:
                    UNREACHABLE("this situation is impossible");
            }
        case LILY_MIR_INSTRUCTION_VAL_KIND_LIST:
            TODO("push list");
        case LILY_MIR_INSTRUCTION_VAL_KIND_NIL:
            TODO("push nil");
        case LILY_MIR_INSTRUCTION_VAL_KIND_PARAM:
            TODO("push param");
        case LILY_MIR_INSTRUCTION_VAL_KIND_REG:
            TODO("push reg");
        case LILY_MIR_INSTRUCTION_VAL_KIND_SLICE:
            TODO("push slice");
        case LILY_MIR_INSTRUCTION_VAL_KIND_STR:
            TODO("push str");
        case LILY_MIR_INSTRUCTION_VAL_KIND_STRUCT:
            TODO("push struct");
        case LILY_MIR_INSTRUCTION_VAL_KIND_TRACE:
            TODO("push trace");
        case LILY_MIR_INSTRUCTION_VAL_KIND_TUPLE:
            TODO("push tuple");
        case LILY_MIR_INSTRUCTION_VAL_KIND_UINT:
            switch (val->dt->kind) {
                case LILY_MIR_DT_KIND_U8:
                    return VM_PUSH(&local_stack,
                                   NEW_VARIANT(LilyInterpreterValue,
                                               uint8,
                                               (Uint8)val->uint));
                case LILY_MIR_DT_KIND_U16:
                    return VM_PUSH(&local_stack,
                                   NEW_VARIANT(LilyInterpreterValue,
                                               uint16,
                                               (Uint16)val->uint));
                case LILY_MIR_DT_KIND_U32:
                    return VM_PUSH(&local_stack,
                                   NEW_VARIANT(LilyInterpreterValue,
                                               uint32,
                                               (Uint32)val->uint));
                case LILY_MIR_DT_KIND_U64:
                    return VM_PUSH(&local_stack,
                                   NEW_VARIANT(LilyInterpreterValue,
                                               uint64,
                                               (Uint64)val->uint));
                case LILY_MIR_DT_KIND_USIZE:
                    return VM_PUSH(&local_stack,
                                   NEW_VARIANT(LilyInterpreterValue,
                                               usize,
                                               (Usize)val->uint));
                default:
                    UNREACHABLE("this situation is impossible");
            }
        case LILY_MIR_INSTRUCTION_VAL_KIND_UNDEF:
            VM_PUSH(
              &local_stack,
              NEW(LilyInterpreterValue, LILY_INTERPRETER_VALUE_KIND_UNDEF));
            return;
        case LILY_MIR_INSTRUCTION_VAL_KIND_UNIT:
            VM_PUSH(
              &local_stack,
              NEW(LilyInterpreterValue, LILY_INTERPRETER_VALUE_KIND_UNIT));
            return;
        case LILY_MIR_INSTRUCTION_VAL_KIND_VAR:
            TODO("push var");
        default:
            UNREACHABLE("unknown variant");
    }
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

#ifdef LILY_USE_COMPUTED_GOTOS
#define INT_INST_LOOKUP(inst)                                            \
    static void *inst##_lookup[17] = {                                   \
        [LILY_INTERPRETER_VALUE_KIND_TRUE] = &&label__unknown_i##inst,   \
        [LILY_INTERPRETER_VALUE_KIND_FALSE] = &&label__unknown_i##inst,  \
        [LILY_INTERPRETER_VALUE_KIND_INT8] =                             \
          &&label__LILY_INTERPRETER_VALUE_KIND_INT8_i##inst,             \
        [LILY_INTERPRETER_VALUE_KIND_INT16] =                            \
          &&label__LILY_INTERPRETER_VALUE_KIND_INT16_i##inst,            \
        [LILY_INTERPRETER_VALUE_KIND_INT32] =                            \
          &&label__LILY_INTERPRETER_VALUE_KIND_INT32_i##inst,            \
        [LILY_INTERPRETER_VALUE_KIND_INT64] =                            \
          &&label__LILY_INTERPRETER_VALUE_KIND_INT64_i##inst,            \
        [LILY_INTERPRETER_VALUE_KIND_ISIZE] =                            \
          &&label__LILY_INTERPRETER_VALUE_KIND_ISIZE_i##inst,            \
        [LILY_INTERPRETER_VALUE_KIND_FLOAT] = &&label__unknown_i##inst,  \
        [LILY_INTERPRETER_VALUE_KIND_NIL] = &&label__unknown_i##inst,    \
        [LILY_INTERPRETER_VALUE_KIND_OBJECT] = &&label__unknown_i##inst, \
        [LILY_INTERPRETER_VALUE_KIND_UNDEF] = &&label__unknown_i##inst,  \
        [LILY_INTERPRETER_VALUE_KIND_UINT8] =                            \
          &&label__LILY_INTERPRETER_VALUE_KIND_UINT8_i##inst,            \
        [LILY_INTERPRETER_VALUE_KIND_UINT16] =                           \
          &&label__LILY_INTERPRETER_VALUE_KIND_UINT16_i##inst,           \
        [LILY_INTERPRETER_VALUE_KIND_UINT32] =                           \
          &&label__LILY_INTERPRETER_VALUE_KIND_UINT32_i##inst,           \
        [LILY_INTERPRETER_VALUE_KIND_UINT64] =                           \
          &&label__LILY_INTERPRETER_VALUE_KIND_UINT64_i##inst,           \
        [LILY_INTERPRETER_VALUE_KIND_USIZE] =                            \
          &&label__LILY_INTERPRETER_VALUE_KIND_USIZE_i##inst,            \
        [LILY_INTERPRETER_VALUE_KIND_UNIT] = &&label__unknown_i##inst,   \
    };

#define INT_INST_START(inst, inst_kind) \
    {                                   \
        goto *inst##_lookup[inst_kind]
#define INT_INST_TYPE(value_kind, inst) label__##value_kind##_i##inst:
#define INT_INST_DEFAULT(inst) label__unknown_i##inst:
#define INT_INST_END() }
#else
#define INT_INST_LOOKUP(inst)
#define INT_INST_START(inst_kind) switch (inst_kind) {
#define INT_INST_TYPE(value_kind) case value_kind:
#define INT_INST_DEFAULT() default:
#define INT_INST_END() }
#endif

#define FREE_BVAL()                   \
    FREE(LilyInterpreterValue, &lhs); \
    FREE(LilyInterpreterValue, &rhs);

#define FREE_UVAL() FREE(LilyInterpreterValue, &rhs);

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
        LilyInterpreterValue *rhs = VM_POP(stack);
        LilyInterpreterValue *lhs = VM_POP(stack);

#ifdef LILY_FULL_ASSERT_VM
        ASSERT(lhs->kind == LILY_INTERPRETER_VALUE_KIND_FLOAT);
        ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_FLOAT);
#endif

        VM_PUSH(
          stack,
          NEW_VARIANT(LilyInterpreterValue, float, lhs->float_ + rhs->float_));

        FREE_BVAL();
        EAT_NEXT_LABEL();
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_FCMP_EQ)
    {
        LilyInterpreterValue *rhs = VM_POP(stack);
        LilyInterpreterValue *lhs = VM_POP(stack);

#ifdef LILY_FULL_ASSERT_VM
        ASSERT(lhs->kind == LILY_INTERPRETER_VALUE_KIND_FLOAT);
        ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_FLOAT);
#endif

        VM_PUSH(stack, NEW(LilyInterpreterValue, lhs->float_ == rhs->float_));

        FREE_BVAL();
        EAT_NEXT_LABEL();
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_FCMP_NE)
    {
        LilyInterpreterValue *rhs = VM_POP(stack);
        LilyInterpreterValue *lhs = VM_POP(stack);

#ifdef LILY_FULL_ASSERT_VM
        ASSERT(lhs->kind == LILY_INTERPRETER_VALUE_KIND_FLOAT);
        ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_FLOAT);
#endif

        VM_PUSH(stack, NEW(LilyInterpreterValue, lhs->float_ != rhs->float_));

        FREE_BVAL();
        EAT_NEXT_LABEL();
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_FCMP_LE)
    {
        LilyInterpreterValue *rhs = VM_POP(stack);
        LilyInterpreterValue *lhs = VM_POP(stack);

#ifdef LILY_FULL_ASSERT_VM
        ASSERT(lhs->kind == LILY_INTERPRETER_VALUE_KIND_FLOAT);
        ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_FLOAT);
#endif

        VM_PUSH(stack, NEW(LilyInterpreterValue, lhs->float_ <= rhs->float_));

        FREE_BVAL();
        EAT_NEXT_LABEL();
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_FCMP_LT)
    {
        LilyInterpreterValue *rhs = VM_POP(stack);
        LilyInterpreterValue *lhs = VM_POP(stack);

#ifdef LILY_FULL_ASSERT_VM
        ASSERT(lhs->kind == LILY_INTERPRETER_VALUE_KIND_FLOAT);
        ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_FLOAT);
#endif

        VM_PUSH(stack, NEW(LilyInterpreterValue, lhs->float_ <= rhs->float_));

        FREE(LilyInterpreterValue, &lhs);
        FREE(LilyInterpreterValue, &rhs);

        FREE_BVAL();
        EAT_NEXT_LABEL();
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_FCMP_GE)
    {
        LilyInterpreterValue *rhs = VM_POP(stack);
        LilyInterpreterValue *lhs = VM_POP(stack);

#ifdef LILY_FULL_ASSERT_VM
        ASSERT(lhs->kind == LILY_INTERPRETER_VALUE_KIND_FLOAT);
        ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_FLOAT);
#endif

        VM_PUSH(stack, NEW(LilyInterpreterValue, lhs->float_ >= rhs->float_));

        FREE(LilyInterpreterValue, &lhs);
        FREE(LilyInterpreterValue, &rhs);

        FREE_BVAL();
        EAT_NEXT_LABEL();
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_FCMP_GT)
    {
        LilyInterpreterValue *rhs = VM_POP(stack);
        LilyInterpreterValue *lhs = VM_POP(stack);

#ifdef LILY_FULL_ASSERT_VM
        ASSERT(lhs->kind == LILY_INTERPRETER_VALUE_KIND_FLOAT);
        ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_FLOAT);
#endif

        VM_PUSH(stack, NEW(LilyInterpreterValue, lhs->float_ > rhs->float_));

        FREE_BVAL();
        EAT_NEXT_LABEL();
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_FDIV)
    {
        LilyInterpreterValue *rhs = VM_POP(stack);
        LilyInterpreterValue *lhs = VM_POP(stack);

#ifdef LILY_FULL_ASSERT_VM
        ASSERT(lhs->kind == LILY_INTERPRETER_VALUE_KIND_FLOAT);
        ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_FLOAT);
#endif

        VM_PUSH(
          stack,
          NEW_VARIANT(LilyInterpreterValue, float, lhs->float_ / rhs->float_));

        FREE_BVAL();
        EAT_NEXT_LABEL();
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_FMUL)
    {
        LilyInterpreterValue *rhs = VM_POP(stack);
        LilyInterpreterValue *lhs = VM_POP(stack);

#ifdef LILY_FULL_ASSERT_VM
        ASSERT(lhs->kind == LILY_INTERPRETER_VALUE_KIND_FLOAT);
        ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_FLOAT);
#endif

        VM_PUSH(
          stack,
          NEW_VARIANT(LilyInterpreterValue, float, lhs->float_ * rhs->float_));

        FREE_BVAL();
        EAT_NEXT_LABEL();
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_FNEG)
    {
        LilyInterpreterValue *rhs = VM_POP(stack);

#ifdef LILY_FULL_ASSERT_VM
        ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_FLOAT);
#endif

        VM_PUSH(stack, NEW_VARIANT(LilyInterpreterValue, float, -rhs->float_));

        FREE_UVAL();
        EAT_NEXT_LABEL();
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_FREM)
    {
        LilyInterpreterValue *rhs = VM_POP(stack);
        LilyInterpreterValue *lhs = VM_POP(stack);

#ifdef LILY_FULL_ASSERT_VM
        ASSERT(lhs->kind == LILY_INTERPRETER_VALUE_KIND_FLOAT);
        ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_FLOAT);
#endif

        VM_PUSH(stack,
                NEW_VARIANT(LilyInterpreterValue,
                            float,
                            mod__Float64(lhs->float_, rhs->float_)));

        FREE_BVAL();
        EAT_NEXT_LABEL();
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_FSUB)
    {
        LilyInterpreterValue *rhs = VM_POP(stack);
        LilyInterpreterValue *lhs = VM_POP(stack);

#ifdef LILY_FULL_ASSERT_VM
        ASSERT(lhs->kind == LILY_INTERPRETER_VALUE_KIND_FLOAT);
        ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_FLOAT);
#endif

        VM_PUSH(
          stack,
          NEW_VARIANT(LilyInterpreterValue, float, lhs->float_ - rhs->float_));

        FREE_BVAL();
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
        INT_INST_LOOKUP(add);

        LilyInterpreterValue *rhs = VM_POP(stack);
        LilyInterpreterValue *lhs = VM_POP(stack);

        INT_INST_START(add, lhs->kind);

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT8, add)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_INT8);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(LilyInterpreterValue,
                                int8,
                                self->check_overflow ? add_with_overflow__Int8(
                                                         lhs->int8, rhs->int8)
                                                     : lhs->int8 + rhs->int8));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT16, add)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_INT16);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue,
                          int16,
                          self->check_overflow
                            ? add_with_overflow__Int16(lhs->int16, rhs->int16)
                            : lhs->int16 + rhs->int16));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT32, add)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_INT32);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue,
                          int32,
                          self->check_overflow
                            ? add_with_overflow__Int32(lhs->int32, rhs->int32)
                            : lhs->int32 + rhs->int32));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT64, add)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_INT64);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue,
                          int64,
                          self->check_overflow
                            ? add_with_overflow__Int64(lhs->int64, rhs->int64)
                            : lhs->int64 + rhs->int64));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_ISIZE, add)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_ISIZE);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue,
                          isize,
                          self->check_overflow
                            ? add_with_overflow__Isize(lhs->isize, rhs->isize)
                            : lhs->isize + rhs->isize));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT8, add)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_UINT8);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue,
                          uint8,
                          self->check_overflow
                            ? add_with_overflow__Uint8(lhs->uint8, rhs->uint8)
                            : lhs->uint8 + rhs->uint8));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT16, add)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_UINT16);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(
                      LilyInterpreterValue,
                      uint16,
                      self->check_overflow
                        ? add_with_overflow__Uint16(lhs->uint16, rhs->uint16)
                        : lhs->uint16 + rhs->uint16));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT32, add)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_UINT32);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(
                      LilyInterpreterValue,
                      uint32,
                      self->check_overflow
                        ? add_with_overflow__Uint32(lhs->uint32, rhs->uint32)
                        : lhs->uint32 + rhs->uint32));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT64, add)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_UINT64);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(
                      LilyInterpreterValue,
                      uint64,
                      self->check_overflow
                        ? add_with_overflow__Uint64(lhs->uint64, rhs->uint64)
                        : lhs->uint64 + rhs->uint64));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_USIZE, add)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_USIZE);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue,
                          usize,
                          self->check_overflow
                            ? add_with_overflow__Usize(lhs->usize, rhs->usize)
                            : lhs->usize + rhs->usize));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_DEFAULT(add)
        {
            RUNTIME_ERROR_UNREACHABLE("expected Int... or Uint...");
        }

        INT_INST_END();
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_ICMP_EQ)
    {
        INT_INST_LOOKUP(cmp_eq);

        LilyInterpreterValue *rhs = VM_POP(stack);
        LilyInterpreterValue *lhs = VM_POP(stack);

        INT_INST_START(cmp_eq, lhs->kind);

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT8, cmp_eq)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_INT8);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs->int8 == rhs->int8));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT16, cmp_eq)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_INT16);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs->int16 == rhs->int16));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT32, cmp_eq)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_INT32);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs->int32 == rhs->int32));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT64, cmp_eq)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_INT64);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs->int64 == rhs->int64));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_ISIZE, cmp_eq)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_ISIZE);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs->isize == rhs->isize));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT8, cmp_eq)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_UINT8);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs->uint8 == rhs->uint8));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT16, cmp_eq)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_UINT16);
#endif

            VM_PUSH(stack,
                    NEW(LilyInterpreterValue, lhs->uint16 == rhs->uint16));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT32, cmp_eq)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_UINT32);
#endif

            VM_PUSH(stack,
                    NEW(LilyInterpreterValue, lhs->uint32 == rhs->uint32));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT64, cmp_eq)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_UINT64);
#endif

            VM_PUSH(stack,
                    NEW(LilyInterpreterValue, lhs->uint64 == rhs->uint64));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_USIZE, cmp_eq)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_USIZE);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs->usize == rhs->usize));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_DEFAULT(cmp_eq)
        {
            RUNTIME_ERROR_UNREACHABLE("expected Int... or Uint...");
        }

        INT_INST_END();
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_ICMP_NE)
    {
        INT_INST_LOOKUP(cmp_ne);

        LilyInterpreterValue *rhs = VM_POP(stack);
        LilyInterpreterValue *lhs = VM_POP(stack);

        INT_INST_START(cmp_ne, lhs->kind);

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT8, cmp_ne)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_INT8);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs->int8 != rhs->int8));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT16, cmp_ne)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_INT16);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs->int16 != rhs->int16));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT32, cmp_ne)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_INT32);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs->int32 != rhs->int32));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT64, cmp_ne)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_INT64);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs->int64 != rhs->int64));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_ISIZE, cmp_ne)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_ISIZE);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs->isize != rhs->isize));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT8, cmp_ne)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_UINT8);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs->uint8 != rhs->uint8));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT16, cmp_ne)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_UINT16);
#endif

            VM_PUSH(stack,
                    NEW(LilyInterpreterValue, lhs->uint16 != rhs->uint16));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT32, cmp_ne)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_UINT32);
#endif

            VM_PUSH(stack,
                    NEW(LilyInterpreterValue, lhs->uint32 != rhs->uint32));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT64, cmp_ne)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_UINT64);
#endif

            VM_PUSH(stack,
                    NEW(LilyInterpreterValue, lhs->uint64 != rhs->uint64));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_USIZE, cmp_ne)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_USIZE);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs->usize != rhs->usize));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_DEFAULT(cmp_ne)
        {
            RUNTIME_ERROR_UNREACHABLE("expected Int... or Uint...");
        }

        INT_INST_END();
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_ICMP_LE)
    {
        INT_INST_LOOKUP(cmp_le);

        LilyInterpreterValue *rhs = VM_POP(stack);
        LilyInterpreterValue *lhs = VM_POP(stack);

        INT_INST_START(cmp_le, lhs->kind);

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT8, cmp_le)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_INT8);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs->int8 <= rhs->int8));
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT16, cmp_le)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_INT16);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs->int16 <= rhs->int16));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT32, cmp_le)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_INT32);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs->int32 <= rhs->int32));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT64, cmp_le)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_INT64);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs->int64 <= rhs->int64));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_ISIZE, cmp_le)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_ISIZE);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs->isize <= rhs->isize));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT8, cmp_le)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_UINT8);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs->uint8 <= rhs->uint8));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT16, cmp_le)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_UINT16);
#endif

            VM_PUSH(stack,
                    NEW(LilyInterpreterValue, lhs->uint16 <= rhs->uint16));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT32, cmp_le)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_UINT32);
#endif

            VM_PUSH(stack,
                    NEW(LilyInterpreterValue, lhs->uint32 <= rhs->uint32));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT64, cmp_le)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_UINT64);
#endif

            VM_PUSH(stack,
                    NEW(LilyInterpreterValue, lhs->uint64 <= rhs->uint64));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_USIZE, cmp_le)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_USIZE);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs->usize <= rhs->usize));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_DEFAULT(cmp_le)
        {
            RUNTIME_ERROR_UNREACHABLE("expected Int... or Uint...");
        }

        INT_INST_END();
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_ICMP_LT)
    {
        INT_INST_LOOKUP(cmp_lt);

        LilyInterpreterValue *rhs = VM_POP(stack);
        LilyInterpreterValue *lhs = VM_POP(stack);

        INT_INST_START(cmp_lt, lhs->kind);

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT8, cmp_lt)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_INT8);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs->int8 < rhs->int8));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT16, cmp_lt)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_INT16);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs->int16 < rhs->int16));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT32, cmp_lt)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_INT32);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs->int32 < rhs->int32));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT64, cmp_lt)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_INT64);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs->int64 < rhs->int64));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_ISIZE, cmp_lt)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_ISIZE);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs->isize < rhs->isize));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT8, cmp_lt)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_UINT8);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs->uint8 < rhs->uint8));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT16, cmp_lt)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_UINT16);
#endif

            VM_PUSH(stack,
                    NEW(LilyInterpreterValue, lhs->uint16 < rhs->uint16));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT32, cmp_lt)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_UINT32);
#endif

            VM_PUSH(stack,
                    NEW(LilyInterpreterValue, lhs->uint32 < rhs->uint32));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT64, cmp_lt)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_UINT64);
#endif

            VM_PUSH(stack,
                    NEW(LilyInterpreterValue, lhs->uint64 < rhs->uint64));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_USIZE, cmp_lt)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_USIZE);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs->usize < rhs->usize));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_DEFAULT(cmp_lt)
        {
            RUNTIME_ERROR_UNREACHABLE("expected Int... or Uint...");
        }

        INT_INST_END();
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_ICMP_GE)
    {
        INT_INST_LOOKUP(cmp_ge);

        LilyInterpreterValue *rhs = VM_POP(stack);
        LilyInterpreterValue *lhs = VM_POP(stack);

        INT_INST_START(cmp_ge, lhs->kind);

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT8, cmp_ge)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_INT8);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs->int8 >= rhs->int8));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT16, cmp_ge)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_INT16);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs->int16 >= rhs->int16));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT32, cmp_ge)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_INT32);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs->int32 >= rhs->int32));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT64, cmp_ge)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_INT64);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs->int64 >= rhs->int64));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_ISIZE, cmp_ge)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_ISIZE);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs->isize >= rhs->isize));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT8, cmp_ge)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_UINT8);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs->uint8 >= rhs->uint8));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT16, cmp_ge)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_UINT16);
#endif

            VM_PUSH(stack,
                    NEW(LilyInterpreterValue, lhs->uint16 >= rhs->uint16));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT32, cmp_ge)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_UINT32);
#endif

            VM_PUSH(stack,
                    NEW(LilyInterpreterValue, lhs->uint32 >= rhs->uint32));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT64, cmp_ge)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_UINT64);
#endif

            VM_PUSH(stack,
                    NEW(LilyInterpreterValue, lhs->uint64 >= rhs->uint64));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_USIZE, cmp_ge)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_USIZE);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs->usize >= rhs->usize));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_DEFAULT(cmp_ge)
        {
            RUNTIME_ERROR_UNREACHABLE("expected Int... or Uint...");
        }

        INT_INST_END();
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_ICMP_GT)
    {
        INT_INST_LOOKUP(cmp_gt);

        LilyInterpreterValue *rhs = VM_POP(stack);
        LilyInterpreterValue *lhs = VM_POP(stack);

        INT_INST_START(cmp_gt, lhs->kind);

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT8, cmp_gt)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_INT8);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs->int8 > rhs->int8));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT16, cmp_gt)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_INT16);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs->int16 > rhs->int16));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT32, cmp_gt)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_INT32);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs->int32 > rhs->int32));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT64, cmp_gt)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_INT64);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs->int64 > rhs->int64));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_ISIZE, cmp_gt)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_ISIZE);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs->isize > rhs->isize));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT8, cmp_gt)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_UINT8);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs->uint8 > rhs->uint8));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT16, cmp_gt)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_UINT16);
#endif

            VM_PUSH(stack,
                    NEW(LilyInterpreterValue, lhs->uint16 > rhs->uint16));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT32, cmp_gt)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_UINT32);
#endif

            VM_PUSH(stack,
                    NEW(LilyInterpreterValue, lhs->uint32 > rhs->uint32));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT64, cmp_gt)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_UINT64);
#endif

            VM_PUSH(stack,
                    NEW(LilyInterpreterValue, lhs->uint64 > rhs->uint64));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_USIZE, cmp_gt)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_USIZE);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs->usize > rhs->usize));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_DEFAULT(cmp_gt)
        {
            RUNTIME_ERROR_UNREACHABLE("expected Int... or Uint...");
        }

        INT_INST_END();
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_IDIV)
    {
        INT_INST_LOOKUP(div);

        LilyInterpreterValue *rhs = VM_POP(stack);
        LilyInterpreterValue *lhs = VM_POP(stack);

        INT_INST_START(div, lhs->kind);

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT8, div)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_INT8);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(LilyInterpreterValue,
                                int8,
                                self->check_overflow ? div_with_overflow__Int8(
                                                         lhs->int8, rhs->int8)
                                                     : lhs->int8 / rhs->int8));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT16, div)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_INT16);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue,
                          int16,
                          self->check_overflow
                            ? div_with_overflow__Int16(lhs->int16, rhs->int16)
                            : lhs->int16 / rhs->int16));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT32, div)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_INT32);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue,
                          int32,
                          self->check_overflow
                            ? div_with_overflow__Int32(lhs->int32, rhs->int32)
                            : lhs->int32 / rhs->int32));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT64, div)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_INT64);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue,
                          int64,
                          self->check_overflow
                            ? div_with_overflow__Int64(lhs->int64, rhs->int64)
                            : lhs->int64 / rhs->int64));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_ISIZE, div)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_ISIZE);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue,
                          isize,
                          self->check_overflow
                            ? div_with_overflow__Isize(lhs->isize, rhs->isize)
                            : lhs->isize / rhs->isize));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT8, div)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_UINT8);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue,
                          uint8,
                          self->check_overflow
                            ? div_with_overflow__Uint8(lhs->uint8, rhs->uint8)
                            : lhs->uint8 / rhs->uint8));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT16, div)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_UINT16);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(
                      LilyInterpreterValue,
                      uint16,
                      self->check_overflow
                        ? div_with_overflow__Uint16(lhs->uint16, rhs->uint16)
                        : lhs->uint16 / rhs->uint16));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT32, div)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_UINT32);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(
                      LilyInterpreterValue,
                      uint32,
                      self->check_overflow
                        ? div_with_overflow__Uint32(lhs->uint32, rhs->uint32)
                        : lhs->uint32 / rhs->uint32));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT64, div)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_UINT64);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(
                      LilyInterpreterValue,
                      uint64,
                      self->check_overflow
                        ? div_with_overflow__Uint64(lhs->uint64, rhs->uint64)
                        : lhs->uint64 / rhs->uint64));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_USIZE, div)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_USIZE);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue,
                          usize,
                          self->check_overflow
                            ? div_with_overflow__Usize(lhs->usize, rhs->usize)
                            : lhs->usize / rhs->usize));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_DEFAULT(div)
        {
            RUNTIME_ERROR_UNREACHABLE("expected Int... or Uint...");
        }

        INT_INST_END();
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_IMUL)
    {
        INT_INST_LOOKUP(mul);

        LilyInterpreterValue *rhs = VM_POP(stack);
        LilyInterpreterValue *lhs = VM_POP(stack);

        INT_INST_START(mul, lhs->kind);

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT8, mul)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_INT8);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(LilyInterpreterValue,
                                int8,
                                self->check_overflow ? mul_with_overflow__Int8(
                                                         lhs->int8, rhs->int8)
                                                     : lhs->int8 * rhs->int8));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT16, mul)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_INT16);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue,
                          int16,
                          self->check_overflow
                            ? mul_with_overflow__Int16(lhs->int16, rhs->int16)
                            : lhs->int16 * rhs->int16));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT32, mul)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_INT32);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue,
                          int32,
                          self->check_overflow
                            ? mul_with_overflow__Int32(lhs->int32, rhs->int32)
                            : lhs->int32 * rhs->int32));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT64, mul)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_INT64);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue,
                          int64,
                          self->check_overflow
                            ? mul_with_overflow__Int64(lhs->int64, rhs->int64)
                            : lhs->int64 * rhs->int64));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_ISIZE, mul)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_ISIZE);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue,
                          isize,
                          self->check_overflow
                            ? mul_with_overflow__Isize(lhs->isize, rhs->isize)
                            : lhs->isize * rhs->isize));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT8, mul)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_UINT8);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue,
                          uint8,
                          self->check_overflow
                            ? mul_with_overflow__Uint8(lhs->uint8, rhs->uint8)
                            : lhs->uint8 * rhs->uint8));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT16, mul)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_UINT16);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(
                      LilyInterpreterValue,
                      uint16,
                      self->check_overflow
                        ? mul_with_overflow__Uint16(lhs->uint16, rhs->uint16)
                        : lhs->uint16 * rhs->uint16));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT32, mul)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_UINT32);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(
                      LilyInterpreterValue,
                      uint32,
                      self->check_overflow
                        ? mul_with_overflow__Uint32(lhs->uint32, rhs->uint32)
                        : lhs->uint32 * rhs->uint32));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT64, mul)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_UINT64);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(
                      LilyInterpreterValue,
                      uint64,
                      self->check_overflow
                        ? mul_with_overflow__Uint64(lhs->uint64, rhs->uint64)
                        : lhs->uint64 * rhs->uint64));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_USIZE, mul)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_USIZE);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue,
                          usize,
                          self->check_overflow
                            ? mul_with_overflow__Usize(lhs->usize, rhs->usize)
                            : lhs->usize * rhs->usize));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_DEFAULT(mul)
        {
            RUNTIME_ERROR_UNREACHABLE("expected Int... or Uint...");
        }

        INT_INST_END();
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_INCTRACE) {}

    VM_INST(LILY_MIR_INSTRUCTION_KIND_INEG)
    {
        INT_INST_LOOKUP(neg);

        LilyInterpreterValue *rhs = VM_POP(stack);

        INT_INST_START(neg, rhs->kind);

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT8, neg)
        {
            VM_PUSH(stack, NEW_VARIANT(LilyInterpreterValue, int8, -rhs->int8));

            FREE_UVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT16, neg)
        {
            VM_PUSH(stack,
                    NEW_VARIANT(LilyInterpreterValue, int16, -rhs->int16));

            FREE_UVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT32, neg)
        {
            VM_PUSH(stack,
                    NEW_VARIANT(LilyInterpreterValue, int32, -rhs->int32));

            FREE_UVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT64, neg)
        {
            VM_PUSH(stack,
                    NEW_VARIANT(LilyInterpreterValue, int64, -rhs->int64));

            FREE_UVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_ISIZE, neg)
        {
            VM_PUSH(stack,
                    NEW_VARIANT(LilyInterpreterValue, isize, -rhs->isize));

            FREE_UVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT8, neg)
        {
            RUNTIME_ERROR_UNREACHABLE("expected Int...");
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT16, neg)
        {
            RUNTIME_ERROR_UNREACHABLE("expected Int...");
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT32, neg)
        {
            RUNTIME_ERROR_UNREACHABLE("expected Int...");
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT64, neg)
        {
            RUNTIME_ERROR_UNREACHABLE("expected Int...");
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_USIZE, neg)
        {
            RUNTIME_ERROR_UNREACHABLE("expected Int...");
        }

        INT_INST_DEFAULT(neg)
        {
            RUNTIME_ERROR_UNREACHABLE("expected Int...");
        }

        INT_INST_END();
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_IREM)
    {
        INT_INST_LOOKUP(rem);

        LilyInterpreterValue *rhs = VM_POP(stack);
        LilyInterpreterValue *lhs = VM_POP(stack);

        INT_INST_START(rem, lhs->kind);

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT8, rem)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_INT8);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue, int8, lhs->int8 % rhs->int8));

            FREE_UVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT16, rem)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_INT16);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(
                      LilyInterpreterValue, int16, lhs->int16 % rhs->int16));

            FREE_UVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT32, rem)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_INT32);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(
                      LilyInterpreterValue, int32, lhs->int32 % rhs->int32));

            FREE_UVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT64, rem)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_INT64);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(
                      LilyInterpreterValue, int64, lhs->int64 % rhs->int64));

            FREE_UVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_ISIZE, rem)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_ISIZE);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(
                      LilyInterpreterValue, isize, lhs->isize % rhs->isize));

            FREE_UVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT8, rem)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_UINT8);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(
                      LilyInterpreterValue, uint8, lhs->uint8 % rhs->uint8));

            FREE_UVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT16, rem)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_UINT16);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(
                      LilyInterpreterValue, uint16, lhs->uint16 % rhs->uint16));

            FREE_UVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT32, rem)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_UINT32);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(
                      LilyInterpreterValue, uint32, lhs->uint32 % rhs->uint32));

            FREE_UVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT64, rem)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_UINT64);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(
                      LilyInterpreterValue, uint64, lhs->uint64 % rhs->uint64));

            FREE_UVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_USIZE, rem)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_USIZE);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(
                      LilyInterpreterValue, usize, lhs->usize % rhs->usize));

            FREE_UVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_DEFAULT(rem)
        {
            RUNTIME_ERROR_UNREACHABLE("expected Int... or Uint...");
        }

        INT_INST_END();
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_ISOK) {}

    VM_INST(LILY_MIR_INSTRUCTION_KIND_ISERR) {}

    VM_INST(LILY_MIR_INSTRUCTION_KIND_ISUB)
    {
        INT_INST_LOOKUP(sub);

        LilyInterpreterValue *rhs = VM_POP(stack);
        LilyInterpreterValue *lhs = VM_POP(stack);

        INT_INST_START(sub, lhs->kind);

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT8, sub)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_INT8);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue, int8, lhs->int8 - rhs->int8));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT16, sub)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_INT16);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(
                      LilyInterpreterValue, int16, lhs->int16 - rhs->int16));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT32, sub)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_INT32);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(
                      LilyInterpreterValue, int32, lhs->int32 - rhs->int32));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT64, sub)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_INT64);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(
                      LilyInterpreterValue, int64, lhs->int64 - rhs->int64));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_ISIZE, sub)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_ISIZE);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(
                      LilyInterpreterValue, isize, lhs->isize - rhs->isize));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT8, sub)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_UINT8);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(
                      LilyInterpreterValue, uint8, lhs->uint8 - rhs->uint8));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT16, sub)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_UINT16);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(
                      LilyInterpreterValue, uint16, lhs->uint16 - rhs->uint16));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT32, sub)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_UINT32);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(
                      LilyInterpreterValue, uint32, lhs->uint32 - rhs->uint32));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT64, sub)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_UINT64);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(
                      LilyInterpreterValue, uint64, lhs->uint64 - rhs->uint64));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_USIZE, sub)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_USIZE);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(
                      LilyInterpreterValue, usize, lhs->usize - rhs->usize));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_DEFAULT(sub)
        {
            RUNTIME_ERROR_UNREACHABLE("expected Int... or Uint...");
        }

        INT_INST_END();
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_JMP)
    {
        CLEAN_BLOCK_STACK(false);

        LilyMirInstruction *block_inst =
          get__OrderedHashMap(current_block_iter.ordered_hash_map,
                              (char *)current_block_inst->jmp->name);

        VM_SET_CURRENT_BLOCK(&block_inst->block);
        VM_SET_CURRENT_BLOCK_INST_ITER(NEW(VecIter, block_inst->block.insts));

#ifdef LILY_FULL_ASSERT_VM
        ASSERT(block_inst->block.insts->len > 0);
#endif

        VM_SET_CURRENT_BLOCK_INST(next__VecIter(&current_block_inst_iter));

#ifdef LILY_FULL_ASSERT_VM
        ASSERT(current_block_inst);
        ASSERT(current_frame);
#endif
        add_block_frame__LilyInterpreterVMStackFrame(
          current_frame,
          block_inst->block.limit->id,
          (char *)block_inst->block.name,
          stack->len);

        return run_insts__LilyInterpreterVM(self);
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_JMP_COND)
    {
        push_value__LilyInterpreterVM(self, current_block_inst->jmpcond.cond);

        LilyInterpreterValue *cond = VM_POP(stack);
        LilyMirInstruction *block_inst = NULL;

        CLEAN_BLOCK_STACK(false);

#ifdef LILY_FULL_ASSERT_VM
        ASSERT(cond->kind == LILY_INTERPRETER_VALUE_KIND_TRUE ||
               cond->kind == LILY_INTERPRETER_VALUE_KIND_FALSE);
#endif

        if (cond->kind) {
            block_inst = get__OrderedHashMap(
              current_block_iter.ordered_hash_map,
              (char *)current_block_inst->jmpcond.then_block->name);
        } else {
            block_inst = get__OrderedHashMap(
              current_block_iter.ordered_hash_map,
              (char *)current_block_inst->jmpcond.else_block->name);
        }

        VM_SET_CURRENT_BLOCK(&block_inst->block);
        VM_SET_CURRENT_BLOCK_INST_ITER(NEW(VecIter, block_inst->block.insts));

#ifdef LILY_FULL_ASSERT_VM
        ASSERT(block_inst->block.insts->len > 0);
#endif

        VM_SET_CURRENT_BLOCK_INST(next__VecIter(&current_block_inst_iter));

#ifdef LILY_FULL_ASSERT_VM
        ASSERT(current_block_inst);
        ASSERT(current_frame);
#endif

        add_block_frame__LilyInterpreterVMStackFrame(
          current_frame,
          block_inst->block.limit->id,
          (char *)block_inst->block.name,
          stack->len);

        return run_insts__LilyInterpreterVM(self);
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_LEN) {}

    VM_INST(LILY_MIR_INSTRUCTION_KIND_LOAD) {}

    VM_INST(LILY_MIR_INSTRUCTION_KIND_MAKEREF) {}

    VM_INST(LILY_MIR_INSTRUCTION_KIND_MAKEOPT) {}

    VM_INST(LILY_MIR_INSTRUCTION_KIND_NON_NIL) {}

    VM_INST(LILY_MIR_INSTRUCTION_KIND_NOT)
    {
        LilyInterpreterValue *rhs = VM_POP(stack);

#ifdef LILY_FULL_ASSERT_VM
        ASSERT(rhs->kind == LILY_INTERPRETER_VALUE_KIND_TRUE ||
               rhs->kind == LILY_INTERPRETER_VALUE_KIND_FALSE);
#endif

        VM_PUSH(stack, NEW(LilyInterpreterValue, !rhs->kind));

        FREE_UVAL();
        EAT_NEXT_LABEL();
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_REF_PTR) {}

    VM_INST(LILY_MIR_INSTRUCTION_KIND_REG)
    {
        SET_NEXT_LABEL(reg_finish);

        VM_SET_CURRENT_BLOCK_INST(current_block_inst->reg.inst);
        VM_GOTO_INST(current_block_inst);

    reg_finish : {
        EAT_NEXT_LABEL();
    }
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_RET)
    {
        SET_NEXT_LABEL(ret_finish);

        VM_SET_CURRENT_BLOCK_INST(current_block_inst->ret);
        VM_GOTO_INST(current_block_inst);

    ret_finish : {
        LilyInterpreterValue *ret_value = VM_POP(stack);

        ASSERT(current_frame);
        set_return__LilyInterpreterVMStackFrame(
          current_frame,
          NEW_VARIANT(LilyInterpreterVMStackFrameReturn, normal, ret_value));

        CLEAN_BLOCK_STACK(true);
        EAT_NEXT_LABEL();
    }
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_SHL) {}

    VM_INST(LILY_MIR_INSTRUCTION_KIND_SHR) {}

    VM_INST(LILY_MIR_INSTRUCTION_KIND_STORE) {}

    VM_INST(LILY_MIR_INSTRUCTION_KIND_STRUCT) {}

    VM_INST(LILY_MIR_INSTRUCTION_KIND_SWITCH) {}

    VM_INST(LILY_MIR_INSTRUCTION_KIND_SYS_CALL) {}

    VM_INST(LILY_MIR_INSTRUCTION_KIND_TRUNC) {}

    VM_INST(LILY_MIR_INSTRUCTION_KIND_TRY) {}

    VM_INST(LILY_MIR_INSTRUCTION_KIND_TRY_PTR) {}

    VM_INST(LILY_MIR_INSTRUCTION_KIND_UNREACHABLE) {}

    VM_INST(LILY_MIR_INSTRUCTION_KIND_VAL)
    {
        push_value__LilyInterpreterVM(self, current_block_inst->val);
        EAT_NEXT_LABEL();
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_VAR) {}

    VM_INST(LILY_MIR_INSTRUCTION_KIND_XOR) {}

    VM_DEFAULT()
    {
        UNREACHABLE("unknown instruction");
    }

    VM_END();
}

void
run_insts__LilyInterpreterVM(LilyInterpreterVM *self)
{
run : {
    run_inst__LilyInterpreterVM(self);
    VM_NEXT_INST(run, exit);
}

exit : {
    return;
}
}

void
run__LilyInterpreterVM(LilyInterpreterVM *self)
{
run_vm : {
    run_inst__LilyInterpreterVM(self);
    VM_NEXT_INST(run_vm, exit_vm);
}

exit_vm : {
    // Clean up

    FREE(LilyInterpreterVMStack, &local_stack);
}
}
