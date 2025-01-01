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

#ifndef LILY_CORE_LILY_MIR_H
#define LILY_CORE_LILY_MIR_H

#include <base/assert.h>
#include <base/ordered_hash_map.h>
#include <base/stack.h>
#include <base/vec.h>

#include <core/lily/analysis/checked/expr.h>
#include <core/lily/analysis/checked/signature.h>
#include <core/lily/analysis/checked/stmt/block.h>
#include <core/lily/analysis/checked/stmt/if.h>
#include <core/lily/analysis/checked/stmt/match.h>
#include <core/lily/analysis/checked/stmt/unsafe.h>
#include <core/lily/mir/block_limit.h>
#include <core/lily/mir/instruction.h>
#include <core/lily/mir/scope.h>

#include <stdio.h>

#define MAX_CURRENT_INST 8192

enum LilyMirCurrentKind
{
    LILY_MIR_CURRENT_KIND_CONST,
    LILY_MIR_CURRENT_KIND_FUN,
    LILY_MIR_CURRENT_KIND_STRUCT,
};

typedef struct LilyMirCurrent
{
    enum LilyMirCurrentKind kind;
    LilyMirInstruction *inst; // LilyMirInstruction* (&)
} LilyMirCurrent;

/**
 *
 * @brief Construct LilyMirCurrent type (LILY_MIR_CURRENT_KIND_CONST).
 */
VARIANT_CONSTRUCTOR(LilyMirCurrent *,
                    LilyMirCurrent,
                    const,
                    LilyMirInstruction *const_);

/**
 *
 * @brief Construct LilyMirCurrent type (LILY_MIR_CURRENT_KIND_FUN).
 */
VARIANT_CONSTRUCTOR(LilyMirCurrent *,
                    LilyMirCurrent,
                    fun,
                    LilyMirInstruction *fun);

/**
 *
 * @brief Construct LilyMirCurrent type (LILY_MIR_CURRENT_KIND_STRUCT).
 */
VARIANT_CONSTRUCTOR(LilyMirCurrent *,
                    LilyMirCurrent,
                    struct,
                    LilyMirInstruction *struct_);

/**
 *
 * @brief Free LilyMirCurrent type.
 */
inline DESTRUCTOR(LilyMirCurrent, LilyMirCurrent *self)
{
    lily_free(self);
}

typedef struct LilyMirModule
{
    OrderedHashMap *insts; // OrderedHashMap<LilyMirInstruction*>*
    Stack *current;        // Stack<LilyMirCurrent*>*
    Vec *files;            // Vec<LilyMirDebugInfo*>*
    LilyMirDebugInfoManager debug_info_manager;
} LilyMirModule;

/**
 *
 * @brief Create MIR module.
 */
inline LilyMirModule
LilyMirCreateModule()
{
    return (LilyMirModule){ .insts = NEW(OrderedHashMap),
                            .current = NEW(Stack, MAX_CURRENT_INST),
                            .files = NEW(Vec),
                            .debug_info_manager =
                              NEW(LilyMirDebugInfoManager) };
}

/**
 *
 * @brief Add instruction to the module.
 */
void
LilyMirAddInst(LilyMirModule *Module, LilyMirInstruction *Inst);

/**
 *
 * @brief Reset the current field.
 */
void
LilyMirPopCurrent(LilyMirModule *Module);

LilyMirScopeVar *
LilyMirScopeGetVar(const LilyMirScope *Scope, String *name);

LilyMirScopeParam *
LilyMirScopeGetParam(const LilyMirModule *Module, Usize id);

LilyCheckedDataType *
LilyMirGetCheckedDtFromExpr(const LilyMirModule *Module,
                            const LilyMirScope *Scope,
                            const LilyCheckedExpr *Expr);

inline void
LilyMirAddVar(LilyMirScope *Scope, String *name, LilyCheckedDataType *data_type)
{
    push__Vec(Scope->vars, NEW(LilyMirScopeVar, name, data_type));
}

inline void
LilyMirAddParam(LilyMirScope *Scope, LilyCheckedDataType *data_type)
{
    push__Vec(Scope->params, NEW(LilyMirScopeParam, data_type));
}

void
add_scope__LilyMirScope(LilyMirScope **self, LilyMirBlockLimit *limit);

void
remove_scope__LilyMirScope(LilyMirScope **self);

LilyMirInstructionFunLoad *
search_load__LilyMirScope(const LilyMirScope *self,
                          LilyMirInstructionFunLoadName name);

void
remove_load__LilyMirScope(const LilyMirScope *self,
                          LilyMirInstructionFunLoadName name);

