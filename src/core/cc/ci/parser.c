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

#include <base/assert.h>
#include <base/atof.h>
#include <base/atoi.h>
#include <base/format.h>

#include <core/cc/ci/diagnostic/error.h>
#include <core/cc/ci/include.h>
#include <core/cc/ci/parser.h>
#include <core/cc/ci/resolver/expr.h>
#include <core/cc/ci/result.h>
#include <core/shared/diagnostic.h>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

enum CIDataTypeCombination
{
    CI_DATA_TYPE_COMBINATION_NONE = 0,
    CI_DATA_TYPE_COMBINATION_CHAR = 1 << 0,
    CI_DATA_TYPE_COMBINATION_DOUBLE = 1 << 1,
    CI_DATA_TYPE_COMBINATION_FLOAT = 1 << 2,
    CI_DATA_TYPE_COMBINATION_INT = 1 << 3,
    CI_DATA_TYPE_COMBINATION_LONG = 1 << 4,
    CI_DATA_TYPE_COMBINATION_LONG_LONG = 1 << 5,
    CI_DATA_TYPE_COMBINATION_SHORT = 1 << 6,
    CI_DATA_TYPE_COMBINATION_SIGNED = 1 << 7,
    CI_DATA_TYPE_COMBINATION_UNSIGNED = 1 << 8,
    CI_DATA_TYPE_COMBINATION__COMPLEX = 1 << 9,
    CI_DATA_TYPE_COMBINATION__IMAGINARY = 1 << 10,
};

struct CurrentSwitch
{
    bool is_integer;
    bool is_float;
    bool is_present;
};

struct CurrentGenericParams
{
    CIGenericParams *called; // CIGenericParams*? (&)
    CIGenericParams *decl;   // CIGenericParams*? (&)
};

static inline CONSTRUCTOR(struct CurrentGenericParams,
                          CurrentGenericParams,
                          CIGenericParams *called,
                          CIGenericParams *decl);

static struct CurrentGenericParams
default__CurrentGenericParams();

struct CITypecheckContext
{
    const CIDecl *current_decl; // const CIDecl*? (&)
    struct CurrentGenericParams current_generic_params;
    const CIScopeID *current_scope_id; // const CIScopeID*? (&)
    struct CurrentSwitch current_switch;
};

static inline CONSTRUCTOR(struct CITypecheckContext,
                          CITypecheckContext,
                          const CIDecl *current_decl,
                          CIGenericParams *called_generic_params,
                          CIGenericParams *decl_generic_params);

/// @return Old value of generic params
static inline struct CurrentGenericParams
set_current_generic_params__CITypecheckContext(
  struct CITypecheckContext *self,
  struct CurrentGenericParams new_current_generic_params);

static inline void
init_function_to_visit_waiting_list__CIParser(CIParser *self, String *name);

static inline void
init_typedef_to_visit_waiting_list__CIParser(CIParser *self, String *name);

static inline void
init_union_to_visit_waiting_list__CIParser(CIParser *self, String *name);

static inline void
init_struct_to_visit_waiting_list__CIParser(CIParser *self, String *name);

static void
add_function_to_visit_waiting_list__CIParser(CIParser *self,
                                             String *name,
                                             CIGenericParams *generic_params);

static void
add_typedef_to_visit_waiting_list__CIParser(CIParser *self,
                                            String *name,
                                            CIGenericParams *generic_params);

static void
add_union_to_visit_waiting_list__CIParser(CIParser *self,
                                          String *name,
                                          CIGenericParams *generic_params);

static void
add_struct_to_visit_waiting_list__CIParser(CIParser *self,
                                           String *name,
                                           CIGenericParams *generic_params);

static void
add_item_to_visit_waiting_list__CIParser(CIParser *self,
                                         enum CIDeclKind kind,
                                         String *name,
                                         CIGenericParams *generic_params);

/// @return CIParserVisitWaitingListItem*? (&)
static inline CIParserVisitWaitingListItem *
get_function_from_visit_waiting_list__CIParser(const CIParser *self,
                                               const String *name);

/// @return CIParserVisitWaitingListItem*? (&)
static inline CIParserVisitWaitingListItem *
get_typedef_from_visit_waiting_list__CIParser(const CIParser *self,
                                              const String *name);

/// @return CIParserVisitWaitingListItem*? (&)
static inline CIParserVisitWaitingListItem *
get_union_from_visit_waiting_list__CIParser(const CIParser *self,
                                            const String *name);

/// @return CIParserVisitWaitingListItem*? (&)
static inline CIParserVisitWaitingListItem *
get_struct_from_visit_waiting_list__CIParser(const CIParser *self,
                                             const String *name);

/// @brief Checks if the corresponding function name is recursive.
/// @note The corresponding function must exist.
static inline bool
is_recursive_function__CIParser(const CIParser *self, const String *name);

static inline bool
is_recursive_typedef__CIParser(const CIParser *self, const String *name);

/// @brief Checks if the corresponding union name is recursive.
/// @note The corresponding union must exist.
static inline bool
is_recursive_union__CIParser(const CIParser *self, const String *name);

/// @brief Checks if the corresponding struct name is recursive.
/// @note The corresponding struct must exist.
static inline bool
is_recursive_struct__CIParser(const CIParser *self, const String *name);

/// @return Rc<String*>* (&)
static Rc *
generate_name_error__CIParser();

/// @param called_generic_params const CIGenericParams*? (&)
/// @param decl_generic_params const CIGenericParams*? (&)
static bool
is_integer_data_type__CIParser(CIParser *self,
                               CIDataType *data_type,
                               bool allow_implicit_cast,
                               const CIGenericParams *called_generic_params,
                               const CIGenericParams *decl_generic_params);

/// @param called_generic_params const CIGenericParams*? (&)
/// @param decl_generic_params const CIGenericParams*? (&)
static bool
is_float_data_type__CIParser(CIParser *self,
                             CIDataType *data_type,
                             const CIGenericParams *called_generic_params,
                             const CIGenericParams *decl_generic_params);

/// @param called_generic_params const CIGenericParams*? (&)
/// @param decl_generic_params const CIGenericParams*? (&)
static bool
is_ptr_data_type__CIParser(CIParser *self,
                           CIDataType *data_type,
                           const CIGenericParams *called_generic_params,
                           const CIGenericParams *decl_generic_params);

/// @param called_generic_params const CIGenericParams*? (&)
/// @param decl_generic_params const CIGenericParams*? (&)
static bool
is_void_ptr_data_type__CIParser(CIParser *self,
                                const CIDataType *data_type,
                                const CIGenericParams *called_generic_params,
                                const CIGenericParams *decl_generic_params);

/// @param called_generic_params const CIGenericParams*? (&)
/// @param decl_generic_params const CIGenericParams*? (&)
static bool
is_struct_or_union_data_type__CIParser(
  CIParser *self,
  CIDataType *data_type,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params);

/// @param called_generic_params const CIGenericParams*? (&)
/// @param decl_generic_params const CIGenericParams*? (&)
/// @return The depth of pointer compatible data type.
static Usize
count_compatible_pointer_depth__CIParser(
  CIParser *self,
  const CIDataType *data_type,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params);

/// @param called_generic_params const CIGenericParams*? (&)
/// @param decl_generic_params const CIGenericParams*? (&)
static bool
is_compatible_with_void_ptr_data_type__CIParser(
  CIParser *self,
  const CIDataType *left,
  const CIDataType *right,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params);

/// @param called_generic_params const CIGenericParams*? (&)
/// @param decl_generic_params const CIGenericParams*? (&)
/// @return CIDataType* (&)
static CIDataType *
unwrap_implicit_ptr_data_type__CIParser(
  CIParser *self,
  CIDataType *data_type,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params);

static void
resolve_function_visit_waiting_list__CIParser(CIParser *self);

static void
resolve_typedef_visit_waiting_list__CIParser(CIParser *self);

static void
resolve_union_visit_waiting_list__CIParser(CIParser *self);

static void
resolve_struct_visit_waiting_list__CIParser(CIParser *self);

static void
resolve_visit_waiting_list__CIParser(CIParser *self);

static void
set_current_token__CIParser(CIParser *self, CIToken *next_token);

/// @brief Check that the token is available in accordance with the standard.
/// @note This function can modify the token type.
static void
check_standard__CIParser(CIParser *self, CIToken *token);

/// @brief Advance to one token on the current iterator.
static void
next_token__CIParser(CIParser *self);

static inline void
set_current_scope__CIParser(CIParser *self);

/// @brief Peek token at position + n.
static CIToken *
peek_token__CIParser(CIParser *self, Usize n);

/// @brief Check if the current token has the same kind than passed in
/// parameter.
static bool
expect__CIParser(CIParser *self, enum CITokenKind kind, bool emit_error);

/// @brief Check if the current token has the same type as the current element
/// in the type list.
static bool
expect_many__CIParser(CIParser *self, Usize n, ...);

/// @brief Check if the current token has the same kind than an element in the
/// list of kind (only one element needs to match for this function not to emit
/// an error).
static void
expect_with_list__CIParser(CIParser *self, Usize n, ...);

/// @brief Advance to n token(s).
static void
jump__CIParser(CIParser *self, Usize n);

/// @brief Check if the passed token is a data type.
static bool
token_is_data_type__CIParser(CIParser *self, const CIToken *token);

/// @brief Check if the current token can be a data type.
static inline bool
is_data_type__CIParser(CIParser *self);

/// @brief Check if the passed token is a storage class.
static bool
token_is_storage_class__CIParser(CIParser *self, const CIToken *token);

/// @brief Check if the current token can be a storage class.
static inline bool
is_storage_class__CIParser(CIParser *self);

/// @brief Check if the passed token is a data type qualifier.
static bool
token_is_data_type_qualifier__CIParser(CIParser *self, const CIToken *token);

/// @brief Check if the current token can be a data type qualifier.
static inline bool
is_data_type_qualifier__CIParser(CIParser *self);

static CIDataType *
substitute_and_generate_from_data_type__CIParser(
  CIParser *self,
  CIDataType *data_type,
  CIGenericParams *generic_params,
  CIGenericParams *called_generic_params);

/// @return the generated fields.
static Vec *
visit_struct_or_union__CIParser(CIParser *self,
                                const CIDecl *decl,
                                CIGenericParams *called_generic_params);

/// @return the generated data type.
/// e.g. typedef <data_type> <name>;
static CIDataType *
visit_typedef__CIParser(CIParser *self,
                        const CIDecl *decl,
                        CIGenericParams *called_generic_params);

static void
visit_function_data_type__CIParser(CIParser *self,
                                   const CIDataTypeFunction *function,
                                   CIGenericParams *called_generic_params,
                                   CIGenericParams *decl_generic_params);

static void
visit_data_type__CIParser(CIParser *self,
                          const CIDataType *data_type,
                          CIGenericParams *called_generic_params,
                          CIGenericParams *decl_generic_params);

static void
visit_function_decl_variable__CIParser(CIParser *self,
                                       const CIDeclVariable *variable,
                                       CIGenericParams *called_generic_params,
                                       CIGenericParams *decl_generic_params);

static void
visit_function_decl__CIParser(CIParser *self,
                              const CIDecl *decl,
                              CIGenericParams *called_generic_params,
                              CIGenericParams *decl_generic_params);

static void
visit_function_expr_array_access__CIParser(
  CIParser *self,
  const CIExprArrayAccess *array_access,
  CIGenericParams *called_generic_params,
  CIGenericParams *decl_generic_params);

static void
visit_function_expr_binary__CIParser(CIParser *self,
                                     const CIExprBinary *binary,
                                     CIGenericParams *called_generic_params,
                                     CIGenericParams *decl_generic_params);

static void
visit_function_expr_cast__CIParser(CIParser *self,
                                   const CIExprCast *cast,
                                   CIGenericParams *called_generic_params,
                                   CIGenericParams *decl_generic_params);

static void
visit_function_expr_ternary__CIParser(CIParser *self,
                                      const CIExprTernary *ternary,
                                      CIGenericParams *called_generic_params,
                                      CIGenericParams *decl_generic_params);

static void
visit_function_expr_function_call__CIParser(
  CIParser *self,
  const CIExprFunctionCall *function_call,
  CIGenericParams *called_generic_params,
  CIGenericParams *decl_generic_params);

static void
visit_function_expr_function_call_builtin__CIParser(
  CIParser *self,
  const CIExprFunctionCallBuiltin *function_call_builtin,
  CIGenericParams *called_generic_params,
  CIGenericParams *decl_generic_params);

static void
visit_function_expr_struct_call__CIParser(
  CIParser *self,
  const CIExprStructCall *struct_call,
  CIGenericParams *called_generic_params,
  CIGenericParams *decl_generic_params);

static void
visit_function_expr__CIParser(CIParser *self,
                              const CIExpr *expr,
                              CIGenericParams *called_generic_params,
                              CIGenericParams *decl_generic_params);

static inline void
visit_function_stmt_block__CIParser(CIParser *self,
                                    const CIStmtBlock *block,
                                    CIGenericParams *called_generic_params,
                                    CIGenericParams *decl_generic_params);

static inline void
visit_function_stmt_do_while__CIParser(CIParser *self,
                                       const CIStmtDoWhile *do_while,
                                       CIGenericParams *called_generic_params,
                                       CIGenericParams *decl_generic_params);

static void
visit_function_stmt_for__CIParser(CIParser *self,
                                  const CIStmtFor *for_,
                                  CIGenericParams *called_generic_params,
                                  CIGenericParams *decl_generic_params);

static void
visit_function_stmt_if_branch__CIParser(CIParser *self,
                                        const CIStmtIfBranch *if_branch,
                                        CIGenericParams *called_generic_params,
                                        CIGenericParams *decl_generic_params);

static void
visit_function_stmt_if__CIParser(CIParser *self,
                                 const CIStmtIf *if_,
                                 CIGenericParams *called_generic_params,
                                 CIGenericParams *decl_generic_params);

static inline void
visit_function_stmt_switch__CIParser(CIParser *self,
                                     const CIStmtSwitch *switch_,
                                     CIGenericParams *called_generic_params,
                                     CIGenericParams *decl_generic_params);

static inline void
visit_function_stmt_while__CIParser(CIParser *self,
                                    const CIStmtWhile *while_,
                                    CIGenericParams *called_generic_params,
                                    CIGenericParams *decl_generic_params);

static void
visit_function_stmt__CIParser(CIParser *self,
                              const CIStmt *stmt,
                              CIGenericParams *called_generic_params,
                              CIGenericParams *decl_generic_params);

static void
visit_function_item__CIParser(CIParser *self,
                              const CIDeclFunctionItem *item,
                              CIGenericParams *called_generic_params,
                              CIGenericParams *decl_generic_params);

static void
visit_function_body__CIParser(CIParser *self,
                              const CIDeclFunctionBody *body,
                              CIGenericParams *called_generic_params,
                              CIGenericParams *decl_generic_params);

static void
visit_function__CIParser(CIParser *self,
                         const CIDecl *function_decl,
                         CIGenericParams *called_generic_params);

/// @brief Generaste function gen.
static void
generate_function_gen__CIParser(CIParser *self,
                                String *function_name,
                                CIGenericParams *called_generic_params);

/// @brief Generate struct, union or typedef gen.
/// @param name String* (&)
static void
generate_type_gen__CIParser(CIParser *self,
                            enum CIDeclKind kind,
                            String *name,
                            CIGenericParams *called_generic_params,
                            bool(const CIParser *, const String *),
                            CIDecl *(*search_decl)(const CIResultFile *,
                                                   const String *));

/// @param struct_name String* (&)
static inline void
generate_struct_gen__CIParser(CIParser *self,
                              String *struct_name,
                              CIGenericParams *called_generic_params);

/// @param union_name String* (&)
static inline void
generate_union_gen__CIParser(CIParser *self,
                             String *union_name,
                             CIGenericParams *called_generic_params);

/// @param typedef_name_ref String* (&)
static inline void
generate_typedef_gen__CIParser(CIParser *self,
                               String *typedef_name_ref,
                               CIGenericParams *called_generic_params);

/// @brief Parse data type context(s), such as: !heap, !non_null, !stack or
/// !trace.
static int
parse_data_type_contexts__CIParser(CIParser *self);

/// @brief Parse post data type, like pointer, ...
/// @param CIDataType*?
static CIDataType *
parse_post_data_type__CIParser(CIParser *self, CIDataType *data_type);

/// @brief Parse variable name (name + data type).
/// @param name_ref Rc<String*>* (&)* (&)
///
/// @example field, variable, parameter
///
/// variable:
///
/// int a[2];
/// //   ^^^
///
/// int *(*fn)(int, int);
/// //   ^^^^^^^^^^^^^^^
///
/// field:
///
/// struct {
/// 	int a[2];
/// 	//   ^^^
/// 	int *(*fn)(int, int);
/// 	//   ^^^^^^^^^^^^^^^
/// }
static void
parse_variable_name_and_data_type__CIParser(CIParser *self,
                                            CIDataType **data_type_ref,
                                            Rc **name_ref,
                                            bool name_is_required);

static void
parse_storage_class_specifiers_and_data_type_qualifiers__CIParser(
  CIParser *self,
  int *data_type_qualifier_flag,
  int *storage_class_flag);

/// @brief Set and verify if the qualifier is valid given the data type.
static void
set_and_check_data_type_qualifier__CIParser(CIParser *self,
                                            CIDataType *data_type,
                                            int data_type_qualifier_flag);

/// @brief Check if the data type context is compatible with other data type
/// context contains in `context_flag`.
static void
valid_data_type_context__CIParser(CIParser *self, int context_flag);

static void
valid_data_type_context_according_data_type__CIParser(CIParser *self,
                                                      CIDataType *data_type,
                                                      int context_flag);

static void
set_and_check_data_type_context__CIParser(CIParser *self,
                                          CIDataType *data_type,
                                          int context_flag);

/// @brief Add flag to multi part keyword.
static void
add_flag__CIDataTypeCombination(CIParser *self,
                                Uint32 *flags,
                                enum CIDataTypeCombination flag);

/// @brief Convert data type combination to token kind.
static enum CIDataTypeCombination
convert_token_kind_to_data_type_combination__CIDataTypeCombination(
  enum CITokenKind kind);

/// @brief Check if the token kind is multi part keyword.
bool
is_multi_part_keyword_from_token_kind__CIKeywordMultiPart(
  enum CITokenKind kind);

/// @return enum CIDataTypeKind | int (-1)
static int
convert_data_type_combination_to_data_type__CIParser(CIParser *self,
                                                     int combination);

/// @example signed int, long long int, ...
/// @return enum CIDataTypeKind | int (-1)
static int
parse_data_type_combination__CIParser(CIParser *self);

static CIDataType *
parse_pre_data_type__CIParser(CIParser *self);

/// @brief Parse enum variants.
/// @return Vec<CIDeclEnumVariant*>*
static Vec *
parse_enum_variants__CIParser(CIParser *self);

/// @brief Parse enum declaration.
/// @param name Rc<String*>* (&)
static CIDecl *
parse_enum__CIParser(CIParser *self, int storage_class_flag, Rc *name);

/// @brief Parse function params.
/// @param parent_function_scope CIScope*? (&)
/// @return Vec<CIDeclFunctionParam*>*?
static Vec *
parse_function_params__CIParser(CIParser *self, CIScope *parent_function_scope);

/// @brief Parse generic params.
/// @return Vec<CIDataType*>*?
/// @example
///
/// struct Vec.[@T] {
/// 	@T *buffer;
/// 	Usize len;
/// 	Usize capacity;
/// };
///
/// @T get.[@T](@T x) {
/// 	return x;
/// }
static CIGenericParams *
parse_generic_params__CIParser(CIParser *self);

/// @brief Parser function call.
/// @param identifier Rc<String*>* (&)
static CIExpr *
parse_function_call__CIParser(CIParser *self,
                              Rc *identifier,
                              CIGenericParams *generic_params);

/// @brief Parse literal expression.
/// @brief @return CIExpr*?
static CIExpr *
parse_literal_expr__CIParser(CIParser *self);

/// @brief Parse primary expression.
/// @return CIExpr*?
static CIExpr *
parse_primary_expr__CIParser(CIParser *self);

static CIExpr *
parse_struct_call__CIParser(CIParser *self);

static CIExpr *
parse_array__CIParser(CIParser *self);

static CIExpr *
parse_binary_expr__CIParser(CIParser *self, CIExpr *expr);

/// @example x++, x[0], ...
static CIExpr *
parse_post_expr__CIParser(CIParser *self, CIExpr *expr);

/// @return CIDataType*
static CIDataType *
perform_implicit_cast_on_array__CIParser(const CIParser *self,
                                         CIDataType *data_type);

/// @param data_type_access CIDataType* (&)
/// @param called_generic_params const CIGenericParams*? (&)
/// @param decl_generic_params const CIGenericParams*? (&)
/// @return CIDataType* (&)
static CIDataType *
resolve_data_type_access__CIParser(const CIParser *self,
                                   CIDataType *data_type_access,
                                   enum CIExprBinaryKind binary_kind,
                                   const CIGenericParams *called_generic_params,
                                   const CIGenericParams *decl_generic_params);

/// @param fields const Vec<CIDeclStructField*>* (&)
/// @param called_generic_params const CIGenericParams*? (&)
/// @param decl_generic_params const CIGenericParams*? (&)
/// @return CIDataType*
static CIDataType *
infer_expr_access_data_type__CIParser(
  const CIParser *self,
  const CIExpr *expr_access,
  const Vec *fields,
  const CIScopeID *current_scope_id,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params);

/// @param called_generic_params const CIGenericParams*? (&)
/// @param decl_generic_params const CIGenericParams*? (&)
static CIDataType *
infer_expr_dot_binary_access_data_type__CIParser(
  const CIParser *self,
  const CIExprBinary *dot_access,
  const CIScopeID *current_scope_id,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params);

/// @param called_generic_params const CIGenericParams*? (&)
/// @param decl_generic_params const CIGenericParams*? (&)
static CIDataType *
infer_expr_arrow_binary_access_data_type__CIParser(
  const CIParser *self,
  const CIExprBinary *arrow_access,
  const CIScopeID *current_scope_id,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params);

static CIDataType *
infer_expr_literal_data_type__CIParser(const CIParser *self,
                                       const CIExprLiteral *literal);

/// @param called_generic_params const CIGenericParams*? (&)
/// @param decl_generic_params const CIGenericParams*? (&)
static CIDataType *
infer_expr_unary_data_type__CIParser(
  const CIParser *self,
  const CIExprUnary *unary,
  const CIScopeID *current_scope_id,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params);

/// @param called_generic_params CIGenericParams*? (&)
/// @return CIDecl*? (&)
static CIDecl *
search_decl__CIParser(const CIParser *self,
                      const String *name,
                      CIGenericParams *called_generic_params,
                      CIDecl *(*search_decl)(const CIResultFile *self,
                                             const String *name));

/// @param called_generic_params CIGenericParams*? (&)
/// @return CIDecl*? (&)
static inline CIDecl *
search_function__CIParser(const CIParser *self,
                          const String *name,
                          CIGenericParams *called_generic_params);

/// @brief Search generalizable declaration from the given name, if
/// gen_decl_generic_params is NULL otherwise, it serializes the given
/// name and searches declaration with this serialized name.
/// @note In cases where the declaration with the base name exists, but the
/// declaration with the serialized name does not, this function generates a
/// "gen declaration".
/// @param gen_decl_generic_params CIGenericParams*? (&)
/// @param called_generic_params CIGenericParams*? (&)
/// @param decl_generic_params CIGenericParams*? (&)
/// @return CIDecl*? (&)
///
/// @example of use:
///
/// @T get.[@T](@T x) {
///   return x;
/// }
///
/// @T get2.[@T](@T x) {
///   return get.[@T](x);
/// //       ^^^^^^^^^^^ need to search this declaration with this function:
/// //                   `search_decl_in_generic_context__CIParser`.
/// }
static CIDecl *
search_decl_in_generic_context__CIParser(
  const CIParser *self,
  const String *name,
  CIGenericParams *gen_decl_generic_params,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params,
  CIDecl *(*search_decl)(const CIResultFile *self, const String *name),
  void (*generate_decl_gen)(CIParser *self,
                            String *name,
                            CIGenericParams *called_generic_params));

/// @brief Search function from the given name, if function_generic_params is
/// NULL otherwise, it serializes the given name and searches generic function
/// with this serialized name.
/// @param function_generic_params CIGenericParams*? (&)
/// @param called_generic_params CIGenericParams*? (&)
/// @param decl_generic_params CIGenericParams*? (&)
/// @return CIDecl*? (&)
static inline CIDecl *
search_function_in_generic_context__CIParser(
  const CIParser *self,
  const String *name,
  CIGenericParams *function_generic_params,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params);

/// @brief Search struct from the given name, if struct_generic_params is NULL
/// otherwise, it serializes the given name and searches generic struct with
/// this serialized name.
/// @param struct_generic_params CIGenericParams*? (&)
/// @param called_generic_params CIGenericParams*? (&)
/// @param decl_generic_params CIGenericParams*? (&)
/// @return CIDecl*? (&)
static inline CIDecl *
search_struct_in_generic_context__CIParser(
  const CIParser *self,
  const String *name,
  CIGenericParams *struct_generic_params,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params);

/// @brief Search typedef from the given name, if typedef_generic_params is NULL
/// otherwise, it serializes the given name and searches generic typedef with
/// this serialized name.
/// @param typedef_generic_params CIGenericParams*? (&)
/// @param called_generic_params CIGenericParams*? (&)
/// @param decl_generic_params CIGenericParams*? (&)
/// @return CIDecl*? (&)
static inline CIDecl *
search_typedef_in_generic_context__CIParser(
  const CIParser *self,
  const String *name,
  CIGenericParams *typedef_generic_params,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params);

/// @brief Search union from the given name, if union_generic_params is NULL
/// otherwise, it serializes the given name and searches generic union with this
/// serialized name.
/// @param union_generic_params CIGenericParams*? (&)
/// @param called_generic_params CIGenericParams*? (&)
/// @param decl_generic_params CIGenericParams*? (&)
/// @return CIDecl*? (&)
static inline CIDecl *
search_union_in_generic_context__CIParser(
  const CIParser *self,
  const String *name,
  CIGenericParams *union_generic_params,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params);

/// @param called_generic_params const CIGenericParams*? (&)
/// @param decl_generic_params const CIGenericParams*? (&)
/// @return CIDataType*
static CIDataType *
resolve_generic_data_type__CIParser(
  const CIParser *self,
  CIDataType *data_type,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params);

/// @brief This function returns the given data type if it is not struct,
/// otherwise it attempts to resolve the struct data type.
/// @param called_generic_params const CIGenericParams*? (&)
/// @param decl_generic_params const CIGenericParams*? (&)
/// @return CIDataType* (&)
static CIDataType *
resolve_struct_data_type__CIParser(const CIParser *self,
                                   CIDataType *data_type,
                                   const CIGenericParams *called_generic_params,
                                   const CIGenericParams *decl_generic_params);

/// @brief This function returns the given data type if it is not typedef,
/// otherwise it attempts to resolve the typedef data type.
/// @param called_generic_params const CIGenericParams*? (&)
/// @param decl_generic_params const CIGenericParams*? (&)
/// @return CIDataType*
static CIDataType *
resolve_typedef_data_type__CIParser(
  const CIParser *self,
  CIDataType *data_type,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params);

/// @param called_generic_params const CIGenericParams*? (&)
/// @param decl_generic_params const CIGenericParams*? (&)
static CIDataType *
resolve_union_data_type__CIParser(const CIParser *self,
                                  CIDataType *data_type,
                                  const CIGenericParams *called_generic_params,
                                  const CIGenericParams *decl_generic_params);

/// @param called_generic_params const CIGenericParams*? (&)
/// @param decl_generic_params const CIGenericParams*? (&)
/// @return const CIDataType* (&)
static CIDataType *
resolve_data_type__CIParser(const CIParser *self,
                            CIDataType *data_type,
                            const CIGenericParams *called_generic_params,
                            const CIGenericParams *decl_generic_params);

/// @param called_generic_params const CIGenericParams*? (&)
/// @param decl_generic_params const CIGenericParams*? (&)
/// @return const Vec<CIDeclStructField*>*? (&)
static const Vec *
get_fields_from_data_type__CIParser(
  const CIParser *self,
  const CIDataType *data_type,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params);

/// @param left_fields const Vec<CIDeclStructField*>* (&)
/// @param right_fields const Vec<CIDeclStructField*>* (&)
static bool
typecheck_struct_field__CIParser(const CIParser *self,
                                 const Vec *left_fields,
                                 const Vec *right_fields,
                                 struct CITypecheckContext *typecheck_ctx);

/// @param left_fields const Vec<CIDeclStructField*>* (&)
/// @param right_fields const Vec<CIDeclStructField*>* (&)
static bool
typecheck_union_field__CIParser(const CIParser *self,
                                const Vec *decl_fields,
                                const Vec *called_fields,
                                struct CITypecheckContext *typecheck_ctx);

/// @note This is a very specific case, but it allows you to manage
/// the situation where you declare a struct with an array
/// initializer, and all the fields in the struct are of the same
/// type.
///
/// @example
///
/// struct X {
/// 	int x;
/// 	int y;
/// 	int z;
/// };
///
/// struct X x = { 1, 2, 3 };
/// ^^^^^^^^     ^^^^^^^^^^^
/// |                      |
/// inferred as struct     inferred as array
static bool
is_valid_implicit_cast_from_array_to_struct__CIParser(
  const CIParser *self,
  CIDataType *array_dt,
  CIDataType *struct_dt,
  struct CITypecheckContext *typecheck_ctx);

static bool
is_valid_implicit_cast_from_array_to_union__CIParser(
  const CIParser *self,
  CIDataType *array_dt,
  CIDataType *union_dt,
  struct CITypecheckContext *typecheck_ctx);

static bool
is_valid_implicit_cast_from_struct_to_union__CIParser(
  const CIParser *self,
  CIDataType *struct_dt,
  CIDataType *union_dt,
  struct CITypecheckContext *typecheck_ctx);

static bool
is_valid_implicit_cast__CIParser(const CIParser *self,
                                 CIDataType *left,
                                 CIDataType *right,
                                 struct CITypecheckContext *typecheck_ctx);

static bool
perform_typecheck__CIParser(const CIParser *self,
                            CIDataType *expected_data_type,
                            CIDataType *given_data_type,
                            bool can_try,
                            struct CITypecheckContext *typecheck_ctx);

static void
typecheck_variable_decl__CIParser(const CIParser *self,
                                  const CIDeclVariable *variable,
                                  struct CITypecheckContext *typecheck_ctx);

static void
typecheck_decl__CIParser(const CIParser *self,
                         const CIDecl *decl,
                         struct CITypecheckContext *typecheck_ctx);

static void
typecheck_binary_expr__CIParser(const CIParser *self,
                                const CIExprBinary *binary,
                                struct CITypecheckContext *typecheck_ctx);

/// @param decl_params Vec<CIDeclFunctionParam*>* (&)
/// @param called_params Vec<CIExpr*>* (&)
static void
typecheck_function_call_expr_params__CIParser(
  const CIParser *self,
  const CIDecl *decl_function_call,
  const Vec *called_params,
  struct CITypecheckContext *typecheck_ctx);

static void
typecheck_function_call_expr__CIParser(
  const CIParser *self,
  const CIExprFunctionCall *function_call,
  struct CITypecheckContext *typecheck_ctx);

/// @param decl_params Vec<CIDataType*>* (&)
/// @param called_params Vec<CIExpr*>* (&)
static void
typecheck_function_call_builtin_expr_params__CIParser(
  const CIParser *self,
  const CIBuiltinFunction *decl_function_call_builtin,
  const Vec *called_params,
  struct CITypecheckContext *typecheck_ctx);

static void
typecheck_function_call_builtin_expr__CIParser(
  const CIParser *self,
  const CIExprFunctionCallBuiltin *function_call_builtin,
  struct CITypecheckContext *typecheck_ctx);

static void
typecheck_ternary_expr__CIParser(const CIParser *self,
                                 const CIExprTernary *ternary,
                                 struct CITypecheckContext *typecheck_ctx);

static void
typecheck_unary_expr__CIParser(const CIParser *self,
                               const CIExprUnary *unary,
                               struct CITypecheckContext *typecheck_ctx);

/// @param called_generic_params const CIGenericParams*? (&)
/// @param decl_generic_params const CIGenericParams*? (&)
static void
typecheck_expr__CIParser(const CIParser *self,
                         CIDataType *expected_data_type,
                         const CIExpr *given_expr,
                         struct CITypecheckContext *typecheck_ctx);

/// @brief Typecheck the expression and try to discard it whenever possible.
static void
typecheck_expr_and_try_discard(const CIParser *self,
                               CIExpr *expr,
                               struct CITypecheckContext *typecheck_ctx);

/// @param body const CIDeclFunctionItem* (&)
/// @param called_generic_params const CIGenericParams*? (&)
/// @param decl_generic_params const CIGenericParams*? (&)
static void
typecheck_body_item__CIParser(const CIParser *self,
                              const CIDeclFunctionItem *item,
                              struct CITypecheckContext *typecheck_ctx);

/// @param body const Vec<CIDeclFunctionItem*>*? (&)
/// @param called_generic_params const CIGenericParams*? (&)
/// @param decl_generic_params const CIGenericParams*? (&)
static void
typecheck_body__CIParser(const CIParser *self,
                         const CIDeclFunctionBody *body,
                         struct CITypecheckContext *typecheck_ctx);

static void
typecheck_case_stmt__CIParser(const CIParser *self,
                              const CIStmtSwitchCase *case_,
                              struct CITypecheckContext *typecheck_ctx);

static void
typecheck_condition_expr__CIParser(const CIParser *self,
                                   const CIExpr *cond,
                                   struct CITypecheckContext *typecheck_ctx);

static void
typecheck_do_while_stmt__CIParser(const CIParser *self,
                                  const CIStmtDoWhile *do_while,
                                  struct CITypecheckContext *typecheck_ctx);

static void
typecheck_if_stmt_branch__CIParser(const CIParser *self,
                                   const CIStmtIfBranch *if_branch,
                                   struct CITypecheckContext *typecheck_ctx);

static void
typecheck_if_stmt__CIParser(const CIParser *self,
                            const CIStmtIf *if_,
                            struct CITypecheckContext *typecheck_ctx);

static void
typecheck_for_stmt__CIParser(const CIParser *self,
                             const CIStmtFor *for_,
                             struct CITypecheckContext *typecheck_ctx);

/// @param goto_ const String* (&)
static void
typecheck_goto_stmt__CIParser(const CIParser *self,
                              const String *goto_,
                              struct CITypecheckContext *typecheck_ctx);

/// @param return_ const CIExpr*? (&)
static void
typecheck_return_stmt__CIParser(const CIParser *self,
                                const CIExpr *return_,
                                struct CITypecheckContext *typecheck_ctx);

static void
typecheck_switch_stmt__CIParser(const CIParser *self,
                                const CIStmtSwitch *switch_,
                                struct CITypecheckContext *typecheck_ctx);

static void
typecheck_while_stmt__CIParser(const CIParser *self,
                               const CIStmtWhile *while_,
                               struct CITypecheckContext *typecheck_ctx);

/// @param called_generic_params const CIGenericParams*? (&)
/// @param decl_generic_params const CIGenericParams*? (&)
static void
typecheck_stmt__CIParser(const CIParser *self,
                         const CIStmt *given_stmt,
                         struct CITypecheckContext *typecheck_ctx);

static void
typecheck_function__CIParser(const CIParser *self, const CIDecl *function_decl);

