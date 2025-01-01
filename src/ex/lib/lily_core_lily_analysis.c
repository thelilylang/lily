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

#ifndef LILY_EX_LIB_LILY_CORE_LILY_ANALYSIS_C
#define LILY_EX_LIB_LILY_CORE_LILY_ANALYSIS_C

#include <core/lily/analysis/analysis.h>
#include <core/lily/analysis/checked/access.h>
#include <core/lily/analysis/checked/body/fun.h>
#include <core/lily/analysis/checked/captured_variable.h>
#include <core/lily/analysis/checked/compiler_generic.h>
#include <core/lily/analysis/checked/data_type.h>
#include <core/lily/analysis/checked/decl/alias.h>
#include <core/lily/analysis/checked/decl/attribute.h>
#include <core/lily/analysis/checked/decl/class.h>
#include <core/lily/analysis/checked/decl/constant.h>
#include <core/lily/analysis/checked/decl/enum.h>
#include <core/lily/analysis/checked/decl/enum_object.h>
#include <core/lily/analysis/checked/decl/error.h>
#include <core/lily/analysis/checked/decl/fun.h>
#include <core/lily/analysis/checked/decl/method.h>
#include <core/lily/analysis/checked/decl/module.h>
#include <core/lily/analysis/checked/decl/object.h>
#include <core/lily/analysis/checked/decl/prototype.h>
#include <core/lily/analysis/checked/decl/record.h>
#include <core/lily/analysis/checked/decl/record_object.h>
#include <core/lily/analysis/checked/decl/trait.h>
#include <core/lily/analysis/checked/decl/type.h>
#include <core/lily/analysis/checked/expr.h>
#include <core/lily/analysis/checked/expr/access.h>
#include <core/lily/analysis/checked/expr/array.h>
#include <core/lily/analysis/checked/expr/binary.h>
#include <core/lily/analysis/checked/expr/call.h>
#include <core/lily/analysis/checked/expr/cast.h>
#include <core/lily/analysis/checked/expr/compiler_fun.h>
#include <core/lily/analysis/checked/expr/lambda.h>
#include <core/lily/analysis/checked/expr/list.h>
#include <core/lily/analysis/checked/expr/literal.h>
#include <core/lily/analysis/checked/expr/tuple.h>
#include <core/lily/analysis/checked/expr/unary.h>
#include <core/lily/analysis/checked/field_object.h>
#include <core/lily/analysis/checked/generic_param.h>
#include <core/lily/analysis/checked/global_name.h>
#include <core/lily/analysis/checked/history.h>
#include <core/lily/analysis/checked/operator.h>
#include <core/lily/analysis/checked/operator_register.h>
#include <core/lily/analysis/checked/parent.h>
#include <core/lily/analysis/checked/pattern/array.h>
#include <core/lily/analysis/checked/pattern/as.h>
#include <core/lily/analysis/checked/pattern/list.h>
#include <core/lily/analysis/checked/pattern/list_head.h>
#include <core/lily/analysis/checked/pattern/list_tail.h>
#include <core/lily/analysis/checked/pattern/literal.h>
#include <core/lily/analysis/checked/pattern/name.h>
#include <core/lily/analysis/checked/pattern/range.h>
#include <core/lily/analysis/checked/pattern/record_call.h>
#include <core/lily/analysis/checked/pattern/table.h>
#include <core/lily/analysis/checked/pattern/tuple.h>
#include <core/lily/analysis/checked/pattern/variant_call.h>
#include <core/lily/analysis/checked/scope.h>
#include <core/lily/analysis/checked/scope_container.h>
#include <core/lily/analysis/checked/scope_decls.h>
#include <core/lily/analysis/checked/scope_response.h>
#include <core/lily/analysis/checked/scope_stmt.h>
#include <core/lily/analysis/checked/signature.h>
#include <core/lily/analysis/checked/stmt.h>
#include <core/lily/analysis/checked/stmt/asm.h>
#include <core/lily/analysis/checked/stmt/await.h>
#include <core/lily/analysis/checked/stmt/block.h>
#include <core/lily/analysis/checked/stmt/break.h>
#include <core/lily/analysis/checked/stmt/drop.h>
#include <core/lily/analysis/checked/stmt/if.h>
#include <core/lily/analysis/checked/stmt/match.h>
#include <core/lily/analysis/checked/stmt/next.h>
#include <core/lily/analysis/checked/stmt/raise.h>
#include <core/lily/analysis/checked/stmt/return.h>
#include <core/lily/analysis/checked/stmt/switch.h>
#include <core/lily/analysis/checked/stmt/try.h>
#include <core/lily/analysis/checked/stmt/unsafe.h>
#include <core/lily/analysis/checked/stmt/variable.h>
#include <core/lily/analysis/checked/stmt/while.h>
#include <core/lily/analysis/checked/virtual_scope.h>
#include <core/lily/analysis/checked/virtual_variable_manager.h>

#include "lily_core_lily_functions.c"

// <core/lily/analysis/analysis.h>
extern inline CONSTRUCTOR(LilyAnalysis,
                          LilyAnalysis,
                          LilyPackage *package,
                          LilyPackage *root_package,
                          const LilyParser *parser,
                          bool use_switch);

// <core/lily/analysis/checked/access.h>
extern inline DESTRUCTOR(LilyCheckedAccessScope, LilyCheckedAccessScope *self);

extern inline CONSTRUCTOR(LilyCheckedAccessModule,
                          LilyCheckedAccessModule,
                          Usize id);

extern inline bool
eq__LilyCheckedAccessModule(const LilyCheckedAccessModule *self,
                            const LilyCheckedAccessModule *other);

extern inline CONSTRUCTOR(LilyCheckedAccessConstant,
                          LilyCheckedAccessConstant,
                          LilyCheckedAccessScope scope,
                          Usize id);

extern inline CONSTRUCTOR(LilyCheckedAccessEnum,
                          LilyCheckedAccessEnum,
                          LilyCheckedAccessScope scope,
                          Usize id);

extern inline CONSTRUCTOR(LilyCheckedAccessRecord,
                          LilyCheckedAccessRecord,
                          LilyCheckedAccessScope scope,
                          Usize id);

extern inline CONSTRUCTOR(LilyCheckedAccessAlias,
                          LilyCheckedAccessAlias,
                          LilyCheckedAccessScope scope,
                          Usize id);

extern inline CONSTRUCTOR(LilyCheckedAccessError,
                          LilyCheckedAccessError,
                          LilyCheckedAccessScope scope,
                          Usize id);

extern inline CONSTRUCTOR(LilyCheckedAccessEnumObject,
                          LilyCheckedAccessEnumObject,
                          LilyCheckedAccessScope scope,
                          Usize id);

extern inline CONSTRUCTOR(LilyCheckedAccessRecordObject,
                          LilyCheckedAccessRecordObject,
                          LilyCheckedAccessScope scope,
                          Usize id);

extern inline CONSTRUCTOR(LilyCheckedAccessClass,
                          LilyCheckedAccessClass,
                          LilyCheckedAccessScope scope,
                          Usize id);

extern inline CONSTRUCTOR(LilyCheckedAccessTrait,
                          LilyCheckedAccessTrait,
                          LilyCheckedAccessScope scope,
                          Usize id);

