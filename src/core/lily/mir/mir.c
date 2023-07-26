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

#include <base/format.h>
#include <base/string.h>

#include <core/lily/mir/mir.h>

#include <string.h>

DESTRUCTOR(LilyMirNameManager, const LilyMirNameManager *self)
{
    for (Usize i = 0; i < self->names->len; ++i) {
        lily_free(get__Vec(self->names, i));
    }

    FREE(Vec, self->names);
}

DESTRUCTOR(LilyMirCurrentFun, const LilyMirCurrentFun *self)
{
    FREE(LilyMirNameManager, &self->block_manager);
    FREE(LilyMirNameManager, &self->reg_manager);
}

void
LilyMirAddInst(LilyMirModule *Module, LilyMirInstruction *Inst)
{
    switch (Inst->kind) {
        case LILY_MIR_INSTRUCTION_KIND_CONST:
            push__Stack(Module->current,
                        NEW_VARIANT(LilyMirCurrent, const, Inst));
            break;
        case LILY_MIR_INSTRUCTION_KIND_FUN:
            push__Stack(
              Module->current,
              NEW_VARIANT(LilyMirCurrent, fun, NEW(LilyMirCurrentFun, Inst)));
            break;
        case LILY_MIR_INSTRUCTION_KIND_STRUCT:
            push__Stack(Module->current,
                        NEW_VARIANT(LilyMirCurrent, struct, Inst));
            break;
        default: {
            LilyMirCurrent *current = Module->current->top;

            if (!current) {
                ASSERT("cannot push this instrution in global");
            }

            switch (current->kind) {
                case LILY_MIR_CURRENT_KIND_FUN: {
                    LilyMirInstructionBlock *block =
                      current->fun.fun->fun.block_stack->top;

                    ASSERT(block);

                    push__Vec(block->insts, Inst);

                    break;
                }
                default:
                    ASSERT("cannot push this instruction in this instruction");
            }
        }
    }
}

VARIANT_CONSTRUCTOR(LilyMirCurrent *,
                    LilyMirCurrent,
                    const,
                    LilyMirInstruction *const_)
{
    LilyMirCurrent *self = lily_malloc(sizeof(LilyMirCurrent));

    self->kind = LILY_MIR_CURRENT_KIND_CONST;
    self->const_ = const_;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirCurrent *,
                    LilyMirCurrent,
                    fun,
                    LilyMirCurrentFun fun)
{
    LilyMirCurrent *self = lily_malloc(sizeof(LilyMirCurrent));

    self->kind = LILY_MIR_CURRENT_KIND_FUN;
    self->fun = fun;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirCurrent *,
                    LilyMirCurrent,
                    struct,
                    LilyMirInstruction *struct_)
{
    LilyMirCurrent *self = lily_malloc(sizeof(LilyMirCurrent));

    self->kind = LILY_MIR_CURRENT_KIND_STRUCT;
    self->struct_ = struct_;

    return self;
}

DESTRUCTOR(LilyMirCurrent, LilyMirCurrent *self)
{
    switch (self->kind) {
        case LILY_MIR_CURRENT_KIND_FUN:
            FREE(LilyMirCurrentFun, &self->fun);
            lily_free(self);
            break;
        default:
            lily_free(self);
    }
}

void
LilyMirPopCurrent(LilyMirModule *Module)
{
    if (Module->current->len == 0) {
        return;
    }

    LilyMirCurrent *current = pop__Stack(Module->current);

    switch (current->kind) {
        case LILY_MIR_CURRENT_KIND_CONST:
            insert__OrderedHashMap(Module->insts,
                                   (char *)current->const_->const_.name,
                                   current->const_);
            break;
        case LILY_MIR_CURRENT_KIND_FUN:
            insert__OrderedHashMap(Module->insts,
                                   (char *)current->fun.fun->fun.name,
                                   current->fun.fun);
            break;
        case LILY_MIR_CURRENT_KIND_STRUCT:
            insert__OrderedHashMap(Module->insts,
                                   (char *)current->struct_->struct_.name,
                                   current->struct_);
            break;
        default:
            UNREACHABLE("unknown variant");
    }

    FREE(LilyMirCurrent, current);
}

LilyMirScopeVar *
LilyMirScopeGetVar(const LilyMirScope *Scope, String *name)
{
    for (Usize i = 0; i < Scope->vars->len; ++i) {
        LilyMirScopeVar *var = get__Vec(Scope->vars, i);

        if (!strcmp(var->name->buffer, name->buffer)) {
            return var;
        }
    }

    if (Scope->parent) {
        return LilyMirScopeGetVar(Scope->parent, name);
    }

    UNREACHABLE("the analysis has a bug!!");
}