static void
typecheck_function_gen__CIParser(const CIParser *self,
                                 const CIDecl *function_gen_decl);

static CIDeclFunctionItem *
parse_case__CIParser(CIParser *self);

static CIDeclFunctionItem *
parse_do_while_stmt__CIParser(CIParser *self, bool in_switch);

static CIDeclFunctionItem *
parse_for_stmt__CIParser(CIParser *self, bool in_switch);

static CIStmtIfBranch *
parse_if_branch__CIParser(CIParser *self, bool in_loop, bool in_switch);

static bool
has_else_if__CIParser(CIParser *self);

static CIDeclFunctionItem *
parse_if_stmt__CIParser(CIParser *self, bool in_loop, bool in_switch);

static CIDeclFunctionItem *
parse_while_stmt__CIParser(CIParser *self, bool in_switch);

static CIDeclFunctionItem *
parse_switch_stmt__CIParser(CIParser *self, bool in_loop);

static CIDeclFunctionItem *
parse_block_stmt__CIParser(CIParser *self, bool in_loop, bool in_switch);

/// @brief Parse statement.
/// @return CIDeclFunctionItem*?
static CIDeclFunctionItem *
parse_stmt__CIParser(CIParser *self, bool in_loop, bool in_switch);

/// @brief Parse function body (base).
/// @param parent_function_scope CIScope* (&)*? (&)
/// @return CIDeclFunctionBody*
static CIDeclFunctionBody *
parse_function_body_base__CIParser(CIParser *self,
                                   bool in_loop,
                                   bool in_switch,
                                   CIScope **parent_function_scope);

/// @brief Parse function body.
/// @param parent_function_scope CIScope* (&)*? (&)
/// @return CIDeclFunctionBody*
static inline CIDeclFunctionBody *
parse_function_body__CIParser(CIParser *self,
                              bool in_loop,
                              bool in_switch,
                              CIScope **parent_function_scope);

/// @brief Parse function block
/// @return CIDeclFunctionBody*
static inline CIDeclFunctionBody *
parse_function_block__CIParser(CIParser *self, bool in_loop, bool in_switch);

static CIDeclFunctionItem *
parse_function_body_item__CIParser(CIParser *self,
                                   bool in_loop,
                                   bool in_switch);

/// @brief Parse function declaration. Note that if the
/// "storage_class_flag" variable contains the storage class "typedef", this
/// function will parse and return a typedef declaration, but if it doesn't, it
/// will parse and return a function declaration.
/// @param name Rc<String*>* (&)
/// @param typedef_generic_params CIGenericParams*?
static CIDecl *
parse_function__CIParser(CIParser *self,
                         int storage_class_flag,
                         CIDataType *return_data_type,
                         Rc *name,
                         CIGenericParams *generic_params,
                         Vec *attributes);

/// @brief Parse fields.
static Vec *
parse_fields__CIParser(CIParser *self);

/// @brief Parse struct or union fields.
/// @return Vec<CIDeclStructField*>*?
static Vec *
parse_struct_or_union_fields__CIParser(CIParser *self);

/// @brief Parse struct declaration.
/// @param name Rc<String*>* (&)
static CIDecl *
parse_struct__CIParser(CIParser *self,
                       int storage_class_flag,
                       Rc *name,
                       CIGenericParams *generic_params);

/// @brief Parse typedef declaration.
/// @param aliased_decl CIDecl* - This can be a variable declaration or a
/// function.
/// @param generic_params CIGenericParams* (&)
/// @note Free aliased_decl.
static CIDecl *
parse_typedef__CIParser(CIParser *self,
                        CIDecl *aliased_decl,
                        CIGenericParams *generic_params);

/// @brief Parse union declaration.
/// @param name Rc<String*>* (&)
static CIDecl *
parse_union__CIParser(CIParser *self,
                      int storage_class_flag,
                      Rc *name,
                      CIGenericParams *generic_params);

static inline bool
is_only_initialization_compatible_data_type__CIParser(CIParser *self,
                                                      CIDataType *data_type);

static inline bool
is_initialization_expr__CIParser(CIParser *self, CIExpr *expr);

static void
check_for_initialization_expr__CIParser(CIParser *self,
                                        CIDataType *data_type,
                                        CIExpr *expr);

/// @brief Parse variable declaration.
/// @param name Rc<String*>*? (&)
static CIDecl *
parse_variable__CIParser(CIParser *self,
                         int storage_class_flag,
                         CIDataType *data_type,
                         Rc *name,
                         bool no_expr,
                         bool is_local);

/// @brief Parse list of variable declaration. Note that if the
/// "storage_class_flag" variable contains the storage class "typedef", this
/// function will parse and return a typedef declaration, but if it doesn't, it
/// will parse and return a variable declaration.
/// @param typedef_generic_params CIGenericParams*?
static CIDecl *
parse_variable_list__CIParser(CIParser *self,
                              CIDecl *current_var,
                              int storage_class_flag,
                              CIDataType *data_type,
                              CIGenericParams *typedef_generic_params);

static CIDecl *
parse_label__CIParser(CIParser *self);

/// @brief Parse declaration.
static CIDecl *
parse_decl__CIParser(CIParser *self);

/// @brief Add declaration to scope. If the declaration addition succeeds, the
/// function returns a NULL pointer, otherwise it returns the pointer to the
/// declaration that conflicts with the declaration we previously tried to add.
/// @param must_free Free the value associated with "decl_ref", if the addition
/// fails.
/// @return const CIDecl*? (&)
/// @note If the passed declaration has no name (e.g. anonymous struct, enum or
/// union), the function will return a NULL pointer by default, as if the
/// addition had actually taken place, but obviously the function won't try to
/// add this declaration to the scope.
static const CIDecl *
add_decl_to_scope__CIParser(CIParser *self, CIDecl **decl_ref, bool must_free);

static CIAttribute *
parse_attribute__CIParser(CIParser *self);

/// @param attributes Vec<CIAttribute*>** (&)
static void
parse_attributes__CIParser(CIParser *self, Vec **attributes);

/// @brief Parse data type qualifier.
/// @return true if the token is a data type qualifier, false otherwise.
static bool
parse_data_type_qualifier__CIParser(CIParser *self,
                                    int *data_type_qualifier_flag);

/// @brief Parse many data type qualifiers.
static void
parse_data_type_qualifiers__CIParser(CIParser *self,
                                     int *data_type_qualifier_flag);

/// @brief Parse storage class specifier.
/// @return true if the token is a storage class specifier, false otherwise.
static bool
parse_storage_class_specifier__CIParser(CIParser *self,
                                        int *storage_class_flag);

/// @brief Parse many storage class specifiers.
static void
parse_storage_class_specifiers__CIParser(CIParser *self,
                                         int *storage_class_flag);

// NOTE: If the program is multi-threaded, you'll need to adapt these variables
// to multi-threading.
// If a name (of a variable, a function, etc.) is expected by the compiler, but
// is not here, it will generate a name and therefore increment the
// `name_error_count` and store all `name_error` in the `names_error` vector, as
// declaration strucutures (CIDeclVariable, CIDeclFunction, ...), are not
// supposed to own the name (in other words not to be able to free it).
static Vec *names_error = NULL; // Vec<Rc<String*>*>*
static Usize name_error_count = 0;
// The `data_type_as_expression` variable is used to keep the `DataType`
// pointer, when in `parse_decl__CIParser`, when `in_function_body` is true is
// that the following (current) token is not a `=`, `;` or `(`. Then, in the
// `parse_expr__CIParser` function, if `data_type_as_expression` is not NULL,
// the data type is returned as an expression.
static CIDataType *data_type_as_expression = NULL;

static int storage_class_flag = CI_STORAGE_CLASS_NONE;

#define RESET_STORAGE_CLASS_FLAG() storage_class_flag = CI_STORAGE_CLASS_NONE;
#define HAS_TYPEDEF_STORAGE_CLASS_FLAG() \
    (storage_class_flag & CI_STORAGE_CLASS_TYPEDEF)

static int data_type_qualifier_flag = CI_DATA_TYPE_QUALIFIER_NONE;

#define RESET_DATA_TYPE_QUALIFIER_FLAG() \
    data_type_qualifier_flag = CI_DATA_TYPE_QUALIFIER_NONE;

// The `in_label` variable checks whether you're in a label and emits an error
// if there's a variable declaration in a label (also in case and default
// statetement).
static bool in_label = false;

#define ENABLE_IN_LABEL() in_label = true;
#define DISABLE_IN_LABEL() \
    if (in_label) {        \
        in_label = false;  \
    }

// Represent the `scope` of the current block being analyzed.
static CIScope *current_scope = NULL; // CIScope*?

// Represent the `body` of the current block being analyzed.
static CIDeclFunctionBody *current_body = NULL; // CIDeclFunctionBody*?

// Allow initialization.
//
// e.g.
//
// Array: { 3, 2, 1, ... }
//
// Struct call: { .a = 1, .b = 2, ... } or { 1, 2, ... }
static bool allow_initialization = false;

#define ENABLE_ALLOW_INITIALIZATION() allow_initialization = true;
#define DISABLE_ALLOW_INITIALIZATION() allow_initialization = false;

static bool resolve_visit_waiting_list = false;

#define ENABLE_RESOLVE_VISIT_WAITING_LIST() resolve_visit_waiting_list = true;
#define DISABLE_RESOLVE_VISIT_WAITING_LIST() resolve_visit_waiting_list = false;

static const CIFeature *tokens_feature = NULL; // CIFeature* (&)

#define HAS_REACH_EOF(token) (token->kind == CI_TOKEN_KIND_EOF)
#define HAS_REACH_EOT(token) (token->kind == CI_TOKEN_KIND_EOT)

// The function is set to true if the parse is currently parseing the body of a
// function.
static bool in_function_body = false;

#define SET_IN_FUNCTION_BODY() in_function_body = true;
#define UNSET_IN_FUNCTION_BODY() in_function_body = false;

// In some situations we don't want to eat the `;` token, for example when
// parsing variable declarations in a `for` loop.
//
// e.g.
//
// for (int i = 0, a = 20;;);
static bool eat_semicolon = true;

#define SET_EAT_SEMICOLON() eat_semicolon = true;
#define UNSET_EAT_SEMICOLON() eat_semicolon = false;

static const SizedStr ci_standard_attributes[CI_N_STANDARD_ATTRIBUTE] = {
    SIZED_STR_FROM_RAW("_Noreturn"),
    SIZED_STR_FROM_RAW("___Noreturn__"),
    SIZED_STR_FROM_RAW("__deprecated__"),
    SIZED_STR_FROM_RAW("__falthrough__"),
    SIZED_STR_FROM_RAW("__maybe_unused__"),
    SIZED_STR_FROM_RAW("__nodiscard__"),
    SIZED_STR_FROM_RAW("__noreturn__"),
    SIZED_STR_FROM_RAW("__unsequenced__"),
    SIZED_STR_FROM_RAW("__reproducible__"),
    SIZED_STR_FROM_RAW("deprecated"),
    SIZED_STR_FROM_RAW("fallthrough"),
    SIZED_STR_FROM_RAW("maybe_unused"),
    SIZED_STR_FROM_RAW("nodiscard"),
    SIZED_STR_FROM_RAW("noreturn"),
    SIZED_STR_FROM_RAW("unsequenced"),
    SIZED_STR_FROM_RAW("reproducible"),
};

static const enum CIAttributeStandardKind
  ci_standard_attribute_ids[CI_N_STANDARD_ATTRIBUTE] = {
      CI_ATTRIBUTE_STANDARD_KIND_NORETURN,
      CI_ATTRIBUTE_STANDARD_KIND_NORETURN,
      CI_ATTRIBUTE_STANDARD_KIND_DEPRECATED,
      CI_ATTRIBUTE_STANDARD_KIND_FALLTHROUGH,
      CI_ATTRIBUTE_STANDARD_KIND_MAYBE_UNUSED,
      CI_ATTRIBUTE_STANDARD_KIND_NODISCARD,
      CI_ATTRIBUTE_STANDARD_KIND_NORETURN,
      CI_ATTRIBUTE_STANDARD_KIND_UNSEQUENCED,
      CI_ATTRIBUTE_STANDARD_KIND_REPRODUCIBLE,
      CI_ATTRIBUTE_STANDARD_KIND_DEPRECATED,
      CI_ATTRIBUTE_STANDARD_KIND_FALLTHROUGH,
      CI_ATTRIBUTE_STANDARD_KIND_MAYBE_UNUSED,
      CI_ATTRIBUTE_STANDARD_KIND_NODISCARD,
      CI_ATTRIBUTE_STANDARD_KIND_NORETURN,
      CI_ATTRIBUTE_STANDARD_KIND_UNSEQUENCED,
      CI_ATTRIBUTE_STANDARD_KIND_REPRODUCIBLE
  };

CONSTRUCTOR(struct CurrentGenericParams,
            CurrentGenericParams,
            CIGenericParams *called,
            CIGenericParams *decl)
{
    return (struct CurrentGenericParams){ .called = called, .decl = decl };
}

struct CurrentGenericParams
default__CurrentGenericParams()
{
    return (struct CurrentGenericParams){ .called = NULL, .decl = NULL };
}

CONSTRUCTOR(struct CITypecheckContext,
            CITypecheckContext,
            const CIDecl *current_decl,
            CIGenericParams *called_generic_params,
            CIGenericParams *decl_generic_params)
{
    return (struct CITypecheckContext){
        .current_decl = current_decl,
        .current_generic_params =
          NEW(CurrentGenericParams, called_generic_params, decl_generic_params),
        .current_scope_id = NULL,
        .current_switch = { .is_integer = false,
                            .is_float = false,
                            .is_present = false }
    };
}

struct CurrentGenericParams
set_current_generic_params__CITypecheckContext(
  struct CITypecheckContext *self,
  struct CurrentGenericParams new_current_generic_params)
{
    struct CurrentGenericParams old_current_generic_params =
      self->current_generic_params;

    self->current_generic_params = new_current_generic_params;

    return old_current_generic_params;
}

#define INIT_X_TO_WAIT_TO_VISIT(hm)                      \
    {                                                    \
        CIParserVisitWaitingListItem *item =             \
          NEW(CIParserVisitWaitingListItem, name, NULL); \
                                                         \
        if (insert__HashMap(hm, name->buffer, item)) {   \
            FREE(CIParserVisitWaitingListItem, item);    \
        }                                                \
    }

void
init_function_to_visit_waiting_list__CIParser(CIParser *self, String *name)
{
    INIT_X_TO_WAIT_TO_VISIT(self->visit_waiting_list.functions);
}

void
init_typedef_to_visit_waiting_list__CIParser(CIParser *self, String *name)
{
    INIT_X_TO_WAIT_TO_VISIT(self->visit_waiting_list.typedefs);
}

void
init_union_to_visit_waiting_list__CIParser(CIParser *self, String *name)
{
    INIT_X_TO_WAIT_TO_VISIT(self->visit_waiting_list.unions);
}

void
init_struct_to_visit_waiting_list__CIParser(CIParser *self, String *name)
{
    INIT_X_TO_WAIT_TO_VISIT(self->visit_waiting_list.structs);
}

#define ADD_X_TO_VISIT_WAITING_LIST(hm)                                      \
    CIParserVisitWaitingListItem *item = get__HashMap(hm, name->buffer);     \
                                                                             \
    /* NOTE: Normally, it should already have been created, before the first \
    signature can be sent to the visit queue. */                             \
    ASSERT(item);                                                            \
                                                                             \
    if (item->generic_params_list) {                                         \
        for (Usize i = 0; i < item->generic_params_list->len; ++i) {         \
            if (eq__CIGenericParams(get__Vec(item->generic_params_list, i),  \
                                    generic_params)) {                       \
                return;                                                      \
            }                                                                \
        }                                                                    \
    } else {                                                                 \
        item->generic_params_list = NEW(Vec);                                \
    }                                                                        \
                                                                             \
    push__Vec(item->generic_params_list, generic_params);

void
add_function_to_visit_waiting_list__CIParser(CIParser *self,
                                             String *name,
                                             CIGenericParams *generic_params)
{
    ADD_X_TO_VISIT_WAITING_LIST(self->visit_waiting_list.functions);
}

void
add_typedef_to_visit_waiting_list__CIParser(CIParser *self,
                                            String *name,
                                            CIGenericParams *generic_params)
{
    ADD_X_TO_VISIT_WAITING_LIST(self->visit_waiting_list.typedefs);
}

void
add_union_to_visit_waiting_list__CIParser(CIParser *self,
                                          String *name,
                                          CIGenericParams *generic_params)
{
    ADD_X_TO_VISIT_WAITING_LIST(self->visit_waiting_list.unions);
}

void
add_struct_to_visit_waiting_list__CIParser(CIParser *self,
                                           String *name,
                                           CIGenericParams *generic_params)
{
    ADD_X_TO_VISIT_WAITING_LIST(self->visit_waiting_list.structs);
}

void
add_item_to_visit_waiting_list__CIParser(CIParser *self,
                                         enum CIDeclKind kind,
                                         String *name,
                                         CIGenericParams *generic_params)
{
    switch (kind) {
        case CI_DECL_KIND_FUNCTION:
            add_function_to_visit_waiting_list__CIParser(
              self, name, generic_params);

            break;
        case CI_DECL_KIND_TYPEDEF:
            add_typedef_to_visit_waiting_list__CIParser(
              self, name, generic_params);

            break;
        case CI_DECL_KIND_UNION:
            add_union_to_visit_waiting_list__CIParser(
              self, name, generic_params);

            break;
        case CI_DECL_KIND_STRUCT:
            add_struct_to_visit_waiting_list__CIParser(
              self, name, generic_params);

            break;
        default:
            UNREACHABLE("this kind of variant is not expected to add to visit "
                        "waiting lists");
    }
}

CIParserVisitWaitingListItem *
get_function_from_visit_waiting_list__CIParser(const CIParser *self,
                                               const String *name)
{
    return get__HashMap(self->visit_waiting_list.functions, name->buffer);
}

CIParserVisitWaitingListItem *
get_typedef_from_visit_waiting_list__CIParser(const CIParser *self,
                                              const String *name)
{
    return get__HashMap(self->visit_waiting_list.typedefs, name->buffer);
}

CIParserVisitWaitingListItem *
get_union_from_visit_waiting_list__CIParser(const CIParser *self,
                                            const String *name)
{
    return get__HashMap(self->visit_waiting_list.unions, name->buffer);
}

CIParserVisitWaitingListItem *
get_struct_from_visit_waiting_list__CIParser(const CIParser *self,
                                             const String *name)
{
    return get__HashMap(self->visit_waiting_list.structs, name->buffer);
}

bool
is_recursive_function__CIParser(const CIParser *self, const String *name)
{
    ASSERT(!search_function__CIParser(self, name, NULL));

    return get_function_from_visit_waiting_list__CIParser(self, name);
}

bool
is_recursive_typedef__CIParser(const CIParser *self, const String *name)
{
    // NOTE: A typedef cannot be recursive.
    return false;
}

bool
is_recursive_union__CIParser(const CIParser *self, const String *name)
{
    ASSERT(!search_union__CIResultFile(self->file, name));

    return get_union_from_visit_waiting_list__CIParser(self, name);
}

bool
is_recursive_struct__CIParser(const CIParser *self, const String *name)
{
    ASSERT(!search_struct__CIResultFile(self->file, name));

    return get_struct_from_visit_waiting_list__CIParser(self, name);
}

CONSTRUCTOR(CIParserVisitWaitingListItem *,
            CIParserVisitWaitingListItem,
            String *name,
            Vec *generic_params_list)
{
    CIParserVisitWaitingListItem *self =
      lily_malloc(sizeof(CIParserVisitWaitingListItem));

    self->name = name;
    self->generic_params_list = generic_params_list;

    return self;
}

DESTRUCTOR(CIParserVisitWaitingListItem, CIParserVisitWaitingListItem *self)
{
    if (self->generic_params_list) {
        FREE(Vec, self->generic_params_list);
    }

    lily_free(self);
}

DESTRUCTOR(CIParserVisitWaitingList, const CIParserVisitWaitingList *self)
{
    FREE_HASHMAP_VALUES(self->functions, CIParserVisitWaitingListItem);
    FREE(HashMap, self->functions);
    FREE_HASHMAP_VALUES(self->typedefs, CIParserVisitWaitingListItem);
    FREE(HashMap, self->typedefs);
    FREE_HASHMAP_VALUES(self->unions, CIParserVisitWaitingListItem);
    FREE(HashMap, self->unions);
    FREE_HASHMAP_VALUES(self->structs, CIParserVisitWaitingListItem);
    FREE(HashMap, self->structs);
}

CONSTRUCTOR(CIParser, CIParser, CIResultFile *file)
{
    if (!names_error) {
        names_error = NEW(Vec);
    }

    if (!tokens_feature) {
        tokens_feature = get_tokens_feature__CIScanner();
    }

    CIParserSpan current_span = default__CIParserSpan();

    return (CIParser){ .file = file,
                       .count_error = &file->file_analysis->count_error,
                       .count_warning = &file->file_analysis->count_warning,
                       .resolved_tokens = NULL,
                       .resolved_tokens_count = 0,
                       .current_token = NULL,
                       .previous_token = NULL,
                       .current_span = current_span,
                       .previous_span = current_span,
                       .visit_waiting_list = NEW(CIParserVisitWaitingList) };
}

Rc *
generate_name_error__CIParser()
{
    ASSERT(names_error);

    String *name_error = format__String("%__error__{zu}", name_error_count++);
    Rc *rc_name_error = NEW(Rc, name_error);

    push__Vec(names_error, rc_name_error);

    return rc_name_error;
}

bool
is_integer_data_type__CIParser(CIParser *self,
                               CIDataType *data_type,
                               bool allow_implicit_cast,
                               const CIGenericParams *called_generic_params,
                               const CIGenericParams *decl_generic_params)
{
    switch (data_type->kind) {
        case CI_DATA_TYPE_KIND_TYPEDEF: {
            CIDataType *resolved_dt = resolve_typedef_data_type__CIParser(
              self, data_type, called_generic_params, decl_generic_params);
            bool res = is_integer__CIDataType(resolved_dt);

            FREE(CIDataType, resolved_dt);

            return res;
        }
        case CI_DATA_TYPE_KIND_ARRAY:
        case CI_DATA_TYPE_KIND_PTR: // via implicit cast
            return allow_implicit_cast;
        default:
            return is_integer__CIDataType(data_type);
    }
}

bool
is_float_data_type__CIParser(CIParser *self,
                             CIDataType *data_type,
                             const CIGenericParams *called_generic_params,
                             const CIGenericParams *decl_generic_params)
{
    switch (data_type->kind) {
        case CI_DATA_TYPE_KIND_TYPEDEF: {
            CIDataType *resolved_dt = resolve_typedef_data_type__CIParser(
              self, data_type, called_generic_params, decl_generic_params);
            bool res = is_float__CIDataType(resolved_dt);

            FREE(CIDataType, resolved_dt);

            return res;
        }
        default:
            return is_float__CIDataType(data_type);
    }
}

bool
is_ptr_data_type__CIParser(CIParser *self,
                           CIDataType *data_type,
                           const CIGenericParams *called_generic_params,
                           const CIGenericParams *decl_generic_params)
{
    switch (data_type->kind) {
        case CI_DATA_TYPE_KIND_TYPEDEF: {
            CIDataType *resolved_dt = resolve_typedef_data_type__CIParser(
              self, data_type, called_generic_params, decl_generic_params);
            bool res = is_ptr_data_type__CIParser(
              self, resolved_dt, called_generic_params, decl_generic_params);

            FREE(CIDataType, resolved_dt);

            return res;
        }
        case CI_DATA_TYPE_KIND_PTR:
        case CI_DATA_TYPE_KIND_ARRAY:
            return true;
        default:
            return false;
    }
}

bool
is_void_ptr_data_type__CIParser(CIParser *self,
                                const CIDataType *data_type,
                                const CIGenericParams *called_generic_params,
                                const CIGenericParams *decl_generic_params)
{
    CIDataType *current_dt = (CIDataType *)data_type;

    while (is_ptr_data_type__CIParser(
      self, current_dt, called_generic_params, decl_generic_params)) {
        current_dt = unwrap_implicit_ptr_data_type__CIParser(
          self, current_dt, called_generic_params, decl_generic_params);
    }

    return current_dt->kind == CI_DATA_TYPE_KIND_VOID;
}

bool
is_struct_or_union_data_type__CIParser(
  CIParser *self,
  CIDataType *data_type,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params)
{
    switch (data_type->kind) {
        case CI_DATA_TYPE_KIND_STRUCT:
        case CI_DATA_TYPE_KIND_UNION:
            return true;
        case CI_DATA_TYPE_KIND_TYPEDEF: {
            CIDataType *resolved_dt = resolve_typedef_data_type__CIParser(
              self, data_type, called_generic_params, decl_generic_params);
            bool res = is_struct_or_union_data_type__CIParser(
              self, resolved_dt, called_generic_params, decl_generic_params);

            FREE(CIDataType, resolved_dt);

            return res;
        }
        default:
            FAILED("expected struct or union");
    }
}

Usize
count_compatible_pointer_depth__CIParser(
  CIParser *self,
  const CIDataType *data_type,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params)
{
    CIDataType *current_dt = (CIDataType *)data_type;
    Usize depth_count = 0;

    while (current_dt &&
           is_ptr_data_type__CIParser(
             self, current_dt, called_generic_params, decl_generic_params)) {
        ++depth_count;
        current_dt = unwrap_implicit_ptr_data_type__CIParser(
          self, current_dt, called_generic_params, decl_generic_params);
    }

    return depth_count;
}

bool
is_compatible_with_void_ptr_data_type__CIParser(
  CIParser *self,
  const CIDataType *left,
  const CIDataType *right,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params)
{
    if (is_void_ptr_data_type__CIParser(
          self, left, called_generic_params, decl_generic_params) ||
        is_void_ptr_data_type__CIParser(
          self, right, called_generic_params, decl_generic_params)) {
        return count_compatible_pointer_depth__CIParser(
                 self, left, called_generic_params, decl_generic_params) ==
               count_compatible_pointer_depth__CIParser(
                 self, right, called_generic_params, decl_generic_params);
    }

    return false;
}

CIDataType *
unwrap_implicit_ptr_data_type__CIParser(
  CIParser *self,
  CIDataType *data_type,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params)
{
    ASSERT(is_ptr_data_type__CIParser(
      self, data_type, called_generic_params, decl_generic_params));

    switch (data_type->kind) {
        case CI_DATA_TYPE_KIND_TYPEDEF: {
            CIDecl *decl = search_typedef__CIParser(
              self,
              GET_PTR_RC(String, data_type->typedef_.name),
              data_type->typedef_.generic_params);
            CIDataType *typedef_data_type =
              (CIDataType *)get_typedef_data_type__CIDecl(decl);

            ASSERT(is_ptr_data_type__CIParser(self,
                                              typedef_data_type,
                                              called_generic_params,
                                              decl_generic_params));

            return unwrap_implicit_ptr_data_type__CIParser(
              self,
              typedef_data_type,
              called_generic_params,
              decl_generic_params);
        }
        case CI_DATA_TYPE_KIND_PTR:
            return data_type->ptr;
        case CI_DATA_TYPE_KIND_ARRAY:
            return data_type->array.data_type;
        default:
            UNREACHABLE("this kind of data type is not expected, expected "
                        "pointer compatible data type");
    }
}

#define RESOLVE_VISIT_WAITING_LIST(hm, gen)                                    \
    HashMapIter iter = NEW(HashMapIter, hm);                                   \
    CIParserVisitWaitingListItem *item = NULL;                                 \
                                                                               \
    while ((item = next__HashMapIter(&iter))) {                                \
        if (item->generic_params_list) {                                       \
            for (Usize i = 0; i < item->generic_params_list->len; ++i) {       \
                gen(self, item->name, get__Vec(item->generic_params_list, i)); \
            }                                                                  \
        }                                                                      \
    }

void
resolve_function_visit_waiting_list__CIParser(CIParser *self)
{
    RESOLVE_VISIT_WAITING_LIST(self->visit_waiting_list.functions,
                               generate_function_gen__CIParser);
}

void
resolve_typedef_visit_waiting_list__CIParser(CIParser *self)
{
    RESOLVE_VISIT_WAITING_LIST(self->visit_waiting_list.typedefs,
                               generate_typedef_gen__CIParser);
}

void
resolve_union_visit_waiting_list__CIParser(CIParser *self)
{
    RESOLVE_VISIT_WAITING_LIST(self->visit_waiting_list.unions,
                               generate_union_gen__CIParser);
}

void
resolve_struct_visit_waiting_list__CIParser(CIParser *self)
{
    RESOLVE_VISIT_WAITING_LIST(self->visit_waiting_list.structs,
                               generate_struct_gen__CIParser);
}

void
resolve_visit_waiting_list__CIParser(CIParser *self)
{
    ENABLE_RESOLVE_VISIT_WAITING_LIST();

    resolve_function_visit_waiting_list__CIParser(self);
    resolve_typedef_visit_waiting_list__CIParser(self);
    resolve_union_visit_waiting_list__CIParser(self);
    resolve_struct_visit_waiting_list__CIParser(self);

    DISABLE_RESOLVE_VISIT_WAITING_LIST();
}

void
set_current_token__CIParser(CIParser *self, CIToken *next_token)
{
    self->previous_token =
      self->current_token ? self->current_token : next_token;
    self->current_token = next_token;

    if (is_same_filename__CIResultFile(
          self->file, self->current_token->location.filename)) {
        self->previous_span = self->current_span;
        self->current_span = from_token__CIParserSpan(self->current_token);
    }
}

void
init__CIParser(CIParser *self, const CIResolvedTokens *resolved_tokens)
{
    ASSERT(resolved_tokens);
    ASSERT(count__CIResolvedTokens(resolved_tokens) > 0);

    CIToken *first_token = get__CIResolvedTokens(resolved_tokens, 0);

    check_standard__CIParser(self, first_token);

    self->resolved_tokens = resolved_tokens;
    self->current_token = first_token;
    self->previous_token = self->current_token;
    self->current_span = from_token__CIParserSpan(self->current_token);
    self->previous_span = self->current_span;
    ++self->resolved_tokens_count;
}

void
set_current_scope__CIParser(CIParser *self)
{
    current_scope = self->file->scope_base;
}

void
check_standard__CIParser(CIParser *self, CIToken *token)
{
    ASSERT(token->kind < CI_TOKEN_KIND_MAX);

    Location location_error = clone__Location(&token->location);
    const CIFeature *feature = &tokens_feature[token->kind];

    CI_CHECK_STANDARD_SINCE(
      self->file->entity.result->config->standard, feature->since, {
          enum CIErrorKind error_kind;

          switch (feature->since) {
              case CI_STANDARD_NONE:
                  return;
              case CI_STANDARD_KR:
                  UNREACHABLE("since: no error with K&R standard");
              case CI_STANDARD_89:
                  error_kind = CI_ERROR_KIND_REQUIRED_C89_OR_LATER;

                  break;
              case CI_STANDARD_95:
                  error_kind = CI_ERROR_KIND_REQUIRED_C95_OR_LATER;

                  break;
              case CI_STANDARD_99:
                  error_kind = CI_ERROR_KIND_REQUIRED_C99_OR_LATER;

                  break;
              case CI_STANDARD_11:
                  error_kind = CI_ERROR_KIND_REQUIRED_C11_OR_LATER;

                  break;
              case CI_STANDARD_17:
                  error_kind = CI_ERROR_KIND_REQUIRED_C17_OR_LATER;

                  break;
              case CI_STANDARD_23:
                  error_kind = CI_ERROR_KIND_REQUIRED_C23_OR_LATER;

                  break;
              default:
                  UNREACHABLE("unknown standard");
          }

          emit__Diagnostic(NEW_VARIANT(Diagnostic,
                                       simple_ci_error,
                                       &self->file->file_input,
                                       &location_error,
                                       NEW(CIError, error_kind),
                                       NULL,
                                       NULL,
                                       NULL),
                           self->count_error);
      });

    CI_CHECK_STANDARD_UNTIL(
      self->file->entity.result->config->standard, feature->until, {
          String *note = NULL;

          switch (feature->until) {
              case CI_STANDARD_NONE:
                  return;
              case CI_STANDARD_KR:
                  UNREACHABLE("since: no error with K&R standard");
              case CI_STANDARD_89:
                  note =
                    from__String("this feature is no longer available in C89");

                  break;
              case CI_STANDARD_95:
                  note =
                    from__String("this feature is no longer available in C95");

                  break;
              case CI_STANDARD_99:
                  note =
                    from__String("this feature is no longer available in C99");

                  break;
              case CI_STANDARD_11:
                  note =
                    from__String("this feature is no longer available in C11");

                  break;
              case CI_STANDARD_17:
                  note =
                    from__String("this feature is no longer available in C17");

                  break;
              case CI_STANDARD_23:
                  note =
                    from__String("this feature is no longer available in C23");

                  break;
              default:
                  UNREACHABLE("unknown standard");
          }

          emit_note__Diagnostic(NEW_VARIANT(Diagnostic,
                                            simple_ci_note,
                                            &self->file->file_input,
                                            &location_error,
                                            note,
                                            NULL,
                                            NULL,
                                            NULL));
      });
}

void
next_token__CIParser(CIParser *self)
{
    if (self->resolved_tokens_count <
        count__CIResolvedTokens(self->resolved_tokens)) {
        CIToken *next_token = get__CIResolvedTokens(
          self->resolved_tokens, self->resolved_tokens_count++);

        ASSERT(next_token);

        check_standard__CIParser(self, next_token);
        set_current_token__CIParser(self, next_token);
    }
}

CIToken *
peek_token__CIParser(CIParser *self, Usize n)
{
    CIToken *peeked = self->current_token;
    Usize resolved_tokens_count = self->resolved_tokens_count;
    Usize resolved_tokens_total_count =
      count__CIResolvedTokens(self->resolved_tokens);

    while (resolved_tokens_count < resolved_tokens_total_count && n--) {
        peeked =
          get__CIResolvedTokens(self->resolved_tokens, resolved_tokens_count++);
    }

    return peeked;
}

bool
expect__CIParser(CIParser *self, enum CITokenKind kind, bool emit_error)
{
    if (self->current_token->kind == kind) {
        next_token__CIParser(self);

        return true;
    }

    if (emit_error) {
        FAILED("expected: ...");
    }

    return false;
}

bool
expect_many__CIParser(CIParser *self, Usize n, ...)
{
    ASSERT(n > 0);

    va_list vl;

    va_start(vl, n);

    while (n--) {
        if (!expect__CIParser(self, va_arg(vl, enum CITokenKind), true)) {
            return false;
        }
    }

    va_end(vl);

    return true;
}

void
expect_with_list__CIParser(CIParser *self, Usize n, ...)
{
    ASSERT(n > 0);

    va_list vl;

    va_start(vl, n);

    while (n--) {
        if (expect__CIParser(self, va_arg(vl, enum CITokenKind), false)) {
            return;
        }
    }

    va_end(vl);

    FAILED("expected ...");
}

void
jump__CIParser(CIParser *self, Usize n)
{
    while (n--) {
        next_token__CIParser(self);
    }
}

