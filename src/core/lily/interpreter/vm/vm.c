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

#include <core/lily/interpreter/vm.h>
#include <core/lily/interpreter/vm/runtime.h>
#include <core/lily/interpreter/vm/vm.h>

// Stack-based VM

#if defined(CLANG_VERSION) || defined(GCC_VERSION)
#define LILY_USE_COMPUTED_GOTOS
#endif

#define VM_PEEK(stack) peek__LilyInterpreterVMStack(stack)
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

#define VM_SET_CURRENT_FUN_INSTS(insts) current_fun_insts = insts;
#define VM_SET_CURRENT_BLOCK(block) current_block = block;
#define VM_SET_CURRENT_BLOCK_INST_ITER(iter) current_block_inst_iter = iter;
#define VM_SET_CURRENT_BLOCK_INST(new_inst) current_block_inst = new_inst;
#define VM_SET_CURRENT_BLOCK_FRAME(block_frame) \
    current_block_frame = block_frame;
#define VM_SET_CURRENT_FRAME(frame) current_frame = frame;
#define VM_GOTO_INST(inst) goto *inst_lookup[inst->kind];
#define VM_NEXT(run) goto run
#define VM_EXIT(exit) goto exit
#define VM_END() }
#define VM_NEXT_INST(run, exit)                                           \
    if ((current_block_inst = next__VecIter(&current_block_inst_iter))) { \
        VM_NEXT(run);                                                     \
    } else {                                                              \
        VM_EXIT(exit);                                                    \
    }

#define CLEAN_BLOCK_STACK(limit_id)                          \
    /* Clean stack of the last block. */                     \
    clean_block_stack__LilyInterpreterVMStack(&local_stack); \
    FREE(LilyInterpreterVMStackBlockFrame,                   \
         &current_frame->block_frames[limit_id]);

