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

#ifndef LILY_EX_LIB_LILY_CORE_LILY_PARSER_C
#define LILY_EX_LIB_LILY_CORE_LILY_PARSER_C

#include <core/lily/parser/ast/data_type.h>
#include <core/lily/parser/ast/decl/alias.h>
#include <core/lily/parser/ast/decl/attribute.h>
#include <core/lily/parser/ast/decl/class.h>
#include <core/lily/parser/ast/decl/constant.h>
#include <core/lily/parser/ast/decl/enum.h>
#include <core/lily/parser/ast/decl/enum_object.h>
#include <core/lily/parser/ast/decl/error.h>
#include <core/lily/parser/ast/decl/fun.h>
#include <core/lily/parser/ast/decl/include.h>
#include <core/lily/parser/ast/decl/method.h>
#include <core/lily/parser/ast/decl/module.h>
#include <core/lily/parser/ast/decl/object.h>
#include <core/lily/parser/ast/decl/prototype.h>
#include <core/lily/parser/ast/decl/record.h>
#include <core/lily/parser/ast/decl/record_object.h>
#include <core/lily/parser/ast/decl/trait.h>
#include <core/lily/parser/ast/decl/type.h>
#include <core/lily/parser/ast/decl/use.h>
#include <core/lily/parser/ast/expr/access.h>
#include <core/lily/parser/ast/expr/array.h>
#include <core/lily/parser/ast/expr/binary.h>
#include <core/lily/parser/ast/expr/call.h>
#include <core/lily/parser/ast/expr/cast.h>
#include <core/lily/parser/ast/expr/identifier.h>
#include <core/lily/parser/ast/expr/lambda.h>
#include <core/lily/parser/ast/expr/list.h>
#include <core/lily/parser/ast/expr/literal.h>
#include <core/lily/parser/ast/expr/try.h>
#include <core/lily/parser/ast/expr/tuple.h>
#include <core/lily/parser/ast/expr/unary.h>
#include <core/lily/parser/ast/field_object.h>
#include <core/lily/parser/ast/generic_param.h>
#include <core/lily/parser/ast/pattern/array.h>
#include <core/lily/parser/ast/pattern/as.h>
#include <core/lily/parser/ast/pattern/error.h>
#include <core/lily/parser/ast/pattern/list.h>
#include <core/lily/parser/ast/pattern/list_head.h>
#include <core/lily/parser/ast/pattern/list_tail.h>
#include <core/lily/parser/ast/pattern/literal.h>
#include <core/lily/parser/ast/pattern/name.h>
#include <core/lily/parser/ast/pattern/range.h>
#include <core/lily/parser/ast/pattern/record_call.h>
#include <core/lily/parser/ast/pattern/tuple.h>
#include <core/lily/parser/ast/pattern/variant_call.h>
#include <core/lily/parser/ast/stmt.h>
#include <core/lily/parser/ast/stmt/asm.h>
#include <core/lily/parser/ast/stmt/await.h>
#include <core/lily/parser/ast/stmt/block.h>
#include <core/lily/parser/ast/stmt/break.h>
#include <core/lily/parser/ast/stmt/defer.h>
#include <core/lily/parser/ast/stmt/drop.h>
#include <core/lily/parser/ast/stmt/if.h>
#include <core/lily/parser/ast/stmt/match.h>
#include <core/lily/parser/ast/stmt/next.h>
#include <core/lily/parser/ast/stmt/raise.h>
#include <core/lily/parser/ast/stmt/return.h>
#include <core/lily/parser/ast/stmt/try.h>
#include <core/lily/parser/ast/stmt/unsafe.h>
#include <core/lily/parser/ast/stmt/variable.h>
#include <core/lily/parser/ast/stmt/while.h>

#include "lily_core_lily_preparser.c"
#include "lily_core_lily_shared.c"

// <core/lily/parser/ast/data_type.h>
extern inline CONSTRUCTOR(LilyAstDataTypeLambda,
                          LilyAstDataTypeLambda,
                          Vec *params,
                          LilyAstDataType *return_type);

extern inline CONSTRUCTOR(LilyAstDataTypeArray,
                          LilyAstDataTypeArray,
                          enum LilyAstDataTypeArrayKind kind,
                          LilyAstDataType *data_type);

extern inline VARIANT_CONSTRUCTOR(LilyAstDataTypeArray,
                                  LilyAstDataTypeArray,
                                  sized,
                                  LilyAstDataType *data_type,
                                  Usize size);

extern inline CONSTRUCTOR(LilyAstDataTypeCustom,
                          LilyAstDataTypeCustom,
                          String *name,
                          Vec *generics);