extern inline DESTRUCTOR(LilyCheckedAccessFun, LilyCheckedAccessFun *self);

extern inline DESTRUCTOR(LilyCheckedAccessLabel, LilyCheckedAccessLabel *self);

// <core/lily/analysis/checked/captured_variable.h>
extern inline DESTRUCTOR(LilyCheckedCapturedVariable,
                         LilyCheckedCapturedVariable *self);

// <core/lily/analysis/checked/compiler_generic.h>
extern inline DESTRUCTOR(LilyCheckedCompilerGenericValue,
                         LilyCheckedCompilerGenericValue *self);

extern inline void
add_compiler_generic__LilyCheckedCompilerGeneric(Vec *used_compiler_generic);

// <core/lily/analysis/checked/data_type.h>
extern inline CONSTRUCTOR(LilyCheckedDataTypeLambda,
                          LilyCheckedDataTypeLambda,
                          Vec *params,
                          LilyCheckedDataType *return_type);

extern inline CONSTRUCTOR(LilyCheckedDataTypeArray,
                          LilyCheckedDataTypeArray,
                          enum LilyCheckedDataTypeArrayKind kind,
                          LilyCheckedDataType *data_type);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedDataTypeArray,
                                  LilyCheckedDataTypeArray,
                                  sized,
                                  LilyCheckedDataType *data_type,
                                  Usize sized);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedDataTypeArray,
                                  LilyCheckedDataTypeArray,
                                  unknown,
                                  LilyCheckedDataType *data_type,
                                  Usize unknown);

extern inline CONSTRUCTOR(LilyCheckedDataTypeCustom,
                          LilyCheckedDataTypeCustom,
                          Usize scope_id,
                          LilyCheckedAccessScope scope,
                          String *name,
                          String *global_name,
                          Vec *generics,
                          enum LilyCheckedDataTypeCustomKind kind,
                          bool is_recursive);

extern inline LilyCheckedDataTypeCondition *
ref__LilyCheckedDataTypeCondition(LilyCheckedDataTypeCondition *self);

extern inline CONSTRUCTOR(LilyCheckedDataTypeConditionalCompilerChoice,
                          LilyCheckedDataTypeConditionalCompilerChoice,
                          Vec *choices,
                          Vec *conds);

extern inline CONSTRUCTOR(LilyCheckedDataTypeCompilerGeneric,
                          LilyCheckedDataTypeCompilerGeneric,
                          String *name);

extern inline LilyCheckedDataTypeResult *
ref__LilyCheckedDataTypeResult(LilyCheckedDataTypeResult *self);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedDataTypeLen,
                                  LilyCheckedDataTypeLen,
                                  undef);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedDataTypeLen,
                                  LilyCheckedDataTypeLen,
                                  def,
                                  Usize len);

extern inline LilyCheckedDataTypeLen
clone__LilyCheckedDataTypeLen(const LilyCheckedDataTypeLen *self);

extern inline bool
eq__LilyCheckedDataTypeLen(const LilyCheckedDataTypeLen *self,
                           const LilyCheckedDataTypeLen *other);

extern inline bool
is_literal_data_type__LilyCheckedDataType(LilyCheckedDataType *self);

extern inline bool
is_known_data_type__LilyCheckedDataType(LilyCheckedDataType *self);

extern inline bool
is_compiler_defined_and_known_dt__LilyCheckedDataType(
  LilyCheckedDataType *self);

extern inline bool
is_compiler_defined__LilyCheckedDataType(LilyCheckedDataType *self);

extern inline bool
is_unknown_data_type__LilyCheckedDataType(LilyCheckedDataType *self);

extern inline LilyCheckedDataType *
ref__LilyCheckedDataType(LilyCheckedDataType *self);

// <core/lily/analysis/checked/expr.h>
extern inline LilyCheckedExpr *
ref__LilyCheckedExpr(LilyCheckedExpr *self);

extern inline bool
is_node__LilyCheckedExpr(const LilyCheckedExpr *self);

// <core/lily/analysis/checked/field_object.h>
extern inline CONSTRUCTOR(LilyCheckedFieldObject,
                          LilyCheckedFieldObject,
                          String *name,
                          LilyCheckedDataType *data_type,
                          LilyCheckedExpr *optional_expr,
                          enum LilyVisibility visibility);

// <core/lily/analysis/checked/generic_param.h>
extern inline CONSTRUCTOR(LilyCheckedGenericParamConstraint,
                          LilyCheckedGenericParamConstraint,
                          String *name,
                          Vec *constraints);

// <core/lily/analysis/checked/global_name.h>
extern inline void
generate_global_variant_name__LilyCheckedGlobalName(
  String *global_name,
  LilyCheckedDataType *resolve_dt);

// <core/lily/analysis/checked/history.h>
extern inline void
add__LilyCheckedHistory(LilyCheckedHistory *self, LilyCheckedDecl *item);

extern inline void
pop__LilyCheckedHistory(LilyCheckedHistory *self);

// <core/lily/analysis/checked/operator_register.h>
extern inline CONSTRUCTOR(LilyCheckedOperatorRegister,
                          LilyCheckedOperatorRegister);

// <core/lily/analysis/checked/operator.h>
extern inline LilyCheckedOperator *
ref__LilyCheckedOperator(LilyCheckedOperator *self);

// <core/lily/analysis/checked/parent.h>
extern inline DESTRUCTOR(LilyCheckedParent, LilyCheckedParent *self);

// <core/lily/analysis/checked/scope_container.h>
extern inline DESTRUCTOR(LilyCheckedScopeContainerCapturedVariable,
                         LilyCheckedScopeContainerCapturedVariable *self);

extern inline DESTRUCTOR(LilyCheckedScopeContainerModule,
                         LilyCheckedScopeContainerModule *self);

extern inline DESTRUCTOR(LilyCheckedScopeContainerConstant,
                         LilyCheckedScopeContainerConstant *self);

extern inline DESTRUCTOR(LilyCheckedScopeContainerEnum,
                         LilyCheckedScopeContainerEnum *self);

extern inline DESTRUCTOR(LilyCheckedScopeContainerRecord,
                         LilyCheckedScopeContainerRecord *self);

extern inline DESTRUCTOR(LilyCheckedScopeContainerAlias,
                         LilyCheckedScopeContainerAlias *self);

extern inline DESTRUCTOR(LilyCheckedScopeContainerError,
                         LilyCheckedScopeContainerError *self);

extern inline DESTRUCTOR(LilyCheckedScopeContainerEnumObject,
                         LilyCheckedScopeContainerEnumObject *self);

extern inline DESTRUCTOR(LilyCheckedScopeContainerRecordObject,
                         LilyCheckedScopeContainerRecordObject *self);

extern inline DESTRUCTOR(LilyCheckedScopeContainerClass,
                         LilyCheckedScopeContainerClass *self);

extern inline DESTRUCTOR(LilyCheckedScopeContainerTrait,
                         LilyCheckedScopeContainerTrait *self);

extern inline DESTRUCTOR(LilyCheckedScopeContainerLabel,
                         LilyCheckedScopeContainerLabel *self);

extern inline DESTRUCTOR(LilyCheckedScopeContainerVariable,
                         LilyCheckedScopeContainerVariable *self);

extern inline DESTRUCTOR(LilyCheckedScopeContainerGeneric,
                         LilyCheckedScopeContainerGeneric *self);