#define JMP_CLEAN_BLOCK_STACK()                                               \
    /* Clean the last block if the its limit id is equal to its id, before to \
     * run the new current block. */                                          \
    if (last_block->id == last_block->limit->id) {                            \
        LilyInterpreterVMStackBlockFrame *current_block_frame_parent =        \
          current_block_frame->parent;                                        \
                                                                              \
        current_block_frame_parent->end = 0;                                  \
                                                                              \
        CLEAN_BLOCK_STACK(last_block->id);                                    \
                                                                              \
        VM_SET_CURRENT_BLOCK_FRAME(current_block_frame_parent);               \
                                                                              \
        current_frame->current_block_frame_limit_id =                         \
          current_block_frame->limit_id;                                      \
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

static threadlocal OrderedHashMap *current_fun_insts = NULL;
static threadlocal LilyMirInstructionBlock *current_block = NULL;
static threadlocal VecIter current_block_inst_iter;
static threadlocal LilyMirInstruction *current_block_inst = NULL;
static threadlocal LilyInterpreterVMStack local_stack;
static threadlocal LilyInterpreterVMStackBlockFrame *current_block_frame = NULL;
static threadlocal LilyInterpreterVMStackFrame *current_frame = NULL;

CONSTRUCTOR(LilyInterpreterVMStackBlockFrame *,
            LilyInterpreterVMStackBlockFrame,
            char *name,
            Usize begin,
            Usize end,
            Usize limit_id)
{
    LilyInterpreterVMStackBlockFrame *self =
      lily_malloc(sizeof(LilyInterpreterVMStackBlockFrame));

    self->names = init__Vec(1, name);
    self->variables = NEW(HashMap);
    self->regs = NEW(HashMap);
    self->begin = begin;
    self->end = end;
    self->limit_id = limit_id;
    self->parent = NULL;

    return self;
}

void
add_name__LilyInterpreterVMStackBlockFrame(
  const LilyInterpreterVMStackBlockFrame *self,
  char *name)
{
    for (Usize i = 0; i < self->names->len; ++i) {
        if (!strcmp(get__Vec(self->names, i), name)) {
            return;
        }
    }

    return push__Vec(self->names, name);
}

void
add_reg__LilyInterpreterVMStackBlockFrame(
  const LilyInterpreterVMStackBlockFrame *self,
  char *name,
  LilyInterpreterValue *value)
{
#ifdef LILY_FULL_ASSERT_VM
    LilyInterpreterValue *duplicate_value =
      insert__HashMap(self->regs, name, value);
    ASSERT(!duplicate_value);
#else
    insert__HashMap(self->regs, name, value);
#endif
}

void
add_variable__LilyInterpreterVMStackBlockFrame(
  const LilyInterpreterVMStackBlockFrame *self,
  char *name,
  LilyInterpreterValue *value)
{
#ifdef LILY_FULL_ASSERT_VM
    LilyInterpreterValue *duplicate_value =
      insert__HashMap(self->variables, name, value);
    ASSERT(!duplicate_value);
#else
    insert__HashMap(self->variables, name, value);
#endif
}

LilyInterpreterValue *
search_reg__LilyInterpreterVMStackBlockFrame(
  const LilyInterpreterVMStackBlockFrame *self,
  char *name)
{
    LilyInterpreterValue *reg_value = get__HashMap(self->regs, name);

    if (reg_value) {
        return ref__LilyInterpreterValue(reg_value);
    }

#ifdef LILY_FULL_ASSERT_VM
    if (self->parent) {
        return search_reg__LilyInterpreterVMStackBlockFrame(self->parent, name);
    }

    UNREACHABLE("the parent is NULL, the reg is not found");
#else
    // NOTE: Technically, the function cannot return NULL.
    return self->parent
             ? search_reg__LilyInterpreterVMStackBlockFrame(self->parent, name)
             : NULL;
#endif
}

LilyInterpreterValue *
search_variable__LilyInterpreterVMStackBlockFrame(
  const LilyInterpreterVMStackBlockFrame *self,
  char *name)
{
#ifdef LILY_FULL_ASSERT_VM
    ASSERT(self);
#endif

    LilyInterpreterValue *var_value = get__HashMap(self->variables, name);

    if (var_value) {
        return ref__LilyInterpreterValue(var_value);
    }

#ifdef LILY_FULL_ASSERT_VM
    if (self->parent) {
        return search_variable__LilyInterpreterVMStackBlockFrame(self->parent,
                                                                 name);
    }

    UNREACHABLE("the parent is NULL, the variable is not found");
#else
    // NOTE: Technically, the function cannot return NULL.
    return self->parent ? search_variable__LilyInterpreterVMStackBlockFrame(
                            self->parent, name)
                        : NULL;
#endif
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
    self->params_len = params_len;
    self->begin = begin;
    self->end = 0;
    self->current_block_frame_limit_id = current_block_frame_limit_id;
    self->block_frames = lily_calloc(block_frames_len, PTR_SIZE);
    self->block_frames_len = block_frames_len;
    self->next = NULL;

    for (Usize i = 0; i < self->params_len; ++i) {
        self->params[i] = params[i];
    }

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
    LilyInterpreterVMStackBlockFrame **current =
      &self->block_frames[current_block_frame_limit_id];
    // Set end value to the last current block frame.
    // Set a parent (last) value to the current block frame.
    LilyInterpreterVMStackBlockFrame *last =
      self->block_frames[self->current_block_frame_limit_id];

    if (!(*current)) {
        // Set a new block frame.
        *current = NEW(LilyInterpreterVMStackBlockFrame,
                       name,
                       begin,
                       0,
                       current_block_frame_limit_id);
    } else {
        // Add name to the already created block frame.
        add_name__LilyInterpreterVMStackBlockFrame(*current, name);
        (*current)->end = 0;
    }

    if (last && last != *current) {
        last->end = begin;
        (*current)->parent = last;
    }

    self->current_block_frame_limit_id = current_block_frame_limit_id;
    VM_SET_CURRENT_BLOCK_FRAME(*current);
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

    for (Usize i = 0; i < (*self)->block_frames_len; ++i) {
        LilyInterpreterVMStackBlockFrame *current = (*self)->block_frames[i];

        if (current) {
            FREE(LilyInterpreterVMStackBlockFrame, &current);
        }
    }

    while (local_stack.len > (*self)->begin) {
        LilyInterpreterValue value = VM_POP(&local_stack);

        FREE(LilyInterpreterValue, &value);
    }

    lily_free((*self)->block_frames);
    lily_free(*self);

    *self = NULL;
}

CONSTRUCTOR(LilyInterpreterVMStack, LilyInterpreterVMStack, Usize max_capacity)
{
    Usize mc = max_capacity == 0 ? DEFAULT_MAX_STACK_CAPACITY : max_capacity;

    return (LilyInterpreterVMStack){ .buffer = __alloc__$Alloc(mc, 0),
                                     .current_frame = NULL,
                                     .max_capacity = mc,
                                     .len = 0 };
}

void
push__LilyInterpreterVMStack(LilyInterpreterVMStack *self,
                             LilyInterpreterValue value)
{
    if (self->len + 1 > self->max_capacity) {
        RUNTIME_ERROR_STACK_OVERFLOW();
    }

    self->buffer[self->len++] = value;
}

LilyInterpreterValue
pop__LilyInterpreterVMStack(LilyInterpreterVMStack *self)
{
    if (self->len == 0) {
        RUNTIME_ERROR_STACK_EMPTY();
    }

    return self->buffer[--self->len];
}

LilyInterpreterValue *
peek__LilyInterpreterVMStack(LilyInterpreterVMStack *self)
{
    if (self->len == 0) {
        RUNTIME_ERROR_STACK_EMPTY();
    }

    return &self->buffer[self->len - 1];
}

void
set_frame__LilyInterpreterVMStack(LilyInterpreterVMStack *self,
                                  LilyInterpreterVMStackFrame *frame)
{
    ASSERT(!self->current_frame);
    self->current_frame = frame;
    current_frame = self->current_frame;
}

void
clean_block_stack__LilyInterpreterVMStack(LilyInterpreterVMStack *self)
{
    LilyInterpreterVMStackBlockFrame *current_block_frame =
      current_frame->block_frames[current_frame->current_block_frame_limit_id];

    // NOTE: No block frame in front of it is expected.
#ifdef LILY_FULL_ASSERT_VM
    ASSERT(current_block_frame);
    ASSERT(current_block_frame->end == 0);
#endif

    while (self->len > current_block_frame->begin) {
        LilyInterpreterValue value = VM_POP(self);

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

DESTRUCTOR(LilyInterpreterVMStack, LilyInterpreterVMStack *self)
{
    while (self->len > 0) {
        LilyInterpreterValue value = VM_POP(self);

        FREE(LilyInterpreterValue, &value);
    }

    __free__$Alloc((void **)&self->buffer, self->max_capacity, 0);
}

CONSTRUCTOR(LilyInterpreterVM,
            LilyInterpreterVM,
            Usize heap_capacity,
            Usize stack_capacity,
            const LilyMirModule *module,
            LilyInterpreterVMResources resources,
            bool check_overflow)
{
    LilyInterpreterMemory memory =
      heap_capacity
        ? NEW_VARIANT(LilyInterpreterMemory, capacity, heap_capacity)
        : NEW(LilyInterpreterMemory);
    LilyMirInstruction *entry_point =
      get__OrderedHashMap(module->insts, "main");

    ASSERT(entry_point);
    ASSERT(resources.args);
    ASSERT(entry_point->kind == LILY_MIR_INSTRUCTION_KIND_FUN);
    ASSERT(entry_point->fun.insts->len >= 1);

    VM_SET_CURRENT_FUN_INSTS(entry_point->fun.insts);

    {
        LilyMirInstruction *inst =
          get_from_id__OrderedHashMap(current_fun_insts, 0);

        ASSERT(inst);

        current_block = &inst->block;
    }

    current_block_inst_iter = NEW(VecIter, current_block->insts);
    current_block_inst = next__VecIter(&current_block_inst_iter);
    local_stack = stack_capacity
                    ? NEW(LilyInterpreterVMStack, stack_capacity)
                    : NEW(LilyInterpreterVMStack, DEFAULT_MAX_STACK_CAPACITY);

    LilyInterpreterValue argc_value =
      NEW_VARIANT(LilyInterpreterValue, int32, resources.args->len);
    LilyInterpreterValue argv_value =
      NEW_VARIANT(LilyInterpreterValue, ptr, resources.args->buffer);

    VM_PUSH(&local_stack, argv_value);

    LilyInterpreterValue *argv_value_ref = VM_PEEK(&local_stack);

    VM_PUSH(&local_stack, argc_value);

    LilyInterpreterValue *argc_value_ref = VM_PEEK(&local_stack);

    set_frame__LilyInterpreterVMStack(
      &local_stack,
      NEW(LilyInterpreterVMStackFrame,
          entry_point->fun.name,
          (LilyInterpreterValue *[MAX_FUN_PARAMS]){ argc_value_ref,
                                                    argv_value_ref },
          2,
          local_stack.len,
          current_block->limit->id,
          entry_point->fun.insts->len));
    add_block_frame__LilyInterpreterVMStackFrame(current_frame,
                                                 current_block->limit->id,
                                                 (char *)current_block->name,
                                                 local_stack.len);

    ASSERT(current_frame);
    ASSERT(current_block_frame);
    ASSERT(current_block);
    ASSERT(current_block_inst);

    return (LilyInterpreterVM){ .memory = memory,
                                .module = module,
                                .entry_point = entry_point,
                                .resources = resources,
                                .check_overflow = check_overflow };
}

void
push_value__LilyInterpreterVM(LilyInterpreterVM *self,
                              const LilyMirInstructionVal *val)
{
    switch (val->kind) {
        case LILY_MIR_INSTRUCTION_VAL_KIND_ARRAY:
            TODO("push array");
        case LILY_MIR_INSTRUCTION_VAL_KIND_BYTES:
            return VM_PUSH(&local_stack,
                           NEW_VARIANT(LilyInterpreterValue,
                                       bytes,
                                       NEW(LilyInterpreterValueBytes,
                                           (Uint8 *)val->bytes,
                                           val->dt->bytes.len)));
        case LILY_MIR_INSTRUCTION_VAL_KIND_CONST:
            TODO("push const");
        case LILY_MIR_INSTRUCTION_VAL_KIND_CSTR:
            return VM_PUSH(
              &local_stack,
              NEW_VARIANT(LilyInterpreterValue, cstr, (char *)val->cstr));
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
            return VM_PUSH(
              &local_stack,
              NEW(LilyInterpreterValue, LILY_INTERPRETER_VALUE_KIND_NIL));
        case LILY_MIR_INSTRUCTION_VAL_KIND_PARAM:
            return VM_PUSH(
              &local_stack,
              *ref__LilyInterpreterValue(current_frame->params[val->param]));
        case LILY_MIR_INSTRUCTION_VAL_KIND_REG:
            return VM_PUSH(&local_stack,
                           *search_reg__LilyInterpreterVMStackBlockFrame(
                             current_block_frame, (char *)val->reg));
        case LILY_MIR_INSTRUCTION_VAL_KIND_SLICE:
            TODO("push slice");
        case LILY_MIR_INSTRUCTION_VAL_KIND_STR:
            return VM_PUSH(
              &local_stack,
              NEW_VARIANT(
                LilyInterpreterValue,
                str,
                NEW(LilyInterpreterValueStr, val->str->buffer, val->str->len)));
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
            return VM_PUSH(
              &local_stack,
              NEW(LilyInterpreterValue, LILY_INTERPRETER_VALUE_KIND_UNDEF));
        case LILY_MIR_INSTRUCTION_VAL_KIND_UNIT:
            return VM_PUSH(
              &local_stack,
              NEW(LilyInterpreterValue, LILY_INTERPRETER_VALUE_KIND_UNIT));
        case LILY_MIR_INSTRUCTION_VAL_KIND_VAR:
            // Bring and back to the front the var value.
            return VM_PUSH(&local_stack,
                           *search_variable__LilyInterpreterVMStackBlockFrame(
                             current_block_frame, (char *)val->var));
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
#define INT_INST_LOOKUP(inst)                                                 \
    static void *inst##_lookup[28] = {                                        \
        [LILY_INTERPRETER_VALUE_KIND_TRUE] = &&label__unknown_i##inst,        \
        [LILY_INTERPRETER_VALUE_KIND_FALSE] = &&label__unknown_i##inst,       \
        [LILY_INTERPRETER_VALUE_KIND_BYTES] = &&label__unknown_i##inst,       \
        [LILY_INTERPRETER_VALUE_KIND_CSTR] = &&label__unknown_i##inst,        \
        [LILY_INTERPRETER_VALUE_KIND_DESTROYED] = &&label__unknown_i##inst,   \
        [LILY_INTERPRETER_VALUE_KIND_DYNAMIC_ARRAY] =                         \
          &&label__unknown_i##inst,                                           \
        [LILY_INTERPRETER_VALUE_KIND_INT8] =                                  \
          &&label__LILY_INTERPRETER_VALUE_KIND_INT8_i##inst,                  \
        [LILY_INTERPRETER_VALUE_KIND_INT16] =                                 \
          &&label__LILY_INTERPRETER_VALUE_KIND_INT16_i##inst,                 \
        [LILY_INTERPRETER_VALUE_KIND_INT32] =                                 \
          &&label__LILY_INTERPRETER_VALUE_KIND_INT32_i##inst,                 \
        [LILY_INTERPRETER_VALUE_KIND_INT64] =                                 \
          &&label__LILY_INTERPRETER_VALUE_KIND_INT64_i##inst,                 \
        [LILY_INTERPRETER_VALUE_KIND_ISIZE] =                                 \
          &&label__LILY_INTERPRETER_VALUE_KIND_ISIZE_i##inst,                 \
        [LILY_INTERPRETER_VALUE_KIND_FLOAT] = &&label__unknown_i##inst,       \
        [LILY_INTERPRETER_VALUE_KIND_LIST] = &&label__unknown_i##inst,        \
        [LILY_INTERPRETER_VALUE_KIND_MULTI_POINTERS_ARRAY] =                  \
          &&label__unknown_i##inst,                                           \
        [LILY_INTERPRETER_VALUE_KIND_NIL] = &&label__unknown_i##inst,         \
        [LILY_INTERPRETER_VALUE_KIND_PTR] = &&label__unknown_i##inst,         \
        [LILY_INTERPRETER_VALUE_KIND_RESULT] = &&label__unknown_i##inst,      \
        [LILY_INTERPRETER_VALUE_KIND_SIZED_ARRAY] = &&label__unknown_i##inst, \
        [LILY_INTERPRETER_VALUE_KIND_STR] = &&label__unknown_i##inst,         \
        [LILY_INTERPRETER_VALUE_KIND_STRUCT] = &&label__unknown_i##inst,      \
        [LILY_INTERPRETER_VALUE_KIND_UNDEF] = &&label__unknown_i##inst,       \
        [LILY_INTERPRETER_VALUE_KIND_UINT8] =                                 \
          &&label__LILY_INTERPRETER_VALUE_KIND_UINT8_i##inst,                 \
        [LILY_INTERPRETER_VALUE_KIND_UINT16] =                                \
          &&label__LILY_INTERPRETER_VALUE_KIND_UINT16_i##inst,                \
        [LILY_INTERPRETER_VALUE_KIND_UINT32] =                                \
          &&label__LILY_INTERPRETER_VALUE_KIND_UINT32_i##inst,                \
        [LILY_INTERPRETER_VALUE_KIND_UINT64] =                                \
          &&label__LILY_INTERPRETER_VALUE_KIND_UINT64_i##inst,                \
        [LILY_INTERPRETER_VALUE_KIND_USIZE] =                                 \
          &&label__LILY_INTERPRETER_VALUE_KIND_USIZE_i##inst,                 \
        [LILY_INTERPRETER_VALUE_KIND_UNIT] = &&label__unknown_i##inst,        \
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

    VM_INST(LILY_MIR_INSTRUCTION_KIND_ALLOC)
    {
        // NOTE: The `alloc` instruction is not used when executing a basic
        // interpreter. But if we add an option to calculate the stack usage for
        // each function or something like that, in that case the alloc
        // instruction could be used.
        EAT_NEXT_LABEL();
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_ARG)
    {
        TODO("arg");
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_ASM)
    {
        TODO("asm");
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_BITCAST)
    {
        TODO("bitcast");
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_BITAND)
    {
        INT_INST_LOOKUP(bitand);

        push_value__LilyInterpreterVM(self, current_block_inst->bitand.dest);
        push_value__LilyInterpreterVM(self, current_block_inst->bitand.src);

        LilyInterpreterValue rhs = VM_POP(stack);
        LilyInterpreterValue lhs = VM_POP(stack);

        INT_INST_START(bitand, lhs.kind);

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT8, bitand)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT8);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue, int8, lhs.int8 & rhs.int8));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT16, bitand)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT16);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue, int16, lhs.int16 & rhs.int16));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT32, bitand)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT32);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue, int32, lhs.int32 & rhs.int32));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT64, bitand)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT64);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue, int64, lhs.int64 & rhs.int64));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_ISIZE, bitand)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_ISIZE);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue, isize, lhs.isize & rhs.isize));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT8, bitand)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT8);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue, uint8, lhs.uint8 & rhs.uint8));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT16, bitand)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT16);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(
                      LilyInterpreterValue, uint16, lhs.uint16 & rhs.uint16));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT32, bitand)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT32);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(
                      LilyInterpreterValue, uint32, lhs.uint32 & rhs.uint32));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT64, bitand)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT64);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(
                      LilyInterpreterValue, uint64, lhs.uint64 & rhs.uint64));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_USIZE, bitand)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_USIZE);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue, usize, lhs.usize & rhs.usize));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_DEFAULT(bitand)
        {
            RUNTIME_ERROR_UNREACHABLE("expected Int... or Uint...");
        }

        INT_INST_END();
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_BITNOT)
    {
        INT_INST_LOOKUP(bitnot);

        push_value__LilyInterpreterVM(self, current_block_inst->bitnot.src);

        LilyInterpreterValue rhs = VM_POP(stack);

        INT_INST_START(bitnot, rhs.kind);

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT8, bitnot)
        {
            VM_PUSH(stack, NEW_VARIANT(LilyInterpreterValue, int8, ~rhs.int8));

            FREE_UVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT16, bitnot)
        {
            VM_PUSH(stack,
                    NEW_VARIANT(LilyInterpreterValue, int16, ~rhs.int16));

            FREE_UVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT32, bitnot)
        {
            VM_PUSH(stack,
                    NEW_VARIANT(LilyInterpreterValue, int32, ~rhs.int32));

            FREE_UVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT64, bitnot)
        {
            VM_PUSH(stack,
                    NEW_VARIANT(LilyInterpreterValue, int64, ~rhs.int64));

            FREE_UVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_ISIZE, bitnot)
        {
            VM_PUSH(stack,
                    NEW_VARIANT(LilyInterpreterValue, isize, ~rhs.isize));

            FREE_UVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT8, bitnot)
        {
            RUNTIME_ERROR_UNREACHABLE("expected Int...");
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT16, bitnot)
        {
            RUNTIME_ERROR_UNREACHABLE("expected Int...");
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT32, bitnot)
        {
            RUNTIME_ERROR_UNREACHABLE("expected Int...");
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT64, bitnot)
        {
            RUNTIME_ERROR_UNREACHABLE("expected Int...");
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_USIZE, bitnot)
        {
            RUNTIME_ERROR_UNREACHABLE("expected Int...");
        }

        INT_INST_DEFAULT(bitnot)
        {
            RUNTIME_ERROR_UNREACHABLE("expected Int...");
        }

        INT_INST_END();
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_BITOR)
    {
        INT_INST_LOOKUP(bitor);

        push_value__LilyInterpreterVM(self, current_block_inst->bitor.dest);
        push_value__LilyInterpreterVM(self, current_block_inst->bitor.src);

        LilyInterpreterValue rhs = VM_POP(stack);
        LilyInterpreterValue lhs = VM_POP(stack);

        INT_INST_START(bitor, lhs.kind);

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT8, bitor)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT8);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue, int8, lhs.int8 | rhs.int8));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT16, bitor)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT16);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue, int16, lhs.int16 | rhs.int16));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT32, bitor)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT32);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue, int32, lhs.int32 | rhs.int32));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT64, bitor)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT64);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue, int64, lhs.int64 | rhs.int64));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_ISIZE, bitor)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_ISIZE);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue, isize, lhs.isize | rhs.isize));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT8, bitor)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT8);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue, uint8, lhs.uint8 | rhs.uint8));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT16, bitor)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT16);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(
                      LilyInterpreterValue, uint16, lhs.uint16 | rhs.uint16));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT32, bitor)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT32);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(
                      LilyInterpreterValue, uint32, lhs.uint32 | rhs.uint32));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT64, bitor)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT64);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(
                      LilyInterpreterValue, uint64, lhs.uint64 | rhs.uint64));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_USIZE, bitor)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_USIZE);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue, usize, lhs.usize | rhs.usize));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_DEFAULT(bitor)
        {
            RUNTIME_ERROR_UNREACHABLE("expected Int... or Uint...");
        }

        INT_INST_END();
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_BLOCK)
    {
        UNREACHABLE("block is not expected");
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_BUILTIN_CALL)
    {
        TODO("builtin call");
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_CALL)
    {
        OrderedHashMap *last_current_fun_insts = current_fun_insts;
        LilyMirInstructionBlock *last_current_block = current_block;
        VecIter last_current_block_inst_iter = current_block_inst_iter;
        LilyMirInstruction *last_current_block_inst = current_block_inst;
        LilyInterpreterVMStackBlockFrame *last_current_block_frame =
          current_block_frame;
        // TODO: Manage function prototype.
        LilyMirInstruction *fun_inst = get__OrderedHashMap(
          self->module->insts, (char *)last_current_block_inst->call.name);
        LilyInterpreterVMStackFrame *last_current_frame = current_frame;

#ifdef LILY_FULL_ASSERT_VM
        ASSERT(fun_inst->kind == LILY_MIR_INSTRUCTION_KIND_FUN);
#endif

        VM_SET_CURRENT_FUN_INSTS(fun_inst->fun.insts);

        {
            LilyMirInstruction *block_inst =
              get_from_id__OrderedHashMap(current_fun_insts, 0);

#ifdef LILY_FULL_ASSERT_VM
            ASSERT(block_inst);
#endif

            VM_SET_CURRENT_BLOCK(&block_inst->block);
            VM_SET_CURRENT_BLOCK_INST_ITER(
              NEW(VecIter, block_inst->block.insts));

#ifdef LILY_FULL_ASSERT_VM
            ASSERT(block_inst->block.insts->len > 0);
#endif

            VM_SET_CURRENT_BLOCK_INST(next__VecIter(&current_block_inst_iter));

#ifdef LILY_FULL_ASSERT_VM
            ASSERT(current_block_inst);
#endif
        }

        Usize params_len = last_current_block_inst->call.params->len;
        LilyInterpreterValue *params[MAX_FUN_PARAMS] = { 0 };

        for (Usize i = params_len; i--;) {
            push_value__LilyInterpreterVM(
              self, get__Vec(last_current_block_inst->call.params, i));

            params[i] = VM_PEEK(stack);
        }

        VM_SET_CURRENT_FRAME(attach_stack_frame__LilyInterpreterVMStackFrame(
          last_current_frame,
          last_current_block_inst->call.name,
          params,
          params_len,
          stack->len,
          current_block->limit->id,
          fun_inst->fun.insts->len));

        add_block_frame__LilyInterpreterVMStackFrame(
          current_frame,
          current_block->limit->id,
          (char *)current_block->name,
          local_stack.len);

#ifdef LILY_FULL_ASSERT_VM
        ASSERT(current_frame);
        ASSERT(current_block_frame);
        ASSERT(current_block);
        ASSERT(current_block_inst);
#endif

        // Run the function.
        run_insts__LilyInterpreterVM(self);

        // Get the return value.
        LilyInterpreterValue return_value;

        switch (current_frame->return_.kind) {
            case LILY_INTERPRETER_VM_STACK_FRAME_RETURN_KIND_NORMAL:
                return_value = current_frame->return_.value;
                break;
            case LILY_INTERPRETER_VM_STACK_FRAME_RETURN_KIND_RAISE:
                TODO("raise value");
            default:
                UNREACHABLE("unknown variant");
        }

        // Clean the frame
        FREE(LilyInterpreterVMStackFrame, &last_current_frame->next);

        // Clean the stack (params).
        for (Usize i = 0; i < params_len; ++i) {
            LilyInterpreterValue param = VM_POP(stack);

            FREE(LilyInterpreterValue, &param);
        }

        VM_PUSH(stack, return_value);

        // Restore last values
        VM_SET_CURRENT_FUN_INSTS(last_current_fun_insts);
        VM_SET_CURRENT_BLOCK(last_current_block);
        VM_SET_CURRENT_BLOCK_INST_ITER(last_current_block_inst_iter);
        VM_SET_CURRENT_BLOCK_INST(last_current_block_inst);
        VM_SET_CURRENT_BLOCK_FRAME(last_current_block_frame);
        VM_SET_CURRENT_FRAME(last_current_frame);

        EAT_NEXT_LABEL();
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_CONST)
    {
        TODO("const");
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_DROP)
    {
        TODO("drop");
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_EXP)
    {
        TODO("exp");
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_FADD)
    {
        push_value__LilyInterpreterVM(self, current_block_inst->fadd.dest);
        push_value__LilyInterpreterVM(self, current_block_inst->fadd.src);

        LilyInterpreterValue rhs = VM_POP(stack);
        LilyInterpreterValue lhs = VM_POP(stack);

#ifdef LILY_FULL_ASSERT_VM
        ASSERT(lhs.kind == LILY_INTERPRETER_VALUE_KIND_FLOAT);
        ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_FLOAT);