LilyMirScopeParam *
LilyMirScopeGetParam(const LilyMirModule *Module, Usize id)
{
    LilyMirCurrent *current = Module->current->top;

    ASSERT(current->kind == LILY_MIR_CURRENT_KIND_FUN);

    return get__Vec(current->fun.fun->fun.scope.params, id);
}

LilyCheckedDataType *
LilyMirGetCheckedDtFromExpr(const LilyMirModule *Module,
                            const LilyMirScope *Scope,
                            const LilyCheckedExpr *Expr)
{
    switch (Expr->kind) {
        case LILY_CHECKED_EXPR_KIND_CALL:
            switch (Expr->call.kind) {
                case LILY_CHECKED_EXPR_CALL_KIND_FUN_PARAM:
                    return LilyMirScopeGetParam(Module, Expr->call.fun_param)
                      ->data_type;
                case LILY_CHECKED_EXPR_CALL_KIND_VARIABLE:
                    return LilyMirScopeGetVar(Scope, Expr->call.global_name)
                      ->data_type;
                default:
                    return Expr->data_type;
            }
        default:
            return Expr->data_type;
    }
}

LilyMirInstruction *
LilyMirBuildReg(LilyMirModule *Module, LilyMirInstruction *Inst)
{
    LilyMirCurrent *current = Module->current->top;

    ASSERT(current->kind == LILY_MIR_CURRENT_KIND_FUN);

    char *name = LilyMirGenerateName(&current->fun.reg_manager);

    return NEW_VARIANT(LilyMirInstruction,
                       reg,
                       NEW(LilyMirInstructionReg, from__String(name), Inst));
}

LilyMirInstructionVal *
LilyMirBuildLoad(LilyMirModule *Module,
                 LilyMirInstructionVal *src,
                 LilyMirDt *dt,
                 String *value_name)
{
    LilyMirCurrent *current = Module->current->top;

    ASSERT(current->kind == LILY_MIR_CURRENT_KIND_FUN);

    LilyMirInstructionFunLoad *matched_load = NULL;

    for (Usize i = 0; i < current->fun.fun->fun.scope.loads->len; ++i) {
        LilyMirInstructionFunLoad *load =
          get__Vec(current->fun.fun->fun.scope.loads, i);

        if (!strcmp(load->value_name->buffer, value_name->buffer)) {
            matched_load = load;
        }
    }

    if (matched_load) {
        // assume: %<name> = <val_inst>
        FREE(LilyMirInstructionVal, src);
        FREE(LilyMirDt, dt);

        return NEW_VARIANT(
          LilyMirInstructionVal,
          reg,
          clone__LilyMirDt(matched_load->inst->reg.inst->load.dt),
          clone__String(matched_load->inst->reg.name));
    }

    char *name = LilyMirGenerateName(&current->fun.reg_manager);

    LilyMirInstruction *load_inst =
      NEW_VARIANT(LilyMirInstruction,
                  reg,
                  NEW(LilyMirInstructionReg,
                      from__String(name),
                      NEW_VARIANT(LilyMirInstruction,
                                  load,
                                  NEW(LilyMirInstructionLoad,
                                      NEW(LilyMirInstructionSrc, src),
                                      clone__LilyMirDt(dt)))));

    push__Vec(current->fun.fun->fun.scope.loads,
              NEW(LilyMirInstructionFunLoad,
                  value_name,
                  load_inst,
                  LilyMirGetBlockId(Module)));

    LilyMirAddInst(Module, load_inst);

    return NEW_VARIANT(LilyMirInstructionVal, reg, dt, from__String(name));
}

LilyMirInstruction *
LilyMirBuildVar(LilyMirModule *Module,
                char *name,
                LilyMirDt *dt,
                LilyMirInstruction *inst)
{
    LilyMirAddInst(
      Module,
      NEW_VARIANT(
        LilyMirInstruction,
        var,
        NEW(LilyMirInstructionVar, name, LilyMirBuildAlloc(Module, dt))));

    LilyMirInstruction *var = LilyMirBuildStore(
      NEW_VARIANT(LilyMirInstructionVal, var, clone__LilyMirDt(dt), name),
      inst->val);

    lily_free(inst);

    return var;
}

LilyMirInstruction *
LilyMirBuildCall(LilyMirModule *Module,
                 LilyMirDt *ReturnDt,
                 const char *Name,
                 Vec *Params)
{
    LilyMirCurrent *current = Module->current->top;

    ASSERT(current->kind == LILY_MIR_CURRENT_KIND_FUN);

    LilyMirInstruction *inst = NULL;

    if (ReturnDt->kind == LILY_MIR_DT_KIND_UNIT) {
        inst = NEW_VARIANT(
          LilyMirInstruction,
          call,
          NEW(
            LilyMirInstructionCall, clone__LilyMirDt(ReturnDt), Name, Params));

        LilyMirAddInst(Module, inst);

        return NULL;
    }

    char *name = LilyMirGenerateName(&current->fun.reg_manager);

    inst = NEW_VARIANT(LilyMirInstruction,
                       reg,
                       NEW(LilyMirInstructionReg,
                           from__String(name),
                           NEW_VARIANT(LilyMirInstruction,
                                       call,
                                       NEW(LilyMirInstructionCall,
                                           clone__LilyMirDt(ReturnDt),
                                           Name,
                                           Params))));

    LilyMirAddInst(Module, inst);

    return NEW_VARIANT(
      LilyMirInstruction,
      val,
      NEW_VARIANT(LilyMirInstructionVal, reg, ReturnDt, from__String(name)));
}