void
LilyMirRemoveScopeByLimit(LilyMirModule *Module,
                          const LilyMirBlockLimit *limit);

LilyMirInstructionFunLoad *
LilyMirSearchLoad(LilyMirModule *Module, LilyMirInstructionFunLoadName name);

void
LilyMirRemoveLoad(LilyMirModule *Module, LilyMirInstructionFunLoadName name);

LilyMirDebugInfo *
LilyMirBuildDIFile(LilyMirModule *Module, String *filename, String *directory);

LilyMirDebugInfo *
LilyMirBuildDIBlock(LilyMirModule *Module,
                    const LilyMirDebugInfo *Scope,
                    const LilyMirDebugInfoFile *File,
                    Usize Line,
                    Usize Column);

LilyMirDebugInfo *
LilyMirBuildDILocation(LilyMirModule *Module,
                       const LilyMirDebugInfo *Scope,
                       Usize Line,
                       Usize Column);

LilyMirDebugInfo *
LilyMirBuildDISubProgram(LilyMirModule *Module,
                         const LilyMirDebugInfo *Scope,
                         const LilyMirDebugInfoFile *File,
                         Usize Line,
                         Usize Column);

LilyMirDebugInfo *
LilyMirBuildDIEnumerator(LilyMirModule *Module,
                         const char *name,
                         Usize value,
                         bool is_unsigned);

LilyMirDebugInfo *
LilyMirBuildDIGlobalVariable(LilyMirModule *Module,
                             const LilyMirDebugInfo *Scope,
                             const LilyMirDebugInfoFile *File,
                             const char *Name,
                             const char *LinkageName,
                             bool IsLocal,
                             bool IsDefinition);

LilyMirDebugInfo *
LilyMirBuildDILocalVariable(LilyMirModule *Module,
                            const LilyMirDebugInfo *Scope,
                            const LilyMirDebugInfoFile *File,
                            const LilyMirDebugInfo *Type,
                            const char *Name,
                            Usize ArgCount,
                            Usize Line);

LilyMirDebugInfo *
LilyMirBuildDIExpression(LilyMirModule *Module, LilyMirDebugInfo *Expression);

LilyMirDebugInfo *
LilyMirBuildDIType(LilyMirModule *Module,
                   const char *Name,
                   Usize Size,
                   enum LilyMirDebugInfoEncoding Encoding);

LilyMirDebugInfo *
LilyMirBuildDIDerivedType(LilyMirModule *Module,
                          const LilyMirDebugInfo *Scope,
                          const LilyMirDebugInfo *BaseType,
                          enum LilyMirDebugInfoTag Tag,
                          const char *Name,
                          Usize Size,
                          Usize Align,
                          Usize Offset);

LilyMirDebugInfo *
LilyMirBuildDICompositeType(LilyMirModule *Module,
                            enum LilyMirDebugInfoTag Tag,
                            const char *Name,
                            Usize Size,
                            Usize Align,
                            LilyMirDebugInfo *Elements);

LilyMirDebugInfo *
LilyMirBuildDIElements(LilyMirModule *Module, Vec *items);

LilyMirInstructionVal *
LilyMirBuildVirtualVariable(LilyMirModule *Module, LilyMirDt *dt);

#define LILY_MIR_SET_DI(inst, build_di, ...) \
    inst->debug_info = build_di(__VA_ARGS__)

void
LilyMirDisposeModule(const LilyMirModule *Module);

inline void
LilyMirNextBlock(LilyMirModule *Module)
{
    ASSERT(CAST(LilyMirCurrent *, Module->current->top)->kind ==
           LILY_MIR_CURRENT_KIND_FUN);
    pop__Stack(
      CAST(LilyMirCurrent *, Module->current->top)->inst->fun.block_stack);
}

void
LilyMirNextBlockAndClearScope(LilyMirModule *Module, LilyMirScope *Scope);

/**
 *
 * @param limit LilyMirBlockLimit*
 */
LilyMirInstruction *
LilyMirBuildBlock(LilyMirModule *Module, LilyMirBlockLimit *limit);

void
LilyMirAddBlock(LilyMirModule *Module, LilyMirInstruction *Block);

LilyMirInstructionBlock *
LilyMirPopBlock(LilyMirModule *Module);

LilyMirInstructionBlock *
LilyMirGetInsertBlock(LilyMirModule *Module);

LilyMirInstruction *
LilyMirGetInsertBlockInst(LilyMirModule *Module);

bool
LilyMirEmptyBlock(LilyMirModule *Module);