#endif

        VM_PUSH(
          stack,
          NEW_VARIANT(LilyInterpreterValue, float, lhs.float_ + rhs.float_));

        FREE_BVAL();
        EAT_NEXT_LABEL();
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_FCMP_EQ)
    {
        push_value__LilyInterpreterVM(self, current_block_inst->fcmp_eq.dest);
        push_value__LilyInterpreterVM(self, current_block_inst->fcmp_eq.src);

        LilyInterpreterValue rhs = VM_POP(stack);
        LilyInterpreterValue lhs = VM_POP(stack);

#ifdef LILY_FULL_ASSERT_VM
        ASSERT(lhs.kind == LILY_INTERPRETER_VALUE_KIND_FLOAT);
        ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_FLOAT);
#endif

        VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.float_ == rhs.float_));

        FREE_BVAL();
        EAT_NEXT_LABEL();
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_FCMP_NE)
    {
        push_value__LilyInterpreterVM(self, current_block_inst->fcmp_ne.dest);
        push_value__LilyInterpreterVM(self, current_block_inst->fcmp_ne.src);

        LilyInterpreterValue rhs = VM_POP(stack);
        LilyInterpreterValue lhs = VM_POP(stack);

#ifdef LILY_FULL_ASSERT_VM
        ASSERT(lhs.kind == LILY_INTERPRETER_VALUE_KIND_FLOAT);
        ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_FLOAT);