extern inline CONSTRUCTOR(LilyAstDataTypeResult,
                          LilyAstDataTypeResult,
                          LilyAstDataType *ok,
                          Vec *errs);

// <core/lily/parser/ast/field_object.h>
extern inline CONSTRUCTOR(LilyAstFieldObject,
                          LilyAstFieldObject,
                          String *name,
                          LilyAstDataType *data_type,
                          LilyAstExpr *optional_expr,
                          enum LilyVisibility visibility);

// <core/lily/parser/ast/generic_param.h>
extern inline CONSTRUCTOR(LilyAstGenericParamConstraint,
                          LilyAstGenericParamConstraint,
                          String *name,
                          Vec *constraints);

// <core/lily/parser/ast/stmt.h>
extern inline VARIANT_CONSTRUCTOR(LilyAstStmt,
                                  LilyAstStmt,
                                  asm,
                                  Location location,
                                  LilyAstStmtAsm asm_);

extern inline VARIANT_CONSTRUCTOR(LilyAstStmt,
                                  LilyAstStmt,
                                  await,
                                  Location location,
                                  LilyAstStmtAwait await);

extern inline VARIANT_CONSTRUCTOR(LilyAstStmt,
                                  LilyAstStmt,
                                  block,
                                  Location location,
                                  LilyAstStmtBlock block);

extern inline VARIANT_CONSTRUCTOR(LilyAstStmt,
                                  LilyAstStmt,
                                  break,
                                  Location location,
                                  LilyAstStmtBreak break_);

extern inline VARIANT_CONSTRUCTOR(LilyAstStmt,
                                  LilyAstStmt,
                                  defer,
                                  Location location,
                                  LilyAstStmtDefer defer);

extern inline VARIANT_CONSTRUCTOR(LilyAstStmt,
                                  LilyAstStmt,
                                  drop,
                                  Location location,
                                  LilyAstStmtDrop drop);

extern inline VARIANT_CONSTRUCTOR(LilyAstStmt,
                           LilyAstStmt,
                           for,
						   Location location,
                           LilyAstStmtFor for_);

extern inline VARIANT_CONSTRUCTOR(LilyAstStmt,
                                  LilyAstStmt,
                                  if,
                                  Location location,
                                  LilyAstStmtIf if_);

extern inline VARIANT_CONSTRUCTOR(LilyAstStmt,
                                  LilyAstStmt,
                                  match,
                                  Location location,
                                  LilyAstStmtMatch match);

extern inline VARIANT_CONSTRUCTOR(LilyAstStmt,
                                  LilyAstStmt,
                                  next,
                                  Location location,
                                  LilyAstStmtNext next);

extern inline VARIANT_CONSTRUCTOR(LilyAstStmt,
                                  LilyAstStmt,
                                  raise,
                                  Location location,
                                  LilyAstStmtRaise raise);

extern inline VARIANT_CONSTRUCTOR(LilyAstStmt,
                                  LilyAstStmt,
                                  return,
                                  Location location,
                                  LilyAstStmtReturn return_);

extern inline VARIANT_CONSTRUCTOR(LilyAstStmt,
                                  LilyAstStmt,
                                  try,
                                  Location location,
                                  LilyAstStmtTry try);

extern inline VARIANT_CONSTRUCTOR(LilyAstStmt,
                                  LilyAstStmt,
                                  unsafe,
                                  Location location,
                                  LilyAstStmtUnsafe unsafe);

extern inline VARIANT_CONSTRUCTOR(LilyAstStmt,
                                  LilyAstStmt,
                                  variable,
                                  Location location,
                                  LilyAstStmtVariable variable);

extern inline VARIANT_CONSTRUCTOR(LilyAstStmt,
                                  LilyAstStmt,
                                  while,
                                  Location location,
                                  LilyAstStmtWhile while_);

// <core/lily/parser/ast/decl/alias.h>
extern inline CONSTRUCTOR(LilyAstDeclAlias,
                          LilyAstDeclAlias,
                          String *name,
                          Vec *generic_params,
                          LilyAstDataType *data_type,
                          enum LilyVisibility visibility);

// <core/lily/parser/ast/decl/attribute.h>
extern inline CONSTRUCTOR(LilyAstDeclAttribute,
                          LilyAstDeclAttribute,
                          String *name,
                          LilyAstDataType *data_type,
                          LilyAstExpr *optional_expr,
                          bool is_set,
                          bool is_get,
                          enum LilyVisibility visibility);

// <core/lily/parser/ast/decl/class.h>
extern inline CONSTRUCTOR(LilyAstDeclClass,
                          LilyAstDeclClass,
                          String *name,
                          Vec *generic_params,
                          Vec *inherit_params,
                          Vec *impl_params,
                          Vec *body,
                          enum LilyVisibility visibility,
                          bool is_close);