extern inline VARIANT_CONSTRUCTOR(
  LilyCheckedScopeContainer,
  LilyCheckedScopeContainer,
  captured_variable,
  Usize scope_id,
  const LilyCheckedScopeContainerCapturedVariable *captured_variable);

extern inline VARIANT_CONSTRUCTOR(
  LilyCheckedScopeContainer,
  LilyCheckedScopeContainer,
  module,
  Usize scope_id,
  const LilyCheckedScopeContainerModule *module);

extern inline VARIANT_CONSTRUCTOR(
  LilyCheckedScopeContainer,
  LilyCheckedScopeContainer,
  constant,
  Usize scope_id,
  const LilyCheckedScopeContainerConstant *constant);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedScopeContainer,
                                  LilyCheckedScopeContainer,
                                  enum_,
                                  Usize scope_id,
                                  const LilyCheckedScopeContainerEnum *enum_);

extern inline VARIANT_CONSTRUCTOR(
  LilyCheckedScopeContainer,
  LilyCheckedScopeContainer,
  record,
  Usize scope_id,
  const LilyCheckedScopeContainerRecord *record);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedScopeContainer,
                                  LilyCheckedScopeContainer,
                                  alias,
                                  Usize scope_id,
                                  const LilyCheckedScopeContainerAlias *alias);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedScopeContainer,
                                  LilyCheckedScopeContainer,
                                  error,
                                  Usize scope_id,
                                  const LilyCheckedScopeContainerError *error);

extern inline VARIANT_CONSTRUCTOR(
  LilyCheckedScopeContainer,
  LilyCheckedScopeContainer,
  enum_object,
  Usize scope_id,
  const LilyCheckedScopeContainerEnumObject *enum_object);

extern inline VARIANT_CONSTRUCTOR(
  LilyCheckedScopeContainer,
  LilyCheckedScopeContainer,
  record_object,
  Usize scope_id,
  const LilyCheckedScopeContainerRecordObject *record_object);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedScopeContainer,
                                  LilyCheckedScopeContainer,
                                  class,
                                  Usize scope_id,
                                  const LilyCheckedScopeContainerClass *class);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedScopeContainer,
                                  LilyCheckedScopeContainer,
                                  trait,
                                  Usize scope_id,
                                  const LilyCheckedScopeContainerTrait *trait);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedScopeContainer,
                                  LilyCheckedScopeContainer,
                                  fun,
                                  Usize scope_id,
                                  const LilyCheckedScopeContainerFun *fun);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedScopeContainer,
                                  LilyCheckedScopeContainer,
                                  label,
                                  Usize scope_id,
                                  const LilyCheckedScopeContainerLabel *label);

extern inline VARIANT_CONSTRUCTOR(
  LilyCheckedScopeContainer,
  LilyCheckedScopeContainer,
  variable,
  Usize scope_id,
  const LilyCheckedScopeContainerVariable *variable);

extern inline VARIANT_CONSTRUCTOR(
  LilyCheckedScopeContainer,
  LilyCheckedScopeContainer,
  generic,
  Usize scope_id,
  const LilyCheckedScopeContainerGeneric *generic);

extern inline VARIANT_CONSTRUCTOR(
  LilyCheckedScopeContainer,
  LilyCheckedScopeContainer,
  method,
  Usize scope_id,
  const LilyCheckedScopeContainerMethod *method);

// <core/lily/analysis/checked/scope_decls.h>
extern inline VARIANT_CONSTRUCTOR(LilyCheckedScopeDecls,
                                  LilyCheckedScopeDecls,
                                  decl,
                                  LilyCheckedDecl *decl);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedScopeDecls,
                                  LilyCheckedScopeDecls,
                                  module,
                                  LilyCheckedDeclModule *module);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedScopeDecls,
                                  LilyCheckedScopeDecls,
                                  scope,
                                  Vec *scope);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedScopeDecls,
                                  LilyCheckedScopeDecls,
                                  stmt,
                                  LilyCheckedScopeStmt stmt);

// <core/lily/analysis/checked/scope_response.h>
extern inline CONSTRUCTOR(LilyCheckedScopeResponse, LilyCheckedScopeResponse);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedScopeResponse,
                                  LilyCheckedScopeResponse,
                                  catch_variable,
                                  const Location *location,
                                  HashMap *catch_variable);

extern inline VARIANT_CONSTRUCTOR(
  LilyCheckedScopeResponse,
  LilyCheckedScopeResponse,
  captured_variable,
  const Location *location,
  LilyCheckedScopeContainer scope_container,
  LilyCheckedCapturedVariable *captured_variable);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedScopeResponse,
                                  LilyCheckedScopeResponse,
                                  module,
                                  const Location *location,
                                  LilyCheckedScopeContainer scope_container,
                                  LilyCheckedDeclModule *module);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedScopeResponse,
                                  LilyCheckedScopeResponse,
                                  constant,
                                  const Location *location,
                                  LilyCheckedScopeContainer scope_container,
                                  LilyCheckedDeclConstant *constant);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedScopeResponse,
                                  LilyCheckedScopeResponse,
                                  enum_,
                                  const Location *location,
                                  LilyCheckedScopeContainer scope_container,
                                  LilyCheckedDecl *decl,
                                  LilyCheckedDeclEnum *enum_);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedScopeResponse,
                                  LilyCheckedScopeResponse,
                                  enum_variant,
                                  const Location *location,
                                  LilyCheckedScopeContainer scope_container,
                                  LilyCheckedVariant *enum_variant);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedScopeResponse,
                                  LilyCheckedScopeResponse,
                                  enum_variant_object,
                                  const Location *location,
                                  LilyCheckedScopeContainer scope_container,
                                  LilyCheckedVariant *enum_variant_object);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedScopeResponse,
                                  LilyCheckedScopeResponse,
                                  record,
                                  const Location *location,
                                  LilyCheckedScopeContainer scope_container,
                                  LilyCheckedDecl *decl,
                                  LilyCheckedDeclRecord *record);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedScopeResponse,
                                  LilyCheckedScopeResponse,
                                  record_field,
                                  const Location *location,
                                  LilyCheckedScopeContainer scope_container,
                                  LilyCheckedField *record_field);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedScopeResponse,
                                  LilyCheckedScopeResponse,
                                  record_field_object,
                                  const Location *location,
                                  LilyCheckedScopeContainer scope_container,
                                  LilyCheckedFieldObject *record_field_object);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedScopeResponse,
                                  LilyCheckedScopeResponse,
                                  alias,
                                  const Location *location,
                                  LilyCheckedScopeContainer scope_container,
                                  LilyCheckedDecl *decl,
                                  LilyCheckedDeclAlias *alias);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedScopeResponse,
                                  LilyCheckedScopeResponse,
                                  error,
                                  const Location *location,
                                  LilyCheckedScopeContainer scope_container,
                                  LilyCheckedDecl *decl,
                                  LilyCheckedDeclError *error);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedScopeResponse,
                                  LilyCheckedScopeResponse,
                                  enum_object,
                                  const Location *location,
                                  LilyCheckedScopeContainer scope_container,
                                  LilyCheckedDecl *decl,
                                  LilyCheckedDeclEnumObject *enum_object);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedScopeResponse,
                                  LilyCheckedScopeResponse,
                                  record_object,
                                  const Location *location,
                                  LilyCheckedScopeContainer scope_container,
                                  LilyCheckedDecl *decl,
                                  LilyCheckedDeclRecordObject *record_object);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedScopeResponse,
                                  LilyCheckedScopeResponse,
                                  class,
                                  const Location *location,
                                  LilyCheckedScopeContainer scope_container,
                                  LilyCheckedDecl *decl,
                                  LilyCheckedDeclClass *class);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedScopeResponse,
                                  LilyCheckedScopeResponse,
                                  trait,
                                  const Location *location,
                                  LilyCheckedScopeContainer scope_container,
                                  LilyCheckedDecl *decl,
                                  LilyCheckedDeclTrait *trait);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedScopeResponse,
                                  LilyCheckedScopeResponse,
                                  fun,
                                  const Location *location,
                                  LilyCheckedScopeContainer scope_container,
                                  Vec *fun);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedScopeResponse,
                                  LilyCheckedScopeResponse,
                                  label,
                                  const Location *location,
                                  LilyCheckedScopeContainer scope_container);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedScopeResponse,
                                  LilyCheckedScopeResponse,
                                  variable,
                                  const Location *location,
                                  LilyCheckedScopeContainer scope_container,
                                  LilyCheckedStmtVariable *variable);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedScopeResponse,
                                  LilyCheckedScopeResponse,
                                  fun_param,
                                  const Location *location,
                                  LilyCheckedScopeContainer scope_container,
                                  LilyCheckedDeclFunParam *fun_param);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedScopeResponse,
                                  LilyCheckedScopeResponse,
                                  method_param,
                                  const Location *location,
                                  LilyCheckedScopeContainer scope_container,
                                  LilyCheckedDeclMethodParam *method_param);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedScopeResponse,
                                  LilyCheckedScopeResponse,
                                  generic,
                                  const Location *location,
                                  LilyCheckedScopeContainer scope_container,
                                  LilyCheckedGenericParam *generic);