#endif

        VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.float_ != rhs.float_));

        FREE_BVAL();
        EAT_NEXT_LABEL();
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_FCMP_LE)
    {
        push_value__LilyInterpreterVM(self, current_block_inst->fcmp_le.dest);
        push_value__LilyInterpreterVM(self, current_block_inst->fcmp_le.src);

        LilyInterpreterValue rhs = VM_POP(stack);
        LilyInterpreterValue lhs = VM_POP(stack);

#ifdef LILY_FULL_ASSERT_VM
        ASSERT(lhs.kind == LILY_INTERPRETER_VALUE_KIND_FLOAT);
        ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_FLOAT);
#endif

        VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.float_ <= rhs.float_));

        FREE_BVAL();
        EAT_NEXT_LABEL();
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_FCMP_LT)
    {
        push_value__LilyInterpreterVM(self, current_block_inst->fcmp_lt.dest);
        push_value__LilyInterpreterVM(self, current_block_inst->fcmp_lt.src);

        LilyInterpreterValue rhs = VM_POP(stack);
        LilyInterpreterValue lhs = VM_POP(stack);

#ifdef LILY_FULL_ASSERT_VM
        ASSERT(lhs.kind == LILY_INTERPRETER_VALUE_KIND_FLOAT);
        ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_FLOAT);
#endif

        VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.float_ <= rhs.float_));

        FREE(LilyInterpreterValue, &lhs);
        FREE(LilyInterpreterValue, &rhs);

        FREE_BVAL();
        EAT_NEXT_LABEL();
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_FCMP_GE)
    {
        push_value__LilyInterpreterVM(self, current_block_inst->fcmp_ge.dest);
        push_value__LilyInterpreterVM(self, current_block_inst->fcmp_ge.src);

        LilyInterpreterValue rhs = VM_POP(stack);
        LilyInterpreterValue lhs = VM_POP(stack);

#ifdef LILY_FULL_ASSERT_VM
        ASSERT(lhs.kind == LILY_INTERPRETER_VALUE_KIND_FLOAT);
        ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_FLOAT);
#endif

        VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.float_ >= rhs.float_));

        FREE(LilyInterpreterValue, &lhs);
        FREE(LilyInterpreterValue, &rhs);

        FREE_BVAL();
        EAT_NEXT_LABEL();
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_FCMP_GT)
    {
        push_value__LilyInterpreterVM(self, current_block_inst->fcmp_gt.dest);
        push_value__LilyInterpreterVM(self, current_block_inst->fcmp_gt.src);

        LilyInterpreterValue rhs = VM_POP(stack);
        LilyInterpreterValue lhs = VM_POP(stack);

#ifdef LILY_FULL_ASSERT_VM
        ASSERT(lhs.kind == LILY_INTERPRETER_VALUE_KIND_FLOAT);
        ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_FLOAT);
#endif

        VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.float_ > rhs.float_));

        FREE_BVAL();
        EAT_NEXT_LABEL();
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_FDIV)
    {
        push_value__LilyInterpreterVM(self, current_block_inst->fdiv.dest);
        push_value__LilyInterpreterVM(self, current_block_inst->fdiv.src);

        LilyInterpreterValue rhs = VM_POP(stack);
        LilyInterpreterValue lhs = VM_POP(stack);

#ifdef LILY_FULL_ASSERT_VM
        ASSERT(lhs.kind == LILY_INTERPRETER_VALUE_KIND_FLOAT);
        ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_FLOAT);
#endif

        VM_PUSH(
          stack,
          NEW_VARIANT(LilyInterpreterValue, float, lhs.float_ / rhs.float_));

        FREE_BVAL();
        EAT_NEXT_LABEL();
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_FMUL)
    {
        push_value__LilyInterpreterVM(self, current_block_inst->fmul.dest);
        push_value__LilyInterpreterVM(self, current_block_inst->fmul.src);

        LilyInterpreterValue rhs = VM_POP(stack);
        LilyInterpreterValue lhs = VM_POP(stack);

#ifdef LILY_FULL_ASSERT_VM
        ASSERT(lhs.kind == LILY_INTERPRETER_VALUE_KIND_FLOAT);
        ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_FLOAT);
#endif

        VM_PUSH(
          stack,
          NEW_VARIANT(LilyInterpreterValue, float, lhs.float_ *rhs.float_));

        FREE_BVAL();
        EAT_NEXT_LABEL();
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_FNEG)
    {
        push_value__LilyInterpreterVM(self, current_block_inst->fneg.src);

        LilyInterpreterValue rhs = VM_POP(stack);

#ifdef LILY_FULL_ASSERT_VM
        ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_FLOAT);
#endif

        VM_PUSH(stack, NEW_VARIANT(LilyInterpreterValue, float, -rhs.float_));

        FREE_UVAL();
        EAT_NEXT_LABEL();
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_FREM)
    {
        push_value__LilyInterpreterVM(self, current_block_inst->frem.dest);
        push_value__LilyInterpreterVM(self, current_block_inst->frem.src);

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

        FREE_BVAL();
        EAT_NEXT_LABEL();
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_FSUB)
    {
        push_value__LilyInterpreterVM(self, current_block_inst->fsub.dest);
        push_value__LilyInterpreterVM(self, current_block_inst->fsub.src);

        LilyInterpreterValue rhs = VM_POP(stack);
        LilyInterpreterValue lhs = VM_POP(stack);

#ifdef LILY_FULL_ASSERT_VM
        ASSERT(lhs.kind == LILY_INTERPRETER_VALUE_KIND_FLOAT);
        ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_FLOAT);
