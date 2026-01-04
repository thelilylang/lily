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

#ifndef LILY_EX_LIB_LILY_CORE_LILY_MIR_C
#define LILY_EX_LIB_LILY_CORE_LILY_MIR_C

#include <core/lily/mir/block_limit.h>
#include <core/lily/mir/debug_info.h>
#include <core/lily/mir/dt.h>
#include <core/lily/mir/instruction.h>
#include <core/lily/mir/mir.h>
#include <core/lily/mir/name_manager.h>
#include <core/lily/mir/scope.h>

#include "lily_base.c"
#include "lily_core_lily_analysis.c"

// <core/lily/mir/block_limit.h>
extern inline LilyMirBlockLimit *
ref__LilyMirBlockLimit(LilyMirBlockLimit *self);

// <core/lily/mir/debug_info.h>
extern inline CONSTRUCTOR(LilyMirDebugInfoFile,
                          LilyMirDebugInfoFile,
                          String *filename,
                          String *directory);

extern inline CONSTRUCTOR(LilyMirDebugInfoBlock,
                          LilyMirDebugInfoBlock,
                          const LilyMirDebugInfo *scope,
                          const LilyMirDebugInfoFile *file,
                          Usize line,
                          Usize column);

extern inline CONSTRUCTOR(LilyMirDebugInfoLocation,
                          LilyMirDebugInfoLocation,
                          const LilyMirDebugInfo *scope,
                          Usize line,
                          Usize column);

extern inline CONSTRUCTOR(LilyMirDebugInfoSubProgram,
                          LilyMirDebugInfoSubProgram,
                          const LilyMirDebugInfo *scope,
                          const LilyMirDebugInfoFile *file,
                          Usize line,
                          Usize column);

extern inline CONSTRUCTOR(LilyMirDebugInfoEnumerator,
                          LilyMirDebugInfoEnumerator,
                          const char *name,
                          Usize value,
                          bool is_unsigned);

extern inline CONSTRUCTOR(LilyMirDebugInfoGlobalVariable,
                          LilyMirDebugInfoGlobalVariable,
                          const LilyMirDebugInfo *scope,
                          const LilyMirDebugInfoFile *file,
                          const char *name,
                          const char *linkage_name,
                          bool is_local,
                          bool is_definition);

extern inline CONSTRUCTOR(LilyMirDebugInfoLocalVariable,
                          LilyMirDebugInfoLocalVariable,
                          const LilyMirDebugInfo *scope,
                          const LilyMirDebugInfoFile *file,
                          const LilyMirDebugInfo *type,
                          const char *name,
                          Usize arg_count,
                          Usize line);

extern inline CONSTRUCTOR(LilyMirDebugInfoType,
                          LilyMirDebugInfoType,
                          const char *name,
                          Usize size,
                          enum LilyMirDebugInfoEncoding encoding);

extern inline CONSTRUCTOR(LilyMirDebugInfoDerivedType,
                          LilyMirDebugInfoDerivedType,
                          const LilyMirDebugInfo *scope,
                          const LilyMirDebugInfo *base_type,
                          enum LilyMirDebugInfoTag tag,
                          const char *name,
                          Usize size,
                          Usize align,
                          Usize offset);

extern inline CONSTRUCTOR(LilyMirDebugInfoCompositeType,
                          LilyMirDebugInfoCompositeType,
                          enum LilyMirDebugInfoTag tag,
                          const char *name,
                          Usize size,
                          Usize align,
                          const LilyMirDebugInfo *elements);

extern inline CONSTRUCTOR(LilyMirDebugInfoElements,
                          LilyMirDebugInfoElements,
                          Vec *items);

extern inline CONSTRUCTOR(LilyMirDebugInfoManager, LilyMirDebugInfoManager);

// <core/lily/mir/dt.h>
extern inline VARIANT_CONSTRUCTOR(LilyMirDtLen, LilyMirDtLen, undef);

extern inline VARIANT_CONSTRUCTOR(LilyMirDtLen, LilyMirDtLen, def, Usize len);

extern inline LilyMirDtLen
clone__LilyMirDtLen(const LilyMirDtLen *self);

extern inline bool
eq__LilyMirDtLen(const LilyMirDtLen *self, const LilyMirDtLen *other);

extern inline LilyMirDtLen
from_check_len__LilyMirDtLen(const LilyCheckedDataTypeLen *self);