bool
token_is_data_type__CIParser(CIParser *self, const CIToken *token)
{
    switch (token->kind) {
        case CI_TOKEN_KIND_IDENTIFIER:
            return search_typedef__CIResultFile(
                     self->file, GET_PTR_RC(String, token->identifier)) ||
                   is__CIBuiltinType(GET_PTR_RC(String, token->identifier));
        case CI_TOKEN_KIND_AT: // TODO: check if the next token is an identifier
                               // (not needed for the moment)
        case CI_TOKEN_KIND_KEYWORD_BOOL:
        case CI_TOKEN_KIND_KEYWORD_CHAR:
        case CI_TOKEN_KIND_KEYWORD_DOUBLE:
        case CI_TOKEN_KIND_KEYWORD_ENUM:
        case CI_TOKEN_KIND_KEYWORD_FLOAT:
        case CI_TOKEN_KIND_KEYWORD_INT:
        case CI_TOKEN_KIND_KEYWORD_LONG:
        case CI_TOKEN_KIND_KEYWORD_SHORT:
        case CI_TOKEN_KIND_KEYWORD_SIGNED:
        case CI_TOKEN_KIND_KEYWORD_STRUCT:
        case CI_TOKEN_KIND_KEYWORD_TYPEOF:
        case CI_TOKEN_KIND_KEYWORD_UNION:
        case CI_TOKEN_KIND_KEYWORD_UNSIGNED:
        case CI_TOKEN_KIND_KEYWORD_VOID:
        case CI_TOKEN_KIND_KEYWORD__BOOL:
        case CI_TOKEN_KIND_KEYWORD__COMPLEX:
        case CI_TOKEN_KIND_KEYWORD__DECIMAL128:
        case CI_TOKEN_KIND_KEYWORD__DECIMAL32:
        case CI_TOKEN_KIND_KEYWORD__DECIMAL64:
        case CI_TOKEN_KIND_KEYWORD__IMAGINARY:
            return true;
        default:
            return token_is_data_type_qualifier__CIParser(self, token);
    }
}

bool
is_data_type__CIParser(CIParser *self)
{
    return token_is_data_type__CIParser(self, self->current_token);
}

bool
token_is_storage_class__CIParser(CIParser *self, const CIToken *token)
{
    switch (token->kind) {
        case CI_TOKEN_KIND_KEYWORD_AUTO:
        case CI_TOKEN_KIND_KEYWORD_CONSTEXPR:
        case CI_TOKEN_KIND_KEYWORD_EXTERN:
        case CI_TOKEN_KIND_KEYWORD_INLINE:
        case CI_TOKEN_KIND_KEYWORD_REGISTER:
        case CI_TOKEN_KIND_KEYWORD_STATIC:
        case CI_TOKEN_KIND_KEYWORD_THREAD_LOCAL:
        case CI_TOKEN_KIND_KEYWORD__THREAD_LOCAL:
        case CI_TOKEN_KIND_KEYWORD_TYPEDEF:
            return true;
        default:
            return false;
    }
}

bool
is_storage_class__CIParser(CIParser *self)
{
    return token_is_storage_class__CIParser(self, self->current_token);
}

bool
token_is_data_type_qualifier__CIParser(CIParser *self, const CIToken *token)
{
    switch (token->kind) {
        case CI_TOKEN_KIND_KEYWORD_CONST:
        case CI_TOKEN_KIND_KEYWORD_VOLATILE:
        case CI_TOKEN_KIND_KEYWORD_RESTRICT:
        case CI_TOKEN_KIND_KEYWORD__ATOMIC:
        case CI_TOKEN_KIND_KEYWORD__NORETURN:
        case CI_TOKEN_KIND_KEYWORD___RESTRICT:
        case CI_TOKEN_KIND_KEYWORD___RESTRICT__:
            return true;
        default:
            return false;
    }
}

bool
is_data_type_qualifier__CIParser(CIParser *self)
{
    return token_is_data_type_qualifier__CIParser(self, self->current_token);
}

CIDataType *
substitute_generic__CIParser(const String *generic_name,
                             CIGenericParams *generic_params,
                             CIGenericParams *called_generic_params)
{
    Usize id = 0;

    for (Usize i = 0; i < generic_params->params->len; ++i) {
        CIDataType *generic_param = get__Vec(generic_params->params, i);

        switch (generic_param->kind) {
            case CI_DATA_TYPE_KIND_GENERIC:
                if (!strcmp(GET_PTR_RC(String, generic_param->generic)->buffer,
                            generic_name->buffer)) {
                    id = i;
                    goto exit_loop;
                }

                break;
            default:
                FAILED("expected only generic data type for the moment");
        }
    }

    FAILED("generic param name not found");

    return NULL;

exit_loop:
    return get__Vec(called_generic_params->params, id);
}

CIGenericParams *
substitute_generic_params__CIParser(
  CIGenericParams *unresolved_generic_params,
  const CIGenericParams *generic_params,
  const CIGenericParams *called_generic_params)
{
    if (unresolved_generic_params) {
        if (generic_params && called_generic_params &&
            has_generic__CIGenericParams(unresolved_generic_params)) {
            Vec *subs_params = NEW(Vec); // Vec<CIDataType*>*

            for (Usize i = 0; i < unresolved_generic_params->params->len; ++i) {
                CIDataType *subs_param = substitute_data_type__CIParser(
                  get__Vec(unresolved_generic_params->params, i),
                  (CIGenericParams *)generic_params,
                  (CIGenericParams *)called_generic_params);

                if (subs_param) {
                    push__Vec(subs_params, subs_param);
                }
            }

            return NEW(CIGenericParams, subs_params);
        }

        return ref__CIGenericParams(unresolved_generic_params);
    }

    return NULL;
}

CIDataType *
substitute_data_type__CIParser(CIDataType *data_type,
                               CIGenericParams *generic_params,
                               CIGenericParams *called_generic_params)
{
    CIDataType *res = NULL;

    switch (data_type->kind) {
        case CI_DATA_TYPE_KIND_ARRAY: {
            CIDataType *subs = substitute_data_type__CIParser(
              data_type, generic_params, called_generic_params);

            if (subs) {
                switch (data_type->array.kind) {
                    case CI_DATA_TYPE_ARRAY_KIND_SIZED:
                        res = NEW_VARIANT(CIDataType,
                                          array,
                                          NEW_VARIANT(CIDataTypeArray,
                                                      sized,
                                                      subs,
                                                      data_type->array.name,
                                                      data_type->array.size));

                        break;
                    case CI_DATA_TYPE_ARRAY_KIND_NONE:
                        res = NEW_VARIANT(CIDataType,
                                          array,
                                          NEW_VARIANT(CIDataTypeArray,
                                                      none,
                                                      subs,
                                                      data_type->array.name));

                        break;
                    default:
                        UNREACHABLE("unknown variant");
                }
            } else {
                return ref__CIDataType(data_type);
            }

            break;
        }
        case CI_DATA_TYPE_KIND_FUNCTION: {
            Vec *subs_params = NEW(Vec);

            for (Usize i = 0; i < data_type->function.params->len; ++i) {
                CIDeclFunctionParam *param =
                  get__Vec(data_type->function.params, i);

                switch (param->kind) {
                    case CI_DECL_FUNCTION_PARAM_KIND_NORMAL: {
                        CIDataType *subs_data_type =
                          substitute_data_type__CIParser(param->data_type,
                                                         generic_params,
                                                         called_generic_params);

                        if (subs_data_type) {
                            push__Vec(subs_params,
                                      NEW_VARIANT(CIDeclFunctionParam,
                                                  normal,
                                                  param->name,
                                                  subs_data_type));
                        }

                        break;
                    }
                    case CI_DECL_FUNCTION_PARAM_KIND_VARIADIC:
                        push__Vec(subs_params,
                                  NEW_VARIANT(CIDeclFunctionParam, variadic));

                        break;
                    default:
                        UNREACHABLE("unknown variant");
                }
            }

            CIDataType *subs_return_data_type = substitute_data_type__CIParser(
              data_type->function.return_data_type,
              generic_params,
              called_generic_params);

            if (subs_return_data_type) {
                return ref__CIDataType(data_type);
            } else {
                FREE_BUFFER_ITEMS(
                  subs_params->buffer, subs_params->len, CIDataType);
                FREE(Vec, subs_params);
            }

            res = NEW_VARIANT(
              CIDataType,
              function,
              NEW(CIDataTypeFunction,
                  data_type->function.name,
                  subs_params,
                  subs_return_data_type,
                  clone__CIDataType(data_type->function.function_data_type)));

            break;
        }
        case CI_DATA_TYPE_KIND_GENERIC: {
            CIDataType *res_generic = substitute_generic__CIParser(
              GET_PTR_RC(String, data_type->generic),
              generic_params,
              called_generic_params);

            if (res_generic) {
                return ref__CIDataType(res_generic);
            }

            return NULL;
        }
        case CI_DATA_TYPE_KIND_PTR: {
            if (data_type->ptr) {
                CIDataType *subs = substitute_data_type__CIParser(
                  data_type->ptr, generic_params, called_generic_params);

                if (subs) {
                    res = NEW_VARIANT(CIDataType, ptr, subs);
                } else {
                    return ref__CIDataType(data_type);
                }
            } else {
                return NULL;
            }

            break;
        }
        case CI_DATA_TYPE_KIND_STRUCT: {
#define SUBSTITUTE_STRUCT_OR_UNION(decl_name, decl_ty, variant)                \
    if (data_type->decl_name.generic_params || data_type->decl_name.fields) {  \
        Vec *subs_fields = NULL;                                               \
                                                                               \
        if (data_type->decl_name.fields) {                                     \
            subs_fields = NEW(Vec);                                            \
                                                                               \
            for (Usize i = 0; i < data_type->decl_name.fields->len; ++i) {     \
                CIDeclStructField *field =                                     \
                  get__Vec(data_type->decl_name.fields, i);                    \
                CIDataType *subs_field_dt = substitute_data_type__CIParser(    \
                  field->data_type, generic_params, called_generic_params);    \
                                                                               \
                push__Vec(subs_fields,                                         \
                          NEW(CIDeclStructField, field->name, subs_field_dt)); \
            }                                                                  \
        }                                                                      \
                                                                               \
        res = NEW_VARIANT(CIDataType,                                          \
                          variant,                                             \
                          NEW(decl_ty,                                         \
                              data_type->decl_name.name,                       \
                              substitute_generic_params__CIParser(             \
                                data_type->decl_name.generic_params,           \
                                generic_params,                                \
                                called_generic_params),                        \
                              subs_fields));                                   \
                                                                               \
        break;                                                                 \
    }                                                                          \
                                                                               \
    return ref__CIDataType(data_type);

            SUBSTITUTE_STRUCT_OR_UNION(struct_, CIDataTypeStruct, struct);
        }
        case CI_DATA_TYPE_KIND_TYPEDEF: {
            if (data_type->typedef_.generic_params) {
                res = NEW_VARIANT(CIDataType,
                                  typedef,
                                  NEW(CIDataTypeTypedef,
                                      data_type->typedef_.name,
                                      substitute_generic_params__CIParser(
                                        data_type->typedef_.generic_params,
                                        generic_params,
                                        called_generic_params)));
            } else {
                return ref__CIDataType(data_type);
            }

            break;
        }
        case CI_DATA_TYPE_KIND_UNION: {
            SUBSTITUTE_STRUCT_OR_UNION(union_, CIDataTypeUnion, union);
        }
        default:
            return ref__CIDataType(data_type);
    }

    set_context__CIDataType(res, data_type->ctx);
    set_qualifier__CIDataType(res, data_type->qualifier);

    return res;
}

CIDataType *
substitute_and_generate_from_data_type__CIParser(
  CIParser *self,
  CIDataType *data_type,
  CIGenericParams *generic_params,
  CIGenericParams *called_generic_params)
{
    CIDataType *subs_data_type = substitute_data_type__CIParser(
      data_type, generic_params, called_generic_params);

    switch (subs_data_type->kind) {
        case CI_DATA_TYPE_KIND_STRUCT:
            if (subs_data_type->struct_.name) {
                generate_struct_gen__CIParser(
                  self,
                  GET_PTR_RC(String, subs_data_type->struct_.name),
                  subs_data_type->struct_.generic_params);
            }

            break;
        case CI_DATA_TYPE_KIND_UNION:
            if (subs_data_type->union_.name) {
                generate_union_gen__CIParser(
                  self,
                  GET_PTR_RC(String, subs_data_type->union_.name),
                  subs_data_type->union_.generic_params);
            }

            break;
        case CI_DATA_TYPE_KIND_TYPEDEF:
            generate_typedef_gen__CIParser(
              self,
              GET_PTR_RC(String, subs_data_type->typedef_.name),
              subs_data_type->typedef_.generic_params);

            break;
        default:
            break;
    }

    return subs_data_type;
}

Vec *
visit_struct_or_union__CIParser(CIParser *self,
                                const CIDecl *decl,
                                CIGenericParams *called_generic_params)
{
    if (has_generic__CIDecl(decl)) {
        const Vec *fields = get_fields__CIDecl(decl);

        if (!fields) {
            return NULL;
        }

        CIGenericParams *generic_params = get_generic_params__CIDecl(decl);
        Vec *gen_fields = NEW(Vec);

        for (Usize i = 0; i < fields->len; ++i) {
            const CIDeclStructField *field = get__Vec(fields, i);
            CIDataType *subs_data_type =
              substitute_and_generate_from_data_type__CIParser(
                self, field->data_type, generic_params, called_generic_params);

            if (subs_data_type) {
                push__Vec(gen_fields,
                          NEW(CIDeclStructField, field->name, subs_data_type));
            }
        }

        return gen_fields;
    }

    return NULL;
}

CIDataType *
visit_typedef__CIParser(CIParser *self,
                        const CIDecl *decl,
                        CIGenericParams *called_generic_params)
{
    ASSERT(decl->kind == CI_DECL_KIND_TYPEDEF);

    return substitute_and_generate_from_data_type__CIParser(
      self,
      decl->typedef_.data_type,
      decl->typedef_.generic_params,
      called_generic_params);
}

void
visit_function_data_type__CIParser(CIParser *self,
                                   const CIDataTypeFunction *function,
                                   CIGenericParams *called_generic_params,
                                   CIGenericParams *decl_generic_params)
{
    visit_data_type__CIParser(self,
                              function->return_data_type,
                              called_generic_params,
                              decl_generic_params);

    if (function->params) {
        for (Usize i = 0; i < function->params->len; ++i) {
            CIDeclFunctionParam *param = get__Vec(function->params, i);

            visit_data_type__CIParser(self,
                                      param->data_type,
                                      called_generic_params,
                                      decl_generic_params);
        }
    }

    if (function->function_data_type) {
        visit_data_type__CIParser(self,
                                  function->function_data_type,
                                  called_generic_params,
                                  decl_generic_params);
    }
}

void
visit_data_type__CIParser(CIParser *self,
                          const CIDataType *data_type,
                          CIGenericParams *called_generic_params,
                          CIGenericParams *decl_generic_params)
{
    switch (data_type->kind) {
        case CI_DATA_TYPE_KIND_ARRAY:
            visit_data_type__CIParser(self,
                                      data_type->array.data_type,
                                      called_generic_params,
                                      decl_generic_params);

            break;
        case CI_DATA_TYPE_KIND_FUNCTION:
            visit_function_data_type__CIParser(self,
                                               &data_type->function,
                                               called_generic_params,
                                               decl_generic_params);

            break;
        case CI_DATA_TYPE_KIND_PTR:
            if (data_type->ptr) {
                visit_data_type__CIParser(self,
                                          data_type->ptr,
                                          called_generic_params,
                                          decl_generic_params);
            }

            break;
        case CI_DATA_TYPE_KIND_STRUCT:
            if (data_type->struct_.name) {
                generate_struct_gen__CIParser(
                  self,
                  GET_PTR_RC(String, data_type->struct_.name),
                  data_type->struct_.generic_params);
            }

            break;
        case CI_DATA_TYPE_KIND_TYPEDEF:
            generate_typedef_gen__CIParser(
              self,
              GET_PTR_RC(String, data_type->typedef_.name),
              called_generic_params);

            break;
        case CI_DATA_TYPE_KIND_UNION:
            if (data_type->union_.name) {
                generate_union_gen__CIParser(
                  self,
                  GET_PTR_RC(String, data_type->union_.name),
                  data_type->union_.generic_params);
            }

            break;
        default:
            break;
    }
}

void
visit_function_decl_variable__CIParser(CIParser *self,
                                       const CIDeclVariable *variable,
                                       CIGenericParams *called_generic_params,
                                       CIGenericParams *decl_generic_params)
{
    visit_data_type__CIParser(
      self, variable->data_type, called_generic_params, decl_generic_params);
    visit_function_expr__CIParser(
      self, variable->expr, called_generic_params, decl_generic_params);
}

void
visit_function_decl__CIParser(CIParser *self,
                              const CIDecl *decl,
                              CIGenericParams *called_generic_params,
                              CIGenericParams *decl_generic_params)
{
    switch (decl->kind) {
        case CI_DECL_KIND_VARIABLE:
            visit_function_decl_variable__CIParser(self,
                                                   &decl->variable,
                                                   called_generic_params,
                                                   decl_generic_params);

            break;
        default:
            // TODO: visit other declarations
            break;
    }
}

void
visit_function_expr_array_access__CIParser(
  CIParser *self,
  const CIExprArrayAccess *array_access,
  CIGenericParams *called_generic_params,
  CIGenericParams *decl_generic_params)
{
    visit_function_expr__CIParser(
      self, array_access->array, called_generic_params, decl_generic_params);
    visit_function_expr__CIParser(
      self, array_access->access, called_generic_params, decl_generic_params);
}

void
visit_function_expr_binary__CIParser(CIParser *self,
                                     const CIExprBinary *binary,
                                     CIGenericParams *called_generic_params,
                                     CIGenericParams *decl_generic_params)
{
    visit_function_expr__CIParser(
      self, binary->left, called_generic_params, decl_generic_params);
    visit_function_expr__CIParser(
      self, binary->right, called_generic_params, decl_generic_params);
}

void
visit_function_expr_cast__CIParser(CIParser *self,
                                   const CIExprCast *cast,
                                   CIGenericParams *called_generic_params,
                                   CIGenericParams *decl_generic_params)
{
    visit_function_expr__CIParser(
      self, cast->expr, called_generic_params, decl_generic_params);
    visit_data_type__CIParser(
      self, cast->data_type, called_generic_params, decl_generic_params);
}

void
visit_function_expr_ternary__CIParser(CIParser *self,
                                      const CIExprTernary *ternary,
                                      CIGenericParams *called_generic_params,
                                      CIGenericParams *decl_generic_params)
{
    visit_function_expr__CIParser(
      self, ternary->cond, called_generic_params, decl_generic_params);
    visit_function_expr__CIParser(
      self, ternary->if_, called_generic_params, decl_generic_params);
    visit_function_expr__CIParser(
      self, ternary->else_, called_generic_params, decl_generic_params);
}

void
visit_function_expr_function_call__CIParser(
  CIParser *self,
  const CIExprFunctionCall *function_call,
  CIGenericParams *called_generic_params,
  CIGenericParams *decl_generic_params)
{
    generate_function_gen__CIParser(
      self,
      GET_PTR_RC(String, function_call->identifier),
      function_call->generic_params);

    for (Usize i = 0; i < function_call->params->len; ++i) {
        visit_function_expr__CIParser(self,
                                      get__Vec(function_call->params, i),
                                      called_generic_params,
                                      decl_generic_params);
    }
}

void
visit_function_expr_function_call_builtin__CIParser(
  CIParser *self,
  const CIExprFunctionCallBuiltin *function_call_builtin,
  CIGenericParams *called_generic_params,
  CIGenericParams *decl_generic_params)
{
    for (Usize i = 0; i < function_call_builtin->params->len; ++i) {
        visit_function_expr__CIParser(
          self,
          get__Vec(function_call_builtin->params, i),
          called_generic_params,
          decl_generic_params);
    }
}

void
visit_function_expr_struct_call__CIParser(
  CIParser *self,
  const CIExprStructCall *struct_call,
  CIGenericParams *called_generic_params,
  CIGenericParams *decl_generic_params)
{
    for (Usize i = 0; i < struct_call->fields->len; ++i) {
        CIExprStructFieldCall *field_call = get__Vec(struct_call->fields, i);

        visit_function_expr__CIParser(
          self, field_call->value, called_generic_params, decl_generic_params);
    }
}

void
visit_function_expr__CIParser(CIParser *self,
                              const CIExpr *expr,
                              CIGenericParams *called_generic_params,
                              CIGenericParams *decl_generic_params)
{
    switch (expr->kind) {
        case CI_EXPR_KIND_ALIGNOF:
            visit_function_expr__CIParser(
              self, expr->alignof_, called_generic_params, decl_generic_params);

            break;
        case CI_EXPR_KIND_ARRAY_ACCESS:
            visit_function_expr_array_access__CIParser(self,
                                                       &expr->array_access,
                                                       called_generic_params,
                                                       decl_generic_params);

            break;
        case CI_EXPR_KIND_BINARY:
            visit_function_expr_binary__CIParser(
              self, &expr->binary, called_generic_params, decl_generic_params);

            break;
        case CI_EXPR_KIND_CAST:
            visit_function_expr_cast__CIParser(
              self, &expr->cast, called_generic_params, decl_generic_params);

            break;
        case CI_EXPR_KIND_DATA_TYPE:
            visit_data_type__CIParser(self,
                                      expr->data_type,
                                      called_generic_params,
                                      decl_generic_params);

            break;
        case CI_EXPR_KIND_FUNCTION_CALL:
            visit_function_expr_function_call__CIParser(self,
                                                        &expr->function_call,
                                                        called_generic_params,
                                                        decl_generic_params);

            break;
        case CI_EXPR_KIND_FUNCTION_CALL_BUILTIN:
            visit_function_expr_function_call_builtin__CIParser(
              self,
              &expr->function_call_builtin,
              called_generic_params,
              decl_generic_params);

            break;
        case CI_EXPR_KIND_GROUPING:
            visit_function_expr__CIParser(
              self, expr->grouping, called_generic_params, decl_generic_params);

            break;
        case CI_EXPR_KIND_IDENTIFIER:
            // TODO: Handling the case of generic pass-by-reference functions.
            break;
        case CI_EXPR_KIND_LITERAL:
            break;
        case CI_EXPR_KIND_STRUCT_CALL:
            visit_function_expr_struct_call__CIParser(self,
                                                      &expr->struct_call,
                                                      called_generic_params,
                                                      decl_generic_params);

            break;
        case CI_EXPR_KIND_SIZEOF:
            visit_function_expr__CIParser(
              self, expr->sizeof_, called_generic_params, decl_generic_params);

            break;
        case CI_EXPR_KIND_TERNARY:
            visit_function_expr_ternary__CIParser(
              self, &expr->ternary, called_generic_params, decl_generic_params);

            break;
        case CI_EXPR_KIND_UNARY:
            visit_function_expr__CIParser(self,
                                          expr->unary.expr,
                                          called_generic_params,
                                          decl_generic_params);

            break;
        default:
            UNREACHABLE("unknown variant");
    }
}

void
visit_function_stmt_block__CIParser(CIParser *self,
                                    const CIStmtBlock *block,
                                    CIGenericParams *called_generic_params,
                                    CIGenericParams *decl_generic_params)
{
    visit_function_body__CIParser(
      self, block->body, called_generic_params, decl_generic_params);
}

void
visit_function_stmt_do_while__CIParser(CIParser *self,
                                       const CIStmtDoWhile *do_while,
                                       CIGenericParams *called_generic_params,
                                       CIGenericParams *decl_generic_params)
{
    visit_function_expr__CIParser(
      self, do_while->cond, called_generic_params, decl_generic_params);
    visit_function_body__CIParser(
      self, do_while->body, called_generic_params, decl_generic_params);
}

void
visit_function_stmt_for__CIParser(CIParser *self,
                                  const CIStmtFor *for_,
                                  CIGenericParams *called_generic_params,
                                  CIGenericParams *decl_generic_params)
{
    if (for_->init_clauses) {
        for (Usize i = 0; i < for_->init_clauses->len; ++i) {
            visit_function_item__CIParser(self,
                                          get__Vec(for_->init_clauses, i),
                                          called_generic_params,
                                          decl_generic_params);
        }
    }

    if (for_->expr1) {
        visit_function_expr__CIParser(
          self, for_->expr1, called_generic_params, decl_generic_params);
    }

    if (for_->exprs2) {
        for (Usize i = 0; i < for_->exprs2->len; ++i) {
            visit_function_expr__CIParser(self,
                                          get__Vec(for_->exprs2, i),
                                          called_generic_params,
                                          decl_generic_params);
        }
    }

    visit_function_body__CIParser(
      self, for_->body, called_generic_params, decl_generic_params);
}

void
visit_function_stmt_if_branch__CIParser(CIParser *self,
                                        const CIStmtIfBranch *if_branch,
                                        CIGenericParams *called_generic_params,
                                        CIGenericParams *decl_generic_params)
{
    visit_function_expr__CIParser(
      self, if_branch->cond, called_generic_params, decl_generic_params);
    visit_function_body__CIParser(
      self, if_branch->body, called_generic_params, decl_generic_params);
}

void
visit_function_stmt_if__CIParser(CIParser *self,
                                 const CIStmtIf *if_,
                                 CIGenericParams *called_generic_params,
                                 CIGenericParams *decl_generic_params)
{
    visit_function_stmt_if_branch__CIParser(
      self, if_->if_, called_generic_params, decl_generic_params);

    if (if_->else_ifs) {
        for (Usize i = 0; i < if_->else_ifs->len; ++i) {
            visit_function_stmt_if_branch__CIParser(self,
                                                    get__Vec(if_->else_ifs, i),
                                                    called_generic_params,
                                                    decl_generic_params);
        }
    }

    if (if_->else_) {
        visit_function_body__CIParser(
          self, if_->else_, called_generic_params, decl_generic_params);
    }
}

void
visit_function_stmt_switch__CIParser(CIParser *self,
                                     const CIStmtSwitch *switch_,
                                     CIGenericParams *called_generic_params,
                                     CIGenericParams *decl_generic_params)
{
    visit_function_expr__CIParser(
      self, switch_->expr, called_generic_params, decl_generic_params);
    visit_function_body__CIParser(
      self, switch_->body, called_generic_params, decl_generic_params);
}

void
visit_function_stmt_while__CIParser(CIParser *self,
                                    const CIStmtWhile *while_,
                                    CIGenericParams *called_generic_params,
                                    CIGenericParams *decl_generic_params)
{
    visit_function_expr__CIParser(
      self, while_->cond, called_generic_params, decl_generic_params);
    visit_function_body__CIParser(
      self, while_->body, called_generic_params, decl_generic_params);
}

void
visit_function_stmt__CIParser(CIParser *self,
                              const CIStmt *stmt,
                              CIGenericParams *called_generic_params,
                              CIGenericParams *decl_generic_params)
{
    switch (stmt->kind) {
        case CI_STMT_KIND_BLOCK:
            visit_function_stmt_block__CIParser(
              self, &stmt->block, called_generic_params, decl_generic_params);

            break;
        case CI_STMT_KIND_BREAK:
        case CI_STMT_KIND_CASE: // NOTE: We skip this case, can only contain
                                // constant expressions.
        case CI_STMT_KIND_CONTINUE:
        case CI_STMT_KIND_DEFAULT:
            break;
        case CI_STMT_KIND_DO_WHILE:
            return visit_function_stmt_do_while__CIParser(self,
                                                          &stmt->do_while,
                                                          called_generic_params,
                                                          decl_generic_params);
        case CI_STMT_KIND_FOR:
            visit_function_stmt_for__CIParser(
              self, &stmt->for_, called_generic_params, decl_generic_params);

            break;
        case CI_STMT_KIND_GOTO:
            break;
        case CI_STMT_KIND_IF:
            visit_function_stmt_if__CIParser(
              self, &stmt->if_, called_generic_params, decl_generic_params);

            break;
        case CI_STMT_KIND_RETURN:
            visit_function_expr__CIParser(
              self, stmt->return_, called_generic_params, decl_generic_params);

            break;
        case CI_STMT_KIND_SWITCH:
            visit_function_stmt_switch__CIParser(
              self, &stmt->switch_, called_generic_params, decl_generic_params);

            break;
        case CI_STMT_KIND_WHILE:
            visit_function_stmt_while__CIParser(
              self, &stmt->while_, called_generic_params, decl_generic_params);

            break;
        default:
            UNREACHABLE("unknown variant");
    }
}

void
visit_function_item__CIParser(CIParser *self,
                              const CIDeclFunctionItem *item,
                              CIGenericParams *called_generic_params,
                              CIGenericParams *decl_generic_params)
{
    switch (item->kind) {
        case CI_DECL_FUNCTION_ITEM_KIND_DECL:
            visit_function_decl__CIParser(
              self, item->decl, called_generic_params, decl_generic_params);

            break;
        case CI_DECL_FUNCTION_ITEM_KIND_EXPR:
            visit_function_expr__CIParser(
              self, item->expr, called_generic_params, decl_generic_params);

            break;
        case CI_DECL_FUNCTION_ITEM_KIND_STMT:
            visit_function_stmt__CIParser(
              self, &item->stmt, called_generic_params, decl_generic_params);

            break;
        default:
            UNREACHABLE("unknown variant");
    }
}

void
visit_function_body__CIParser(CIParser *self,
                              const CIDeclFunctionBody *body,
                              CIGenericParams *called_generic_params,
                              CIGenericParams *decl_generic_params)
{
    for (Usize i = 0; i < body->content->len; ++i) {
        visit_function_item__CIParser(self,
                                      get__Vec(body->content, i),
                                      called_generic_params,
                                      decl_generic_params);
    }
}

void
visit_function__CIParser(CIParser *self,
                         const CIDecl *function_decl,
                         CIGenericParams *called_generic_params)
{
    visit_function_body__CIParser(self,
                                  function_decl->function.body,
                                  called_generic_params,
                                  function_decl->function.generic_params);
}

void
generate_function_gen__CIParser(CIParser *self,
                                String *function_name,
                                CIGenericParams *called_generic_params)
{
    const CIDecl *function_decl =
      search_function__CIParser(self, function_name, NULL);

    if (!function_decl) {
        if (is_recursive_function__CIParser(self, function_name)) {
            TODO("function is recursive");
        }

        FAILED("unknown function, impossible to call unknown function");
    }

    // Generate gen function declaration
    if (called_generic_params &&
        !has_generic__CIGenericParams(called_generic_params)) {
        if (function_decl->is_prototype) {
            add_item_to_visit_waiting_list__CIParser(
              self,
              function_decl->kind,
              GET_PTR_RC(String, function_decl->function.name),
              called_generic_params);
        } else {
            String *serialized_called_function_name =
              serialize_name__CIDeclFunction(&function_decl->function,
                                             called_generic_params);
            const CIDecl *function_gen = search_function__CIResultFile(
              self->file, serialized_called_function_name);

            if (!function_gen) {
                CIDataType *subs_return_data_type =
                  substitute_data_type__CIParser(
                    function_decl->function.return_data_type,
                    function_decl->function.generic_params,
                    called_generic_params);

                visit_function__CIParser(
                  self, function_decl, called_generic_params);

                CIDecl *function_gen_decl =
                  NEW_VARIANT(CIDecl,
                              function_gen,
                              (CIDecl *)function_decl,
                              ref__CIGenericParams(called_generic_params),
                              serialized_called_function_name,
                              subs_return_data_type ? subs_return_data_type
                                                    : ref__CIDataType(
                                                        function_decl->function
                                                          .return_data_type) /* Return a ref data type, when the substituted data type is NULL, to avoid an optional data type in the `return_data_type` field. */);

                add_decl_to_scope__CIParser(self, &function_gen_decl, true);

                if (function_gen_decl) {
                    typecheck_function_gen__CIParser(self, function_gen_decl);
                }
            }
        }
    }
}

void
generate_type_gen__CIParser(CIParser *self,
                            enum CIDeclKind kind,
                            String *name,
                            CIGenericParams *called_generic_params,
                            bool (*is_recursive)(const CIParser *,
                                                 const String *),
                            CIDecl *(*search_decl)(const CIResultFile *,
                                                   const String *))
{
    CIDecl *decl = search_decl(self->file, name);

    if (!decl) {
        if (is_recursive(self, name)) {
            if (called_generic_params) {
                add_item_to_visit_waiting_list__CIParser(
                  self, kind, name, called_generic_params);
            }

            return;
        }

        FAILED("struct, typedef or union not found");
    }

    ASSERT(kind == decl->kind);

    if (called_generic_params &&
        !has_generic__CIGenericParams(called_generic_params)) {
        if (decl->is_prototype && !is_typedef__CIDecl(decl) &&
            !resolve_visit_waiting_list) {
            add_item_to_visit_waiting_list__CIParser(
              self, decl->kind, get_name__CIDecl(decl), called_generic_params);
        } else {
            String *serialized_called_decl_name = NULL;

            switch (decl->kind) {
                case CI_DECL_KIND_STRUCT:
                    serialized_called_decl_name = serialize_name__CIDeclStruct(
                      &decl->struct_, called_generic_params);

                    break;
                case CI_DECL_KIND_TYPEDEF:
                    serialized_called_decl_name = serialize_name__CIDeclTypedef(
                      &decl->typedef_, called_generic_params);

                    break;
                case CI_DECL_KIND_UNION:
                    serialized_called_decl_name = serialize_name__CIDeclUnion(
                      &decl->union_, called_generic_params);

                    break;
                default:
                    UNREACHABLE("this kind of variant is not exepcted");
            }

            CIDecl *decl_gen =
              search_decl(self->file, serialized_called_decl_name);

            if (!decl_gen) {
                switch (decl->kind) {
                    case CI_DECL_KIND_STRUCT: {
                        Vec *fields = visit_struct_or_union__CIParser(
                          self, decl, called_generic_params);

                        decl_gen = NEW_VARIANT(
                          CIDecl,
                          struct_gen,
                          decl,
                          ref__CIGenericParams(called_generic_params),
                          serialized_called_decl_name,
                          fields);

                        break;
                    }
                    case CI_DECL_KIND_TYPEDEF: {
                        CIDataType *data_type = visit_typedef__CIParser(
                          self, decl, called_generic_params);

                        decl_gen = NEW_VARIANT(
                          CIDecl,
                          typedef_gen,
                          decl,
                          ref__CIGenericParams(called_generic_params),
                          serialized_called_decl_name,
                          data_type);

                        break;
                    }
                    case CI_DECL_KIND_UNION: {
                        Vec *fields = visit_struct_or_union__CIParser(
                          self, decl, called_generic_params);

                        decl_gen = NEW_VARIANT(
                          CIDecl,
                          union_gen,
                          decl,
                          ref__CIGenericParams(called_generic_params),
                          serialized_called_decl_name,
                          fields);

                        break;
                    }
                    default:
                        UNREACHABLE("this kind of variant is not expected");
                }

                ASSERT(decl_gen->kind & CI_DECL_KIND_GEN);

                add_decl_to_scope__CIParser(self, &decl_gen, true);
            } else {
                FREE(String, serialized_called_decl_name);
            }
        }
    }
}

void
generate_struct_gen__CIParser(CIParser *self,
                              String *struct_name_ref,
                              CIGenericParams *called_generic_params)
{
    generate_type_gen__CIParser(self,
                                CI_DECL_KIND_STRUCT,
                                struct_name_ref,
                                called_generic_params,
                                &is_recursive_struct__CIParser,
                                &search_struct__CIResultFile);
}

void
generate_union_gen__CIParser(CIParser *self,
                             String *union_name_ref,
                             CIGenericParams *called_generic_params)
{
    generate_type_gen__CIParser(self,
                                CI_DECL_KIND_UNION,
                                union_name_ref,
                                called_generic_params,
                                &is_recursive_union__CIParser,
                                &search_union__CIResultFile);
}