extern inline DESTRUCTOR(LilyCheckedScopeResponse,
                         const LilyCheckedScopeResponse *self);

// <core/lily/analysis/checked/scope_stmt.h>
extern inline VARIANT_CONSTRUCTOR(LilyCheckedScopeStmt,
                                  LilyCheckedScopeStmt,
                                  else_branch,
                                  const LilyCheckedStmtElseBranch *else_branch);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedScopeStmt,
                           LilyCheckedScopeStmt,
                           for,
                           const LilyCheckedStmtFor *for_);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedScopeStmt,
                                  LilyCheckedScopeStmt,
                                  if_branch,
                                  const LilyCheckedStmtIfBranch *if_branch);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedScopeStmt,
                                  LilyCheckedScopeStmt,
                                  match_case,
                                  const LilyCheckedStmtMatchCase *match_case);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedScopeStmt,
                                  LilyCheckedScopeStmt,
                                  while,
                                  const LilyCheckedStmtWhile *while_);

// <core/lily/analysis/checked/scope.h>
extern inline DESTRUCTOR(LilyCheckedScopeCatch, LilyCheckedScopeCatch *self);

// <core/lily/analysis/checked/signature.h>
extern inline void
add_signature__LilyCheckedSignatureVariant(String *global_name,
                                           LilyCheckedDataType *resolve_dt,
                                           Vec *signatures);

// <core/lily/analysis/checked/stmt.h>
extern inline VARIANT_CONSTRUCTOR(LilyCheckedStmt,
                                  LilyCheckedStmt,
                                  asm,
                                  const Location *location,
                                  const LilyAstStmt *ast_stmt,
                                  LilyCheckedStmtAsm asm_);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedStmt,
                                  LilyCheckedStmt,
                                  await,
                                  const Location *location,
                                  const LilyAstStmt *ast_stmt,
                                  LilyCheckedStmtAwait await);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedStmt,
                                  LilyCheckedStmt,
                                  block,
                                  const Location *location,
                                  const LilyAstStmt *ast_stmt,
                                  LilyCheckedStmtBlock block);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedStmt,
                                  LilyCheckedStmt,
                                  break,
                                  const Location *location,
                                  const LilyAstStmt *ast_stmt,
                                  LilyCheckedStmtBreak break_);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedStmt,
                                  LilyCheckedStmt,
                                  drop,
                                  const Location *location,
                                  const LilyAstStmt *ast_stmt,
                                  LilyCheckedStmtDrop drop);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedStmt,
                           LilyCheckedStmt,
                           for,
                           const Location *location,
                           const LilyAstStmt *ast_stmt,
                           LilyCheckedStmtFor for_);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedStmt,
                                  LilyCheckedStmt,
                                  if,
                                  const Location *location,
                                  const LilyAstStmt *ast_stmt,
                                  LilyCheckedStmtIf if_);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedStmt,
                                  LilyCheckedStmt,
                                  match,
                                  const Location *location,
                                  const LilyAstStmt *ast_stmt,
                                  LilyCheckedStmtMatch match);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedStmt,
                                  LilyCheckedStmt,
                                  next,
                                  const Location *location,
                                  const LilyAstStmt *ast_stmt,
                                  LilyCheckedStmtNext next);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedStmt,
                                  LilyCheckedStmt,
                                  raise,
                                  const Location *location,
                                  const LilyAstStmt *ast_stmt,
                                  LilyCheckedStmtRaise raise);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedStmt,
                                  LilyCheckedStmt,
                                  return,
                                  const Location *location,
                                  const LilyAstStmt *ast_stmt,
                                  LilyCheckedStmtReturn return_);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedStmt,
                                  LilyCheckedStmt,
                                  switch,
                                  const Location *location,
                                  const LilyAstStmt *ast_stmt,
                                  LilyCheckedStmtSwitch switch_);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedStmt,
                                  LilyCheckedStmt,
                                  try,
                                  const Location *location,
                                  const LilyAstStmt *ast_stmt,
                                  LilyCheckedStmtTry try);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedStmt,
                                  LilyCheckedStmt,
                                  unsafe,
                                  const Location *location,
                                  const LilyAstStmt *ast_stmt,
                                  LilyCheckedStmtUnsafe unsafe);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedStmt,
                                  LilyCheckedStmt,
                                  variable,
                                  const Location *location,
                                  const LilyAstStmt *ast_stmt,
                                  LilyCheckedStmtVariable variable);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedStmt,
                                  LilyCheckedStmt,
                                  while,
                                  const Location *location,
                                  const LilyAstStmt *ast_stmt,
                                  LilyCheckedStmtWhile while_);

// <core/lily/analysis/checked/virtual_scope.h>
extern inline DESTRUCTOR(LilyCheckedVirtualVariable,
                         LilyCheckedVirtualVariable *self);

extern inline DESTRUCTOR(LilyCheckedVirtualFunParam,
                         LilyCheckedVirtualFunParam *self);

extern inline void
add_fun_param__LilyCheckedVirtualScope(LilyCheckedVirtualScope *self,
                                       LilyCheckedVirtualFunParam *fun_param);

extern inline void
add_variable__LilyCheckedVirtualScope(LilyCheckedVirtualScope *self,
                                      LilyCheckedVirtualVariable *variable);

// <core/lily/analysis/checked/virtual_variable_manager.h>
extern inline CONSTRUCTOR(LilyCheckedVirtualVariableManager,
                          LilyCheckedVirtualVariableManager,
                          const char *base_name);

