/*
 * MIT License
 *
 * Copyright (c) 2022-2024 ArthurPV
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

#ifndef LILY_EX_LIB_LILY_CORE_CC_CI_C
#define LILY_EX_LIB_LILY_CORE_CC_CI_C

#include <core/cc/ci/ast.h>
#include <core/cc/ci/parser.h>
#include <core/cc/ci/project_config.h>
#include <core/cc/ci/result.h>
#include <core/cc/ci/scanner.h>
#include <core/cc/ci/token.h>

#include "lily_base.c"
#include "lily_core_cc_ci_diagnostic.c"
#include "lily_core_shared.c"

// <core/cc/ci/ast.h>
extern inline DESTRUCTOR(CIScopeID, CIScopeID *self);

extern inline CONSTRUCTOR(CIFileID, CIFileID, Usize id, Uint8 kind);

extern inline bool
eq__CIFileID(const CIFileID *self, const CIFileID *other);

extern inline DESTRUCTOR(CIEnumID, CIEnumID *self);

extern inline DESTRUCTOR(CIFunctionID, CIFunctionID *self);

extern inline DESTRUCTOR(CILabelID, CILabelID *self);

extern inline DESTRUCTOR(CIStructID, CIStructID *self);

extern inline DESTRUCTOR(CITypedefID, CITypedefID *self);

extern inline DESTRUCTOR(CIUnionID, CIUnionID *self);

extern inline DESTRUCTOR(CIVariableID, CIVariableID *self);

extern inline const CIEnumID *
add_enum__CIScope(const CIScope *self,
                  const String *name,
                  CIFileID file_id,
                  Usize id);

extern inline const CIFunctionID *
add_function__CIScope(const CIScope *self,
                      const String *name,
                      CIFileID file_id,
                      Usize id);

extern inline const CILabelID *
add_label__CIScope(const CIScope *self,
                   const String *name,
                   CIScopeID scope_id,
                   CIFileID file_id,
                   Usize id);

extern inline const CIStructID *
add_struct__CIScope(const CIScope *self,
                    const String *name,
                    CIFileID file_id,
                    Usize id);

extern inline const CITypedefID *
add_typedef__CIScope(const CIScope *self,
                     const String *name,
                     CIFileID file_id,
                     Usize id);

extern inline const CIUnionID *
add_union__CIScope(const CIScope *self,
                   const String *name,
                   CIFileID file_id,
                   Usize id);

extern inline const CIVariableID *
add_variable__CIScope(const CIScope *self,
                      const String *name,
                      CIScopeID scope_id,
                      CIFileID file_id,
                      Usize id);

extern inline const CIEnumID *
search_enum__CIScope(const CIScope *self, const String *name);

extern inline const CIFunctionID *
search_function__CIScope(const CIScope *self, const String *name);

extern inline const CILabelID *
search_label__CIScope(const CIScope *self, const String *name);

extern inline const CIStructID *
search_struct__CIScope(const CIScope *self, const String *name);

extern inline const CITypedefID *
search_typedef__CIScope(const CIScope *self, const String *name);

extern inline const CIUnionID *
search_union__CIScope(const CIScope *self, const String *name);

extern inline const CIVariableID *
search_variable__CIScope(const CIScope *self, const String *name);

extern inline CIGenericParams *
ref__CIGenericParams(CIGenericParams *self);

extern bool
eq_op__CIGenericParams(const CIGenericParams *self,
                       const CIGenericParams *other);

extern inline CONSTRUCTOR(CISizeInfo, CISizeInfo, Usize size, Usize alignment);

extern inline VARIANT_CONSTRUCTOR(CIDataTypeArray,
                                  CIDataTypeArray,
                                  sized,
                                  struct CIDataType *data_type,
                                  String *name,
                                  Usize size);

extern inline VARIANT_CONSTRUCTOR(CIDataTypeArray,
                                  CIDataTypeArray,
                                  none,
                                  struct CIDataType *data_type,
                                  String *name);

extern inline CONSTRUCTOR(CIDataTypeFunction,
                          CIDataTypeFunction,
                          String *name,
                          Vec *params,
                          struct CIDataType *return_data_type,
                          struct CIDataType *function_data_type);

extern inline CONSTRUCTOR(CIDataTypeStruct,
                          CIDataTypeStruct,
                          String *name,
                          CIGenericParams *generic_params,
                          Vec *fields);

extern inline CONSTRUCTOR(CIDataTypeTypedef,
                          CIDataTypeTypedef,
                          String *name,
                          CIGenericParams *generic_params);

extern inline CONSTRUCTOR(CIDataTypeUnion,
                          CIDataTypeUnion,
                          String *name,
                          CIGenericParams *generic_params,
                          Vec *fields);

extern inline CIDataType *
ref__CIDataType(CIDataType *self);

extern inline void
set_context__CIDataType(CIDataType *self, int ctx);

extern inline CONSTRUCTOR(CIAttributeStandard,
                          CIAttributeStandard,
                          enum CIAttributeStandardKind kind);

extern inline VARIANT_CONSTRUCTOR(CIAttributeStandard,
                                  CIAttributeStandard,
                                  deprecated,
                                  String *deprecated);

extern inline VARIANT_CONSTRUCTOR(CIAttributeStandard,
                                  CIAttributeStandard,
                                  nodiscard,
                                  String *nodiscard);

extern inline DESTRUCTOR(CIAttribute, CIAttribute *self);

extern inline DESTRUCTOR(CIDeclEnumVariant, CIDeclEnumVariant *self);

extern inline CONSTRUCTOR(CIDeclEnum,
                          CIDeclEnum,
                          String *name,
                          Vec *variants,
                          CIDataType *data_type,
                          CISizeInfo size_info);

extern inline void
free_as_prototype__CIDeclEnum(const CIDeclEnum *self);

extern inline void
add__CIDeclFunctionBody(CIDeclFunctionBody *self, CIDeclFunctionItem *item);

extern inline CONSTRUCTOR(CIDeclFunction,
                          CIDeclFunction,
                          String *name,
                          CIDataType *return_data_type,
                          CIGenericParams *generic_params,
                          Vec *params,
                          CIDeclFunctionBody *body,
                          Vec *attributes);

extern inline bool
eq_generic_params__CIDeclFunction(const CIDeclFunction *self,
                                  const CIDeclFunction *other);

extern inline CONSTRUCTOR(CIDeclFunctionGen,
                          CIDeclFunctionGen,
                          const CIDeclFunction *function,
                          String *name,
                          CIGenericParams *called_generic_params,
                          CIDataType *return_data_type);

extern inline CONSTRUCTOR(CIDeclLabel, CIDeclLabel, String *name);

extern inline CONSTRUCTOR(CIDeclStruct,
                          CIDeclStruct,
                          String *name,
                          CIGenericParams *generic_params,
                          Vec *fields,
                          CISizeInfo size_info);

extern inline bool
eq_generic_params__CIDeclStruct(const CIDeclStruct *self,
                                const CIDeclStruct *other);

extern inline CONSTRUCTOR(CIDeclStructGen,
                          CIDeclStructGen,
                          const CIDeclStruct *struct_,
                          String *name,
                          CIGenericParams *called_generic_params,
                          Vec *fields,
                          CISizeInfo size_info);

extern inline CONSTRUCTOR(CIDeclTypedef,
                          CIDeclTypedef,
                          String *name,
                          CIGenericParams *generic_params,
                          CIDataType *data_type,
                          CISizeInfo size_info);

extern inline DESTRUCTOR(CIDeclTypedef, const CIDeclTypedef *self);

extern inline bool
eq_generic_params__CIDeclTypedef(const CIDeclTypedef *self,
                                 const CIDeclTypedef *other);

extern inline CONSTRUCTOR(CIDeclTypedefGen,
                          CIDeclTypedefGen,
                          const CIDeclTypedef *typedef_,
                          String *name,
                          CIGenericParams *called_generic_params,
                          CIDataType *data_type,
                          CISizeInfo size_info);

extern inline CONSTRUCTOR(CIDeclUnion,
                          CIDeclUnion,
                          String *name,
                          CIGenericParams *generic_params,
                          Vec *fields,
                          CISizeInfo size_info);

extern inline bool
eq_generic_params__CIDeclUnion(const CIDeclUnion *self,
                               const CIDeclUnion *other);

extern inline CONSTRUCTOR(CIDeclUnionGen,
                          CIDeclUnionGen,
                          const CIDeclUnion *union_,
                          String *name,
                          CIGenericParams *called_generic_params,
                          Vec *fields,
                          CISizeInfo size_info);

extern inline CONSTRUCTOR(CIDeclVariable,
                          CIDeclVariable,
                          CIDataType *data_type,
                          String *name,
                          CIExpr *expr,
                          bool is_local);

extern inline CIDecl *
ref__CIDecl(CIDecl *self);

extern inline bool
is_typedef__CIDecl(CIDecl *self);

extern inline CONSTRUCTOR(CIExprArray, CIExprArray, Vec *array);

extern inline CONSTRUCTOR(CIExprArrayAccess,
                          CIExprArrayAccess,
                          CIExpr *array,
                          CIExpr *access);

extern inline CONSTRUCTOR(CIExprBinary,
                          CIExprBinary,
                          enum CIExprBinaryKind kind,
                          CIExpr *left,
                          CIExpr *right);

extern inline VARIANT_CONSTRUCTOR(CIExprLiteral,
                                  CIExprLiteral,
                                  bool,
                                  bool bool_);

extern inline VARIANT_CONSTRUCTOR(CIExprLiteral,
                                  CIExprLiteral,
                                  char,
                                  char char_);

extern inline VARIANT_CONSTRUCTOR(CIExprLiteral,
                                  CIExprLiteral,
                                  float,
                                  double float_);

extern inline VARIANT_CONSTRUCTOR(CIExprLiteral,
                                  CIExprLiteral,
                                  signed_int,
                                  Isize signed_int);

extern inline VARIANT_CONSTRUCTOR(CIExprLiteral,
                                  CIExprLiteral,
                                  string,
                                  String *value,
                                  bool must_free);

extern inline VARIANT_CONSTRUCTOR(CIExprLiteral,
                                  CIExprLiteral,
                                  unsigned_int,
                                  Usize unsigned_int);

extern inline CONSTRUCTOR(CIExprUnary,
                          CIExprUnary,
                          enum CIExprUnaryKind kind,
                          CIExpr *expr);

extern inline CONSTRUCTOR(CIExprTernary,
                          CIExprTernary,
                          CIExpr *cond,
                          CIExpr *if_,
                          CIExpr *else_);

extern inline CONSTRUCTOR(CIExprCast,
                          CIExprCast,
                          CIDataType *data_type,
                          CIExpr *expr);

extern inline CONSTRUCTOR(CIExprFunctionCall,
                          CIExprFunctionCall,
                          String *identifier,
                          Vec *params,
                          CIGenericParams *generic_params);

extern inline CONSTRUCTOR(CIExprFunctionCallBuiltin,
                          CIExprFunctionCallBuiltin,
                          Usize id,
                          Vec *params);

extern inline CIExpr *
ref__CIExpr(CIExpr *self);

extern inline CONSTRUCTOR(CIExprStructCall, CIExprStructCall, Vec *fields);

extern inline CONSTRUCTOR(CIStmtBlock, CIStmtBlock, CIDeclFunctionBody *body);

extern inline DESTRUCTOR(CIStmtBlock, const CIStmtBlock *self);

extern inline CONSTRUCTOR(CIStmtDoWhile,
                          CIStmtDoWhile,
                          CIDeclFunctionBody *body,
                          CIExpr *cond);

extern inline CONSTRUCTOR(CIStmtFor,
                          CIStmtFor,
                          CIDeclFunctionBody *body,
                          CIDeclFunctionItem *init_clause,
                          CIExpr *expr1,
                          Vec *exprs2);

extern inline CONSTRUCTOR(CIStmtIf,
                          CIStmtIf,
                          CIStmtIfBranch *if_,
                          Vec *else_ifs,
                          CIDeclFunctionBody *else_);

extern inline CONSTRUCTOR(CIStmtSwitchCase, CIStmtSwitchCase, CIExpr *value);

extern inline CONSTRUCTOR(CIStmtSwitch,
                          CIStmtSwitch,
                          CIExpr *expr,
                          CIDeclFunctionBody *body);

extern inline CONSTRUCTOR(CIStmtWhile,
                          CIStmtWhile,
                          CIExpr *cond,
                          CIDeclFunctionBody *body);

extern inline VARIANT_CONSTRUCTOR(CIStmt, CIStmt, block, CIStmtBlock block);

extern inline VARIANT_CONSTRUCTOR(CIStmt, CIStmt, break);

extern inline VARIANT_CONSTRUCTOR(CIStmt, CIStmt, case, CIStmtSwitchCase case_);

extern inline VARIANT_CONSTRUCTOR(CIStmt, CIStmt, default);

extern inline VARIANT_CONSTRUCTOR(CIStmt, CIStmt, continue);

extern inline VARIANT_CONSTRUCTOR(CIStmt,
                                  CIStmt,
                                  do_while,
                                  CIStmtDoWhile do_while);

extern 
inline VARIANT_CONSTRUCTOR(CIStmt, CIStmt, for, CIStmtFor for_);

extern inline VARIANT_CONSTRUCTOR(CIStmt, CIStmt, goto, String *goto_);

extern inline VARIANT_CONSTRUCTOR(CIStmt, CIStmt, if, CIStmtIf if_);

extern inline VARIANT_CONSTRUCTOR(CIStmt, CIStmt, label, String *label);

extern inline VARIANT_CONSTRUCTOR(CIStmt, CIStmt, return, CIExpr *return_);

extern inline VARIANT_CONSTRUCTOR(CIStmt, CIStmt, switch, CIStmtSwitch switch_);

extern inline VARIANT_CONSTRUCTOR(CIStmt, CIStmt, while, CIStmtWhile while_);

// <core/cc/ci/config.h>
extern inline CONSTRUCTOR(CIProjectConfigCompiler,
                          CIProjectConfigCompiler,
                          enum CIProjectConfigCompilerKind kind,
                          String *path);

extern inline DESTRUCTOR(CIProjectConfigCompiler,
                         const CIProjectConfigCompiler *self);

extern inline DESTRUCTOR(CIProjectConfigLibrary, CIProjectConfigLibrary *self);

extern inline CONSTRUCTOR(CIProjectConfig,
                          CIProjectConfig,
                          YAMLLoadRes yaml_load_res,
                          enum CIStandard standard,
                          CIProjectConfigCompiler compiler,
                          const Vec *include_dirs,
                          Vec *libraries,
                          Vec *bins);

// <core/cc/ci/parser.h>
extern inline CONSTRUCTOR(CIParserVisitWaitingList, CIParserVisitWaitingList);

extern inline CONSTRUCTOR(CIParserMacroCallParams, CIParserMacroCallParams);

extern inline CIParserMacroCallParam *
get_macro_param_variadic__CIParserMacroCallParams(
  const CIParserMacroCallParams *self);

// <core/cc/ci/result.h>
extern inline CIResultDefine *
ref__CIResultDefine(CIResultDefine *self);

extern inline void
increment_repeat_count__CIResultInclude(CIResultInclude *self);

extern inline DESTRUCTOR(CIResultInclude, CIResultInclude *self);

extern inline Usize
get_next_scope_id__CIResultFile(const CIResultFile *self);

extern inline CONSTRUCTOR(CIResultEntity,
                          CIResultEntity,
                          Usize id,
                          const CIResult *result,
                          enum CIResultEntityKind kind,
                          String *filename_result);

extern inline CONSTRUCTOR(CIResult,
                          CIResult,
                          const CIProjectConfig *config,
                          const CIBuiltin *builtin);

extern inline bool
has_header__CIResult(const CIResult *self, const String *filename_result);

extern inline bool
has_source__CIResult(const CIResult *self, const String *filename_result);

// <core/cc/ci/scanner.h>
extern inline CONSTRUCTOR(CIScanner,
                          CIScanner,
                          Source source,
                          Usize *count_error,
                          const CIProjectConfig *config);

extern inline void
set_predefined__CIScanner(CIScanner *self);

extern inline DESTRUCTOR(CIScanner, const CIScanner *self);

extern inline VARIANT_CONSTRUCTOR(CIScannerContext,
                                  CIScannerContext,
                                  macro,
                                  CITokens *tokens,
                                  Vec *macro);

extern inline VARIANT_CONSTRUCTOR(CIScannerContext,
                                  CIScannerContext,
                                  preprocessor_cond,
                                  CITokens *tokens);

extern inline VARIANT_CONSTRUCTOR(CIScannerContext,
                                  CIScannerContext,
                                  preprocessor_if,
                                  CITokens *tokens);

extern inline VARIANT_CONSTRUCTOR(CIScannerContext,
                                  CIScannerContext,
                                  preprocessor_else,
                                  CITokens *tokens);

extern inline CONSTRUCTOR(CIScannerContext,
                          CIScannerContext,
                          enum CIScannerContextLocation ctx_location,
                          CITokens *tokens);

extern inline bool
is_in_macro__CIScannerContext(const CIScannerContext *self);

extern inline bool
is_in_prepro_cond__CIScannerContext(const CIScannerContext *self);

extern inline bool
is_in_prepro_if__CIScannerContext(const CIScannerContext *self);

extern inline bool
is_in_prepro_else__CIScannerContext(const CIScannerContext *self);

// <core/cc/ci/token.h>
extern inline CONSTRUCTOR(CITokens, CITokens);

extern inline bool
is_empty__CITokens(const CITokens *self);

extern inline CONSTRUCTOR(CITokenEot, CITokenEot, enum CITokenEotContext ctx);

extern inline VARIANT_CONSTRUCTOR(CITokenEot, CITokenEot, macro_call);

extern inline VARIANT_CONSTRUCTOR(CITokenEot, CITokenEot, macro_param);

extern inline bool
is_eot_break__CITokenEot(const CITokenEot *self);

extern inline CONSTRUCTOR(CITokenLiteralConstantInt,
                          CITokenLiteralConstantInt,
                          enum CITokenLiteralConstantIntSuffix suffix,
                          String *literal_constant_int);

extern inline DESTRUCTOR(CITokenLiteralConstantInt,
                         const CITokenLiteralConstantInt *self);

extern inline CONSTRUCTOR(CITokenLiteralConstantFloat,
                          CITokenLiteralConstantFloat,
                          enum CITokenLiteralConstantFloatSuffix suffix,
                          String *literal_constant_float);

extern inline DESTRUCTOR(CITokenLiteralConstantFloat,
                         const CITokenLiteralConstantFloat *self);

extern inline CONSTRUCTOR(CITokenPreprocessorDefine,
                          CITokenPreprocessorDefine,
                          String *name,
                          Vec *params,
                          CITokens tokens,
                          bool is_variadic);

extern inline CONSTRUCTOR(CITokenPreprocessorEmbed,
                          CITokenPreprocessorEmbed,
                          String *value,
                          CITokens content);

extern inline CONSTRUCTOR(CITokenPreprocessorLine,
                          CITokenPreprocessorLine,
                          Usize line,
                          String *filename);

extern inline CONSTRUCTOR(CITokenPreprocessorIf,
                          CITokenPreprocessorIf,
                          CITokens cond,
                          CITokens content);

extern inline CONSTRUCTOR(CITokenPreprocessorElif,
                          CITokenPreprocessorElif,
                          CITokens cond,
                          CITokens content);

extern inline DESTRUCTOR(CITokenPreprocessorElif,
                         const CITokenPreprocessorElif *self);

extern inline CONSTRUCTOR(CITokenPreprocessorIfdef,
                          CITokenPreprocessorIfdef,
                          String *identifier,
                          CITokens content);

extern inline CONSTRUCTOR(CITokenPreprocessorIfndef,
                          CITokenPreprocessorIfndef,
                          String *identifier,
                          CITokens content);

extern inline CONSTRUCTOR(CITokenPreprocessorElifdef,
                          CITokenPreprocessorElifdef,
                          String *identifier,
                          CITokens content);

extern inline CONSTRUCTOR(CITokenPreprocessorElifndef,
                          CITokenPreprocessorElifndef,
                          String *identifier,
                          CITokens content);

extern inline CONSTRUCTOR(CITokenPreprocessorElse,
                          CITokenPreprocessorElse,
                          CITokens content);

extern inline CONSTRUCTOR(CITokenPreprocessorInclude,
                          CITokenPreprocessorInclude,
                          String *value);

extern inline DESTRUCTOR(CITokenPreprocessorInclude,
                         const CITokenPreprocessorInclude *self);

extern inline DESTRUCTOR(CITokenMacroCallId, CITokenMacroCallId *self);

extern inline CONSTRUCTOR(CITokenMacroParam, CITokenMacroParam, Usize id);

extern inline DESTRUCTOR(CITokenMacroParam, const CITokenMacroParam *self);

extern inline CONSTRUCTOR(CITokenMacroParamVariadic, CITokenMacroParamVariadic);

extern inline DESTRUCTOR(CITokenMacroParamVariadic,
                         const CITokenMacroParamVariadic *self);

#endif // LILY_EX_LIB_LILY_CORE_CC_CI_C