extern inline CONSTRUCTOR(LilyMirDtArray,
                          LilyMirDtArray,
                          LilyMirDtLen len,
                          LilyMirDt *dt);

extern inline DESTRUCTOR(LilyMirDtArray, const LilyMirDtArray *self);

extern inline CONSTRUCTOR(LilyMirDtResult,
                          LilyMirDtResult,
                          LilyMirDt *ok,
                          LilyMirDt *err);

extern inline DESTRUCTOR(LilyMirDtResult, const LilyMirDtResult *self);

// <core/lily/mir/instruction.h>
extern inline LilyMirInstructionVal *
ref__LilyMirInstructionVal(LilyMirInstructionVal *self);

extern inline CONSTRUCTOR(LilyMirInstructionDestSrc,
                          LilyMirInstructionDestSrc,
                          LilyMirInstructionVal *dest,
                          LilyMirInstructionVal *src);

extern inline DESTRUCTOR(LilyMirInstructionDestSrc,
                         const LilyMirInstructionDestSrc *self);

extern inline CONSTRUCTOR(LilyMirInstructionSrc,
                          LilyMirInstructionSrc,
                          LilyMirInstructionVal *src);

extern inline DESTRUCTOR(LilyMirInstructionSrc,
                         const LilyMirInstructionSrc *self);

extern inline CONSTRUCTOR(LilyMirInstructionAlloc,
                          LilyMirInstructionAlloc,
                          LilyMirDt *dt);

extern inline DESTRUCTOR(LilyMirInstructionAlloc,
                         const LilyMirInstructionAlloc *self);

extern inline CONSTRUCTOR(LilyMirInstructionArg,
                          LilyMirInstructionArg,
                          LilyMirDt *dt,
                          Usize id);

extern inline DESTRUCTOR(LilyMirInstructionArg,
                         const LilyMirInstructionArg *self);

extern inline CONSTRUCTOR(LilyMirInstructionAsm,
                          LilyMirInstructionAsm,
                          const char *content);

extern inline CONSTRUCTOR(LilyMirInstructionValDt,
                          LilyMirInstructionValDt,
                          LilyMirInstructionVal *val,
                          LilyMirDt *dt);

extern inline DESTRUCTOR(LilyMirInstructionValDt,
                         const LilyMirInstructionValDt *self);

extern inline CONSTRUCTOR(LilyMirInstructionBlock,
                          LilyMirInstructionBlock,
                          const char *name,
                          LilyMirBlockLimit *limit,
                          Usize id);

extern inline CONSTRUCTOR(LilyMirInstructionCall,
                          LilyMirInstructionCall,
                          LilyMirDt *return_dt,
                          const char *name,
                          Vec *params);

extern inline CONSTRUCTOR(LilyMirInstructionConst,
                          LilyMirInstructionConst,
                          enum LilyMirLinkage linkage,
                          const char *name,
                          LilyMirInstructionVal *val);

extern inline DESTRUCTOR(LilyMirInstructionConst,
                         const LilyMirInstructionConst *self);

extern inline VARIANT_CONSTRUCTOR(LilyMirInstructionFunLoadName,
                                  LilyMirInstructionFunLoadName,
                                  const,
                                  const char *const_);

extern inline VARIANT_CONSTRUCTOR(LilyMirInstructionFunLoadName,
                                  LilyMirInstructionFunLoadName,
                                  param,
                                  Usize param);

extern inline VARIANT_CONSTRUCTOR(LilyMirInstructionFunLoadName,
                                  LilyMirInstructionFunLoadName,
                                  reg,
                                  const char *reg);

extern inline VARIANT_CONSTRUCTOR(LilyMirInstructionFunLoadName,
                                  LilyMirInstructionFunLoadName,
                                  var,
                                  const char *var);

extern inline DESTRUCTOR(LilyMirInstructionFunLoad,
                         LilyMirInstructionFunLoad *self);

extern inline CONSTRUCTOR(LilyMirInstructionFunPrototype,
                          LilyMirInstructionFunPrototype,
                          const char *name,
                          Vec *params,
                          LilyMirDt *return_data_type,
                          enum LilyMirLinkage linkage);

extern inline CONSTRUCTOR(LilyMirInstructionGetArray,
                          LilyMirInstructionGetArray,
                          LilyMirDt *dt,
                          LilyMirInstructionVal *val,
                          Vec *indexes,
                          bool is_const);