// <core/lily/analysis/checked/body/fun.h>
extern inline LilyCheckedBodyFunItem *
ref__LilyCheckedBodyFunItem(LilyCheckedBodyFunItem *self);

extern inline CONSTRUCTOR(LilyCheckedBodyFunInfo,
                          LilyCheckedBodyFunInfo,
                          Vec *body,
                          LilyCheckedScope *scope);

// <core/lily/analysis/checked/decl/alias.h>
extern inline CONSTRUCTOR(LilyCheckedDeclAlias,
                          LilyCheckedDeclAlias,
                          String *name,
                          String *global_name,
                          LilyCheckedScope *scope,
                          LilyCheckedScope *jump_scope,
                          Vec *generic_params,
                          const LilyCheckedSignatureType *signature,
                          LilyCheckedDataType *data_type,
                          enum LilyVisibility visibility);

// <core/lily/analysis/checked/decl/attribute.h>
extern inline CONSTRUCTOR(LilyCheckedDeclAttribute,
                          LilyCheckedDeclAttribute,
                          String *name,
                          String *global_name,
                          LilyCheckedDataType *data_type,
                          LilyCheckedExpr *optional_expr,
                          bool is_set,
                          bool is_get,
                          enum LilyVisibility visibility);

// <core/lily/analysis/checked/decl/class.h>
extern inline CONSTRUCTOR(LilyCheckedDeclClass,
                          LilyCheckedDeclClass,
                          String *name,
                          String *global_name,
                          Vec *generic_params,
                          Vec *inherit_params,
                          Vec *impl_params,
                          Vec *body,
                          LilyCheckedScope *scope,
                          enum LilyVisibility visibility,
                          bool is_close);

// <core/lily/analysis/checked/decl/constant.h>
extern inline CONSTRUCTOR(LilyCheckedDeclConstant,
                          LilyCheckedDeclConstant,
                          String *name,
                          String *global_name,
                          LilyCheckedDataType *data_type,
                          LilyCheckedExpr *expr,
                          enum LilyVisibility visibility);

// <core/lily/analysis/checked/decl/enum_object.h>
extern inline CONSTRUCTOR(LilyCheckedDeclEnumObject,
                          LilyCheckedDeclEnumObject,
                          String *name,
                          String *global_name,
                          Vec *generic_params,
                          Vec *impl_params,
                          Vec *body,
                          LilyCheckedScope *scope,
                          enum LilyVisibility visibility);

// <core/lily/analysis/checked/decl/enum.h>
extern inline CONSTRUCTOR(LilyCheckedDeclEnum,
                          LilyCheckedDeclEnum,
                          String *name,
                          String *global_name,
                          Vec *generic_params,
                          Vec *variants,
                          LilyCheckedScope *scope,
                          enum LilyVisibility visibility);

// <core/lily/analysis/checked/decl/error.h>
extern inline CONSTRUCTOR(LilyCheckedDeclError,
                          LilyCheckedDeclError,
                          String *name,
                          String *global_name,
                          Vec *generic_params,
                          LilyCheckedScope *scope,
                          LilyCheckedDataType *data_type,
                          enum LilyVisibility visibility);

// <core/lily/analysis/checked/decl/fun.h>
extern inline CONSTRUCTOR(LilyCheckedDeclFun,
                          LilyCheckedDeclFun,
                          String *name,
                          String *global_name,
                          Vec *generic_params,
                          Vec *params,
                          LilyAstDataType *default_return_data_type,
                          LilyCheckedDataType *return_data_type,
                          Vec *body,
                          LilyCheckedScope *scope,
                          enum LilyVisibility visibility,
                          bool is_async,
                          bool is_operator);

extern inline int
add_signature__LilyCheckedDeclFun(LilyCheckedDeclFun *self,
                                  Vec *types,
                                  HashMap *generic_params);

// <core/lily/analysis/checked/decl/method.h>
extern inline CONSTRUCTOR(LilyCheckedDeclMethod,
                          LilyCheckedDeclMethod,
                          String *name,
                          String *global_name,
                          String *object_impl,
                          Vec *generic_params,
                          Vec *params,
                          LilyCheckedDataType *return_data_type,
                          Vec *body,
                          LilyCheckedScope *scope,
                          LilyCheckedAccessFun *access,
                          enum LilyVisibility visibility,
                          bool is_async,
                          bool is_operator,
                          bool is_checked);

// <core/lily/analysis/checked/decl/module.h>
extern inline CONSTRUCTOR(LilyCheckedDeclModule,
                          LilyCheckedDeclModule,
                          String *name,
                          String *global_name,
                          Vec *decls,
                          LilyCheckedScope *scope,
                          enum LilyVisibility visibility);

// <core/lily/analysis/checked/decl/object.h>
extern inline VARIANT_CONSTRUCTOR(LilyCheckedDeclObject,
                                  LilyCheckedDeclObject,
                                  class,
                                  LilyCheckedDeclClass class);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedDeclObject,
                                  LilyCheckedDeclObject,
                                  enum,
                                  LilyCheckedDeclEnumObject enum_);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedDeclObject,
                                  LilyCheckedDeclObject,
                                  record,
                                  LilyCheckedDeclRecordObject record);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedDeclObject,
                                  LilyCheckedDeclObject,
                                  trait,
                                  LilyCheckedDeclTrait trait);

// <core/lily/analysis/checked/decl/prototype.h>
extern inline CONSTRUCTOR(LilyCheckedDeclPrototype,
                          LilyCheckedDeclPrototype,
                          String *name,
                          String *global_name,
                          Vec *generic_params,
                          Vec *params,
                          LilyCheckedDataType *return_data_type);

// <core/lily/analysis/checked/decl/record_object.h>
extern inline CONSTRUCTOR(LilyCheckedDeclRecordObject,
                          LilyCheckedDeclRecordObject,
                          String *name,
                          String *global_name,
                          Vec *generic_params,
                          Vec *impl_params,
                          Vec *body,
                          LilyCheckedScope *scope,
                          enum LilyVisibility visibility);

// <core/lily/analysis/checked/decl/record.h>
extern inline CONSTRUCTOR(LilyCheckedDeclRecord,
                          LilyCheckedDeclRecord,
                          String *name,
                          String *global_name,
                          Vec *generic_params,
                          Vec *fields,
                          LilyCheckedScope *scope,
                          enum LilyVisibility visibility);

extern inline int
add_signature__LilyCheckedDeclRecord(LilyCheckedDeclRecord *self,
                                     OrderedHashMap *generic_params);

// <core/lily/analysis/checked/decl/record_object.h>
extern inline CONSTRUCTOR(LilyCheckedDeclRecordObject,
                          LilyCheckedDeclRecordObject,
                          String *name,
                          String *global_name,
                          Vec *generic_params,
                          Vec *impl_params,
                          Vec *body,
                          LilyCheckedScope *scope,
                          enum LilyVisibility visibility);

// <core/lily/analysis/checked/decl/trait.h>
extern inline CONSTRUCTOR(LilyCheckedDeclTrait,
                          LilyCheckedDeclTrait,
                          String *name,
                          String *global_name,
                          Vec *generic_params,
                          Vec *inherit_params,
                          Vec *body,
                          LilyCheckedScope *scope,
                          enum LilyVisibility visibility,
                          bool is_close);