void
LilyMirDeleteBasicBlock(LilyMirInstructionBlock *Block);

void
LilyMirDisposeBuilder(LilyMirModule *Module);

char *
LilyMirGenerateName(LilyMirNameManager *NameManager);

inline const char *
LilyMirGetLastRegName(LilyMirModule *Module)
{
    return last__Vec(CAST(LilyMirCurrent *, Module->current->top)
                       ->inst->fun.reg_manager.names);
}

LilyMirInstruction *
LilyMirBuildReg(LilyMirModule *Module, LilyMirInstruction *Inst);

inline LilyMirInstructionVal *
LilyMirBuildRegVal(LilyMirModule *Module, LilyMirDt *dt, const char *name)
{
    return NEW_VARIANT(LilyMirInstructionVal, reg, dt, name);
}

inline LilyMirInstruction *
LilyMirBuildAlloc(LilyMirModule *Module, LilyMirDt *dt)
{
    return NEW_VARIANT(
      LilyMirInstruction, alloc, NEW(LilyMirInstructionAlloc, dt));
}

inline Usize
LilyMirGetBlockId(LilyMirModule *Module)
{
    return CAST(LilyMirCurrent *, Module->current->top)->inst->fun.block_count;
}

LilyMirInstruction *
LilyMirBuildRetUnit(LilyMirModule *Module);

LilyMirInstruction *
LilyMirBuildRet(LilyMirModule *Module);

inline LilyMirInstruction *
LilyMirBuildJmp(LilyMirModule *Module, LilyMirInstructionBlock *block)
{
    return NEW_VARIANT(LilyMirInstruction, jmp, block);
}

inline LilyMirInstruction *
LilyMirBuildJmpWithInst(LilyMirModule *Module, LilyMirInstruction *block)
{
    return NEW_VARIANT(LilyMirInstruction, jmp, &block->block);
}

LilyMirInstruction *
LilyMirBuildJmpCondWithInst(LilyMirModule *Module,
                            LilyMirInstruction *Cond,
                            LilyMirInstruction *ThenBlock,
                            LilyMirInstruction *ElseBlock);

/**
 *
 * @param inst LilyMirInstruction*
 * @note The pointer `inst` is free.
 */
LilyMirInstruction *
LilyMirLoadFromVal(LilyMirModule *Module, LilyMirInstruction *inst);

/**
 *
 * @param dt LilyMirDt*
 * @param value_nam e.g. name of the variable.
 */
LilyMirInstructionVal *
LilyMirBuildLoad(LilyMirModule *Module,
                 LilyMirInstructionVal *src,
                 LilyMirDt *dt,
                 LilyMirInstructionFunLoadName value_name);

inline void
LilyMirBuildVarAlloc(LilyMirModule *Module, char *name, LilyMirDt *dt)
{
    LilyMirAddInst(
      Module,
      NEW_VARIANT(
        LilyMirInstruction,
        var,
        NEW(LilyMirInstructionVar, name, LilyMirBuildAlloc(Module, dt))));
}

void
LilyMirBuildVar(LilyMirModule *Module,
                char *name,
                LilyMirDt *dt,
                LilyMirInstruction *inst,
                bool allow_alloc);

inline LilyMirInstructionVal *
LilyMirBuildStruct(LilyMirModule *Module, LilyMirDt *dt, Vec *struct_)
{
    return NEW_VARIANT(LilyMirInstructionVal, struct, dt, struct_);
}

/**
 *
 * @note dest and src params could be free in this function.
 * @param dest LilyMirInstructionVal*
 * @param src LilyMirInstructionVal*
 * @return LilyMirInstruction*?
 */
LilyMirInstruction *
LilyMirBuildStore(LilyMirModule *Module,
                  LilyMirInstructionVal *dest,
                  LilyMirInstructionVal *src);

LilyMirInstruction *
LilyMirBuildCall(LilyMirModule *Module,
                 LilyMirDt *ReturnDt,
                 const char *Name,
                 Vec *Params);

LilyMirInstruction *
LilyMirBuildSysCall(LilyMirModule *Module,
                    LilyMirDt *ReturnDt,
                    const char *Name,
                    Vec *Params);

LilyMirInstruction *
LilyMirBuildBuiltinCall(LilyMirModule *Module,
                        LilyMirDt *ReturnDt,
                        const char *Name,
                        Vec *Params);

inline LilyMirInstruction *
LilyMirBuildGetField(LilyMirModule *Module,
                     LilyMirDt *dt,
                     LilyMirInstructionVal *val,
                     Vec *indexes)
{
    return NEW_VARIANT(LilyMirInstruction,
                       getfield,
                       NEW(LilyMirInstructionGetField, dt, val, indexes));
}