void
generate_typedef_gen__CIParser(CIParser *self,
                               String *typedef_name_ref,
                               CIGenericParams *called_generic_params)
{
    generate_type_gen__CIParser(self,
                                CI_DECL_KIND_TYPEDEF,
                                typedef_name_ref,
                                called_generic_params,
                                &is_recursive_typedef__CIParser,
                                &search_typedef__CIResultFile);
}

int
parse_data_type_contexts__CIParser(CIParser *self)
{
    int data_type_ctx = CI_DATA_TYPE_CONTEXT_NONE;

    while (self->current_token->kind == CI_TOKEN_KIND_BANG) {
        next_token__CIParser(self);

        switch (self->current_token->kind) {
            case CI_TOKEN_KIND_IDENTIFIER: {
#define CTX_IDS_LEN 4
                enum CIDataTypeContext ctx_ids[CTX_IDS_LEN] = {
                    CI_DATA_TYPE_CONTEXT_HEAP,
                    CI_DATA_TYPE_CONTEXT_NON_NULL,
                    CI_DATA_TYPE_CONTEXT_STACK,
                    CI_DATA_TYPE_CONTEXT_TRACE
                };
                static SizedStr ctx_ids_s[CTX_IDS_LEN] = {
                    SIZED_STR_FROM_RAW("heap"),
                    SIZED_STR_FROM_RAW("non_null"),
                    SIZED_STR_FROM_RAW("stack"),
                    SIZED_STR_FROM_RAW("trace")
                };
                int current_ctx = get_id__Search(
                  GET_PTR_RC(String, self->current_token->identifier),
                  ctx_ids_s,
                  (const Int32 *)ctx_ids,
                  CTX_IDS_LEN);

                if (current_ctx == -1) {
                    FAILED("expected only !heap, !non_null, !stack or !trace "
                           "as data type context");
                } else {
                    data_type_ctx |= current_ctx;
                }

#undef CTX_IDS_LEN

                next_token__CIParser(self);

                break;
            }
            default:
                FAILED(
                  "expected identifier after data type context, e.g. !<id>");
        }
    }

    return data_type_ctx;
}

CIDataType *
parse_post_data_type__CIParser(CIParser *self, CIDataType *data_type)
{
    bool is_data_type_qualifier = false;

    while (self->current_token->kind == CI_TOKEN_KIND_STAR ||
           (is_data_type_qualifier = is_data_type_qualifier__CIParser(self))) {
        if (is_data_type_qualifier) {
            parse_data_type_qualifiers__CIParser(self,
                                                 &data_type_qualifier_flag);
            set_and_check_data_type_qualifier__CIParser(
              self, data_type, data_type_qualifier_flag);

            RESET_DATA_TYPE_QUALIFIER_FLAG();
            is_data_type_qualifier = false;

            continue;
        }

        switch (self->current_token->kind) {
            case CI_TOKEN_KIND_STAR:
                data_type = NEW_VARIANT(CIDataType, ptr, data_type);

                break;
            default:
                goto exit;
        }

        next_token__CIParser(self);
    }

exit:
    set_and_check_data_type_context__CIParser(
      self, data_type, parse_data_type_contexts__CIParser(self));

    return data_type;
}

void
parse_variable_name_and_data_type__CIParser(CIParser *self,
                                            CIDataType **data_type_ref,
                                            Rc **name_ref,
                                            bool name_is_required)
{
#define PARSE_VARIABLE_NAME(block)                           \
    switch (self->current_token->kind) {                     \
        case CI_TOKEN_KIND_IDENTIFIER:                       \
            if (*name_ref) {                                 \
                FAILED("identifier is already declared");    \
            } else {                                         \
                *name_ref = self->current_token->identifier; \
                                                             \
                next_token__CIParser(self);                  \
            }                                                \
                                                             \
            break;                                           \
            block;                                           \
        default:                                             \
            if (name_is_required) {                          \
                FAILED("expected identifier");               \
                /* return; */                                \
            }                                                \
    }

    ASSERT(data_type_ref);
    ASSERT(name_ref);

    PARSE_VARIABLE_NAME(case CI_TOKEN_KIND_LPAREN : break;);

    Usize i = 0;

loop:
    i += 1;

    switch (self->current_token->kind) {
        case CI_TOKEN_KIND_LHOOK: {
            next_token__CIParser(self); // skip `[`

            switch (self->current_token->kind) {
                case CI_TOKEN_KIND_RHOOK:
                    // <id>[n][p]
                    *data_type_ref =
                      NEW_VARIANT(CIDataType,
                                  array,
                                  NEW_VARIANT(CIDataTypeArray,
                                              none,
                                              *data_type_ref,
                                              i == 1 ? *name_ref : NULL));

                    next_token__CIParser(self);

                    goto loop;
                default:
                    break;
            }

            CIExpr *expr = parse_expr__CIParser(self);

            if (!expr) {
                return;
            }

            const CIResolverExpr resolver_expr =
              NEW(CIResolverExpr, self, current_scope, false);
            CIExpr *resolved_expr = run__CIResolverExpr(&resolver_expr, expr);

            Usize size = 0;

            switch (resolved_expr->kind) {
                case CI_EXPR_KIND_LITERAL:
                    switch (resolved_expr->literal.kind) {
                        case CI_EXPR_LITERAL_KIND_SIGNED_INT:
                            if (resolved_expr->literal.signed_int < 0) {
                                FAILED("indexed integer must be positive");
                            } else {
                                size = resolved_expr->literal.signed_int;
                            }

                            break;
                        case CI_EXPR_LITERAL_KIND_UNSIGNED_INT:
                            size = resolved_expr->literal.unsigned_int;

                            break;
                        default:
                            goto expected_integer_literal;
                    }

                    break;
                default:
                expected_integer_literal:
                    FAILED("expected integer literal expression");
            }

            FREE(CIExpr, expr);
            FREE(CIExpr, resolved_expr);

            expect__CIParser(self, CI_TOKEN_KIND_RHOOK, true);

            *data_type_ref = NEW_VARIANT(CIDataType,
                                         array,
                                         NEW_VARIANT(CIDataTypeArray,
                                                     sized,
                                                     *data_type_ref,
                                                     i == 1 ? *name_ref : NULL,
                                                     size));

            goto loop;
        }
        case CI_TOKEN_KIND_LPAREN: {
            // NOTE: If the name is not NULL at this level, this is the
            // beginning of a function declaration and not a function data type.
            if (*name_ref) {
                goto exit;
            }

            next_token__CIParser(self); // `(`

            CIDataType *function_data_type = NULL; // CIDataType*?

            switch (self->current_token->kind) {
                case CI_TOKEN_KIND_STAR:
                    function_data_type =
                      parse_post_data_type__CIParser(self, NULL);

                    break;
                default:
                    break;
            }

            PARSE_VARIABLE_NAME({});

            expect__CIParser(self, CI_TOKEN_KIND_RPAREN, true);

            Vec *params = NULL;

            switch (self->current_token->kind) {
                case CI_TOKEN_KIND_LPAREN: {
                    CIScope *parent_function_scope = add_scope__CIResultFile(
                      self->file, current_scope->scope_id, true);
                    params = parse_function_params__CIParser(
                      self, parent_function_scope);

                    break;
                }
                default:
                    break;
            }

            *data_type_ref = NEW_VARIANT(CIDataType,
                                         function,
                                         NEW(CIDataTypeFunction,
                                             *name_ref,
                                             params,
                                             *data_type_ref,
                                             function_data_type));

            goto loop;
        }
        default:
            goto exit;
    }

exit:
#undef PARSE_VARIABLE_NAME
}

void
parse_storage_class_specifiers_and_data_type_qualifiers__CIParser(
  CIParser *self,
  int *data_type_qualifier_flag,
  int *storage_class_flag)
{
    bool is_data_type_qualifier = false;
    bool is_storage_class_specifier = false;

    while ((is_data_type_qualifier = is_data_type_qualifier__CIParser(self)) ||
           (is_storage_class_specifier = is_storage_class__CIParser(self))) {
        if (is_data_type_qualifier) {
            parse_data_type_qualifiers__CIParser(self,
                                                 data_type_qualifier_flag);
        } else if (is_storage_class_specifier) {
            parse_storage_class_specifiers__CIParser(self, storage_class_flag);
        }
    }
}

void
set_and_check_data_type_qualifier__CIParser(CIParser *self,
                                            CIDataType *data_type,
                                            int data_type_qualifier_flag)
{
    if (data_type_qualifier_flag & CI_DATA_TYPE_QUALIFIER_RESTRICT) {
        if (!is_ptr_data_type__CIParser(self, data_type, NULL, NULL)) {
            FAILED("expected pointer with restrict qualifier");
        }
    }

    set_qualifier__CIDataType(data_type, data_type_qualifier_flag);
}

void
valid_data_type_context__CIParser(CIParser *self, int context_flag)
{
    static enum CIDataTypeContext rejected_context_when_none_is_present[] = {};
    static enum CIDataTypeContext rejected_context_when_heap_is_present[] = {
        CI_DATA_TYPE_CONTEXT_STACK
    };
    static enum CIDataTypeContext
      rejected_context_when_non_null_is_present[] = {};
    static enum CIDataTypeContext rejected_context_when_stack_is_present[] = {
        CI_DATA_TYPE_CONTEXT_HEAP
    };
    static enum CIDataTypeContext rejected_context_when_trace_is_present[] = {};
    typedef struct CIDataTypeRejectedContexts
    {
        enum CIDataTypeContext *buffer;
        Usize len;
    } CIDataTypeRejectedContexts;
    static CIDataTypeRejectedContexts
      rejected_context_when_x_context_is_present[] = {
          { rejected_context_when_none_is_present,
            sizeof(rejected_context_when_none_is_present) /
              sizeof(*rejected_context_when_none_is_present) },
          { rejected_context_when_heap_is_present,
            sizeof(rejected_context_when_heap_is_present) /
              sizeof(*rejected_context_when_heap_is_present) },
          { rejected_context_when_non_null_is_present,
            sizeof(rejected_context_when_non_null_is_present) /
              sizeof(*rejected_context_when_non_null_is_present) },
          { rejected_context_when_stack_is_present,
            sizeof(rejected_context_when_stack_is_present) /
              sizeof(*rejected_context_when_stack_is_present) },
          { rejected_context_when_trace_is_present,
            sizeof(rejected_context_when_trace_is_present) /
              sizeof(*rejected_context_when_trace_is_present) }
      };
    CIDataTypeContextIds data_type_context_ids = get_ids__CIDataTypeContext();

    ASSERT(data_type_context_ids.len ==
           sizeof(rejected_context_when_x_context_is_present) /
             sizeof(*rejected_context_when_x_context_is_present));

    for (Usize i = 0; i < data_type_context_ids.len; ++i) {
        if (context_flag & data_type_context_ids.buffer[i]) {
            const CIDataTypeRejectedContexts *rejected_context_item =
              &rejected_context_when_x_context_is_present[i];

            for (Usize j = 0; j < rejected_context_item->len; ++j) {
                if (context_flag & rejected_context_item->buffer[j]) {
                    FAILED("incompatible data type context");
                }
            }
        }
    }
}

void
valid_data_type_context_according_data_type__CIParser(CIParser *self,
                                                      CIDataType *data_type,
                                                      int context_flag)
{
    if (context_flag != CI_DATA_TYPE_CONTEXT_NONE &&
        !is_ptr_data_type__CIParser(self, data_type, NULL, NULL)) {
        FAILED("the data type need to be a pointer to set this kind of context "
               "flag");
    }
}

void
set_and_check_data_type_context__CIParser(CIParser *self,
                                          CIDataType *data_type,
                                          int context_flag)
{
    valid_data_type_context__CIParser(self, context_flag);
    valid_data_type_context_according_data_type__CIParser(
      self, data_type, context_flag);
    set_context__CIDataType(data_type, context_flag);
}

void
add_flag__CIDataTypeCombination(CIParser *self,
                                Uint32 *flags,
                                enum CIDataTypeCombination flag)
{
    Uint32 old_flags = *flags;

    *flags |= flag;

    if (old_flags == *flags) {
        // Set `long long` flag if `long` flag is already set.
        if (flag == CI_DATA_TYPE_COMBINATION_LONG) {
            *flags &= ~CI_DATA_TYPE_COMBINATION_LONG;
            *flags |= CI_DATA_TYPE_COMBINATION_LONG_LONG;
        } else {
            FAILED("flag already set");
        }
    }
}

enum CIDataTypeCombination
convert_token_kind_to_data_type_combination__CIDataTypeCombination(
  enum CITokenKind kind)
{
    switch (kind) {
        case CI_TOKEN_KIND_KEYWORD_CHAR:
            return CI_DATA_TYPE_COMBINATION_CHAR;
        case CI_TOKEN_KIND_KEYWORD_DOUBLE:
            return CI_DATA_TYPE_COMBINATION_DOUBLE;
        case CI_TOKEN_KIND_KEYWORD_FLOAT:
            return CI_DATA_TYPE_COMBINATION_FLOAT;
        case CI_TOKEN_KIND_KEYWORD_INT:
            return CI_DATA_TYPE_COMBINATION_INT;
        case CI_TOKEN_KIND_KEYWORD_LONG:
            return CI_DATA_TYPE_COMBINATION_LONG;
        case CI_TOKEN_KIND_KEYWORD_SHORT:
            return CI_DATA_TYPE_COMBINATION_SHORT;
        case CI_TOKEN_KIND_KEYWORD_SIGNED:
            return CI_DATA_TYPE_COMBINATION_SIGNED;
        case CI_TOKEN_KIND_KEYWORD_UNSIGNED:
            return CI_DATA_TYPE_COMBINATION_UNSIGNED;
        case CI_TOKEN_KIND_KEYWORD__COMPLEX:
            return CI_DATA_TYPE_COMBINATION__COMPLEX;
        case CI_TOKEN_KIND_KEYWORD__IMAGINARY:
            return CI_DATA_TYPE_COMBINATION__IMAGINARY;
        default:
            return CI_DATA_TYPE_COMBINATION_NONE;
    }
}

bool
is_multi_part_keyword_from_token_kind__CIKeywordMultiPart(enum CITokenKind kind)
{
    switch (kind) {
        case CI_TOKEN_KIND_KEYWORD_CHAR:
        case CI_TOKEN_KIND_KEYWORD_DOUBLE:
        case CI_TOKEN_KIND_KEYWORD_FLOAT:
        case CI_TOKEN_KIND_KEYWORD_INT:
        case CI_TOKEN_KIND_KEYWORD_LONG:
        case CI_TOKEN_KIND_KEYWORD_SHORT:
        case CI_TOKEN_KIND_KEYWORD_SIGNED:
        case CI_TOKEN_KIND_KEYWORD_UNSIGNED:
        case CI_TOKEN_KIND_KEYWORD__COMPLEX:
        case CI_TOKEN_KIND_KEYWORD__IMAGINARY:
            return true;
        default:
            return false;
    }
}

int
convert_data_type_combination_to_data_type__CIParser(CIParser *self,
                                                     int combination)
{
    switch (combination) {
        case CI_DATA_TYPE_COMBINATION_CHAR:
            return CI_DATA_TYPE_KIND_CHAR;
        case CI_DATA_TYPE_COMBINATION_DOUBLE:
            return CI_DATA_TYPE_KIND_DOUBLE;
        case CI_DATA_TYPE_COMBINATION_DOUBLE |
          CI_DATA_TYPE_COMBINATION__COMPLEX:
            return CI_DATA_TYPE_KIND_DOUBLE__COMPLEX;
        case CI_DATA_TYPE_COMBINATION_DOUBLE |
          CI_DATA_TYPE_COMBINATION__IMAGINARY:
            return CI_DATA_TYPE_KIND_DOUBLE__IMAGINARY;
        case CI_DATA_TYPE_COMBINATION_FLOAT:
            return CI_DATA_TYPE_KIND_FLOAT;
        case CI_DATA_TYPE_COMBINATION_FLOAT | CI_DATA_TYPE_COMBINATION__COMPLEX:
            return CI_DATA_TYPE_KIND_FLOAT__COMPLEX;
        case CI_DATA_TYPE_COMBINATION_FLOAT |
          CI_DATA_TYPE_COMBINATION__IMAGINARY:
            return CI_DATA_TYPE_KIND_FLOAT__IMAGINARY;
        case CI_DATA_TYPE_COMBINATION_INT:
        case CI_DATA_TYPE_COMBINATION_SIGNED:
        case CI_DATA_TYPE_COMBINATION_SIGNED | CI_DATA_TYPE_COMBINATION_INT:
            return CI_DATA_TYPE_KIND_INT;
        case CI_DATA_TYPE_COMBINATION_LONG:
        case CI_DATA_TYPE_COMBINATION_LONG | CI_DATA_TYPE_COMBINATION_INT:
        case CI_DATA_TYPE_COMBINATION_SIGNED | CI_DATA_TYPE_COMBINATION_LONG:
        case CI_DATA_TYPE_COMBINATION_SIGNED | CI_DATA_TYPE_COMBINATION_LONG |
          CI_DATA_TYPE_COMBINATION_INT:
            return CI_DATA_TYPE_KIND_LONG_INT;
        case CI_DATA_TYPE_COMBINATION_LONG | CI_DATA_TYPE_COMBINATION_DOUBLE:
            return CI_DATA_TYPE_KIND_LONG_DOUBLE;
        case CI_DATA_TYPE_COMBINATION_LONG | CI_DATA_TYPE_COMBINATION_DOUBLE |
          CI_DATA_TYPE_COMBINATION__COMPLEX:
            return CI_DATA_TYPE_KIND_LONG_DOUBLE__COMPLEX;
        case CI_DATA_TYPE_COMBINATION_LONG | CI_DATA_TYPE_COMBINATION_DOUBLE |
          CI_DATA_TYPE_COMBINATION__IMAGINARY:
            return CI_DATA_TYPE_KIND_LONG_DOUBLE__IMAGINARY;
        case CI_DATA_TYPE_COMBINATION_LONG_LONG:
        case CI_DATA_TYPE_COMBINATION_LONG_LONG | CI_DATA_TYPE_COMBINATION_INT:
        case CI_DATA_TYPE_COMBINATION_SIGNED |
          CI_DATA_TYPE_COMBINATION_LONG_LONG:
        case CI_DATA_TYPE_COMBINATION_SIGNED |
          CI_DATA_TYPE_COMBINATION_LONG_LONG | CI_DATA_TYPE_COMBINATION_INT:
            return CI_DATA_TYPE_KIND_LONG_LONG_INT;
        case CI_DATA_TYPE_COMBINATION_SHORT:
        case CI_DATA_TYPE_COMBINATION_SHORT | CI_DATA_TYPE_COMBINATION_INT:
        case CI_DATA_TYPE_COMBINATION_SIGNED | CI_DATA_TYPE_COMBINATION_SHORT:
        case CI_DATA_TYPE_COMBINATION_SIGNED | CI_DATA_TYPE_COMBINATION_SHORT |
          CI_DATA_TYPE_COMBINATION_INT:
            return CI_DATA_TYPE_KIND_SHORT_INT;
        case CI_DATA_TYPE_COMBINATION_SIGNED | CI_DATA_TYPE_COMBINATION_CHAR:
            return CI_DATA_TYPE_KIND_SIGNED_CHAR;
        case CI_DATA_TYPE_COMBINATION_UNSIGNED:
        case CI_DATA_TYPE_COMBINATION_UNSIGNED | CI_DATA_TYPE_COMBINATION_INT:
            return CI_DATA_TYPE_KIND_UNSIGNED_INT;
        case CI_DATA_TYPE_COMBINATION_UNSIGNED | CI_DATA_TYPE_COMBINATION_CHAR:
            return CI_DATA_TYPE_KIND_UNSIGNED_CHAR;
        case CI_DATA_TYPE_COMBINATION_UNSIGNED | CI_DATA_TYPE_COMBINATION_LONG:
        case CI_DATA_TYPE_COMBINATION_UNSIGNED | CI_DATA_TYPE_COMBINATION_LONG |
          CI_DATA_TYPE_COMBINATION_INT:
            return CI_DATA_TYPE_KIND_UNSIGNED_LONG_INT;
        case CI_DATA_TYPE_COMBINATION_UNSIGNED |
          CI_DATA_TYPE_COMBINATION_LONG_LONG:
        case CI_DATA_TYPE_COMBINATION_UNSIGNED |
          CI_DATA_TYPE_COMBINATION_LONG_LONG | CI_DATA_TYPE_COMBINATION_INT:
            return CI_DATA_TYPE_KIND_UNSIGNED_LONG_LONG_INT;
        case CI_DATA_TYPE_COMBINATION_UNSIGNED | CI_DATA_TYPE_COMBINATION_SHORT:
        case CI_DATA_TYPE_COMBINATION_UNSIGNED |
          CI_DATA_TYPE_COMBINATION_SHORT | CI_DATA_TYPE_COMBINATION_INT:
            return CI_DATA_TYPE_KIND_UNSIGNED_SHORT_INT;
        default:
            return -1;
    }
}

int
parse_data_type_combination__CIParser(CIParser *self)
{
    Uint32 keyword_multi_part_flag = 0;

    while (is_multi_part_keyword_from_token_kind__CIKeywordMultiPart(
      self->previous_token->kind)) {
        enum CIDataTypeCombination data_type_combination =
          convert_token_kind_to_data_type_combination__CIDataTypeCombination(
            self->previous_token->kind);

        add_flag__CIDataTypeCombination(
          self, &keyword_multi_part_flag, data_type_combination);

        if (is_multi_part_keyword_from_token_kind__CIKeywordMultiPart(
              self->current_token->kind)) {
            next_token__CIParser(self);
        } else {
            break;
        }
    }

    return convert_data_type_combination_to_data_type__CIParser(
      self, keyword_multi_part_flag);
}

CIDataType *
parse_pre_data_type__CIParser(CIParser *self)
{
    CIDataType *res = NULL;

    // <storage_class_flag | dt_qualifier> <pre_dt>
    // e.g. static int, const int
    parse_storage_class_specifiers_and_data_type_qualifiers__CIParser(
      self, &data_type_qualifier_flag, &storage_class_flag);
    next_token__CIParser(self);

    switch (self->previous_token->kind) {
        case CI_TOKEN_KIND_IDENTIFIER: {
            if (is__CIBuiltinType(
                  GET_PTR_RC(String, self->previous_token->identifier))) {
                res = NEW_VARIANT(CIDataType,
                                  builtin,
                                  get_id__CIBuiltinType(GET_PTR_RC(
                                    String, self->previous_token->identifier)));

                break;
            }

            Rc *name = self->previous_token->identifier; // Rc<String*>* (&)
            CIGenericParams *generic_params =
              parse_generic_params__CIParser(self);

            res = NEW_VARIANT(CIDataType,
                              typedef,
                              NEW(CIDataTypeTypedef, name, generic_params));

            generate_typedef_gen__CIParser(
              self, GET_PTR_RC(String, name), generic_params);

            break;
        }
        case CI_TOKEN_KIND_AT: {
            Rc *generic = NULL; // Rc<String*>* (&)

            if (expect__CIParser(self, CI_TOKEN_KIND_IDENTIFIER, true)) {
                generic = self->previous_token->identifier;
            } else {
                generic = generate_name_error__CIParser();
            }

            res = NEW_VARIANT(CIDataType, generic, generic);

            break;
        }
        case CI_TOKEN_KIND_KEYWORD_BOOL:
        case CI_TOKEN_KIND_KEYWORD__BOOL:
            res = NEW(CIDataType, CI_DATA_TYPE_KIND_BOOL);

            break;
        case CI_TOKEN_KIND_KEYWORD_ENUM: {
            Rc *name = NULL; // Rc<String*>*? (&)

            // enum <name> ...;
            // enum <name> : ...;
            // enum <name> { ... } ...;
            // enum { ... } ...;
            // NOTE: A enum can be anonymous.
            switch (self->current_token->kind) {
                case CI_TOKEN_KIND_IDENTIFIER:
                    name = self->current_token->identifier;
                    next_token__CIParser(self);

                    break;
                default:
                    break;
            }

            res = NEW_VARIANT(CIDataType, enum, name);

            switch (self->current_token->kind) {
                case CI_TOKEN_KIND_COLON:
                case CI_TOKEN_KIND_LBRACE:
                case CI_TOKEN_KIND_SEMICOLON: {
                    CIDecl *enum_decl =
                      parse_enum__CIParser(self, storage_class_flag, name);

                    add_decl_to_scope__CIParser(self, &enum_decl, true);

                    break;
                }
                default:
                    break;
            }

            break;
        }
        case CI_TOKEN_KIND_KEYWORD_STRUCT:
        case CI_TOKEN_KIND_KEYWORD_UNION: {
            enum CITokenKind previous_token_kind = self->previous_token->kind;
            Rc *name = NULL; // Rc<String*>*? (&)

            // struct <name><generic_params, ...> ...;
            // struct <name><generic_params, ...> { ... } ...;
            // struct { ... } ...;
            // NOTE: A union or struct can be anonymous.
            switch (self->current_token->kind) {
                case CI_TOKEN_KIND_IDENTIFIER:
                    name = self->current_token->identifier;
                    next_token__CIParser(self);

                    break;
                default:
                    break;
            }

            CIGenericParams *generic_params =
              parse_generic_params__CIParser(self); // CIGenericParams*?

            switch (previous_token_kind) {
                case CI_TOKEN_KIND_KEYWORD_STRUCT:
                    res = NEW_VARIANT(
                      CIDataType,
                      struct,
                      NEW(CIDataTypeStruct, name, generic_params, NULL));

                    break;
                case CI_TOKEN_KIND_KEYWORD_UNION:
                    res = NEW_VARIANT(
                      CIDataType,
                      union,
                      NEW(CIDataTypeUnion, name, generic_params, NULL));

                    break;
                default:
                    UNREACHABLE("unknown variant");
            }

            switch (previous_token_kind) {
                case CI_TOKEN_KIND_KEYWORD_STRUCT: {
                    CIDecl *struct_decl = parse_struct__CIParser(
                      self,
                      CI_STORAGE_CLASS_NONE,
                      name,
                      generic_params ? ref__CIGenericParams(generic_params)
                                     : NULL);

                    if (add_decl_to_scope__CIParser(self, &struct_decl, true)) {
                        generate_struct_gen__CIParser(
                          self, GET_PTR_RC(String, name), generic_params);

                        break;
                    }

                    if (struct_decl->struct_.fields &&
                        !struct_decl->struct_.name) {
                        res->struct_.fields = clone_fields__CIDeclStructField(
                          struct_decl->struct_.fields);
                        FREE(CIDecl, struct_decl);
                    }

                    break;
                }
                case CI_TOKEN_KIND_KEYWORD_UNION: {
                    CIDecl *union_decl = parse_union__CIParser(
                      self,
                      CI_STORAGE_CLASS_NONE,
                      name,
                      generic_params ? ref__CIGenericParams(generic_params)
                                     : NULL);

                    if (add_decl_to_scope__CIParser(self, &union_decl, true)) {
                        generate_union_gen__CIParser(
                          self, GET_PTR_RC(String, name), generic_params);

                        break;
                    }

                    if (union_decl->union_.fields && !union_decl->union_.name) {
                        res->union_.fields = clone_fields__CIDeclStructField(
                          union_decl->union_.fields);
                        FREE(CIDecl, union_decl);
                    }

                    break;
                }
                default:
                    UNREACHABLE("unknown variant");
            }

            break;
        }
        case CI_TOKEN_KIND_KEYWORD_TYPEOF: {
            expect__CIParser(self, CI_TOKEN_KIND_LPAREN, true);

            CIExpr *expr = parse_expr__CIParser(self);

            expect__CIParser(self, CI_TOKEN_KIND_RPAREN, true);

            res = infer_expr_data_type__CIParser(
              self,
              expr,
              current_scope ? current_scope->scope_id : NULL,
              NULL,
              NULL);

            FREE(CIExpr, expr);

            break;
        }
        case CI_TOKEN_KIND_KEYWORD_VOID:
            res = NEW(CIDataType, CI_DATA_TYPE_KIND_VOID);

            break;
        case CI_TOKEN_KIND_KEYWORD__DECIMAL128:
            res = NEW(CIDataType, CI_DATA_TYPE_KIND__DECIMAL128);

            break;
        case CI_TOKEN_KIND_KEYWORD__DECIMAL32:
            res = NEW(CIDataType, CI_DATA_TYPE_KIND__DECIMAL32);

            break;
        case CI_TOKEN_KIND_KEYWORD__DECIMAL64:
            res = NEW(CIDataType, CI_DATA_TYPE_KIND__DECIMAL64);

            break;
        default: {
            int data_type_kind = parse_data_type_combination__CIParser(self);

            if (data_type_kind == -1) {
                FAILED("expected data type");
            }

            res = NEW(CIDataType, data_type_kind);
        }
    }

    // <pre_dt> <storage_class_flag | dt_qualifier>
    // e.g. int static, int const
    parse_storage_class_specifiers_and_data_type_qualifiers__CIParser(
      self, &data_type_qualifier_flag, &storage_class_flag);

    set_and_check_data_type_qualifier__CIParser(
      self, res, data_type_qualifier_flag);

    RESET_DATA_TYPE_QUALIFIER_FLAG();

    return res;
}

CIDataType *
parse_data_type__CIParser(CIParser *self)
{
    RESET_DATA_TYPE_QUALIFIER_FLAG();

    CIDataType *pre = parse_pre_data_type__CIParser(self);

    return parse_post_data_type__CIParser(self, pre);
}

Vec *
parse_enum_variants__CIParser(CIParser *self)
{
    Vec *variants = NEW(Vec); // Vec<CIDeclEnumVariant*>*
    Isize precedent_value = -1;

    while (self->current_token->kind != CI_TOKEN_KIND_RBRACE &&
           self->current_token->kind != CI_TOKEN_KIND_EOF) {
        Rc *name = NULL; // Rc<String*>* (&)

        if (expect__CIParser(self, CI_TOKEN_KIND_IDENTIFIER, true)) {
            name = self->previous_token->identifier;
        } else {
            name = generate_name_error__CIParser();
        }

        CIDeclEnumVariant *variant = NULL;

        switch (self->current_token->kind) {
            case CI_TOKEN_KIND_EQ: {
                next_token__CIParser(self);

                CIExpr *custom_expr = parse_expr__CIParser(self);
                CIResolverExpr resolver_expr =
                  NEW(CIResolverExpr, self, current_scope, false);
                CIExpr *resolved_custom_expr =
                  run__CIResolverExpr(&resolver_expr, custom_expr);
                Isize custom_value = 0;

                switch (resolved_custom_expr->kind) {
                    case CI_EXPR_KIND_LITERAL:
                        custom_value = to_literal_integer_value__CIResolverExpr(
                          resolved_custom_expr);

                        break;
                    default:
                        FAILED(
                          "expected literal expression (passing a constant or "
                          "enum variant is not yet supported).");
                }

                precedent_value = custom_value;
                variant = NEW(CIDeclEnumVariant, name, custom_value);

                push__Vec(variants, variant);

                FREE(CIExpr, custom_expr);
                FREE(CIExpr, resolved_custom_expr);

                break;
            }
            default:
                variant = NEW(CIDeclEnumVariant, name, ++precedent_value);

                push__Vec(variants, variant);
        }

        {
            CIDecl *variant_decl =
              NEW_VARIANT(CIDecl, enum_variant, CI_STORAGE_CLASS_NONE, variant);

            add_decl_to_scope__CIParser(self, &variant_decl, true);
        }

        if (self->current_token->kind != CI_TOKEN_KIND_RBRACE) {
            expect__CIParser(self, CI_TOKEN_KIND_COMMA, true);
        }
    }

    expect__CIParser(self, CI_TOKEN_KIND_RBRACE, true);

    if (variants->len == 0) {
        FAILED("expected one or many variants");
    }

    return variants;
}

CIDecl *
parse_enum__CIParser(CIParser *self, int storage_class_flag, Rc *name)
{
    ASSERT(self->current_token->kind == CI_TOKEN_KIND_COLON ||
           self->current_token->kind == CI_TOKEN_KIND_LBRACE ||
           self->current_token->kind == CI_TOKEN_KIND_SEMICOLON);

    CIDataType *data_type = NULL;

    switch (self->current_token->kind) {
        case CI_TOKEN_KIND_COLON:
            next_token__CIParser(self);

            data_type = parse_data_type__CIParser(self);

            if (data_type) {
                if (!is_integer_data_type__CIParser(
                      self, data_type, false, NULL, NULL)) {
                    FAILED("expected integer data type");
                }
            }

            break;
        default:
            break;
    }

    switch (self->current_token->kind) {
        case CI_TOKEN_KIND_LBRACE:
            next_token__CIParser(self);
            break;
        case CI_TOKEN_KIND_SEMICOLON:
            return NEW_VARIANT(CIDecl,
                               enum,
                               storage_class_flag,
                               true,
                               NEW(CIDeclEnum, name, NULL, data_type));
        default:
            FAILED("expected `{` or `;`");
    }

    return NEW_VARIANT(
      CIDecl,
      enum,
      storage_class_flag,
      false,
      NEW(CIDeclEnum, name, parse_enum_variants__CIParser(self), data_type));
}

Vec *
parse_function_params__CIParser(CIParser *self, CIScope *parent_function_scope)
{
    next_token__CIParser(self); // skip `(`

    switch (self->current_token->kind) {
        case CI_TOKEN_KIND_RPAREN:
            next_token__CIParser(self);

            return NULL;
        default:
            break;
    }

    Vec *params = NEW(Vec); // Vec<CIDeclFunctionParam*>*

    while (self->current_token->kind != CI_TOKEN_KIND_RPAREN &&
           self->current_token->kind != CI_TOKEN_KIND_EOF) {
        switch (self->current_token->kind) {
            case CI_TOKEN_KIND_DOT_DOT_DOT: {
                push__Vec(params, NEW_VARIANT(CIDeclFunctionParam, variadic));
                next_token__CIParser(self);

                break;
            }
            default: {
                CIDataType *data_type = parse_data_type__CIParser(self);
                Rc *name = NULL; // Rc<String*>*? (&)

                parse_variable_name_and_data_type__CIParser(
                  self, &data_type, &name, false);

                push__Vec(
                  params,
                  NEW_VARIANT(CIDeclFunctionParam, normal, name, data_type));

                if (parent_function_scope && name) {
                    CIDecl *param_decl =
                      NEW_VARIANT(CIDecl,
                                  variable,
                                  CI_STORAGE_CLASS_NONE,
                                  false,
                                  NEW(CIDeclVariable,
                                      ref__CIDataType(data_type),
                                      name,
                                      NULL,
                                      true));

                    add_variable__CIResultFile(
                      self->file, parent_function_scope, param_decl);
                }
            }
        }

        if (self->current_token->kind != CI_TOKEN_KIND_RPAREN) {
            expect__CIParser(self, CI_TOKEN_KIND_COMMA, true);
        }
    }

    if (self->current_token->kind == CI_TOKEN_KIND_EOF) {
        FAILED("hit EOF");
    } else {
        next_token__CIParser(self); // skip `)`
    }

    return params;
}