// <core/lily/parser/ast/decl/constant.h>
extern inline CONSTRUCTOR(LilyAstDeclConstant,
                          LilyAstDeclConstant,
                          String *name,
                          LilyAstDataType *data_type,
                          LilyAstExpr *expr,
                          enum LilyVisibility visibility);

// <core/lily/parser/ast/decl/enum_object.h>
extern inline CONSTRUCTOR(LilyAstDeclEnumObject,
                          LilyAstDeclEnumObject,
                          String *name,
                          Vec *generic_params,
                          Vec *impl_params,
                          Vec *body,
                          enum LilyVisibility visibility);

// <core/lily/parser/ast/decl/enum.h>
extern inline CONSTRUCTOR(LilyAstDeclEnum,
                          LilyAstDeclEnum,
                          String *name,
                          Vec *generic_params,
                          Vec *variants,
                          enum LilyVisibility visibility);

// <core/lily/parser/ast/decl/error.h>
extern inline CONSTRUCTOR(LilyAstDeclError,
                          LilyAstDeclError,
                          String *name,
                          Vec *generic_params,
                          LilyAstDataType *data_type,
                          enum LilyVisibility visibility);

// <core/lily/parser/ast/decl/fun.h>
extern inline CONSTRUCTOR(LilyAstDeclFun,
                          LilyAstDeclFun,
                          String *name,
                          String *object_impl,
                          Vec *generic_params,
                          Vec *params,
                          LilyAstDataType *return_data_type,
                          Vec *body,
                          Vec *req,
                          Vec *when,
                          enum LilyVisibility visibility,
                          bool is_async,
                          bool is_operator,
                          bool req_is_comptime,
                          bool when_is_comptime);

// <core/lily/parser/ast/decl/include.h>
extern inline CONSTRUCTOR(LilyAstDeclInclude,
                          LilyAstDeclInclude,
                          LilyAstExpr *access);

extern inline DESTRUCTOR(LilyAstDeclInclude, const LilyAstDeclInclude *self);

// <core/lily/parser/ast/decl/method.h>
extern inline CONSTRUCTOR(LilyAstDeclMethod,
                          LilyAstDeclMethod,
                          String *name,
                          String *object_impl,
                          Vec *generic_params,
                          Vec *params,
                          LilyAstDataType *return_data_type,
                          Vec *body,
                          Vec *req,
                          Vec *when,
                          enum LilyVisibility visibility,
                          bool is_async,
                          bool is_operator,
                          bool req_is_comptime,
                          bool when_is_comptime);

// <core/lily/parser/ast/decl/module.h>
extern inline CONSTRUCTOR(LilyAstDeclModule,
                          LilyAstDeclModule,
                          String *name,
                          Vec *decls,
                          enum LilyVisibility visibility);

// <core/lily/parser/ast/decl/object.h>
extern inline VARIANT_CONSTRUCTOR(LilyAstDeclObject,
                                  LilyAstDeclObject,
                                  class,
                                  LilyAstDeclClass class);

extern inline VARIANT_CONSTRUCTOR(LilyAstDeclObject,
                                  LilyAstDeclObject,
                                  enum,
                                  LilyAstDeclEnumObject enum_);

extern inline VARIANT_CONSTRUCTOR(LilyAstDeclObject,
                                  LilyAstDeclObject,
                                  record,
                                  LilyAstDeclRecordObject record);

extern inline VARIANT_CONSTRUCTOR(LilyAstDeclObject,
                                  LilyAstDeclObject,
                                  trait,
                                  LilyAstDeclTrait trait);

// <core/lily/parser/ast/decl/prototype.h>
extern inline CONSTRUCTOR(LilyAstDeclPrototype,
                          LilyAstDeclPrototype,
                          String *name,
                          Vec *generic_params,
                          Vec *params,
                          LilyAstDataType *return_data_type);

// <core/lily/parser/ast/decl/record_object.h>
extern inline CONSTRUCTOR(LilyAstDeclRecordObject,
                          LilyAstDeclRecordObject,
                          String *name,
                          Vec *generic_params,
                          Vec *impl_params,
                          Vec *body,
                          enum LilyVisibility visibility);

// <core/lily/parser/ast/decl/record.h>
extern inline CONSTRUCTOR(LilyAstDeclRecord,
                          LilyAstDeclRecord,
                          String *name,
                          Vec *generic_params,
                          Vec *fields,
                          enum LilyVisibility visibility);

// <core/lily/parser/ast/decl/trait.h>
extern inline CONSTRUCTOR(LilyAstDeclTrait,
                          LilyAstDeclTrait,
                          String *name,
                          Vec *generic_params,
                          Vec *inherit_params,
                          Vec *body,
                          enum LilyVisibility visibility,
                          bool is_close);