#endif

        VM_PUSH(
          stack,
          NEW_VARIANT(LilyInterpreterValue, float, lhs.float_ - rhs.float_));

        FREE_BVAL();
        EAT_NEXT_LABEL();
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_FUN)
    {
        TODO("fun");
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_FUN_PROTOTYPE)
    {
        TODO("fun prototype");
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_GETARG)
    {
        TODO("getarg");
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_GETARRAY)
    {
        TODO("getarray");
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_GETLIST)
    {
        TODO("getlist");
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_GETSLICE)
    {
        TODO("getslice");
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_GETFIELD)
    {
        TODO("getfield");
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_GETPTR)
    {
        TODO("getptr");
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_IADD)
    {
        INT_INST_LOOKUP(add);

        push_value__LilyInterpreterVM(self, current_block_inst->iadd.dest);
        push_value__LilyInterpreterVM(self, current_block_inst->iadd.src);

        LilyInterpreterValue rhs = VM_POP(stack);
        LilyInterpreterValue lhs = VM_POP(stack);

        INT_INST_START(add, lhs.kind);

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT8, add)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT8);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(LilyInterpreterValue,
                                int8,
                                self->check_overflow
                                  ? add_with_overflow__Int8(lhs.int8, rhs.int8)
                                  : lhs.int8 + rhs.int8));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT16, add)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT16);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(LilyInterpreterValue,
                                int16,
                                self->check_overflow ? add_with_overflow__Int16(
                                                         lhs.int16, rhs.int16)
                                                     : lhs.int16 + rhs.int16));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT32, add)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT32);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(LilyInterpreterValue,
                                int32,
                                self->check_overflow ? add_with_overflow__Int32(
                                                         lhs.int32, rhs.int32)
                                                     : lhs.int32 + rhs.int32));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT64, add)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT64);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(LilyInterpreterValue,
                                int64,
                                self->check_overflow ? add_with_overflow__Int64(
                                                         lhs.int64, rhs.int64)
                                                     : lhs.int64 + rhs.int64));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_ISIZE, add)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_ISIZE);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(LilyInterpreterValue,
                                isize,
                                self->check_overflow ? add_with_overflow__Isize(
                                                         lhs.isize, rhs.isize)
                                                     : lhs.isize + rhs.isize));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT8, add)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT8);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(LilyInterpreterValue,
                                uint8,
                                self->check_overflow ? add_with_overflow__Uint8(
                                                         lhs.uint8, rhs.uint8)
                                                     : lhs.uint8 + rhs.uint8));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT16, add)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT16);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue,
                          uint16,
                          self->check_overflow
                            ? add_with_overflow__Uint16(lhs.uint16, rhs.uint16)
                            : lhs.uint16 + rhs.uint16));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT32, add)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT32);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue,
                          uint32,
                          self->check_overflow
                            ? add_with_overflow__Uint32(lhs.uint32, rhs.uint32)
                            : lhs.uint32 + rhs.uint32));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT64, add)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT64);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue,
                          uint64,
                          self->check_overflow
                            ? add_with_overflow__Uint64(lhs.uint64, rhs.uint64)
                            : lhs.uint64 + rhs.uint64));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_USIZE, add)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_USIZE);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(LilyInterpreterValue,
                                usize,
                                self->check_overflow ? add_with_overflow__Usize(
                                                         lhs.usize, rhs.usize)
                                                     : lhs.usize + rhs.usize));

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

        push_value__LilyInterpreterVM(self, current_block_inst->icmp_eq.dest);
        push_value__LilyInterpreterVM(self, current_block_inst->icmp_eq.src);

        LilyInterpreterValue rhs = VM_POP(stack);
        LilyInterpreterValue lhs = VM_POP(stack);

        INT_INST_START(cmp_eq, lhs.kind);

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT8, cmp_eq)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT8);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.int8 == rhs.int8));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT16, cmp_eq)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT16);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.int16 == rhs.int16));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT32, cmp_eq)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT32);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.int32 == rhs.int32));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT64, cmp_eq)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT64);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.int64 == rhs.int64));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_ISIZE, cmp_eq)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_ISIZE);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.isize == rhs.isize));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT8, cmp_eq)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT8);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.uint8 == rhs.uint8));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT16, cmp_eq)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT16);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.uint16 == rhs.uint16));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT32, cmp_eq)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT32);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.uint32 == rhs.uint32));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT64, cmp_eq)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT64);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.uint64 == rhs.uint64));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_USIZE, cmp_eq)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_USIZE);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.usize == rhs.usize));

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

        push_value__LilyInterpreterVM(self, current_block_inst->icmp_ne.dest);
        push_value__LilyInterpreterVM(self, current_block_inst->icmp_ne.src);

        LilyInterpreterValue rhs = VM_POP(stack);
        LilyInterpreterValue lhs = VM_POP(stack);

        INT_INST_START(cmp_ne, lhs.kind);

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT8, cmp_ne)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT8);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.int8 != rhs.int8));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT16, cmp_ne)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT16);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.int16 != rhs.int16));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT32, cmp_ne)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT32);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.int32 != rhs.int32));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT64, cmp_ne)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT64);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.int64 != rhs.int64));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_ISIZE, cmp_ne)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_ISIZE);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.isize != rhs.isize));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT8, cmp_ne)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT8);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.uint8 != rhs.uint8));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT16, cmp_ne)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT16);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.uint16 != rhs.uint16));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT32, cmp_ne)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT32);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.uint32 != rhs.uint32));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT64, cmp_ne)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT64);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.uint64 != rhs.uint64));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_USIZE, cmp_ne)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_USIZE);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.usize != rhs.usize));

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

        push_value__LilyInterpreterVM(self, current_block_inst->icmp_le.dest);
        push_value__LilyInterpreterVM(self, current_block_inst->icmp_le.src);

        LilyInterpreterValue rhs = VM_POP(stack);
        LilyInterpreterValue lhs = VM_POP(stack);

        INT_INST_START(cmp_le, lhs.kind);

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT8, cmp_le)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT8);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.int8 <= rhs.int8));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT16, cmp_le)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT16);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.int16 <= rhs.int16));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT32, cmp_le)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT32);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.int32 <= rhs.int32));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT64, cmp_le)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT64);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.int64 <= rhs.int64));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_ISIZE, cmp_le)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_ISIZE);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.isize <= rhs.isize));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT8, cmp_le)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT8);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.uint8 <= rhs.uint8));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT16, cmp_le)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT16);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.uint16 <= rhs.uint16));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT32, cmp_le)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT32);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.uint32 <= rhs.uint32));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT64, cmp_le)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT64);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.uint64 <= rhs.uint64));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_USIZE, cmp_le)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_USIZE);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.usize <= rhs.usize));

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

        push_value__LilyInterpreterVM(self, current_block_inst->icmp_lt.dest);
        push_value__LilyInterpreterVM(self, current_block_inst->icmp_lt.src);

        LilyInterpreterValue rhs = VM_POP(stack);
        LilyInterpreterValue lhs = VM_POP(stack);

        INT_INST_START(cmp_lt, lhs.kind);

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT8, cmp_lt)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT8);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.int8 < rhs.int8));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT16, cmp_lt)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT16);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.int16 < rhs.int16));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT32, cmp_lt)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT32);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.int32 < rhs.int32));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT64, cmp_lt)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT64);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.int64 < rhs.int64));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_ISIZE, cmp_lt)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_ISIZE);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.isize < rhs.isize));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT8, cmp_lt)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT8);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.uint8 < rhs.uint8));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT16, cmp_lt)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT16);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.uint16 < rhs.uint16));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT32, cmp_lt)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT32);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.uint32 < rhs.uint32));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT64, cmp_lt)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT64);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.uint64 < rhs.uint64));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_USIZE, cmp_lt)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_USIZE);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.usize < rhs.usize));

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

        push_value__LilyInterpreterVM(self, current_block_inst->icmp_ge.dest);
        push_value__LilyInterpreterVM(self, current_block_inst->icmp_ge.src);

        LilyInterpreterValue rhs = VM_POP(stack);
        LilyInterpreterValue lhs = VM_POP(stack);

        INT_INST_START(cmp_ge, lhs.kind);

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT8, cmp_ge)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT8);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.int8 >= rhs.int8));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT16, cmp_ge)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT16);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.int16 >= rhs.int16));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT32, cmp_ge)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT32);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.int32 >= rhs.int32));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT64, cmp_ge)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT64);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.int64 >= rhs.int64));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_ISIZE, cmp_ge)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_ISIZE);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.isize >= rhs.isize));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT8, cmp_ge)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT8);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.uint8 >= rhs.uint8));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT16, cmp_ge)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT16);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.uint16 >= rhs.uint16));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT32, cmp_ge)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT32);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.uint32 >= rhs.uint32));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT64, cmp_ge)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT64);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.uint64 >= rhs.uint64));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_USIZE, cmp_ge)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_USIZE);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.usize >= rhs.usize));

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

        push_value__LilyInterpreterVM(self, current_block_inst->icmp_gt.dest);
        push_value__LilyInterpreterVM(self, current_block_inst->icmp_gt.src);

        LilyInterpreterValue rhs = VM_POP(stack);
        LilyInterpreterValue lhs = VM_POP(stack);

        INT_INST_START(cmp_gt, lhs.kind);

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT8, cmp_gt)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT8);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.int8 > rhs.int8));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT16, cmp_gt)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT16);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.int16 > rhs.int16));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT32, cmp_gt)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT32);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.int32 > rhs.int32));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT64, cmp_gt)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT64);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.int64 > rhs.int64));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_ISIZE, cmp_gt)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_ISIZE);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.isize > rhs.isize));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT8, cmp_gt)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT8);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.uint8 > rhs.uint8));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT16, cmp_gt)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT16);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.uint16 > rhs.uint16));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT32, cmp_gt)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT32);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.uint32 > rhs.uint32));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT64, cmp_gt)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT64);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.uint64 > rhs.uint64));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_USIZE, cmp_gt)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_USIZE);
#endif

            VM_PUSH(stack, NEW(LilyInterpreterValue, lhs.usize > rhs.usize));

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

        push_value__LilyInterpreterVM(self, current_block_inst->idiv.dest);
        push_value__LilyInterpreterVM(self, current_block_inst->idiv.src);

        LilyInterpreterValue rhs = VM_POP(stack);
        LilyInterpreterValue lhs = VM_POP(stack);

        INT_INST_START(div, lhs.kind);

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT8, div)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT8);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(LilyInterpreterValue,
                                int8,
                                self->check_overflow
                                  ? div_with_overflow__Int8(lhs.int8, rhs.int8)
                                  : lhs.int8 / rhs.int8));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT16, div)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT16);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(LilyInterpreterValue,
                                int16,
                                self->check_overflow ? div_with_overflow__Int16(
                                                         lhs.int16, rhs.int16)
                                                     : lhs.int16 / rhs.int16));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT32, div)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT32);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(LilyInterpreterValue,
                                int32,
                                self->check_overflow ? div_with_overflow__Int32(
                                                         lhs.int32, rhs.int32)
                                                     : lhs.int32 / rhs.int32));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT64, div)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT64);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(LilyInterpreterValue,
                                int64,
                                self->check_overflow ? div_with_overflow__Int64(
                                                         lhs.int64, rhs.int64)
                                                     : lhs.int64 / rhs.int64));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_ISIZE, div)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_ISIZE);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(LilyInterpreterValue,
                                isize,
                                self->check_overflow ? div_with_overflow__Isize(
                                                         lhs.isize, rhs.isize)
                                                     : lhs.isize / rhs.isize));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT8, div)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT8);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(LilyInterpreterValue,
                                uint8,
                                self->check_overflow ? div_with_overflow__Uint8(
                                                         lhs.uint8, rhs.uint8)
                                                     : lhs.uint8 / rhs.uint8));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT16, div)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT16);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue,
                          uint16,
                          self->check_overflow
                            ? div_with_overflow__Uint16(lhs.uint16, rhs.uint16)
                            : lhs.uint16 / rhs.uint16));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT32, div)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT32);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue,
                          uint32,
                          self->check_overflow
                            ? div_with_overflow__Uint32(lhs.uint32, rhs.uint32)
                            : lhs.uint32 / rhs.uint32));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT64, div)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT64);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue,
                          uint64,
                          self->check_overflow
                            ? div_with_overflow__Uint64(lhs.uint64, rhs.uint64)
                            : lhs.uint64 / rhs.uint64));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_USIZE, div)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_USIZE);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(LilyInterpreterValue,
                                usize,
                                self->check_overflow ? div_with_overflow__Usize(
                                                         lhs.usize, rhs.usize)
                                                     : lhs.usize / rhs.usize));

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

        push_value__LilyInterpreterVM(self, current_block_inst->imul.dest);
        push_value__LilyInterpreterVM(self, current_block_inst->imul.src);

        LilyInterpreterValue rhs = VM_POP(stack);
        LilyInterpreterValue lhs = VM_POP(stack);

        INT_INST_START(mul, lhs.kind);

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT8, mul)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT8);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(LilyInterpreterValue,
                                int8,
                                self->check_overflow
                                  ? mul_with_overflow__Int8(lhs.int8, rhs.int8)
                                  : lhs.int8 * rhs.int8));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT16, mul)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT16);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(LilyInterpreterValue,
                                int16,
                                self->check_overflow ? mul_with_overflow__Int16(
                                                         lhs.int16, rhs.int16)
                                                     : lhs.int16 * rhs.int16));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT32, mul)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT32);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(LilyInterpreterValue,
                                int32,
                                self->check_overflow ? mul_with_overflow__Int32(
                                                         lhs.int32, rhs.int32)
                                                     : lhs.int32 * rhs.int32));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT64, mul)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT64);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(LilyInterpreterValue,
                                int64,
                                self->check_overflow ? mul_with_overflow__Int64(
                                                         lhs.int64, rhs.int64)
                                                     : lhs.int64 * rhs.int64));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_ISIZE, mul)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_ISIZE);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(LilyInterpreterValue,
                                isize,
                                self->check_overflow ? mul_with_overflow__Isize(
                                                         lhs.isize, rhs.isize)
                                                     : lhs.isize * rhs.isize));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT8, mul)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT8);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(LilyInterpreterValue,
                                uint8,
                                self->check_overflow ? mul_with_overflow__Uint8(
                                                         lhs.uint8, rhs.uint8)
                                                     : lhs.uint8 * rhs.uint8));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT16, mul)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT16);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue,
                          uint16,
                          self->check_overflow
                            ? mul_with_overflow__Uint16(lhs.uint16, rhs.uint16)
                            : lhs.uint16 * rhs.uint16));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT32, mul)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT32);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue,
                          uint32,
                          self->check_overflow
                            ? mul_with_overflow__Uint32(lhs.uint32, rhs.uint32)
                            : lhs.uint32 * rhs.uint32));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT64, mul)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT64);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue,
                          uint64,
                          self->check_overflow
                            ? mul_with_overflow__Uint64(lhs.uint64, rhs.uint64)
                            : lhs.uint64 * rhs.uint64));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_USIZE, mul)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_USIZE);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(LilyInterpreterValue,
                                usize,
                                self->check_overflow ? mul_with_overflow__Usize(
                                                         lhs.usize, rhs.usize)
                                                     : lhs.usize * rhs.usize));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_DEFAULT(mul)
        {
            RUNTIME_ERROR_UNREACHABLE("expected Int... or Uint...");
        }

        INT_INST_END();
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_INCTRACE)
    {
        TODO("inctrace");
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_INEG)
    {
        INT_INST_LOOKUP(neg);

        push_value__LilyInterpreterVM(self, current_block_inst->ineg.src);

        LilyInterpreterValue rhs = VM_POP(stack);

        INT_INST_START(neg, rhs.kind);

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT8, neg)
        {
            VM_PUSH(stack, NEW_VARIANT(LilyInterpreterValue, int8, -rhs.int8));

            FREE_UVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT16, neg)
        {
            VM_PUSH(stack,
                    NEW_VARIANT(LilyInterpreterValue, int16, -rhs.int16));

            FREE_UVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT32, neg)
        {
            VM_PUSH(stack,
                    NEW_VARIANT(LilyInterpreterValue, int32, -rhs.int32));

            FREE_UVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT64, neg)
        {
            VM_PUSH(stack,
                    NEW_VARIANT(LilyInterpreterValue, int64, -rhs.int64));

            FREE_UVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_ISIZE, neg)
        {
            VM_PUSH(stack,
                    NEW_VARIANT(LilyInterpreterValue, isize, -rhs.isize));

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

        push_value__LilyInterpreterVM(self, current_block_inst->irem.dest);
        push_value__LilyInterpreterVM(self, current_block_inst->irem.src);

        LilyInterpreterValue rhs = VM_POP(stack);
        LilyInterpreterValue lhs = VM_POP(stack);

        INT_INST_START(rem, lhs.kind);

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT8, rem)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT8);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue, int8, lhs.int8 % rhs.int8));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT16, rem)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT16);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue, int16, lhs.int16 % rhs.int16));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT32, rem)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT32);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue, int32, lhs.int32 % rhs.int32));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT64, rem)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT64);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue, int64, lhs.int64 % rhs.int64));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_ISIZE, rem)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_ISIZE);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue, isize, lhs.isize % rhs.isize));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT8, rem)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT8);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue, uint8, lhs.uint8 % rhs.uint8));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT16, rem)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT16);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(
                      LilyInterpreterValue, uint16, lhs.uint16 % rhs.uint16));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT32, rem)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT32);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(
                      LilyInterpreterValue, uint32, lhs.uint32 % rhs.uint32));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT64, rem)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT64);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(
                      LilyInterpreterValue, uint64, lhs.uint64 % rhs.uint64));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_USIZE, rem)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_USIZE);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue, usize, lhs.usize % rhs.usize));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_DEFAULT(rem)
        {
            RUNTIME_ERROR_UNREACHABLE("expected Int... or Uint...");
        }

        INT_INST_END();
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_ISOK)
    {
        TODO("isok");
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_ISERR)
    {
        TODO("iserr");
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_ISUB)
    {
        INT_INST_LOOKUP(sub);

        push_value__LilyInterpreterVM(self, current_block_inst->isub.dest);
        push_value__LilyInterpreterVM(self, current_block_inst->isub.src);

        LilyInterpreterValue rhs = VM_POP(stack);
        LilyInterpreterValue lhs = VM_POP(stack);

        INT_INST_START(sub, lhs.kind);

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT8, sub)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT8);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue, int8, lhs.int8 - rhs.int8));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT16, sub)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT16);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue, int16, lhs.int16 - rhs.int16));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT32, sub)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT32);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue, int32, lhs.int32 - rhs.int32));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT64, sub)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT64);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue, int64, lhs.int64 - rhs.int64));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_ISIZE, sub)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_ISIZE);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue, isize, lhs.isize - rhs.isize));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT8, sub)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT8);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue, uint8, lhs.uint8 - rhs.uint8));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT16, sub)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT16);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(
                      LilyInterpreterValue, uint16, lhs.uint16 - rhs.uint16));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT32, sub)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT32);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(
                      LilyInterpreterValue, uint32, lhs.uint32 - rhs.uint32));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT64, sub)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT64);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(
                      LilyInterpreterValue, uint64, lhs.uint64 - rhs.uint64));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_USIZE, sub)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_USIZE);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue, usize, lhs.usize - rhs.usize));

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
        LilyMirInstructionBlock *last_block = current_block;
        LilyMirInstruction *new_current_block_inst = get__OrderedHashMap(
          current_fun_insts, (char *)current_block_inst->jmp->name);

        VM_SET_CURRENT_BLOCK(&new_current_block_inst->block);
        VM_SET_CURRENT_BLOCK_INST_ITER(
          NEW(VecIter, new_current_block_inst->block.insts));