LilyMirInstructionVal *
LilyMirGetValFromInst(LilyMirInstruction *inst);

bool
LilyMirValidTypesOfFun(LilyMirInstructionFun *Fun,
                       LilyMirDt **Types,
                       Usize Len);

const char *
LilyMirGetFunNameFromTypes(LilyMirModule *Module,
                           const char *BaseName,
                           LilyMirDt **Types,
                           Usize Len);

void
LilyMirAddFinalInstruction(LilyMirModule *Module,
                           LilyMirInstructionBlock *exit_block);

void
LilyMirAddFinalInstructionWithInst(LilyMirModule *Module,
                                   LilyMirInstruction *exit_block);

bool
LilyMirHasFinalInstruction(LilyMirModule *Module);

bool
LilyMirHasRetInstruction(LilyMirModule *Module);

inline void
LilyMirSetBlockLimit(LilyMirBlockLimit *block_limit, Usize id)
{
    block_limit->id = id;
    block_limit->is_set = true;
}

/// @param signature LilyCheckedSignatureFun*? (&)
void
LilyMirBuildIfBranch(LilyMirModule *Module,
                     LilyCheckedSignatureFun *fun_signature,
                     LilyMirScope *scope,
                     const LilyCheckedStmtIfBranch *if_branch,
                     LilyMirInstruction *next_block,
                     LilyMirInstruction *exit_block);

/// @param signature LilyCheckedSignatureFun*? (&)
void
LilyMirBuildElifBranch(LilyMirModule *Module,
                       LilyCheckedSignatureFun *fun_signature,
                       LilyMirScope *scope,
                       const LilyCheckedStmtIfBranch *elif_branch,
                       LilyMirInstruction *next_block,
                       LilyMirInstruction *current_block,
                       LilyMirInstruction *exit_block);

/// @param signature LilyCheckedSignatureFun*? (&)
void
LilyMirBuildElseBranch(LilyMirModule *Module,
                       LilyCheckedSignatureFun *fun_signature,
                       LilyMirScope *scope,
                       const LilyCheckedStmtElseBranch *else_branch,
                       LilyMirInstruction *current_block,
                       LilyMirInstruction *exit_block);

/// @param fun_signature LilyCheckedSignatureFun*? (&)
void
LilyMirBuildIf(LilyMirModule *Module,
               LilyCheckedSignatureFun *fun_signature,
               LilyMirScope *scope,
               LilyMirBlockLimit *parent_block_limit,
               const LilyCheckedStmtIf *if_stmt);

/// @param fun_signature LilyCheckedSignatureFun*? (&)
void
LilyMirBuildWhile(LilyMirModule *Module,
                  LilyCheckedSignatureFun *fun_signature,
                  LilyMirScope *scope,
                  LilyMirBlockLimit *parent_block_limit,
                  const LilyCheckedStmtWhile *while_stmt);

/// @param fun_signature LilyCheckedSignatureFun*? (&)
void
LilyMirBuildBlockStmt(LilyMirModule *Module,
                      LilyCheckedSignatureFun *fun_signature,
                      LilyMirScope *scope,
                      LilyMirBlockLimit *parent_block_limit,
                      const LilyCheckedStmtBlock *block_stmt);

void
LilyMirBuildBreak(LilyMirModule *Module, LilyMirInstruction *exit_block);

void
LilyMirBuildNext(LilyMirModule *Module, LilyMirInstruction *next_block);

void
LilyMirBuildUnsafe(LilyMirModule *Module,
                   LilyCheckedSignatureFun *fun_signature,
                   LilyMirScope *scope,
                   LilyMirBlockLimit *parent_block_limit,
                   const LilyCheckedStmtUnsafe *unsafe_stmt);

void
LilyMirBuildSwitch(LilyMirModule *Module,
                   LilyCheckedSignatureFun *fun_signature,
                   LilyMirScope *scope,
                   LilyMirBlockLimit *parent_block_limit,
                   const LilyCheckedStmtSwitch *switch_stmt);

inline LilyMirCurrent *
LilyMirGetCurrentOnTop(LilyMirModule *Module)
{
    return Module->current->top;
}

/**
 *
 * @brief Check if the key of the instruction is unique.
 */
inline bool
LilyMirKeyIsUnique(LilyMirModule *Module, char *key)
{
    return !get__OrderedHashMap(Module->insts, key);
}

#endif // LILY_CORE_LILY_MIR_H