// <core/lily/parser/ast/decl/type.h>
extern inline VARIANT_CONSTRUCTOR(LilyAstDeclType,
                                  LilyAstDeclType,
                                  alias,
                                  LilyAstDeclAlias alias);

extern inline VARIANT_CONSTRUCTOR(LilyAstDeclType,
                                  LilyAstDeclType,
                                  enum,
                                  LilyAstDeclEnum enum_);

extern inline VARIANT_CONSTRUCTOR(LilyAstDeclType,
                                  LilyAstDeclType,
                                  record,
                                  LilyAstDeclRecord record);

// <core/lily/parser/ast/decl/use.h>
extern inline CONSTRUCTOR(LilyAstDeclUse, LilyAstDeclUse, LilyAstExpr *access);

extern inline DESTRUCTOR(LilyAstDeclUse, const LilyAstDeclUse *self);

// <core/lily/parser/ast/expr/access.h>
extern inline CONSTRUCTOR(LilyAstExprAccessHook,
                          LilyAstExprAccessHook,
                          LilyAstExpr *access,
                          LilyAstExpr *expr);

extern inline CONSTRUCTOR(LilyAstExprAccessAt,
                          LilyAstExprAccessAt,
                          LilyAstExpr *access,
                          Vec *objects);

extern inline VARIANT_CONSTRUCTOR(LilyAstExprAccess,
                                  LilyAstExprAccess,
                                  global_path,
                                  Vec *global_path);

extern inline VARIANT_CONSTRUCTOR(LilyAstExprAccess,
                                  LilyAstExprAccess,
                                  hook,
                                  LilyAstExprAccessHook hook);

extern inline VARIANT_CONSTRUCTOR(LilyAstExprAccess,
                                  LilyAstExprAccess,
                                  at,
                                  LilyAstExprAccessAt at);

extern inline VARIANT_CONSTRUCTOR(LilyAstExprAccess,
                                  LilyAstExprAccess,
                                  path,
                                  Vec *path);

extern inline VARIANT_CONSTRUCTOR(LilyAstExprAccess,
                                  LilyAstExprAccess,
                                  object_path,
                                  Vec *object_path);

extern inline VARIANT_CONSTRUCTOR(LilyAstExprAccess,
                                  LilyAstExprAccess,
                                  property_init,
                                  Vec *property_init);

extern inline VARIANT_CONSTRUCTOR(LilyAstExprAccess,
                                  LilyAstExprAccess,
                                  Self_path,
                                  Vec *Self_path);

extern inline VARIANT_CONSTRUCTOR(LilyAstExprAccess,
                                  LilyAstExprAccess,
                                  self_path,
                                  Vec *self_path);

// <core/lily/parser/ast/expr/array.h>
extern inline CONSTRUCTOR(LilyAstExprArray, LilyAstExprArray, Vec *items);

// <core/lily/parser/ast/expr/binary.h>
extern inline CONSTRUCTOR(LilyAstExprBinary,
                          LilyAstExprBinary,
                          enum LilyAstExprBinaryKind kind,
                          LilyAstExpr *left,
                          LilyAstExpr *right);

// <core/lily/parser/ast/expr/call.h>
extern inline CONSTRUCTOR(LilyAstExprCallFun,
                          LilyAstExprCallFun,
                          LilyAstExpr *id,
                          Vec *params);

extern inline CONSTRUCTOR(LilyAstExprCallFunSys,
                          LilyAstExprCallFunSys,
                          String *name,
                          Vec *params);

extern inline CONSTRUCTOR(LilyAstExprCallFunBuiltin,
                          LilyAstExprCallFunBuiltin,
                          String *name,
                          Vec *params);

extern inline CONSTRUCTOR(LilyAstExprCallRecord,
                          LilyAstExprCallRecord,
                          LilyAstExpr *id,
                          Vec *params);

extern inline CONSTRUCTOR(LilyAstExprCallVariant,
                          LilyAstExprCallVariant,
                          LilyAstExpr *id,
                          LilyAstExpr *value);

extern inline VARIANT_CONSTRUCTOR(LilyAstExprCall,
                                  LilyAstExprCall,
                                  fun,
                                  LilyAstExprCallFun fun);

extern inline VARIANT_CONSTRUCTOR(LilyAstExprCall,
                                  LilyAstExprCall,
                                  fun_builtin,
                                  LilyAstExprCallFunBuiltin fun_builtin);

extern inline VARIANT_CONSTRUCTOR(LilyAstExprCall,
                                  LilyAstExprCall,
                                  fun_sys,
                                  LilyAstExprCallFunSys fun_sys);