extern inline CONSTRUCTOR(LilyMirInstructionGetField,
                          LilyMirInstructionGetField,
                          LilyMirDt *dt,
                          LilyMirInstructionVal *val,
                          Vec *indexes);

extern inline CONSTRUCTOR(LilyMirInstructionLoad,
                          LilyMirInstructionLoad,
                          LilyMirInstructionSrc src,
                          LilyMirDt *dt);

extern inline CONSTRUCTOR(LilyMirInstructionJmpCond,
                          LilyMirInstructionJmpCond,
                          LilyMirInstructionVal *cond,
                          LilyMirInstructionBlock *then_block,
                          LilyMirInstructionBlock *else_block);

extern inline DESTRUCTOR(LilyMirInstructionJmpCond,
                         const LilyMirInstructionJmpCond *self);

extern inline CONSTRUCTOR(LilyMirInstructionReg,
                          LilyMirInstructionReg,
                          const char *name,
                          LilyMirInstruction *inst);

extern inline CONSTRUCTOR(LilyMirInstructionVar,
                          LilyMirInstructionVar,
                          char *name,
                          LilyMirInstruction *inst);

extern inline CONSTRUCTOR(LilyMirInstructionSwitch,
                          LilyMirInstructionSwitch,
                          LilyMirInstructionVal *val,
                          LilyMirInstructionBlock *default_block,
                          Vec *cases);

extern inline CONSTRUCTOR(LilyMirInstructionTry,
                          LilyMirInstructionTry,
                          LilyMirInstructionVal *val,
                          LilyMirInstructionBlock *try_block,
                          LilyMirInstructionVal *catch_val,
                          LilyMirInstructionBlock *catch_block);

extern inline DESTRUCTOR(LilyMirInstructionTry,
                         const LilyMirInstructionTry *self);

extern inline CONSTRUCTOR(LilyMirInstructionStruct,
                          LilyMirInstructionStruct,
                          enum LilyMirLinkage linkage,
                          const char *name,
                          Vec *fields,
                          OrderedHashMap *generic_params);

// <core/lily/mir/mir.h>
extern inline DESTRUCTOR(LilyMirCurrent, LilyMirCurrent *self);

extern inline LilyMirModule
LilyMirCreateModule();

extern inline void
LilyMirAddVar(LilyMirScope *Scope,
              String *name,
              LilyCheckedDataType *data_type);

extern inline void
LilyMirAddParam(LilyMirScope *Scope, LilyCheckedDataType *data_type);

extern inline void
LilyMirNextBlock(LilyMirModule *Module);

extern inline const char *
LilyMirGetLastRegName(LilyMirModule *Module);

extern inline LilyMirInstructionVal *
LilyMirBuildRegVal(LilyMirModule *Module, LilyMirDt *dt, const char *name);

extern inline LilyMirInstruction *
LilyMirBuildAlloc(LilyMirModule *Module, LilyMirDt *dt);

extern inline Usize
LilyMirGetBlockId(LilyMirModule *Module);

extern inline LilyMirInstruction *
LilyMirBuildJmp(LilyMirModule *Module, LilyMirInstructionBlock *block);

extern inline LilyMirInstruction *
LilyMirBuildJmpWithInst(LilyMirModule *Module, LilyMirInstruction *block);

extern inline void
LilyMirBuildVarAlloc(LilyMirModule *Module, char *name, LilyMirDt *dt);

extern inline LilyMirInstructionVal *
LilyMirBuildStruct(LilyMirModule *Module, LilyMirDt *dt, Vec *struct_);

extern inline LilyMirInstruction *
LilyMirBuildGetField(LilyMirModule *Module,
                     LilyMirDt *dt,
                     LilyMirInstructionVal *val,
                     Vec *indexes);

extern inline void
LilyMirSetBlockLimit(LilyMirBlockLimit *block_limit, Usize id);

extern inline LilyMirCurrent *
LilyMirGetCurrentOnTop(LilyMirModule *Module);

extern inline bool
LilyMirKeyIsUnique(LilyMirModule *Module, char *key);

// <core/lily/mir/name_manager.h>
extern inline CONSTRUCTOR(LilyMirNameManager,
                          LilyMirNameManager,
                          const char *base_name);

// <core/lily/mir/scope.h>
extern inline DESTRUCTOR(LilyMirScopeParam, LilyMirScopeParam *self);

extern inline DESTRUCTOR(LilyMirScopeVar, LilyMirScopeVar *self);

#endif // LILY_EX_LIB_LILY_CORE_LILY_MIR_C