// <core/lily/analysis/checked/decl/type.h>
extern inline VARIANT_CONSTRUCTOR(LilyCheckedDeclType,
                                  LilyCheckedDeclType,
                                  alias,
                                  LilyCheckedDeclAlias alias);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedDeclType,
                                  LilyCheckedDeclType,
                                  enum,
                                  LilyCheckedDeclEnum enum_);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedDeclType,
                                  LilyCheckedDeclType,
                                  record,
                                  LilyCheckedDeclRecord record);

// <core/lily/analysis/checked/expr/access.h>
extern inline CONSTRUCTOR(LilyCheckedExprAccessHook,
                          LilyCheckedExprAccessHook,
                          LilyCheckedExpr *left,
                          LilyCheckedExpr *index);

extern inline CONSTRUCTOR(LilyCheckedExprAccessTuple,
                          LilyCheckedExprAccessTuple,
                          LilyCheckedExpr *left,
                          LilyCheckedExpr *n);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedExprAccess,
                                  LilyCheckedExprAccess,
                                  hook,
                                  LilyCheckedExprAccessHook hook);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedExprAccess,
                                  LilyCheckedExprAccess,
                                  tuple,
                                  LilyCheckedExprAccessTuple tuple);

// <core/lily/analysis/checked/expr/array.h>
extern inline CONSTRUCTOR(LilyCheckedExprArray,
                          LilyCheckedExprArray,
                          Vec *items);

// <core/lily/analysis/checked/expr/binary.h>
extern inline CONSTRUCTOR(LilyCheckedExprBinary,
                          LilyCheckedExprBinary,
                          enum LilyCheckedExprBinaryKind kind,
                          LilyCheckedExpr *left,
                          LilyCheckedExpr *right);

// <core/lily/analysis/checked/expr/call.h>
extern inline CONSTRUCTOR(LilyCheckedExprCallError,
                          LilyCheckedExprCallError,
                          Vec *params);

extern inline CONSTRUCTOR(LilyCheckedExprCallFun,
                          LilyCheckedExprCallFun,
                          LilyCheckedDecl *fun,
                          Vec *params,
                          HashMap *generic_params);

extern inline CONSTRUCTOR(LilyCheckedExprCallFunBuiltin,
                          LilyCheckedExprCallFunBuiltin,
                          Vec *params,
                          const LilyBuiltinFun *builtin_fun_signature);

extern inline CONSTRUCTOR(LilyCheckedExprCallFunSys,
                          LilyCheckedExprCallFunSys,
                          Vec *params,
                          const LilySysFun *sys_fun_signature);

extern inline CONSTRUCTOR(LilyCheckedExprCallMethod,
                          LilyCheckedExprCallMethod,
                          Vec *params);

extern inline CONSTRUCTOR(LilyCheckedExprCallRecord,
                          LilyCheckedExprCallRecord,
                          Vec *params);

extern inline CONSTRUCTOR(LilyCheckedExprCallRecordFieldSingle,
                          LilyCheckedExprCallRecordFieldSingle,
                          LilyCheckedAccessScope record_access,
                          String *record_global_name,
                          Usize index);

extern inline CONSTRUCTOR(LilyCheckedExprCallRecordFieldAccess,
                          LilyCheckedExprCallRecordFieldAccess,
                          Vec *accesses);

extern inline CONSTRUCTOR(LilyCheckedExprCallVariant,
                          LilyCheckedExprCallVariant,
                          LilyCheckedExpr *value);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedExprCall,
                                  LilyCheckedExprCall,
                                  cstr_len,
                                  LilyCheckedExpr *cstr_len);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedExprCall,
                                  LilyCheckedExprCall,
                                  catch_variable,
                                  String *global_name,
                                  HashMap *catch_variable);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedExprCall,
                                  LilyCheckedExprCall,
                                  enum,
                                  LilyCheckedAccessScope scope,
                                  String *global_name,
                                  LilyCheckedDecl *enum_);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedExprCall,
                                  LilyCheckedExprCall,
                                  error,
                                  LilyCheckedAccessScope scope,
                                  String *global_name,
                                  LilyCheckedExprCallError error);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedExprCall,
                                  LilyCheckedExprCall,
                                  fun,
                                  LilyCheckedAccessScope scope,
                                  String *global_name,
                                  LilyCheckedExprCallFun fun);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedExprCall,
                                  LilyCheckedExprCall,
                                  fun_builtin,
                                  LilyCheckedExprCallFunBuiltin fun_builtin);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedExprCall,
                                  LilyCheckedExprCall,
                                  fun_sys,
                                  LilyCheckedExprCallFunSys fun_sys);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedExprCall,
                                  LilyCheckedExprCall,
                                  fun_param,
                                  LilyCheckedAccessScope scope,
                                  String *global_name,
                                  Usize fun_param);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedExprCall,
                                  LilyCheckedExprCall,
                                  method,
                                  LilyCheckedAccessScope scope,
                                  String *global_name,
                                  LilyCheckedExprCallMethod method);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedExprCall,
                                  LilyCheckedExprCall,
                                  record,
                                  LilyCheckedAccessScope scope,
                                  String *global_name,
                                  LilyCheckedExprCallRecord record);

extern inline VARIANT_CONSTRUCTOR(
  LilyCheckedExprCall,
  LilyCheckedExprCall,
  record_field_single,
  LilyCheckedAccessScope scope,
  String *global_name,
  LilyCheckedExprCallRecordFieldSingle record_field_single);

extern inline VARIANT_CONSTRUCTOR(
  LilyCheckedExprCall,
  LilyCheckedExprCall,
  record_field_access,
  LilyCheckedAccessScope scope,
  String *global_name,
  LilyCheckedExprCallRecordFieldAccess record_field_access);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedExprCall,
                                  LilyCheckedExprCall,
                                  str_len,
                                  LilyCheckedExpr *str_len);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedExprCall,
                                  LilyCheckedExprCall,
                                  variant,
                                  LilyCheckedAccessScope scope,
                                  String *global_name,
                                  LilyCheckedExprCallVariant variant);

extern inline CONSTRUCTOR(LilyCheckedExprCall,
                          LilyCheckedExprCall,
                          enum LilyCheckedExprCallKind kind,
                          String *global_name,
                          LilyCheckedAccessScope scope);

// <core/lily/analysis/checked/expr/cast.h>
extern inline CONSTRUCTOR(LilyCheckedExprCast,
                          LilyCheckedExprCast,
                          enum LilyCheckedExprCastKind kind,
                          LilyCheckedExpr *expr,
                          LilyCheckedDataType *dest_data_type);

// <core/lily/analysis/checked/expr/compiler_fun.h>
extern inline CONSTRUCTOR(LilyCheckedExprCompilerFunGetField,
                          LilyCheckedExprCompilerFunGetField,
                          LilyCheckedExpr *record,
                          Usize id);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedExprCompilerFun,
                                  LilyCheckedExprCompilerFun,
                                  get_field,
                                  LilyCheckedExprCompilerFunGetField get_field);

// <core/lily/analysis/checked/expr/lambda.h>
extern inline CONSTRUCTOR(LilyCheckedExprLambda,
                          LilyCheckedExprLambda,
                          Vec *params,
                          LilyCheckedDataType *return_data_type,
                          Vec *body);