extern inline VARIANT_CONSTRUCTOR(LilyAstExprCall,
                                  LilyAstExprCall,
                                  len,
                                  LilyAstExpr *len);

extern inline VARIANT_CONSTRUCTOR(LilyAstExprCall,
                                  LilyAstExprCall,
                                  record,
                                  LilyAstExprCallRecord record);

extern inline VARIANT_CONSTRUCTOR(LilyAstExprCall,
                                  LilyAstExprCall,
                                  variant,
                                  LilyAstExprCallVariant variant);

// <core/lily/parser/ast/expr/cast.h>
extern inline CONSTRUCTOR(LilyAstExprCast,
                          LilyAstExprCast,
                          LilyAstExpr *expr,
                          LilyAstDataType *dest_data_type);

// <core/lily/parser/ast/expr/identifier.h>
extern inline CONSTRUCTOR(LilyAstExprIdentifier,
                          LilyAstExprIdentifier,
                          String *name,
                          Vec *generic_params);

extern inline CONSTRUCTOR(LilyAstExprIdentifierDollar,
                          LilyAstExprIdentifierDollar,
                          String *name);

extern inline DESTRUCTOR(LilyAstExprIdentifierDollar,
                         const LilyAstExprIdentifierDollar *self);

// <core/lily/parser/ast/expr/lambda.h>
extern inline CONSTRUCTOR(LilyAstExprLambda,
                          LilyAstExprLambda,
                          String *name,
                          Vec *params,
                          LilyAstDataType *return_data_type,
                          LilyAstBodyFunItem *item,
                          Vec *params_call);

// <core/lily/parser/ast/expr/list.h>
extern inline CONSTRUCTOR(LilyAstExprList, LilyAstExprList, Vec *items);

// <core/lily/parser/ast/expr/literal.h>
extern inline VARIANT_CONSTRUCTOR(LilyAstExprLiteral,
                                  LilyAstExprLiteral,
                                  bool_,
                                  bool bool_);

extern inline VARIANT_CONSTRUCTOR(LilyAstExprLiteral,
                                  LilyAstExprLiteral,
                                  byte,
                                  Uint8 byte);

extern inline VARIANT_CONSTRUCTOR(LilyAstExprLiteral,
                                  LilyAstExprLiteral,
                                  bytes,
                                  Uint8 *bytes);

extern inline VARIANT_CONSTRUCTOR(LilyAstExprLiteral,
                                  LilyAstExprLiteral,
                                  char,
                                  char char_);

extern inline VARIANT_CONSTRUCTOR(LilyAstExprLiteral,
                                  LilyAstExprLiteral,
                                  cstr,
                                  char *cstr);

extern inline VARIANT_CONSTRUCTOR(LilyAstExprLiteral,
                                  LilyAstExprLiteral,
                                  float32,
                                  Float32 float32);

extern inline VARIANT_CONSTRUCTOR(LilyAstExprLiteral,
                                  LilyAstExprLiteral,
                                  float64,
                                  Float64 float64);

extern inline VARIANT_CONSTRUCTOR(LilyAstExprLiteral,
                                  LilyAstExprLiteral,
                                  int32,
                                  Int32 int32);

extern inline VARIANT_CONSTRUCTOR(LilyAstExprLiteral,
                                  LilyAstExprLiteral,
                                  int64,
                                  Int64 int64);

extern inline VARIANT_CONSTRUCTOR(LilyAstExprLiteral,
                                  LilyAstExprLiteral,
                                  str,
                                  String *str);

extern inline VARIANT_CONSTRUCTOR(LilyAstExprLiteral,
                                  LilyAstExprLiteral,
                                  suffix_float32,
                                  Float32 suffix_float32);

extern inline VARIANT_CONSTRUCTOR(LilyAstExprLiteral,
                                  LilyAstExprLiteral,
                                  suffix_float64,
                                  Float64 suffix_float64);

extern inline VARIANT_CONSTRUCTOR(LilyAstExprLiteral,
                                  LilyAstExprLiteral,
                                  suffix_int8,
                                  Int8 suffix_int8);

extern inline VARIANT_CONSTRUCTOR(LilyAstExprLiteral,
                                  LilyAstExprLiteral,
                                  suffix_int16,
                                  Int16 suffix_int16);

extern inline VARIANT_CONSTRUCTOR(LilyAstExprLiteral,
                                  LilyAstExprLiteral,
                                  suffix_int32,
                                  Int32 suffix_int32);

extern inline VARIANT_CONSTRUCTOR(LilyAstExprLiteral,
                                  LilyAstExprLiteral,
                                  suffix_int64,
                                  Int64 suffix_int64);