CIGenericParams *
parse_generic_params__CIParser(CIParser *self)
{

    switch (self->current_token->kind) {
        case CI_TOKEN_KIND_DOT: {
            CIToken *peeked = peek_token__CIParser(self, 1);

            switch (peeked->kind) {
                case CI_TOKEN_KIND_LHOOK:
                    break;
                default:
                    return NULL;
            }

            jump__CIParser(self, 2);

            break;
        }
        default:
            return NULL;
    }

    Vec *params = NEW(Vec); // Vec<CIDataType*>*

    while (self->current_token->kind != CI_TOKEN_KIND_RHOOK &&
           self->current_token->kind != CI_TOKEN_KIND_EOF) {
        CIDataType *data_type = parse_data_type__CIParser(self);

        if (data_type) {
            push__Vec(params, data_type);
        }

        if (self->current_token->kind != CI_TOKEN_KIND_RHOOK) {
            expect__CIParser(self, CI_TOKEN_KIND_COMMA, true);
        }
    }

    if (params->len == 0) {
        FAILED("expected at least one generic param");
    }

    switch (self->current_token->kind) {
        case CI_TOKEN_KIND_RHOOK:
            next_token__CIParser(self);

            break;
        case CI_TOKEN_KIND_EOF:
            FAILED("unexpected EOF");
        default:
            UNREACHABLE("expected `]` or `EOF`");
    }

    return params ? NEW(CIGenericParams, params) : NULL;
}

CIExpr *
parse_function_call__CIParser(CIParser *self,
                              Rc *identifier,
                              CIGenericParams *generic_params)
{
    next_token__CIParser(self); // skip `(`

    Vec *params = NEW(Vec); // Vec<CIExpr*>*

    while (self->current_token->kind != CI_TOKEN_KIND_RPAREN &&
           self->current_token->kind != CI_TOKEN_KIND_EOF) {
        CIExpr *param = parse_expr__CIParser(self);

        if (param) {
            push__Vec(params, param);
        }

        if (self->current_token->kind != CI_TOKEN_KIND_RPAREN) {
            expect__CIParser(self, CI_TOKEN_KIND_COMMA, true);
        }
    }

    expect__CIParser(self, CI_TOKEN_KIND_RPAREN, true);

    if (is__CIBuiltinFunction(GET_PTR_RC(String, identifier))) {
        return NEW_VARIANT(
          CIExpr,
          function_call_builtin,
          NEW(CIExprFunctionCallBuiltin,
              get_id__CIBuiltinFunction(GET_PTR_RC(String, identifier)),
              params));
    }

    generate_function_gen__CIParser(
      self, GET_PTR_RC(String, identifier), generic_params);

    return NEW_VARIANT(
      CIExpr,
      function_call,
      NEW(CIExprFunctionCall, identifier, params, generic_params));
}

CIExpr *
parse_literal_expr__CIParser(CIParser *self)
{
    switch (self->previous_token->kind) {
        case CI_TOKEN_KIND_KEYWORD_TRUE:
            return NEW_VARIANT(
              CIExpr, literal, NEW_VARIANT(CIExprLiteral, bool, true));
        case CI_TOKEN_KIND_KEYWORD_FALSE:
            return NEW_VARIANT(
              CIExpr, literal, NEW_VARIANT(CIExprLiteral, bool, false));
        case CI_TOKEN_KIND_LITERAL_CONSTANT_INT:
        case CI_TOKEN_KIND_LITERAL_CONSTANT_OCTAL:
        case CI_TOKEN_KIND_LITERAL_CONSTANT_HEX:
        case CI_TOKEN_KIND_LITERAL_CONSTANT_BIN: {
            String *int_s = NULL;
            int base = 10;

            switch (self->previous_token->kind) {
                case CI_TOKEN_KIND_LITERAL_CONSTANT_INT:
                    int_s = self->previous_token->literal_constant_int.value;

                    break;
                case CI_TOKEN_KIND_LITERAL_CONSTANT_OCTAL:
                    int_s = self->previous_token->literal_constant_octal.value;
                    base = 8;

                    break;
                case CI_TOKEN_KIND_LITERAL_CONSTANT_HEX:
                    int_s = self->previous_token->literal_constant_hex.value;
                    base = 16;

                    break;
                case CI_TOKEN_KIND_LITERAL_CONSTANT_BIN:
                    int_s = self->previous_token->literal_constant_bin.value;
                    base = 2;

                    break;
                default:
                    UNREACHABLE("this kind of integer is not expected");
            }

            Optional *res_op = atoi_safe__Isize(int_s->buffer, base);

            if (is_none__Optional(res_op)) {
                FREE(Optional, res_op);

                FAILED("bad integer");
            }

            Isize res = (Isize)(Uptr)get__Optional(res_op);

            FREE(Optional, res_op);

            return NEW_VARIANT(
              CIExpr, literal, NEW_VARIANT(CIExprLiteral, signed_int, res));
        }
        case CI_TOKEN_KIND_LITERAL_CONSTANT_FLOAT:
            // TODO: Check if the float is overflow/underflow.
            // Update `base/atof.h` header.
            return NEW_VARIANT(
              CIExpr,
              literal,
              NEW_VARIANT(
                CIExprLiteral,
                float,
                atof__Float64(
                  self->previous_token->literal_constant_float.value->buffer)));
        case CI_TOKEN_KIND_LITERAL_CONSTANT_CHARACTER:
            return NEW_VARIANT(
              CIExpr,
              literal,
              NEW_VARIANT(CIExprLiteral,
                          char,
                          self->previous_token->literal_constant_character));
        case CI_TOKEN_KIND_LITERAL_CONSTANT_STRING:
            return NEW_VARIANT(
              CIExpr,
              literal,
              NEW_VARIANT(CIExprLiteral,
                          string,
                          self->previous_token->literal_constant_string));
        default:
            UNREACHABLE("unexpected token");
    }
}

CIExpr *
parse_struct_call__CIParser(CIParser *self)
{
    Vec *fields = NEW(Vec);

    next_token__CIParser(self); // skip `{`

    while (self->current_token->kind != CI_TOKEN_KIND_RBRACE &&
           self->current_token->kind != CI_TOKEN_KIND_EOF) {
        Vec *path = NEW(Vec);

        while (self->current_token->kind == CI_TOKEN_KIND_DOT) {
            next_token__CIParser(self);

            if (expect__CIParser(self, CI_TOKEN_KIND_IDENTIFIER, true)) {
                push__Vec(path, ref__Rc(self->previous_token->identifier));
            }
        }

        expect__CIParser(self, CI_TOKEN_KIND_EQ, true);

        CIExpr *value = parse_expr__CIParser(self);

        if (self->current_token->kind != CI_TOKEN_KIND_RBRACE) {
            expect__CIParser(self, CI_TOKEN_KIND_COMMA, true);
        }

        if (!value) {
            FREE(Vec, path);

            continue;
        }

        push__Vec(fields, NEW(CIExprStructFieldCall, path, value));
    }

    expect__CIParser(self, CI_TOKEN_KIND_RBRACE, true);

    return NEW_VARIANT(CIExpr, struct_call, NEW(CIExprStructCall, fields));
}

CIExpr *
parse_array__CIParser(CIParser *self)
{
    next_token__CIParser(self); // skip `{`

    Vec *array = NEW(Vec);

    while (self->current_token->kind != CI_TOKEN_KIND_RBRACE &&
           self->current_token->kind != CI_TOKEN_KIND_EOF) {
        CIExpr *expr = parse_expr__CIParser(self);

        if (expr) {
            push__Vec(array, expr);
        }

        if (self->current_token->kind != CI_TOKEN_KIND_RBRACE) {
            expect__CIParser(self, CI_TOKEN_KIND_COMMA, true);
        }
    }

    expect__CIParser(self, CI_TOKEN_KIND_RBRACE, true);

    return NEW_VARIANT(CIExpr, array, NEW(CIExprArray, array));
}

CIExpr *
parse_primary_expr__CIParser(CIParser *self)
{
    if (data_type_as_expression) {
        CIDataType *data_type = data_type_as_expression;

        data_type_as_expression = NULL;

        return NEW_VARIANT(CIExpr, data_type, data_type);
    } else if (is_data_type__CIParser(self)) {
        CIDataType *data_type = parse_data_type__CIParser(self);

        return NEW_VARIANT(CIExpr, data_type, data_type);
    }

    next_token__CIParser(self);

    CIExpr *res = NULL;

    switch (self->previous_token->kind) {
        case CI_TOKEN_KIND_KEYWORD_ALIGNOF: {
            CIExpr *alignof_expr = parse_expr__CIParser(self);

            if (alignof_expr) {
                res = NEW_VARIANT(CIExpr, alignof, alignof_expr);
                break;
            }

            return NULL;
        }
        case CI_TOKEN_KIND_LPAREN: {
            if (is_data_type__CIParser(self)) {
                CIDataType *data_type = parse_data_type__CIParser(self);

                expect__CIParser(self, CI_TOKEN_KIND_RPAREN, true);

                CIExpr *expr = parse_expr__CIParser(self);

                if (!data_type || !expr) {
                    if (data_type) {
                        FREE(CIDataType, data_type);
                    } else if (expr) {
                        FREE(CIExpr, expr);
                    }

                    return NULL;
                }

                check_for_initialization_expr__CIParser(
                  (CIParser *)self, data_type, expr);

                res =
                  NEW_VARIANT(CIExpr, cast, NEW(CIExprCast, data_type, expr));

                break;
            }

            CIExpr *expr = parse_expr__CIParser(self);

            expect__CIParser(self, CI_TOKEN_KIND_RPAREN, true);

            res = NEW_VARIANT(CIExpr, grouping, expr);

            break;
        }
        case CI_TOKEN_KIND_KEYWORD_SIZEOF: {
            bool has_open_paren = false;

            switch (self->current_token->kind) {
                case CI_TOKEN_KIND_LPAREN:
                    has_open_paren = true;

                    next_token__CIParser(self);

                    break;
                default:
                    break;
            }

            CIExpr *sizeof_expr = parse_expr__CIParser(self);

            if (has_open_paren) {
                expect__CIParser(self, CI_TOKEN_KIND_RPAREN, true);
            }

            if (sizeof_expr) {
                res = NEW_VARIANT(CIExpr, sizeof, sizeof_expr);

                break;
            }

            return NULL;
        }
        case CI_TOKEN_KIND_IDENTIFIER: {
            Rc *identifier = self->previous_token->identifier;
            CIGenericParams *generic_params =
              parse_generic_params__CIParser(self); // CIGenericParams*?

            switch (self->current_token->kind) {
                case CI_TOKEN_KIND_LPAREN:
                    res = parse_function_call__CIParser(
                      self, identifier, generic_params);

                    break;
                case CI_TOKEN_KIND_LBRACE:
                    TODO("parse struct call");
                default:
                    res = NEW_VARIANT(CIExpr, identifier, identifier);
            }

            break;
        }
        case CI_TOKEN_KIND_KEYWORD_TRUE:
        case CI_TOKEN_KIND_KEYWORD_FALSE:
        case CI_TOKEN_KIND_LITERAL_CONSTANT_INT:
        case CI_TOKEN_KIND_LITERAL_CONSTANT_FLOAT:
        case CI_TOKEN_KIND_LITERAL_CONSTANT_OCTAL:
        case CI_TOKEN_KIND_LITERAL_CONSTANT_HEX:
        case CI_TOKEN_KIND_LITERAL_CONSTANT_BIN:
        case CI_TOKEN_KIND_LITERAL_CONSTANT_CHARACTER:
        case CI_TOKEN_KIND_LITERAL_CONSTANT_STRING:
            res = parse_literal_expr__CIParser(self);

            break;
        case CI_TOKEN_KIND_BANG:
        case CI_TOKEN_KIND_AMPERSAND:
        case CI_TOKEN_KIND_MINUS:
        case CI_TOKEN_KIND_PLUS:
        case CI_TOKEN_KIND_WAVE:
        case CI_TOKEN_KIND_STAR:
        case CI_TOKEN_KIND_PLUS_PLUS:
        case CI_TOKEN_KIND_MINUS_MINUS: {
            enum CITokenKind unary_token_kind = self->previous_token->kind;
            CIExpr *expr = parse_primary_expr__CIParser(self);

            if (!expr) {
                return NULL;
            }

            switch (unary_token_kind) {
                case CI_TOKEN_KIND_BANG:
                    res = NEW_VARIANT(
                      CIExpr,
                      unary,
                      NEW(CIExprUnary, CI_EXPR_UNARY_KIND_NOT, expr));

                    break;
                case CI_TOKEN_KIND_AMPERSAND:
                    res = NEW_VARIANT(
                      CIExpr,
                      unary,
                      NEW(CIExprUnary, CI_EXPR_UNARY_KIND_REF, expr));

                    break;
                case CI_TOKEN_KIND_MINUS:
                    res = NEW_VARIANT(
                      CIExpr,
                      unary,
                      NEW(CIExprUnary, CI_EXPR_UNARY_KIND_NEGATIVE, expr));

                    break;
                case CI_TOKEN_KIND_PLUS:
                    res = NEW_VARIANT(
                      CIExpr,
                      unary,
                      NEW(CIExprUnary, CI_EXPR_UNARY_KIND_POSITIVE, expr));

                    break;
                case CI_TOKEN_KIND_WAVE:
                    res = NEW_VARIANT(
                      CIExpr,
                      unary,
                      NEW(CIExprUnary, CI_EXPR_UNARY_KIND_BIT_NOT, expr));

                    break;
                case CI_TOKEN_KIND_STAR:
                    res = NEW_VARIANT(
                      CIExpr,
                      unary,
                      NEW(CIExprUnary, CI_EXPR_UNARY_KIND_DEREFERENCE, expr));

                    break;
                case CI_TOKEN_KIND_PLUS_PLUS:
                    res = NEW_VARIANT(
                      CIExpr,
                      unary,
                      NEW(CIExprUnary, CI_EXPR_UNARY_KIND_PRE_INCREMENT, expr));

                    break;
                case CI_TOKEN_KIND_MINUS_MINUS:
                    res = NEW_VARIANT(
                      CIExpr,
                      unary,
                      NEW(CIExprUnary, CI_EXPR_UNARY_KIND_PRE_DECREMENT, expr));

                    break;
                default:
                    UNREACHABLE("unknown variant");
            }

            break;
        }
        case CI_TOKEN_KIND_KEYWORD_NULLPTR:
            return NEW(CIExpr, CI_EXPR_KIND_NULLPTR);
        case CI_TOKEN_KIND_BUILTIN_MACRO___HAS_FEATURE: {
            bool has_feature = false;

            switch (self->previous_token->has_feature) {
                case CI_EXTENSIONS_HAS_FEATURE_C_ALIGNAS:
                case CI_EXTENSIONS_HAS_FEATURE_C_ALIGNOF:
                case CI_EXTENSIONS_HAS_FEATURE_C_ATOMIC:
                case CI_EXTENSIONS_HAS_FEATURE_C_GENERIC_SELECTIONS:
                case CI_EXTENSIONS_HAS_FEATURE_C_GENERIC_SELECTION_WITH_CONTROLLING_TYPE:
                case CI_EXTENSIONS_HAS_FEATURE_C_STATIC_ASSERT:
                case CI_EXTENSIONS_HAS_FEATURE_C_THREAD_LOCAL:
                    if (self->file->entity.result->config->standard ==
                        CI_STANDARD_11) {
                        has_feature = true;
                    }

                    break;
                case CI_EXTENSIONS_HAS_FEATURE_ADDRESS_SANITIZER:
                    if (self->file->entity.result->config->compiler.kind ==
                        CI_PROJECT_CONFIG_COMPILER_KIND_CLANG) {
                        has_feature = true;
                    }

                    break;
                case CI_EXTENSIONS_HAS_FEATURE_MODULES:
                    break;
                default:
                    UNREACHABLE("unknown variant");
            }

            res = NEW_VARIANT(
              CIExpr, literal, NEW_VARIANT(CIExprLiteral, bool, has_feature));

            break;
        }
        // NOTE: Standard predefined macro cannot be redefined outside of
        // builtin file.
        case CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO___DATE__: {
            Rc *date_rc =
              NEW(Rc,
                  clone__String(
                    self->previous_token->standard_predefined_macro___date__));

            res = NEW_VARIANT(
              CIExpr, literal, NEW_VARIANT(CIExprLiteral, string, date_rc));

            FREE_RC(String, date_rc);

            break;
        }
        case CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO___FILE__: {
            Rc *file_rc = NEW(
              Rc, from__String(self->file->file_input.name)); // Rc<String*>*

            res = NEW_VARIANT(
              CIExpr, literal, NEW_VARIANT(CIExprLiteral, string, file_rc));

            FREE_RC(String, file_rc);

            break;
        }
        case CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO___LINE__:
            res = NEW_VARIANT(CIExpr,
                              literal,
                              NEW_VARIANT(CIExprLiteral,
                                          unsigned_int,
                                          self->previous_span.line));

            break;
        case CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO___TIME__: {
            Rc *time_rc =
              NEW(Rc,
                  clone__String(
                    self->previous_token
                      ->standard_predefined_macro___time__)); // Rc<String*>*

            res = NEW_VARIANT(
              CIExpr, literal, NEW_VARIANT(CIExprLiteral, string, time_rc));

            FREE_RC(String, time_rc);

            break;
        }
        default:
            FAILED("unexpected token");
    }

    return parse_post_expr__CIParser(self, res);
}

CIExpr *
parse_binary_expr__CIParser(CIParser *self, CIExpr *expr)
{
    Vec *stack = NEW(Vec); // Vec<CIExpr* | enum CIExprBinaryKind*>*
    Usize last_precedence = to_precedence__CIExpr(expr);

    push__Vec(stack, expr);

    while (self->current_token->kind == CI_TOKEN_KIND_EQ ||
           self->current_token->kind == CI_TOKEN_KIND_PLUS_EQ ||
           self->current_token->kind == CI_TOKEN_KIND_MINUS_EQ ||
           self->current_token->kind == CI_TOKEN_KIND_STAR_EQ ||
           self->current_token->kind == CI_TOKEN_KIND_SLASH_EQ ||
           self->current_token->kind == CI_TOKEN_KIND_PERCENTAGE_EQ ||
           self->current_token->kind == CI_TOKEN_KIND_AMPERSAND_EQ ||
           self->current_token->kind == CI_TOKEN_KIND_BAR_EQ ||
           self->current_token->kind == CI_TOKEN_KIND_HAT_EQ ||
           self->current_token->kind == CI_TOKEN_KIND_LSHIFT_LSHIFT_EQ ||
           self->current_token->kind == CI_TOKEN_KIND_RSHIFT_RSHIFT_EQ ||
           self->current_token->kind == CI_TOKEN_KIND_PLUS ||
           self->current_token->kind == CI_TOKEN_KIND_MINUS ||
           self->current_token->kind == CI_TOKEN_KIND_STAR ||
           self->current_token->kind == CI_TOKEN_KIND_SLASH ||
           self->current_token->kind == CI_TOKEN_KIND_PERCENTAGE ||
           self->current_token->kind == CI_TOKEN_KIND_AMPERSAND ||
           self->current_token->kind == CI_TOKEN_KIND_BAR ||
           self->current_token->kind == CI_TOKEN_KIND_HAT ||
           self->current_token->kind == CI_TOKEN_KIND_LSHIFT_LSHIFT ||
           self->current_token->kind == CI_TOKEN_KIND_RSHIFT_RSHIFT ||
           self->current_token->kind == CI_TOKEN_KIND_AMPERSAND_AMPERSAND ||
           self->current_token->kind == CI_TOKEN_KIND_BAR_BAR ||
           self->current_token->kind == CI_TOKEN_KIND_EQ_EQ ||
           self->current_token->kind == CI_TOKEN_KIND_BANG_EQ ||
           self->current_token->kind == CI_TOKEN_KIND_LSHIFT ||
           self->current_token->kind == CI_TOKEN_KIND_RSHIFT ||
           self->current_token->kind == CI_TOKEN_KIND_LSHIFT_EQ ||
           self->current_token->kind == CI_TOKEN_KIND_RSHIFT_EQ) {
        enum CIExprBinaryKind op =
          from_token__CIExprBinaryKind(self->current_token);
        Usize precedence = to_precedence__CIExprBinaryKind(op);

        next_token__CIParser(self);

        CIExpr *right = parse_primary_expr__CIParser(self);

        if (!right) {
            for (Usize i = 0; i < stack->len; i += 2) {
                FREE(CIExpr, get__Vec(stack, i));
            }

            FREE(Vec, stack);

            return NULL;
        }

        while (precedence <= last_precedence && stack->len > 1) {
            CIExpr *top_right = pop__Vec(stack);
            enum CIExprBinaryKind top_op =
              (enum CIExprBinaryKind)(Uptr)pop__Vec(stack);

            last_precedence = to_precedence__CIExprBinaryKind(top_op);

            if (last_precedence < precedence) {
                push__Vec(stack, (int *)top_op);
                push__Vec(stack, top_right);

                break;
            }

            CIExpr *top_left = pop__Vec(stack);

            push__Vec(
              stack,
              NEW_VARIANT(CIExpr,
                          binary,
                          NEW(CIExprBinary, top_op, top_left, top_right)));
        }

        push__Vec(stack, (int *)op);
        push__Vec(stack, right);

        last_precedence = precedence;
    }

    while (stack->len > 1) {
        CIExpr *rhs = pop__Vec(stack);
        enum CIExprBinaryKind op = (enum CIExprBinaryKind)(Uptr)pop__Vec(stack);
        CIExpr *lhs = pop__Vec(stack);

        push__Vec(stack,
                  NEW_VARIANT(CIExpr, binary, NEW(CIExprBinary, op, lhs, rhs)));
    }

    CIExpr *res = pop__Vec(stack);

    FREE(Vec, stack);

    return res;
}

CIExpr *
parse_post_expr__CIParser(CIParser *self, CIExpr *expr)
{
    if (!expr) {
        return NULL;
    }

loop:
    switch (self->current_token->kind) {
        case CI_TOKEN_KIND_PLUS_PLUS:
            next_token__CIParser(self);

            expr = NEW_VARIANT(
              CIExpr,
              unary,
              NEW(CIExprUnary, CI_EXPR_UNARY_KIND_POST_INCREMENT, expr));

            goto loop;
        case CI_TOKEN_KIND_MINUS_MINUS:
            next_token__CIParser(self);

            expr = NEW_VARIANT(
              CIExpr,
              unary,
              NEW(CIExprUnary, CI_EXPR_UNARY_KIND_POST_DECREMENT, expr));

            goto loop;
        // Parse array access
        case CI_TOKEN_KIND_LHOOK: {
            next_token__CIParser(self);

            CIExpr *access = parse_expr__CIParser(self);

            expect__CIParser(self, CI_TOKEN_KIND_RHOOK, true);

            expr = NEW_VARIANT(
              CIExpr, array_access, NEW(CIExprArrayAccess, expr, access));

            goto loop;
        }
        case CI_TOKEN_KIND_DOT:
            next_token__CIParser(self);

            expr = NEW_VARIANT(CIExpr,
                               binary,
                               NEW(CIExprBinary,
                                   CI_EXPR_BINARY_KIND_DOT,
                                   expr,
                                   parse_primary_expr__CIParser(self)));

            goto loop;
        case CI_TOKEN_KIND_ARROW:
            next_token__CIParser(self);

            expr = NEW_VARIANT(CIExpr,
                               binary,
                               NEW(CIExprBinary,
                                   CI_EXPR_BINARY_KIND_ARROW,
                                   expr,
                                   parse_primary_expr__CIParser(self)));

            goto loop;
        default:
            return expr;
    }
}

CIExpr *
parse_expr__CIParser(CIParser *self)
{
    switch (self->current_token->kind) {
        case CI_TOKEN_KIND_LBRACE: {
            if (!allow_initialization) {
                FAILED("cannot declare array, struct call or union call "
                       "outside of variable initialization");
            }

            CIToken *peeked = peek_token__CIParser(self, 1);

            if (peeked && peeked->kind == CI_TOKEN_KIND_DOT) {
                return parse_struct_call__CIParser(self);
            } else {
                return parse_array__CIParser(self);
            }

            break;
        }
        default:
            break;
    }

    CIExpr *expr = parse_primary_expr__CIParser(self);

    if (!expr) {
        return NULL;
    }

    expr = parse_post_expr__CIParser(self, expr);

loop:
    switch (self->current_token->kind) {
        // Parse binary expression
        case CI_TOKEN_KIND_EQ:
        case CI_TOKEN_KIND_PLUS_EQ:
        case CI_TOKEN_KIND_MINUS_EQ:
        case CI_TOKEN_KIND_STAR_EQ:
        case CI_TOKEN_KIND_SLASH_EQ:
        case CI_TOKEN_KIND_PERCENTAGE_EQ:
        case CI_TOKEN_KIND_AMPERSAND_EQ:
        case CI_TOKEN_KIND_BAR_EQ:
        case CI_TOKEN_KIND_HAT_EQ:
        case CI_TOKEN_KIND_LSHIFT_LSHIFT_EQ:
        case CI_TOKEN_KIND_RSHIFT_RSHIFT_EQ:
        case CI_TOKEN_KIND_PLUS:
        case CI_TOKEN_KIND_MINUS:
        case CI_TOKEN_KIND_STAR:
        case CI_TOKEN_KIND_SLASH:
        case CI_TOKEN_KIND_PERCENTAGE:
        case CI_TOKEN_KIND_AMPERSAND:
        case CI_TOKEN_KIND_BAR:
        case CI_TOKEN_KIND_HAT:
        case CI_TOKEN_KIND_LSHIFT_LSHIFT:
        case CI_TOKEN_KIND_RSHIFT_RSHIFT:
        case CI_TOKEN_KIND_AMPERSAND_AMPERSAND:
        case CI_TOKEN_KIND_BAR_BAR:
        case CI_TOKEN_KIND_EQ_EQ:
        case CI_TOKEN_KIND_BANG_EQ:
        case CI_TOKEN_KIND_LSHIFT:
        case CI_TOKEN_KIND_RSHIFT:
        case CI_TOKEN_KIND_LSHIFT_EQ:
        case CI_TOKEN_KIND_RSHIFT_EQ:
            expr = parse_binary_expr__CIParser(self, expr);

            goto loop;
        case CI_TOKEN_KIND_INTERROGATION:
            next_token__CIParser(self); // skip `?`

            CIExpr *expr_if = parse_expr__CIParser(self);

            if (!expr_if) {
                return expr;
            }

            expect__CIParser(self, CI_TOKEN_KIND_COLON, true);

            CIExpr *expr_else = parse_expr__CIParser(self);

            if (!expr_else) {
                FREE(CIExpr, expr_if);

                return expr;
            }

            return NEW_VARIANT(
              CIExpr, ternary, NEW(CIExprTernary, expr, expr_if, expr_else));
        default:
            return parse_post_expr__CIParser(self, expr);
    }
}

CIDataType *
perform_implicit_cast_on_array__CIParser(const CIParser *self,
                                         CIDataType *data_type)
{
    switch (data_type->kind) {
        case CI_DATA_TYPE_KIND_ARRAY:
            return NEW_VARIANT(CIDataType,
                               ptr,
                               perform_implicit_cast_on_array__CIParser(
                                 self, data_type->array.data_type));
        default:
            return ref__CIDataType(data_type);
    }
}

CIDataType *
resolve_data_type_access__CIParser(const CIParser *self,
                                   CIDataType *data_type_access,
                                   enum CIExprBinaryKind binary_kind,
                                   const CIGenericParams *called_generic_params,
                                   const CIGenericParams *decl_generic_params)
{
    switch (binary_kind) {
        case CI_EXPR_BINARY_KIND_ARROW:
            if (!is_ptr_data_type__CIParser((CIParser *)self,
                                            data_type_access,
                                            called_generic_params,
                                            decl_generic_params) &&
                count_compatible_pointer_depth__CIParser((CIParser *)self,
                                                         data_type_access,
                                                         called_generic_params,
                                                         decl_generic_params) ==
                  1) {
                FAILED("expected pointer with a depth of one");
            }

            CIDataType *struct_or_union_dt =
              unwrap_implicit_ptr_data_type__CIParser((CIParser *)self,
                                                      data_type_access,
                                                      called_generic_params,
                                                      decl_generic_params);

            if (!is_struct_or_union_data_type__CIParser((CIParser *)self,
                                                        struct_or_union_dt,
                                                        called_generic_params,
                                                        decl_generic_params)) {
                FAILED("expected struct or union data type");
            }

            return struct_or_union_dt;
        case CI_EXPR_BINARY_KIND_DOT:
            if (!is_struct_or_union_data_type__CIParser((CIParser *)self,
                                                        data_type_access,
                                                        called_generic_params,
                                                        decl_generic_params)) {
                FAILED("expected struct or union data type");
            }

            return data_type_access;
        default:
            UNREACHABLE("expected arrow or dot binary expression");
    }
}

CIDataType *
infer_expr_access_data_type__CIParser(
  const CIParser *self,
  const CIExpr *expr_access,
  const Vec *fields,
  const CIScopeID *current_scope_id,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params)
{
    Vec *current_fields = (Vec *)fields;
    CIDataType *current_infer_dt = NULL;

    while (true) {
        switch (expr_access->kind) {
            case CI_EXPR_KIND_ARRAY_ACCESS: {
                CIDataType *array_dt = infer_expr_access_data_type__CIParser(
                  self,
                  expr_access->array_access.array,
                  fields,
                  current_scope_id,
                  called_generic_params,
                  decl_generic_params);

                if (current_infer_dt) {
                    FREE(CIDataType, current_infer_dt);
                }

                // NOTE: Lower the `array_dt` count before assigning it.
                ASSERT(array_dt->ref_count >= 1);
                FREE(CIDataType, array_dt);

                if (is_ptr_data_type__CIParser((CIParser *)self,
                                               array_dt,
                                               called_generic_params,
                                               decl_generic_params)) {
                    current_infer_dt = unwrap_implicit_ptr_data_type__CIParser(
                      (CIParser *)self,
                      array_dt,
                      called_generic_params,
                      decl_generic_params);
                } else {
                    current_infer_dt = NULL;
                    FAILED("expected array or pointer");
                }

                goto exit;
            }
            case CI_EXPR_KIND_BINARY:
                switch (expr_access->binary.kind) {
                    case CI_EXPR_BINARY_KIND_ARROW:
                    case CI_EXPR_BINARY_KIND_DOT: {
                        switch (expr_access->binary.left->kind) {
                            case CI_EXPR_KIND_IDENTIFIER: {
                                CIDataType *field_data_type =
                                  get__CIDeclStructField(
                                    current_fields,
                                    GET_PTR_RC(
                                      String,
                                      expr_access->binary.left->identifier));

                                if (!field_data_type) {
                                    FAILED("the field doesn't exist");
                                }

                                CIDataType *resolved_field_data_type =
                                  resolve_data_type__CIParser(
                                    self,
                                    field_data_type,
                                    called_generic_params,
                                    decl_generic_params);
                                CIDataType *resolved_access_data_type =
                                  resolve_data_type_access__CIParser(
                                    self,
                                    resolved_field_data_type,
                                    expr_access->binary.kind,
                                    called_generic_params,
                                    decl_generic_params);

                                FREE(CIDataType, resolved_field_data_type);

                                if (current_infer_dt) {
                                    FREE(CIDataType, current_infer_dt);
                                }

                                current_infer_dt = resolved_access_data_type;

                                if (is_struct_or_union_data_type__CIParser(
                                      (CIParser *)self,
                                      current_infer_dt,
                                      called_generic_params,
                                      decl_generic_params)) {
                                    current_fields = (Vec *)
                                      get_fields_from_data_type__CIParser(
                                        self,
                                        current_infer_dt,
                                        called_generic_params,
                                        decl_generic_params);

                                    continue;
                                }

                                continue;
                            }
                            default:
                                FAILED("expected identifier");
                        }

                        break;
                    }
                    default:
                        goto exit;
                }

                break;
            case CI_EXPR_KIND_IDENTIFIER:
                if (current_infer_dt) {
                    FREE(CIDataType, current_infer_dt);
                }

                current_infer_dt = get__CIDeclStructField(
                  current_fields, GET_PTR_RC(String, expr_access->identifier));

                goto exit;
            default:
                goto exit;
        }
    }

exit:
    if (!current_infer_dt) {
        FAILED("there are something wrong during the inference of path");
    }

    return ref__CIDataType(current_infer_dt);
}

CIDataType *
infer_expr_dot_binary_access_data_type__CIParser(
  const CIParser *self,
  const CIExprBinary *dot_access,
  const CIScopeID *current_scope_id,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params)
{
    CIDataType *left_dt = infer_expr_data_type__CIParser(self,
                                                         dot_access->left,
                                                         current_scope_id,
                                                         called_generic_params,
                                                         decl_generic_params);
    CIDataType *resolved_left_dt =
      resolve_data_type_access__CIParser(self,
                                         left_dt,
                                         CI_EXPR_BINARY_KIND_DOT,
                                         called_generic_params,
                                         decl_generic_params);
    CIDataType *struct_or_union_dt = resolve_data_type__CIParser(
      self, resolved_left_dt, called_generic_params, decl_generic_params);
    const Vec *struct_or_union_fields = get_fields_from_data_type__CIParser(
      self, struct_or_union_dt, called_generic_params, decl_generic_params);

    FREE(CIDataType, left_dt);
    FREE(CIDataType, struct_or_union_dt);

    return infer_expr_access_data_type__CIParser(self,
                                                 dot_access->right,
                                                 struct_or_union_fields,
                                                 current_scope_id,
                                                 called_generic_params,
                                                 decl_generic_params);
}

CIDataType *
infer_expr_arrow_binary_access_data_type__CIParser(
  const CIParser *self,
  const CIExprBinary *arrow_access,
  const CIScopeID *current_scope_id,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params)
{
    CIDataType *left_dt = infer_expr_data_type__CIParser(self,
                                                         arrow_access->left,
                                                         current_scope_id,
                                                         called_generic_params,
                                                         decl_generic_params);
    CIDataType *resolved_left_dt =
      resolve_data_type_access__CIParser(self,
                                         left_dt,
                                         CI_EXPR_BINARY_KIND_ARROW,
                                         called_generic_params,
                                         decl_generic_params);
    CIDataType *struct_or_union_dt = resolve_data_type__CIParser(
      self, resolved_left_dt, called_generic_params, decl_generic_params);
    const Vec *struct_or_union_fields = get_fields_from_data_type__CIParser(
      self, struct_or_union_dt, called_generic_params, decl_generic_params);

    FREE(CIDataType, left_dt);
    FREE(CIDataType, struct_or_union_dt);

    return infer_expr_access_data_type__CIParser(self,
                                                 arrow_access->right,
                                                 struct_or_union_fields,
                                                 current_scope_id,
                                                 called_generic_params,
                                                 decl_generic_params);
}

CIDataType *
infer_expr_literal_data_type__CIParser(const CIParser *self,
                                       const CIExprLiteral *literal)
{
    switch (literal->kind) {
        case CI_EXPR_LITERAL_KIND_BOOL:
            return NEW(CIDataType, CI_DATA_TYPE_KIND_BOOL);
        case CI_EXPR_LITERAL_KIND_CHAR:
            return NEW(CIDataType, CI_DATA_TYPE_KIND_CHAR);
        case CI_EXPR_LITERAL_KIND_FLOAT:
            return NEW(CIDataType, CI_DATA_TYPE_KIND_FLOAT);
        case CI_EXPR_LITERAL_KIND_SIGNED_INT:
            return NEW(CIDataType, CI_DATA_TYPE_KIND_INT);
        case CI_EXPR_LITERAL_KIND_STRING: {
            CIDataType *string_dt = NEW_VARIANT(
              CIDataType,
              array,
              NEW_VARIANT(CIDataTypeArray,
                          sized,
                          NEW(CIDataType, CI_DATA_TYPE_KIND_CHAR),
                          NULL,
                          GET_PTR_RC(String, literal->string)->len));

            set_context__CIDataType(string_dt, CI_DATA_TYPE_CONTEXT_STACK);

            return string_dt;
        }
        case CI_EXPR_LITERAL_KIND_UNSIGNED_INT:
            return NEW(CIDataType, CI_DATA_TYPE_KIND_UNSIGNED_INT);
        default:
            UNREACHABLE("unknown variant");
    }
}