// <core/lily/analysis/checked/expr/list.h>
extern inline CONSTRUCTOR(LilyCheckedExprList, LilyCheckedExprList, Vec *items);

// <core/lily/analysis/checked/expr/literal.h>
extern inline VARIANT_CONSTRUCTOR(LilyCheckedExprLiteral,
                                  LilyCheckedExprLiteral,
                                  bool_,
                                  bool bool_);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedExprLiteral,
                                  LilyCheckedExprLiteral,
                                  byte,
                                  Uint8 byte);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedExprLiteral,
                                  LilyCheckedExprLiteral,
                                  bytes,
                                  Uint8 *bytes);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedExprLiteral,
                                  LilyCheckedExprLiteral,
                                  cstr,
                                  char *cstr);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedExprLiteral,
                                  LilyCheckedExprLiteral,
                                  char,
                                  char char_);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedExprLiteral,
                                  LilyCheckedExprLiteral,
                                  float32,
                                  Float32 float32);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedExprLiteral,
                                  LilyCheckedExprLiteral,
                                  float64,
                                  Float64 float64);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedExprLiteral,
                                  LilyCheckedExprLiteral,
                                  int32,
                                  Int32 int32);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedExprLiteral,
                                  LilyCheckedExprLiteral,
                                  int64,
                                  Int64 int64);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedExprLiteral,
                                  LilyCheckedExprLiteral,
                                  str,
                                  String *str);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedExprLiteral,
                                  LilyCheckedExprLiteral,
                                  suffix_float32,
                                  Float32 suffix_float32);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedExprLiteral,
                                  LilyCheckedExprLiteral,
                                  suffix_float64,
                                  Float64 suffix_float64);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedExprLiteral,
                                  LilyCheckedExprLiteral,
                                  suffix_int8,
                                  Int8 suffix_int8);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedExprLiteral,
                                  LilyCheckedExprLiteral,
                                  suffix_int16,
                                  Int16 suffix_int16);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedExprLiteral,
                                  LilyCheckedExprLiteral,
                                  suffix_int32,
                                  Int32 suffix_int32);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedExprLiteral,
                                  LilyCheckedExprLiteral,
                                  suffix_int64,
                                  Int64 suffix_int64);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedExprLiteral,
                                  LilyCheckedExprLiteral,
                                  suffix_isize,
                                  Isize suffix_isize);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedExprLiteral,
                                  LilyCheckedExprLiteral,
                                  suffix_uint8,
                                  Uint8 suffix_uint8);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedExprLiteral,
                                  LilyCheckedExprLiteral,
                                  suffix_uint16,
                                  Uint16 suffix_uint16);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedExprLiteral,
                                  LilyCheckedExprLiteral,
                                  suffix_uint32,
                                  Uint32 suffix_uint32);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedExprLiteral,
                                  LilyCheckedExprLiteral,
                                  suffix_uint64,
                                  Uint64 suffix_uint64);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedExprLiteral,
                                  LilyCheckedExprLiteral,
                                  suffix_usize,
                                  Usize suffix_usize);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedExprLiteral,
                                  LilyCheckedExprLiteral,
                                  uint32,
                                  Uint32 uint32);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedExprLiteral,
                                  LilyCheckedExprLiteral,
                                  uint64,
                                  Uint64 uint64);

extern inline CONSTRUCTOR(LilyCheckedExprLiteral,
                          LilyCheckedExprLiteral,
                          enum LilyCheckedExprLiteralKind kind);

// <core/lily/analysis/checked/expr/tuple.h>
extern inline CONSTRUCTOR(LilyCheckedExprTuple,
                          LilyCheckedExprTuple,
                          Vec *items);

// <core/lily/analysis/checked/expr/unary.h>
extern inline CONSTRUCTOR(LilyCheckedExprUnary,
                          LilyCheckedExprUnary,
                          enum LilyCheckedExprUnaryKind kind,
                          LilyCheckedExpr *right);

// <core/lily/analysis/checked/pattern/array.h>
extern inline CONSTRUCTOR(LilyCheckedPatternArray,
                          LilyCheckedPatternArray,
                          Usize len,
                          bool must_eq,
                          LilyCheckedPatternTable table);

// <core/lily/analysis/checked/pattern/as.h>
extern inline CONSTRUCTOR(LilyCheckedPatternAs,
                          LilyCheckedPatternAs,
                          LilyCheckedPattern *pattern,
                          String *name);

// <core/lily/analysis/checked/pattern/error.h>
extern inline CONSTRUCTOR(LilyCheckedPatternError,
                          LilyCheckedPatternError,
                          LilyCheckedExpr *id,
                          LilyCheckedPattern *pattern);

// <core/lily/analysis/chedcked/pattern/list_head.h>
extern inline CONSTRUCTOR(LilyCheckedPatternListHead,
                          LilyCheckedPatternListHead,
                          LilyCheckedPattern *left,
                          LilyCheckedPattern *right);

// <core/lily/analysis/checked/pattern/list_tail.h>
extern inline CONSTRUCTOR(LilyCheckedPatternListTail,
                          LilyCheckedPatternListTail,
                          LilyCheckedPattern *left,
                          LilyCheckedPattern *right);

// <core/lily/analysis/checked/pattern/list.h>
extern inline CONSTRUCTOR(LilyCheckedPatternList,
                          LilyCheckedPatternList,
                          Usize len,
                          bool must_eq,
                          LilyCheckedPatternTable table);

// <core/lily/analysis/checked/pattern/literal.h>
extern inline VARIANT_CONSTRUCTOR(LilyCheckedPatternLiteral,
                                  LilyCheckedPatternLiteral,
                                  bool_,
                                  bool bool_);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedPatternLiteral,
                                  LilyCheckedPatternLiteral,
                                  byte,
                                  Uint8 byte);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedPatternLiteral,
                                  LilyCheckedPatternLiteral,
                                  bytes,
                                  Uint8 *bytes);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedPatternLiteral,
                                  LilyCheckedPatternLiteral,
                                  char,
                                  char char_);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedPatternLiteral,
                                  LilyCheckedPatternLiteral,
                                  cstr,
                                  char *cstr);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedPatternLiteral,
                                  LilyCheckedPatternLiteral,
                                  float32,
                                  Float32 float32);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedPatternLiteral,
                                  LilyCheckedPatternLiteral,
                                  float64,
                                  Float64 float64);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedPatternLiteral,
                                  LilyCheckedPatternLiteral,
                                  int32,
                                  Int32 int32);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedPatternLiteral,
                                  LilyCheckedPatternLiteral,
                                  int64,
                                  Int64 int64);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedPatternLiteral,
                                  LilyCheckedPatternLiteral,
                                  str,
                                  String *str);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedPatternLiteral,
                                  LilyCheckedPatternLiteral,
                                  suffix_float32,
                                  Float32 suffix_float32);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedPatternLiteral,
                                  LilyCheckedPatternLiteral,
                                  suffix_float64,
                                  Float64 suffix_float64);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedPatternLiteral,
                                  LilyCheckedPatternLiteral,
                                  suffix_int8,
                                  Int8 suffix_int8);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedPatternLiteral,
                                  LilyCheckedPatternLiteral,
                                  suffix_int16,
                                  Int16 suffix_int16);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedPatternLiteral,
                                  LilyCheckedPatternLiteral,
                                  suffix_int32,
                                  Int32 suffix_int32);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedPatternLiteral,
                                  LilyCheckedPatternLiteral,
                                  suffix_int64,
                                  Int64 suffix_int64);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedPatternLiteral,
                                  LilyCheckedPatternLiteral,
                                  suffix_isize,
                                  Isize suffix_isize);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedPatternLiteral,
                                  LilyCheckedPatternLiteral,
                                  suffix_uint8,
                                  Uint8 suffix_uint8);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedPatternLiteral,
                                  LilyCheckedPatternLiteral,
                                  suffix_uint16,
                                  Uint16 suffix_uint16);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedPatternLiteral,
                                  LilyCheckedPatternLiteral,
                                  suffix_uint32,
                                  Uint32 suffix_uint32);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedPatternLiteral,
                                  LilyCheckedPatternLiteral,
                                  suffix_uint64,
                                  Uint64 suffix_uint64);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedPatternLiteral,
                                  LilyCheckedPatternLiteral,
                                  suffix_usize,
                                  Usize suffix_usize);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedPatternLiteral,
                                  LilyCheckedPatternLiteral,
                                  uint32,
                                  Uint32 uint32);