extern inline VARIANT_CONSTRUCTOR(LilyAstExprLiteral,
                                  LilyAstExprLiteral,
                                  suffix_isize,
                                  Isize suffix_isize);

extern inline VARIANT_CONSTRUCTOR(LilyAstExprLiteral,
                                  LilyAstExprLiteral,
                                  suffix_uint8,
                                  Uint8 suffix_uint8);

extern inline VARIANT_CONSTRUCTOR(LilyAstExprLiteral,
                                  LilyAstExprLiteral,
                                  suffix_uint16,
                                  Uint16 suffix_uint16);

extern inline VARIANT_CONSTRUCTOR(LilyAstExprLiteral,
                                  LilyAstExprLiteral,
                                  suffix_uint32,
                                  Uint32 suffix_uint32);

extern inline VARIANT_CONSTRUCTOR(LilyAstExprLiteral,
                                  LilyAstExprLiteral,
                                  suffix_uint64,
                                  Uint64 suffix_uint64);

extern inline VARIANT_CONSTRUCTOR(LilyAstExprLiteral,
                                  LilyAstExprLiteral,
                                  suffix_usize,
                                  Usize suffix_usize);

extern inline VARIANT_CONSTRUCTOR(LilyAstExprLiteral,
                                  LilyAstExprLiteral,
                                  uint32,
                                  Uint32 uint32);

extern inline VARIANT_CONSTRUCTOR(LilyAstExprLiteral,
                                  LilyAstExprLiteral,
                                  uint64,
                                  Uint64 uint64);

extern inline CONSTRUCTOR(LilyAstExprLiteral,
                          LilyAstExprLiteral,
                          enum LilyAstExprLiteralKind kind);

extern inline VARIANT_DESTRUCTOR(LilyAstExprLiteral,
                                 str,
                                 const LilyAstExprLiteral *self);

extern inline DESTRUCTOR(LilyAstExprLiteral, const LilyAstExprLiteral *self);

// <core/lily/parser/ast/expr/try.h>
extern inline CONSTRUCTOR(LilyAstExprTry, LilyAstExprTry, LilyAstExpr *expr);

// <core/lily/parser/ast/expr/tuple.h>
extern inline CONSTRUCTOR(LilyAstExprTuple, LilyAstExprTuple, Vec *items);

// <core/lily/parser/ast/expr/unary.h>
extern inline CONSTRUCTOR(LilyAstExprUnary,
                          LilyAstExprUnary,
                          enum LilyAstExprUnaryKind kind,
                          LilyAstExpr *right);

// <core/lily/parser/ast/pattern/array.h>
extern inline CONSTRUCTOR(LilyAstPatternArray,
                          LilyAstPatternArray,
                          Vec *patterns);

// <core/lily/parser/ast/pattern/as.h>
extern inline CONSTRUCTOR(LilyAstPatternAs,
                          LilyAstPatternAs,
                          LilyAstPattern *pattern,
                          String *name);

// <core/lily/parser/ast/pattern/error.h>
extern inline CONSTRUCTOR(LilyAstPatternError,
                          LilyAstPatternError,
                          LilyAstExpr *id,
                          LilyAstPattern *pattern);

// <core/lily/parser/ast/pattern/list_head.h>
extern inline CONSTRUCTOR(LilyAstPatternListHead,
                          LilyAstPatternListHead,
                          LilyAstPattern *left,
                          LilyAstPattern *right);

// <core/lily/parser/ast/pattern/list_tail.h>
extern inline CONSTRUCTOR(LilyAstPatternListTail,
                          LilyAstPatternListTail,
                          LilyAstPattern *left,
                          LilyAstPattern *right);

// <core/lily/parser/ast/pattern/list.h>
extern inline CONSTRUCTOR(LilyAstPatternList,
                          LilyAstPatternList,
                          Vec *patterns);

// <core/lily/parser/ast/pattern/literal.h>
extern inline VARIANT_CONSTRUCTOR(LilyAstPatternLiteral,
                                  LilyAstPatternLiteral,
                                  bool_,
                                  bool bool_);

extern inline VARIANT_CONSTRUCTOR(LilyAstPatternLiteral,
                                  LilyAstPatternLiteral,
                                  byte,
                                  Uint8 byte);

extern inline VARIANT_CONSTRUCTOR(LilyAstPatternLiteral,
                                  LilyAstPatternLiteral,
                                  bytes,
                                  Uint8 *bytes);

extern inline VARIANT_CONSTRUCTOR(LilyAstPatternLiteral,
                                  LilyAstPatternLiteral,
                                  char,
                                  char char_);

extern inline VARIANT_CONSTRUCTOR(LilyAstPatternLiteral,
                                  LilyAstPatternLiteral,
                                  cstr,
                                  char *cstr);

