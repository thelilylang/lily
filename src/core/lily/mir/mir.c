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

#include <base/format.h>
#include <base/string.h>

#include <core/lily/analysis/checked/body/fun.h>
#include <core/lily/mir/generator/body.h>
#include <core/lily/mir/generator/case_value.h>
#include <core/lily/mir/generator/expr.h>
#include <core/lily/mir/generator/stmt.h>
#include <core/lily/mir/mir.h>

#include <string.h>

void
LilyMirAddInst(LilyMirModule *Module, LilyMirInstruction *Inst)
{
    switch (Inst->kind) {
        case LILY_MIR_INSTRUCTION_KIND_CONST:
            push__Stack(Module->current,
                        NEW_VARIANT(LilyMirCurrent, const, Inst));
            break;
        case LILY_MIR_INSTRUCTION_KIND_FUN:
            push__Stack(Module->current,
                        NEW_VARIANT(LilyMirCurrent, fun, Inst));
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
                      current->inst->fun.block_stack->top;

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
    self->inst = const_;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirCurrent *,
                    LilyMirCurrent,
                    fun,
                    LilyMirInstruction *fun)
{
    LilyMirCurrent *self = lily_malloc(sizeof(LilyMirCurrent));

    self->kind = LILY_MIR_CURRENT_KIND_FUN;
    self->inst = fun;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirCurrent *,
                    LilyMirCurrent,
                    struct,
                    LilyMirInstruction *struct_)
{
    LilyMirCurrent *self = lily_malloc(sizeof(LilyMirCurrent));

    self->kind = LILY_MIR_CURRENT_KIND_STRUCT;
    self->inst = struct_;

    return self;
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
            insert__OrderedHashMap(
              Module->insts, (char *)current->inst->const_.name, current->inst);
            break;
        case LILY_MIR_CURRENT_KIND_FUN:
            insert__OrderedHashMap(
              Module->insts, (char *)current->inst->fun.name, current->inst);
            break;
        case LILY_MIR_CURRENT_KIND_STRUCT:
            insert__OrderedHashMap(Module->insts,
                                   (char *)current->inst->struct_.name,
                                   current->inst);
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
    ASSERT(current->inst->fun.root_scope->kind == LILY_MIR_SCOPE_KIND_ROOT);

    return get__Vec(current->inst->fun.root_scope->params, id);
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

void
add_scope__LilyMirScope(LilyMirScope **self, LilyMirBlockLimit *limit)
{
    *self = NEW(LilyMirScope, LILY_MIR_SCOPE_KIND_NORMAL, limit, *self);
}

void
remove_scope__LilyMirScope(LilyMirScope **self)
{
    LilyMirScope *parent = (*self)->parent;

    FREE(LilyMirScope, *self);

    *self = parent;
}

LilyMirInstructionFunLoad *
search_load__LilyMirScope(const LilyMirScope *self,
                          LilyMirInstructionFunLoadName name)
{
    for (Usize i = 0; i < self->loads->len; ++i) {
        LilyMirInstructionFunLoad *load = get__Vec(self->loads, i);

        if (eq__LilyMirInstructionFunLoadName(&load->value_name, &name)) {
            return load;
        }
    }

    return NULL;
}

void
remove_load__LilyMirScope(const LilyMirScope *self,
                          LilyMirInstructionFunLoadName name)
{
    for (Usize i = 0; i < self->loads->len; ++i) {
        LilyMirInstructionFunLoad *load = get__Vec(self->loads, i);

        if (eq__LilyMirInstructionFunLoadName(&load->value_name, &name)) {
            FREE(LilyMirInstructionFunLoad, remove__Vec(self->loads, i));

            return;
        }
    }
}

void
LilyMirRemoveScopeByLimit(LilyMirModule *Module, const LilyMirBlockLimit *limit)
{
    LilyMirCurrent *current = Module->current->top;

    ASSERT(current->kind == LILY_MIR_CURRENT_KIND_FUN);

    LilyMirScope **current_scope = &current->inst->fun.scope;
    LilyMirScope *prev_scope = NULL;

    while (*current_scope) {
        if ((*current_scope)->limit == limit) {
            remove_scope__LilyMirScope(current_scope);

            if (prev_scope && *current_scope) {
                prev_scope->parent = *current_scope;
            }
        } else {
            prev_scope = *current_scope;
            current_scope = &(*current_scope)->parent;
        }
    }
}

LilyMirInstructionFunLoad *
LilyMirSearchLoad(LilyMirModule *Module, LilyMirInstructionFunLoadName name)
{
    LilyMirCurrent *current = LilyMirGetCurrentOnTop(Module);

    ASSERT(current);
    ASSERT(current->kind == LILY_MIR_CURRENT_KIND_FUN);

    return search_load__LilyMirScope(current->inst->fun.scope, name);
}

void
LilyMirRemoveLoad(LilyMirModule *Module, LilyMirInstructionFunLoadName name)
{
    LilyMirCurrent *current = LilyMirGetCurrentOnTop(Module);

    ASSERT(current);
    ASSERT(current->kind == LILY_MIR_CURRENT_KIND_FUN);

    return remove_load__LilyMirScope(current->inst->fun.scope, name);
}

LilyMirInstruction *
LilyMirBuildReg(LilyMirModule *Module, LilyMirInstruction *Inst)
{
    LilyMirCurrent *current = Module->current->top;

    ASSERT(current->kind == LILY_MIR_CURRENT_KIND_FUN);

    char *name = LilyMirGenerateName(&current->inst->fun.reg_manager);

    return NEW_VARIANT(
      LilyMirInstruction, reg, NEW(LilyMirInstructionReg, name, Inst));
}

LilyMirInstruction *
LilyMirBuildJmpCondWithInst(LilyMirModule *Module,
                            LilyMirInstruction *Cond,
                            LilyMirInstruction *ThenBlock,
                            LilyMirInstruction *ElseBlock)
{
    ASSERT(Cond->kind == LILY_MIR_INSTRUCTION_KIND_VAL);

    if (Cond->val->dt->kind == LILY_MIR_DT_KIND_I1) {
        return NEW_VARIANT(LilyMirInstruction,
                           jmpcond,
                           NEW(LilyMirInstructionJmpCond,
                               Cond->val,
                               &ThenBlock->block,
                               &ElseBlock->block));
    } else if (Cond->val->dt->kind == LILY_MIR_DT_KIND_PTR &&
               Cond->val->dt->ptr->kind == LILY_MIR_DT_KIND_I1) {
        return NEW_VARIANT(
          LilyMirInstruction,
          jmpcond,
          NEW(
            LilyMirInstructionJmpCond,
            LilyMirBuildLoad(
              Module,
              Cond->val,
              clone__LilyMirDt(Cond->val->dt->ptr),
              NEW_VARIANT(LilyMirInstructionFunLoadName, var, Cond->val->var)),
            &ThenBlock->block,
            &ElseBlock->block));
    }

    UNREACHABLE("expected i1 or *i1");
}

LilyMirInstruction *
LilyMirLoadFromVal(LilyMirModule *Module, LilyMirInstruction *inst)
{
    ASSERT(inst->kind == LILY_MIR_INSTRUCTION_KIND_VAL);

    LilyMirInstructionVal *val = inst->val;

    partial_free__LilyMirInstruction(inst);

    switch (inst->val->kind) {
        case LILY_MIR_INSTRUCTION_VAL_KIND_VAR:
            ASSERT(val->dt->kind == LILY_MIR_DT_KIND_PTR);

            return NEW_VARIANT(
              LilyMirInstruction,
              val,
              LilyMirBuildLoad(
                Module,
                val,
                clone__LilyMirDt(val->dt->ptr),
                NEW_VARIANT(LilyMirInstructionFunLoadName, var, val->var)));
        case LILY_MIR_INSTRUCTION_VAL_KIND_PARAM:
            ASSERT(val->dt->kind == LILY_MIR_DT_KIND_PTR);

            return NEW_VARIANT(
              LilyMirInstruction,
              val,
              LilyMirBuildLoad(
                Module,
                val,
                clone__LilyMirDt(val->dt->ptr),
                NEW_VARIANT(LilyMirInstructionFunLoadName, param, val->param)));
        default:
            return NEW_VARIANT(LilyMirInstruction, val, val);
    }
}

LilyMirInstructionVal *
LilyMirBuildLoad(LilyMirModule *Module,
                 LilyMirInstructionVal *src,
                 LilyMirDt *dt,
                 LilyMirInstructionFunLoadName value_name)
{
    LilyMirCurrent *current = Module->current->top;

    ASSERT(current->kind == LILY_MIR_CURRENT_KIND_FUN);

    LilyMirInstructionFunLoad *matched_load =
      LilyMirSearchLoad(Module, value_name);

    if (matched_load) {
        // assume: %<name> = <val_inst>
        FREE(LilyMirInstructionVal, src);
        FREE(LilyMirDt, dt);

        return NEW_VARIANT(
          LilyMirInstructionVal,
          reg,
          clone__LilyMirDt(matched_load->inst->reg.inst->load.dt),
          matched_load->inst->reg.name);
    }

    char *name = LilyMirGenerateName(&current->inst->fun.reg_manager);

    LilyMirInstruction *load_inst =
      NEW_VARIANT(LilyMirInstruction,
                  reg,
                  NEW(LilyMirInstructionReg,
                      name,
                      NEW_VARIANT(LilyMirInstruction,
                                  load,
                                  NEW(LilyMirInstructionLoad,
                                      NEW(LilyMirInstructionSrc, src),
                                      clone__LilyMirDt(dt)))));

    push__Vec(current->inst->fun.scope->loads,
              NEW(LilyMirInstructionFunLoad,
                  value_name,
                  load_inst,
                  LilyMirGetBlockId(Module)));

    LilyMirAddInst(Module, load_inst);

    return NEW_VARIANT(LilyMirInstructionVal, reg, dt, name);
}

void
LilyMirBuildVar(LilyMirModule *Module,
                char *name,
                LilyMirDt *dt,
                LilyMirInstruction *inst,
                bool allow_alloc)
{
    ASSERT(inst->kind == LILY_MIR_INSTRUCTION_KIND_VAL);

    if (allow_alloc) {
        LilyMirAddInst(
          Module,
          NEW_VARIANT(LilyMirInstruction,
                      var,
                      NEW(LilyMirInstructionVar,
                          name,
                          LilyMirBuildAlloc(Module, clone__LilyMirDt(dt)))));
    }

    LilyMirInstruction *var_store = LilyMirBuildStore(
      Module, NEW_VARIANT(LilyMirInstructionVal, var, dt, name), inst->val);

    if (var_store) {
        LilyMirAddInst(Module, var_store);
    }

    partial_free__LilyMirInstruction(inst);
}

LilyMirInstruction *
LilyMirBuildStore(LilyMirModule *Module,
                  LilyMirInstructionVal *dest,
                  LilyMirInstructionVal *src)
{
    // NOTE: itself assigments are normally detected and rejected by the
    // analysis, but in some situations in the MIR generation we can get itself
    // assignement, so to avoid unused instruction we will compare (equal) both
    // value to solve this problem.
    if (!eq__LilyMirInstructionVal(dest, src)) {
        // Remove load from the scope to avoid no-updating value when you
        // re-store a value
        switch (dest->kind) {
            case LILY_MIR_INSTRUCTION_VAL_KIND_REG:
                LilyMirRemoveLoad(
                  Module,
                  NEW_VARIANT(LilyMirInstructionFunLoadName, reg, dest->reg));

                break;
            case LILY_MIR_INSTRUCTION_VAL_KIND_PARAM:
                LilyMirRemoveLoad(Module,
                                  NEW_VARIANT(LilyMirInstructionFunLoadName,
                                              param,
                                              dest->param));

                break;
            case LILY_MIR_INSTRUCTION_VAL_KIND_VAR:
                LilyMirRemoveLoad(
                  Module,
                  NEW_VARIANT(LilyMirInstructionFunLoadName, var, dest->var));

                break;
            default:
                break;
        }

        return NEW_VARIANT(
          LilyMirInstruction, store, NEW(LilyMirInstructionDestSrc, dest, src));
    }

    FREE(LilyMirInstructionVal, dest);
    FREE(LilyMirInstructionVal, src);

    return NULL;
}

LilyMirInstruction *
LilyMirBuildCall(LilyMirModule *Module,
                 LilyMirDt *ReturnDt,
                 const char *Name,
                 Vec *Params)
{
    LilyMirCurrent *current = Module->current->top;

    ASSERT(current->kind == LILY_MIR_CURRENT_KIND_FUN);

    if (ReturnDt->kind == LILY_MIR_DT_KIND_UNIT) {
        return NEW_VARIANT(
          LilyMirInstruction,
          call,
          NEW(
            LilyMirInstructionCall, clone__LilyMirDt(ReturnDt), Name, Params));
    }

    char *name = LilyMirGenerateName(&current->inst->fun.reg_manager);

    LilyMirInstruction *inst =
      NEW_VARIANT(LilyMirInstruction,
                  reg,
                  NEW(LilyMirInstructionReg,
                      name,
                      NEW_VARIANT(LilyMirInstruction,
                                  call,
                                  NEW(LilyMirInstructionCall,
                                      clone__LilyMirDt(ReturnDt),
                                      Name,
                                      Params))));

    LilyMirAddInst(Module, inst);

    return NEW_VARIANT(LilyMirInstruction,
                       val,
                       NEW_VARIANT(LilyMirInstructionVal, reg, ReturnDt, name));
}

LilyMirInstruction *
LilyMirBuildSysCall(LilyMirModule *Module,
                    LilyMirDt *ReturnDt,
                    const char *Name,
                    Vec *Params)
{
    LilyMirCurrent *current = Module->current->top;

    ASSERT(current->kind == LILY_MIR_CURRENT_KIND_FUN);

    if (ReturnDt->kind == LILY_MIR_DT_KIND_UNIT) {
        return NEW_VARIANT(
          LilyMirInstruction,
          sys_call,
          NEW(
            LilyMirInstructionCall, clone__LilyMirDt(ReturnDt), Name, Params));
    }

    char *name = LilyMirGenerateName(&current->inst->fun.reg_manager);

    LilyMirInstruction *inst =
      NEW_VARIANT(LilyMirInstruction,
                  reg,
                  NEW(LilyMirInstructionReg,
                      name,
                      NEW_VARIANT(LilyMirInstruction,
                                  sys_call,
                                  NEW(LilyMirInstructionCall,
                                      clone__LilyMirDt(ReturnDt),
                                      Name,
                                      Params))));

    LilyMirAddInst(Module, inst);

    return NEW_VARIANT(LilyMirInstruction,
                       val,
                       NEW_VARIANT(LilyMirInstructionVal, reg, ReturnDt, name));
}

LilyMirInstruction *
LilyMirBuildBuiltinCall(LilyMirModule *Module,
                        LilyMirDt *ReturnDt,
                        const char *Name,
                        Vec *Params)
{
    LilyMirCurrent *current = Module->current->top;

    ASSERT(current->kind == LILY_MIR_CURRENT_KIND_FUN);

    if (ReturnDt->kind == LILY_MIR_DT_KIND_UNIT) {
        return NEW_VARIANT(
          LilyMirInstruction,
          builtin_call,
          NEW(
            LilyMirInstructionCall, clone__LilyMirDt(ReturnDt), Name, Params));
    }

    char *name = LilyMirGenerateName(&current->inst->fun.reg_manager);

    LilyMirInstruction *inst =
      NEW_VARIANT(LilyMirInstruction,
                  reg,
                  NEW(LilyMirInstructionReg,
                      name,
                      NEW_VARIANT(LilyMirInstruction,
                                  builtin_call,
                                  NEW(LilyMirInstructionCall,
                                      clone__LilyMirDt(ReturnDt),
                                      Name,
                                      Params))));

    LilyMirAddInst(Module, inst);

    return NEW_VARIANT(LilyMirInstruction,
                       val,
                       NEW_VARIANT(LilyMirInstructionVal, reg, ReturnDt, name));
}

LilyMirDebugInfo *
LilyMirBuildDIFile(LilyMirModule *Module, String *filename, String *directory)
{
    LilyMirDebugInfo *file =
      NEW_VARIANT(LilyMirDebugInfo,
                  file,
                  Module->debug_info_manager.count,
                  NEW(LilyMirDebugInfoFile, filename, directory));

    for (Usize i = 0; i < Module->files->len; ++i) {
        LilyMirDebugInfo *item = get__Vec(Module->files, i);

        if (eq__LilyMirDebugInfo(item, file)) {
            FREE(LilyMirDebugInfo, file);

            return item;
        }
    }

    ++Module->debug_info_manager.count;

    push__Vec(Module->files, file);

    return file;
}

LilyMirDebugInfo *
LilyMirBuildDIBlock(LilyMirModule *Module,
                    const LilyMirDebugInfo *Scope,
                    const LilyMirDebugInfoFile *File,
                    Usize Line,
                    Usize Column)
{
    LilyMirDebugInfo *block_debug_info =
      NEW_VARIANT(LilyMirDebugInfo,
                  block,
                  Module->debug_info_manager.count,
                  NEW(LilyMirDebugInfoBlock, Scope, File, Line, Column));

    return add__LilyMirDebugInfoManager(&Module->debug_info_manager,
                                        block_debug_info);
}

LilyMirDebugInfo *
LilyMirBuildDILocation(LilyMirModule *Module,
                       const LilyMirDebugInfo *Scope,
                       Usize Line,
                       Usize Column)
{
    LilyMirDebugInfo *location_debug_info =
      NEW_VARIANT(LilyMirDebugInfo,
                  location,
                  Module->debug_info_manager.count,
                  NEW(LilyMirDebugInfoLocation, Scope, Line, Column));

    return add__LilyMirDebugInfoManager(&Module->debug_info_manager,
                                        location_debug_info);
}

LilyMirDebugInfo *
LilyMirBuildDISubProgram(LilyMirModule *Module,
                         const LilyMirDebugInfo *Scope,
                         const LilyMirDebugInfoFile *File,
                         Usize Line,
                         Usize Column)
{
    LilyMirDebugInfo *sub_program_debug_info =
      NEW_VARIANT(LilyMirDebugInfo,
                  sub_program,
                  Module->debug_info_manager.count,
                  NEW(LilyMirDebugInfoSubProgram, Scope, File, Line, Column));

    return add__LilyMirDebugInfoManager(&Module->debug_info_manager,
                                        sub_program_debug_info);
}

LilyMirDebugInfo *
LilyMirBuildDIEnumerator(LilyMirModule *Module,
                         const char *name,
                         Usize value,
                         bool is_unsigned)
{
    LilyMirDebugInfo *enumerator_debug_info =
      NEW_VARIANT(LilyMirDebugInfo,
                  enumerator,
                  Module->debug_info_manager.count,
                  NEW(LilyMirDebugInfoEnumerator, name, value, is_unsigned));

    return add__LilyMirDebugInfoManager(&Module->debug_info_manager,
                                        enumerator_debug_info);
}

LilyMirDebugInfo *
LilyMirBuildDIGlobalVariable(LilyMirModule *Module,
                             const LilyMirDebugInfo *Scope,
                             const LilyMirDebugInfoFile *File,
                             const char *Name,
                             const char *LinkageName,
                             bool IsLocal,
                             bool IsDefinition)
{
    LilyMirDebugInfo *global_variable_debug_info =
      NEW_VARIANT(LilyMirDebugInfo,
                  global_variable,
                  Module->debug_info_manager.count,
                  NEW(LilyMirDebugInfoGlobalVariable,
                      Scope,
                      File,
                      Name,
                      LinkageName,
                      IsLocal,
                      IsDefinition));

    return add__LilyMirDebugInfoManager(&Module->debug_info_manager,
                                        global_variable_debug_info);
}

LilyMirDebugInfo *
LilyMirBuildDILocalVariable(LilyMirModule *Module,
                            const LilyMirDebugInfo *Scope,
                            const LilyMirDebugInfoFile *File,
                            const LilyMirDebugInfo *Type,
                            const char *Name,
                            Usize ArgCount,
                            Usize Line)
{
    LilyMirDebugInfo *local_variable_debug_info =
      NEW_VARIANT(LilyMirDebugInfo,
                  local_variable,
                  Module->debug_info_manager.count,
                  NEW(LilyMirDebugInfoLocalVariable,
                      Scope,
                      File,
                      Type,
                      Name,
                      ArgCount,
                      Line));

    return add__LilyMirDebugInfoManager(&Module->debug_info_manager,
                                        local_variable_debug_info);
}

LilyMirDebugInfo *
LilyMirBuildDIExpression(LilyMirModule *Module, LilyMirDebugInfo *Expression)
{
    LilyMirDebugInfo *expression_debug_info =
      NEW_VARIANT(LilyMirDebugInfo,
                  expression,
                  Module->debug_info_manager.count,
                  Expression);

    return add__LilyMirDebugInfoManager(&Module->debug_info_manager,
                                        expression_debug_info);
}

LilyMirDebugInfo *
LilyMirBuildDIType(LilyMirModule *Module,
                   const char *Name,
                   Usize Size,
                   enum LilyMirDebugInfoEncoding Encoding)
{
    LilyMirDebugInfo *type_debug_info =
      NEW_VARIANT(LilyMirDebugInfo,
                  type,
                  Module->debug_info_manager.count,
                  NEW(LilyMirDebugInfoType, Name, Size, Encoding));

    return add__LilyMirDebugInfoManager(&Module->debug_info_manager,
                                        type_debug_info);
}

LilyMirDebugInfo *
LilyMirBuildDIDerivedType(LilyMirModule *Module,
                          const LilyMirDebugInfo *Scope,
                          const LilyMirDebugInfo *BaseType,
                          enum LilyMirDebugInfoTag Tag,
                          const char *Name,
                          Usize Size,
                          Usize Align,
                          Usize Offset)
{
    LilyMirDebugInfo *derived_type_debug_info =
      NEW_VARIANT(LilyMirDebugInfo,
                  derived_type,
                  Module->debug_info_manager.count,
                  NEW(LilyMirDebugInfoDerivedType,
                      Scope,
                      BaseType,
                      Tag,
                      Name,
                      Size,
                      Align,
                      Offset));

    return add__LilyMirDebugInfoManager(&Module->debug_info_manager,
                                        derived_type_debug_info);
}

LilyMirDebugInfo *
LilyMirBuildDICompositeType(LilyMirModule *Module,
                            enum LilyMirDebugInfoTag Tag,
                            const char *Name,
                            Usize Size,
                            Usize Align,
                            LilyMirDebugInfo *Elements)
{
    LilyMirDebugInfo *composite_type_debug_info = NEW_VARIANT(
      LilyMirDebugInfo,
      comp_type,
      Module->debug_info_manager.count,
      NEW(LilyMirDebugInfoCompositeType, Tag, Name, Size, Align, Elements));

    return add__LilyMirDebugInfoManager(&Module->debug_info_manager,
                                        composite_type_debug_info);
}

LilyMirDebugInfo *
LilyMirBuildDIElements(LilyMirModule *Module, Vec *items)
{
    LilyMirDebugInfo *elements_debug_info =
      NEW_VARIANT(LilyMirDebugInfo,
                  elements,
                  Module->debug_info_manager.count,
                  NEW(LilyMirDebugInfoElements, items));

    return add__LilyMirDebugInfoManager(&Module->debug_info_manager,
                                        elements_debug_info);
}

LilyMirInstructionVal *
LilyMirBuildVirtualVariable(LilyMirModule *Module, LilyMirDt *dt)
{
    LilyMirCurrent *current = LilyMirGetCurrentOnTop(Module);

    ASSERT(current);
    ASSERT(current->kind == LILY_MIR_CURRENT_KIND_FUN);

    char *name =
      LilyMirGenerateName(&current->inst->fun.virtual_variable_manager);

    // NOTE: We don't push virtual variable in the scope, because that's not
    // used anywhere and only used by the compiler.
    LilyMirAddInst(
      Module,
      NEW_VARIANT(LilyMirInstruction,
                  var,
                  NEW(LilyMirInstructionVar,
                      name,
                      LilyMirBuildAlloc(Module, clone__LilyMirDt(dt)))));

    return NEW_VARIANT(LilyMirInstructionVal, var, dt, name);
}

void
LilyMirDisposeModule(const LilyMirModule *Module)
{
    FREE_ORD_HASHMAP_VALUES(Module->insts, LilyMirInstruction);
    FREE(OrderedHashMap, Module->insts);
    FREE_STACK_ITEMS(Module->current, LilyMirCurrent);
    FREE(Stack, Module->current);
    FREE_BUFFER_ITEMS(
      Module->files->buffer, Module->files->len, LilyMirDebugInfoFile);
    FREE(Vec, Module->files);
    FREE(LilyMirDebugInfoManager, &Module->debug_info_manager);
}

void
LilyMirNextBlockAndClearScope(LilyMirModule *Module, LilyMirScope *Scope)
{
    ASSERT(Module->current->len > 0);

    LilyMirCurrent *current = Module->current->top;

    ASSERT(current->kind == LILY_MIR_CURRENT_KIND_FUN);

    pop__Stack(current->inst->fun.block_stack);

    FREE(LilyMirScope, Scope);
}

LilyMirInstruction *
LilyMirBuildBlock(LilyMirModule *Module, LilyMirBlockLimit *limit)
{
    LilyMirCurrent *current = Module->current->top;

    ASSERT(current->kind == LILY_MIR_CURRENT_KIND_FUN);

    // if (current->inst->fun.scope->limit->is_set) {
    //     // Remove all scopes with this limit.
    //     LilyMirRemoveScopeByLimit(Module, current->inst->fun.scope->limit);
    // }

    add_scope__LilyMirScope(&current->inst->fun.scope, limit);

    char *name = LilyMirGenerateName(&current->inst->fun.block_manager);

    return NEW_VARIANT(LilyMirInstruction,
                       block,
                       NEW(LilyMirInstructionBlock,
                           name,
                           limit,
                           current->inst->fun.block_count++));
}

void
LilyMirAddBlock(LilyMirModule *Module, LilyMirInstruction *Block)
{
    LilyMirCurrent *current = LilyMirGetCurrentOnTop(Module);

    ASSERT(current->kind == LILY_MIR_CURRENT_KIND_FUN);

    insert__OrderedHashMap(
      current->inst->fun.insts, (char *)Block->block.name, Block);
    push__Stack(current->inst->fun.block_stack, &Block->block);
}

LilyMirInstructionBlock *
LilyMirPopBlock(LilyMirModule *Module)
{
    LilyMirCurrent *current = LilyMirGetCurrentOnTop(Module);

    ASSERT(current->kind == LILY_MIR_CURRENT_KIND_FUN);
    ASSERT(current->inst->fun.block_stack->len > 0);

    LilyMirInstructionBlock *block = pop__Stack(current->inst->fun.block_stack);

    if (block->limit->is_set) {
        // Remove all scopes with this limit.
        LilyMirRemoveScopeByLimit(Module, current->inst->fun.scope->limit);
    }

    return block;
}

LilyMirInstructionBlock *
LilyMirGetInsertBlock(LilyMirModule *Module)
{
    LilyMirCurrent *current = LilyMirGetCurrentOnTop(Module);

    ASSERT(current->kind == LILY_MIR_CURRENT_KIND_FUN);
    ASSERT(current->inst->fun.block_stack->len > 0);

    return current->inst->fun.block_stack->top;
}

LilyMirInstruction *
LilyMirGetInsertBlockInst(LilyMirModule *Module)
{
    LilyMirCurrent *current = LilyMirGetCurrentOnTop(Module);

    ASSERT(current->kind == LILY_MIR_CURRENT_KIND_FUN);
    ASSERT(current->inst->fun.block_stack->len > 0);

    return last__OrderedHashMap(current->inst->fun.insts);
}

bool
LilyMirEmptyBlock(LilyMirModule *Module)
{
    LilyMirCurrent *current = LilyMirGetCurrentOnTop(Module);

    ASSERT(current->kind == LILY_MIR_CURRENT_KIND_FUN);
    ASSERT(current->inst->fun.block_stack->len > 0);

    return CAST(LilyMirInstructionBlock *, current->inst->fun.block_stack->top)
             ->insts->len == 0;
}

char *
LilyMirGenerateName(LilyMirNameManager *NameManager)
{
    char *name = format("{s}{zu}", NameManager->base_name, NameManager->count);

    push__Vec(NameManager->names, name);

    ++NameManager->count;

    return name;
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
        LilyMirCurrent *top = Module->current->top;

        if (top) {
            ASSERT(top->kind == LILY_MIR_CURRENT_KIND_FUN);

            if (!strcmp(BaseName, top->inst->fun.base_name) &&
                LilyMirValidTypesOfFun(&top->inst->fun, Types, Len)) {
                return top->inst->fun.name;
            }
        }

        for (Usize i = 0; i < Module->current->len - 1; ++i) {
            LilyMirCurrent *current = Module->current->buffer[i];

            ASSERT(current->kind == LILY_MIR_CURRENT_KIND_FUN);

            if (!strcmp(BaseName, current->inst->fun.base_name) &&
                LilyMirValidTypesOfFun(&current->inst->fun, Types, Len)) {
                return current->inst->fun.name;
            }
        }
    }

    UNREACHABLE("the analysis has a bug!!");
}

#define LILY_MIR_ADD_FINAL_INSTRUCTION(build_jmp)                             \
    LilyMirCurrent *top = Module->current->top;                               \
                                                                              \
    ASSERT(top->kind == LILY_MIR_CURRENT_KIND_FUN);                           \
    ASSERT(top->inst->fun.insts->len > 0);                                    \
                                                                              \
    LilyMirInstructionBlock *current_block = top->inst->fun.block_stack->top; \
                                                                              \
    if (current_block->insts->len == 0) {                                     \
        return LilyMirAddInst(Module, build_jmp(Module, exit_block));         \
    }                                                                         \
                                                                              \
    LilyMirInstruction *last_inst = last__Vec(current_block->insts);          \
                                                                              \
    if (last_inst->kind != LILY_MIR_INSTRUCTION_KIND_RET &&                   \
        last_inst->kind != LILY_MIR_INSTRUCTION_KIND_JMP &&                   \
        last_inst->kind != LILY_MIR_INSTRUCTION_KIND_JMPCOND &&               \
        last_inst->kind != LILY_MIR_INSTRUCTION_KIND_UNREACHABLE) {           \
        return LilyMirAddInst(Module, build_jmp(Module, exit_block));         \
    }

void
LilyMirAddFinalInstruction(LilyMirModule *Module,
                           LilyMirInstructionBlock *exit_block)
{
    LILY_MIR_ADD_FINAL_INSTRUCTION(LilyMirBuildJmp);
}

void
LilyMirAddFinalInstructionWithInst(LilyMirModule *Module,
                                   LilyMirInstruction *exit_block)
{
    LILY_MIR_ADD_FINAL_INSTRUCTION(LilyMirBuildJmpWithInst);
}

bool
LilyMirHasFinalInstruction(LilyMirModule *Module)
{
    LilyMirCurrent *top = Module->current->top;

    ASSERT(top->kind == LILY_MIR_CURRENT_KIND_FUN);
    ASSERT(top->inst->fun.insts->len > 0);

    LilyMirInstructionBlock *current_block = top->inst->fun.block_stack->top;

    ASSERT(current_block);

    if (current_block->insts->len == 0) {
        return false;
    }

    LilyMirInstruction *last_inst = last__Vec(current_block->insts);

    if (last_inst->kind != LILY_MIR_INSTRUCTION_KIND_RET &&
        last_inst->kind != LILY_MIR_INSTRUCTION_KIND_JMP &&
        last_inst->kind != LILY_MIR_INSTRUCTION_KIND_JMPCOND &&
        last_inst->kind != LILY_MIR_INSTRUCTION_KIND_UNREACHABLE) {
        return false;
    }

    return true;
}

bool
LilyMirHasRetInstruction(LilyMirModule *Module)
{
    LilyMirCurrent *top = Module->current->top;

    ASSERT(top->kind == LILY_MIR_CURRENT_KIND_FUN);
    ASSERT(top->inst->fun.insts->len > 0);

    LilyMirInstructionBlock *current_block = top->inst->fun.block_stack->top;

    if (current_block->insts->len == 0) {
        return false;
    }

    LilyMirInstruction *last_inst = last__Vec(current_block->insts);

    if (last_inst->kind != LILY_MIR_INSTRUCTION_KIND_RET) {
        return false;
    }

    return true;
}

void
LilyMirBuildIfBranch(LilyMirModule *Module,
                     LilyCheckedSignatureFun *fun_signature,
                     LilyMirScope *scope,
                     const LilyCheckedStmtIfBranch *if_branch,
                     LilyMirInstruction *next_block,
                     LilyMirInstruction *exit_block)
{
    //   ; ...
    //   jmp cond
    // cond:
    //   %0 = <cond>
    //   jmpcond %0, if, next
    // if:
    //   ; ...
    //   jmp exit
    // next:
    //   ; ...
    // exit:
    //   ; ...

    // If the previous block is empty, no condition block will be created.
    LilyMirBlockLimit *block_limit = NEW(LilyMirBlockLimit);

    LilyMirInstruction *cond_block =
      LilyMirEmptyBlock(Module)
        ? NULL
        : LilyMirBuildBlock(Module, ref__LilyMirBlockLimit(block_limit));

    // 1. Add jmp cond
    // 2. Add cond block
    if (cond_block) {
        LilyMirAddInst(Module, LilyMirBuildJmpWithInst(Module, cond_block));
        LilyMirPopBlock(Module);
        LilyMirAddBlock(Module, cond_block);
    }

    LilyMirInstruction *cond = generate_expr__LilyMir(
      Module, fun_signature, scope, if_branch->cond, NULL, false);

    ASSERT(cond);

    LilyMirInstruction *if_block = LilyMirBuildBlock(Module, block_limit);

    // 3. Add conditional jmp
    LilyMirAddInst(
      Module, LilyMirBuildJmpCondWithInst(Module, cond, if_block, next_block));

    LilyMirPopBlock(Module);
    LilyMirAddBlock(Module, if_block);

    // 4. Generate the content of the body
    GENERATE_BODY(
      Module, fun_signature, scope, block_limit, NULL, NULL, if_branch->body);

    // 5. Add final instruction
    LilyMirSetBlockLimit(block_limit, LilyMirGetInsertBlock(Module)->id);
    LilyMirAddFinalInstructionWithInst(Module, exit_block);

    partial_free__LilyMirInstruction(cond);
}

void
LilyMirBuildElifBranch(LilyMirModule *Module,
                       LilyCheckedSignatureFun *fun_signature,
                       LilyMirScope *scope,
                       const LilyCheckedStmtIfBranch *elif_branch,
                       LilyMirInstruction *next_block,
                       LilyMirInstruction *current_block,
                       LilyMirInstruction *exit_block)
{
    LilyMirPopBlock(Module);
    LilyMirAddBlock(Module, current_block);

    LilyMirInstruction *cond = generate_expr__LilyMir(
      Module, fun_signature, scope, elif_branch->cond, NULL, false);

    ASSERT(cond);

    LilyMirInstruction *elif_block = LilyMirBuildBlock(
      Module, ref__LilyMirBlockLimit(current_block->block.limit));

    // 1. Add conditional jmp
    LilyMirAddInst(
      Module,
      LilyMirBuildJmpCondWithInst(Module, cond, elif_block, next_block));

    LilyMirPopBlock(Module);
    LilyMirAddBlock(Module, elif_block);

    // 2. Generate the content of the body
    GENERATE_BODY(Module,
                  fun_signature,
                  scope,
                  elif_block->block.limit,
                  NULL,
                  NULL,
                  elif_branch->body);

    // 3. Add final instruction
    LilyMirSetBlockLimit(elif_block->block.limit,
                         LilyMirGetInsertBlock(Module)->id);
    LilyMirAddFinalInstructionWithInst(Module, exit_block);

    partial_free__LilyMirInstruction(cond);
}

void
LilyMirBuildElseBranch(LilyMirModule *Module,
                       LilyCheckedSignatureFun *fun_signature,
                       LilyMirScope *scope,
                       const LilyCheckedStmtElseBranch *else_branch,
                       LilyMirInstruction *current_block,
                       LilyMirInstruction *exit_block)
{
    LilyMirPopBlock(Module);
    LilyMirAddBlock(Module, current_block);

    // 1. Generate the content of the body
    GENERATE_BODY(Module,
                  fun_signature,
                  scope,
                  current_block->block.limit,
                  NULL,
                  NULL,
                  else_branch->body);

    // 2. Add final instruction
    LilyMirSetBlockLimit(current_block->block.limit,
                         LilyMirGetInsertBlock(Module)->id);
    LilyMirAddFinalInstructionWithInst(Module, exit_block);
}

void
LilyMirBuildIf(LilyMirModule *Module,
               LilyCheckedSignatureFun *fun_signature,
               LilyMirScope *scope,
               LilyMirBlockLimit *parent_block_limit,
               const LilyCheckedStmtIf *if_stmt)
{
    LilyMirInstruction *exit_block =
      LilyMirBuildBlock(Module, ref__LilyMirBlockLimit(parent_block_limit));
    LilyMirInstruction *next_block =
      !if_stmt->elifs && !if_stmt->else_
        ? exit_block
        : LilyMirBuildBlock(Module, NEW(LilyMirBlockLimit));

    LilyMirBuildIfBranch(
      Module, fun_signature, scope, if_stmt->if_, next_block, exit_block);

    if (if_stmt->elifs) {
        for (Usize i = 0; i < if_stmt->elifs->len; ++i) {
            LilyMirInstruction *current_block = next_block;

            next_block = i + 1 == if_stmt->elifs->len && !if_stmt->else_
                           ? exit_block
                           : LilyMirBuildBlock(Module, NEW(LilyMirBlockLimit));

            LilyMirBuildElifBranch(Module,
                                   fun_signature,
                                   scope,
                                   get__Vec(if_stmt->elifs, i),
                                   next_block,
                                   current_block,
                                   exit_block);
        }
    }

    if (if_stmt->else_) {
        LilyMirBuildElseBranch(
          Module, fun_signature, scope, if_stmt->else_, next_block, exit_block);

        if (!LilyMirHasRetInstruction(Module)) {
            LilyMirPopBlock(Module);
            LilyMirAddBlock(Module, exit_block);
        } else {
            FREE(LilyMirInstruction, exit_block);
        }
    } else {
        LilyMirPopBlock(Module);
        LilyMirAddBlock(Module, exit_block);
    }
}

void
LilyMirBuildWhile(LilyMirModule *Module,
                  LilyCheckedSignatureFun *fun_signature,
                  LilyMirScope *scope,
                  LilyMirBlockLimit *parent_block_limit,
                  const LilyCheckedStmtWhile *while_stmt)
{
    //   ; ...
    //   jmp cond
    // cond:
    //   %0 = <cond>
    //   jmpcond %0, if, exit
    // while:
    //   ; ...
    //   jmp cond
    // exit:
    //   ; ...

    LilyMirInstruction *last_block = LilyMirGetInsertBlockInst(Module);
    LilyMirBlockLimit *block_limit = NEW(LilyMirBlockLimit);
    LilyMirInstruction *cond_block =
      LilyMirEmptyBlock(Module)
        ? NULL
        : LilyMirBuildBlock(Module, ref__LilyMirBlockLimit(block_limit));
    LilyMirInstruction *while_block = LilyMirBuildBlock(Module, block_limit);
    LilyMirInstruction *exit_block =
      LilyMirBuildBlock(Module, ref__LilyMirBlockLimit(parent_block_limit));

    // 1. Add jmp cond
    // 2. Add cond block
    if (cond_block) {
        LilyMirAddInst(Module, LilyMirBuildJmpWithInst(Module, cond_block));
        LilyMirPopBlock(Module);
        LilyMirAddBlock(Module, cond_block);
    }

    LilyMirInstruction *cond = generate_expr__LilyMir(
      Module, fun_signature, scope, while_stmt->cond, NULL, false);

    ASSERT(cond);

    // 3. Add conditional jmp
    LilyMirAddInst(
      Module,
      LilyMirBuildJmpCondWithInst(Module, cond, while_block, exit_block));

    LilyMirPopBlock(Module);
    LilyMirAddBlock(Module, while_block);

    // 4. Generate the content of the body
    GENERATE_BODY(Module,
                  fun_signature,
                  scope,
                  block_limit,
                  exit_block,
                  cond_block,
                  while_stmt->body);

    // 5. Add final instruction
    LilyMirSetBlockLimit(block_limit, LilyMirGetInsertBlock(Module)->id);
    LilyMirAddFinalInstructionWithInst(Module,
                                       cond_block ? cond_block : last_block);

    LilyMirPopBlock(Module);
    LilyMirAddBlock(Module, exit_block);

    partial_free__LilyMirInstruction(cond);
}

void
LilyMirBuildBlockStmt(LilyMirModule *Module,
                      LilyCheckedSignatureFun *fun_signature,
                      LilyMirScope *scope,
                      LilyMirBlockLimit *parent_block_limit,
                      const LilyCheckedStmtBlock *block_stmt)
{
    //   ; ...
    //   jmp block
    // block:
    //   ; ...
    //   jmp exit
    // exit:
    //   ; ...

    LilyMirInstruction *block =
      LilyMirBuildBlock(Module, NEW(LilyMirBlockLimit));
    LilyMirInstruction *exit_block =
      LilyMirBuildBlock(Module, ref__LilyMirBlockLimit(parent_block_limit));

    LilyMirAddInst(Module, LilyMirBuildJmpWithInst(Module, block));
    LilyMirPopBlock(Module);
    LilyMirAddBlock(Module, block);

    GENERATE_BODY(Module,
                  fun_signature,
                  scope,
                  block->block.limit,
                  NULL,
                  NULL,
                  block_stmt->body);

    LilyMirSetBlockLimit(block->block.limit, LilyMirGetInsertBlock(Module)->id);

    if (LilyMirHasRetInstruction(Module)) {
        FREE(LilyMirInstruction, exit_block);
    } else {
        LilyMirAddFinalInstructionWithInst(Module, exit_block);
        LilyMirPopBlock(Module);
        LilyMirAddBlock(Module, exit_block);
    }
}

void
LilyMirBuildBreak(LilyMirModule *Module, LilyMirInstruction *exit_block)
{
    ASSERT(exit_block);
    LilyMirAddInst(Module, LilyMirBuildJmpWithInst(Module, exit_block));
}

void
LilyMirBuildNext(LilyMirModule *Module, LilyMirInstruction *next_block)
{
    ASSERT(next_block);
    LilyMirAddInst(Module, LilyMirBuildJmpWithInst(Module, next_block));
}

void
LilyMirBuildUnsafe(LilyMirModule *Module,
                   LilyCheckedSignatureFun *fun_signature,
                   LilyMirScope *scope,
                   LilyMirBlockLimit *parent_block_limit,
                   const LilyCheckedStmtUnsafe *unsafe_stmt)
{
    //   ; ...
    //   jmp unsafe
    // unsafe:
    //   ; ...
    //   jmp exit
    // exit:
    //   ; ...

    LilyMirInstruction *block =
      LilyMirBuildBlock(Module, NEW(LilyMirBlockLimit));
    LilyMirInstruction *exit_block =
      LilyMirBuildBlock(Module, ref__LilyMirBlockLimit(parent_block_limit));

    LilyMirAddInst(Module, LilyMirBuildJmpWithInst(Module, block));
    LilyMirPopBlock(Module);
    LilyMirAddBlock(Module, block);

    GENERATE_BODY(Module,
                  fun_signature,
                  scope,
                  block->block.limit,
                  NULL,
                  NULL,
                  unsafe_stmt->body);

    LilyMirSetBlockLimit(block->block.limit, LilyMirGetInsertBlock(Module)->id);

    if (LilyMirHasRetInstruction(Module)) {
        FREE(LilyMirInstruction, exit_block);
    } else {
        LilyMirAddFinalInstructionWithInst(Module, exit_block);
        LilyMirPopBlock(Module);
        LilyMirAddBlock(Module, exit_block);
    }
}

void
LilyMirBuildSwitch(LilyMirModule *Module,
                   LilyCheckedSignatureFun *fun_signature,
                   LilyMirScope *scope,
                   LilyMirBlockLimit *parent_block_limit,
                   const LilyCheckedStmtSwitch *switch_stmt)
{
    // ; ...
    // switch <val>, block default {
    //   case val(i64) 0, block bb0
    //   case val(i64) 1, block bb1
    // }
    // bb0:
    //   ; ...
    // bb1:
    //   ; ...
    // default:
    //   ; ...

    LilyMirInstruction *exit_block =
      LilyMirBuildBlock(Module, ref__LilyMirBlockLimit(parent_block_limit));
    LilyMirInstruction *switched_inst = generate_expr__LilyMir(
      Module, fun_signature, scope, switch_stmt->switched_expr, NULL, false);

    ASSERT(switched_inst);

    LilyMirInstructionVal *switched_val = switched_inst->val;
    Vec *cases = NEW(Vec); // Vec<LilyMirInstructionSwitchCase*>*
    LilyMirInstruction *default_block =
      LilyMirBuildBlock(Module, NEW(LilyMirBlockLimit));
    LilyMirInstruction *switch_inst = NEW_VARIANT(
      LilyMirInstruction,
      switch,
      NEW(
        LilyMirInstructionSwitch, switched_val, &default_block->block, cases));

    LilyMirAddInst(Module, switch_inst);

    for (Usize i = 0; i < switch_stmt->cases->len; ++i) {
        if (i + 1 == switch_stmt->cases->len && switch_stmt->has_else) {
            break;
        }

        LilyCheckedStmtSwitchCase *case_ = get__Vec(switch_stmt->cases, i);
        LilyMirInstruction *case_block =
          LilyMirBuildBlock(Module, NEW(LilyMirBlockLimit));
        Vec *case_values = generate_case_value__LilyMir(
          Module,
          fun_signature,
          scope,
          case_->case_value); // Vec<LilyMirInstructionVal*?>*

        for (Usize j = 0; j < case_->sub_cases->len; ++j) {
            LilyCheckedStmtSwitchSubCase *sub_case =
              get__Vec(case_->sub_cases, j);
            LilyMirInstruction *sub_case_block =
              LilyMirBuildBlock(Module, NEW(LilyMirBlockLimit));

            LilyMirPopBlock(Module);
            LilyMirAddBlock(Module, sub_case_block);

            if (sub_case->cond) {
                LilyMirInstruction *next_block =
                  LilyMirBuildBlock(Module, NEW(LilyMirBlockLimit));
                LilyMirInstruction *cond_inst = generate_expr__LilyMir(
                  Module, fun_signature, scope, sub_case->cond, NULL, false);

                LilyMirAddInst(Module,
                               LilyMirBuildJmpCondWithInst(
                                 Module, cond_inst, next_block, exit_block));

                LilyMirPopBlock(Module);
                LilyMirAddBlock(Module, next_block);

                GENERATE_BODY_ITEM(Module,
                                   fun_signature,
                                   scope,
                                   next_block->block.limit,
                                   exit_block,
                                   NULL,
                                   sub_case->body_item);

                LilyMirAddFinalInstructionWithInst(Module, exit_block);
                LilyMirSetBlockLimit(next_block->block.limit,
                                     LilyMirGetInsertBlock(Module)->id);
            } else {
                GENERATE_BODY_ITEM(Module,
                                   fun_signature,
                                   scope,
                                   sub_case_block->block.limit,
                                   exit_block,
                                   NULL,
                                   sub_case->body_item);

                LilyMirAddFinalInstructionWithInst(Module, exit_block);
                LilyMirSetBlockLimit(sub_case_block->block.limit,
                                     LilyMirGetInsertBlock(Module)->id);
            }
        }

        LilyMirPopBlock(Module);
        LilyMirAddBlock(Module, case_block);

        for (Usize j = 0; j < case_values->len; ++j) {
            LilyMirInstructionVal *case_value = get__Vec(case_values, j);

            // NOTE: else case value is not expected at this time
            ASSERT(case_value);

            push__Vec(cases,
                      NEW(LilyMirInstructionSwitchCase,
                          case_value,
                          &case_block->block));
        }

        LilyMirSetBlockLimit(case_block->block.limit,
                             LilyMirGetInsertBlock(Module)->id);

        FREE(Vec, case_values);
    }

    if (switch_stmt->has_else) {
        LilyCheckedStmtSwitchCase *last_case = last__Vec(switch_stmt->cases);
    }

    LilyMirPopBlock(Module);
    LilyMirAddBlock(Module, exit_block);
}

// void
// LilyMirBuildMatch(LilyMirModule *Module,
//                   LilyCheckedSignatureFun *fun_signature,
//                   LilyMirScope *scope,
//                   LilyMirBlockLimit *parent_block_limit,
//                   const LilyCheckedStmtMatch *match_stmt)
// {
//     LilyMirInstruction *exit_block =
//       LilyMirBuildBlock(Module, ref__LilyMirBlockLimit(parent_block_limit));
//
//     if (match_stmt->use_switch) {
//         LilyMirInstruction *switched_inst = generate_expr__LilyMir(
//           Module, fun_signature, scope, match_stmt->expr, false);
//
//         ASSERT(switched_inst);
//
//         LilyMirInstructionVal *switched_val = switched_inst->val;
//         Vec *cases = NEW(Vec); // Vec<LilyMirInstructionSwitchCase*>*
//
//         LilyMirInstruction *default_block =
//           LilyMirBuildBlock(Module, NEW(LilyMirBlockLimit));
//         LilyMirInstruction *switch_inst =
//           NEW_VARIANT(LilyMirInstruction,
//                       switch,
//                       NEW(LilyMirInstructionSwitch,
//                           switched_val,
//                           &default_block->block,
//                           cases));
//
//         LilyMirAddInst(Module, switch_inst);
//
//         for (Usize i = 0; i < match_stmt->cases->len; ++i) {
//             if (i + 1 == match_stmt->cases->len && match_stmt->has_else) {
//                 break;
//             }
//
//             LilyCheckedStmtMatchCase *case_ = get__Vec(match_stmt->cases, i);
//             LilyMirInstructionVal *case_pattern = generate_pattern__LilyMir(
//               Module, fun_signature, scope, case_->pattern);
//             LilyMirInstruction *block =
//               LilyMirBuildBlock(Module, NEW(LilyMirBlockLimit));
//
//             if (case_->conds) {
//                 TODO("generate cond...");
//             }
//
//             LilyMirPopBlock(Module);
//             LilyMirAddBlock(Module, block);
//
//             GENERATE_BODY_ITEM(Module,
//                                fun_signature,
//                                scope,
//                                block->block.limit,
//                                NULL,
//                                NULL,
//                                case_->body_item);
//
//             LilyMirSetBlockLimit(block->block.limit,
//                                  LilyMirGetInsertBlock(Module)->id);
//             LilyMirAddFinalInstructionWithInst(Module, exit_block);
//
//             push__Vec(
//               cases,
//               NEW(LilyMirInstructionSwitchCase, case_pattern,
//               &block->block));
//         }
//
//         LilyMirPopBlock(Module);
//         LilyMirAddBlock(Module, default_block);
//
//         if (match_stmt->has_else) {
//             LilyCheckedStmtMatchCase *default_case =
//               last__Vec(match_stmt->cases);
//
//             GENERATE_BODY_ITEM(Module,
//                                fun_signature,
//                                scope,
//                                default_block->block.limit,
//                                exit_block,
//                                NULL,
//                                default_case->body_item);
//
//             LilyMirSetBlockLimit(default_block->block.limit,
//                                  LilyMirGetInsertBlock(Module)->id);
//             LilyMirAddFinalInstructionWithInst(Module, exit_block);
//         } else {
//             LilyMirAddInst(Module,
//                            NEW_VARIANT(LilyMirInstruction, unreachable));
//             LilyMirSetBlockLimit(default_block->block.limit,
//                                  LilyMirGetInsertBlock(Module)->id);
//         }
//
//         partial_free__LilyMirInstruction(switched_inst);
//     } else {
//         TODO("generate match stmt");
//     }
//
//     LilyMirPopBlock(Module);
//     LilyMirAddBlock(Module, exit_block);
// }