#ifdef LILY_FULL_ASSERT_VM
        ASSERT(new_current_block_inst->block.insts->len > 0);
        ASSERT(current_frame);
#endif

        JMP_CLEAN_BLOCK_STACK();

        add_block_frame__LilyInterpreterVMStackFrame(
          current_frame,
          new_current_block_inst->block.limit->id,
          (char *)new_current_block_inst->block.name,
          stack->len);

        // Run the block.
        return;
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_JMP_COND)
    {
        push_value__LilyInterpreterVM(self, current_block_inst->jmpcond.cond);

        LilyInterpreterValue cond = VM_POP(stack);
        LilyMirInstructionBlock *last_block = current_block;
        LilyMirInstruction *new_current_block_inst = NULL;

#ifdef LILY_FULL_ASSERT_VM
        ASSERT(cond.kind == LILY_INTERPRETER_VALUE_KIND_TRUE ||
               cond.kind == LILY_INTERPRETER_VALUE_KIND_FALSE);
#endif

        if (cond.kind) {
            new_current_block_inst = get__OrderedHashMap(
              current_fun_insts,
              (char *)current_block_inst->jmpcond.then_block->name);
        } else {
            new_current_block_inst = get__OrderedHashMap(
              current_fun_insts,
              (char *)current_block_inst->jmpcond.else_block->name);
        }

        FREE(LilyInterpreterValue, &cond);

        VM_SET_CURRENT_BLOCK(&new_current_block_inst->block);
        VM_SET_CURRENT_BLOCK_INST_ITER(
          NEW(VecIter, new_current_block_inst->block.insts));

#ifdef LILY_FULL_ASSERT_VM
        ASSERT(new_current_block_inst->block.insts->len > 0);
        ASSERT(current_frame);
#endif

        JMP_CLEAN_BLOCK_STACK();

        add_block_frame__LilyInterpreterVMStackFrame(
          current_frame,
          new_current_block_inst->block.limit->id,
          (char *)new_current_block_inst->block.name,
          stack->len);

        // Run the block.
        return;
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_LEN)
    {
        TODO("len");
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_LOAD)
    {
        push_value__LilyInterpreterVM(self, current_block_inst->load.src.src);

        EAT_NEXT_LABEL();
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_MAKEREF)
    {
        TODO("makeref");
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_MAKEOPT)
    {
        TODO("makeopt");
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_NON_NIL)
    {
        TODO("non nil");
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_NOT)
    {
        push_value__LilyInterpreterVM(self, current_block_inst->not.src);

        LilyInterpreterValue rhs = VM_POP(stack);

#ifdef LILY_FULL_ASSERT_VM
        ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_TRUE ||
               rhs.kind == LILY_INTERPRETER_VALUE_KIND_FALSE);