CIDataType *
infer_expr_unary_data_type__CIParser(
  const CIParser *self,
  const CIExprUnary *unary,
  const CIScopeID *current_scope_id,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params)
{
    CIDataType *unary_right_expr_data_type =
      infer_expr_data_type__CIParser(self,
                                     unary->expr,
                                     current_scope_id,
                                     called_generic_params,
                                     decl_generic_params);

    switch (unary->kind) {
        case CI_EXPR_UNARY_KIND_REF:
            return wrap_ptr__CIDataType(unary_right_expr_data_type,
                                        CI_DATA_TYPE_CONTEXT_NONE);
        case CI_EXPR_UNARY_KIND_DEREFERENCE: {
            if (is_ptr_data_type__CIParser((CIParser *)self,
                                           unary_right_expr_data_type,
                                           called_generic_params,
                                           decl_generic_params)) {
                TODO("get the type after dereferencing");
            }

            return unary_right_expr_data_type;
        }
        default:
            return unary_right_expr_data_type;
    }
}

CIDataType *
infer_expr_data_type__CIParser(const CIParser *self,
                               const CIExpr *expr,
                               const CIScopeID *current_scope_id,
                               const CIGenericParams *called_generic_params,
                               const CIGenericParams *decl_generic_params)
{
    switch (expr->kind) {
        case CI_EXPR_KIND_ALIGNOF:
            // FIXME: Get the size_t data type for 32 bits.
            return NEW(CIDataType,
                       CI_DATA_TYPE_KIND_UNSIGNED_LONG_INT); // size_t
        case CI_EXPR_KIND_ARRAY: {
            if (expr->array.array->len == 0) {
                FAILED("cannot infer on array");
            }

            Vec *expr_dts = NEW(Vec); // Vec<CIDataType*>*
            bool is_struct = false;

            for (Usize i = 0; i < expr->array.array->len; ++i) {
                CIExpr *expr_item = get__Vec(expr->array.array, i);
                CIDataType *expr_item_dt =
                  infer_expr_data_type__CIParser(self,
                                                 expr_item,
                                                 current_scope_id,
                                                 called_generic_params,
                                                 decl_generic_params);
                CIDataType *last_dt = safe_last__Vec(expr_dts);

                if (last_dt && !is_struct) {
                    if (!eq__CIDataType(expr_item_dt, last_dt)) {
                        is_struct = true;
                    }
                }

                push__Vec(expr_dts, expr_item_dt);
            }

            if (is_struct) {
                Vec *fields = expr_dts;

                for (Usize i = 0; i < fields->len; ++i) {
                    CIDataType *expr_dt = get__Vec(fields, i);

                    replace__Vec(
                      fields, i, NEW(CIDeclStructField, NULL, expr_dt));
                }

                return NEW_VARIANT(CIDataType,
                                   struct,
                                   NEW(CIDataTypeStruct, NULL, NULL, fields));
            }

            CIDataType *array_data_type = get__Vec(expr_dts, 0);
            Usize expr_dts_len = expr_dts->len;

            // NOTE: We don't free the first item contains in this buffer.
            FREE_BUFFER_ITEMS(
              (expr_dts->buffer + 1), expr_dts->len - 1, CIDataType);
            FREE(Vec, expr_dts);

            return NEW_VARIANT(
              CIDataType,
              array,
              NEW_VARIANT(
                CIDataTypeArray, sized, array_data_type, NULL, expr_dts_len));
        }
        case CI_EXPR_KIND_ARRAY_ACCESS: {
            CIDataType *array_data_type =
              infer_expr_data_type__CIParser(self,
                                             expr->array_access.array,
                                             current_scope_id,
                                             called_generic_params,
                                             decl_generic_params);
            CIDataType *ptr_array_data_type =
              perform_implicit_cast_on_array__CIParser(self, array_data_type);

            switch (ptr_array_data_type->kind) {
                case CI_DATA_TYPE_KIND_PTR: {
                    CIDataType *res = ref__CIDataType(ptr_array_data_type->ptr);

                    FREE(CIDataType, ptr_array_data_type);
                    FREE(CIDataType, array_data_type);

                    return res;
                }
                default:
                    FAILED("expected pointer after implicit cast on array");
            }
        }
        case CI_EXPR_KIND_BINARY: {
            switch (expr->binary.kind) {
                case CI_EXPR_BINARY_KIND_DOT:
                    return infer_expr_dot_binary_access_data_type__CIParser(
                      self,
                      &expr->binary,
                      current_scope_id,
                      called_generic_params,
                      decl_generic_params);
                case CI_EXPR_BINARY_KIND_ARROW:
                    return infer_expr_arrow_binary_access_data_type__CIParser(
                      self,
                      &expr->binary,
                      current_scope_id,
                      called_generic_params,
                      decl_generic_params);
                default:
                    break;
            }

            CIDataType *left_dt =
              infer_expr_data_type__CIParser(self,
                                             expr->binary.left,
                                             current_scope_id,
                                             called_generic_params,
                                             decl_generic_params);

            switch (expr->binary.kind) {
                case CI_EXPR_BINARY_KIND_ASSIGN:
                    return left_dt;
                default:
                    break;
            }

            CIDataType *right_dt =
              infer_expr_data_type__CIParser(self,
                                             expr->binary.right,
                                             current_scope_id,
                                             called_generic_params,
                                             decl_generic_params);

            bool left_dt_is_integer =
              is_integer_data_type__CIParser((CIParser *)self,
                                             left_dt,
                                             true,
                                             called_generic_params,
                                             decl_generic_params);
            bool left_dt_is_float =
              is_float_data_type__CIParser((CIParser *)self,
                                           left_dt,
                                           called_generic_params,
                                           decl_generic_params);
            bool right_dt_is_integer =
              is_integer_data_type__CIParser((CIParser *)self,
                                             right_dt,
                                             true,
                                             called_generic_params,
                                             decl_generic_params);
            bool right_dt_is_float =
              is_float_data_type__CIParser((CIParser *)self,
                                           right_dt,
                                           called_generic_params,
                                           decl_generic_params);

            FREE(CIDataType, left_dt);
            FREE(CIDataType, right_dt);

            if (left_dt_is_float || right_dt_is_float) {
                return NEW(CIDataType, CI_DATA_TYPE_KIND_FLOAT);
            } else if (left_dt_is_integer && right_dt_is_integer) {
                return NEW(CIDataType, CI_DATA_TYPE_KIND_INT);
            }

            FAILED("this kind of operation is not possible");
        }
        case CI_EXPR_KIND_CAST:
            return ref__CIDataType(expr->cast.data_type);
        case CI_EXPR_KIND_DATA_TYPE:
            return NEW(CIDataType, CI_DATA_TYPE_KIND_TYPE_INFO);
        case CI_EXPR_KIND_FUNCTION_CALL: {
            CIDecl *function_decl =
              search_function_in_generic_context__CIParser(
                self,
                GET_PTR_RC(String, expr->function_call.identifier),
                expr->function_call.generic_params,
                called_generic_params,
                decl_generic_params);

            ASSERT(function_decl->kind & CI_DECL_KIND_FUNCTION);

            if (function_decl) {
                return ref__CIDataType(
                  (CIDataType *)get_return_data_type__CIDecl(function_decl));
            }

            return NEW(CIDataType, CI_DATA_TYPE_KIND_INT);
        }
        case CI_EXPR_KIND_FUNCTION_CALL_BUILTIN: {
            CIBuiltin *builtin = get_ref__CIBuiltin();
            const CIBuiltinFunction *builtin_function =
              get_builtin_function__CIBuiltin(builtin,
                                              expr->function_call_builtin.id);

            return ref__CIDataType(builtin_function->return_data_type);
        }
        case CI_EXPR_KIND_GROUPING:
            return infer_expr_data_type__CIParser(self,
                                                  expr->grouping,
                                                  current_scope_id,
                                                  called_generic_params,
                                                  decl_generic_params);
        case CI_EXPR_KIND_IDENTIFIER: {
            CIScope *local_current_scope =
              get_scope_from_id__CIResultFile(self->file, current_scope_id);

            ASSERT(local_current_scope);

            CIDecl *decl = search_identifier__CIResultFile(
              self->file,
              local_current_scope,
              GET_PTR_RC(String, expr->identifier));

            if (decl) {
                switch (decl->kind) {
                    case CI_DECL_KIND_FUNCTION: {
                        // TODO: Call generic function is not yet implemented
                        CIDataType *function_return_data_type =
                          (CIDataType *)get_return_data_type__CIDecl(decl);
                        CIDataType *resolved_function_return_data_type =
                          resolve_data_type__CIParser(self,
                                                      function_return_data_type,
                                                      called_generic_params,
                                                      decl_generic_params);
                        const Vec *function_params =
                          get_function_params__CIDecl(decl);
                        // NOTE: Maybe use ref count instead of.
                        Vec *cloned_function_params =
                          function_params
                            ? clone_params__CIDeclFunctionParam(function_params)
                            : NULL;

                        return NEW_VARIANT(
                          CIDataType,
                          function,
                          NEW(CIDataTypeFunction,
                              NULL,
                              cloned_function_params,
                              resolved_function_return_data_type,
                              NULL));
                    }
                    case CI_DECL_KIND_VARIABLE:
                        return ref__CIDataType(decl->variable.data_type);
                    default:
                        UNREACHABLE("this declaration is not expected");
                }
            }

            FAILED("cannot infer on unknown identifier");
        }
        case CI_EXPR_KIND_LITERAL:
            return infer_expr_literal_data_type__CIParser(self, &expr->literal);
        case CI_EXPR_KIND_NULLPTR:
            TODO("infer nullptr");
        case CI_EXPR_KIND_SIZEOF:
            // FIXME: Get the size_t data type for 32 bits.
            return NEW(CIDataType,
                       CI_DATA_TYPE_KIND_UNSIGNED_LONG_INT); // size_t
        case CI_EXPR_KIND_STRUCT_CALL: {
            Vec *fields = NEW(Vec); // Vec<CIDeclStructField*>*

            for (Usize i = 0; i < expr->struct_call.fields->len; ++i) {
                CIExprStructFieldCall *field =
                  get__Vec(expr->struct_call.fields, i);

                push__Vec(
                  fields,
                  NEW(CIDeclStructField,
                      NULL,
                      infer_expr_data_type__CIParser(self,
                                                     field->value,
                                                     current_scope_id,
                                                     called_generic_params,
                                                     decl_generic_params)));
            }

            return NEW_VARIANT(
              CIDataType, struct, NEW(CIDataTypeStruct, NULL, NULL, fields));
        }
        case CI_EXPR_KIND_TERNARY:
            // NOTE: We only need to infer one condition branch.
            return infer_expr_data_type__CIParser(self,
                                                  expr->ternary.if_,
                                                  current_scope_id,
                                                  called_generic_params,
                                                  decl_generic_params);
        case CI_EXPR_KIND_UNARY:
            return infer_expr_unary_data_type__CIParser(self,
                                                        &expr->unary,
                                                        current_scope_id,
                                                        called_generic_params,
                                                        decl_generic_params);
        default:
            UNREACHABLE("unknown variant");
    }
}

CIDecl *
search_decl__CIParser(const CIParser *self,
                      const String *name,
                      CIGenericParams *called_generic_params,
                      CIDecl *(*search_decl)(const CIResultFile *self,
                                             const String *name))
{
    CIDecl *base_decl = search_decl(self->file, name);

    if (!base_decl) {
        FAILED("declaration is not found");
    }

    if (called_generic_params) {
        ASSERT(!has_generic__CIGenericParams(called_generic_params));

        String *serialized_name =
          serialize_name__CIGenericParams(called_generic_params, name);
        CIDecl *res = search_decl(self->file, serialized_name);

        FREE(String, serialized_name);

        return res;
    }

    return base_decl;
}

CIDecl *
search_enum__CIParser(const CIParser *self, const String *name)
{
    return search_decl__CIParser(self, name, NULL, &search_enum__CIResultFile);
}

CIDecl *
search_function__CIParser(const CIParser *self,
                          const String *name,
                          CIGenericParams *called_generic_params)
{
    return search_decl__CIParser(
      self, name, called_generic_params, &search_function__CIResultFile);
}

CIDecl *
search_struct__CIParser(const CIParser *self,
                        const String *name,
                        CIGenericParams *called_generic_params)
{
    return search_decl__CIParser(
      self, name, called_generic_params, &search_struct__CIResultFile);
}

CIDecl *
search_typedef__CIParser(const CIParser *self,
                         const String *name,
                         CIGenericParams *called_generic_params)
{
    return search_decl__CIParser(
      self, name, called_generic_params, &search_typedef__CIResultFile);
}

CIDecl *
search_union__CIParser(const CIParser *self,
                       const String *name,
                       CIGenericParams *called_generic_params)
{
    return search_decl__CIParser(
      self, name, called_generic_params, &search_union__CIResultFile);
}

CIDecl *
search_decl_in_generic_context__CIParser(
  const CIParser *self,
  const String *name,
  CIGenericParams *gen_decl_generic_params,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params,
  CIDecl *(*search_decl)(const CIResultFile *self, const String *name),
  void (*generate_decl_gen)(CIParser *self,
                            String *name,
                            CIGenericParams *called_generic_params))
{
    CIDecl *base_decl = search_decl(self->file, name);

    if (!base_decl) {
        FAILED("declaration is not found");
    }

    if (gen_decl_generic_params &&
        ((called_generic_params && decl_generic_params) ||
         !has_generic__CIGenericParams(gen_decl_generic_params))) {
        CIGenericParams *substituted_generic_params =
          substitute_generic_params__CIParser(gen_decl_generic_params,
                                              decl_generic_params,
                                              called_generic_params);

        ASSERT(substituted_generic_params);

        String *serialized_name =
          serialize_name__CIGenericParams(substituted_generic_params, name);
        CIDecl *decl = search_decl(self->file, serialized_name);

        if (!decl) {
            generate_decl_gen((CIParser *)self,
                              (String *)name,
                              (CIGenericParams *)called_generic_params);

            decl = search_decl(self->file, serialized_name);
        }

        ASSERT(decl);
        ASSERT(decl->kind & CI_DECL_KIND_GEN);

        FREE(String, serialized_name);
        FREE(CIGenericParams, substituted_generic_params);

        return decl;
    }

    return base_decl;
}

CIDecl *
search_function_in_generic_context__CIParser(
  const CIParser *self,
  const String *name,
  CIGenericParams *function_generic_params,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params)
{
    return search_decl_in_generic_context__CIParser(
      self,
      name,
      function_generic_params,
      called_generic_params,
      decl_generic_params,
      &search_function__CIResultFile,
      &generate_function_gen__CIParser);
}

CIDecl *
search_struct_in_generic_context__CIParser(
  const CIParser *self,
  const String *name,
  CIGenericParams *struct_generic_params,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params)
{
    return search_decl_in_generic_context__CIParser(
      self,
      name,
      struct_generic_params,
      called_generic_params,
      decl_generic_params,
      &search_struct__CIResultFile,
      &generate_struct_gen__CIParser);
}

CIDecl *
search_typedef_in_generic_context__CIParser(
  const CIParser *self,
  const String *name,
  CIGenericParams *typedef_generic_params,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params)
{
    return search_decl_in_generic_context__CIParser(
      self,
      name,
      typedef_generic_params,
      called_generic_params,
      decl_generic_params,
      &search_typedef__CIResultFile,
      &generate_typedef_gen__CIParser);
}

CIDecl *
search_union_in_generic_context__CIParser(
  const CIParser *self,
  const String *name,
  CIGenericParams *union_generic_params,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params)
{
    return search_decl_in_generic_context__CIParser(
      self,
      name,
      union_generic_params,
      called_generic_params,
      decl_generic_params,
      &search_union__CIResultFile,
      &generate_union_gen__CIParser);
}

CIDataType *
resolve_generic_data_type__CIParser(
  const CIParser *self,
  CIDataType *data_type,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params)
{
    bool has_generic = called_generic_params && decl_generic_params;

    if (has_generic) {
        Isize generic_params_index = find_generic__CIGenericParams(
          decl_generic_params, GET_PTR_RC(String, data_type->generic));

        if (generic_params_index == -1) {
            FAILED("generic params is not found");
        }

        return ref__CIDataType(
          get__Vec(called_generic_params->params, generic_params_index));
    }

    FAILED("expected generic param, to use generic data type");
}

CIDataType *
resolve_struct_data_type__CIParser(const CIParser *self,
                                   CIDataType *data_type,
                                   const CIGenericParams *called_generic_params,
                                   const CIGenericParams *decl_generic_params)
{
    if (data_type->struct_.name && data_type->struct_.generic_params) {
        CIDecl *struct_decl = search_struct_in_generic_context__CIParser(
          self,
          GET_PTR_RC(String, data_type->struct_.name),
          data_type->struct_.generic_params,
          called_generic_params,
          decl_generic_params);

        if (!struct_decl) {
            FAILED("struct is not found");
        }

        return NEW_VARIANT(
          CIDataType,
          struct,
          NEW(CIDataTypeStruct,
              NEW(Rc, clone__String(struct_decl->struct_gen.name)),
              NULL,
              NULL));
    }

    return ref__CIDataType(data_type);
}

CIDataType *
resolve_typedef_data_type__CIParser(
  const CIParser *self,
  CIDataType *data_type,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params)
{
    switch (data_type->kind) {
        case CI_DATA_TYPE_KIND_TYPEDEF: {
            CIDecl *typedef_decl = search_typedef_in_generic_context__CIParser(
              self,
              GET_PTR_RC(String, data_type->typedef_.name),
              data_type->typedef_.generic_params,
              called_generic_params,
              decl_generic_params);

            switch (typedef_decl->kind) {
                case CI_DECL_KIND_TYPEDEF:
                    return resolve_typedef_data_type__CIParser(
                      self,
                      typedef_decl->typedef_.data_type,
                      called_generic_params,
                      decl_generic_params);
                case CI_DECL_KIND_TYPEDEF_GEN:
                    return resolve_typedef_data_type__CIParser(
                      self,
                      typedef_decl->typedef_gen.data_type,
                      called_generic_params,
                      decl_generic_params);
                default:
                    UNREACHABLE("expected typedef");
            }
        }
        default:
            return ref__CIDataType(data_type);
    }
}

CIDataType *
resolve_union_data_type__CIParser(const CIParser *self,
                                  CIDataType *data_type,
                                  const CIGenericParams *called_generic_params,
                                  const CIGenericParams *decl_generic_params)
{
    if (data_type->union_.name && data_type->union_.generic_params) {
        CIDecl *union_decl = search_union_in_generic_context__CIParser(
          self,
          GET_PTR_RC(String, data_type->union_.name),
          data_type->union_.generic_params,
          called_generic_params,
          decl_generic_params);

        if (!union_decl) {
            FAILED("union declaration is not found");
        }

        return NEW_VARIANT(
          CIDataType,
          union,
          NEW(CIDataTypeUnion,
              NEW(Rc, clone__String(union_decl->union_gen.name)),
              NULL,
              NULL));
    }

    return ref__CIDataType(data_type);
}

CIDataType *
resolve_data_type__CIParser(const CIParser *self,
                            CIDataType *data_type,
                            const CIGenericParams *called_generic_params,
                            const CIGenericParams *decl_generic_params)
{
    switch (data_type->kind) {
        case CI_DATA_TYPE_KIND_GENERIC:
            return resolve_generic_data_type__CIParser(
              self, data_type, called_generic_params, decl_generic_params);
        case CI_DATA_TYPE_KIND_STRUCT:
            return resolve_struct_data_type__CIParser(
              self, data_type, called_generic_params, decl_generic_params);
        case CI_DATA_TYPE_KIND_TYPEDEF:
            return resolve_typedef_data_type__CIParser(
              self, data_type, called_generic_params, decl_generic_params);
        case CI_DATA_TYPE_KIND_UNION:
            return resolve_union_data_type__CIParser(
              self, data_type, called_generic_params, decl_generic_params);
        case CI_DATA_TYPE_KIND_PTR:
            return NEW_VARIANT(
              CIDataType,
              ptr,
              resolve_data_type__CIParser(self,
                                          data_type->ptr,
                                          called_generic_params,
                                          decl_generic_params));
        default:
            return ref__CIDataType(data_type);
    }
}

const Vec *
get_fields_from_data_type__CIParser(
  const CIParser *self,
  const CIDataType *data_type,
  const CIGenericParams *called_generic_params,
  const CIGenericParams *decl_generic_params)
{
    ASSERT(data_type->kind == CI_DATA_TYPE_KIND_STRUCT ||
           data_type->kind == CI_DATA_TYPE_KIND_UNION);

    const Vec *dt_fields = get_fields__CIDataType(data_type);

    if (dt_fields) {
        return dt_fields;
    }

    String *name = get_name__CIDataType(data_type);
    CIGenericParams *data_type_generic_params =
      (CIGenericParams *)get_generic_params__CIDataType(data_type);
    CIDecl *decl = NULL;

    ASSERT(name);

    switch (data_type->kind) {
        case CI_DATA_TYPE_KIND_TYPEDEF:
            UNREACHABLE("data type is resolved at this point");
        case CI_DATA_TYPE_KIND_STRUCT:
            decl = search_struct_in_generic_context__CIParser(
              self,
              name,
              data_type_generic_params,
              called_generic_params,
              decl_generic_params);

            break;
        case CI_DATA_TYPE_KIND_UNION:
            decl = search_union_in_generic_context__CIParser(
              self,
              name,
              data_type_generic_params,
              called_generic_params,
              decl_generic_params);

            break;
        default:
            UNREACHABLE("unexpected data type");
    }

    if (!decl) {
        FAILED("struct or union is not found");
    }

    return get_fields__CIDecl(decl);
}

bool
typecheck_struct_field__CIParser(const CIParser *self,
                                 const Vec *left_fields,
                                 const Vec *right_fields,
                                 struct CITypecheckContext *typecheck_ctx)
{
    if (left_fields->len != right_fields->len) {
        return false;
    }

    for (Usize i = 0; i < left_fields->len; ++i) {
        CIDeclStructField *left_field = get__Vec(left_fields, i);
        CIDeclStructField *right_field = get__Vec(right_fields, i);

        if (!perform_typecheck__CIParser(self,
                                         left_field->data_type,
                                         right_field->data_type,
                                         false,
                                         typecheck_ctx)) {
            return false;
        }
    }

    return true;
}

bool
typecheck_union_field__CIParser(const CIParser *self,
                                const Vec *decl_fields,
                                const Vec *called_fields,
                                struct CITypecheckContext *typecheck_ctx)
{
    if (called_fields->len > 1) {
        FAILED("expected one or zero field for a union initializer");
    } else if (called_fields->len == 0) {
        return true;
    }

    CIDeclStructField *called_field = last__Vec(called_fields);

    for (Usize i = 0; i < decl_fields->len; ++i) {
        CIDeclStructField *decl_field = get__Vec(decl_fields, i);

        if ((called_field->name &&
             !strcmp(GET_PTR_RC(String, called_field->name)->buffer,
                     GET_PTR_RC(String, decl_field->name)->buffer)) ||
            !called_field->name) {
            if (perform_typecheck__CIParser(self,
                                            called_field->data_type,
                                            decl_field->data_type,
                                            true,
                                            typecheck_ctx)) {
                return true;
            }
        }
    }

    FAILED("the data type of the given field in the union don't match with any "
           "fields of the union");

    return false;
}

bool
is_valid_implicit_cast_from_array_to_struct__CIParser(
  const CIParser *self,
  CIDataType *array_dt,
  CIDataType *struct_dt,
  struct CITypecheckContext *typecheck_ctx)
{
    ASSERT(is_sized_array__CIDataType(array_dt));

    const Vec *struct_dt_fields = get_fields_from_data_type__CIParser(
      self,
      struct_dt,
      typecheck_ctx->current_generic_params.called,
      typecheck_ctx->current_generic_params.decl);
    Vec *fields_from_array_dt = build_fields_from_data_type__CIDeclStructField(
      array_dt->array.data_type, array_dt->array.size);
    bool is_valid_struct_field = typecheck_struct_field__CIParser(
      self, struct_dt_fields, fields_from_array_dt, typecheck_ctx);

    FREE_BUFFER_ITEMS(fields_from_array_dt->buffer,
                      fields_from_array_dt->len,
                      CIDeclStructField);
    FREE(Vec, fields_from_array_dt);

    return is_valid_struct_field;
}

bool
is_valid_implicit_cast_from_array_to_union__CIParser(
  const CIParser *self,
  CIDataType *array_dt,
  CIDataType *union_dt,
  struct CITypecheckContext *typecheck_ctx)
{
    ASSERT(is_sized_array__CIDataType(array_dt));

    const Vec *union_dt_fields = get_fields_from_data_type__CIParser(
      self,
      union_dt,
      typecheck_ctx->current_generic_params.called,
      typecheck_ctx->current_generic_params.decl);
    Vec *fields_from_array_dt = build_fields_from_data_type__CIDeclStructField(
      array_dt->array.data_type, 1);
    bool is_valid_union_field = typecheck_union_field__CIParser(
      self, union_dt_fields, fields_from_array_dt, typecheck_ctx);

    FREE_BUFFER_ITEMS(fields_from_array_dt->buffer,
                      fields_from_array_dt->len,
                      CIDeclStructField);
    FREE(Vec, fields_from_array_dt);

    return is_valid_union_field;
}

bool
is_valid_implicit_cast_from_struct_to_union__CIParser(
  const CIParser *self,
  CIDataType *struct_dt,
  CIDataType *union_dt,
  struct CITypecheckContext *typecheck_ctx)
{
    const Vec *struct_fields = get_fields_from_data_type__CIParser(
      self,
      struct_dt,
      typecheck_ctx->current_generic_params.called,
      typecheck_ctx->current_generic_params.decl);
    const Vec *union_fields = get_fields_from_data_type__CIParser(
      self,
      union_dt,
      typecheck_ctx->current_generic_params.called,
      typecheck_ctx->current_generic_params.decl);

    ASSERT(struct_fields);
    ASSERT(union_fields);

    return typecheck_union_field__CIParser(
      self, union_fields, struct_fields, typecheck_ctx);
}

bool
is_valid_implicit_cast__CIParser(const CIParser *self,
                                 CIDataType *left,
                                 CIDataType *right,
                                 struct CITypecheckContext *typecheck_ctx)
{
    switch (right->kind) {
        case CI_DATA_TYPE_KIND_BOOL:
        case CI_DATA_TYPE_KIND_CHAR:
        case CI_DATA_TYPE_KIND_INT:
        case CI_DATA_TYPE_KIND_LONG_INT:
        case CI_DATA_TYPE_KIND_LONG_LONG_INT:
        case CI_DATA_TYPE_KIND_SHORT_INT:
        case CI_DATA_TYPE_KIND_SIGNED_CHAR:
        case CI_DATA_TYPE_KIND_UNSIGNED_INT:
        case CI_DATA_TYPE_KIND_UNSIGNED_CHAR:
        case CI_DATA_TYPE_KIND_UNSIGNED_LONG_INT:
        case CI_DATA_TYPE_KIND_UNSIGNED_LONG_LONG_INT:
        case CI_DATA_TYPE_KIND_UNSIGNED_SHORT_INT:
            // NOTE: For the moment, we accept a cast from a float to an integer
            // without emitting a warning/error.
            return is_integer_data_type__CIParser(
                     (CIParser *)self,
                     left,
                     true,
                     typecheck_ctx->current_generic_params.called,
                     typecheck_ctx->current_generic_params.decl) ||
                   is_float_data_type__CIParser(
                     (CIParser *)self,
                     left,
                     typecheck_ctx->current_generic_params.called,
                     typecheck_ctx->current_generic_params.decl);
        case CI_DATA_TYPE_KIND_DOUBLE:
        case CI_DATA_TYPE_KIND_DOUBLE__COMPLEX:
        case CI_DATA_TYPE_KIND_DOUBLE__IMAGINARY:
        case CI_DATA_TYPE_KIND__DECIMAL128:
        case CI_DATA_TYPE_KIND__DECIMAL32:
        case CI_DATA_TYPE_KIND__DECIMAL64:
        case CI_DATA_TYPE_KIND_FLOAT:
        case CI_DATA_TYPE_KIND_FLOAT__COMPLEX:
        case CI_DATA_TYPE_KIND_FLOAT__IMAGINARY:
        case CI_DATA_TYPE_KIND_LONG_DOUBLE:
        case CI_DATA_TYPE_KIND_LONG_DOUBLE__COMPLEX:
        case CI_DATA_TYPE_KIND_LONG_DOUBLE__IMAGINARY:
            // NOTE: For the moment, we accept a cast from an integer to a float
            // without emitting a warning/error.
            return is_float_data_type__CIParser(
                     (CIParser *)self,
                     left,
                     typecheck_ctx->current_generic_params.called,
                     typecheck_ctx->current_generic_params.decl) ||
                   is_integer_data_type__CIParser(
                     (CIParser *)self,
                     left,
                     true,
                     typecheck_ctx->current_generic_params.called,
                     typecheck_ctx->current_generic_params.decl);
        case CI_DATA_TYPE_KIND_TYPEDEF:
            UNREACHABLE(
              "impossible to get typedef at this point (already resolved)");
        case CI_DATA_TYPE_KIND_ARRAY:
        case CI_DATA_TYPE_KIND_PTR:
            if (is_sized_array__CIDataType(right)) {
                switch (left->kind) {
                    case CI_DATA_TYPE_KIND_STRUCT:
                        return is_valid_implicit_cast_from_array_to_struct__CIParser(
                          self, right, left, typecheck_ctx);
                    case CI_DATA_TYPE_KIND_UNION:
                        return is_valid_implicit_cast_from_array_to_union__CIParser(
                          self, right, left, typecheck_ctx);
                    default:
                        break;
                }
            }

            // In this case, we check whether the pointers being compared are
            // compatible. For example, we check whether a `void*` is compatible
            // with an `int*`.
            if (is_ptr_data_type__CIParser(
                  (CIParser *)self,
                  left,
                  typecheck_ctx->current_generic_params.called,
                  typecheck_ctx->current_generic_params.decl)) {
                if (is_compatible_with_void_ptr_data_type__CIParser(
                      (CIParser *)self,
                      left,
                      right,
                      typecheck_ctx->current_generic_params.called,
                      typecheck_ctx->current_generic_params.decl)) {
                    return true;
                }

                CIDataType *left_ptr_dt =
                  unwrap_implicit_ptr_data_type__CIParser(
                    (CIParser *)self,
                    left,
                    typecheck_ctx->current_generic_params.called,
                    typecheck_ctx->current_generic_params.decl);
                CIDataType *right_ptr_dt =
                  unwrap_implicit_ptr_data_type__CIParser(
                    (CIParser *)self,
                    right,
                    typecheck_ctx->current_generic_params.called,
                    typecheck_ctx->current_generic_params.decl);

                return is_valid_implicit_cast__CIParser(
                  self, left_ptr_dt, right_ptr_dt, typecheck_ctx);
            }

            // This case is designed to ensure that a pointer or an array is
            // compatible with an integer.
            //
            // Valid: (void*)0, {1,2,3}, "hello"
            if (is_integer_data_type__CIParser(
                  (CIParser *)self,
                  left,
                  true,
                  typecheck_ctx->current_generic_params.called,
                  typecheck_ctx->current_generic_params.decl)) {
                return true;
            }

            return false;
        case CI_DATA_TYPE_KIND_STRUCT:
        case CI_DATA_TYPE_KIND_UNION:
            if (is_sized_array__CIDataType(left)) {
                return is_valid_implicit_cast_from_array_to_struct__CIParser(
                  self, left, right, typecheck_ctx);
            } else if ((left->kind == CI_DATA_TYPE_KIND_STRUCT &&
                        right->kind == CI_DATA_TYPE_KIND_STRUCT) ||
                       (left->kind == CI_DATA_TYPE_KIND_UNION &&
                        right->kind == CI_DATA_TYPE_KIND_UNION)) {
                String *left_name = get_name__CIDataType(left);
                String *right_name = get_name__CIDataType(right);

                if (left_name && right_name) {
                    return !strcmp(left_name->buffer, right_name->buffer);
                }

                const Vec *unresolved_left_fields =
                  get_fields__CIDataType(left);
                const Vec *unresolved_right_fields =
                  get_fields__CIDataType(right);

                // NOTE: We perform a check if one or both fields are NULL, as
                // the `eq__CIDataType` function returns a false, because at the
                // point of this function, we can't perform a thorough check
                // (search structure).
                if (!unresolved_left_fields || !unresolved_right_fields) {
                    const Vec *left_fields =
                      get_fields_from_data_type__CIParser(
                        self,
                        left,
                        typecheck_ctx->current_generic_params.called,
                        typecheck_ctx->current_generic_params.decl);
                    const Vec *right_fields =
                      get_fields_from_data_type__CIParser(
                        self,
                        right,
                        typecheck_ctx->current_generic_params.called,
                        typecheck_ctx->current_generic_params.decl);

                    if (left_fields && right_fields) {
                        switch (left->kind) {
                            case CI_DATA_TYPE_KIND_STRUCT:
                                return typecheck_struct_field__CIParser(
                                  self,
                                  left_fields,
                                  right_fields,
                                  typecheck_ctx);
                            case CI_DATA_TYPE_KIND_UNION:
                                return typecheck_union_field__CIParser(
                                  self,
                                  left_fields,
                                  right_fields,
                                  typecheck_ctx);
                            default:
                                UNREACHABLE("unknown variant in this case")
                        }
                    }
                }

                return false;
            } else if (left->kind == CI_DATA_TYPE_KIND_STRUCT &&
                       right->kind == CI_DATA_TYPE_KIND_UNION) {
                return is_valid_implicit_cast_from_struct_to_union__CIParser(
                  self, left, right, typecheck_ctx);
            } else if (left->kind == CI_DATA_TYPE_KIND_UNION &&
                       right->kind == CI_DATA_TYPE_KIND_STRUCT) {
                return is_valid_implicit_cast_from_struct_to_union__CIParser(
                  self, right, left, typecheck_ctx);
            }

            return false;
        default:
            return false;
    }
}

bool
perform_typecheck__CIParser(const CIParser *self,
                            CIDataType *expected_data_type,
                            CIDataType *given_data_type,
                            bool can_try,
                            struct CITypecheckContext *typecheck_ctx)
{
    CIDataType *resolved_expected_data_type =
      resolve_data_type__CIParser(self,
                                  expected_data_type,
                                  typecheck_ctx->current_generic_params.called,
                                  typecheck_ctx->current_generic_params.decl);
    CIDataType *resolved_given_data_type =
      resolve_data_type__CIParser(self,
                                  given_data_type,
                                  typecheck_ctx->current_generic_params.called,
                                  typecheck_ctx->current_generic_params.decl);

    if (!eq__CIDataType(resolved_expected_data_type,
                        resolved_given_data_type)) {
        if (!is_valid_implicit_cast__CIParser(self,
                                              resolved_expected_data_type,
                                              resolved_given_data_type,
                                              typecheck_ctx) ||
            !is_valid_implicit_cast__CIParser(self,
                                              resolved_given_data_type,
                                              resolved_expected_data_type,
                                              typecheck_ctx)) {
            FREE(CIDataType, resolved_expected_data_type);
            FREE(CIDataType, resolved_given_data_type);

            if (!can_try) {
                FAILED("data types don't match");
            }

            return false;
        }
    }