extern inline VARIANT_CONSTRUCTOR(LilyAstPatternLiteral,
                                  LilyAstPatternLiteral,
                                  float32,
                                  Float32 float32);

extern inline VARIANT_CONSTRUCTOR(LilyAstPatternLiteral,
                                  LilyAstPatternLiteral,
                                  float64,
                                  Float64 float64);

extern inline VARIANT_CONSTRUCTOR(LilyAstPatternLiteral,
                                  LilyAstPatternLiteral,
                                  int32,
                                  Int32 int32);

extern inline VARIANT_CONSTRUCTOR(LilyAstPatternLiteral,
                                  LilyAstPatternLiteral,
                                  int64,
                                  Int64 int64);

extern inline VARIANT_CONSTRUCTOR(LilyAstPatternLiteral,
                                  LilyAstPatternLiteral,
                                  str,
                                  String *str);

extern inline VARIANT_CONSTRUCTOR(LilyAstPatternLiteral,
                                  LilyAstPatternLiteral,
                                  suffix_float32,
                                  Float32 suffix_float32);

extern inline VARIANT_CONSTRUCTOR(LilyAstPatternLiteral,
                                  LilyAstPatternLiteral,
                                  suffix_float64,
                                  Float64 suffix_float64);

extern inline VARIANT_CONSTRUCTOR(LilyAstPatternLiteral,
                                  LilyAstPatternLiteral,
                                  suffix_int8,
                                  Int8 suffix_int8);

extern inline VARIANT_CONSTRUCTOR(LilyAstPatternLiteral,
                                  LilyAstPatternLiteral,
                                  suffix_int16,
                                  Int16 suffix_int16);

extern inline VARIANT_CONSTRUCTOR(LilyAstPatternLiteral,
                                  LilyAstPatternLiteral,
                                  suffix_int32,
                                  Int32 suffix_int32);

extern inline VARIANT_CONSTRUCTOR(LilyAstPatternLiteral,
                                  LilyAstPatternLiteral,
                                  suffix_int64,
                                  Int64 suffix_int64);

extern inline VARIANT_CONSTRUCTOR(LilyAstPatternLiteral,
                                  LilyAstPatternLiteral,
                                  suffix_isize,
                                  Isize suffix_isize);

extern inline VARIANT_CONSTRUCTOR(LilyAstPatternLiteral,
                                  LilyAstPatternLiteral,
                                  suffix_uint8,
                                  Uint8 suffix_uint8);

extern inline VARIANT_CONSTRUCTOR(LilyAstPatternLiteral,
                                  LilyAstPatternLiteral,
                                  suffix_uint16,
                                  Uint16 suffix_uint16);

extern inline VARIANT_CONSTRUCTOR(LilyAstPatternLiteral,
                                  LilyAstPatternLiteral,
                                  suffix_uint32,
                                  Uint32 suffix_uint32);

extern inline VARIANT_CONSTRUCTOR(LilyAstPatternLiteral,
                                  LilyAstPatternLiteral,
                                  suffix_uint64,
                                  Uint64 suffix_uint64);

extern inline VARIANT_CONSTRUCTOR(LilyAstPatternLiteral,
                                  LilyAstPatternLiteral,
                                  suffix_usize,
                                  Usize suffix_usize);

extern inline VARIANT_CONSTRUCTOR(LilyAstPatternLiteral,
                                  LilyAstPatternLiteral,
                                  uint32,
                                  Uint32 uint32);

extern inline VARIANT_CONSTRUCTOR(LilyAstPatternLiteral,
                                  LilyAstPatternLiteral,
                                  uint64,
                                  Uint64 uint64);

extern inline CONSTRUCTOR(LilyAstPatternLiteral,
                          LilyAstPatternLiteral,
                          enum LilyAstPatternLiteralKind kind);

extern inline VARIANT_DESTRUCTOR(LilyAstPatternLiteral,
                                 str,
                                 const LilyAstPatternLiteral *self);

extern inline DESTRUCTOR(LilyAstPatternLiteral,
                         const LilyAstPatternLiteral *self);

// <core/lily/parser/ast/pattern/name.h>
extern inline CONSTRUCTOR(LilyAstPatternName, LilyAstPatternName, String *name);

extern inline DESTRUCTOR(LilyAstPatternName, const LilyAstPatternName *self);

// <core/lily/parser/ast/pattern/range.h>
extern inline CONSTRUCTOR(LilyAstPatternRange,
                          LilyAstPatternRange,
                          LilyAstPattern *left,
                          LilyAstPattern *right);

// <core/lily/parser/ast/pattern/record_call.h>
extern inline CONSTRUCTOR(LilyAstPatternRecordCall,
                          LilyAstPatternRecordCall,
                          LilyAstExpr *id,
                          Vec *fields);