void
LilyMirDisposeModule(const LilyMirModule *Module)
{
    FREE_ORD_HASHMAP_VALUES(Module->insts, LilyMirInstruction);
    FREE(OrderedHashMap, Module->insts);
    FREE_STACK_ITEMS(Module->current, LilyMirCurrent);
    FREE(Stack, Module->current);
}

void
LilyMirNextBlockAndClearScope(LilyMirModule *Module, LilyMirScope *Scope)
{
    ASSERT(Module->current->len > 0);

    LilyMirCurrent *current = Module->current->top;

    ASSERT(current->kind == LILY_MIR_CURRENT_KIND_FUN);

    pop__Stack(current->fun.fun->fun.block_stack);

    FREE(LilyMirScope, Scope);
}

LilyMirInstruction *
LilyMirBuildBlock(LilyMirModule *Module)
{
    LilyMirCurrent *current = Module->current->top;

    ASSERT(current->kind == LILY_MIR_CURRENT_KIND_FUN);

    char *name = LilyMirGenerateName(&current->fun.block_manager);

    return NEW_VARIANT(LilyMirInstruction,
                       block,
                       NEW(LilyMirInstructionBlock,
                           from__String(name),
                           current->fun.fun->fun.block_count++));
}

void
LilyMirAddBlock(LilyMirModule *Module, LilyMirInstruction *Block)
{
    LilyMirCurrent *current = Module->current->top;

    ASSERT(current->kind == LILY_MIR_CURRENT_KIND_FUN);

    push__Vec(current->fun.fun->fun.insts, Block);
    push__Stack(current->fun.fun->fun.block_stack, &Block->block);
}

LilyMirInstructionVal *
LilyMirGetValFromInst(LilyMirInstruction *inst)
{
    switch (inst->kind) {
        case LILY_MIR_INSTRUCTION_KIND_VAL:
            return inst->val;
        default:
            return NULL;
    }
}

bool
LilyMirValidTypesOfFun(LilyMirInstructionFun *Fun, LilyMirDt **types, Usize len)
{
    for (Usize i = 0; i < Fun->args->len; ++i) {
        LilyMirInstruction *arg = get__Vec(Fun->args, i);

        if (!eq__LilyMirDt(arg->arg.dt, types[i])) {
            return false;
        }
    }

    return eq__LilyMirDt(Fun->return_data_type, types[len - 1]);
}

const char *
LilyMirGetFunNameFromTypes(LilyMirModule *Module,
                           const char *BaseName,
                           LilyMirDt **Types,
                           Usize Len)
{
    {
        OrderedHashMapIter insts_iter = NEW(OrderedHashMapIter, Module->insts);
        LilyMirInstruction *current = NULL;

        while ((current = next__OrderedHashMapIter(&insts_iter))) {
            switch (current->kind) {
                case LILY_MIR_INSTRUCTION_KIND_FUN:
                    if (!strcmp(BaseName, current->fun.base_name) &&
                        LilyMirValidTypesOfFun(&current->fun, Types, Len)) {
                        return current->fun.name;
                    }

                    break;
                default:
                    continue;
            }
        }
    }

    // Search in the stack (for unpushed function to the insts field)
    if (Module->current->len > 0) {
        LilyMirInstruction *top = Module->current->top;

        if (top) {
            if (!strcmp(BaseName, top->fun.base_name) &&
                LilyMirValidTypesOfFun(&top->fun, Types, Len)) {
                return top->fun.name;
            }
        }

        for (Usize i = 0; i < Module->current->len - 1; ++i) {
            LilyMirInstruction *current = Module->current->buffer[i];

            switch (current->kind) {
                case LILY_MIR_INSTRUCTION_KIND_FUN:
                    if (!strcmp(BaseName, current->fun.base_name) &&
                        LilyMirValidTypesOfFun(&current->fun, Types, Len)) {
                        return current->fun.name;
                    }

                    break;
                default:
                    continue;
            }
        }
    }

    UNREACHABLE("the analysis has a bug!!");
}

char *
LilyMirGenerateName(LilyMirNameManager *NameManager)
{
    char *name = format("{s}{d}", NameManager->base_name, NameManager->count);

    push__Vec(NameManager->names, name);

    ++NameManager->count;

    return name;
}