    FREE(CIDataType, resolved_expected_data_type);
    FREE(CIDataType, resolved_given_data_type);

    return true;
}

void
typecheck_variable_decl__CIParser(const CIParser *self,
                                  const CIDeclVariable *variable,
                                  struct CITypecheckContext *typecheck_ctx)
{
    if (variable->expr) {
        typecheck_expr__CIParser(
          self, variable->data_type, variable->expr, typecheck_ctx);
    }
}

void
typecheck_decl__CIParser(const CIParser *self,
                         const CIDecl *decl,
                         struct CITypecheckContext *typecheck_ctx)
{
    switch (decl->kind) {
        case CI_DECL_KIND_LABEL:
            break;
        case CI_DECL_KIND_VARIABLE:
            typecheck_variable_decl__CIParser(
              self, &decl->variable, typecheck_ctx);

            break;
        default:
            UNREACHABLE("this kind of declaration is not expected or not yet "
                        "implement in body function");
    }
}

void
typecheck_binary_expr__CIParser(const CIParser *self,
                                const CIExprBinary *binary,
                                struct CITypecheckContext *typecheck_ctx)
{
    switch (binary->kind) {
        case CI_EXPR_BINARY_KIND_ASSIGN:
        case CI_EXPR_BINARY_KIND_ASSIGN_ADD:
        case CI_EXPR_BINARY_KIND_ASSIGN_SUB:
        case CI_EXPR_BINARY_KIND_ASSIGN_MUL:
        case CI_EXPR_BINARY_KIND_ASSIGN_DIV:
        case CI_EXPR_BINARY_KIND_ASSIGN_MOD:
        case CI_EXPR_BINARY_KIND_ASSIGN_BIT_AND:
        case CI_EXPR_BINARY_KIND_ASSIGN_BIT_OR:
        case CI_EXPR_BINARY_KIND_ASSIGN_XOR:
        case CI_EXPR_BINARY_KIND_ASSIGN_BIT_LSHIFT:
        case CI_EXPR_BINARY_KIND_ASSIGN_BIT_RSHIFT: {
            CIDataType *left_dt = infer_expr_data_type__CIParser(
              self,
              binary->left,
              typecheck_ctx->current_scope_id,
              typecheck_ctx->current_generic_params.called,
              typecheck_ctx->current_generic_params.decl);
            CIDataType *right_dt = infer_expr_data_type__CIParser(
              self,
              binary->right,
              typecheck_ctx->current_scope_id,
              typecheck_ctx->current_generic_params.called,
              typecheck_ctx->current_generic_params.called);

            perform_typecheck__CIParser(
              self, left_dt, right_dt, false, typecheck_ctx);

            FREE(CIDataType, left_dt);
            FREE(CIDataType, right_dt);

            break;
        }
        default:
            break;
    }
}

void
typecheck_function_call_expr_params__CIParser(
  const CIParser *self,
  const CIDecl *decl_function_call,
  const Vec *called_params,
  struct CITypecheckContext *typecheck_ctx)
{
    const Vec *decl_function_call_params =
      get_function_params__CIDecl(decl_function_call);
    bool is_variadic =
      decl_function_call_params
        ? is_variadic__CIDeclFunctionParam(decl_function_call_params)
        : false;
    Usize decl_function_call_params_len =
      decl_function_call_params ? decl_function_call_params->len : 0;
    Usize called_params_len = called_params->len;

    if (decl_function_call_params_len != called_params_len &&
        (is_variadic &&
         called_params_len < decl_function_call_params_len - 1)) {
        FAILED("number of params don't matched");
    }

    struct CurrentGenericParams old_current_generic_params =
      decl_function_call->kind == CI_DECL_KIND_FUNCTION_GEN
        ? set_current_generic_params__CITypecheckContext(
            typecheck_ctx,
            NEW(CurrentGenericParams,
                decl_function_call->function_gen.called_generic_params,
                decl_function_call->function_gen.function->generic_params))
        : default__CurrentGenericParams();

    for (Usize i = 0; i < called_params_len; ++i) {
        const CIDeclFunctionParam *decl_param = get__Vec(
          decl_function_call_params,
          i >= decl_function_call_params_len ? decl_function_call_params_len - 1
                                             : i);

        switch (decl_param->kind) {
            case CI_DECL_FUNCTION_PARAM_KIND_NORMAL: {
                const CIExpr *called_param = get__Vec(called_params, i);

                typecheck_expr__CIParser(
                  self, decl_param->data_type, called_param, typecheck_ctx);

                break;
            }
            case CI_DECL_FUNCTION_PARAM_KIND_VARIADIC:
                break;
            default:
                UNREACHABLE("unknown variant");
        }
    }

    if (decl_function_call->kind == CI_DECL_KIND_FUNCTION_GEN) {
        set_current_generic_params__CITypecheckContext(
          typecheck_ctx, old_current_generic_params);
    }
}

void
typecheck_function_call_expr__CIParser(const CIParser *self,
                                       const CIExprFunctionCall *function_call,
                                       struct CITypecheckContext *typecheck_ctx)
{
    CIDecl *function_decl = search_function_in_generic_context__CIParser(
      self,
      GET_PTR_RC(String, function_call->identifier),
      function_call->generic_params,
      typecheck_ctx->current_generic_params.called,
      typecheck_ctx->current_generic_params.decl);

    if (!function_decl) {
        UNREACHABLE("the function has not been generated");
    }

    if (function_decl) {
        switch (function_decl->kind) {
            case CI_DECL_KIND_FUNCTION:
            case CI_DECL_KIND_FUNCTION_GEN:
                typecheck_function_call_expr_params__CIParser(
                  self, function_decl, function_call->params, typecheck_ctx);

                break;
            default:
                UNREACHABLE("expected function or function gen");
        }
    }
}

void
typecheck_function_call_builtin_expr_params__CIParser(
  const CIParser *self,
  const CIBuiltinFunction *decl_function_call_builtin,
  const Vec *called_params,
  struct CITypecheckContext *typecheck_ctx)
{
    if (called_params->len != decl_function_call_builtin->params->len) {
        FAILED("number of params don't matched");
    }

    for (Usize i = 0; i < decl_function_call_builtin->params->len; ++i) {
        CIDataType *builtin_param =
          get__Vec(decl_function_call_builtin->params, i);
        const CIExpr *called_param = get__Vec(called_params, i);

        typecheck_expr__CIParser(
          self, builtin_param, called_param, typecheck_ctx);
    }
}

void
typecheck_function_call_builtin_expr__CIParser(
  const CIParser *self,
  const CIExprFunctionCallBuiltin *function_call_builtin,
  struct CITypecheckContext *typecheck_ctx)
{
    CIBuiltin *builtin = get_ref__CIBuiltin();
    const CIBuiltinFunction *builtin_function =
      get_builtin_function__CIBuiltin(builtin, function_call_builtin->id);

    ASSERT(builtin_function);

    typecheck_function_call_builtin_expr_params__CIParser(
      self, builtin_function, function_call_builtin->params, typecheck_ctx);
}

void
typecheck_unary_expr__CIParser(const CIParser *self,
                               const CIExprUnary *unary,
                               struct CITypecheckContext *typecheck_ctx)
{
    CIDataType *right_dt = infer_expr_data_type__CIParser(
      self,
      unary->expr,
      typecheck_ctx->current_scope_id,
      typecheck_ctx->current_generic_params.called,
      typecheck_ctx->current_generic_params.decl);

    switch (unary->kind) {
        case CI_EXPR_UNARY_KIND_PRE_INCREMENT:
        case CI_EXPR_UNARY_KIND_PRE_DECREMENT:
        case CI_EXPR_UNARY_KIND_POST_INCREMENT:
        case CI_EXPR_UNARY_KIND_POST_DECREMENT:
        case CI_EXPR_UNARY_KIND_POSITIVE:
        case CI_EXPR_UNARY_KIND_NEGATIVE:
            if (!(is_integer_data_type__CIParser(
                    (CIParser *)self,
                    right_dt,
                    true,
                    typecheck_ctx->current_generic_params.called,
                    typecheck_ctx->current_generic_params.decl) ||
                  is_float_data_type__CIParser(
                    (CIParser *)self,
                    right_dt,
                    typecheck_ctx->current_generic_params.called,
                    typecheck_ctx->current_generic_params.decl))) {
                FAILED("this operation is not allowed for this data type, "
                       "expected float or integer");
            }

            break;
        case CI_EXPR_UNARY_KIND_BIT_NOT:
        case CI_EXPR_UNARY_KIND_NOT:
            if (!is_integer_data_type__CIParser(
                  (CIParser *)self,
                  right_dt,
                  true,
                  typecheck_ctx->current_generic_params.called,
                  typecheck_ctx->current_generic_params.decl)) {
                FAILED("this operation is not allowed for this data type, "
                       "expected integer");
            }

            break;
        case CI_EXPR_UNARY_KIND_DEREFERENCE:
            if (!is_ptr_data_type__CIParser(
                  (CIParser *)self,
                  right_dt,
                  typecheck_ctx->current_generic_params.called,
                  typecheck_ctx->current_generic_params.decl)) {
                FAILED("this operation is not allowed for this data type, "
                       "expected pointer");
            }

            break;
        case CI_EXPR_UNARY_KIND_REF:
            break;
        default:
            UNREACHABLE("unknown variant");
    }

    FREE(CIDataType, right_dt);
}

void
typecheck_ternary_expr__CIParser(const CIParser *self,
                                 const CIExprTernary *ternary,
                                 struct CITypecheckContext *typecheck_ctx)
{
    CIDataType *cond_dt = infer_expr_data_type__CIParser(
      self,
      ternary->cond,
      typecheck_ctx->current_scope_id,
      typecheck_ctx->current_generic_params.called,
      typecheck_ctx->current_generic_params.decl);

    if (!is_integer_data_type__CIParser(
          (CIParser *)self,
          cond_dt,
          true,
          typecheck_ctx->current_generic_params.called,
          typecheck_ctx->current_generic_params.decl)) {
        FAILED("expected interger");
    }

    FREE(CIDataType, cond_dt);

    CIDataType *if_dt = infer_expr_data_type__CIParser(
      self,
      ternary->if_,
      typecheck_ctx->current_scope_id,
      typecheck_ctx->current_generic_params.called,
      typecheck_ctx->current_generic_params.decl);
    CIDataType *else_dt = infer_expr_data_type__CIParser(
      self,
      ternary->else_,
      typecheck_ctx->current_scope_id,
      typecheck_ctx->current_generic_params.called,
      typecheck_ctx->current_generic_params.decl);

    perform_typecheck__CIParser(self, if_dt, else_dt, false, typecheck_ctx);

    FREE(CIDataType, if_dt);
    FREE(CIDataType, else_dt);
}

void
typecheck_expr__CIParser(const CIParser *self,
                         CIDataType *expected_data_type,
                         const CIExpr *given_expr,
                         struct CITypecheckContext *typecheck_ctx)
{
    ASSERT((typecheck_ctx->current_generic_params.called &&
            typecheck_ctx->current_generic_params.decl) ||
           (!typecheck_ctx->current_generic_params.called &&
            !typecheck_ctx->current_generic_params.decl));

    // Typecheck expression content.
    switch (given_expr->kind) {
        case CI_EXPR_KIND_BINARY:
            typecheck_binary_expr__CIParser(
              self, &given_expr->binary, typecheck_ctx);

            break;
        case CI_EXPR_KIND_CAST: {
            // To allow cast (void) from all types, we expect any data type.
            CIDataType *expected_dt =
              given_expr->cast.data_type->kind == CI_DATA_TYPE_KIND_VOID
                ? NEW(CIDataType, CI_DATA_TYPE_KIND_ANY)
                : ref__CIDataType(given_expr->cast.data_type);

            typecheck_expr__CIParser(
              self, expected_dt, given_expr->cast.expr, typecheck_ctx);

            // TODO: Implement a specific check to verify what can be cast
            // explicitly.

            FREE(CIDataType, expected_dt);

            break;
        }
        case CI_EXPR_KIND_FUNCTION_CALL:
            typecheck_function_call_expr__CIParser(
              self, &given_expr->function_call, typecheck_ctx);

            break;
        case CI_EXPR_KIND_FUNCTION_CALL_BUILTIN:
            typecheck_function_call_builtin_expr__CIParser(
              self, &given_expr->function_call_builtin, typecheck_ctx);

            break;
        case CI_EXPR_KIND_GROUPING:
            typecheck_expr__CIParser(
              self, expected_data_type, given_expr->grouping, typecheck_ctx);

            break;
        case CI_EXPR_KIND_TERNARY:
            typecheck_ternary_expr__CIParser(
              self, &given_expr->ternary, typecheck_ctx);

            break;
        case CI_EXPR_KIND_UNARY:
            typecheck_unary_expr__CIParser(
              self, &given_expr->unary, typecheck_ctx);

            break;
        default:
            break;
    }

    CIDataType *given_expr_dt = infer_expr_data_type__CIParser(
      self,
      given_expr,
      typecheck_ctx->current_scope_id,
      typecheck_ctx->current_generic_params.called,
      typecheck_ctx->current_generic_params.decl);

    ASSERT(given_expr_dt);

    perform_typecheck__CIParser(
      self, expected_data_type, given_expr_dt, false, typecheck_ctx);

    FREE(CIDataType, given_expr_dt);
}

void
typecheck_expr_and_try_discard(const CIParser *self,
                               CIExpr *expr,
                               struct CITypecheckContext *typecheck_ctx)
{
    CIDataType *expected_dt = NULL;

    // Here are the expressions whose value can be discarded by default.
    switch (expr->kind) {
        case CI_EXPR_KIND_BINARY:
            switch (expr->binary.kind) {
                case CI_EXPR_BINARY_KIND_ASSIGN:
                case CI_EXPR_BINARY_KIND_ASSIGN_ADD:
                case CI_EXPR_BINARY_KIND_ASSIGN_SUB:
                case CI_EXPR_BINARY_KIND_ASSIGN_MUL:
                case CI_EXPR_BINARY_KIND_ASSIGN_DIV:
                case CI_EXPR_BINARY_KIND_ASSIGN_MOD:
                case CI_EXPR_BINARY_KIND_ASSIGN_BIT_AND:
                case CI_EXPR_BINARY_KIND_ASSIGN_BIT_OR:
                case CI_EXPR_BINARY_KIND_ASSIGN_XOR:
                case CI_EXPR_BINARY_KIND_ASSIGN_BIT_LSHIFT:
                case CI_EXPR_BINARY_KIND_ASSIGN_BIT_RSHIFT:
                    goto expected_any;
                default:
                    goto expected_void;
            }
        case CI_EXPR_KIND_FUNCTION_CALL:
        case CI_EXPR_KIND_FUNCTION_CALL_BUILTIN:
        expected_any:
            // NOTE: In order to discard the value of a function call,
            // we expect the any type, which is usually only used for
            // builtins, but which has the particularity of accepting
            // all types.
            expected_dt = NEW(CIDataType, CI_DATA_TYPE_KIND_ANY);

            break;
        case CI_EXPR_KIND_UNARY:
            switch (expr->unary.kind) {
                case CI_EXPR_UNARY_KIND_PRE_INCREMENT:
                case CI_EXPR_UNARY_KIND_POST_INCREMENT:
                case CI_EXPR_UNARY_KIND_PRE_DECREMENT:
                case CI_EXPR_UNARY_KIND_POST_DECREMENT:
                    goto expected_any;
                default:
                    goto expected_void;
            }
        default:
        expected_void:
            expected_dt = NEW(CIDataType, CI_DATA_TYPE_KIND_VOID);
    }

    typecheck_expr__CIParser(self, expected_dt, expr, typecheck_ctx);

    FREE(CIDataType, expected_dt);
}

void
typecheck_body_item__CIParser(const CIParser *self,
                              const CIDeclFunctionItem *item,
                              struct CITypecheckContext *typecheck_ctx)
{
    switch (item->kind) {
        case CI_DECL_FUNCTION_ITEM_KIND_DECL:
            typecheck_decl__CIParser(self, item->decl, typecheck_ctx);

            break;
        case CI_DECL_FUNCTION_ITEM_KIND_EXPR:
            typecheck_expr_and_try_discard(self, item->expr, typecheck_ctx);

            break;
        case CI_DECL_FUNCTION_ITEM_KIND_STMT:
            typecheck_stmt__CIParser(self, &item->stmt, typecheck_ctx);

            break;
        default:
            UNREACHABLE("unknown variant");
    }
}

void
typecheck_body__CIParser(const CIParser *self,
                         const CIDeclFunctionBody *body,
                         struct CITypecheckContext *typecheck_ctx)
{
    if (body) {
        VecIter body_content_iter = NEW(VecIter, body->content);
        CIDeclFunctionItem *body_content_item = NULL;
        const CIScopeID *parent_scope_id = typecheck_ctx->current_scope_id;

        typecheck_ctx->current_scope_id = body->scope_id;

        while ((body_content_item = next__VecIter(&body_content_iter))) {
            typecheck_body_item__CIParser(
              self, body_content_item, typecheck_ctx);
        }

        typecheck_ctx->current_scope_id = parent_scope_id;
    }
}

void
typecheck_case_stmt__CIParser(const CIParser *self,
                              const CIStmtSwitchCase *case_,
                              struct CITypecheckContext *typecheck_ctx)
{
    if (typecheck_ctx->current_switch.is_present) {
        CIDataType *expr_data_type = infer_expr_data_type__CIParser(
          self,
          case_->value,
          typecheck_ctx->current_scope_id,
          typecheck_ctx->current_generic_params.called,
          typecheck_ctx->current_generic_params.decl);

        if (typecheck_ctx->current_switch.is_integer) {
            if (!is_integer_data_type__CIParser(
                  (CIParser *)self,
                  expr_data_type,
                  false,
                  typecheck_ctx->current_generic_params.called,
                  typecheck_ctx->current_generic_params.decl)) {
                FAILED("expected integer compatible data type");
            }

            FREE(CIDataType, expr_data_type);

            return;
        } else if (typecheck_ctx->current_switch.is_float) {
            if (!is_float_data_type__CIParser(
                  (CIParser *)self,
                  expr_data_type,
                  typecheck_ctx->current_generic_params.called,
                  typecheck_ctx->current_generic_params.decl)) {
                FAILED("expected float compatible data type");
            }

            FREE(CIDataType, expr_data_type);

            return;
        }

        UNREACHABLE("is_integer or is_float must be true");
    }

    // NOTE: We don't do anything because we're outside the switch
    // statement.
}

void
typecheck_condition_expr__CIParser(const CIParser *self,
                                   const CIExpr *cond,
                                   struct CITypecheckContext *typecheck_ctx)
{
    CIDataType *cond_expr_dt = infer_expr_data_type__CIParser(
      self,
      cond,
      typecheck_ctx->current_scope_id,
      typecheck_ctx->current_generic_params.called,
      typecheck_ctx->current_generic_params.decl);

    if (!is_integer_data_type__CIParser(
          (CIParser *)self,
          cond_expr_dt,
          true,
          typecheck_ctx->current_generic_params.called,
          typecheck_ctx->current_generic_params.decl)) {
        FAILED("expected boolean compatible expression data type");
    }

    FREE(CIDataType, cond_expr_dt);
}

void
typecheck_do_while_stmt__CIParser(const CIParser *self,
                                  const CIStmtDoWhile *do_while,
                                  struct CITypecheckContext *typecheck_ctx)
{
    typecheck_condition_expr__CIParser(self, do_while->cond, typecheck_ctx);
    typecheck_body__CIParser(self, do_while->body, typecheck_ctx);
}

void
typecheck_if_stmt_branch__CIParser(const CIParser *self,
                                   const CIStmtIfBranch *if_branch,
                                   struct CITypecheckContext *typecheck_ctx)
{
    typecheck_condition_expr__CIParser(self, if_branch->cond, typecheck_ctx);
    typecheck_body__CIParser(self, if_branch->body, typecheck_ctx);
}

void
typecheck_if_stmt__CIParser(const CIParser *self,
                            const CIStmtIf *if_,
                            struct CITypecheckContext *typecheck_ctx)
{
    typecheck_if_stmt_branch__CIParser(self, if_->if_, typecheck_ctx);

    if (if_->else_ifs) {
        for (Usize i = 0; i < if_->else_ifs->len; ++i) {
            const CIStmtIfBranch *else_if = get__Vec(if_->else_ifs, i);

            typecheck_if_stmt_branch__CIParser(self, else_if, typecheck_ctx);
        }
    }

    if (if_->else_) {
        typecheck_body__CIParser(self, if_->else_, typecheck_ctx);
    }
}

void
typecheck_for_stmt__CIParser(const CIParser *self,
                             const CIStmtFor *for_,
                             struct CITypecheckContext *typecheck_ctx)
{
    if (for_->init_clauses) {
        for (Usize i = 0; i < for_->init_clauses->len; ++i) {
            typecheck_body_item__CIParser(
              self, get__Vec(for_->init_clauses, i), typecheck_ctx);
        }
    }

    if (for_->expr1) {
        typecheck_condition_expr__CIParser(self, for_->expr1, typecheck_ctx);
    }

    if (for_->exprs2) {
        CIDataType *expected_expr2_dt = NEW(CIDataType, CI_DATA_TYPE_KIND_VOID);

        for (Usize i = 0; i < for_->exprs2->len; ++i) {
            CIExpr *expr2 = get__Vec(for_->exprs2, i);

            typecheck_expr_and_try_discard(self, expr2, typecheck_ctx);
        }

        FREE(CIDataType, expected_expr2_dt);
    }

    typecheck_body__CIParser(self, for_->body, typecheck_ctx);
}

void
typecheck_goto_stmt__CIParser(const CIParser *self,
                              const String *goto_,
                              struct CITypecheckContext *typecheck_ctx)
{
    const CIScope *local_current_scope = get_scope_from_id__CIResultFile(
      self->file, typecheck_ctx->current_scope_id);

    CIDecl *label_decl =
      search_label__CIResultFile(self->file, local_current_scope, goto_);

    if (!label_decl) {
        FAILED("the specified label defined in goto doesn't exist");
    }
}

void
typecheck_return_stmt__CIParser(const CIParser *self,
                                const CIExpr *return_,
                                struct CITypecheckContext *typecheck_ctx)
{
    ASSERT(typecheck_ctx->current_decl);

    const CIDataType *given_return_data_type =
      get_return_data_type__CIDecl(typecheck_ctx->current_decl);

    if (return_) {
        typecheck_expr__CIParser(
          self, (CIDataType *)given_return_data_type, return_, typecheck_ctx);
    } else {
        CIDataType *expected_return_data_type =
          NEW(CIDataType, CI_DATA_TYPE_KIND_VOID);

        if (!eq__CIDataType(expected_return_data_type,
                            given_return_data_type)) {
            FAILED("expected void return data type");
        }

        FREE(CIDataType, expected_return_data_type);
    }
}

void
typecheck_switch_stmt__CIParser(const CIParser *self,
                                const CIStmtSwitch *switch_,
                                struct CITypecheckContext *typecheck_ctx)
{
    CIDataType *expr_data_type = infer_expr_data_type__CIParser(
      self,
      switch_->expr,
      typecheck_ctx->current_scope_id,
      typecheck_ctx->current_generic_params.called,
      typecheck_ctx->current_generic_params.decl);
    struct CurrentSwitch parent_current_switch = typecheck_ctx->current_switch;

    typecheck_ctx->current_switch.is_present = true;

    if (is_integer_data_type__CIParser(
          (CIParser *)self,
          expr_data_type,
          false,
          typecheck_ctx->current_generic_params.called,
          typecheck_ctx->current_generic_params.decl)) {
        typecheck_ctx->current_switch.is_integer = true;
    } else if (is_float_data_type__CIParser(
                 (CIParser *)self,
                 expr_data_type,
                 typecheck_ctx->current_generic_params.called,
                 typecheck_ctx->current_generic_params.decl)) {
        typecheck_ctx->current_switch.is_float = true;
    } else {
        FAILED("expected integer compatible or float compatible data type");
    }

    typecheck_body__CIParser(self, switch_->body, typecheck_ctx);

    typecheck_ctx->current_switch = parent_current_switch;

    FREE(CIDataType, expr_data_type);
}

void
typecheck_while_stmt__CIParser(const CIParser *self,
                               const CIStmtWhile *while_,
                               struct CITypecheckContext *typecheck_ctx)
{
    typecheck_condition_expr__CIParser(self, while_->cond, typecheck_ctx);
    typecheck_body__CIParser(self, while_->body, typecheck_ctx);
}

void
typecheck_stmt__CIParser(const CIParser *self,
                         const CIStmt *given_stmt,
                         struct CITypecheckContext *typecheck_ctx)
{
    ASSERT((typecheck_ctx->current_generic_params.called &&
            typecheck_ctx->current_generic_params.decl) ||
           (!typecheck_ctx->current_generic_params.called &&
            !typecheck_ctx->current_generic_params.decl));

    switch (given_stmt->kind) {
        case CI_STMT_KIND_BLOCK:
            return typecheck_body__CIParser(
              self, given_stmt->block.body, typecheck_ctx);
        case CI_STMT_KIND_BREAK:
            break;
        case CI_STMT_KIND_CASE:
            return typecheck_case_stmt__CIParser(
              self, &given_stmt->case_, typecheck_ctx);
        case CI_STMT_KIND_CONTINUE:
        case CI_STMT_KIND_DEFAULT:
            break;
        case CI_STMT_KIND_DO_WHILE:
            return typecheck_do_while_stmt__CIParser(
              self, &given_stmt->do_while, typecheck_ctx);
        case CI_STMT_KIND_FOR:
            return typecheck_for_stmt__CIParser(
              self, &given_stmt->for_, typecheck_ctx);
        case CI_STMT_KIND_GOTO:
            return typecheck_goto_stmt__CIParser(
              self, GET_PTR_RC(String, given_stmt->goto_), typecheck_ctx);
        case CI_STMT_KIND_IF:
            return typecheck_if_stmt__CIParser(
              self, &given_stmt->if_, typecheck_ctx);
        case CI_STMT_KIND_RETURN:
            return typecheck_return_stmt__CIParser(
              self, given_stmt->return_, typecheck_ctx);
        case CI_STMT_KIND_SWITCH:
            return typecheck_switch_stmt__CIParser(
              self, &given_stmt->switch_, typecheck_ctx);
        case CI_STMT_KIND_WHILE:
            return typecheck_while_stmt__CIParser(
              self, &given_stmt->while_, typecheck_ctx);
        default:
            UNREACHABLE("unknown variant");
    }
}

void
typecheck_function__CIParser(const CIParser *self, const CIDecl *function_decl)
{
    // We can't check the types of a function that has as yet unknown generic
    // types.
    if (function_decl->function.generic_params) {
        return;
    }

    struct CITypecheckContext typecheck_ctx =
      NEW(CITypecheckContext, function_decl, NULL, NULL);

    typecheck_body__CIParser(
      self, function_decl->function.body, &typecheck_ctx);
}

void
typecheck_function_gen__CIParser(const CIParser *self,
                                 const CIDecl *function_gen_decl)
{
    struct CITypecheckContext typecheck_ctx =
      NEW(CITypecheckContext,
          function_gen_decl,
          function_gen_decl->function_gen.called_generic_params,
          function_gen_decl->function_gen.function->generic_params);

    typecheck_body__CIParser(
      self, function_gen_decl->function_gen.function->body, &typecheck_ctx);
}

CIDeclFunctionItem *
parse_do_while_stmt__CIParser(CIParser *self, bool in_switch)
{
    CIDeclFunctionBody *body = NULL;

    switch (self->current_token->kind) {
        case CI_TOKEN_KIND_LBRACE:
            next_token__CIParser(self);
            body = parse_function_block__CIParser(self, true, in_switch);

            break;
        default: {
            CIDeclFunctionItem *item =
              parse_function_body_item__CIParser(self, true, in_switch);
            CIScope *scope = add_scope__CIResultFile(
              self->file, current_scope->scope_id, true);

            body = NEW(CIDeclFunctionBody, scope->scope_id);

            if (item) {
                add__CIDeclFunctionBody(body, item);
            }
        }
    }

    expect_many__CIParser(
      self, 2, CI_TOKEN_KIND_KEYWORD_WHILE, CI_TOKEN_KIND_LPAREN);

    CIExpr *cond = parse_expr__CIParser(self);

    if (!cond) {
        return NULL;
    }

    expect_many__CIParser(
      self, 2, CI_TOKEN_KIND_RPAREN, CI_TOKEN_KIND_SEMICOLON);

    return NEW_VARIANT(
      CIDeclFunctionItem,
      stmt,
      NEW_VARIANT(CIStmt, do_while, NEW(CIStmtDoWhile, body, cond)));
}

CIDeclFunctionItem *
parse_case__CIParser(CIParser *self)
{
    CIExpr *expr = parse_expr__CIParser(self);

    if (!expr) {
        return NULL;
    }

    expect__CIParser(self, CI_TOKEN_KIND_COLON, true);

    ENABLE_IN_LABEL();

    return NEW_VARIANT(CIDeclFunctionItem,
                       stmt,
                       NEW_VARIANT(CIStmt, case, NEW(CIStmtSwitchCase, expr)));
}

CIDeclFunctionItem *
parse_for_stmt__CIParser(CIParser *self, bool in_switch)
{
    Vec *init_clauses = NULL; // Vec<CIDeclFunctionItem*>*?
    CIExpr *expr1 = NULL;
    Vec *exprs2 = NULL; // Vec<CIExpr*>*?
    CIDeclFunctionBody *body = NULL;

    expect__CIParser(self, CI_TOKEN_KIND_LPAREN, true);

    switch (self->current_token->kind) {
        case CI_TOKEN_KIND_SEMICOLON:
            next_token__CIParser(self);
            break;
        default:
            init_clauses = NEW(Vec);

            UNSET_EAT_SEMICOLON();

            while (self->current_token->kind != CI_TOKEN_KIND_EOF) {
                CIDeclFunctionItem *init_clause =
                  parse_function_body_item__CIParser(self, false, false);

                if (!is_for_init_clause__CIDeclFunctionItem(init_clause)) {
                    FAILED("expected valid for init-clause");
                } else if (is_variable__CIDeclFunctionItem(init_clause) &&
                           self->file->config->standard < CI_STANDARD_99) {
                    FAILED("it is impossible to have a variable declaration in "
                           "an `init clause` before C99");
                }

                push__Vec(init_clauses, init_clause);

                switch (self->current_token->kind) {
                    case CI_TOKEN_KIND_COMMA:
                        next_token__CIParser(self);

                        continue;
                    default:
                        goto parse_init_clauses_exit;
                }
            }

        parse_init_clauses_exit:
            expect__CIParser(self, CI_TOKEN_KIND_SEMICOLON, true);

            SET_EAT_SEMICOLON();
    }

    switch (self->current_token->kind) {
        case CI_TOKEN_KIND_SEMICOLON:
            next_token__CIParser(self);
            break;
        default:
            expr1 = parse_expr__CIParser(self);
            expect__CIParser(self, CI_TOKEN_KIND_SEMICOLON, true);
    }

    switch (self->current_token->kind) {
        case CI_TOKEN_KIND_RPAREN:
            next_token__CIParser(self);
            break;
        default:
            exprs2 = NEW(Vec);

            do {
                CIExpr *expr = parse_expr__CIParser(self);

                if (expr) {
                    push__Vec(exprs2, expr);
                }

                if (self->current_token->kind != CI_TOKEN_KIND_RPAREN) {
                    expect__CIParser(self, CI_TOKEN_KIND_COMMA, true);
                }
            } while (self->current_token->kind != CI_TOKEN_KIND_RPAREN &&
                     self->current_token->kind != CI_TOKEN_KIND_EOF);

            expect__CIParser(self, CI_TOKEN_KIND_RPAREN, true);
    }

    switch (self->current_token->kind) {
        case CI_TOKEN_KIND_LBRACE:
            next_token__CIParser(self);
            body = parse_function_block__CIParser(self, true, in_switch);

            break;
        default: {
            CIDeclFunctionItem *item =
              parse_function_body_item__CIParser(self, true, in_switch);
            CIScope *scope = add_scope__CIResultFile(
              self->file, current_scope->scope_id, true);

            body = NEW(CIDeclFunctionBody, scope->scope_id);

            if (item) {
                add__CIDeclFunctionBody(body, item);
            }
        }
    }

    return NEW_VARIANT(CIDeclFunctionItem, stmt, NEW_VARIANT(CIStmt, for, NEW(CIStmtFor, body, init_clauses, expr1, exprs2)));
}

CIStmtIfBranch *
parse_if_branch__CIParser(CIParser *self, bool in_loop, bool in_switch)
{
    expect__CIParser(self, CI_TOKEN_KIND_LPAREN, true);

    CIExpr *cond = parse_expr__CIParser(self);
    CIDeclFunctionBody *body = NULL;

    if (!cond) {
        FAILED("expected if condition");
    }

    expect__CIParser(self, CI_TOKEN_KIND_RPAREN, true);

    switch (self->current_token->kind) {
        case CI_TOKEN_KIND_LBRACE:
            next_token__CIParser(self);
            body = parse_function_block__CIParser(self, in_loop, in_switch);

            break;
        default: {
            CIDeclFunctionItem *item =
              parse_function_body_item__CIParser(self, in_loop, in_switch);
            CIScope *scope = add_scope__CIResultFile(
              self->file, current_scope->scope_id, true);

            body = NEW(CIDeclFunctionBody, scope->scope_id);

            if (item) {
                add__CIDeclFunctionBody(body, item);
            }
        }
    }

    return NEW(CIStmtIfBranch, cond, body);
}

bool
has_else_if__CIParser(CIParser *self)
{
    if (self->current_token->kind == CI_TOKEN_KIND_KEYWORD_ELSE) {
        CIToken *peeked = peek_token__CIParser(self, 1);

        return peeked && peeked->kind == CI_TOKEN_KIND_KEYWORD_IF;
    }

    return false;
}

CIDeclFunctionItem *
parse_if_stmt__CIParser(CIParser *self, bool in_loop, bool in_switch)
{
    CIStmtIfBranch *if_ = parse_if_branch__CIParser(self, in_loop, in_switch);
    Vec *else_ifs = NULL;
    CIDeclFunctionBody *else_ = NULL;

    if (!if_) {
        return NULL;
    }

    for (; has_else_if__CIParser(self);) {
        jump__CIParser(self, 2);

        CIStmtIfBranch *else_if =
          parse_if_branch__CIParser(self, in_loop, in_switch);

        if (else_if) {
            if (!else_ifs) {
                else_ifs = NEW(Vec);
            }

            push__Vec(else_ifs, else_if);
        }
    }

    switch (self->current_token->kind) {
        case CI_TOKEN_KIND_KEYWORD_ELSE:
            next_token__CIParser(self);

            if (expect__CIParser(self, CI_TOKEN_KIND_LBRACE, false)) {
                else_ =
                  parse_function_block__CIParser(self, in_loop, in_switch);
            } else {
                CIDeclFunctionItem *item =
                  parse_function_body_item__CIParser(self, in_loop, false);
                CIScope *scope = add_scope__CIResultFile(
                  self->file, current_scope->scope_id, true);

                else_ = NEW(CIDeclFunctionBody, scope->scope_id);

                if (item) {
                    add__CIDeclFunctionBody(else_, item);
                }
            }

            break;
        default:
            break;
    }

    return NEW_VARIANT(
      CIDeclFunctionItem,
      stmt,
      NEW_VARIANT(CIStmt, if, NEW(CIStmtIf, if_, else_ifs, else_)));
}