// <core/lily/parser/ast/pattern/tuple.h>
extern inline CONSTRUCTOR(LilyAstPatternTuple,
                          LilyAstPatternTuple,
                          Vec *patterns);

// <core/lily/parser/ast/pattern/variant_call.h>
extern inline CONSTRUCTOR(LilyAstPatternVariantCall,
                          LilyAstPatternVariantCall,
                          LilyAstExpr *id,
                          LilyAstPattern *pattern);

// <core/lily/parser/ast/stmt/asm.h>
extern inline CONSTRUCTOR(LilyAstStmtAsm,
                          LilyAstStmtAsm,
                          LilyAstExpr *value,
                          Vec *params);

// <core/lily/parser/ast/stmt/await.h>
extern inline CONSTRUCTOR(LilyAstStmtAwait,
                          LilyAstStmtAwait,
                          LilyAstExpr *expr);

extern inline DESTRUCTOR(LilyAstStmtAwait, const LilyAstStmtAwait *self);

// <core/lily/parser/ast/stmt/block.h>
extern inline CONSTRUCTOR(LilyAstStmtBlock, LilyAstStmtBlock, Vec *body);

// <core/lily/parser/ast/stmt/break.h>
extern inline CONSTRUCTOR(LilyAstStmtBreak, LilyAstStmtBreak, String *name);

extern inline DESTRUCTOR(LilyAstStmtBreak, const LilyAstStmtBreak *self);

// <core/lily/parser/ast/stmt/defer.h>
extern inline CONSTRUCTOR(LilyAstStmtDefer,
                          LilyAstStmtDefer,
                          LilyAstBodyFunItem *item);

// <core/lily/parser/ast/stmt/drop.h>
extern inline CONSTRUCTOR(LilyAstStmtDrop, LilyAstStmtDrop, LilyAstExpr *expr);

extern inline DESTRUCTOR(LilyAstStmtDrop, const LilyAstStmtDrop *self);

// <core/lily/parser/ast/stmt/if.h>
extern inline CONSTRUCTOR(LilyAstStmtIf,
                          LilyAstStmtIf,
                          LilyAstExpr *if_expr,
                          LilyAstCapture *if_capture,
                          Vec *if_body,
                          Vec *elif_exprs,
                          Vec *elif_captures,
                          Vec *elif_bodies,
                          Vec *else_body);

// <core/lily/parser/ast/stmt/match.h>
extern inline CONSTRUCTOR(LilyAstStmtMatch,
                          LilyAstStmtMatch,
                          LilyAstExpr *expr,
                          Vec *cases);

// <core/lily/parser/ast/stmt/next.h>
extern inline CONSTRUCTOR(LilyAstStmtNext, LilyAstStmtNext, String *name);

extern inline DESTRUCTOR(LilyAstStmtNext, const LilyAstStmtNext *self);

// <core/lily/parser/ast/stmt/raise.h>
extern inline CONSTRUCTOR(LilyAstStmtRaise,
                          LilyAstStmtRaise,
                          LilyAstExpr *expr);

extern inline DESTRUCTOR(LilyAstStmtRaise, const LilyAstStmtRaise *self);

// <core/lily/parser/ast/stmt/return.h>
extern inline CONSTRUCTOR(LilyAstStmtReturn,
                          LilyAstStmtReturn,
                          LilyAstExpr *expr);

extern inline DESTRUCTOR(LilyAstStmtReturn, const LilyAstStmtReturn *self);

// <core/lily/parser/ast/stmt/try.h>
extern inline CONSTRUCTOR(LilyAstStmtTry,
                          LilyAstStmtTry,
                          Vec *try_body,
                          LilyAstExpr *catch_expr,
                          Vec *catch_body);

// <core/lily/parser/ast/stmt/unsafe.h>
extern inline CONSTRUCTOR(LilyAstStmtUnsafe, LilyAstStmtUnsafe, Vec *body);

// <core/lily/parser/ast/stmt/variable.h>
extern inline CONSTRUCTOR(LilyAstStmtVariable,
                          LilyAstStmtVariable,
                          String *name,
                          LilyAstDataType *data_type,
                          LilyAstExpr *expr,
                          bool is_mut,
                          bool is_trace,
                          bool is_ref,
                          bool is_drop);

// <core/lily/parser/ast/stmt/while.h>
extern inline CONSTRUCTOR(LilyAstStmtWhile,
                          LilyAstStmtWhile,
                          String *name,
                          LilyAstExpr *expr,
                          Vec *body);

#endif // LILY_EX_LIB_LILY_CORE_LILY_PARSER_C