#endif

        VM_PUSH(stack, NEW(LilyInterpreterValue, !rhs.kind));

        FREE_UVAL();
        EAT_NEXT_LABEL();
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_REF_PTR)
    {
        TODO("ref ptr");
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_REG)
    {
        char *reg_name = (char *)current_block_inst->reg.name;

        SET_NEXT_LABEL(reg_finish);

        VM_SET_CURRENT_BLOCK_INST(current_block_inst->reg.inst);
        VM_GOTO_INST(current_block_inst);

    reg_finish: {
        add_reg__LilyInterpreterVMStackBlockFrame(
          current_block_frame, reg_name, VM_PEEK(stack));
        EAT_NEXT_LABEL();
    }
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_RET)
    {
        SET_NEXT_LABEL(ret_finish);

        VM_SET_CURRENT_BLOCK_INST(current_block_inst->ret);
        VM_GOTO_INST(current_block_inst);

    ret_finish: {
        LilyInterpreterValue ret_value = VM_POP(stack);

        ASSERT(current_frame);
        set_return__LilyInterpreterVMStackFrame(
          current_frame,
          NEW_VARIANT(LilyInterpreterVMStackFrameReturn, normal, ret_value));

        CLEAN_BLOCK_STACK(current_block->limit->id);
        EAT_NEXT_LABEL();
    }
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_SHL)
    {
        INT_INST_LOOKUP(shl);

        push_value__LilyInterpreterVM(self, current_block_inst->shl.dest);
        push_value__LilyInterpreterVM(self, current_block_inst->shl.src);

        LilyInterpreterValue rhs = VM_POP(stack);
        LilyInterpreterValue lhs = VM_POP(stack);

        INT_INST_START(shl, lhs.kind);

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT8, shl)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT8);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue, int8, lhs.int8 << rhs.int8));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT16, shl)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT16);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue, int16, lhs.int16 << rhs.int16));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT32, shl)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT32);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue, int32, lhs.int32 << rhs.int32));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT64, shl)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT64);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue, int64, lhs.int64 << rhs.int64));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_ISIZE, shl)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_ISIZE);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue, isize, lhs.isize << rhs.isize));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT8, shl)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT8);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue, uint8, lhs.uint8 << rhs.uint8));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT16, shl)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT16);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(
                      LilyInterpreterValue, uint16, lhs.uint16 << rhs.uint16));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT32, shl)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT32);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(
                      LilyInterpreterValue, uint32, lhs.uint32 << rhs.uint32));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT64, shl)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT64);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(
                      LilyInterpreterValue, uint64, lhs.uint64 << rhs.uint64));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_USIZE, shl)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_USIZE);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue, usize, lhs.usize << rhs.usize));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_DEFAULT(shl)
        {
            RUNTIME_ERROR_UNREACHABLE("expected Int... or Uint...");
        }

        INT_INST_END();
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_SHR)
    {
        INT_INST_LOOKUP(shr);

        push_value__LilyInterpreterVM(self, current_block_inst->shr.dest);
        push_value__LilyInterpreterVM(self, current_block_inst->shr.src);

        LilyInterpreterValue rhs = VM_POP(stack);
        LilyInterpreterValue lhs = VM_POP(stack);

        INT_INST_START(shr, lhs.kind);

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT8, shr)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT8);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue, int8, lhs.int8 >> rhs.int8));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT16, shr)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT16);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue, int16, lhs.int16 >> rhs.int16));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT32, shr)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT32);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue, int32, lhs.int32 >> rhs.int32));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT64, shr)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT64);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue, int64, lhs.int64 >> rhs.int64));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_ISIZE, shr)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_ISIZE);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue, isize, lhs.isize >> rhs.isize));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT8, shr)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT8);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue, uint8, lhs.uint8 >> rhs.uint8));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT16, shr)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT16);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(
                      LilyInterpreterValue, uint16, lhs.uint16 >> rhs.uint16));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT32, shr)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT32);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(
                      LilyInterpreterValue, uint32, lhs.uint32 >> rhs.uint32));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT64, shr)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT64);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(
                      LilyInterpreterValue, uint64, lhs.uint64 >> rhs.uint64));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_USIZE, shr)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_USIZE);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue, usize, lhs.usize >> rhs.usize));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_DEFAULT(shr)
        {
            RUNTIME_ERROR_UNREACHABLE("expected Int... or Uint...");
        }

        INT_INST_END();
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_STORE)
    {
        push_value__LilyInterpreterVM(self, current_block_inst->store.src);

        LilyInterpreterValue src_value = VM_POP(stack);
        LilyInterpreterValue *dest_value = NULL;
        bool raw_value = true;

        switch (current_block_inst->store.src->kind) {
            case LILY_MIR_INSTRUCTION_VAL_KIND_VAR:
            case LILY_MIR_INSTRUCTION_VAL_KIND_CONST:
            case LILY_MIR_INSTRUCTION_VAL_KIND_REG:
                raw_value = false;
                break;
            default:
                break;
        }

        switch (current_block_inst->store.dest->kind) {
            case LILY_MIR_INSTRUCTION_VAL_KIND_VAR:
                dest_value = search_variable__LilyInterpreterVMStackBlockFrame(
                  current_block_frame,
                  (char *)current_block_inst->store.dest->var);
                break;
            default:
                UNREACHABLE("expected a valid destination value, like var");
        }

#ifdef LILY_FULL_ASSERT_VM
        ASSERT(current_block_inst->store.dest->kind ==
               LILY_MIR_INSTRUCTION_VAL_KIND_VAR)
        ASSERT(dest_value);
#endif

        store__LilyInterpreterValue(dest_value, &src_value, raw_value);

        // NOTE: Decrement the ref_count of the destination value.
        FREE(LilyInterpreterValue, dest_value);

        if (raw_value) {
            minimal_free__LilyInterpreterValue(&src_value);
        } else {
            FREE(LilyInterpreterValue, &src_value);
        }

        EAT_NEXT_LABEL();
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_STRUCT)
    {
        TODO("struct");
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_SWITCH)
    {
        TODO("switch");
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_SYS_CALL)
    {
        for (Usize i = 0; i < current_block_inst->sys_call.params->len; ++i) {
            push_value__LilyInterpreterVM(
              self, get__Vec(current_block_inst->sys_call.params, i));
        }

        // TODO: maybe optimized that, replace sys call name by an id, or
        // something like that
        if (!strcmp(current_block_inst->sys_call.name, "__sys__$read")) {
            LilyInterpreterValue n = VM_POP(stack);
            LilyInterpreterValue buf = VM_POP(stack);
            LilyInterpreterValue fd = VM_POP(stack);

            VM_PUSH(stack, read__LilyInterpreterVMRuntimeSys(fd, buf, n));
        } else if (!strcmp(current_block_inst->sys_call.name,
                           "__sys__$write")) {
            LilyInterpreterValue n = VM_POP(stack);
            LilyInterpreterValue buf = VM_POP(stack);
            LilyInterpreterValue fd = VM_POP(stack);

            VM_PUSH(stack, write__LilyInterpreterVMRuntimeSys(fd, buf, n));
        } else if (!strcmp(current_block_inst->sys_call.name, "__sys__$open")) {
            LilyInterpreterValue mode = VM_POP(stack);
            LilyInterpreterValue flags = VM_POP(stack);
            LilyInterpreterValue pathname = VM_POP(stack);

            VM_PUSH(stack,
                    open__LilyInterpreterVMRuntimeSys(pathname, flags, mode));
        } else if (!strcmp(current_block_inst->sys_call.name,
                           "__sys__$close")) {
            LilyInterpreterValue fd = VM_POP(stack);

            VM_PUSH(stack, close__LilyInterpreterVMRuntimeSys(fd));
        } else if (!strcmp(current_block_inst->sys_call.name,
                           "__sys__$stat_mode")) {
            LilyInterpreterValue pathname = VM_POP(stack);

            VM_PUSH(stack, stat_mode__LilyInterpreterVMRuntimeSys(pathname));
        } else if (!strcmp(current_block_inst->sys_call.name,
                           "__sys__$stat_ino")) {
            LilyInterpreterValue pathname = VM_POP(stack);

            VM_PUSH(stack, stat_ino__LilyInterpreterVMRuntimeSys(pathname));
        } else if (!strcmp(current_block_inst->sys_call.name,
                           "__sys__$stat_dev")) {
            LilyInterpreterValue pathname = VM_POP(stack);

            VM_PUSH(stack, stat_dev__LilyInterpreterVMRuntimeSys(pathname));
        } else if (!strcmp(current_block_inst->sys_call.name,
                           "__sys__$stat_nlink")) {
            LilyInterpreterValue pathname = VM_POP(stack);

            VM_PUSH(stack, stat_nlink__LilyInterpreterVMRuntimeSys(pathname));
        } else if (!strcmp(current_block_inst->sys_call.name,
                           "__sys__$stat_uid")) {
            LilyInterpreterValue pathname = VM_POP(stack);

            VM_PUSH(stack, stat_uid__LilyInterpreterVMRuntimeSys(pathname));
        } else if (!strcmp(current_block_inst->sys_call.name,
                           "__sys__$stat_gid")) {
            LilyInterpreterValue pathname = VM_POP(stack);

            VM_PUSH(stack, stat_gid__LilyInterpreterVMRuntimeSys(pathname));
        } else if (!strcmp(current_block_inst->sys_call.name,
                           "__sys__$stat_size")) {
            LilyInterpreterValue pathname = VM_POP(stack);

            VM_PUSH(stack, stat_size__LilyInterpreterVMRuntimeSys(pathname));
        } else if (!strcmp(current_block_inst->sys_call.name,
                           "__sys__$stat_atime")) {
            LilyInterpreterValue pathname = VM_POP(stack);

            VM_PUSH(stack, stat_atime__LilyInterpreterVMRuntimeSys(pathname));
        } else if (!strcmp(current_block_inst->sys_call.name,
                           "__sys__$stat_mtime")) {
            LilyInterpreterValue pathname = VM_POP(stack);

            VM_PUSH(stack, stat_mtime__LilyInterpreterVMRuntimeSys(pathname));
        } else if (!strcmp(current_block_inst->sys_call.name,
                           "__sys__$stat_ctime")) {
            LilyInterpreterValue pathname = VM_POP(stack);

            VM_PUSH(stack, stat_ctime__LilyInterpreterVMRuntimeSys(pathname));
        } else {
            UNREACHABLE("unknown sys call");
        }

        EAT_NEXT_LABEL();
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_TRUNC)
    {
        TODO("trunc");
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_TRY)
    {
        TODO("try");
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_TRY_PTR)
    {
        TODO("try ptr");
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_UNREACHABLE)
    {
        abort();
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_VAL)
    {
        push_value__LilyInterpreterVM(self, current_block_inst->val);

        if (!next_label) {
            LilyInterpreterValue value = VM_POP(stack);

            FREE(LilyInterpreterValue, &value);
        }

        EAT_NEXT_LABEL();
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_VAR)
    {
        char *name = current_block_inst->var.name;

        LilyInterpreterValue value =
          NEW(LilyInterpreterValue, LILY_INTERPRETER_VALUE_KIND_UNDEF);

        VM_PUSH(stack, value);
        add_variable__LilyInterpreterVMStackBlockFrame(
          current_block_frame, name, VM_PEEK(stack));

        SET_NEXT_LABEL(var_finish);

        VM_SET_CURRENT_BLOCK_INST(current_block_inst->var.inst);
        VM_GOTO_INST(current_block_inst);

    var_finish: {
        EAT_NEXT_LABEL();
    }
    }

    VM_INST(LILY_MIR_INSTRUCTION_KIND_XOR)
    {
        INT_INST_LOOKUP(xor);

        push_value__LilyInterpreterVM(self, current_block_inst->xor.dest);
        push_value__LilyInterpreterVM(self, current_block_inst->xor.src);

        LilyInterpreterValue rhs = VM_POP(stack);
        LilyInterpreterValue lhs = VM_POP(stack);

        INT_INST_START(xor, lhs.kind);

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT8, xor)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT8);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue, int8, lhs.int8 ^ rhs.int8));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT16, xor)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT16);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue, int16, lhs.int16 ^ rhs.int16));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT32, xor)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT32);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue, int32, lhs.int32 ^ rhs.int32));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_INT64, xor)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_INT64);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue, int64, lhs.int64 ^ rhs.int64));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_ISIZE, xor)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_ISIZE);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue, isize, lhs.isize ^ rhs.isize));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT8, xor)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT8);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue, uint8, lhs.uint8 ^ rhs.uint8));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT16, xor)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT16);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(
                      LilyInterpreterValue, uint16, lhs.uint16 ^ rhs.uint16));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT32, xor)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT32);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(
                      LilyInterpreterValue, uint32, lhs.uint32 ^ rhs.uint32));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_UINT64, xor)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_UINT64);