extern inline VARIANT_CONSTRUCTOR(LilyCheckedPatternLiteral,
                                  LilyCheckedPatternLiteral,
                                  uint64,
                                  Uint64 uint64);

extern inline CONSTRUCTOR(LilyCheckedPatternLiteral,
                          LilyCheckedPatternLiteral,
                          enum LilyCheckedPatternLiteralKind kind);

// <core/lily/analysis/checked/name.h>
extern inline CONSTRUCTOR(LilyCheckedPatternName,
                          LilyCheckedPatternName,
                          String *name);

// <core/lily/analysis/checked/pattern/range.h>
extern inline CONSTRUCTOR(LilyCheckedPatternRange,
                          LilyCheckedPatternRange,
                          LilyCheckedPattern *left,
                          LilyCheckedPattern *right);

// <core/lily/analysis/checked/pattern/record_call.h>
extern inline CONSTRUCTOR(LilyCheckedPatternRecordCall,
                          LilyCheckedPatternRecordCall,
                          LilyCheckedExpr *id,
                          Vec *fields);

// <core/lily/analysis/checked/pattern/table.h>
extern inline void
add__LilyCheckedPatternTable(LilyCheckedPatternTable self,
                             LilyCheckedPatternTableItem *item);

// <core/lily/analysis/checked/pattern/tuple.h>
extern inline CONSTRUCTOR(LilyCheckedPatternTuple,
                          LilyCheckedPatternTuple,
                          LilyCheckedPatternTable table);

// <core/lily/analysis/checked/pattern/variant_call.h>
extern inline CONSTRUCTOR(LilyCheckedPatternVariantCall,
                          LilyCheckedPatternVariantCall,
                          LilyCheckedExpr *id,
                          LilyCheckedPattern *pattern);

// <core/lily/analysis/checked/stmt/asm.h>
extern inline CONSTRUCTOR(LilyCheckedStmtAsm,
                          LilyCheckedStmtAsm,
                          LilyCheckedExpr *value,
                          Vec *params);

// <core/lily/analysis/checked/stmt/await.h>
extern inline CONSTRUCTOR(LilyCheckedStmtAwait,
                          LilyCheckedStmtAwait,
                          LilyCheckedExpr *expr);

extern inline DESTRUCTOR(LilyCheckedStmtAwait,
                         const LilyCheckedStmtAwait *self);

// <core/lily/analysis/checked/stmt/block.h>
extern inline CONSTRUCTOR(LilyCheckedStmtBlock,
                          LilyCheckedStmtBlock,
                          Vec *body,
                          LilyCheckedScope *scope);

// <core/lily/analysis/checked/stmt/break.h>
extern inline CONSTRUCTOR(LilyCheckedStmtBreak,
                          LilyCheckedStmtBreak,
                          String *name);

// <core/lily/analysis/checked/stmt/drop.h>
extern inline CONSTRUCTOR(LilyCheckedStmtDrop,
                          LilyCheckedStmtDrop,
                          LilyCheckedExpr *expr);

extern inline DESTRUCTOR(LilyCheckedStmtDrop, const LilyCheckedStmtDrop *self);

// <core/lily/analysis/checked/stmt/if.h>
extern inline CONSTRUCTOR(LilyCheckedStmtIf,
                          LilyCheckedStmtIf,
                          LilyCheckedStmtIfBranch *if_,
                          Vec *elifs,
                          LilyCheckedStmtElseBranch *else_);

// <core/lily/analysis/checked/stmt/match.h>
extern inline CONSTRUCTOR(LilyCheckedStmtMatch,
                          LilyCheckedStmtMatch,
                          LilyCheckedExpr *expr,
                          Vec *cases);

// <core/lily/analysis/checked/stmt/next.h>
extern inline CONSTRUCTOR(LilyCheckedStmtNext,
                          LilyCheckedStmtNext,
                          String *name);

// <core/lily/analysis/checked/stmt/raise.h>
extern inline CONSTRUCTOR(LilyCheckedStmtRaise,
                          LilyCheckedStmtRaise,
                          LilyCheckedExpr *expr);

extern inline DESTRUCTOR(LilyCheckedStmtRaise,
                         const LilyCheckedStmtRaise *self);

// <core/lily/analysis/checked/stmt/return.h>
extern inline CONSTRUCTOR(LilyCheckedStmtReturn,
                          LilyCheckedStmtReturn,
                          LilyCheckedExpr *expr);

extern inline DESTRUCTOR(LilyCheckedStmtReturn,
                         const LilyCheckedStmtReturn *self);

// <core/lily/analysis/checked/stmt/switch.h>
extern inline CONSTRUCTOR(LilyCheckedStmtSwitch,
                          LilyCheckedStmtSwitch,
                          LilyCheckedExpr *switched_expr,
                          Vec *cases);

// <core/lily/analysis/checked/stmt/try.h>
extern inline CONSTRUCTOR(LilyCheckedStmtTry,
                          LilyCheckedStmtTry,
                          Vec *try_body,
                          LilyCheckedScope *try_scope,
                          Vec *catch_body,
                          LilyCheckedScope *catch_scope);

// <core/lily/analysis/checked/stmt/unsafe.h>
extern inline CONSTRUCTOR(LilyCheckedStmtUnsafe,
                          LilyCheckedStmtUnsafe,
                          Vec *body,
                          LilyCheckedScope *scope);

// <core/lily/analysis/checked/stmt/variable.h>
extern inline CONSTRUCTOR(LilyCheckedStmtVariable,
                          LilyCheckedStmtVariable,
                          String *name,
                          LilyCheckedDataType *data_type,
                          LilyCheckedExpr *expr,
                          bool is_mut);

// <core/lily/analysis/checked/stmt/while.h>
extern inline CONSTRUCTOR(LilyCheckedStmtWhile,
                          LilyCheckedStmtWhile,
                          String *name,
                          LilyCheckedExpr *cond,
                          Vec *body,
                          LilyCheckedScope *scope);

#endif // LILY_EX_LIB_LILY_CORE_LILY_ANALYSIS_C