CIDeclFunctionItem *
parse_while_stmt__CIParser(CIParser *self, bool in_switch)
{
    expect__CIParser(self, CI_TOKEN_KIND_LPAREN, true);

    CIExpr *cond = parse_expr__CIParser(self);
    CIDeclFunctionBody *body = NULL;

    if (!cond) {
        return NULL;
    }

    expect__CIParser(self, CI_TOKEN_KIND_RPAREN, true);

    switch (self->current_token->kind) {
        case CI_TOKEN_KIND_LBRACE:
            next_token__CIParser(self);
            body = parse_function_block__CIParser(self, true, in_switch);

            break;
        default: {
            CIDeclFunctionItem *item =
              parse_function_body_item__CIParser(self, true, in_switch);

            if (item) {
                CIScope *scope = add_scope__CIResultFile(
                  self->file, current_scope->scope_id, true);

                body = NEW(CIDeclFunctionBody, scope->scope_id);

                add__CIDeclFunctionBody(body, item);
            }
        }
    }

    return NEW_VARIANT(
      CIDeclFunctionItem,
      stmt,
      NEW_VARIANT(CIStmt, while, NEW(CIStmtWhile, cond, body)));
}

CIDeclFunctionItem *
parse_switch_stmt__CIParser(CIParser *self, bool in_loop)
{
    expect__CIParser(self, CI_TOKEN_KIND_LPAREN, true);

    CIExpr *expr = parse_expr__CIParser(self);

    if (!expr) {
        return NULL;
    }

    expect__CIParser(self, CI_TOKEN_KIND_RPAREN, true);

    CIDeclFunctionBody *body = NULL;

    switch (self->current_token->kind) {
        case CI_TOKEN_KIND_LBRACE:
            next_token__CIParser(self);

            body = parse_function_block__CIParser(self, in_loop, true);

            break;
        default: {
            CIDeclFunctionItem *item =
              parse_function_body_item__CIParser(self, in_loop, true);
            CIScope *scope = add_scope__CIResultFile(
              self->file, current_scope->scope_id, true);

            body = NEW(CIDeclFunctionBody, scope->scope_id);

            if (item) {
                add__CIDeclFunctionBody(body, item);
            }
        }
    }

    return NEW_VARIANT(
      CIDeclFunctionItem,
      stmt,
      NEW_VARIANT(CIStmt, switch, NEW(CIStmtSwitch, expr, body)));
}

CIDeclFunctionItem *
parse_block_stmt__CIParser(CIParser *self, bool in_loop, bool in_switch)
{
    CIDeclFunctionBody *body =
      parse_function_block__CIParser(self, in_loop, in_switch);

    return NEW_VARIANT(CIDeclFunctionItem,
                       stmt,
                       NEW_VARIANT(CIStmt, block, NEW(CIStmtBlock, body)));
}

CIDeclFunctionItem *
parse_stmt__CIParser(CIParser *self, bool in_loop, bool in_switch)
{
    next_token__CIParser(self);

    switch (self->previous_token->kind) {
        case CI_TOKEN_KIND_KEYWORD_BREAK:
            expect__CIParser(self, CI_TOKEN_KIND_SEMICOLON, true);

            if (in_loop || in_switch) {
                return NEW_VARIANT(
                  CIDeclFunctionItem, stmt, NEW_VARIANT(CIStmt, break));
            } else {
                FAILED("break is not expected outside of a loop");
            }
        case CI_TOKEN_KIND_KEYWORD_CASE:
            if (in_switch) {
                return parse_case__CIParser(self);
            } else {
                FAILED("case is not expected outside of a switch");
            }
        case CI_TOKEN_KIND_KEYWORD_CONTINUE:
            expect__CIParser(self, CI_TOKEN_KIND_SEMICOLON, true);

            if (in_loop) {
                return NEW_VARIANT(
                  CIDeclFunctionItem, stmt, NEW_VARIANT(CIStmt, continue));
            } else {
                FAILED("continue is not expected outside of a loop");
            }
        case CI_TOKEN_KIND_KEYWORD_DEFAULT:
            expect__CIParser(self, CI_TOKEN_KIND_COLON, true);

            ENABLE_IN_LABEL();

            if (in_switch) {
                return NEW_VARIANT(
                  CIDeclFunctionItem, stmt, NEW_VARIANT(CIStmt, default));
            } else {
                FAILED("default is not expected outside of a switch");
            }
        case CI_TOKEN_KIND_KEYWORD_DO:
            return parse_do_while_stmt__CIParser(self, in_switch);
        case CI_TOKEN_KIND_KEYWORD_FOR:
            return parse_for_stmt__CIParser(self, in_switch);
        case CI_TOKEN_KIND_KEYWORD_GOTO: {
            Rc *label_identifier = NULL;

            if (expect__CIParser(self, CI_TOKEN_KIND_IDENTIFIER, true)) {
                label_identifier = self->previous_token->identifier;
            } else {
                label_identifier = generate_name_error__CIParser();
            }

            return NEW_VARIANT(CIDeclFunctionItem,
                               stmt,
                               NEW_VARIANT(CIStmt, goto, label_identifier));
        }
        case CI_TOKEN_KIND_KEYWORD_IF:
            return parse_if_stmt__CIParser(self, in_loop, in_switch);
        case CI_TOKEN_KIND_KEYWORD_RETURN: {
            switch (self->current_token->kind) {
                case CI_TOKEN_KIND_SEMICOLON:
                    return NEW_VARIANT(CIDeclFunctionItem,
                                       stmt,
                                       NEW_VARIANT(CIStmt, return, NULL));
                default: {
                    ENABLE_ALLOW_INITIALIZATION();

                    CIExpr *expr = parse_expr__CIParser(self);

                    DISABLE_ALLOW_INITIALIZATION();

                    if (expr) {
                        expect__CIParser(self, CI_TOKEN_KIND_SEMICOLON, true);

                        return NEW_VARIANT(CIDeclFunctionItem,
                                           stmt,
                                           NEW_VARIANT(CIStmt, return, expr));
                    }

                    FAILED("expected expression");
                }
            }
        }
        case CI_TOKEN_KIND_KEYWORD_SWITCH:
            return parse_switch_stmt__CIParser(self, in_loop);
        case CI_TOKEN_KIND_KEYWORD_WHILE:
            return parse_while_stmt__CIParser(self, in_switch);
        case CI_TOKEN_KIND_LBRACE:
            return parse_block_stmt__CIParser(self, in_loop, in_switch);
        default:
            UNREACHABLE("not expected token");
    }
}

CIDeclFunctionItem *
parse_function_body_item__CIParser(CIParser *self, bool in_loop, bool in_switch)
{
    switch (self->current_token->kind) {
        case CI_TOKEN_KIND_IDENTIFIER: {
            CIToken *peeked = peek_token__CIParser(self, 1);

            if (peeked && peeked->kind == CI_TOKEN_KIND_COLON) {
                goto parse_decl;
            }

            goto default_case;
        }
        case CI_TOKEN_KIND_KEYWORD_BREAK:
        case CI_TOKEN_KIND_KEYWORD_CASE:
        case CI_TOKEN_KIND_KEYWORD_CONTINUE:
        case CI_TOKEN_KIND_KEYWORD_DEFAULT:
        case CI_TOKEN_KIND_KEYWORD_DO:
        case CI_TOKEN_KIND_KEYWORD_FOR:
        case CI_TOKEN_KIND_KEYWORD_GOTO:
        case CI_TOKEN_KIND_KEYWORD_IF:
        case CI_TOKEN_KIND_KEYWORD_RETURN:
        case CI_TOKEN_KIND_KEYWORD_SWITCH:
        case CI_TOKEN_KIND_KEYWORD_WHILE:
        case CI_TOKEN_KIND_LBRACE: {
            DISABLE_IN_LABEL();

            return parse_stmt__CIParser(self, in_loop, in_switch);
        }
        case CI_TOKEN_KIND_SEMICOLON:
            next_token__CIParser(self);

            return NULL;
        default:
        default_case: {
            if (is_data_type__CIParser(self) ||
                is_storage_class__CIParser(self)) {
            parse_decl: {
                CIDecl *decl = parse_decl__CIParser(self);

                DISABLE_IN_LABEL();

                if (decl) {
                    return NEW_VARIANT(CIDeclFunctionItem, decl, decl);
                } else if (!data_type_as_expression) {
                    return NULL;
                }
            }
            }

            DISABLE_IN_LABEL();

            CIDeclFunctionItem *item =
              NEW_VARIANT(CIDeclFunctionItem, expr, parse_expr__CIParser(self));

            if (eat_semicolon) {
                expect__CIParser(self, CI_TOKEN_KIND_SEMICOLON, true);
            }

            return item;
        }
    }
}

CIDeclFunctionBody *
parse_function_body_base__CIParser(CIParser *self,
                                   bool in_loop,
                                   bool in_switch,
                                   CIScope **parent_function_scope)
{
    ASSERT(current_scope);

    // Save parent values.
    CIScope *parent_scope = current_scope;
    CIDeclFunctionBody *parent_body = current_body;

    current_scope =
      parent_function_scope
        ? *parent_function_scope
        : add_scope__CIResultFile(self->file, current_scope->scope_id, true);
    current_body = NEW(CIDeclFunctionBody, current_scope->scope_id);

    while (self->current_token->kind != CI_TOKEN_KIND_RBRACE &&
           self->current_token->kind != CI_TOKEN_KIND_EOF) {
        CIDeclFunctionItem *item =
          parse_function_body_item__CIParser(self, in_loop, in_switch);

        if (item) {
            add__CIDeclFunctionBody(current_body, item);
        }
    }

    CIDeclFunctionBody *res_body = current_body;

    // Restore parent values as current.
    current_scope = parent_scope;
    current_body = parent_body;

    DISABLE_IN_LABEL();

    expect__CIParser(self, CI_TOKEN_KIND_RBRACE, true);

    return res_body;
}

CIDeclFunctionBody *
parse_function_body__CIParser(CIParser *self,
                              bool in_loop,
                              bool in_switch,
                              CIScope **parent_function_scope)
{
    return parse_function_body_base__CIParser(
      self, in_loop, in_switch, parent_function_scope);
}

CIDeclFunctionBody *
parse_function_block__CIParser(CIParser *self, bool in_loop, bool in_switch)
{
    return parse_function_body_base__CIParser(self, in_loop, in_switch, NULL);
}

CIDecl *
parse_function__CIParser(CIParser *self,
                         int storage_class_flag,
                         CIDataType *return_data_type,
                         Rc *name,
                         CIGenericParams *generic_params,
                         Vec *attributes)
{
    init_function_to_visit_waiting_list__CIParser(self,
                                                  GET_PTR_RC(String, name));

    CIScope *parent_function_scope =
      add_scope__CIResultFile(self->file, current_scope->scope_id, true);
    Vec *params = parse_function_params__CIParser(
      self, parent_function_scope); // Vec<CIDeclFunctionParam*>*?
    CIDecl *res = NULL;

    switch (self->current_token->kind) {
        case CI_TOKEN_KIND_SEMICOLON:
            next_token__CIParser(self);

            res = NEW_VARIANT(CIDecl,
                              function,
                              storage_class_flag,
                              true,
                              NEW(CIDeclFunction,
                                  name,
                                  return_data_type,
                                  generic_params,
                                  params,
                                  NULL,
                                  attributes));

            if (HAS_TYPEDEF_STORAGE_CLASS_FLAG()) {
                return parse_typedef__CIParser(self, res, generic_params);
            }

            break;
        case CI_TOKEN_KIND_LBRACE:
            if (HAS_TYPEDEF_STORAGE_CLASS_FLAG()) {
                FAILED("not expected to have a body when typedef is passed");
            } else if (is__CIBuiltinFunction(GET_PTR_RC(String, name))) {
                FAILED("cannot redefine a builtin function");
            }

            next_token__CIParser(self);

            SET_IN_FUNCTION_BODY();

            CIDeclFunctionBody *body = parse_function_body__CIParser(
              self, false, false, &parent_function_scope);

            UNSET_IN_FUNCTION_BODY();

            res = NEW_VARIANT(CIDecl,
                              function,
                              storage_class_flag,
                              false,
                              NEW(CIDeclFunction,
                                  name,
                                  return_data_type,
                                  generic_params,
                                  params,
                                  body,
                                  attributes));

            break;
        default:
            FAILED("expected `;` or `{`");
    }

    typecheck_function__CIParser(self, res);

    return res;
}

Vec *
parse_fields__CIParser(CIParser *self)
{
    Vec *fields = NEW(Vec); // Vec<CIDeclStructField*>*

    while (self->current_token->kind != CI_TOKEN_KIND_RBRACE &&
           self->current_token->kind != CI_TOKEN_KIND_EOF) {
        CIDataType *data_type = parse_data_type__CIParser(self);
        Rc *name = NULL;

        switch (data_type->kind) {
            case CI_DATA_TYPE_KIND_STRUCT:
            case CI_DATA_TYPE_KIND_UNION:
                if (expect__CIParser(self, CI_TOKEN_KIND_IDENTIFIER, false)) {
                    name = self->previous_token->identifier;
                }

                break;
            default:
                if (expect__CIParser(self, CI_TOKEN_KIND_IDENTIFIER, true)) {
                    name = self->previous_token->identifier;
                } else {
                    name = generate_name_error__CIParser();
                }
        }

        parse_variable_name_and_data_type__CIParser(
          self, &data_type, &name, false);

        expect__CIParser(self, CI_TOKEN_KIND_SEMICOLON, true);

        // TODO: parse bits set

        push__Vec(fields, NEW(CIDeclStructField, name, data_type));
    }

    expect__CIParser(self, CI_TOKEN_KIND_RBRACE, true);

    return fields;
}

Vec *
parse_struct_or_union_fields__CIParser(CIParser *self)
{
    switch (self->current_token->kind) {
        case CI_TOKEN_KIND_LBRACE:
            next_token__CIParser(self);
            break;
        default:
            return NULL;
    }

    return parse_fields__CIParser(self);
}

CIDecl *
parse_struct__CIParser(CIParser *self,
                       int storage_class_flag,
                       Rc *name,
                       CIGenericParams *generic_params)
{
    if (name) {
        init_struct_to_visit_waiting_list__CIParser(self,
                                                    GET_PTR_RC(String, name));
    }

    Vec *fields = parse_struct_or_union_fields__CIParser(self);

    return NEW_VARIANT(CIDecl,
                       struct,
                       storage_class_flag,
                       fields ? false : true,
                       NEW(CIDeclStruct, name, generic_params, fields));
}

CIDecl *
parse_typedef__CIParser(CIParser *self,
                        CIDecl *aliased_decl,
                        CIGenericParams *generic_params)
{
    if ((storage_class_flag & ~CI_STORAGE_CLASS_TYPEDEF) !=
        CI_STORAGE_CLASS_NONE) {
        FAILED(
          "cannot combine other storage class specifier(s) with `typedef`");
    }

    String *decl_name = get_name__CIDecl(aliased_decl);
    CIDecl *res = NULL;

#define NEW_TYPEDEF(need_ref, dt)                                  \
    ({                                                             \
        Rc *_decl_name_rc = NEW(Rc, clone__String(decl_name));     \
        CIDecl *_res =                                             \
          NEW_VARIANT(CIDecl,                                      \
                      typedef,                                     \
                      NEW(CIDeclTypedef,                           \
                          _decl_name_rc,                           \
                          generic_params && need_ref               \
                            ? ref__CIGenericParams(generic_params) \
                            : generic_params,                      \
                          dt));                                    \
        FREE_RC(String, _decl_name_rc);                            \
        _res;                                                      \
    })

    switch (aliased_decl->kind) {
        case CI_DECL_KIND_VARIABLE:
            res = NEW_TYPEDEF(
              false, ref__CIDataType(aliased_decl->variable.data_type));

            break;
        case CI_DECL_KIND_FUNCTION:
            res = NEW_TYPEDEF(
              true,
              NEW_VARIANT(
                CIDataType,
                function,
                NEW(CIDataTypeFunction,
                    aliased_decl->function.name,
                    aliased_decl->function.params
                      ? clone_params__CIDeclFunctionParam(
                          aliased_decl->function.params)
                      : NULL,
                    ref__CIDataType(aliased_decl->function.return_data_type),
                    NULL)));

            break;
        default:
            UNREACHABLE("this kind of declaration is not exepcted");
    }

    FREE(CIDecl, aliased_decl);

    return res;

#undef NEW_TYPEDEF
}

CIDecl *
parse_union__CIParser(CIParser *self,
                      int storage_class_flag,
                      Rc *name,
                      CIGenericParams *generic_params)
{
    if (name) {
        init_union_to_visit_waiting_list__CIParser(self,
                                                   GET_PTR_RC(String, name));
    }

    Vec *fields = parse_struct_or_union_fields__CIParser(self);

    return NEW_VARIANT(CIDecl,
                       union,
                       storage_class_flag,
                       fields ? false : true,
                       NEW(CIDeclUnion, name, generic_params, fields));
}

bool
is_only_initialization_compatible_data_type__CIParser(CIParser *self,
                                                      CIDataType *data_type)
{
    switch (data_type->kind) {
        case CI_DATA_TYPE_KIND_ARRAY:
            return true;
        default:
            return false;
    }
}

bool
is_initialization_expr__CIParser(CIParser *self, CIExpr *expr)
{
    switch (expr->kind) {
        case CI_EXPR_KIND_ARRAY:
            return true;
        case CI_EXPR_KIND_LITERAL:
            switch (expr->literal.kind) {
                case CI_EXPR_LITERAL_KIND_STRING:
                    return true;
                default:
                    return false;
            }
        default:
            return false;
    }
}

void
check_for_initialization_expr__CIParser(CIParser *self,
                                        CIDataType *data_type,
                                        CIExpr *expr)
{

    if (is_only_initialization_compatible_data_type__CIParser(self,
                                                              data_type) &&
        !is_initialization_expr__CIParser(self, expr)) {
        FAILED("need initialization expression for array data type");
    }
}

CIDecl *
parse_variable__CIParser(CIParser *self,
                         int storage_class_flag,
                         CIDataType *data_type,
                         Rc *name,
                         bool no_expr,
                         bool is_local)
{
    if (!name) {
        FAILED("expected name for variable");
    } else if (in_label) {
        FAILED("Don't accept variable declaration in label");
    }

    if (self->current_token->kind != CI_TOKEN_KIND_COMMA) {
        next_token__CIParser(self); // skip `=` or `;`
    }

    if (no_expr) {
        return NEW_VARIANT(
          CIDecl,
          variable,
          storage_class_flag,
          false,
          NEW(CIDeclVariable, data_type, name, NULL, is_local));
    } else if (HAS_TYPEDEF_STORAGE_CLASS_FLAG()) {
        FAILED("expression is not expected with typedef storage class");
    }

    ENABLE_ALLOW_INITIALIZATION();

    CIExpr *expr = parse_expr__CIParser(self);

    check_for_initialization_expr__CIParser(self, data_type, expr);

    DISABLE_ALLOW_INITIALIZATION();

    switch (self->current_token->kind) {
        case CI_TOKEN_KIND_SEMICOLON:
            if (eat_semicolon) {
                next_token__CIParser(self);
            }

            break;
        case CI_TOKEN_KIND_COMMA:
            break;
        default:
            FAILED("expected `;`");
    }

    return NEW_VARIANT(CIDecl,
                       variable,
                       storage_class_flag,
                       false,
                       NEW(CIDeclVariable, data_type, name, expr, is_local));
}

CIDecl *
parse_variable_list__CIParser(CIParser *self,
                              CIDecl *current_var,
                              int storage_class_flag,
                              CIDataType *data_type,
                              CIGenericParams *typedef_generic_params)
{
    // Parse list of variable.
    // first_variable, <var2>, <var3>;
    while (self->current_token->kind == CI_TOKEN_KIND_COMMA &&
           self->previous_token->kind != CI_TOKEN_KIND_SEMICOLON) {
        if (current_var) {
            if (HAS_TYPEDEF_STORAGE_CLASS_FLAG()) {
                CIDecl *typedef_decl = parse_typedef__CIParser(
                  self, current_var, typedef_generic_params);

                // NOTE: "current_var" is not available after calling
                // "parse_typedef__CIParser"
                current_var = NULL;

                add_decl_to_scope__CIParser(self, &typedef_decl, true);
            } else {
                add_decl_to_scope__CIParser(self, &current_var, true);

                // Push to the current body of the function the current
                // variable.
                if (in_function_body) {
                    ASSERT(current_body);

                    add__CIDeclFunctionBody(
                      current_body,
                      NEW_VARIANT(CIDeclFunctionItem, decl, current_var));
                }
            }
        }

        next_token__CIParser(self); // skip `,`

        CIDataType *post_data_type =
          parse_post_data_type__CIParser(self, clone__CIDataType(data_type));
        Rc *name = NULL; // Rc<String*>*? (&)

        parse_variable_name_and_data_type__CIParser(
          self, &post_data_type, &name, true);

        switch (self->current_token->kind) {
            case CI_TOKEN_KIND_EQ:
                if (HAS_TYPEDEF_STORAGE_CLASS_FLAG()) {
                    FAILED("not expected `=` when typedef storage class is "
                           "specified");
                }

                current_var = parse_variable__CIParser(self,
                                                       storage_class_flag,
                                                       post_data_type,
                                                       name,
                                                       false,
                                                       in_function_body);

                break;
            case CI_TOKEN_KIND_COMMA:
            case CI_TOKEN_KIND_SEMICOLON:
                current_var = parse_variable__CIParser(self,
                                                       storage_class_flag,
                                                       post_data_type,
                                                       name,
                                                       true,
                                                       in_function_body);

                break;
            default:
                FAILED("expected `=`, `*` or `;`");
        }
    }

    return HAS_TYPEDEF_STORAGE_CLASS_FLAG()
             ? parse_typedef__CIParser(
                 self, current_var, typedef_generic_params)
             : current_var;
}

CIDecl *
parse_label__CIParser(CIParser *self)
{
    if (in_function_body) {
        switch (self->current_token->kind) {
            case CI_TOKEN_KIND_IDENTIFIER: {
                Rc *identifier = self->current_token->identifier;
                CIToken *peeked_token = peek_token__CIParser(self, 1);

                if (peeked_token && peeked_token->kind == CI_TOKEN_KIND_COLON) {
                    jump__CIParser(self, 2);

                    return NEW_VARIANT(
                      CIDecl, label, NEW(CIDeclLabel, identifier));
                }

                return NULL;
            }
            default:
                return NULL;
        }
    }

    return NULL;
}

CIDecl *
parse_decl__CIParser(CIParser *self)
{
    CIDecl *res = NULL;

    if ((res = parse_label__CIParser(self))) {
        goto exit;
    }

    Vec *attributes = NULL;

    parse_attributes__CIParser(self, &attributes);

    RESET_STORAGE_CLASS_FLAG();
    RESET_DATA_TYPE_QUALIFIER_FLAG();

    parse_storage_class_specifiers__CIParser(self, &storage_class_flag);

    CIDataType *pre_data_type = parse_pre_data_type__CIParser(self);
    CIDataType *data_type = parse_post_data_type__CIParser(self, pre_data_type);

    switch (self->current_token->kind) {
        case CI_TOKEN_KIND_LPAREN:
        case CI_TOKEN_KIND_IDENTIFIER: {
            Rc *name = NULL; // Rc<String*>*? (&)

            parse_variable_name_and_data_type__CIParser(
              self, &data_type, &name, false);

            CIGenericParams *generic_params =
              parse_generic_params__CIParser(self); // CIGenericParams*?

            switch (self->current_token->kind) {
                // Parse first variable
                case CI_TOKEN_KIND_EQ:
                case CI_TOKEN_KIND_COMMA:
                case CI_TOKEN_KIND_SEMICOLON:
                    if (generic_params && !HAS_TYPEDEF_STORAGE_CLASS_FLAG()) {
                        FAILED("no generic params are expected in variable "
                               "declaration");
                    }

                    res = parse_variable__CIParser(
                      self,
                      storage_class_flag,
                      data_type,
                      name,
                      self->current_token->kind == CI_TOKEN_KIND_EQ ? false
                                                                    : true,
                      in_function_body);

                    break;
                case CI_TOKEN_KIND_LPAREN:
                    res = parse_function__CIParser(self,
                                                   storage_class_flag,
                                                   data_type,
                                                   name,
                                                   generic_params,
                                                   attributes);

                    goto exit;
                default:
                    if (!in_function_body) {
                        FAILED("expected `=`, `;`, `(`");
                    }

                    data_type_as_expression = data_type;

                    return NULL;
            }

            res = parse_variable_list__CIParser(
              self, res, storage_class_flag, pre_data_type, generic_params);

            break;
        }
        default:
            expect__CIParser(self, CI_TOKEN_KIND_SEMICOLON, true);

            FREE(CIDataType, data_type);

            return NULL;
    }

exit:
    if (res) {
        add_decl_to_scope__CIParser(self, &res, true);
    }

    return res;
}

const CIDecl *
add_decl_to_scope__CIParser(CIParser *self, CIDecl **decl_ref, bool must_free)
{
    CIDecl *decl = *decl_ref;
    CIDecl *res = NULL;

    ASSERT(decl);

    switch (decl->kind) {
        case CI_DECL_KIND_ENUM:
            if ((res = (CIDecl *)add_enum__CIResultFile(self->file, decl))) {
                // See `add_enum__CIResultFile` prototype.
                if (decl != res) {
                    FAILED("enum is already defined");
                }

                goto free;
            }

            goto exit;
        case CI_DECL_KIND_ENUM_VARIANT:
            if ((res = (CIDecl *)add_enum_variant__CIResultFile(self->file,
                                                                decl))) {
                // See `add_enum_variant__CIResultFile` prototype.
                if (decl != res) {
                    FAILED("enum variant is already defined");
                }

                goto free;
            }

            goto exit;
        case CI_DECL_KIND_FUNCTION:
        case CI_DECL_KIND_FUNCTION_GEN:
            if ((res =
                   (CIDecl *)add_function__CIResultFile(self->file, decl))) {
                // See `add_function__CIResultFile` prototype.
                if (decl != res) {
                    FAILED("function is already defined");
                }

                goto free;
            }

            goto exit;
        case CI_DECL_KIND_LABEL:
            if ((res = (CIDecl *)add_label__CIResultFile(
                   self->file, current_scope, ref__CIDecl(decl)))) {
                // See `add_label__CIResultFile` prototype.
                if (decl != res) {
                    FAILED("label is already defined");
                }

                goto free;
            }

            goto exit;
        case CI_DECL_KIND_STRUCT:
        case CI_DECL_KIND_STRUCT_GEN:
            if ((res = (CIDecl *)add_struct__CIResultFile(self->file, decl))) {
                // See `add_struct__CIResultFile` prototype.
                if (decl != res) {
                    FAILED("struct is already defined");
                }

                goto free;
            }

            goto exit;
        case CI_DECL_KIND_TYPEDEF:
        case CI_DECL_KIND_TYPEDEF_GEN:
            if ((res = (CIDecl *)add_typedef__CIResultFile(self->file, decl))) {
                // See `add_typedef__CIResultFile` prototype.
                if (decl != res) {
                    FAILED("typedef is already defined");
                }

                goto free;
            }

            goto exit;
        case CI_DECL_KIND_UNION:
        case CI_DECL_KIND_UNION_GEN:
            if ((res = (CIDecl *)add_union__CIResultFile(self->file, decl))) {
                // See `add_union__CIResultFile` prototype.
                if (decl != res) {
                    FAILED("union is already defined");
                }

                goto free;
            }

            goto exit;
        case CI_DECL_KIND_VARIABLE:
            if ((res = (CIDecl *)add_variable__CIResultFile(
                   self->file,
                   current_scope,
                   in_function_body ? ref__CIDecl(decl) : decl))) {
                // See `add_variable__CIResultFile` prototype.
                if (decl != res) {
                    FAILED("variable is already defined");
                }

                goto free;
            }

            goto exit;
        default:
            UNREACHABLE("impossible situation");
    }

exit:
    return res;

free:
    if (must_free) {
        FREE(CIDecl, decl);
        *decl_ref = NULL;
    }

    return res;
}

CIAttribute *
parse_attribute__CIParser(CIParser *self)
{
    switch (self->current_token->kind) {
        case CI_TOKEN_KIND_IDENTIFIER: {
            const String *attribute_identifier =
              GET_PTR_RC(String, self->current_token->identifier);
            const enum CIAttributeStandardKind attr_id =
              get_id__Search(attribute_identifier,
                             ci_standard_attributes,
                             (const Int32 *)ci_standard_attribute_ids,
                             CI_N_STANDARD_ATTRIBUTE);
            CIAttribute *res = NULL;

            next_token__CIParser(self);

            switch (attr_id) {
                case CI_ATTRIBUTE_STANDARD_KIND_DEPRECATED:
                case CI_ATTRIBUTE_STANDARD_KIND_NODISCARD: {
                    expect__CIParser(self, CI_TOKEN_KIND_LPAREN, true);

                    Rc *reason = NULL; // Rc<String*>*? (&)

                    switch (self->current_token->kind) {
                        case CI_TOKEN_KIND_LITERAL_CONSTANT_STRING:
                            reason =
                              self->current_token->literal_constant_string;

                            break;
                        default:
                            FAILED("expected string value");
                    }

                    expect__CIParser(self, CI_TOKEN_KIND_RPAREN, true);

                    switch (attr_id) {
                        case CI_ATTRIBUTE_STANDARD_KIND_DEPRECATED:
                            res = NEW_VARIANT(CIAttribute,
                                              standard,
                                              NEW_VARIANT(CIAttributeStandard,
                                                          deprecated,
                                                          reason));

                            break;
                        case CI_ATTRIBUTE_STANDARD_KIND_NODISCARD:
                            res = NEW_VARIANT(CIAttribute,
                                              standard,
                                              NEW_VARIANT(CIAttributeStandard,
                                                          nodiscard,
                                                          reason));

                            break;
                        default:
                            UNREACHABLE("unknown variant");
                    }

                    break;
                }
                default:
                    res = NEW_VARIANT(
                      CIAttribute, standard, NEW(CIAttributeStandard, attr_id));
            }

            expect__CIParser(self, CI_TOKEN_KIND_RHOOK, true);
            expect__CIParser(self, CI_TOKEN_KIND_RHOOK, true);

            return res;
        }
        default:
            FAILED("expected attribute identifier");
    }

    return NULL;
}

void
parse_attributes__CIParser(CIParser *self, Vec **attributes)
{
    while (true) {
        switch (self->current_token->kind) {
            case CI_TOKEN_KIND_LHOOK: {
                if (!(*attributes)) {
                    *attributes = NEW(Vec);
                }

                next_token__CIParser(self);
                expect__CIParser(self, CI_TOKEN_KIND_LHOOK, true);

                CIAttribute *attr = parse_attribute__CIParser(self);

                if (attr) {
                    push__Vec(*attributes, attr);
                }

                break;
            }
            default:
                return;
        }
    }
}

bool
parse_data_type_qualifier__CIParser(CIParser *self,
                                    int *data_type_qualifier_flag)
{
    switch (self->current_token->kind) {
        case CI_TOKEN_KIND_KEYWORD_CONST:
            *data_type_qualifier_flag |= CI_DATA_TYPE_QUALIFIER_CONST;
            break;
        case CI_TOKEN_KIND_KEYWORD_VOLATILE:
            *data_type_qualifier_flag |= CI_DATA_TYPE_QUALIFIER_VOLATILE;
            break;
        case CI_TOKEN_KIND_KEYWORD_RESTRICT:
        case CI_TOKEN_KIND_KEYWORD___RESTRICT:
        case CI_TOKEN_KIND_KEYWORD___RESTRICT__:
            *data_type_qualifier_flag |= CI_DATA_TYPE_QUALIFIER_RESTRICT;
            break;
        case CI_TOKEN_KIND_KEYWORD__ATOMIC:
            *data_type_qualifier_flag |= CI_DATA_TYPE_QUALIFIER__ATOMIC;
            break;
        case CI_TOKEN_KIND_KEYWORD__NORETURN:
            *data_type_qualifier_flag |= CI_DATA_TYPE_QUALIFIER__NORETURN;
            break;
        default:
            return false;
    }

    return true;
}

void
parse_data_type_qualifiers__CIParser(CIParser *self,
                                     int *data_type_qualifier_flag)
{
    int old_data_type_qualifier_flag = *data_type_qualifier_flag;

    while (
      parse_data_type_qualifier__CIParser(self, data_type_qualifier_flag)) {
        next_token__CIParser(self);

        if (old_data_type_qualifier_flag == *data_type_qualifier_flag) {
            FAILED("warning: duplicate storage class specifier");
        }

        old_data_type_qualifier_flag = *data_type_qualifier_flag;
    }
}

bool
parse_storage_class_specifier__CIParser(CIParser *self, int *storage_class_flag)
{
    switch (self->current_token->kind) {
        case CI_TOKEN_KIND_KEYWORD_AUTO:
            *storage_class_flag |= CI_STORAGE_CLASS_AUTO;
            break;
        case CI_TOKEN_KIND_KEYWORD_CONSTEXPR:
            *storage_class_flag |= CI_STORAGE_CLASS_CONSTEXPR;
            break;
        case CI_TOKEN_KIND_KEYWORD_EXTERN:
            *storage_class_flag |= CI_STORAGE_CLASS_EXTERN;
            break;
        case CI_TOKEN_KIND_KEYWORD_INLINE:
            *storage_class_flag |= CI_STORAGE_CLASS_INLINE;
            break;
        case CI_TOKEN_KIND_KEYWORD_REGISTER:
            *storage_class_flag |= CI_STORAGE_CLASS_REGISTER;
            break;
        case CI_TOKEN_KIND_KEYWORD_STATIC:
            *storage_class_flag |= CI_STORAGE_CLASS_STATIC;
            break;
        case CI_TOKEN_KIND_KEYWORD_THREAD_LOCAL:
            *storage_class_flag |= CI_STORAGE_CLASS_THREAD_LOCAL;
            break;
        case CI_TOKEN_KIND_KEYWORD_TYPEDEF:
            *storage_class_flag |= CI_STORAGE_CLASS_TYPEDEF;
            break;
        default:
            return false;
    }

    return true;
}

void
parse_storage_class_specifiers__CIParser(CIParser *self,
                                         int *storage_class_flag)
{
    int old_storage_class_flag = *storage_class_flag;

    while (parse_storage_class_specifier__CIParser(self, storage_class_flag)) {
        next_token__CIParser(self);

        if (old_storage_class_flag == *storage_class_flag) {
            FAILED("warning: duplicate storage class specifier");
        }

        old_storage_class_flag = *storage_class_flag;
    }
}

void
run__CIParser(CIParser *self, const CIResolvedTokens *resolved_tokens)
{
    // Set the current scope.
    set_current_scope__CIParser(self);
    // Initialize the parser.
    init__CIParser(self, resolved_tokens);

    while (self->current_token->kind != CI_TOKEN_KIND_EOF) {
        parse_decl__CIParser(self);
    }

    resolve_visit_waiting_list__CIParser(self);

    if (*self->count_error > 0) {
        exit(1);
    }

#ifdef ENV_DEBUG
    // TODO: Print debug
#endif
}

DESTRUCTOR(CIParser, const CIParser *self)
{
    if (names_error) {
        FREE_BUFFER_RC_ITEMS(names_error->buffer, names_error->len, String);
        FREE(Vec, names_error);

        names_error = NULL;
    }

    FREE(CIParserVisitWaitingList, &self->visit_waiting_list);
}