#endif

            VM_PUSH(stack,
                    NEW_VARIANT(
                      LilyInterpreterValue, uint64, lhs.uint64 ^ rhs.uint64));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_TYPE(LILY_INTERPRETER_VALUE_KIND_USIZE, xor)
        {
#ifdef LILY_FULL_ASSERT_VM
            ASSERT(rhs.kind == LILY_INTERPRETER_VALUE_KIND_USIZE);
#endif

            VM_PUSH(
              stack,
              NEW_VARIANT(LilyInterpreterValue, usize, lhs.usize ^ rhs.usize));

            FREE_BVAL();
            EAT_NEXT_LABEL();
        }

        INT_INST_DEFAULT(xor)
        {
            RUNTIME_ERROR_UNREACHABLE("expected Int... or Uint...");
        }

        INT_INST_END();
    }

    VM_DEFAULT()
    {
        UNREACHABLE("unknown instruction");
    }

    VM_END();
}

void
run_insts__LilyInterpreterVM(LilyInterpreterVM *self)
{
run: {
    run_inst__LilyInterpreterVM(self);
    VM_NEXT_INST(run, exit);
}

exit: {
    return;
}
}

void
set_max_stack__LilyInterpreterVM(Usize max_stack)
{
    local_stack.max_capacity =
      max_stack == 0 ? DEFAULT_MAX_STACK_CAPACITY : max_stack;
}

void
run__LilyInterpreterVM(LilyInterpreterVM *self)
{
    // TODO: Instead of call `run_inst__*`, execute all instructions and the VM
    // in one function.
run_vm: {
    run_inst__LilyInterpreterVM(self);
    VM_NEXT_INST(run_vm, exit_vm);
}

exit_vm: {
    // Clean up

    FREE(LilyInterpreterVMStack, &local_stack);
}
}

DESTRUCTOR(LilyInterpreterVM, const LilyInterpreterVM *self)
{
    FREE(LilyInterpreterVMResources, &self->resources);
    FREE(LilyInterpreterVMStackFrameReturn, &current_frame->return_);
    FREE(LilyInterpreterVMStackFrame, &current_frame);
}
