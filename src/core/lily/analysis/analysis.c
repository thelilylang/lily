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

#include <base/assert.h>
#include <base/new.h>
#include <base/print.h>

#include <core/lily/analysis/analysis.h>
#include <core/lily/analysis/checked/compiler_generic.h>
#include <core/lily/analysis/checked/history.h>
#include <core/lily/analysis/checked/limits.h>
#include <core/lily/analysis/checked/parent.h>
#include <core/lily/analysis/checked/safety_mode.h>
#include <core/lily/analysis/checked/signature.h>
#include <core/lily/analysis/checked/virtual_scope.h>
#include <core/lily/compiler/package.h>
#include <core/lily/lily.h>
#include <core/lily/parser/ast.h>

#include <core/shared/diagnostic.h>

#define ANALYSIS_EMIT_DIAGNOSTIC(self, name, location, ...)           \
    {                                                                 \
        const File *file = get_file_from_filename__LilyPackage(       \
          self->package, location->filename);                         \
        emit__Diagnostic(                                             \
          NEW_VARIANT(Diagnostic, name, file, location, __VA_ARGS__), \
          &self->package->count_error);                               \
    }

// TODO: add disable_codes field to the package.
#define ANALYSIS_EMIT_WARNING_DIAGNOSTIC(self, name, location, ...)   \
    {                                                                 \
        const File *file = get_file_from_filename__LilyPackage(       \
          self->package, location->filename);                         \
        emit_warning__Diagnostic(                                     \
          NEW_VARIANT(Diagnostic, name, file, location, __VA_ARGS__), \
          &self->package->count_error,                                \
          NULL);                                                      \
    }

static inline void
push_constant__LilyAnalysis(LilyAnalysis *self,
                            LilyAstDecl *constant,
                            LilyCheckedDeclModule *module);

static void
push_error__LilyAnalysis(LilyAnalysis *self,
                         LilyAstDecl *error,
                         LilyCheckedDeclModule *module);

static void
push_fun__LilyAnalysis(LilyAnalysis *self,
                       LilyAstDecl *fun,
                       LilyCheckedDeclModule *module);

static void
push_module__LilyAnalysis(LilyAnalysis *self,
                          LilyAstDecl *module_decl,
                          LilyCheckedDeclModule *module,
                          Usize i);

static void
push_class__LilyAnalysis(LilyAnalysis *self,
                         LilyAstDecl *class,
                         LilyCheckedDeclModule *module);

static void
push_enum_object__LilyAnalysis(LilyAnalysis *self,
                               LilyAstDecl *enum_object,
                               LilyCheckedDeclModule *module);

static void
push_record_object__LilyAnalysis(LilyAnalysis *self,
                                 LilyAstDecl *record_object,
                                 LilyCheckedDeclModule *module);

static void
push_trait__LilyAnalysis(LilyAnalysis *self,
                         LilyAstDecl *trait,
                         LilyCheckedDeclModule *module);

static void
push_alias__LilyAnalysis(LilyAnalysis *self,
                         LilyAstDecl *alias,
                         LilyCheckedDeclModule *module);

static void
push_enum__LilyAnalysis(LilyAnalysis *self,
                        LilyAstDecl *enum_,
                        LilyCheckedDeclModule *module);

static void
push_record__LilyAnalysis(LilyAnalysis *self,
                          LilyAstDecl *record,
                          LilyCheckedDeclModule *module);

static void
push_all_decls__LilyAnalysis(LilyAnalysis *self,
                             const Vec *decls,
                             LilyCheckedDeclModule *module);

/// @param deps Vec<LilyCheckedDecl* (&)>* (&)?
/// @note The deps parameter is always NULL when is used for check data type
/// inside a function or method (not a type or object or error).
static LilyCheckedDataType *
check_data_type__LilyAnalysis(LilyAnalysis *self,
                              LilyAstDataType *data_type,
                              LilyCheckedScope *scope,
                              Vec *deps,
                              enum LilyCheckedSafetyMode safety_mode);

static LilyCheckedExpr *
check_identifier_expr__LilyAnalysis(LilyAnalysis *self,
                                    LilyAstExpr *expr,
                                    LilyCheckedScope *scope,
                                    LilyCheckedDataType *defined_data_type,
                                    enum LilyCheckedSafetyMode safety_mode,
                                    bool must_mut);

static LilyCheckedExpr *
check_binary_expr__LilyAnalysis(LilyAnalysis *self,
                                LilyAstExpr *expr,
                                LilyCheckedScope *scope,
                                enum LilyCheckedSafetyMode safety_mode,
                                LilyCheckedDataType *defined_data_type);

static LilyCheckedExpr *
typecheck_binary_expr__LilyAnalysis(LilyAnalysis *self,
                                    LilyAstExpr *expr,
                                    LilyCheckedScope *scope,
                                    LilyCheckedExpr *left,
                                    LilyCheckedExpr *right,
                                    enum LilyCheckedExprBinaryKind kind,
                                    enum LilyCheckedSafetyMode safety_mode,
                                    LilyCheckedDataType *defined_data_type);

static void
reanalyze_fun_call_with_signature__LilyAnalysis(
  LilyAnalysis *self,
  LilyCheckedSignatureFun *signature,
  LilyCheckedDecl *fun);

/// @return The data type of the expresion
static LilyCheckedDataType *
reanalyze_expr__LilyAnalysis(LilyAnalysis *self,
                             LilyCheckedSignatureFun *signature,
                             const LilyCheckedExpr *expr,
                             LilyCheckedVirtualScope *virtual_scope,
                             LilyCheckedDataType *return_data_type);

static void
reanalyze_stmt__LilyAnalysis(LilyAnalysis *self,
                             LilyCheckedSignatureFun *signature,
                             const LilyCheckedStmt *stmt,
                             LilyCheckedVirtualScope *virtual_scope,
                             LilyCheckedDataType *return_data_type);

static void
valid_cast__LilyAnalysis(LilyAnalysis *self,
                         LilyCheckedDataType *src,
                         LilyCheckedDataType *dest,
                         enum LilyCheckedSafetyMode safety_mode);

static bool
is_drop__LilyAnalysis(LilyAnalysis *self,
                      LilyCheckedScope *scope,
                      LilyCheckedExpr *call);

static LilyCheckedScopeResponse
resolve_id__LilyAnalysis(LilyAnalysis *self,
                         LilyAstExpr *id,
                         LilyCheckedScope *scope,
                         enum LilyCheckedScopeResponseKind res_kind,
                         enum LilyCheckedSafetyMode safety_mode);

/// @brief Build generic params for type (enum, record, alias, class, record
/// object, enum object and trait).
static OrderedHashMap *
build_called_generic_params_for_type__LilyAnalysis(
  LilyAnalysis *self,
  const LilyCheckedDecl *decl,
  LilyCheckedScope *scope,
  enum LilyCheckedSafetyMode safety_mode,
  Vec *ast_called_generic_params);

static bool
valid_function_signature__LilyAnalysis(LilyAnalysis *self,
                                       const Location *location,
                                       Vec *params,
                                       Vec *params_call,
                                       HashMap *called_generic_params);

/// @param called_generic_params HashMap<LilyCheckedDataType*>*?
/// @return Vec<LilyCheckedExprCallFunParam*>*?
static Vec *
check_fun_params_call__LilyAnalysis(LilyAnalysis *self,
                                    const Location *location,
                                    Vec *ast_params,
                                    HashMap *called_generic_params,
                                    LilyCheckedDeclFun *called_fun,
                                    LilyCheckedScope *scope,
                                    enum LilyCheckedSafetyMode safety_mode);

static Vec *
check_builtin_fun_params_call__LilyAnalysis(
  LilyAnalysis *self,
  Vec *ast_params,
  LilyCheckedScope *scope,
  enum LilyCheckedSafetyMode safety_mode);

/// @param sys_params Vec<LilyCheckedDataType*>*
static Vec *
check_sys_fun_params_call__LilyAnalysis(LilyAnalysis *self,
                                        Vec *ast_params,
                                        Vec *sys_params,
                                        LilyCheckedScope *scope,
                                        enum LilyCheckedSafetyMode safety_mode);

static LilyCheckedExpr *
get_call_from_expr__LilyAnalysis(LilyAnalysis *self,
                                 LilyAstExpr *expr,
                                 LilyCheckedScope *scope,
                                 LilyCheckedDataType *defined_data_type,
                                 enum LilyCheckedSafetyMode safety_mode,
                                 bool must_mut);

static LilyCheckedExpr *
check_field_access__LilyAnalysis(LilyAnalysis *self,
                                 LilyAstExpr *path,
                                 LilyCheckedExpr *first,
                                 LilyCheckedScope *scope,
                                 LilyCheckedScope *record_scope,
                                 enum LilyCheckedSafetyMode safety_mode,
                                 bool must_mut);

/// @brief Check the fields of a record call.
/// @param params Vec<LilyAstExprRecordParamCall*>* (&)
/// @param checked_generic_params_call OrderedHashMap<LilyCheckedDataType*>* (&)
static Vec *
check_fields_call__LilyAnalysis(LilyAnalysis *self,
                                LilyCheckedScope *scope,
                                LilyCheckedScope *record_scope,
                                const Vec *params,
                                const Location *call_location,
                                OrderedHashMap *checked_generic_params_call,
                                enum LilyCheckedSafetyMode safety_mode);

static LilyCheckedExpr *
check_global_path_expr__LilyAnalysis(LilyAnalysis *self,
                                     LilyAstExpr *expr,
                                     enum LilyCheckedSafetyMode safety_mode,
                                     LilyCheckedDataType *defined_data_type);

static LilyCheckedExpr *
check_Self_path_expr__LilyAnalysis(LilyAnalysis *self,
                                   LilyAstExpr *expr,
                                   LilyCheckedScope *scope,
                                   enum LilyCheckedSafetyMode safety_mode,
                                   bool must_mut,
                                   LilyCheckedDataType *defined_data_type);

static LilyCheckedExpr *
check_self_path_expr__LilyAnalysis(LilyAnalysis *self,
                                   LilyAstExpr *expr,
                                   LilyCheckedScope *scope,
                                   enum LilyCheckedSafetyMode safety_mode,
                                   bool must_mut,
                                   LilyCheckedDataType *defined_data_type);

static LilyCheckedExpr *
check_hook_access_expr__LilyAnalysis(LilyAnalysis *self,
                                     LilyAstExpr *expr,
                                     LilyCheckedScope *scope,
                                     enum LilyCheckedSafetyMode safety_mode,
                                     bool must_mut,
                                     LilyCheckedDataType *defined_data_type);

/// @return LilyCheckedScope*? (&)
static LilyCheckedScope *
check_object_access_expr__LilyAnalysis(LilyAnalysis *self,
                                       LilyAstExpr *expr,
                                       LilyCheckedScope *scope);

static LilyCheckedExpr *
check_property_init_expr__LilyAnalysis(LilyAnalysis *self,
                                       LilyAstExpr *expr,
                                       LilyCheckedScope *scope,
                                       enum LilyCheckedSafetyMode safety_mode,
                                       bool must_mut,
                                       LilyCheckedDataType *defined_data_type);

static LilyCheckedExpr *
check_path_access_expr__LilyAnalysis(LilyAnalysis *self,
                                     LilyAstExpr *expr,
                                     LilyCheckedScope *scope,
                                     enum LilyCheckedSafetyMode safety_mode,
                                     bool must_mut,
                                     LilyCheckedDataType *defined_data_type);

static LilyCheckedExpr *
check_array_expr__LilyAnalysis(LilyAnalysis *self,
                               LilyAstExpr *expr,
                               LilyCheckedScope *scope,
                               enum LilyCheckedSafetyMode safety_mode,
                               bool must_mut,
                               LilyCheckedDataType *defined_data_type);

static LilyCheckedExpr *
check_fun_call_expr__LilyAnalysis(LilyAnalysis *self,
                                  LilyAstExpr *expr,
                                  LilyCheckedScope *scope,
                                  enum LilyCheckedSafetyMode safety_mode,
                                  bool must_mut,
                                  LilyCheckedDataType *defined_data_type);

static LilyCheckedExpr *
check_call_expr__LilyAnalysis(LilyAnalysis *self,
                              LilyAstExpr *expr,
                              LilyCheckedScope *scope,
                              enum LilyCheckedSafetyMode safety_mode,
                              bool must_mut,
                              LilyCheckedDataType *defined_data_type);

static LilyCheckedExpr *
check_assignable_expr__LilyAnalysis(LilyAnalysis *self,
                                    LilyAstExpr *expr,
                                    LilyCheckedScope *scope,
                                    enum LilyCheckedSafetyMode safety_mode,
                                    LilyCheckedDataType *defined_data_type);

static LilyCheckedExpr *
check_cast_expr__LilyAnalysis(LilyAnalysis *self,
                              LilyAstExpr *expr,
                              LilyCheckedScope *scope,
                              enum LilyCheckedSafetyMode safety_mode,
                              LilyCheckedDataType *defined_data_type);

static LilyCheckedExpr *
check_list_expr__LilyAnalysis(LilyAnalysis *self,
                              LilyAstExpr *expr,
                              LilyCheckedScope *scope,
                              enum LilyCheckedSafetyMode safety_mode,
                              LilyCheckedDataType *defined_data_type);

static LilyCheckedExpr *
check_literal_expr__LilyAnalysis(LilyAnalysis *self,
                                 LilyAstExpr *expr,
                                 LilyCheckedScope *scope,
                                 enum LilyCheckedSafetyMode safety_mode,
                                 LilyCheckedDataType *defined_data_type);

static LilyCheckedExpr *
check_tuple_expr__LilyAnalysis(LilyAnalysis *self,
                               LilyAstExpr *expr,
                               LilyCheckedScope *scope,
                               enum LilyCheckedSafetyMode safety_mode,
                               LilyCheckedDataType *defined_data_type);

static LilyCheckedExpr *
check_unary_expr__LilyAnalysis(LilyAnalysis *self,
                               LilyAstExpr *expr,
                               LilyCheckedScope *scope,
                               enum LilyCheckedSafetyMode safety_mode,
                               LilyCheckedDataType *defined_data_type);

/// @param set_data_type LilyCheckedDataType*?
static LilyCheckedExpr *
check_expr__LilyAnalysis(LilyAnalysis *self,
                         LilyAstExpr *expr,
                         LilyCheckedScope *scope,
                         enum LilyCheckedSafetyMode safety_mode,
                         bool must_mut,
                         LilyCheckedDataType *defined_data_type);

static LilyCheckedBodyFunItem *
check_asm_stmt__LilyAnalysis(LilyAnalysis *self,
                             const LilyAstStmt *stmt,
                             LilyCheckedScope *scope,
                             enum LilyCheckedSafetyMode safety_mode);

static LilyCheckedBodyFunItem *
check_await_stmt__LilyAnalysis(LilyAnalysis *self,
                               const LilyAstStmt *stmt,
                               LilyCheckedScope *scope,
                               enum LilyCheckedSafetyMode safety_mode);

static LilyCheckedBodyFunItem *
check_block_stmt__LilyAnalysis(LilyAnalysis *self,
                               const LilyAstStmt *stmt,
                               LilyCheckedScope *scope,
                               bool in_loop,
                               enum LilyCheckedSafetyMode safety_mode,
                               Vec *end_body,
                               Vec *current_body);

static LilyCheckedBodyFunItem *
check_break_stmt__LilyAnalysis(LilyAnalysis *self,
                               const LilyAstStmt *stmt,
                               LilyCheckedScope *scope,
                               bool in_loop,
                               enum LilyCheckedSafetyMode safety_mode);

static LilyCheckedBodyFunItem *
check_defer_stmt__LilyAnalysis(LilyAnalysis *self,
                               const LilyAstStmt *stmt,
                               LilyCheckedScope *scope,
                               Usize i,
                               bool in_loop,
                               enum LilyCheckedSafetyMode safety_mode,
                               Vec *end_body,
                               Vec *current_body);

static LilyCheckedBodyFunItem *
check_drop_stmt__LilyAnalysis(LilyAnalysis *self,
                              const LilyAstStmt *stmt,
                              LilyCheckedScope *scope,
                              enum LilyCheckedSafetyMode safety_mode);

static LilyCheckedBodyFunItem *
check_for_stmt__LilyAnalysis(LilyAnalysis *self,
                             const LilyAstStmt *stmt,
                             LilyCheckedScope *scope,
                             enum LilyCheckedSafetyMode safety_mode);

static LilyCheckedBodyFunItem *
check_if_stmt__LilyAnalysis(LilyAnalysis *self,
                            const LilyAstStmt *stmt,
                            LilyCheckedScope *scope,
                            bool in_loop,
                            enum LilyCheckedSafetyMode safety_mode,
                            Vec *current_body);

static LilyCheckedBodyFunItem *
check_match_stmt__LilyAnalysis(LilyAnalysis *self,
                               const LilyAstStmt *stmt,
                               LilyCheckedScope *scope,
                               bool in_loop,
                               enum LilyCheckedSafetyMode safety_mode,
                               Vec *current_body);

static LilyCheckedBodyFunItem *
check_next_stmt__LilyAnalysis(LilyAnalysis *self,
                              const LilyAstStmt *stmt,
                              LilyCheckedScope *scope,
                              bool in_loop,
                              enum LilyCheckedSafetyMode safety_mode);

static LilyCheckedBodyFunItem *
check_raise_stmt__LilyAnalysis(LilyAnalysis *self,
                               const LilyAstStmt *stmt,
                               LilyCheckedScope *scope,
                               bool in_loop,
                               enum LilyCheckedSafetyMode safety_mode);

static LilyCheckedBodyFunItem *
check_return_stmt__LilyAnalysis(LilyAnalysis *self,
                                const LilyAstStmt *stmt,
                                LilyCheckedScope *scope,
                                bool in_loop,
                                enum LilyCheckedSafetyMode safety_mode,
                                Vec *end_body,
                                Vec *current_body);

static LilyCheckedBodyFunItem *
check_try_stmt__LilyAnalysis(LilyAnalysis *self,
                             const LilyAstStmt *stmt,
                             LilyCheckedScope *scope,
                             bool in_loop,
                             enum LilyCheckedSafetyMode safety_mode,
                             Vec *current_body);

static LilyCheckedBodyFunItem *
check_unsafe_stmt__LilyAnalysis(LilyAnalysis *self,
                                const LilyAstStmt *stmt,
                                LilyCheckedScope *scope,
                                bool in_loop,
                                enum LilyCheckedSafetyMode safety_mode,
                                Vec *current_body);

static LilyCheckedBodyFunItem *
check_variable_stmt__LilyAnalysis(LilyAnalysis *self,
                                  const LilyAstStmt *stmt,
                                  LilyCheckedScope *scope,
                                  Usize i,
                                  enum LilyCheckedSafetyMode safety_mode,
                                  Vec *end_body);

static LilyCheckedBodyFunItem *
check_while_stmt__LilyAnalysis(LilyAnalysis *self,
                               const LilyAstStmt *stmt,
                               LilyCheckedScope *scope,
                               enum LilyCheckedSafetyMode safety_mode,
                               Vec *current_body);

/// @param end_body Vec<LilyCheckedBodyFunItem*>*
static LilyCheckedBodyFunItem *
check_stmt__LilyAnalysis(LilyAnalysis *self,
                         const LilyAstStmt *stmt,
                         LilyCheckedScope *scope,
                         Usize i,
                         bool in_loop,
                         enum LilyCheckedSafetyMode safety_mode,
                         Vec *end_body,
                         Vec *current_body);

static LilyCheckedBodyFunItem *
check_fun_item__LilyAnalysis(LilyAnalysis *self,
                             const LilyAstBodyFunItem *ast_item,
                             LilyCheckedScope *scope,
                             Usize i,
                             bool in_loop,
                             enum LilyCheckedSafetyMode safety_mode,
                             Vec *end_body,
                             Vec *current_body);

/// @return Vec<LilyCheckedGenericParam*>*
static Vec *
check_generic_params(LilyAnalysis *self,
                     Vec *ast_generic_params,
                     LilyCheckedScope *scope);

/// @return Vec<LilyCheckedFunParam*>*
static Vec *
check_fun_params__LilyAnalysis(LilyAnalysis *self,
                               const Vec *params,
                               LilyCheckedScope *scope);

static void
check_fun_signature__LilyAnalysis(LilyAnalysis *self, LilyCheckedDecl *fun);

static void
check_fun__LilyAnalysis(LilyAnalysis *self, LilyCheckedDecl *fun);

static void
check_constant__LilyAnalysis(LilyAnalysis *self,
                             LilyCheckedDecl *constant,
                             LilyCheckedScope *scope);

static void
check_alias__LilyAnalysis(LilyAnalysis *self, LilyCheckedDecl *alias);

/// @return Return true if is a recursive data type
static bool
check_for_recursive_data_type__LilyAnalysis(LilyAnalysis *self,
                                            LilyCheckedDataType *data_type,
                                            String *global_name);

static void
check_record_fields__LilyAnalysis(LilyAnalysis *self,
                                  Vec *ast_fields,
                                  LilyCheckedScope *scope,
                                  LilyCheckedDecl *record);

static void
check_record__LilyAnalysis(LilyAnalysis *self, LilyCheckedDecl *record);

static void
check_enum_variants__LilyAnalysis(LilyAnalysis *self,
                                  Vec *ast_variants,
                                  LilyCheckedScope *scope,
                                  LilyCheckedDecl *enum_);

static void
check_enum__LilyAnalysis(LilyAnalysis *self, LilyCheckedDecl *enum_);

static void
check_error__LilyAnalysis(LilyAnalysis *self, LilyCheckedDecl *error);

static void
check_decls__LilyAnalysis(LilyAnalysis *self,
                          Vec *decls,
                          LilyCheckedScope *scope);

// Apply import.
static void
run_step0__LilyAnalysis(LilyAnalysis *self);

// Push all declarations.
static inline void
run_step1__LilyAnalysis(LilyAnalysis *self);

// Check all declarations.
static inline void
run_step2__LilyAnalysis(LilyAnalysis *self);

static LilyCheckedHistory *history = NULL;
static bool in_try = false;

#define CHECK_FUN_BODY(ast_body, scope, body, safety_mode, in_loop) \
    {                                                               \
        Vec *end_body = NEW(Vec);                                   \
                                                                    \
        for (Usize j = 0; j < ast_body->len; ++j) {                 \
            LilyCheckedBodyFunItem *item =                          \
              check_fun_item__LilyAnalysis(self,                    \
                                           get__Vec(ast_body, j),   \
                                           scope,                   \
                                           j,                       \
                                           in_loop,                 \
                                           safety_mode,             \
                                           end_body,                \
                                           body);                   \
                                                                    \
            if (item) {                                             \
                push__Vec(body, item);                              \
            }                                                       \
        }                                                           \
                                                                    \
        FREE_BUFFER_ITEMS(                                          \
          end_body->buffer, end_body->len, LilyCheckedBodyFunItem); \
        FREE(Vec, end_body);                                        \
    }

#define EXPECTED_BOOL_EXPR(expr)                                     \
    if (!is_guarantee__LilyCheckedDataType(                          \
          expr->data_type, LILY_CHECKED_DATA_TYPE_KIND_BOOL)) {      \
        ANALYSIS_EMIT_DIAGNOSTIC(                                    \
          self,                                                      \
          simple_lily_error,                                         \
          expr->data_type->location,                                 \
          NEW(LilyError,                                             \
              LILY_ERROR_KIND_EXPECTED_DATA_TYPE_IS_NOT_GUARANTEED), \
          NULL,                                                      \
          NULL,                                                      \
          from__String("the boolean type is not guaranteed"));       \
    }

void
push_constant__LilyAnalysis(LilyAnalysis *self,
                            LilyAstDecl *constant,
                            LilyCheckedDeclModule *module)
{
    push__Vec(module->decls,
              NEW_VARIANT(LilyCheckedDecl,
                          constant,
                          &constant->location,
                          constant,
                          NEW(LilyCheckedDeclConstant,
                              constant->constant.name,
                              format__String("{S}.{S}",
                                             module->global_name,
                                             constant->constant.name),
                              NULL,
                              NULL,
                              constant->constant.visibility)));
}

void
push_error__LilyAnalysis(LilyAnalysis *self,
                         LilyAstDecl *error,
                         LilyCheckedDeclModule *module)
{
    LilyCheckedDecl *checked_error = NEW_VARIANT(
      LilyCheckedDecl,
      error,
      &error->location,
      error,
      NEW(LilyCheckedDeclError,
          error->error.name,
          format__String("{S}.{S}", module->global_name, error->error.name),
          NULL,
          NULL,
          NULL,
          error->error.visibility));

    checked_error->error.scope =
      NEW(LilyCheckedScope,
          NEW_VARIANT(LilyCheckedParent, module, module->scope, module),
          NEW_VARIANT(LilyCheckedScopeDecls, decl, checked_error));

    push__Vec(module->decls, checked_error);
}

void
push_fun__LilyAnalysis(LilyAnalysis *self,
                       LilyAstDecl *fun,
                       LilyCheckedDeclModule *module)
{
    LilyCheckedDecl *checked_fun = NEW_VARIANT(
      LilyCheckedDecl,
      fun,
      &fun->location,
      fun,
      NEW(LilyCheckedDeclFun,
          fun->fun.name,
          format__String("{S}.{S}", module->global_name, fun->fun.name),
          NULL,
          NULL,
          fun->fun.return_data_type,
          NULL,
          NEW(Vec),
          NULL,
          fun->fun.visibility,
          fun->fun.is_async,
          fun->fun.is_operator));

    checked_fun->fun.scope =
      NEW(LilyCheckedScope,
          NEW_VARIANT(LilyCheckedParent, module, module->scope, module),
          NEW_VARIANT(LilyCheckedScopeDecls, decl, checked_fun));

    push__Vec(module->decls, checked_fun);
}

void
push_module__LilyAnalysis(LilyAnalysis *self,
                          LilyAstDecl *module_decl,
                          LilyCheckedDeclModule *module,
                          Usize i)
{
    LilyCheckedDecl *checked_module = NEW_VARIANT(
      LilyCheckedDecl,
      module,
      &module_decl->location,
      module_decl,
      NEW(LilyCheckedDeclModule,
          module_decl->module.name,
          format__String(
            "{S}.{S}", module->global_name, module_decl->module.name),
          NEW(Vec),
          NULL,
          module_decl->module.visibility));

    checked_module->module.scope =
      NEW(LilyCheckedScope,
          NEW_VARIANT(LilyCheckedParent, module, module->scope, module),
          NEW_VARIANT(LilyCheckedScopeDecls, module, &checked_module->module));

    push_all_decls__LilyAnalysis(
      self, module_decl->module.decls, &checked_module->module);

    push__Vec(module->decls, checked_module);
}

void
push_class__LilyAnalysis(LilyAnalysis *self,
                         LilyAstDecl *class,
                         LilyCheckedDeclModule *module)
{

    LilyCheckedDecl *checked_class =
      NEW_VARIANT(LilyCheckedDecl,
                  object,
                  &class->location,
                  class,
                  NEW_VARIANT(LilyCheckedDeclObject,
                              class,
                              NEW(LilyCheckedDeclClass,
                                  class->object.class.name,
                                  format__String("{S}.{S}",
                                                 module->global_name,
                                                 class->object.class.name),
                                  NULL,
                                  NULL,
                                  NULL,
                                  NULL,
                                  NULL,
                                  class->object.class.visibility)));

    checked_class->object.class.scope =
      NEW(LilyCheckedScope,
          NEW_VARIANT(LilyCheckedParent, module, module->scope, module),
          NEW_VARIANT(LilyCheckedScopeDecls, decl, checked_class));

    push__Vec(module->decls, checked_class);
}

void
push_enum_object__LilyAnalysis(LilyAnalysis *self,
                               LilyAstDecl *enum_object,
                               LilyCheckedDeclModule *module)
{
    LilyCheckedDecl *checked_enum_object = NEW_VARIANT(
      LilyCheckedDecl,
      object,
      &enum_object->location,
      enum_object,
      NEW_VARIANT(LilyCheckedDeclObject,
                  enum,
                  NEW(LilyCheckedDeclEnumObject,
                      enum_object->object.enum_.name,
                      format__String("{S}.{S}",
                                     module->global_name,
                                     enum_object->object.enum_.name),
                      NULL,
                      NULL,
                      NULL,
                      NULL,
                      enum_object->object.enum_.visibility)));

    checked_enum_object->object.enum_.scope =
      NEW(LilyCheckedScope,
          NEW_VARIANT(LilyCheckedParent, module, module->scope, module),
          NEW_VARIANT(LilyCheckedScopeDecls, decl, checked_enum_object));

    push__Vec(module->decls, checked_enum_object);
}

void
push_record_object__LilyAnalysis(LilyAnalysis *self,
                                 LilyAstDecl *record_object,
                                 LilyCheckedDeclModule *module)
{
    LilyCheckedDecl *checked_record_object = NEW_VARIANT(
      LilyCheckedDecl,
      object,
      &record_object->location,
      record_object,
      NEW_VARIANT(LilyCheckedDeclObject,
                  record,
                  NEW(LilyCheckedDeclRecordObject,
                      record_object->object.record.name,
                      format__String("{S}.{S}",
                                     module->global_name,
                                     record_object->object.record.name),
                      NULL,
                      NULL,
                      NULL,
                      NULL,
                      record_object->object.record.visibility)));

    checked_record_object->object.record.scope =
      NEW(LilyCheckedScope,
          NEW_VARIANT(LilyCheckedParent, module, module->scope, module),
          NEW_VARIANT(LilyCheckedScopeDecls, decl, checked_record_object));

    push__Vec(module->decls, checked_record_object);
}

void
push_trait__LilyAnalysis(LilyAnalysis *self,
                         LilyAstDecl *trait,
                         LilyCheckedDeclModule *module)
{
    LilyCheckedDecl *checked_trait =
      NEW_VARIANT(LilyCheckedDecl,
                  object,
                  &trait->location,
                  trait,
                  NEW_VARIANT(LilyCheckedDeclObject,
                              trait,
                              NEW(LilyCheckedDeclTrait,
                                  trait->object.trait.name,
                                  format__String("{S}.{S}",
                                                 module->global_name,
                                                 trait->object.trait.name),
                                  NULL,
                                  NULL,
                                  NULL,
                                  NULL,
                                  trait->object.trait.visibility)));

    checked_trait->object.trait.scope =
      NEW(LilyCheckedScope,
          NEW_VARIANT(LilyCheckedParent, module, module->scope, module),
          NEW_VARIANT(LilyCheckedScopeDecls, decl, checked_trait));

    push__Vec(module->decls, checked_trait);
}

void
push_alias__LilyAnalysis(LilyAnalysis *self,
                         LilyAstDecl *alias,
                         LilyCheckedDeclModule *module)
{
    LilyCheckedDecl *checked_alias = NEW_VARIANT(
      LilyCheckedDecl,
      type,
      &alias->location,
      alias,
      NEW_VARIANT(LilyCheckedDeclType,
                  alias,
                  NEW(LilyCheckedDeclAlias,
                      alias->type.alias.name,
                      format__String(
                        "{S}.{S}", module->global_name, alias->type.alias.name),
                      NULL,
                      NULL,
                      NULL,
                      alias->type.alias.visibility)));

    checked_alias->type.alias.scope =
      NEW(LilyCheckedScope,
          NEW_VARIANT(LilyCheckedParent, module, module->scope, module),
          NEW_VARIANT(LilyCheckedScopeDecls, decl, checked_alias));

    push__Vec(module->decls, checked_alias);
}

void
push_enum__LilyAnalysis(LilyAnalysis *self,
                        LilyAstDecl *enum_,
                        LilyCheckedDeclModule *module)
{
    LilyCheckedDecl *checked_enum = NEW_VARIANT(
      LilyCheckedDecl,
      type,
      &enum_->location,
      enum_,
      NEW_VARIANT(LilyCheckedDeclType,
                  enum,
                  NEW(LilyCheckedDeclEnum,
                      enum_->type.enum_.name,
                      format__String(
                        "{S}.{S}", module->global_name, enum_->type.enum_.name),
                      NULL,
                      NULL,
                      NULL,
                      enum_->type.enum_.visibility)));

    checked_enum->type.enum_.scope =
      NEW(LilyCheckedScope,
          NEW_VARIANT(LilyCheckedParent, module, module->scope, module),
          NEW_VARIANT(LilyCheckedScopeDecls, decl, checked_enum));

    push__Vec(module->decls, checked_enum);
}

void
push_record__LilyAnalysis(LilyAnalysis *self,
                          LilyAstDecl *record,
                          LilyCheckedDeclModule *module)
{
    LilyCheckedDecl *checked_record =
      NEW_VARIANT(LilyCheckedDecl,
                  type,
                  &record->location,
                  record,
                  NEW_VARIANT(LilyCheckedDeclType,
                              record,
                              NEW(LilyCheckedDeclRecord,
                                  record->type.record.name,
                                  format__String("{S}.{S}",
                                                 module->global_name,
                                                 record->type.record.name),
                                  NULL,
                                  NULL,
                                  NULL,
                                  record->type.record.visibility)));

    checked_record->type.record.scope =
      NEW(LilyCheckedScope,
          NEW_VARIANT(LilyCheckedParent, module, module->scope, module),
          NEW_VARIANT(LilyCheckedScopeDecls, decl, checked_record));

    push__Vec(module->decls, checked_record);
}

void
push_all_decls__LilyAnalysis(LilyAnalysis *self,
                             const Vec *decls,
                             LilyCheckedDeclModule *module)
{
    for (Usize i = 0; i < decls->len; ++i) {
        LilyAstDecl *decl = get__Vec(decls, i);

        switch (decl->kind) {
            case LILY_AST_DECL_KIND_CONSTANT: {
                push_constant__LilyAnalysis(self, decl, module);

                LilyCheckedScopeContainerConstant *sc_constant = NEW(
                  LilyCheckedScopeContainerConstant, decl->constant.name, i);

                int status =
                  add_constant__LilyCheckedScope(module->scope, sc_constant);

                if (status) {
                    ANALYSIS_EMIT_DIAGNOSTIC(
                      self,
                      simple_lily_error,
                      (&decl->location),
                      NEW(LilyError, LILY_ERROR_KIND_DUPLICATE_CONSTANT),
                      NULL,
                      NULL,
                      NULL);

                    FREE(LilyCheckedScopeContainerConstant, sc_constant);
                }

                break;
            }
            case LILY_AST_DECL_KIND_ERROR: {
                push_error__LilyAnalysis(self, decl, module);

                LilyCheckedScopeContainerError *sc_error =
                  NEW(LilyCheckedScopeContainerError, decl->error.name, i);

                int status =
                  add_error__LilyCheckedScope(module->scope, sc_error);

                if (status) {
                    ANALYSIS_EMIT_DIAGNOSTIC(
                      self,
                      simple_lily_error,
                      (&decl->location),
                      NEW(LilyError, LILY_ERROR_KIND_DUPLICATE_ERROR),
                      NULL,
                      NULL,
                      NULL);

                    FREE(LilyCheckedScopeContainerError, sc_error);
                }

                break;
            }
            case LILY_AST_DECL_KIND_FUN: {
                if (!decl->fun.object_impl) {
                    LilyCheckedScopeContainerFun *overload_fun =
                      get_fun_in_current_scope__LilyCheckedScope(
                        module->scope, decl->fun.name);

                    push_fun__LilyAnalysis(self, decl, module);

                    if (overload_fun) {
                        push__Vec(overload_fun->ids, (Usize *)i);
                    } else {
                        add_fun__LilyCheckedScope(
                          module->scope,
                          NEW(LilyCheckedScopeContainerFun,
                              decl->fun.name,
                              init__Vec(1, (Usize *)i)));
                    }
                }

                break;
            }
            case LILY_AST_DECL_KIND_MODULE: {
                push_module__LilyAnalysis(self, decl, module, i);

                LilyCheckedScopeContainerModule *sc_module =
                  NEW(LilyCheckedScopeContainerModule, decl->module.name, i);

                int status =
                  add_module__LilyCheckedScope(module->scope, sc_module);

                if (status) {
                    ANALYSIS_EMIT_DIAGNOSTIC(
                      self,
                      simple_lily_error,
                      (&decl->location),
                      NEW(LilyError, LILY_ERROR_KIND_DUPLICATE_MODULE),
                      NULL,
                      NULL,
                      NULL);

                    FREE(LilyCheckedScopeContainerModule, sc_module);
                }

                break;
            }
            case LILY_AST_DECL_KIND_OBJECT: {
                switch (decl->object.kind) {
                    case LILY_AST_DECL_OBJECT_KIND_CLASS: {
                        push_class__LilyAnalysis(self, decl, module);

                        LilyCheckedScopeContainerClass *sc_class =
                          NEW(LilyCheckedScopeContainerClass,
                              decl->object.class.name,
                              i);

                        int status =
                          add_class__LilyCheckedScope(module->scope, sc_class);

                        if (status) {
                            ANALYSIS_EMIT_DIAGNOSTIC(
                              self,
                              simple_lily_error,
                              (&decl->location),
                              NEW(LilyError, LILY_ERROR_KIND_DUPLICATE_CLASS),
                              NULL,
                              NULL,
                              NULL);

                            FREE(LilyCheckedScopeContainerClass, sc_class);
                        }

                        break;
                    }
                    case LILY_AST_DECL_OBJECT_KIND_ENUM: {
                        push_enum_object__LilyAnalysis(self, decl, module);

                        LilyCheckedScopeContainerEnumObject *sc_enum_object =
                          NEW(LilyCheckedScopeContainerEnumObject,
                              decl->object.enum_.name,
                              i);

                        int status = add_enum_object__LilyCheckedScope(
                          module->scope, sc_enum_object);

                        if (status) {
                            ANALYSIS_EMIT_DIAGNOSTIC(
                              self,
                              simple_lily_error,
                              (&decl->location),
                              NEW(LilyError,
                                  LILY_ERROR_KIND_DUPLICATE_ENUM_OBJECT),
                              NULL,
                              NULL,
                              NULL);

                            FREE(LilyCheckedScopeContainerEnumObject,
                                 sc_enum_object);
                        }

                        break;
                    }
                    case LILY_AST_DECL_OBJECT_KIND_RECORD: {
                        push_record_object__LilyAnalysis(self, decl, module);

                        LilyCheckedScopeContainerRecordObject
                          *sc_record_object =
                            NEW(LilyCheckedScopeContainerRecordObject,
                                decl->object.record.name,
                                i);

                        int status = add_record_object__LilyCheckedScope(
                          module->scope, sc_record_object);

                        if (status) {
                            ANALYSIS_EMIT_DIAGNOSTIC(
                              self,
                              simple_lily_error,
                              (&decl->location),
                              NEW(LilyError,
                                  LILY_ERROR_KIND_DUPLICATE_RECORD_OBJECT),
                              NULL,
                              NULL,
                              NULL);

                            FREE(LilyCheckedScopeContainerRecordObject,
                                 sc_record_object);
                        }

                        break;
                    }
                    case LILY_AST_DECL_OBJECT_KIND_TRAIT: {
                        push_trait__LilyAnalysis(self, decl, module);

                        LilyCheckedScopeContainerTrait *sc_trait =
                          NEW(LilyCheckedScopeContainerTrait,
                              decl->object.trait.name,
                              i);

                        int status =
                          add_trait__LilyCheckedScope(module->scope, sc_trait);

                        if (status) {
                            ANALYSIS_EMIT_DIAGNOSTIC(
                              self,
                              simple_lily_error,
                              (&decl->location),
                              NEW(LilyError, LILY_ERROR_KIND_DUPLICATE_TRAIT),
                              NULL,
                              NULL,
                              NULL);

                            FREE(LilyCheckedScopeContainerTrait, sc_trait);
                        }

                        break;
                    }
                    default:
                        UNREACHABLE("unknown variant");
                }

                break;
            }
            case LILY_AST_DECL_KIND_TYPE: {
                switch (decl->type.kind) {
                    case LILY_AST_DECL_TYPE_KIND_ALIAS: {
                        push_alias__LilyAnalysis(self, decl, module);

                        LilyCheckedScopeContainerAlias *sc_alias =
                          NEW(LilyCheckedScopeContainerAlias,
                              decl->type.alias.name,
                              i);

                        int status =
                          add_alias__LilyCheckedScope(module->scope, sc_alias);

                        if (status) {
                            ANALYSIS_EMIT_DIAGNOSTIC(
                              self,
                              simple_lily_error,
                              (&decl->location),
                              NEW(LilyError, LILY_ERROR_KIND_DUPLICATE_ALIAS),
                              NULL,
                              NULL,
                              NULL);

                            FREE(LilyCheckedScopeContainerAlias, sc_alias);
                        }

                        break;
                    }
                    case LILY_AST_DECL_TYPE_KIND_ENUM: {
                        push_enum__LilyAnalysis(self, decl, module);

                        LilyCheckedScopeContainerEnum *sc_enum =
                          NEW(LilyCheckedScopeContainerEnum,
                              decl->type.enum_.name,
                              i);

                        int status =
                          add_enum__LilyCheckedScope(module->scope, sc_enum);

                        if (status) {
                            ANALYSIS_EMIT_DIAGNOSTIC(
                              self,
                              simple_lily_error,
                              (&decl->location),
                              NEW(LilyError, LILY_ERROR_KIND_DUPLICATE_ENUM),
                              NULL,
                              NULL,
                              NULL);

                            FREE(LilyCheckedScopeContainerEnum, sc_enum);
                        }

                        break;
                    }
                    case LILY_AST_DECL_TYPE_KIND_RECORD: {
                        push_record__LilyAnalysis(self, decl, module);

                        LilyCheckedScopeContainerRecord *sc_record =
                          NEW(LilyCheckedScopeContainerRecord,
                              decl->type.record.name,
                              i);

                        int status = add_record__LilyCheckedScope(module->scope,
                                                                  sc_record);

                        if (status) {
                            ANALYSIS_EMIT_DIAGNOSTIC(
                              self,
                              simple_lily_error,
                              (&decl->location),
                              NEW(LilyError, LILY_ERROR_KIND_DUPLICATE_RECORD),
                              NULL,
                              NULL,
                              NULL);

                            FREE(LilyCheckedScopeContainerRecord, sc_record);
                        }

                        break;
                    }
                    default:
                        UNREACHABLE("unknown variant");
                }

                break;
            }
            default:
                UNREACHABLE("unknown variant");
        }
    }
}

LilyCheckedDataType *
check_data_type__LilyAnalysis(LilyAnalysis *self,
                              LilyAstDataType *data_type,
                              LilyCheckedScope *scope,
                              Vec *deps,
                              enum LilyCheckedSafetyMode safety_mode)
{
    switch (data_type->kind) {
        case LILY_AST_DATA_TYPE_KIND_ANY:
            if (safety_mode == LILY_CHECKED_SAFETY_MODE_SAFE) {
                ANALYSIS_EMIT_DIAGNOSTIC(
                  self,
                  simple_lily_error,
                  (&data_type->location),
                  NEW(LilyError, LILY_ERROR_KIND_CANNOT_USE_ANY_IN_SAFE_MODE),
                  NULL,
                  NULL,
                  NULL);
            }

            return NEW(LilyCheckedDataType,
                       LILY_CHECKED_DATA_TYPE_KIND_ANY,
                       &data_type->location);
        case LILY_CHECKED_DATA_TYPE_KIND_ARRAY:
            switch (data_type->array.kind) {
                case LILY_AST_DATA_TYPE_ARRAY_KIND_SIZED:
                    return NEW_VARIANT(
                      LilyCheckedDataType,
                      array,
                      &data_type->location,
                      NEW_VARIANT(LilyCheckedDataTypeArray,
                                  sized,
                                  check_data_type__LilyAnalysis(
                                    self,
                                    data_type->array.data_type,
                                    scope,
                                    deps,
                                    safety_mode),
                                  data_type->array.size));
                default:
                    return NEW_VARIANT(LilyCheckedDataType,
                                       array,
                                       &data_type->location,
                                       NEW(LilyCheckedDataTypeArray,
                                           (enum LilyCheckedDataTypeArrayKind)(
                                             int)data_type->array.kind,
                                           check_data_type__LilyAnalysis(
                                             self,
                                             data_type->array.data_type,
                                             scope,
                                             deps,
                                             safety_mode)));
            }
        case LILY_AST_DATA_TYPE_KIND_BOOL:
            return NEW(LilyCheckedDataType,
                       LILY_CHECKED_DATA_TYPE_KIND_BOOL,
                       &data_type->location);
        case LILY_AST_DATA_TYPE_KIND_BYTE:
            return NEW(LilyCheckedDataType,
                       LILY_CHECKED_DATA_TYPE_KIND_BYTE,
                       &data_type->location);
        case LILY_AST_DATA_TYPE_KIND_BYTES:
            return NEW_VARIANT(
              LilyCheckedDataType, bytes, &data_type->location, -1);
        case LILY_AST_DATA_TYPE_KIND_CHAR:
            return NEW(LilyCheckedDataType,
                       LILY_CHECKED_DATA_TYPE_KIND_CHAR,
                       &data_type->location);
        case LILY_AST_DATA_TYPE_KIND_CSHORT:
            return NEW(LilyCheckedDataType,
                       LILY_CHECKED_DATA_TYPE_KIND_CSHORT,
                       &data_type->location);
        case LILY_AST_DATA_TYPE_KIND_CUSHORT:
            return NEW(LilyCheckedDataType,
                       LILY_CHECKED_DATA_TYPE_KIND_CUSHORT,
                       &data_type->location);
        case LILY_AST_DATA_TYPE_KIND_CINT:
            return NEW(LilyCheckedDataType,
                       LILY_CHECKED_DATA_TYPE_KIND_CINT,
                       &data_type->location);
        case LILY_AST_DATA_TYPE_KIND_CUINT:
            return NEW(LilyCheckedDataType,
                       LILY_CHECKED_DATA_TYPE_KIND_CUINT,
                       &data_type->location);
        case LILY_AST_DATA_TYPE_KIND_CLONG:
            return NEW(LilyCheckedDataType,
                       LILY_CHECKED_DATA_TYPE_KIND_CLONG,
                       &data_type->location);
        case LILY_AST_DATA_TYPE_KIND_CULONG:
            return NEW(LilyCheckedDataType,
                       LILY_CHECKED_DATA_TYPE_KIND_CULONG,
                       &data_type->location);
        case LILY_AST_DATA_TYPE_KIND_CLONGLONG:
            return NEW(LilyCheckedDataType,
                       LILY_CHECKED_DATA_TYPE_KIND_CLONGLONG,
                       &data_type->location);
        case LILY_AST_DATA_TYPE_KIND_CULONGLONG:
            return NEW(LilyCheckedDataType,
                       LILY_CHECKED_DATA_TYPE_KIND_CULONGLONG,
                       &data_type->location);
        case LILY_AST_DATA_TYPE_KIND_CFLOAT:
            return NEW(LilyCheckedDataType,
                       LILY_CHECKED_DATA_TYPE_KIND_CFLOAT,
                       &data_type->location);
        case LILY_AST_DATA_TYPE_KIND_CDOUBLE:
            return NEW(LilyCheckedDataType,
                       LILY_CHECKED_DATA_TYPE_KIND_CDOUBLE,
                       &data_type->location);
        case LILY_AST_DATA_TYPE_KIND_CSTR:
            return NEW(LilyCheckedDataType,
                       LILY_CHECKED_DATA_TYPE_KIND_CSTR,
                       &data_type->location);
        case LILY_AST_DATA_TYPE_KIND_CVOID:
            return NEW(LilyCheckedDataType,
                       LILY_CHECKED_DATA_TYPE_KIND_CVOID,
                       &data_type->location);
        case LILY_AST_DATA_TYPE_KIND_CUSTOM: {
            LilyCheckedScopeResponse custom_dt_response =
              search_custom_type__LilyCheckedScope(scope,
                                                   data_type->custom.name);
            Vec *generics = NULL; // Vec<LilyCheckedDataType*>*?

            if (data_type->custom.generics) {
                generics = NEW(Vec);

                for (Usize i = 0; i < data_type->custom.generics->len; ++i) {
                    LilyCheckedDataType *generic =
                      check_data_type__LilyAnalysis(
                        self,
                        get__Vec(data_type->custom.generics, i),
                        scope,
                        deps,
                        safety_mode);

                    if (data_type) {
                        push__Vec(generics, generic);
                    } else {
                        FREE_BUFFER_ITEMS(
                          generics->buffer, generics->len, LilyCheckedDataType);
                        FREE(Vec, generics);

                        return NEW(LilyCheckedDataType,
                                   LILY_CHECKED_DATA_TYPE_KIND_UNKNOWN,
                                   NULL);
                    }
                }
            }

            switch (custom_dt_response.kind) {
                case LILY_CHECKED_SCOPE_RESPONSE_KIND_NOT_FOUND:
                    ANALYSIS_EMIT_DIAGNOSTIC(
                      self,
                      simple_lily_error,
                      (&data_type->location),
                      NEW(LilyError, LILY_ERROR_KIND_DATA_TYPE_NOT_FOUND),
                      NULL,
                      NULL,
                      NULL);

                    return NEW(LilyCheckedDataType,
                               LILY_CHECKED_DATA_TYPE_KIND_UNKNOWN,
                               &data_type->location);
                case LILY_CHECKED_SCOPE_RESPONSE_KIND_RECORD:
                    if (deps) {
                        push__Vec(deps, custom_dt_response.decl);
                    }

                    return NEW_VARIANT(
                      LilyCheckedDataType,
                      custom,
                      &data_type->location,
                      NEW(
                        LilyCheckedDataTypeCustom,
                        custom_dt_response.scope_container.scope_id,
                        (LilyCheckedAccessScope){
                          .id = custom_dt_response.scope_container.record->id },
                        data_type->custom.name,
                        custom_dt_response.record->global_name,
                        generics,
                        LILY_CHECKED_DATA_TYPE_CUSTOM_KIND_RECORD,
                        custom_dt_response.record->is_recursive));
                case LILY_CHECKED_SCOPE_RESPONSE_KIND_ENUM:
                    if (deps) {
                        push__Vec(deps, custom_dt_response.decl);
                    }

                    return NEW_VARIANT(
                      LilyCheckedDataType,
                      custom,
                      &data_type->location,
                      NEW(
                        LilyCheckedDataTypeCustom,
                        custom_dt_response.scope_container.scope_id,
                        (LilyCheckedAccessScope){
                          .id = custom_dt_response.scope_container.enum_->id },
                        data_type->custom.name,
                        custom_dt_response.enum_->global_name,
                        generics,
                        LILY_CHECKED_DATA_TYPE_CUSTOM_KIND_ENUM,
                        custom_dt_response.enum_->is_recursive));
                case LILY_CHECKED_SCOPE_RESPONSE_KIND_GENERIC:
                    return NEW_VARIANT(
                      LilyCheckedDataType,
                      custom,
                      &data_type->location,
                      NEW(LilyCheckedDataTypeCustom,
                          custom_dt_response.scope_container.scope_id,
                          (LilyCheckedAccessScope){
                            .id =
                              custom_dt_response.scope_container.generic->id },
                          data_type->custom.name,
                          data_type->custom.name,
                          generics,
                          LILY_CHECKED_DATA_TYPE_CUSTOM_KIND_GENERIC,
                          false));
                case LILY_CHECKED_SCOPE_RESPONSE_KIND_RECORD_OBJECT:
                    TODO("check record object data type");
                case LILY_CHECKED_SCOPE_RESPONSE_KIND_ENUM_OBJECT:
                    TODO("check enum object data type");
                case LILY_CHECKED_SCOPE_RESPONSE_KIND_CLASS:
                    TODO("check class data type");
                case LILY_CHECKED_SCOPE_RESPONSE_KIND_TRAIT:
                    TODO("check trait data type");
                default:
                    UNREACHABLE("this situation is impossible");
            }

            TODO("check custom data type");
        }
        case LILY_AST_DATA_TYPE_KIND_EXCEPTION:
            return NEW_VARIANT(
              LilyCheckedDataType,
              exception,
              &data_type->location,
              check_data_type__LilyAnalysis(
                self, data_type->exception, scope, deps, safety_mode));
        case LILY_AST_DATA_TYPE_KIND_FLOAT32:
            return NEW(LilyCheckedDataType,
                       LILY_CHECKED_DATA_TYPE_KIND_FLOAT32,
                       &data_type->location);
        case LILY_AST_DATA_TYPE_KIND_FLOAT64:
            return NEW(LilyCheckedDataType,
                       LILY_CHECKED_DATA_TYPE_KIND_FLOAT64,
                       &data_type->location);
        case LILY_AST_DATA_TYPE_KIND_INT16:
            return NEW(LilyCheckedDataType,
                       LILY_CHECKED_DATA_TYPE_KIND_INT16,
                       &data_type->location);
        case LILY_AST_DATA_TYPE_KIND_INT32:
            return NEW(LilyCheckedDataType,
                       LILY_CHECKED_DATA_TYPE_KIND_INT32,
                       &data_type->location);
        case LILY_AST_DATA_TYPE_KIND_INT64:
            return NEW(LilyCheckedDataType,
                       LILY_CHECKED_DATA_TYPE_KIND_INT64,
                       &data_type->location);
        case LILY_AST_DATA_TYPE_KIND_INT8:
            return NEW(LilyCheckedDataType,
                       LILY_CHECKED_DATA_TYPE_KIND_INT8,
                       &data_type->location);
        case LILY_AST_DATA_TYPE_KIND_ISIZE:
            return NEW(LilyCheckedDataType,
                       LILY_CHECKED_DATA_TYPE_KIND_ISIZE,
                       &data_type->location);
        case LILY_AST_DATA_TYPE_KIND_LAMBDA:
            TODO("check lambda");
        case LILY_AST_DATA_TYPE_KIND_LIST:
            return NEW_VARIANT(
              LilyCheckedDataType,
              list,
              &data_type->location,
              check_data_type__LilyAnalysis(
                self, data_type->list, scope, deps, safety_mode));
        case LILY_AST_DATA_TYPE_KIND_MUT:
            return NEW_VARIANT(
              LilyCheckedDataType,
              mut,
              &data_type->location,
              check_data_type__LilyAnalysis(
                self, data_type->mut, scope, deps, safety_mode));
        case LILY_AST_DATA_TYPE_KIND_NEVER:
            return NEW(LilyCheckedDataType,
                       LILY_CHECKED_DATA_TYPE_KIND_NEVER,
                       &data_type->location);
        case LILY_AST_DATA_TYPE_KIND_OBJECT:
            TODO("check Object data type");
        case LILY_AST_DATA_TYPE_KIND_OPTIONAL:
            return NEW_VARIANT(
              LilyCheckedDataType,
              optional,
              &data_type->location,
              check_data_type__LilyAnalysis(
                self, data_type->optional, scope, deps, safety_mode));
        case LILY_AST_DATA_TYPE_KIND_PTR:
            switch (data_type->ptr->kind) {
                case LILY_AST_DATA_TYPE_KIND_MUT:
                    return NEW_VARIANT(
                      LilyCheckedDataType,
                      ptr_mut,
                      &data_type->location,
                      check_data_type__LilyAnalysis(
                        self, data_type->ptr->mut, scope, deps, safety_mode));
                default:
                    return NEW_VARIANT(
                      LilyCheckedDataType,
                      ptr,
                      &data_type->location,
                      check_data_type__LilyAnalysis(
                        self, data_type->ptr, scope, deps, safety_mode));
            }
        case LILY_AST_DATA_TYPE_KIND_REF:
            switch (data_type->ref->kind) {
                case LILY_AST_DATA_TYPE_KIND_MUT:
                    return NEW_VARIANT(
                      LilyCheckedDataType,
                      ref_mut,
                      &data_type->location,
                      check_data_type__LilyAnalysis(
                        self, data_type->ref->mut, scope, deps, safety_mode));
                default:
                    return NEW_VARIANT(
                      LilyCheckedDataType,
                      ref,
                      &data_type->location,
                      check_data_type__LilyAnalysis(
                        self, data_type->ref, scope, deps, safety_mode));
            }
        case LILY_AST_DATA_TYPE_KIND_SELF:
            TODO("Check Self data type");
        case LILY_AST_DATA_TYPE_KIND_STR:
            return NEW_VARIANT(
              LilyCheckedDataType, str, &data_type->location, -1);
        case LILY_AST_DATA_TYPE_KIND_TRACE:
            switch (data_type->trace->kind) {
                case LILY_CHECKED_DATA_TYPE_KIND_MUT:
                    return NEW_VARIANT(
                      LilyCheckedDataType,
                      trace_mut,
                      &data_type->location,
                      check_data_type__LilyAnalysis(
                        self, data_type->trace->mut, scope, deps, safety_mode));
                default:
                    return NEW_VARIANT(
                      LilyCheckedDataType,
                      trace,
                      &data_type->location,
                      check_data_type__LilyAnalysis(
                        self, data_type->trace, scope, deps, safety_mode));
            }
        case LILY_AST_DATA_TYPE_KIND_TUPLE: {
            Vec *tuple = NEW(Vec);

            for (Usize i = 0; i < data_type->tuple->len; ++i) {
                push__Vec(
                  tuple,
                  check_data_type__LilyAnalysis(self,
                                                get__Vec(data_type->tuple, i),
                                                scope,
                                                deps,
                                                safety_mode));
            }

            return NEW_VARIANT(
              LilyCheckedDataType, tuple, &data_type->location, tuple);
        }
        case LILY_AST_DATA_TYPE_KIND_UINT16:
            return NEW(LilyCheckedDataType,
                       LILY_CHECKED_DATA_TYPE_KIND_UINT16,
                       &data_type->location);
        case LILY_AST_DATA_TYPE_KIND_UINT32:
            return NEW(LilyCheckedDataType,
                       LILY_CHECKED_DATA_TYPE_KIND_UINT32,
                       &data_type->location);
        case LILY_AST_DATA_TYPE_KIND_UINT64:
            return NEW(LilyCheckedDataType,
                       LILY_CHECKED_DATA_TYPE_KIND_UINT64,
                       &data_type->location);
        case LILY_AST_DATA_TYPE_KIND_UINT8:
            return NEW(LilyCheckedDataType,
                       LILY_CHECKED_DATA_TYPE_KIND_UINT8,
                       &data_type->location);
        case LILY_AST_DATA_TYPE_KIND_UNIT:
            return NEW(LilyCheckedDataType,
                       LILY_CHECKED_DATA_TYPE_KIND_UNIT,
                       &data_type->location);
        case LILY_AST_DATA_TYPE_KIND_USIZE:
            return NEW(LilyCheckedDataType,
                       LILY_CHECKED_DATA_TYPE_KIND_USIZE,
                       &data_type->location);
        default:
            UNREACHABLE("unknown variant");
    }
}

static LilyCheckedExpr *
check_identifier_expr__LilyAnalysis(LilyAnalysis *self,
                                    LilyAstExpr *expr,
                                    LilyCheckedScope *scope,
                                    LilyCheckedDataType *defined_data_type,
                                    enum LilyCheckedSafetyMode safety_mode,
                                    bool must_mut)
{
    ASSERT(expr->kind == LILY_AST_EXPR_KIND_IDENTIFIER);

    LilyCheckedScopeResponse response =
      search_identifier__LilyCheckedScope(scope, expr->identifier.name);

    if (response.kind == LILY_CHECKED_SCOPE_RESPONSE_KIND_NOT_FOUND) {
        ANALYSIS_EMIT_DIAGNOSTIC(
          self,
          simple_lily_error,
          (&expr->location),
          NEW(LilyError, LILY_ERROR_KIND_IDENTIFIER_NOT_FOUND),
          NULL,
          NULL,
          NULL);

        return NEW_VARIANT(LilyCheckedExpr,
                           call,
                           &expr->location,
                           NEW(LilyCheckedDataType,
                               LILY_CHECKED_DATA_TYPE_KIND_UNKNOWN,
                               &expr->location),
                           expr,
                           NEW(LilyCheckedExprCall,
                               LILY_CHECKED_EXPR_CALL_KIND_UNKNOWN,
                               NULL,
                               (LilyCheckedAccessScope){ .id = 0 }));
    } else {
        LilyCheckedDataType *data_type = NULL;

        switch (response.kind) {
            case LILY_CHECKED_SCOPE_RESPONSE_KIND_VARIABLE:
                // if (response.variable->is_moved) {
                //     emit__Diagnostic(
                //       NEW_VARIANT(
                //         Diagnostic,
                //         simple_lily_error,
                //         &self->package->file,
                //         &expr->location,
                //         NEW(LilyError, LILY_ERROR_KIND_VALUE_HAS_BEEN_MOVED),
                //         NULL,
                //         NULL,
                //         NULL),
                //       &self->package->count_error);
                // }

                // Check if the variable is mutable.
                if (must_mut) {
                    if (!response.variable->is_mut) {
                        ANALYSIS_EMIT_DIAGNOSTIC(
                          self,
                          simple_lily_error,
                          (&expr->location),
                          NEW(LilyError,
                              LILY_ERROR_KIND_EXPECTED_MUTABLE_VARIABLE),
                          NULL,
                          NULL,
                          NULL);
                    }
                }

                data_type =
                  ref__LilyCheckedDataType(response.variable->data_type);

                return NEW_VARIANT(
                  LilyCheckedExpr,
                  call,
                  &expr->location,
                  data_type,
                  expr,
                  NEW(LilyCheckedExprCall,
                      LILY_CHECKED_EXPR_CALL_KIND_VARIABLE,
                      response.variable->name,
                      (LilyCheckedAccessScope){
                        .id = response.scope_container.scope_id }));
            case LILY_CHECKED_SCOPE_RESPONSE_KIND_FUN:
                // for fun passed as parameter
                TODO("found fun!!");
            case LILY_CHECKED_SCOPE_RESPONSE_KIND_CONSTANT:
                if (!response.constant->is_checked) {
                    check_constant__LilyAnalysis(
                      self,
                      get_decl_from_id__LilyCheckedScope(
                        scope,
                        response.scope_container.scope_id,
                        response.scope_container.constant->id),
                      get_scope_from_id__LilyCheckedScope(
                        scope, response.scope_container.scope_id));
                }

                data_type =
                  ref__LilyCheckedDataType(response.constant->data_type);

                return NEW_VARIANT(
                  LilyCheckedExpr,
                  call,
                  &expr->location,
                  data_type,
                  expr,
                  NEW(LilyCheckedExprCall,
                      LILY_CHECKED_EXPR_CALL_KIND_CONSTANT,
                      response.constant->global_name,
                      (LilyCheckedAccessScope){
                        .id = response.scope_container.scope_id }));
            case LILY_CHECKED_SCOPE_RESPONSE_KIND_FUN_PARAM:
                // if (response.fun_param->is_moved) {
                //     emit__Diagnostic(
                //       NEW_VARIANT(
                //         Diagnostic,
                //         simple_lily_error,
                //         &self->package->file,
                //         &expr->location,
                //         NEW(LilyError, LILY_ERROR_KIND_VALUE_HAS_BEEN_MOVED),
                //         NULL,
                //         NULL,
                //         NULL),
                //       &self->package->count_error);
                // }

                // Check if the variable is mutable.
                if (must_mut) {
                    // TODO: check if the param can be mutable.
                    // if (!response.fun_param->is_mut) {
                    //     emit__Diagnostic(
                    //       NEW_VARIANT(
                    //         Diagnostic,
                    //         simple_lily_error,
                    //         &self->package->file,
                    //         &expr->location,
                    //         NEW(LilyError,
                    //             LILY_ERROR_KIND_EXPECTED_MUTABLE_VARIABLE),
                    //         NULL,
                    //         NULL,
                    //         NULL),
                    //       &self->package->count_error);
                    // }
                }

                switch (response.fun_param->data_type->kind) {
                    case LILY_CHECKED_DATA_TYPE_KIND_UNKNOWN:
                        if (defined_data_type) {
                            if (defined_data_type->kind !=
                                LILY_CHECKED_DATA_TYPE_KIND_UNKNOWN) {
                                update_data_type__LilyCheckedDataType(
                                  response.fun_param->data_type,
                                  defined_data_type);
                                data_type = ref__LilyCheckedDataType(
                                  response.fun_param->data_type);
                            } else {
                                goto unknown_defined_data_type;
                            }
                        } else {
                        unknown_defined_data_type : {
                            LilyCheckedScopeDecls *current_fun =
                              get_current_fun__LilyCheckedScope(scope);

                            add_compiler_generic__LilyCheckedCompilerGeneric(
                              current_fun->decl->fun.used_compiler_generic);

                            response.fun_param->data_type->kind =
                              LILY_CHECKED_DATA_TYPE_KIND_COMPILER_GENERIC;
                            response.fun_param->data_type->compiler_generic =
                              NEW(LilyCheckedDataTypeCompilerGeneric,
                                  last__Vec(current_fun->decl->fun
                                              .used_compiler_generic));

                            data_type = ref__LilyCheckedDataType(
                              response.fun_param->data_type);
                        }

                        break;
                        default:
                            data_type = ref__LilyCheckedDataType(
                              response.fun_param->data_type);
                        }
                }

                return NEW_VARIANT(
                  LilyCheckedExpr,
                  call,
                  &expr->location,
                  data_type,
                  expr,
                  NEW_VARIANT(LilyCheckedExprCall,
                              fun_param,
                              (LilyCheckedAccessScope){
                                .id = response.scope_container.scope_id },
                              response.fun_param->name,
                              response.scope_container.variable->id));
            case LILY_CHECKED_SCOPE_RESPONSE_KIND_ENUM: {
                Vec *generic_params = NULL; // Vec<LilyCheckedDataType*>*?

                // Check generic params
                if (expr->identifier.generic_params) {
                    generic_params = NEW(Vec);

                    for (Usize i = 0; i < expr->identifier.generic_params->len;
                         ++i) {
                        LilyCheckedDataType *data_type =
                          check_data_type__LilyAnalysis(
                            self,
                            get__Vec(expr->identifier.generic_params, i),
                            scope,
                            NULL,
                            safety_mode);

                        if (data_type) {
                            push__Vec(generic_params, data_type);
                        } else {
                            FREE_BUFFER_ITEMS(generic_params->buffer,
                                              generic_params->len,
                                              LilyCheckedDataType);
                            FREE(Vec, generic_params);

                            return NEW_VARIANT(
                              LilyCheckedExpr,
                              call,
                              &expr->location,
                              NEW(LilyCheckedDataType,
                                  LILY_CHECKED_DATA_TYPE_KIND_UNKNOWN,
                                  &expr->location),
                              expr,
                              NEW(LilyCheckedExprCall,
                                  LILY_CHECKED_EXPR_CALL_KIND_UNKNOWN,
                                  NULL,
                                  (LilyCheckedAccessScope){ .id = 0 }));
                        }
                    }
                }

                LilyCheckedDataType *data_type =
                  NEW_VARIANT(LilyCheckedDataType,
                              custom,
                              &expr->location,
                              NEW(LilyCheckedDataTypeCustom,
                                  response.scope_container.scope_id,
                                  (LilyCheckedAccessScope){
                                    .id = response.enum_->scope->id },
                                  response.enum_->name,
                                  response.enum_->global_name,
                                  generic_params,
                                  LILY_CHECKED_DATA_TYPE_CUSTOM_KIND_ENUM,
                                  response.enum_->is_recursive));

                return NEW_VARIANT(
                  LilyCheckedExpr,
                  call,
                  &expr->location,
                  data_type,
                  expr,
                  NEW_VARIANT(LilyCheckedExprCall,
                              enum,
                              (LilyCheckedAccessScope){
                                .id = response.scope_container.scope_id },
                              response.enum_->global_name,
                              response.decl));
            }
            case LILY_CHECKED_SCOPE_RESPONSE_KIND_ENUM_VARIANT:
                ASSERT(defined_data_type);
                ASSERT(defined_data_type->ref_count == 0);

                return NEW_VARIANT(
                  LilyCheckedExpr,
                  call,
                  &expr->location,
                  ref__LilyCheckedDataType(defined_data_type),
                  expr,
                  NEW_VARIANT(LilyCheckedExprCall,
                              variant,
                              (LilyCheckedAccessScope){
                                .id = response.scope_container.scope_id },
                              response.enum_variant->global_name,
                              NEW(LilyCheckedExprCallVariant, NULL)));
            default:
                ANALYSIS_EMIT_DIAGNOSTIC(
                  self,
                  simple_lily_error,
                  (&expr->location),
                  NEW(LilyError,
                      LILY_ERROR_KIND_CALL_NOT_EXPECTED_IN_THIS_CONTEXT),
                  NULL,
                  NULL,
                  NULL);

                return NEW_VARIANT(LilyCheckedExpr,
                                   call,
                                   &expr->location,
                                   data_type,
                                   expr,
                                   NEW(LilyCheckedExprCall,
                                       LILY_CHECKED_EXPR_CALL_KIND_UNKNOWN,
                                       NULL,
                                       (LilyCheckedAccessScope){ .id = 0 }));
        }
    }
}

LilyCheckedExpr *
check_binary_expr__LilyAnalysis(LilyAnalysis *self,
                                LilyAstExpr *expr,
                                LilyCheckedScope *scope,
                                enum LilyCheckedSafetyMode safety_mode,
                                LilyCheckedDataType *defined_data_type)
{
    switch (expr->binary.kind) {
        case LILY_AST_EXPR_BINARY_KIND_BIT_AND:
        case LILY_AST_EXPR_BINARY_KIND_BIT_L_SHIFT:
        case LILY_AST_EXPR_BINARY_KIND_BIT_OR:
        case LILY_AST_EXPR_BINARY_KIND_BIT_R_SHIFT:
        case LILY_AST_EXPR_BINARY_KIND_GREATER:
        case LILY_AST_EXPR_BINARY_KIND_GREATER_EQ:
        case LILY_AST_EXPR_BINARY_KIND_LESS:
        case LILY_AST_EXPR_BINARY_KIND_LESS_EQ:
        case LILY_AST_EXPR_BINARY_KIND_ADD:
        case LILY_AST_EXPR_BINARY_KIND_DIV:
        case LILY_AST_EXPR_BINARY_KIND_MOD:
        case LILY_AST_EXPR_BINARY_KIND_MUL:
        case LILY_AST_EXPR_BINARY_KIND_EXP:
        case LILY_AST_EXPR_BINARY_KIND_SUB:
        case LILY_AST_EXPR_BINARY_KIND_AND:
        case LILY_AST_EXPR_BINARY_KIND_OR:
        case LILY_AST_EXPR_BINARY_KIND_XOR:
        case LILY_AST_EXPR_BINARY_KIND_EQ:
        case LILY_AST_EXPR_BINARY_KIND_NOT_EQ:
        case LILY_AST_EXPR_BINARY_KIND_RANGE:
        case LILY_AST_EXPR_BINARY_KIND_LIST_TAIL:
        case LILY_AST_EXPR_BINARY_KIND_LIST_HEAD: {
            LilyCheckedExpr *left = check_expr__LilyAnalysis(
              self, expr->binary.left, scope, safety_mode, false, NULL);
            LilyCheckedExpr *right = check_expr__LilyAnalysis(
              self, expr->binary.right, scope, safety_mode, false, NULL);

            return typecheck_binary_expr__LilyAnalysis(
              self,
              expr,
              scope,
              left,
              right,
              (enum LilyCheckedExprBinaryKind)(int)expr->binary.kind,
              safety_mode,
              defined_data_type);
        }
        case LILY_AST_EXPR_BINARY_KIND_ASSIGN_ADD:
        case LILY_AST_EXPR_BINARY_KIND_ASSIGN_BIT_AND:
        case LILY_AST_EXPR_BINARY_KIND_ASSIGN_BIT_L_SHIFT:
        case LILY_AST_EXPR_BINARY_KIND_ASSIGN_BIT_OR:
        case LILY_AST_EXPR_BINARY_KIND_ASSIGN_BIT_R_SHIFT:
        case LILY_AST_EXPR_BINARY_KIND_ASSIGN_DIV:
        case LILY_AST_EXPR_BINARY_KIND_ASSIGN_EXP:
        case LILY_AST_EXPR_BINARY_KIND_ASSIGN_MOD:
        case LILY_AST_EXPR_BINARY_KIND_ASSIGN_MUL:
        case LILY_AST_EXPR_BINARY_KIND_ASSIGN_SUB:
        case LILY_AST_EXPR_BINARY_KIND_ASSIGN_XOR:
        case LILY_AST_EXPR_BINARY_KIND_ASSIGN: {
            LilyCheckedExpr *left = check_expr__LilyAnalysis(
              self, expr->binary.left, scope, safety_mode, true, NULL);
            LilyCheckedExpr *right = check_expr__LilyAnalysis(
              self, expr->binary.right, scope, safety_mode, false, NULL);

            return typecheck_binary_expr__LilyAnalysis(
              self,
              expr,
              scope,
              left,
              right,
              (enum LilyCheckedExprBinaryKind)(int)expr->binary.kind,
              safety_mode,
              defined_data_type);
        };
        case LILY_AST_EXPR_BINARY_KIND_CHAIN: {
            switch (expr->binary.right->kind) {
                case LILY_AST_EXPR_KIND_CALL:
                    switch (expr->binary.right->call.kind) {
                        case LILY_AST_EXPR_CALL_KIND_FUN:
                            if (expr->binary.right->call.fun.params->len == 0) {
                                push__Vec(
                                  expr->binary.right->call.fun.params,
                                  NEW_VARIANT(LilyAstExprFunParamCall,
                                              normal,
                                              expr->binary.left,
                                              clone__Location(
                                                &expr->binary.left->location)));
                            } else {
                                for (Usize i =
                                       expr->binary.right->call.fun.params->len;
                                     i--;) {
                                    if (
                                      CAST(
                                        LilyAstExprFunParamCall *,
                                        get__Vec(
                                          expr->binary.right->call.fun.params,
                                          i))
                                        ->kind ==
                                      LILY_AST_EXPR_FUN_PARAM_CALL_KIND_NORMAL) {
                                        insert_after__Vec(
                                          expr->binary.right->call.fun.params,
                                          NEW_VARIANT(
                                            LilyAstExprFunParamCall,
                                            normal,
                                            expr->binary.left,
                                            clone__Location(
                                              &expr->binary.left->location)),
                                          i);

                                        break;
                                    }
                                }
                            }

                            break;
                        default:
                        expected_fun_call : {
                            ANALYSIS_EMIT_DIAGNOSTIC(
                              self,
                              simple_lily_error,
                              (&expr->location),
                              NEW(LilyError, LILY_ERROR_KIND_EXPECTED_FUN_CALL),
                              NULL,
                              NULL,
                              NULL);

                            LilyCheckedExpr *left =
                              check_expr__LilyAnalysis(self,
                                                       expr->binary.left,
                                                       scope,
                                                       safety_mode,
                                                       false,
                                                       NULL);
                            LilyCheckedExpr *right =
                              check_expr__LilyAnalysis(self,
                                                       expr->binary.right,
                                                       scope,
                                                       safety_mode,
                                                       false,
                                                       NULL);

                            return NEW_VARIANT(
                              LilyCheckedExpr,
                              binary,
                              &expr->location,
                              ref__LilyCheckedDataType(right->data_type),
                              expr,
                              NEW(LilyCheckedExprBinary,
                                  LILY_CHECKED_EXPR_BINARY_KIND_CHAIN,
                                  left,
                                  right));
                        }
                    }

                    break;
                default:
                    goto expected_fun_call;
            }

            return check_expr__LilyAnalysis(self,
                                            expr->binary.right,
                                            scope,
                                            safety_mode,
                                            false,
                                            defined_data_type);
        }
        default:
            UNREACHABLE("unknown variant");
    }
}

LilyCheckedExpr *
typecheck_binary_expr__LilyAnalysis(LilyAnalysis *self,
                                    LilyAstExpr *expr,
                                    LilyCheckedScope *scope,
                                    LilyCheckedExpr *left,
                                    LilyCheckedExpr *right,
                                    enum LilyCheckedExprBinaryKind kind,
                                    enum LilyCheckedSafetyMode safety_mode,
                                    LilyCheckedDataType *defined_data_type)
{
    // TODO: add support to avoid duplication of data type in compiler choice
    // (not in conditional compiler choice)
    char *binary_kind_string = to_string__LilyCheckedExprBinaryKind(kind);
    bool defined_data_type_is_null = defined_data_type ? 0 : 1;

    Vec *operators = collect_all_operators__LilyCheckedOperatorRegister(
      &self->package->operator_register, binary_kind_string, 3);

    typecheck_binary__LilyCheckedOperatorRegister(operators,
                                                  &expr->location,
                                                  left->data_type,
                                                  right->data_type,
                                                  &defined_data_type);

    FREE(Vec, operators);

    return NEW_VARIANT(LilyCheckedExpr,
                       binary,
                       &expr->location,
                       defined_data_type_is_null
                         ? defined_data_type
                         : clone__LilyCheckedDataType(defined_data_type),
                       expr,
                       NEW(LilyCheckedExprBinary, kind, left, right));
}

#define REANALYZE_BODY(body, virtual_scope)                     \
    for (Usize i = 0; i < body->len; ++i) {                     \
        LilyCheckedBodyFunItem *item = get__Vec(body, i);       \
                                                                \
        switch (item->kind) {                                   \
            case LILY_CHECKED_BODY_FUN_ITEM_KIND_EXPR:          \
                reanalyze_expr__LilyAnalysis(self,              \
                                             signature,         \
                                             item->expr,        \
                                             virtual_scope,     \
                                             return_data_type); \
                break;                                          \
            case LILY_CHECKED_BODY_FUN_ITEM_KIND_STMT:          \
                reanalyze_stmt__LilyAnalysis(self,              \
                                             signature,         \
                                             &item->stmt,       \
                                             virtual_scope,     \
                                             return_data_type); \
                break;                                          \
            default:                                            \
                UNREACHABLE("unknown variant");                 \
        }                                                       \
    }

void
reanalyze_fun_call_with_signature__LilyAnalysis(
  LilyAnalysis *self,
  LilyCheckedSignatureFun *signature,
  LilyCheckedDecl *fun)
{
    LilyCheckedVirtualScope *root_virtual_scope =
      NEW(LilyCheckedVirtualScope, NULL);
    LilyCheckedDataType *return_data_type = last__Vec(signature->types);

    ASSERT(signature->types->len - 1 == fun->fun.params->len);

    // Push params
    for (Usize i = 0; i < fun->fun.params->len; ++i) {
        LilyCheckedDataType *given_type = get__Vec(signature->types, i);

#ifdef ENV_DEBUG
        LilyCheckedDataType *original_param_data_type =
          get__Vec(signature->types, i);
        LilyCheckedDataType *resolved_data_type =
          resolve_generic_data_type_with_hash_map__LilyCheckedDataType(
            original_param_data_type, signature->generic_params);

        if (!resolved_data_type) {
            FAILED("generic resolve has been failed");
        }

        if (!eq__LilyCheckedDataType(resolved_data_type, given_type)) {
            FAILED("resolved data type no match with the given data type");
        }

        FREE(LilyCheckedDataType, resolved_data_type);
#endif

        add_fun_param__LilyCheckedVirtualScope(root_virtual_scope,
                                               NEW(LilyCheckedVirtualFunParam,
                                                   get__Vec(fun->fun.params, i),
                                                   given_type));
    }

    // Re-analyze fun body
    REANALYZE_BODY(fun->fun.body, root_virtual_scope);

    FREE(LilyCheckedVirtualScope, root_virtual_scope);
}

static LilyCheckedDataType *
reanalyze_expr__LilyAnalysis(LilyAnalysis *self,
                             LilyCheckedSignatureFun *signature,
                             const LilyCheckedExpr *expr,
                             LilyCheckedVirtualScope *virtual_scope,
                             LilyCheckedDataType *return_data_type)
{
    switch (expr->kind) {
        case LILY_CHECKED_EXPR_KIND_CALL:
            switch (expr->call.kind) {
                case LILY_CHECKED_EXPR_CALL_KIND_FUN: {
                    HashMap *generic_params =
                      NULL; // HashMap<LilyCheckedDataType*>*?

                    if (expr->call.fun.generic_params) {
                        generic_params = NEW(HashMap);

                        HashMapIter iter =
                          NEW(HashMapIter, expr->call.fun.generic_params);
                        LilyCheckedDataType *current = NULL;
                        Usize count = 0;

                        while ((current = next__HashMapIter(&iter))) {
                            LilyCheckedDataType *resolved_data_type =
                              resolve_generic_data_type_with_hash_map__LilyCheckedDataType(
                                current, signature->generic_params);

                            ASSERT(resolved_data_type);

                            insert__HashMap(
                              generic_params,
                              get_name__LilyCheckedGenericParam(
                                get__Vec(expr->call.fun.fun->fun.generic_params,
                                         count))
                                ->buffer,
                              resolved_data_type);

                            ++count;
                        }
                    }

                    Vec *fun_types = NEW(Vec); // Vec<LilyCheckedDataType* (&)>*

                    if (expr->call.fun.params) {
                        for (Usize i = 0; i < expr->call.fun.params->len; ++i) {
                            push__Vec(fun_types,
                                      reanalyze_expr__LilyAnalysis(
                                        self,
                                        signature,
                                        CAST(LilyCheckedExprCallFunParam *,
                                             get__Vec(expr->call.fun.params, i))
                                          ->value,
                                        virtual_scope,
                                        return_data_type));
                        }
                    }

                    // Push the return data type
                    switch (expr->call.fun.fun->fun.return_data_type->kind) {
                        case LILY_CHECKED_DATA_TYPE_KIND_CONDITIONAL_COMPILER_CHOICE: {
                            LilyCheckedDataType *return_data_type =
                              get_return_data_type_of_conditional_compiler_choice(
                                expr->call.fun.fun->fun.return_data_type,
                                fun_types);

                            ASSERT(return_data_type);

                            push__Vec(
                              fun_types,
                              ref__LilyCheckedDataType(return_data_type));

                            break;
                        }
                        default: {
                            LilyCheckedDataType *return_data_type =
                              resolve_generic_data_type_with_hash_map__LilyCheckedDataType(
                                expr->call.fun.fun->fun.return_data_type,
                                generic_params);

                            ASSERT(return_data_type);

                            push__Vec(fun_types, return_data_type);
                        }
                    }

                    if (add_signature__LilyCheckedDeclFun(
                          &expr->call.fun.fun->fun,
                          fun_types,
                          generic_params)) {
                        LilyCheckedDataType *return_data_type =
                          last__Vec(fun_types);

                        FREE(LilyCheckedDataType, return_data_type);
                        FREE(Vec, fun_types);

                        return return_data_type;
                    } else {
                        LilyCheckedSignatureFun *signature =
                          get_signature__LilyCheckedDeclFun(
                            &expr->call.fun.fun->fun,
                            expr->call.fun.fun->fun.global_name,
                            fun_types);

                        ASSERT(signature);

                        reanalyze_fun_call_with_signature__LilyAnalysis(
                          self, signature, expr->call.fun.fun);

                        return last__Vec(fun_types);
                    }
                }
                case LILY_CHECKED_EXPR_CALL_KIND_VARIABLE:
                    return search_variable__LilyCheckedVirtualScope(
                             virtual_scope, expr->call.global_name)
                      ->virtual_data_type;
                case LILY_CHECKED_EXPR_CALL_KIND_FUN_PARAM:
                    return search_fun_param__LilyCheckedVirtualScope(
                             virtual_scope, expr->call.global_name)
                      ->virtual_data_type;
                default:
                    return expr->data_type;
            }

            break;
        default:
            return expr->data_type;
    }
}

static void
reanalyze_stmt__LilyAnalysis(LilyAnalysis *self,
                             LilyCheckedSignatureFun *signature,
                             const LilyCheckedStmt *stmt,
                             LilyCheckedVirtualScope *virtual_scope,
                             LilyCheckedDataType *return_data_type)
{
    switch (stmt->kind) {
        case LILY_CHECKED_STMT_KIND_AWAIT:
            reanalyze_expr__LilyAnalysis(self,
                                         signature,
                                         stmt->await.expr,
                                         virtual_scope,
                                         return_data_type);

            break;
        case LILY_CHECKED_STMT_KIND_BLOCK: {
            LilyCheckedVirtualScope *block_virtual_scope =
              NEW(LilyCheckedVirtualScope, virtual_scope);

            REANALYZE_BODY(stmt->block.body, block_virtual_scope);

            FREE(LilyCheckedVirtualScope, block_virtual_scope);

            break;
        }
        case LILY_CHECKED_STMT_KIND_DROP:
            reanalyze_expr__LilyAnalysis(self,
                                         signature,
                                         stmt->drop.expr,
                                         virtual_scope,
                                         return_data_type);

            break;
        case LILY_CHECKED_STMT_KIND_FOR: {
            LilyCheckedVirtualScope *for_virtual_scope =
              NEW(LilyCheckedVirtualScope, virtual_scope);

            REANALYZE_BODY(stmt->for_.body, for_virtual_scope);

            FREE(LilyCheckedVirtualScope, for_virtual_scope);

            break;
        }
        case LILY_CHECKED_STMT_KIND_IF: {
            LilyCheckedVirtualScope *if_virtual_scope =
              NEW(LilyCheckedVirtualScope, virtual_scope);

            REANALYZE_BODY(stmt->if_.if_->body, if_virtual_scope);

            if (stmt->if_.elifs) {
                for (Usize i = 0; i < stmt->if_.elifs->len; ++i) {
                    LilyCheckedStmtIfBranch *elif =
                      get__Vec(stmt->if_.elifs, i);
                    LilyCheckedVirtualScope *elif_virtual_scope =
                      NEW(LilyCheckedVirtualScope, virtual_scope);

                    REANALYZE_BODY(elif->body, elif_virtual_scope);

                    FREE(LilyCheckedVirtualScope, elif_virtual_scope);
                }
            }

            if (stmt->if_.else_) {
                LilyCheckedVirtualScope *else_virtual_scope =
                  NEW(LilyCheckedVirtualScope, virtual_scope);

                REANALYZE_BODY(stmt->if_.else_->body, else_virtual_scope);

                FREE(LilyCheckedVirtualScope, else_virtual_scope);
            }

            FREE(LilyCheckedVirtualScope, if_virtual_scope);

            break;
        }
        case LILY_CHECKED_STMT_KIND_MATCH: {
            for (Usize i = 0; i < stmt->match.cases->len; ++i) {
                LilyCheckedStmtMatchCase *case_ =
                  get__Vec(stmt->match.cases, i);
                LilyCheckedVirtualScope *case_virtual_scope =
                  NEW(LilyCheckedVirtualScope, virtual_scope);

                switch (case_->body_item->kind) {
                    case LILY_CHECKED_BODY_FUN_ITEM_KIND_EXPR:
                        reanalyze_expr__LilyAnalysis(self,
                                                     signature,
                                                     case_->body_item->expr,
                                                     case_virtual_scope,
                                                     return_data_type);
                        break;
                    case LILY_CHECKED_BODY_FUN_ITEM_KIND_STMT:
                        reanalyze_stmt__LilyAnalysis(self,
                                                     signature,
                                                     &case_->body_item->stmt,
                                                     case_virtual_scope,
                                                     return_data_type);
                        break;
                    default:
                        UNREACHABLE("unknown variant");
                }

                FREE(LilyCheckedVirtualScope, case_virtual_scope);
            }

            break;
        }
        case LILY_CHECKED_STMT_KIND_RETURN: {
            LilyCheckedDataType *expr_data_type =
              reanalyze_expr__LilyAnalysis(self,
                                           signature,
                                           stmt->return_.expr,
                                           virtual_scope,
                                           return_data_type);

            if (return_data_type->kind == LILY_CHECKED_DATA_TYPE_KIND_UNKNOWN ||
                return_data_type->kind ==
                  LILY_CHECKED_DATA_TYPE_KIND_COMPILER_GENERIC) {
                update_data_type__LilyCheckedDataType(return_data_type,
                                                      expr_data_type);
            }

            break;
        }
        case LILY_CHECKED_STMT_KIND_TRY: {
            LilyCheckedVirtualScope *try_virtual_scope =
              NEW(LilyCheckedVirtualScope, virtual_scope);
            LilyCheckedVirtualScope *catch_virtual_scope =
              NEW(LilyCheckedVirtualScope, virtual_scope);

            REANALYZE_BODY(stmt->try.try_body, try_virtual_scope);
            REANALYZE_BODY(stmt->try.catch_body, catch_virtual_scope);

            FREE(LilyCheckedVirtualScope, try_virtual_scope);
            FREE(LilyCheckedVirtualScope, catch_virtual_scope);

            break;
        }
        case LILY_CHECKED_STMT_KIND_UNSAFE: {
            LilyCheckedVirtualScope *unsafe_virtual_scope =
              NEW(LilyCheckedVirtualScope, virtual_scope);

            REANALYZE_BODY(stmt->unsafe.body, unsafe_virtual_scope);

            FREE(LilyCheckedVirtualScope, unsafe_virtual_scope);

            break;
        }
        case LILY_CHECKED_STMT_KIND_VARIABLE:
            add_variable__LilyCheckedVirtualScope(
              virtual_scope,
              NEW(LilyCheckedVirtualVariable,
                  &stmt->variable,
                  reanalyze_expr__LilyAnalysis(self,
                                               signature,
                                               stmt->variable.expr,
                                               virtual_scope,
                                               return_data_type)));

            break;
        case LILY_CHECKED_STMT_KIND_WHILE: {
            LilyCheckedVirtualScope *while_virtual_scope =
              NEW(LilyCheckedVirtualScope, virtual_scope);

            REANALYZE_BODY(stmt->while_.body, while_virtual_scope);

            FREE(LilyCheckedVirtualScope, while_virtual_scope);

            break;
        }
        default:
            break;
    }
}

void
valid_cast__LilyAnalysis(LilyAnalysis *self,
                         LilyCheckedDataType *src,
                         LilyCheckedDataType *dest,
                         enum LilyCheckedSafetyMode safety_mode)
{
    if ((src->kind == LILY_CHECKED_DATA_TYPE_KIND_ANY ||
         dest->kind == LILY_CHECKED_DATA_TYPE_KIND_ANY) &&
        safety_mode == LILY_CHECKED_SAFETY_MODE_SAFE) {
        ANALYSIS_EMIT_DIAGNOSTIC(
          self,
          simple_lily_error,
          dest->location,
          NEW(LilyError, LILY_ERROR_KIND_CANNOT_CAST_TO_ANY_IN_SAFE_MODE),
          NULL,
          NULL,
          NULL);

        return;
    }

    switch (src->kind) {
        case LILY_CHECKED_DATA_TYPE_KIND_BOOL:
        case LILY_CHECKED_DATA_TYPE_KIND_BYTE:
        case LILY_CHECKED_DATA_TYPE_KIND_CHAR:
        case LILY_CHECKED_DATA_TYPE_KIND_FLOAT32:
        case LILY_CHECKED_DATA_TYPE_KIND_FLOAT64:
        case LILY_CHECKED_DATA_TYPE_KIND_INT16:
        case LILY_CHECKED_DATA_TYPE_KIND_INT32:
        case LILY_CHECKED_DATA_TYPE_KIND_INT64:
        case LILY_CHECKED_DATA_TYPE_KIND_INT8:
        case LILY_CHECKED_DATA_TYPE_KIND_ISIZE:
        case LILY_CHECKED_DATA_TYPE_KIND_UINT16:
        case LILY_CHECKED_DATA_TYPE_KIND_UINT32:
        case LILY_CHECKED_DATA_TYPE_KIND_UINT64:
        case LILY_CHECKED_DATA_TYPE_KIND_UINT8:
        case LILY_CHECKED_DATA_TYPE_KIND_USIZE:
            switch (dest->kind) {
                case LILY_CHECKED_DATA_TYPE_KIND_BYTE:
                case LILY_CHECKED_DATA_TYPE_KIND_CHAR:
                case LILY_CHECKED_DATA_TYPE_KIND_FLOAT32:
                case LILY_CHECKED_DATA_TYPE_KIND_FLOAT64:
                case LILY_CHECKED_DATA_TYPE_KIND_INT16:
                case LILY_CHECKED_DATA_TYPE_KIND_INT32:
                case LILY_CHECKED_DATA_TYPE_KIND_INT64:
                case LILY_CHECKED_DATA_TYPE_KIND_INT8:
                case LILY_CHECKED_DATA_TYPE_KIND_ISIZE:
                case LILY_CHECKED_DATA_TYPE_KIND_UINT16:
                case LILY_CHECKED_DATA_TYPE_KIND_UINT32:
                case LILY_CHECKED_DATA_TYPE_KIND_UINT64:
                case LILY_CHECKED_DATA_TYPE_KIND_UINT8:
                case LILY_CHECKED_DATA_TYPE_KIND_USIZE:
                    if (dest->kind == src->kind) {
                    cannot_cast_to_the_same_data_type : {
                        ANALYSIS_EMIT_DIAGNOSTIC(
                          self,
                          simple_lily_error,
                          dest->location,
                          NEW(LilyError, LILY_ERROR_KIND_BAD_LITERAL_CAST),
                          NULL,
                          NULL,
                          from__String(
                            "you cannot cast to the same data type"));

                        return;
                    }
                    }

                    return;
                default:
                    ANALYSIS_EMIT_DIAGNOSTIC(
                      self,
                      simple_lily_error,
                      dest->location,
                      NEW(LilyError, LILY_ERROR_KIND_BAD_LITERAL_CAST),
                      NULL,
                      NULL,
                      NULL);

                    return;
            }
        case LILY_CHECKED_DATA_TYPE_KIND_BYTES:
        case LILY_CHECKED_DATA_TYPE_KIND_STR:
            switch (dest->kind) {
                case LILY_CHECKED_DATA_TYPE_KIND_BYTES:
                case LILY_CHECKED_DATA_TYPE_KIND_STR:
                case LILY_CHECKED_DATA_TYPE_KIND_CSTR:
                    if (dest->kind == src->kind) {
                        goto cannot_cast_to_the_same_data_type;
                    }

                    return;
                default:
                    return;
            }
        case LILY_CHECKED_DATA_TYPE_KIND_CUSTOM:
            TODO("check dynamic cast");
        default:
            ANALYSIS_EMIT_DIAGNOSTIC(
              self,
              simple_lily_error,
              dest->location,
              NEW(LilyError, LILY_ERROR_KIND_UNKNOWN_CAST),
              NULL,
              NULL,
              NULL);

            return;
    }
}

bool
is_drop__LilyAnalysis(LilyAnalysis *self,
                      LilyCheckedScope *scope,
                      LilyCheckedExpr *expr)
{
    ASSERT(expr->kind == LILY_CHECKED_EXPR_KIND_CALL);

    switch (expr->call.kind) {
        case LILY_CHECKED_EXPR_CALL_KIND_VARIABLE: {
            LilyCheckedBodyFunItem *variable =
              get_variable_from_id__LilyCheckedScope(
                scope, expr->call.scope.id, expr->call.global_name);

            return variable->stmt.variable.is_dropped;
        }
        default:
            TODO("is_drop__LilyAnalysis");
    }
}

LilyCheckedScopeResponse
resolve_id__LilyAnalysis(LilyAnalysis *self,
                         LilyAstExpr *id,
                         LilyCheckedScope *scope,
                         enum LilyCheckedScopeResponseKind res_kind,
                         enum LilyCheckedSafetyMode safety_mode)
{
    switch (id->kind) {
        case LILY_AST_EXPR_KIND_IDENTIFIER:
            switch (res_kind) {
                case LILY_CHECKED_SCOPE_RESPONSE_KIND_FUN:
                    return search_fun__LilyCheckedScope(scope,
                                                        id->identifier.name);
                case LILY_CHECKED_SCOPE_RESPONSE_KIND_RECORD:
                    return search_record__LilyCheckedScope(scope,
                                                           id->identifier.name);
                case LILY_CHECKED_SCOPE_CONTAINER_KIND_ENUM:
                    return search_enum__LilyCheckedScope(scope,
                                                         id->identifier.name);
                case LILY_CHECKED_SCOPE_RESPONSE_KIND_ALIAS:
                    return search_alias__LilyCheckedScope(scope,
                                                          id->identifier.name);
                case LILY_CHECKED_SCOPE_RESPONSE_KIND_ERROR:
                    return search_error__LilyCheckedScope(scope,
                                                          id->identifier.name);
                case LILY_CHECKED_SCOPE_RESPONSE_KIND_CLASS:
                    return search_class__LilyCheckedScope(scope,
                                                          id->identifier.name);
                case LILY_CHECKED_SCOPE_RESPONSE_KIND_RECORD_OBJECT:
                    return search_record_object__LilyCheckedScope(
                      scope, id->identifier.name);
                case LILY_CHECKED_SCOPE_RESPONSE_KIND_ENUM_OBJECT:
                    return search_enum_object__LilyCheckedScope(
                      scope, id->identifier.name);
                case LILY_CHECKED_SCOPE_RESPONSE_KIND_TRAIT:
                    return search_trait__LilyCheckedScope(scope,
                                                          id->identifier.name);
                case LILY_CHECKED_SCOPE_RESPONSE_KIND_CONSTANT:
                    return search_constant__LilyCheckedScope(
                      scope, id->identifier.name);
                case LILY_CHECKED_SCOPE_RESPONSE_KIND_VARIABLE:
                    return search_variable__LilyCheckedScope(
                      scope, id->identifier.name);
                case LILY_CHECKED_SCOPE_RESPONSE_KIND_MODULE:
                    return search_module__LilyCheckedScope(scope,
                                                           id->identifier.name);
                default:
                    UNREACHABLE("this situation is impossible");
            }

            break;
        case LILY_AST_EXPR_KIND_ACCESS: {
            switch (id->access.kind) {
                case LILY_AST_EXPR_ACCESS_KIND_GLOBAL_PATH:
                case LILY_AST_EXPR_ACCESS_KIND_PATH:
                case LILY_AST_EXPR_ACCESS_KIND_SELF_PATH:
                case LILY_AST_EXPR_ACCESS_KIND_self_PATH: {
                    LilyCheckedScope *current_scope =
                      id->access.kind == LILY_AST_EXPR_ACCESS_KIND_GLOBAL_PATH
                        ? self->module.scope
                        : scope;

                    switch (id->access.kind) {
                        case LILY_AST_EXPR_ACCESS_KIND_self_PATH: {
                            LilyCheckedDecl *method =
                              get_current_method__LilyCheckedScope(scope);

                            if (!method) {
                                FAILED("self access is not expected outside of "
                                       "method declaration");
                            }

                            LilyCheckedDecl *object =
                              get_current_object__LilyCheckedScope(
                                method->method.scope);

                            if (!object) {
                                FAILED("the scope of the object is not found");
                            } else {
                                current_scope =
                                  get_scope__LilyCheckedDecl(object);
                            }

                            break;
                        }
                        case LILY_AST_EXPR_ACCESS_KIND_SELF_PATH: {
                            LilyCheckedDecl *object =
                              get_current_object__LilyCheckedScope(
                                current_scope);

                            if (!object) {
                                FAILED("the scope of the object is not found");
                            } else {
                                scope = get_scope__LilyCheckedDecl(object);
                            }

                            break;
                        }
                        default:
                            break;
                    }

                    Vec *path = get_path__LilyAstExprAccess(&id->access);
                    LilyCheckedScopeResponse current_response;

                    for (Usize i = 0; i < path->len; ++i) {
                        if (!current_scope) {
                            FAILED("at this point this call is unexpected");
                        }

                        LilyAstExpr *item = get__Vec(path, i);

                        switch (item->kind) {
                            case LILY_AST_EXPR_KIND_IDENTIFIER:
                                break;
                            default:
                                FAILED("expected identifier in path");
                        }

                        current_response = search_identifier__LilyCheckedScope(
                          current_scope, item->identifier.name);

                        if (i + 1 != path->len) {
                            switch (current_response.kind) {
                                case LILY_CHECKED_SCOPE_RESPONSE_KIND_NOT_FOUND:
                                    FAILED("this identifier is not found.");
                                case LILY_CHECKED_SCOPE_RESPONSE_KIND_ENUM:
                                    if (!current_response.enum_->is_checked) {
                                        check_enum__LilyAnalysis(
                                          self, current_response.decl);
                                    }

                                    current_scope =
                                      current_response.enum_->scope;
                                    break;
                                case LILY_CHECKED_SCOPE_RESPONSE_KIND_ENUM_OBJECT:
                                    if (!current_response.enum_object
                                           ->is_checked) {
                                        TODO("check enum object");
                                    }

                                    current_scope =
                                      current_response.enum_object->scope;
                                    break;
                                case LILY_CHECKED_SCOPE_RESPONSE_KIND_MODULE:
                                    if (!current_response.module->is_checked) {
                                        TODO("check module");
                                    }

                                    current_scope =
                                      current_response.module->scope;
                                    break;
                                case LILY_CHECKED_SCOPE_RESPONSE_KIND_CLASS:
                                    if (!current_response.class->is_checked) {
                                        TODO("check class");
                                    }

                                    current_scope =
                                      current_response.class->scope;
                                    break;
                                case LILY_CHECKED_SCOPE_RESPONSE_KIND_RECORD_OBJECT:
                                    if (!current_response.record_object
                                           ->is_checked) {
                                        TODO("check record object");
                                    }

                                    current_scope =
                                      current_response.record_object->scope;
                                    break;
                                default:
                                    FAILED(
                                      "call not expected in the path context");
                            }
                        }
                    }

                    if (current_response.kind != res_kind) {
                        FAILED("this repsonse is unexpected");
                    }

                    return current_response;
                }
                default:
                    TODO("...");
            }

            TODO("resolve access");
        }
        default:
            FAILED("this expression is not an id");
    }
}

OrderedHashMap *
build_called_generic_params_for_type__LilyAnalysis(
  LilyAnalysis *self,
  const LilyCheckedDecl *decl,
  LilyCheckedScope *scope,
  enum LilyCheckedSafetyMode safety_mode,
  Vec *ast_called_generic_params)
{
    const Vec *generic_params = get_generic_params__LilyCheckedDecl(decl);

    if (generic_params) {
        // TODO: maybe in the future we have to check default generic param.
        ASSERT(ast_called_generic_params->len == generic_params->len);

        OrderedHashMap *checked_called_generic_params = NEW(OrderedHashMap);

        for (Usize i = 0; i < generic_params->len; i++) {
            const LilyCheckedGenericParam *generic_param =
              get__Vec(generic_params, i);
            LilyAstDataType *ast_called_generic_param =
              get__Vec(ast_called_generic_params, i);

            LilyCheckedDataType *checked_data_type =
              check_data_type__LilyAnalysis(
                self, ast_called_generic_param, scope, NULL, safety_mode);

            insert__OrderedHashMap(
              checked_called_generic_params,
              get_name__LilyCheckedGenericParam(generic_param)->buffer,
              checked_data_type);
        }

        return checked_called_generic_params;
    }

    return NULL;
}

bool
valid_function_signature__LilyAnalysis(LilyAnalysis *self,
                                       const Location *location,
                                       Vec *params,
                                       Vec *params_call,
                                       HashMap *called_generic_params)
{
    if (!params_call) {
        bool is_ok = true;

        for (Usize i = 0; i < params->len; ++i) {
            if (CAST(LilyCheckedDeclFunParam *, get__Vec(params, i))->kind !=
                LILY_CHECKED_DECL_FUN_PARAM_KIND_DEFAULT) {
                is_ok = false;
                FAILED("miss some parametters");
            }
        }

        return is_ok;
    }

    if (params) {
        Vec *compiler_generic_values =
          NEW(Vec); // Vec<LilyCheckedCompilerGenericValue*>*

        for (Usize i = 0; i < params->len; ++i) {
            LilyCheckedDeclFunParam *param = get__Vec(params, i);

            if (i < params_call->len) {
                LilyCheckedExprCallFunParam *param_call =
                  get__Vec(params_call, i);

                switch (param->data_type->kind) {
                    case LILY_CHECKED_DATA_TYPE_KIND_COMPILER_GENERIC: {
                        LilyCheckedCompilerGenericValue *value =
                          NEW(LilyCheckedCompilerGenericValue,
                              param->data_type->compiler_generic.name,
                              param_call->value->data_type);

                        // Valid param with compiler generic data type.
                        if (add_value__LilyCheckedCompilerGenericValue(
                              value, compiler_generic_values)) {
                            FREE(LilyCheckedCompilerGenericValue, value);

                            LilyCheckedCompilerGenericValue *pushed_value =
                              get_value__LilyCheckedCompilerGenericValue(
                                compiler_generic_values,
                                param->data_type->compiler_generic.name);

                            if (pushed_value) {
                                if (!eq__LilyCheckedDataType(
                                      pushed_value->data_type,
                                      param_call->value->data_type)) {
                                    ANALYSIS_EMIT_DIAGNOSTIC(
                                      self,
                                      simple_lily_error,
                                      pushed_value->data_type->location,
                                      NEW(LilyError,
                                          LILY_ERROR_KIND_DATA_TYPE_DONT_MATCH),
                                      init__Vec(
                                        1,
                                        format__String(
                                          "the compiler generic is previously "
                                          "defined with an other data type")),
                                      NULL,
                                      NULL);

                                    return false;
                                }
                            } else {
                                UNREACHABLE("this situation is impossible");
                            }
                        }

                        break;
                    }
                    default:
                        switch (param->data_type->kind) {
                            case LILY_CHECKED_DATA_TYPE_KIND_CUSTOM:
                                switch (param->data_type->custom.kind) {
                                    case LILY_CHECKED_DATA_TYPE_CUSTOM_KIND_GENERIC:
                                        ASSERT(called_generic_params);

                                        if (!eq__LilyCheckedDataType(
                                              get__HashMap(
                                                called_generic_params,
                                                param->data_type->custom.name
                                                  ->buffer),
                                              param_call->value->data_type)) {
                                            ANALYSIS_EMIT_DIAGNOSTIC(
                                              self,
                                              simple_lily_error,
                                              param->data_type->location,
                                              NEW(
                                                LilyError,
                                                LILY_ERROR_KIND_DATA_TYPE_DONT_MATCH),
                                              NULL,
                                              NULL,
                                              NULL);

                                            return false;
                                        }

                                        break;
                                    default:
                                        goto valid_data_type_of_param;
                                }

                                break;
                            default:
                            valid_data_type_of_param : {
                                if (!eq__LilyCheckedDataType(
                                      param->data_type,
                                      param_call->value->data_type)) {
                                    ANALYSIS_EMIT_DIAGNOSTIC(
                                      self,
                                      simple_lily_error,
                                      param->data_type->location,
                                      NEW(LilyError,
                                          LILY_ERROR_KIND_DATA_TYPE_DONT_MATCH),
                                      NULL,
                                      NULL,
                                      NULL);

                                    return false;
                                }
                            }
                        }
                }
            } else {
                switch (param->kind) {
                    case LILY_CHECKED_DECL_FUN_PARAM_KIND_DEFAULT:
                        break;
                    default:
                        ANALYSIS_EMIT_DIAGNOSTIC(
                          self,
                          simple_lily_error,
                          param->data_type->location,
                          NEW(LilyError,
                              LILY_ERROR_KIND_NUMBER_OF_PARAMS_MISMATCHED),
                          NULL,
                          NULL,
                          NULL);

                        return false;
                }
            }
        }

        FREE_BUFFER_ITEMS(compiler_generic_values->buffer,
                          compiler_generic_values->len,
                          LilyCheckedCompilerGenericValue);
        FREE(Vec, compiler_generic_values);
    } else if (!params && params_call) {
        ANALYSIS_EMIT_DIAGNOSTIC(
          self,
          simple_lily_error,
          location,
          NEW(LilyError, LILY_ERROR_KIND_TOO_MANY_PARAMS),
          NULL,
          NULL,
          NULL);

        return false;
    }

    return true;
}

Vec *
check_fun_params_call__LilyAnalysis(LilyAnalysis *self,
                                    const Location *location,
                                    Vec *ast_params,
                                    HashMap *called_generic_params,
                                    LilyCheckedDeclFun *called_fun,
                                    LilyCheckedScope *scope,
                                    enum LilyCheckedSafetyMode safety_mode)
{
    if (!ast_params && called_fun->params) {
        ANALYSIS_EMIT_DIAGNOSTIC(
          self,
          simple_lily_error,
          location,
          NEW(LilyError, LILY_ERROR_KIND_TOO_MANY_PARAMS),
          NULL,
          NULL,
          NULL);

        return NULL;
    } else if (ast_params && called_fun->params) {
        if (ast_params->len > called_fun->params->len) {
            ANALYSIS_EMIT_DIAGNOSTIC(
              self,
              simple_lily_error,
              location,
              NEW(LilyError, LILY_ERROR_KIND_TOO_MANY_PARAMS),
              NULL,
              NULL,
              NULL);

            return NULL;
        }
    }

    Vec *checked_params = ast_params->len > 0 ? NEW(Vec) : NULL;

    for (Usize i = 0; i < ast_params->len; ++i) {
        // TODO: check if there are few params
        LilyCheckedDeclFunParam *fun_param = get__Vec(called_fun->params, i);
        LilyAstExprFunParamCall *call_param = get__Vec(ast_params, i);

        LilyCheckedExpr *value = NULL;
        LilyCheckedDataType *defined_data_type =
          resolve_generic_data_type_with_hash_map__LilyCheckedDataType(
            fun_param->data_type, called_generic_params);

        value = check_expr__LilyAnalysis(self,
                                         call_param->value,
                                         scope,
                                         safety_mode,
                                         false,
                                         defined_data_type);

        FREE(LilyCheckedDataType, defined_data_type);

        switch (call_param->kind) {
            case LILY_AST_EXPR_FUN_PARAM_CALL_KIND_DEFAULT:
                break;
            case LILY_AST_EXPR_FUN_PARAM_CALL_KIND_NORMAL:
                push__Vec(checked_params,
                          NEW_VARIANT(LilyCheckedExprCallFunParam,
                                      normal,
                                      value,
                                      &call_param->location));

                break;
            default:
                UNREACHABLE("unknown variant");
        }
    }

    return checked_params;
}

Vec *
check_builtin_fun_params_call__LilyAnalysis(
  LilyAnalysis *self,
  Vec *ast_params,
  LilyCheckedScope *scope,
  enum LilyCheckedSafetyMode safety_mode)
{
    Vec *checked_params = ast_params->len > 0 ? NEW(Vec) : NULL;

    for (Usize i = 0; i < ast_params->len; ++i) {
        LilyAstExprFunParamCall *call_param = get__Vec(ast_params, i);

        LilyCheckedExpr *value = check_expr__LilyAnalysis(
          self, call_param->value, scope, safety_mode, false, NULL);

        switch (call_param->kind) {
            case LILY_AST_EXPR_FUN_PARAM_CALL_KIND_DEFAULT:
                ANALYSIS_EMIT_DIAGNOSTIC(
                  self,
                  simple_lily_error,
                  (&call_param->location),
                  NEW(LilyError, LILY_ERROR_KIND_DEFAULT_PARAM_IS_NOT_EXPECTED),
                  NULL,
                  NULL,
                  from__String(
                    "default param is not expected in builtin function"));

                break;
            case LILY_AST_EXPR_FUN_PARAM_CALL_KIND_NORMAL:
                // TODO: pass &param->location
                push__Vec(checked_params,
                          NEW_VARIANT(LilyCheckedExprCallFunParam,
                                      normal,
                                      value,
                                      &call_param->location));

                break;
            default:
                break;
        }
    }

    return checked_params;
}

Vec *
check_sys_fun_params_call__LilyAnalysis(LilyAnalysis *self,
                                        Vec *ast_params,
                                        Vec *sys_params,
                                        LilyCheckedScope *scope,
                                        enum LilyCheckedSafetyMode safety_mode)
{
    Vec *checked_params = ast_params->len > 0 ? NEW(Vec) : NULL;

    for (Usize i = 0; i < ast_params->len; ++i) {
        LilyCheckedDataType *param_data_type = get__Vec(sys_params, i);
        LilyAstExprFunParamCall *call_param = get__Vec(ast_params, i);

        LilyCheckedExpr *value = check_expr__LilyAnalysis(
          self, call_param->value, scope, safety_mode, false, param_data_type);

        if (!eq__LilyCheckedDataType(param_data_type, value->data_type)) {
            ANALYSIS_EMIT_DIAGNOSTIC(
              self,
              simple_lily_error,
              param_data_type->location,
              NEW(LilyError, LILY_ERROR_KIND_DATA_TYPE_DONT_MATCH),
              NULL,
              NULL,
              NULL);
        }

        switch (call_param->kind) {
            case LILY_AST_EXPR_FUN_PARAM_CALL_KIND_DEFAULT:
                ANALYSIS_EMIT_DIAGNOSTIC(
                  self,
                  simple_lily_error,
                  (&call_param->location),
                  NEW(LilyError, LILY_ERROR_KIND_DEFAULT_PARAM_IS_NOT_EXPECTED),
                  NULL,
                  NULL,
                  from__String(
                    "default param is not expected in sys function"));

                break;
            case LILY_AST_EXPR_FUN_PARAM_CALL_KIND_NORMAL:
                // TODO: pass &param->location
                push__Vec(checked_params,
                          NEW_VARIANT(LilyCheckedExprCallFunParam,
                                      normal,
                                      value,
                                      &call_param->location));

                break;
            default:
                break;
        }
    }

    return checked_params;
}

LilyCheckedExpr *
get_call_from_expr__LilyAnalysis(LilyAnalysis *self,
                                 LilyAstExpr *expr,
                                 LilyCheckedScope *scope,
                                 LilyCheckedDataType *defined_data_type,
                                 enum LilyCheckedSafetyMode safety_mode,
                                 bool must_mut)
{
    switch (expr->kind) {
        case LILY_AST_EXPR_KIND_ACCESS:
            TODO("resolve access");
        case LILY_AST_EXPR_KIND_IDENTIFIER:
            return check_identifier_expr__LilyAnalysis(
              self, expr, scope, defined_data_type, safety_mode, must_mut);
        default:
            UNREACHABLE("this situation is impossible");
    }
}

LilyCheckedExpr *
check_field_access__LilyAnalysis(LilyAnalysis *self,
                                 LilyAstExpr *path,
                                 LilyCheckedExpr *first,
                                 LilyCheckedScope *scope,
                                 LilyCheckedScope *record_scope,
                                 enum LilyCheckedSafetyMode safety_mode,
                                 bool must_mut)
{
    LilyCheckedScope *current_scope = record_scope;
    LilyCheckedExpr *last = first;
    Vec *accesses = init__Vec(1, first); // Vec<LilyCheckedExpr*>*

    for (Usize i = 1; i < path->access.path->len; ++i) {
        LilyAstExpr *path_item = get__Vec(path->access.path, i);

        switch (path_item->kind) {
            case LILY_AST_EXPR_KIND_IDENTIFIER: {
                LilyCheckedScopeResponse field_response =
                  search_field__LilyCheckedScope(current_scope,
                                                 path_item->identifier.name);

                switch (field_response.kind) {
                    case LILY_CHECKED_SCOPE_RESPONSE_KIND_NOT_FOUND:
                        ANALYSIS_EMIT_DIAGNOSTIC(
                          self,
                          simple_lily_error,
                          (&path_item->location),
                          NEW(LilyError, LILY_ERROR_KIND_FIELD_IS_NOT_FOUND),
                          NULL,
                          NULL,
                          NULL);

                        return NEW_VARIANT(
                          LilyCheckedExpr,
                          call,
                          &path_item->location,
                          NEW(LilyCheckedDataType,
                              LILY_CHECKED_DATA_TYPE_KIND_UNKNOWN,
                              &path_item->location),
                          path_item,
                          NEW(LilyCheckedExprCall,
                              LILY_CHECKED_EXPR_CALL_KIND_UNKNOWN,
                              NULL,
                              (LilyCheckedAccessScope){ .id = 0 }));
                    case LILY_CHECKED_SCOPE_RESPONSE_KIND_RECORD_FIELD: {
                        LilyCheckedExpr *field = NEW_VARIANT(
                          LilyCheckedExpr,
                          call,
                          &path_item->location,
                          ref__LilyCheckedDataType(
                            field_response.record_field->data_type),
                          path_item,
                          NEW_VARIANT(
                            LilyCheckedExprCall,
                            record_field_single,
                            (LilyCheckedAccessScope){
                              .id = field_response.scope_container.scope_id },
                            field_response.record_field->name,
                            NEW(LilyCheckedExprCallRecordFieldSingle,
                                last->data_type->custom.scope,
                                last->data_type->custom.global_name,
                                field_response.scope_container.variable->id)));

                        LilyCheckedDataType *field_custom_dt =
                          get_direct_custom_data_type__LilyCheckedDataType(
                            field->data_type);

                        if (field_custom_dt) {
                            switch (field_custom_dt->custom.kind) {
                                case LILY_CHECKED_DATA_TYPE_CUSTOM_KIND_RECORD:
                                    current_scope =
                                      get_decl_from_id__LilyCheckedScope(
                                        current_scope,
                                        field_custom_dt->custom.scope_id,
                                        field_custom_dt->custom.scope.id)
                                        ->type.record.scope;

                                    break;
                                case LILY_CHECKED_DATA_TYPE_CUSTOM_KIND_RECORD_OBJECT:
                                    current_scope =
                                      get_decl_from_id__LilyCheckedScope(
                                        current_scope,
                                        field_custom_dt->custom.scope_id,
                                        field_custom_dt->custom.scope.id)
                                        ->object.record.scope;

                                    break;
                                case LILY_CHECKED_DATA_TYPE_CUSTOM_KIND_CLASS:
                                    current_scope =
                                      get_decl_from_id__LilyCheckedScope(
                                        current_scope,
                                        field_custom_dt->custom.scope_id,
                                        field_custom_dt->custom.scope.id)
                                        ->object.class.scope;

                                    break;
                                case LILY_CHECKED_DATA_TYPE_CUSTOM_KIND_ENUM:
                                    current_scope =
                                      get_decl_from_id__LilyCheckedScope(
                                        current_scope,
                                        field_custom_dt->custom.scope_id,
                                        field_custom_dt->custom.scope.id)
                                        ->type.enum_.scope;

                                    break;
                                case LILY_CHECKED_DATA_TYPE_CUSTOM_KIND_ENUM_OBJECT:
                                    current_scope =
                                      get_decl_from_id__LilyCheckedScope(
                                        current_scope,
                                        field_custom_dt->custom.scope_id,
                                        field_custom_dt->custom.scope.id)
                                        ->object.enum_.scope;

                                    break;
                                case LILY_CHECKED_DATA_TYPE_CUSTOM_KIND_TRAIT:
                                    ANALYSIS_EMIT_DIAGNOSTIC(
                                      self,
                                      simple_lily_error,
                                      field_custom_dt->location,
                                      NEW(
                                        LilyError,
                                        LILY_ERROR_KIND_THERE_IS_NO_FIELD_IN_TRAIT),
                                      NULL,
                                      NULL,
                                      NULL);

                                    return NEW_VARIANT(
                                      LilyCheckedExpr,
                                      call,
                                      &path_item->location,
                                      NEW(LilyCheckedDataType,
                                          LILY_CHECKED_DATA_TYPE_KIND_UNKNOWN,
                                          &path_item->location),
                                      path_item,
                                      NEW(LilyCheckedExprCall,
                                          LILY_CHECKED_EXPR_CALL_KIND_UNKNOWN,
                                          NULL,
                                          (LilyCheckedAccessScope){ .id = 0 }));
                                default:
                                    UNREACHABLE(
                                      "expected a custom data type. "
                                      "maybe get_direct_custom_data_type__"
                                      "LilyCheckedDataType have a bug!!");
                            }
                        } else {
                            if (i + 1 != path->access.path->len) {
                                ANALYSIS_EMIT_DIAGNOSTIC(
                                  self,
                                  simple_lily_error,
                                  field_custom_dt->location,
                                  NEW(
                                    LilyError,
                                    LILY_ERROR_KIND_EXPECTED_CUSTOM_DATA_TYPE),
                                  NULL,
                                  NULL,
                                  NULL);
                            }
                        }

                        last = field;
                        push__Vec(accesses, field);

                        break;
                    }
                    default:
                        TODO("not yet implemented");
                }

                break;
            }
            case LILY_AST_EXPR_KIND_CALL:
                TODO("call is not yet implemented in path");
            default:
                ANALYSIS_EMIT_DIAGNOSTIC(
                  self,
                  simple_lily_error,
                  (&path_item->location),
                  NEW(LilyError,
                      LILY_ERROR_KIND_CALL_NOT_EXPECTED_IN_THIS_CONTEXT),
                  NULL,
                  NULL,
                  NULL);

                return NEW_VARIANT(LilyCheckedExpr,
                                   call,
                                   &path_item->location,
                                   NEW(LilyCheckedDataType,
                                       LILY_CHECKED_DATA_TYPE_KIND_UNKNOWN,
                                       &path_item->location),
                                   path_item,
                                   NEW(LilyCheckedExprCall,
                                       LILY_CHECKED_EXPR_CALL_KIND_UNKNOWN,
                                       NULL,
                                       (LilyCheckedAccessScope){ .id = 0 }));
        }
    }

    switch (last->kind) {
        case LILY_CHECKED_EXPR_KIND_CALL:
            switch (last->call.kind) {
                case LILY_CHECKED_EXPR_CALL_KIND_RECORD_FIELD_SINGLE:
                    return NEW_VARIANT(
                      LilyCheckedExpr,
                      call,
                      &path->location,
                      ref__LilyCheckedDataType(last->data_type),
                      path,
                      NEW_VARIANT(
                        LilyCheckedExprCall,
                        record_field_access,
                        last->call.scope,
                        NULL,
                        NEW(LilyCheckedExprCallRecordFieldAccess, accesses)));
                default:
                    TODO("not yet implemented");
            }

            break;
        default:
            UNREACHABLE("this situation is impossible");
    }
}

Vec *
check_fields_call__LilyAnalysis(LilyAnalysis *self,
                                LilyCheckedScope *scope,
                                LilyCheckedScope *record_scope,
                                const Vec *params,
                                const Location *call_location,
                                OrderedHashMap *checked_generic_params_call,
                                enum LilyCheckedSafetyMode safety_mode)
{
    Vec *checked_record_params = NEW(Vec);

    for (Usize i = 0; i < params->len; ++i) {
        LilyAstExprRecordParamCall *param = get__Vec(params, i);

        LilyCheckedScopeResponse field_response =
          search_field__LilyCheckedScope(record_scope, param->name);

        Usize field_index = 0;

        switch (field_response.kind) {
            case LILY_CHECKED_SCOPE_RESPONSE_KIND_NOT_FOUND:
                ANALYSIS_EMIT_DIAGNOSTIC(
                  self,
                  simple_lily_error,
                  call_location,
                  NEW(LilyError, LILY_ERROR_KIND_FIELD_IS_NOT_FOUND),
                  NULL,
                  NULL,
                  from__String("unknown record in this scope"));

                break;
            default:
                field_index = field_response.scope_container.variable->id;

                break;
        }

        LilyCheckedDataType *field_data_type = NULL;

        {
            bool contains_generic_dt =
              contains_generic_data_type__LilyCheckedDataType(
                field_response.record_field->data_type);

            // 1. Resolved field data type
            // 1.1 Explicit generic params call
            // 1.2 No generic params call
            if (contains_generic_dt && checked_generic_params_call) {
                field_data_type =
                  resolve_generic_data_type_with_ordered_hash_map__LilyCheckedDataType(
                    field_response.record_field->data_type,
                    checked_generic_params_call);
            } else if (!contains_generic_dt) {
                field_data_type = ref__LilyCheckedDataType(
                  field_response.record_field->data_type);
            }
        }

        LilyCheckedExpr *checked_value = check_expr__LilyAnalysis(
          self, param->value, scope, safety_mode, false, field_data_type);

        if (field_data_type) {
            if (!eq__LilyCheckedDataType(checked_value->data_type,
                                         field_data_type)) {
                ANALYSIS_EMIT_DIAGNOSTIC(
                  self,
                  simple_lily_error,
                  checked_value->location,
                  NEW(LilyError, LILY_ERROR_KIND_DATA_TYPE_DONT_MATCH),
                  NULL,
                  NULL,
                  NULL);
            }

            FREE(LilyCheckedDataType, field_data_type);
        }

        push__Vec(checked_record_params,
                  NEW(LilyCheckedExprCallRecordParam,
                      param->name,
                      checked_value,
                      field_index));
    }

    return checked_record_params;
}

LilyCheckedExpr *
check_global_path_expr__LilyAnalysis(LilyAnalysis *self,
                                     LilyAstExpr *expr,
                                     enum LilyCheckedSafetyMode safety_mode,
                                     LilyCheckedDataType *defined_data_type)
{
    TODO("check global path");
}

LilyCheckedExpr *
check_Self_path_expr__LilyAnalysis(LilyAnalysis *self,
                                   LilyAstExpr *expr,
                                   LilyCheckedScope *scope,
                                   enum LilyCheckedSafetyMode safety_mode,
                                   bool must_mut,
                                   LilyCheckedDataType *defined_data_type)
{
    TODO("check Self path");
}

LilyCheckedExpr *
check_self_path_expr__LilyAnalysis(LilyAnalysis *self,
                                   LilyAstExpr *expr,
                                   LilyCheckedScope *scope,
                                   enum LilyCheckedSafetyMode safety_mode,
                                   bool must_mut,
                                   LilyCheckedDataType *defined_data_type)
{
    TODO("check self path");
}

LilyCheckedExpr *
check_hook_access_expr__LilyAnalysis(LilyAnalysis *self,
                                     LilyAstExpr *expr,
                                     LilyCheckedScope *scope,
                                     enum LilyCheckedSafetyMode safety_mode,
                                     bool must_mut,
                                     LilyCheckedDataType *defined_data_type)
{
    TODO("check hook access");
}

LilyCheckedScope *
check_object_access_expr__LilyAnalysis(LilyAnalysis *self,
                                       LilyAstExpr *expr,
                                       LilyCheckedScope *scope)
{
    LilyCheckedDecl *object = get_current_object__LilyCheckedScope(scope);

    if (!object) {
        ANALYSIS_EMIT_DIAGNOSTIC(
          self,
          simple_lily_error,
          (&expr->location),
          NEW(LilyError, LILY_ERROR_KIND_EXPECTED_OBJECT_DECL_AS_PARENT),
          NULL,
          NULL,
          NULL);

        return NULL;
    }

    return get_scope__LilyCheckedDecl(object);
}

LilyCheckedExpr *
check_property_init_expr__LilyAnalysis(LilyAnalysis *self,
                                       LilyAstExpr *expr,
                                       LilyCheckedScope *scope,
                                       enum LilyCheckedSafetyMode safety_mode,
                                       bool must_mut,
                                       LilyCheckedDataType *defined_data_type)
{
    LilyCheckedDecl *method = get_current_method__LilyCheckedScope(scope);

    if (!method) {
        ANALYSIS_EMIT_DIAGNOSTIC(
          self,
          simple_lily_error,
          (&expr->location),
          NEW(LilyError, LILY_ERROR_KIND_EXPECTED_METHOD_AS_PARENT),
          NULL,
          NULL,
          from__String("property init is not expected in function"));

        return NEW_VARIANT(LilyCheckedExpr, unknown, &expr->location, expr);
    }

    TODO("check property init");
}

LilyCheckedExpr *
check_path_access_expr__LilyAnalysis(LilyAnalysis *self,
                                     LilyAstExpr *expr,
                                     LilyCheckedScope *scope,
                                     enum LilyCheckedSafetyMode safety_mode,
                                     bool must_mut,
                                     LilyCheckedDataType *defined_data_type)
{
    LilyAstExpr *first_ast_expr = get__Vec(expr->access.path, 0);

    // Analysis the first expression of the path
    LilyCheckedExpr *first = get_call_from_expr__LilyAnalysis(
      self, first_ast_expr, scope, defined_data_type, safety_mode, false);

    switch (first->call.kind) {
        case LILY_CHECKED_EXPR_CALL_KIND_FUN_PARAM:
        case LILY_CHECKED_EXPR_CALL_KIND_VARIABLE: {
            LilyCheckedDataType *custom =
              get_direct_custom_data_type__LilyCheckedDataType(
                first->data_type);

            if (custom) {
                switch (custom->custom.kind) {
                    case LILY_CHECKED_DATA_TYPE_CUSTOM_KIND_RECORD: {
                        return check_field_access__LilyAnalysis(
                          self,
                          expr,
                          first,
                          scope,
                          get_decl_from_id__LilyCheckedScope(
                            scope,
                            custom->custom.scope_id,
                            custom->custom.scope.id)
                            ->type.record.scope,
                          safety_mode,
                          must_mut);
                    }
                    case LILY_CHECKED_DATA_TYPE_CUSTOM_KIND_RECORD_OBJECT:
                        TODO("field access record object!!");
                    case LILY_CHECKED_DATA_TYPE_CUSTOM_KIND_CLASS:
                        TODO("attribute access!!");
                    default:
                        ANALYSIS_EMIT_DIAGNOSTIC(
                          self,
                          simple_lily_error,
                          (&expr->location),
                          NEW(
                            LilyError,
                            LILY_ERROR_KIND_THIS_KIND_OF_DATA_TYPE_IS_NOT_EXPECTED),
                          NULL,
                          NULL,
                          NULL);

                        return NEW_VARIANT(
                          LilyCheckedExpr, unknown, &expr->location, expr);
                }
            } else {
                ANALYSIS_EMIT_DIAGNOSTIC(
                  self,
                  simple_lily_error,
                  custom->location,
                  NEW(LilyError, LILY_ERROR_KIND_EXPECTED_CUSTOM_DATA_TYPE),
                  NULL,
                  NULL,
                  NULL);

                return NEW_VARIANT(
                  LilyCheckedExpr, unknown, &expr->location, expr);
            }
        }
        case LILY_CHECKED_EXPR_CALL_KIND_FUN:
            TODO("fun call!! (path access)");
        case LILY_CHECKED_EXPR_CALL_KIND_MODULE:
            TODO("module call!! (path access)");
        case LILY_CHECKED_EXPR_CALL_KIND_ENUM: {
            // NOTE: it's an enum variant without value
            if (expr->access.path->len != 2) {
                FAILED("expected variant. NOTE: maybe next "
                       "calls are unexpected");
            }

            LilyCheckedExpr *variant = check_identifier_expr__LilyAnalysis(
              self,
              get__Vec(expr->access.path, 1),
              first->call.enum_->type.enum_.scope,
              first->data_type,
              safety_mode,
              must_mut);

            FREE(LilyCheckedExpr, first);

            return variant;
        }
        case LILY_CHECKED_EXPR_CALL_KIND_UNKNOWN:
            FAILED("unknown call");
        default:
            // TODO: emit a diagnostic
            FAILED("no expected in this context or not yet "
                   "implemented");
    }

    TODO("resolve path access");
}

LilyCheckedExpr *
check_array_expr__LilyAnalysis(LilyAnalysis *self,
                               LilyAstExpr *expr,
                               LilyCheckedScope *scope,
                               enum LilyCheckedSafetyMode safety_mode,
                               bool must_mut,
                               LilyCheckedDataType *defined_data_type)
{
    // FIXME: add support for compiler defined data type
    Vec *items = NEW(Vec); // Vec<LilyCheckedExpr*>*

    for (Usize i = 0; i < expr->array.items->len; ++i) {
        push__Vec(items,
                  check_expr__LilyAnalysis(self,
                                           get__Vec(expr->array.items, i),
                                           scope,
                                           safety_mode,
                                           false,
                                           NULL));
    }

    LilyCheckedDataType *data_type_item =
      CAST(LilyCheckedExpr *, get__Vec(items, 0))->data_type;

    for (Usize i = 1; i < items->len; ++i) {
        if (!eq__LilyCheckedDataType(
              data_type_item,
              CAST(LilyCheckedExpr *, get__Vec(items, i))->data_type)) {
            ANALYSIS_EMIT_DIAGNOSTIC(
              self,
              simple_lily_error,
              data_type_item->location,
              NEW(LilyError, LILY_ERROR_KIND_DATA_TYPE_DONT_MATCH),
              NULL,
              NULL,
              NULL);
        }
    }

    if (defined_data_type) {
        switch (defined_data_type->kind) {
            case LILY_CHECKED_DATA_TYPE_KIND_ARRAY:
                if (!eq__LilyCheckedDataType(
                      data_type_item, defined_data_type->array.data_type)) {
                    ANALYSIS_EMIT_DIAGNOSTIC(
                      self,
                      simple_lily_error,
                      data_type_item->location,
                      NEW(LilyError, LILY_ERROR_KIND_DATA_TYPE_DONT_MATCH),
                      NULL,
                      NULL,
                      NULL);
                }

                return NEW_VARIANT(
                  LilyCheckedExpr,
                  array,
                  &expr->location,
                  NEW_VARIANT(LilyCheckedDataType,
                              array,
                              &expr->location,
                              NEW(LilyCheckedDataTypeArray,
                                  defined_data_type->array.kind,
                                  clone__LilyCheckedDataType(
                                    defined_data_type->array.data_type))),
                  expr,
                  NEW(LilyCheckedExprArray, items));
            default:
                ANALYSIS_EMIT_DIAGNOSTIC(
                  self,
                  simple_lily_error,
                  data_type_item->location,
                  NEW(LilyError, LILY_ERROR_KIND_DATA_TYPE_DONT_MATCH),
                  NULL,
                  NULL,
                  from__String("expected array data type"));
        }
    }

    return NEW_VARIANT(
      LilyCheckedExpr,
      array,
      &expr->location,
      NEW_VARIANT(LilyCheckedDataType,
                  array,
                  &expr->location,
                  NEW(LilyCheckedDataTypeArray,
                      LILY_CHECKED_DATA_TYPE_ARRAY_KIND_SIZED,
                      clone__LilyCheckedDataType(data_type_item))),
      expr,
      NEW(LilyCheckedExprArray, items));
}

LilyCheckedExpr *
check_fun_call_expr__LilyAnalysis(LilyAnalysis *self,
                                  LilyAstExpr *expr,
                                  LilyCheckedScope *scope,
                                  enum LilyCheckedSafetyMode safety_mode,
                                  bool must_mut,
                                  LilyCheckedDataType *defined_data_type)
{
}

LilyCheckedExpr *
check_call_expr__LilyAnalysis(LilyAnalysis *self,
                              LilyAstExpr *expr,
                              LilyCheckedScope *scope,
                              enum LilyCheckedSafetyMode safety_mode,
                              bool must_mut,
                              LilyCheckedDataType *defined_data_type)
{
    switch (expr->call.kind) {
        case LILY_AST_EXPR_CALL_KIND_FUN: {
            LilyCheckedScopeResponse response =
              resolve_id__LilyAnalysis(self,
                                       expr->call.fun.id,
                                       scope,
                                       LILY_CHECKED_SCOPE_RESPONSE_KIND_FUN,
                                       safety_mode);

            switch (response.kind) {
                case LILY_CHECKED_SCOPE_RESPONSE_KIND_NOT_FOUND:
                    ANALYSIS_EMIT_DIAGNOSTIC(
                      self,
                      simple_lily_error,
                      (&expr->location),
                      NEW(LilyError, LILY_ERROR_KIND_FUNCTION_IS_NOT_FOUND),
                      NULL,
                      NULL,
                      from__String("unknown function in this scope"));

                    return NEW_VARIANT(
                      LilyCheckedExpr,
                      call,
                      &expr->location,
                      NEW(LilyCheckedDataType,
                          LILY_CHECKED_DATA_TYPE_KIND_UNKNOWN,
                          &expr->location),
                      expr,
                      NEW(LilyCheckedExprCall,
                          LILY_CHECKED_EXPR_CALL_KIND_UNKNOWN,
                          NULL,
                          (LilyCheckedAccessScope){ .id = 0 }));
                default: {
                    if (response.fun->len == 1) {
                        LilyCheckedDecl *fun = get__Vec(response.fun, 0);

                        if (!fun->fun.is_checked &&
                            !contains_for_fun__LilyCheckedHistory(
                              history,
                              get_original_signature__LilyCheckedDeclFun(
                                &fun->fun))) {
                            // Add fun dependency to the current
                            // function.
                            // TODO: method
                            LilyCheckedScopeDecls *current_fun =
                              get_current_fun__LilyCheckedScope(scope);

                            if (current_fun) {
                                add_fun_dep__LilyCheckedDeclFun(
                                  &current_fun->decl->fun, fun);
                            }

                            ASSERT(history);

                            add__LilyCheckedHistory(history, fun);
                            check_fun__LilyAnalysis(self, fun);

                            FREE(LilyCheckedHistory, &history);
                        }

                        if (fun->fun.is_main) {
                            ANALYSIS_EMIT_DIAGNOSTIC(
                              self,
                              simple_lily_error,
                              (&expr->location),
                              NEW(
                                LilyError,
                                LILY_ERROR_KIND_MAIN_FUNCTION_IS_NOT_CALLABLE),
                              NULL,
                              NULL,
                              NULL);

                            return NEW_VARIANT(
                              LilyCheckedExpr,
                              call,
                              &expr->location,
                              NEW(LilyCheckedDataType,
                                  LILY_CHECKED_DATA_TYPE_KIND_UNKNOWN,
                                  &expr->location),
                              expr,
                              NEW(LilyCheckedExprCall,
                                  LILY_CHECKED_EXPR_CALL_KIND_UNKNOWN,
                                  NULL,
                                  (LilyCheckedAccessScope){ .id = 0 }));
                        }

                        HashMap *generic_params = NULL;

                        if (fun->fun.generic_params) {
                            generic_params = NEW(HashMap);

                            Vec *ast_generic_params =
                              get_generic_params__LilyAstExpr(
                                expr->call.fun.id);

                            ASSERT(ast_generic_params);

                            if (verify_generic_params__LilyCheckedDecl(
                                  fun, ast_generic_params)) {
                                FAILED("the size of the generic params "
                                       "is not the same");
                            }

                            for (Usize i = 0; i < ast_generic_params->len;
                                 ++i) {
                                LilyCheckedGenericParam *generic_param =
                                  get__Vec(fun->fun.generic_params, i);
                                LilyAstDataType *generic_param_call =
                                  get__Vec(ast_generic_params, i);

                                insert__HashMap(
                                  generic_params,
                                  get_name__LilyCheckedGenericParam(
                                    generic_param)
                                    ->buffer,
                                  check_data_type__LilyAnalysis(
                                    self,
                                    generic_param_call,
                                    scope,
                                    NULL,
                                    safety_mode));
                            }
                        }

                        Vec *checked_params =
                          check_fun_params_call__LilyAnalysis(
                            self,
                            &expr->location,
                            expr->call.fun.params,
                            generic_params,
                            &fun->fun,
                            scope,
                            safety_mode);

                        if (!valid_function_signature__LilyAnalysis(
                              self,
                              fun->location,
                              fun->fun.params,
                              checked_params,
                              generic_params)) {
                            LilyCheckedExpr *unknown_call = NEW_VARIANT(
                              LilyCheckedExpr,
                              call,
                              &expr->location,
                              NEW(LilyCheckedDataType,
                                  LILY_CHECKED_DATA_TYPE_KIND_UNKNOWN,
                                  &expr->location),
                              expr,
                              NEW(LilyCheckedExprCall,
                                  LILY_CHECKED_EXPR_CALL_KIND_UNKNOWN,
                                  NULL,
                                  (LilyCheckedAccessScope){ .id = 0 }));

                            FREE(LilyCheckedScopeResponse, &response);

                            return unknown_call;
                        } else {
                            // Get the signature of the function without
                            // the return data type.
                            Vec *fun_types = NEW(Vec);

                            if (checked_params && fun->fun.params) {
                                if (checked_params->len ==
                                    fun->fun.params->len) {
                                    for (Usize i = 0; i < checked_params->len;
                                         ++i) {
                                        push__Vec(
                                          fun_types,
                                          CAST(LilyCheckedExprCallFunParam *,
                                               get__Vec(checked_params, i))
                                            ->value->data_type);
                                    }
                                } else {
                                params_signature : {
                                    for (Usize i = 0; i < fun->fun.params->len;
                                         ++i) {
                                        LilyCheckedDeclFunParam *param =
                                          get__Vec(fun->fun.params, i);

                                        switch (param->kind) {
                                            case LILY_CHECKED_DECL_FUN_PARAM_KIND_NORMAL:
                                                if (
                                                  is_compiler_defined_and_known_dt__LilyCheckedDataType(
                                                    param->data_type)) {
                                                    push__Vec(
                                                      fun_types,
                                                      CAST(
                                                        LilyCheckedExprCallFunParam
                                                          *,
                                                        get__Vec(checked_params,
                                                                 i))
                                                        ->value->data_type);
                                                } else {
                                                    push__Vec(fun_types,
                                                              param->data_type);
                                                }

                                                break;
                                            case LILY_CHECKED_DECL_FUN_PARAM_KIND_DEFAULT:
                                                push__Vec(fun_types,
                                                          param->data_type);

                                                break;
                                            default:
                                                UNREACHABLE("unknown variant");
                                        }
                                    }
                                }
                                }
                            } else if (fun->fun.params) {
                                goto params_signature;
                            }

                            // Get the return data type of the function
                            // call.
                            LilyCheckedDataType *return_data_type = NULL;

                            switch (fun->fun.return_data_type->kind) {
                                case LILY_CHECKED_DATA_TYPE_KIND_CONDITIONAL_COMPILER_CHOICE: {
                                    return_data_type = ref__LilyCheckedDataType(
                                      get_return_data_type_of_conditional_compiler_choice(
                                        fun->fun.return_data_type, fun_types));

                                    if (!return_data_type) {
                                        ANALYSIS_EMIT_DIAGNOSTIC(
                                          self,
                                          simple_lily_error,
                                          fun->location,
                                          NEW(
                                            LilyError,
                                            LILY_ERROR_KIND_IMPOSSIBLE_TO_GET_RETURN_DATA_TYPE),
                                          init__Vec(
                                            1,
                                            from__String("the signature do you "
                                                         "try to pass is "
                                                         "invalid")),
                                          NULL,
                                          NULL);
                                    }

                                    break;
                                }
                                case LILY_CHECKED_DATA_TYPE_KIND_COMPILER_GENERIC:
                                    return_data_type =
                                      ref__LilyCheckedDataType(get__Vec(
                                        fun_types,
                                        get_id_of_param_from_compiler_generic__LilyCheckedDeclFun(
                                          &fun->fun,
                                          fun->fun.return_data_type
                                            ->compiler_generic.name)));

                                    break;
                                default:
                                    return_data_type =
                                      resolve_generic_data_type_with_hash_map__LilyCheckedDataType(
                                        fun->fun.return_data_type,
                                        generic_params);
                            }

                            LilyCheckedExpr *fun_call = NULL;

                            // Push a new signature
                            if (contains_uncertain_dt__LilyCheckedDeclFun(
                                  &fun->fun)) {
                                push__Vec(
                                  fun_types,
                                  ref__LilyCheckedDataType(return_data_type));

                                if (add_signature__LilyCheckedDeclFun(
                                      &fun->fun, fun_types, generic_params)) {
                                    LilyCheckedSignatureFun *signature =
                                      get_signature__LilyCheckedDeclFun(
                                        &fun->fun,
                                        fun->fun.global_name,
                                        fun_types);

                                    fun_call = NEW_VARIANT(
                                      LilyCheckedExpr,
                                      call,
                                      &expr->location,
                                      return_data_type,
                                      expr,
                                      NEW_VARIANT(
                                        LilyCheckedExprCall,
                                        fun,
                                        (LilyCheckedAccessScope){
                                          .id =
                                            response.scope_container.scope_id },
                                        get_ser_global_name_of_signature__LilyCheckedDeclFun(
                                          &fun->fun, signature->types),
                                        NEW(LilyCheckedExprCallFun,
                                            fun,
                                            checked_params,
                                            signature->generic_params)));

                                    FREE_HASHMAP_VALUES(generic_params,
                                                        LilyCheckedDataType);
                                    FREE(HashMap, generic_params);
                                    FREE(LilyCheckedDataType,
                                         last__Vec(fun_types));
                                    FREE(Vec, fun_types);
                                } else {
                                    // Re-analyze called function if the
                                    // param(s) are/is a compiler
                                    // defined data type or the return
                                    // data type is a compiler defined
                                    // data type.
                                    LilyCheckedSignatureFun *signature =
                                      get_signature__LilyCheckedDeclFun(
                                        &fun->fun,
                                        fun->fun.global_name,
                                        fun_types);

                                    ASSERT(signature);

                                    reanalyze_fun_call_with_signature__LilyAnalysis(
                                      self, signature, fun);

                                    fun_call = NEW_VARIANT(
                                      LilyCheckedExpr,
                                      call,
                                      &expr->location,
                                      return_data_type,
                                      expr,
                                      NEW_VARIANT(
                                        LilyCheckedExprCall,
                                        fun,
                                        (LilyCheckedAccessScope){
                                          .id =
                                            response.scope_container.scope_id },
                                        signature->ser_global_name,
                                        NEW(LilyCheckedExprCallFun,
                                            fun,
                                            checked_params,
                                            generic_params)));
                                }
                            } else {
                                fun_call = NEW_VARIANT(
                                  LilyCheckedExpr,
                                  call,
                                  &expr->location,
                                  return_data_type,
                                  expr,
                                  NEW_VARIANT(
                                    LilyCheckedExprCall,
                                    fun,
                                    (LilyCheckedAccessScope){
                                      .id = response.scope_container.scope_id },
                                    CAST(LilyCheckedSignatureFun *,
                                         get__Vec(fun->fun.signatures, 0))
                                      ->ser_global_name,
                                    NEW(LilyCheckedExprCallFun,
                                        fun,
                                        checked_params,
                                        generic_params)));

                                FREE(Vec, fun_types);
                            }

                            {
                                // TODO: later replace
                                // `get_current_fun__LilyCheckedScope` by
                                // `get_parent__LilyCheckedScope`, when method
                                // or lambda are be available.
                                LilyCheckedScopeDecls *current_fun =
                                  get_current_fun__LilyCheckedScope(scope);

                                if (current_fun) {
                                    add_fun_dep__LilyCheckedDeclFun(
                                      &current_fun->decl->fun, fun);
                                    collect_raises__LilyCheckedDeclFun(
                                      &current_fun->decl->fun,
                                      scope,
                                      fun->fun.raises,
                                      in_try);
                                }
                            }

                            FREE(LilyCheckedScopeResponse, &response);

                            return fun_call;
                        }
                    } else {
                        // Vec *checked_params =
                        //   check_fun_params_call__LilyAnalysis(
                        //     self,
                        //     expr->call.fun.params,
                        //     scope,
                        //     safety_mode);

                        TODO("check fun call");

                        break;
                    }
                }
            }
        }
        case LILY_AST_EXPR_CALL_KIND_FUN_BUILTIN: {
            if (!self->package->builtin_is_loaded) {
                ANALYSIS_EMIT_DIAGNOSTIC(
                  self,
                  simple_lily_error,
                  (&expr->location),
                  NEW(LilyError, LILY_ERROR_KIND_IMPORT_BUILTIN_REQUIRED),
                  NULL,
                  NULL,
                  from__String("please import `@builtin`"));

                return NEW_VARIANT(LilyCheckedExpr,
                                   call,
                                   &expr->location,
                                   NEW(LilyCheckedDataType,
                                       LILY_CHECKED_DATA_TYPE_KIND_UNKNOWN,
                                       &expr->location),
                                   expr,
                                   NEW(LilyCheckedExprCall,
                                       LILY_CHECKED_EXPR_CALL_KIND_UNKNOWN,
                                       NULL,
                                       (LilyCheckedAccessScope){ .id = 0 }));
            }

            if (!is_builtin_function__LilyBuiltin(
                  expr->call.fun_builtin.name->buffer)) {
            bad_builtin_function : {
                ANALYSIS_EMIT_DIAGNOSTIC(
                  self,
                  simple_lily_error,
                  (&expr->location),
                  NEW(LilyError, LILY_ERROR_KIND_BAD_BUILTIN_FUNCTION),
                  NULL,
                  NULL,
                  from__String("unknown builtin function"));

                return NEW_VARIANT(LilyCheckedExpr,
                                   call,
                                   &expr->location,
                                   NEW(LilyCheckedDataType,
                                       LILY_CHECKED_DATA_TYPE_KIND_UNKNOWN,
                                       &expr->location),
                                   expr,
                                   NEW(LilyCheckedExprCall,
                                       LILY_CHECKED_EXPR_CALL_KIND_UNKNOWN,
                                       NULL,
                                       (LilyCheckedAccessScope){ .id = 0 }));
            }
            }

            Vec *check_params_call =
              check_builtin_fun_params_call__LilyAnalysis(
                self,
                expr->call.fun_builtin.params,
                scope,
                safety_mode); // Vec<LilyCheckedExprCallFunParam*>*?
            Vec *check_params_call_dt = NEW(Vec); // Vec<LilyCheckedDataType*>*

            if (check_params_call) {
                for (Usize i = 0; i < check_params_call->len; ++i) {
                    push__Vec(check_params_call_dt,
                              CAST(LilyCheckedExprCallFunParam *,
                                   get__Vec(check_params_call, i))
                                ->value->data_type);
                }
            }

            const LilyBuiltinFun *builtin_signature =
              get_builtin__LilyBuiltin(self->root_package->builtins,
                                       expr->call.fun_builtin.name->buffer,
                                       check_params_call_dt);

            if (!builtin_signature) {
                goto bad_builtin_function;
            }

            // Add builtin to builtin_signature to know which prototype
            // to define for IR.
            add_builtin_fun_to_builtin_usage__LilyPackage(
              self->package, (LilyBuiltinFun *)builtin_signature);

            FREE(Vec, check_params_call_dt);

            return NEW_VARIANT(
              LilyCheckedExpr,
              call,
              &expr->location,
              ref__LilyCheckedDataType(builtin_signature->return_data_type),
              expr,
              NEW_VARIANT(LilyCheckedExprCall,
                          fun_builtin,
                          NEW(LilyCheckedExprCallFunBuiltin,
                              check_params_call,
                              builtin_signature)));
        }
        case LILY_AST_EXPR_CALL_KIND_FUN_SYS: {
            if (!self->package->sys_is_loaded) {
                ANALYSIS_EMIT_DIAGNOSTIC(
                  self,
                  simple_lily_error,
                  (&expr->location),
                  NEW(LilyError, LILY_ERROR_KIND_IMPORT_SYS_REQUIRED),
                  NULL,
                  NULL,
                  from__String("please import `@sys`"));

                return NEW_VARIANT(LilyCheckedExpr,
                                   call,
                                   &expr->location,
                                   NEW(LilyCheckedDataType,
                                       LILY_CHECKED_DATA_TYPE_KIND_UNKNOWN,
                                       &expr->location),
                                   expr,
                                   NEW(LilyCheckedExprCall,
                                       LILY_CHECKED_EXPR_CALL_KIND_UNKNOWN,
                                       NULL,
                                       (LilyCheckedAccessScope){ .id = 0 }));
            }

            if (!is_sys_function__LilySys(expr->call.fun_sys.name->buffer)) {
                ANALYSIS_EMIT_DIAGNOSTIC(
                  self,
                  simple_lily_error,
                  (&expr->location),
                  NEW(LilyError, LILY_ERROR_KIND_BAD_SYS_FUNCTION),
                  NULL,
                  NULL,
                  from__String("unknown sys function"));

                return NEW_VARIANT(LilyCheckedExpr,
                                   call,
                                   &expr->location,
                                   NEW(LilyCheckedDataType,
                                       LILY_CHECKED_DATA_TYPE_KIND_UNKNOWN,
                                       &expr->location),
                                   expr,
                                   NEW(LilyCheckedExprCall,
                                       LILY_CHECKED_EXPR_CALL_KIND_UNKNOWN,
                                       NULL,
                                       (LilyCheckedAccessScope){ .id = 0 }));
            }

            const LilySysFun *sys_signature = get_sys__LilySys(
              self->root_package->syss, expr->call.fun_sys.name->buffer);

            Vec *check_params_call = check_sys_fun_params_call__LilyAnalysis(
              self,
              expr->call.fun_sys.params,
              sys_signature->params,
              scope,
              safety_mode); // Vec<LilyCheckedExprCallFunParam*>*?

            // Add sys to sys_signature to know which prototype to
            // define for IR.
            add_sys_fun_to_sys_usage__LilyPackage(self->package,
                                                  (LilySysFun *)sys_signature);

            return NEW_VARIANT(
              LilyCheckedExpr,
              call,
              &expr->location,
              ref__LilyCheckedDataType(sys_signature->return_data_type),
              expr,
              NEW_VARIANT(LilyCheckedExprCall,
                          fun_sys,
                          NEW(LilyCheckedExprCallFunSys,
                              check_params_call,
                              sys_signature)));
        }
        case LILY_AST_EXPR_CALL_KIND_LEN: {
            LilyCheckedExpr *len_expr = check_expr__LilyAnalysis(
              self, expr->call.len, scope, safety_mode, false, NULL);

            // Check if the length is computable at compile-time
            switch (len_expr->kind) {
                case LILY_CHECKED_EXPR_KIND_LITERAL:
                    switch (len_expr->literal.kind) {
                        case LILY_CHECKED_EXPR_LITERAL_KIND_STR: {
                            LilyCheckedExpr *res = NEW_VARIANT(
                              LilyCheckedExpr,
                              literal,
                              &expr->location,
                              NEW(LilyCheckedDataType,
                                  LILY_CHECKED_DATA_TYPE_KIND_USIZE,
                                  &expr->location),
                              expr,
                              NEW_VARIANT(LilyCheckedExprLiteral,
                                          int64,
                                          len_expr->literal.str->len));

                            FREE(LilyCheckedExpr, len_expr);

                            return res;
                        }
                        case LILY_CHECKED_EXPR_LITERAL_KIND_CSTR: {
                            LilyCheckedExpr *res = NEW_VARIANT(
                              LilyCheckedExpr,
                              literal,
                              &expr->location,
                              NEW(LilyCheckedDataType,
                                  LILY_CHECKED_DATA_TYPE_KIND_USIZE,
                                  &expr->location),
                              expr,
                              NEW_VARIANT(LilyCheckedExprLiteral,
                                          int64,
                                          strlen(len_expr->literal.cstr)));

                            FREE(LilyCheckedExpr, len_expr);

                            return res;
                        }
                        default:
                            TODO("do for the rest of compatible literal");
                    }
                default:
                    break;
            }

            switch (len_expr->data_type->kind) {
                case LILY_CHECKED_DATA_TYPE_KIND_STR:
                    return NEW_VARIANT(
                      LilyCheckedExpr,
                      call,
                      &expr->location,
                      NEW(LilyCheckedDataType,
                          LILY_CHECKED_DATA_TYPE_KIND_USIZE,
                          &expr->location),
                      expr,
                      NEW_VARIANT(LilyCheckedExprCall, str_len, len_expr));
                case LILY_CHECKED_DATA_TYPE_KIND_CSTR: {
                    LilyBuiltinFun *len_cstr_fun =
                      &self->root_package->builtins[__len__$CStr];

                    add_builtin_fun_to_builtin_usage__LilyPackage(self->package,
                                                                  len_cstr_fun);

                    return NEW_VARIANT(
                      LilyCheckedExpr,
                      call,
                      &expr->location,
                      NEW(LilyCheckedDataType,
                          LILY_CHECKED_DATA_TYPE_KIND_USIZE,
                          &expr->location),
                      expr,
                      NEW_VARIANT(LilyCheckedExprCall, cstr_len, len_expr));
                }
                default:
                    ANALYSIS_EMIT_DIAGNOSTIC(
                      self,
                      simple_lily_error,
                      len_expr->location,
                      NEW(LilyError, LILY_ERROR_KIND_EXPECTED_DATA_TYPE),
                      NULL,
                      NULL,
                      from__String("expected Str, CStr, Bytes, array "
                                   "or list data type"));
            }
        }
        case LILY_AST_EXPR_CALL_KIND_RECORD: {
            LilyCheckedScopeResponse response =
              resolve_id__LilyAnalysis(self,
                                       expr->call.record.id,
                                       scope,
                                       LILY_CHECKED_SCOPE_RESPONSE_KIND_RECORD,
                                       safety_mode);

            switch (response.kind) {
                case LILY_CHECKED_SCOPE_RESPONSE_KIND_NOT_FOUND:
                    ANALYSIS_EMIT_DIAGNOSTIC(
                      self,
                      simple_lily_error,
                      (&expr->location),
                      NEW(LilyError, LILY_ERROR_KIND_UNKNOWN_TYPE),
                      NULL,
                      NULL,
                      from__String("unknown record in this scope"));

                    return NEW_VARIANT(
                      LilyCheckedExpr,
                      call,
                      &expr->location,
                      NEW(LilyCheckedDataType,
                          LILY_CHECKED_DATA_TYPE_KIND_UNKNOWN,
                          &expr->location),
                      expr,
                      NEW(LilyCheckedExprCall,
                          LILY_CHECKED_EXPR_CALL_KIND_UNKNOWN,
                          NULL,
                          (LilyCheckedAccessScope){ .id = 0 }));
                default: {
                    // 1. Check record if not checked
                    if (!response.record->is_checked) {
                        check_record__LilyAnalysis(
                          self, response.record->scope->decls.decl);
                    }

                    // 2. Get the generic params call
                    Vec *ast_generic_params_call =
                      get_generic_params__LilyAstExpr(expr->call.record.id);

                    // 3. Check generic params
                    OrderedHashMap *checked_generic_params_call = NULL;

                    if (verify_generic_params__LilyCheckedDecl(
                          response.decl, ast_generic_params_call)) {
                        FAILED("the size of the generic params is not "
                               "the same");
                    }

                    Vec *checked_record_params =
                      NULL; // Vec<LilyCheckedExprCallRecordParam*>*

                    // 4. Determine if the record has a generic params
                    // 4.1 Explicit generic params call
                    // 4.2 Implicit generic params call
                    if (ast_generic_params_call &&
                        response.record->generic_params) {
                        checked_generic_params_call =
                          build_called_generic_params_for_type__LilyAnalysis(
                            self,
                            response.decl,
                            scope,
                            safety_mode,
                            ast_generic_params_call);
                    } else if (response.record->generic_params) {
                        checked_record_params = check_fields_call__LilyAnalysis(
                          self,
                          scope,
                          response.record->scope,
                          expr->call.record.params,
                          &expr->location,
                          checked_generic_params_call,
                          safety_mode);

                        checked_generic_params_call =
                          generate_generic_params_from_resolved_fields__LilyCheckedDataType(
                            checked_record_params,
                            response.record->generic_params,
                            response.record->fields);
                    }

                    if (!checked_record_params) {
                        checked_record_params = check_fields_call__LilyAnalysis(
                          self,
                          scope,
                          response.record->scope,
                          expr->call.record.params,
                          &expr->location,
                          checked_generic_params_call,
                          safety_mode);
                    }

                    Vec *generics =
                      checked_generic_params_call ? NEW(Vec) : NULL;
                    String *global_name = response.record->global_name;

                    if (add_signature__LilyCheckedSignatureType(
                          response.record->global_name,
                          checked_generic_params_call,
                          response.record->signatures)) {
                        if (checked_generic_params_call) {
                            OrderedHashMapIter iter = NEW(
                              OrderedHashMapIter, checked_generic_params_call);
                            LilyCheckedDataType *current = NULL;

                            while (
                              (current = next__OrderedHashMapIter(&iter))) {
                                push__Vec(generics, current);
                            }

                            LilyCheckedSignatureType *signature =
                              get_signature__LilyCheckedSignatureType(
                                response.record->signatures,
                                checked_generic_params_call);

                            ASSERT(signature);

                            global_name = signature->ser_global_name;

                            FREE(OrderedHashMap, checked_generic_params_call);
                        }
                    } else if (checked_generic_params_call) {
                        OrderedHashMapIter iter =
                          NEW(OrderedHashMapIter, checked_generic_params_call);
                        LilyCheckedDataType *current = NULL;

                        while ((current = next__OrderedHashMapIter(&iter))) {
                            push__Vec(generics,
                                      clone__LilyCheckedDataType(current));
                        }

                        // Get the last pushed signature
                        LilyCheckedSignatureType *signature =
                          last__Vec(response.record->signatures);

                        global_name = signature->ser_global_name;
                    }

                    LilyCheckedDataType *expr_data_type = NEW_VARIANT(
                      LilyCheckedDataType,
                      custom,
                      &expr->location,
                      NEW(LilyCheckedDataTypeCustom,
                          response.scope_container.scope_id,
                          (LilyCheckedAccessScope){
                            .id = response.scope_container.record->id },
                          response.record->name,
                          global_name,
                          generics,
                          LILY_CHECKED_DATA_TYPE_CUSTOM_KIND_RECORD,
                          response.record->is_recursive));

                    return NEW_VARIANT(
                      LilyCheckedExpr,
                      call,
                      &expr->location,
                      expr_data_type,
                      expr,
                      NEW_VARIANT(
                        LilyCheckedExprCall,
                        record,
                        (LilyCheckedAccessScope){
                          .id = response.scope_container.scope_id },
                        response.record->global_name,
                        NEW(LilyCheckedExprCallRecord, checked_record_params)));
                }
            }
        }
        case LILY_AST_EXPR_CALL_KIND_VARIANT: {
            LilyCheckedScopeResponse response = resolve_id__LilyAnalysis(
              self,
              expr->call.variant.id,
              scope,
              LILY_CHECKED_SCOPE_RESPONSE_KIND_ENUM_VARIANT,
              safety_mode);

            switch (response.kind) {
                case LILY_CHECKED_SCOPE_RESPONSE_KIND_NOT_FOUND:
                    ANALYSIS_EMIT_DIAGNOSTIC(
                      self,
                      simple_lily_error,
                      (&expr->location),
                      NEW(LilyError, LILY_ERROR_KIND_UNKNOWN_TYPE),
                      NULL,
                      NULL,
                      from__String("unknown enum in this scope"));

                    return NEW_VARIANT(
                      LilyCheckedExpr,
                      call,
                      &expr->location,
                      NEW(LilyCheckedDataType,
                          LILY_CHECKED_DATA_TYPE_KIND_UNKNOWN,
                          &expr->location),
                      expr,
                      NEW(LilyCheckedExprCall,
                          LILY_CHECKED_EXPR_CALL_KIND_UNKNOWN,
                          NULL,
                          (LilyCheckedAccessScope){ .id = 0 }));
                case LILY_CHECKED_SCOPE_RESPONSE_KIND_ENUM_VARIANT: {
                    ASSERT(response.enum_variant->enum_->type.enum_.is_checked);

                    // Verify if the enum variant has a data type (also
                    // a value).
                    if (!response.enum_variant->data_type) {
                        FAILED("expected data type");
                    }

                    // 1. Get the generic params call.
                    Vec *ast_generic_params_call =
                      get_generic_params_from_variant_call__LilyAstExpr(expr);

                    // 2. Check generic params.
                    OrderedHashMap *checked_generic_params_call = NULL;

                    if (verify_generic_params__LilyCheckedDecl(
                          response.enum_variant->enum_,
                          ast_generic_params_call)) {
                        FAILED("the size of the generic params is not "
                               "the same");
                    }

                    LilyCheckedDataType *checked_value_data_type = NULL;
                    LilyCheckedExpr *checked_value = NULL;

                    // 3. Determine if the enum has a generic params
                    // 3.1 Explicit generic params call
                    // 3.2 Implicit generic params call
                    // 3.3 No generic params call
                    if (ast_generic_params_call &&
                        response.enum_variant->enum_->type.enum_
                          .generic_params) {
                        checked_generic_params_call =
                          build_called_generic_params_for_type__LilyAnalysis(
                            self,
                            response.enum_variant->enum_,
                            scope,
                            safety_mode,
                            ast_generic_params_call);

                        checked_value_data_type =
                          resolve_generic_data_type_with_ordered_hash_map__LilyCheckedDataType(
                            response.enum_variant->data_type,
                            checked_generic_params_call);
                    } else if (response.enum_variant->enum_->type.enum_
                                 .generic_params &&
                               contains_generic_data_type__LilyCheckedDataType(
                                 response.enum_variant->data_type)) {
                        checked_value =
                          check_expr__LilyAnalysis(self,
                                                   expr->call.variant.value,
                                                   scope,
                                                   safety_mode,
                                                   false,
                                                   NULL);

                        checked_value_data_type =
                          ref__LilyCheckedDataType(checked_value->data_type);
                        checked_generic_params_call =
                          generate_generic_params_from_resolved_data_type__LilyCheckedDataType(
                            checked_value_data_type,
                            response.enum_variant->enum_->type.enum_
                              .generic_params,
                            response.enum_variant->data_type);
                    } else {
                        checked_value_data_type = ref__LilyCheckedDataType(
                          response.enum_variant->data_type);
                    }

                    if (!checked_value) {
                        checked_value =
                          check_expr__LilyAnalysis(self,
                                                   expr->call.variant.value,
                                                   scope,
                                                   safety_mode,
                                                   false,
                                                   checked_value_data_type);
                    }

                    // Check the data type of enum variant
                    if (checked_value) {
                        if (!eq__LilyCheckedDataType(
                              checked_value_data_type,
                              checked_value->data_type)) {
                            FAILED("data doesn't match");
                        }
                    }

                    LilyCheckedScopeResponse enum_response =
                      search_custom_type__LilyCheckedScope(
                        scope, response.enum_variant->enum_->type.enum_.name);

                    ASSERT(enum_response.kind !=
                           LILY_CHECKED_SCOPE_RESPONSE_KIND_NOT_FOUND);

                    Vec *generics = checked_generic_params_call
                                      ? NEW(Vec)
                                      : NULL; // Vec<LilyCheckedDataType*>*

                    if (add_signature__LilyCheckedSignatureType(
                          enum_response.enum_->global_name,
                          checked_generic_params_call,
                          enum_response.enum_->signatures)) {
                        if (checked_generic_params_call) {
                            OrderedHashMapIter iter = NEW(
                              OrderedHashMapIter, checked_generic_params_call);
                            LilyCheckedDataType *current = NULL;

                            while (
                              (current = next__OrderedHashMapIter(&iter))) {
                                push__Vec(generics, current);
                            }

                            FREE(OrderedHashMap, checked_generic_params_call);
                        }
                    } else if (checked_generic_params_call) {
                        OrderedHashMapIter iter =
                          NEW(OrderedHashMapIter, checked_generic_params_call);
                        LilyCheckedDataType *current = NULL;

                        while ((current = next__OrderedHashMapIter(&iter))) {
                            push__Vec(generics,
                                      clone__LilyCheckedDataType(current));
                        }
                    }

                    LilyCheckedDataType *expr_data_type = NEW_VARIANT(
                      LilyCheckedDataType,
                      custom,
                      &expr->location,
                      NEW(
                        LilyCheckedDataTypeCustom,
                        response.enum_variant->enum_->type.enum_.scope->id,
                        (LilyCheckedAccessScope){
                          .id = enum_response.scope_container.enum_->id },
                        response.enum_variant->enum_->type.enum_.name,
                        response.enum_variant->enum_->type.enum_.global_name,
                        generics,
                        LILY_CHECKED_DATA_TYPE_CUSTOM_KIND_ENUM,
                        response.enum_variant->enum_->type.enum_.is_recursive));

                    return NEW_VARIANT(
                      LilyCheckedExpr,
                      call,
                      &expr->location,
                      expr_data_type,
                      expr,
                      NEW_VARIANT(
                        LilyCheckedExprCall,
                        variant,
                        (LilyCheckedAccessScope){
                          .id = response.scope_container.scope_id },
                        response.enum_variant->global_name,
                        NEW(LilyCheckedExprCallVariant, checked_value)));
                }
                case LILY_CHECKED_SCOPE_RESPONSE_KIND_ENUM_VARIANT_OBJECT:
                    TODO("variant of enum object!!");
                default:
                    FAILED("expected variant");
            }
        }
        default:
            UNREACHABLE("unknown variant");
    }
}

LilyCheckedExpr *
check_assignable_expr__LilyAnalysis(LilyAnalysis *self,
                                    LilyAstExpr *expr,
                                    LilyCheckedScope *scope,
                                    enum LilyCheckedSafetyMode safety_mode,
                                    LilyCheckedDataType *defined_data_type)
{
    switch (expr->kind) {
        case LILY_AST_EXPR_KIND_IDENTIFIER:
        case LILY_AST_EXPR_KIND_ACCESS: {
            LilyCheckedExpr *checked_expr = check_expr__LilyAnalysis(
              self, expr, scope, safety_mode, true, defined_data_type);

            ASSERT(checked_expr->kind == LILY_CHECKED_EXPR_KIND_CALL);

            switch (checked_expr->call.kind) {
                case LILY_CHECKED_EXPR_CALL_KIND_FUN:
                case LILY_CHECKED_EXPR_CALL_KIND_METHOD:
                case LILY_CHECKED_EXPR_CALL_KIND_VARIABLE:
                case LILY_CHECKED_EXPR_CALL_KIND_FUN_PARAM:
                case LILY_CHECKED_EXPR_CALL_KIND_ATTRIBUTE:
                case LILY_CHECKED_EXPR_CALL_KIND_RECORD_FIELD_ACCESS:
                case LILY_CHECKED_EXPR_CALL_KIND_RECORD_FIELD_SINGLE:
                    return checked_expr;
                default:
                    FAILED("expected assignable expression");
            }
        }
        default:
            FAILED("expected assignable expression");
    }
}

LilyCheckedExpr *
check_cast_expr__LilyAnalysis(LilyAnalysis *self,
                              LilyAstExpr *expr,
                              LilyCheckedScope *scope,
                              enum LilyCheckedSafetyMode safety_mode,
                              LilyCheckedDataType *defined_data_type)
{
    LilyCheckedExpr *left = check_expr__LilyAnalysis(
      self, expr->cast.expr, scope, safety_mode, false, NULL);
    LilyCheckedDataType *dest = check_data_type__LilyAnalysis(
      self, expr->cast.dest_data_type, scope, NULL, safety_mode);
    enum LilyCheckedExprCastKind kind;

    valid_cast__LilyAnalysis(self, left->data_type, dest, safety_mode);

    if (is_string_data_type__LilyCheckedDataType(left->data_type)) {
        kind = LILY_CHECKED_EXPR_CAST_KIND_STRING;
    } else if (is_literal_data_type__LilyCheckedDataType(left->data_type)) {
        kind = LILY_CHECKED_EXPR_CAST_KIND_LITERAL;
    } else {
        kind = LILY_CHECKED_EXPR_CAST_KIND_DYNAMIC;
    }

    return NEW_VARIANT(LilyCheckedExpr,
                       cast,
                       &expr->location,
                       clone__LilyCheckedDataType(dest),
                       expr,
                       NEW(LilyCheckedExprCast, kind, left, dest));
}

LilyCheckedExpr *
check_list_expr__LilyAnalysis(LilyAnalysis *self,
                              LilyAstExpr *expr,
                              LilyCheckedScope *scope,
                              enum LilyCheckedSafetyMode safety_mode,
                              LilyCheckedDataType *defined_data_type)
{
    // FIXME: add support for compiler defined data type
    Vec *items = NEW(Vec); // Vec<LilyCheckedExpr*>*

    for (Usize i = 0; i < expr->list.items->len; ++i) {
        push__Vec(items,
                  check_expr__LilyAnalysis(self,
                                           get__Vec(expr->array.items, i),
                                           scope,
                                           safety_mode,
                                           false,
                                           NULL));
    }

    LilyCheckedDataType *data_type_item =
      CAST(LilyCheckedExpr *, get__Vec(items, 0))->data_type;

    for (Usize i = 1; i < items->len; ++i) {
        if (!eq__LilyCheckedDataType(
              data_type_item,
              CAST(LilyCheckedExpr *, get__Vec(items, i))->data_type)) {
            ANALYSIS_EMIT_DIAGNOSTIC(
              self,
              simple_lily_error,
              data_type_item->location,
              NEW(LilyError, LILY_ERROR_KIND_DATA_TYPE_DONT_MATCH),
              NULL,
              NULL,
              NULL);
        }
    }

    if (defined_data_type) {
        switch (defined_data_type->kind) {
            case LILY_CHECKED_DATA_TYPE_KIND_LIST:
                if (!eq__LilyCheckedDataType(data_type_item,
                                             defined_data_type->list)) {
                    ANALYSIS_EMIT_DIAGNOSTIC(
                      self,
                      simple_lily_error,
                      data_type_item->location,
                      NEW(LilyError, LILY_ERROR_KIND_DATA_TYPE_DONT_MATCH),
                      NULL,
                      NULL,
                      NULL);
                }

                return NEW_VARIANT(LilyCheckedExpr,
                                   list,
                                   &expr->location,
                                   NEW_VARIANT(LilyCheckedDataType,
                                               list,
                                               &expr->location,
                                               clone__LilyCheckedDataType(
                                                 defined_data_type->list)),
                                   expr,
                                   NEW(LilyCheckedExprList, items));
            default:
                ANALYSIS_EMIT_DIAGNOSTIC(
                  self,
                  simple_lily_error,
                  data_type_item->location,
                  NEW(LilyError, LILY_ERROR_KIND_DATA_TYPE_DONT_MATCH),
                  NULL,
                  NULL,
                  from__String("expected list data type"));
        }
    }

    return NEW_VARIANT(LilyCheckedExpr,
                       list,
                       &expr->location,
                       NEW_VARIANT(LilyCheckedDataType,
                                   list,
                                   &expr->location,
                                   clone__LilyCheckedDataType(data_type_item)),
                       expr,
                       NEW(LilyCheckedExprList, items));
}

LilyCheckedExpr *
check_literal_expr__LilyAnalysis(LilyAnalysis *self,
                                 LilyAstExpr *expr,
                                 LilyCheckedScope *scope,
                                 enum LilyCheckedSafetyMode safety_mode,
                                 LilyCheckedDataType *defined_data_type)
{
    switch (expr->literal.kind) {
        case LILY_AST_EXPR_LITERAL_KIND_BOOL:
            return NEW_VARIANT(
              LilyCheckedExpr,
              literal,
              &expr->location,
              NEW(LilyCheckedDataType,
                  LILY_CHECKED_DATA_TYPE_KIND_BOOL,
                  &expr->location),
              expr,
              NEW_VARIANT(LilyCheckedExprLiteral, bool_, expr->literal.bool_));
        case LILY_AST_EXPR_LITERAL_KIND_BYTE:
            return NEW_VARIANT(
              LilyCheckedExpr,
              literal,
              &expr->location,
              NEW(LilyCheckedDataType,
                  LILY_CHECKED_DATA_TYPE_KIND_BYTE,
                  &expr->location),
              expr,
              NEW_VARIANT(LilyCheckedExprLiteral, byte, expr->literal.byte));
        case LILY_AST_EXPR_LITERAL_KIND_BYTES:
            return NEW_VARIANT(
              LilyCheckedExpr,
              literal,
              &expr->location,
              NEW_VARIANT(LilyCheckedDataType,
                          bytes,
                          &expr->location,
                          strlen((char *)expr->literal.bytes)),
              expr,
              NEW_VARIANT(LilyCheckedExprLiteral, bytes, expr->literal.bytes));
        case LILY_AST_EXPR_LITERAL_KIND_CHAR:
            return NEW_VARIANT(
              LilyCheckedExpr,
              literal,
              &expr->location,
              NEW(LilyCheckedDataType,
                  LILY_CHECKED_DATA_TYPE_KIND_CHAR,
                  &expr->location),
              expr,
              NEW_VARIANT(LilyCheckedExprLiteral, char, expr->literal.char_));
        case LILY_AST_EXPR_LITERAL_KIND_CSTR:
            return NEW_VARIANT(
              LilyCheckedExpr,
              literal,
              &expr->location,
              NEW(LilyCheckedDataType,
                  LILY_CHECKED_DATA_TYPE_KIND_CSTR,
                  &expr->location),
              expr,
              NEW_VARIANT(LilyCheckedExprLiteral, cstr, expr->literal.cstr));
        case LILY_AST_EXPR_LITERAL_KIND_FLOAT32:
            return NEW_VARIANT(LilyCheckedExpr,
                               literal,
                               &expr->location,
                               NEW(LilyCheckedDataType,
                                   LILY_CHECKED_DATA_TYPE_KIND_FLOAT32,
                                   &expr->location),
                               expr,
                               NEW_VARIANT(LilyCheckedExprLiteral,
                                           float32,
                                           expr->literal.float32));
        case LILY_AST_EXPR_LITERAL_KIND_FLOAT64: {
            LilyCheckedDataType *literal_data_type = NULL;

            if (defined_data_type) {
                switch (defined_data_type->kind) {
                    case LILY_CHECKED_DATA_TYPE_KIND_FLOAT32:
                    case LILY_CHECKED_DATA_TYPE_KIND_CFLOAT:
                        // TODO: Check if the Float64 is in range of
                        // Float32.

                        literal_data_type =
                          clone__LilyCheckedDataType(defined_data_type);

                        break;
                    case LILY_CHECKED_DATA_TYPE_KIND_CDOUBLE:
                        literal_data_type =
                          clone__LilyCheckedDataType(defined_data_type);

                        break;
                    default:
                        literal_data_type =
                          NEW(LilyCheckedDataType,
                              LILY_CHECKED_DATA_TYPE_KIND_FLOAT64,
                              NULL);

                        break;
                }

                literal_data_type->location = &expr->location;
            } else {
                literal_data_type = NEW(LilyCheckedDataType,
                                        LILY_CHECKED_DATA_TYPE_KIND_FLOAT64,
                                        &expr->location);
            }

            return NEW_VARIANT(LilyCheckedExpr,
                               literal,
                               &expr->location,
                               literal_data_type,
                               expr,
                               NEW_VARIANT(LilyCheckedExprLiteral,
                                           float64,
                                           expr->literal.float64));
        }
        case LILY_AST_EXPR_LITERAL_KIND_INT32: {
            LilyCheckedDataType *literal_data_type = NULL;

            if (defined_data_type) {
                switch (defined_data_type->kind) {
                    case LILY_CHECKED_DATA_TYPE_KIND_INT8:
                        if (expr->literal.int32 > INT8_MAX ||
                            expr->literal.int32 < INT8_MIN) {
                            ANALYSIS_EMIT_DIAGNOSTIC(
                              self,
                              simple_lily_error,
                              (&expr->location),
                              NEW(LilyError,
                                  LILY_ERROR_KIND_COMPTIME_CAST_OVERFLOW),
                              NULL,
                              NULL,
                              from__String(
                                "Int32 is out of range to cast Int8"));

                            return NEW_VARIANT(
                              LilyCheckedExpr, unknown, &expr->location, expr);
                        } else {
                            literal_data_type =
                              clone__LilyCheckedDataType(defined_data_type);
                        }

                        break;
                    case LILY_CHECKED_DATA_TYPE_KIND_INT16:
                    case LILY_CHECKED_DATA_TYPE_KIND_CSHORT:
                        if (expr->literal.int32 > INT16_MAX ||
                            expr->literal.int32 < INT16_MIN) {
                            ANALYSIS_EMIT_DIAGNOSTIC(
                              self,
                              simple_lily_error,
                              (&expr->location),
                              NEW(LilyError,
                                  LILY_ERROR_KIND_COMPTIME_CAST_OVERFLOW),
                              NULL,
                              NULL,
                              from__String("Int32 is out of range to "
                                           "cast Int16 or CShort"));

                            return NEW_VARIANT(
                              LilyCheckedExpr, unknown, &expr->location, expr);
                        } else {
                            literal_data_type =
                              clone__LilyCheckedDataType(defined_data_type);
                        }

                        break;
                    case LILY_CHECKED_DATA_TYPE_KIND_CINT:
                    case LILY_CHECKED_DATA_TYPE_KIND_CLONG:
                        literal_data_type =
                          clone__LilyCheckedDataType(defined_data_type);

                        break;
                    case LILY_CHECKED_DATA_TYPE_KIND_INT64:
                    case LILY_CHECKED_DATA_TYPE_KIND_ISIZE:
                    case LILY_CHECKED_DATA_TYPE_KIND_CLONGLONG:
                        literal_data_type =
                          clone__LilyCheckedDataType(defined_data_type);

                        break;
                    case LILY_CHECKED_DATA_TYPE_KIND_UINT8:
                        if (expr->literal.int32 > UINT8_MAX ||
                            expr->literal.int32 < 0) {
                            ANALYSIS_EMIT_DIAGNOSTIC(
                              self,
                              simple_lily_error,
                              (&expr->location),
                              NEW(LilyError,
                                  LILY_ERROR_KIND_COMPTIME_CAST_OVERFLOW),
                              NULL,
                              NULL,
                              from__String(
                                "Int32 is out of range to cast Uint8"));

                            return NEW_VARIANT(
                              LilyCheckedExpr, unknown, &expr->location, expr);
                        } else {
                            literal_data_type =
                              clone__LilyCheckedDataType(defined_data_type);
                        }

                        break;
                    case LILY_CHECKED_DATA_TYPE_KIND_UINT16:
                    case LILY_CHECKED_DATA_TYPE_KIND_CUSHORT:
                        if (expr->literal.int32 > UINT16_MAX ||
                            expr->literal.int32 < 0) {
                            ANALYSIS_EMIT_DIAGNOSTIC(
                              self,
                              simple_lily_error,
                              (&expr->location),
                              NEW(LilyError,
                                  LILY_ERROR_KIND_COMPTIME_CAST_OVERFLOW),
                              NULL,
                              NULL,
                              from__String("Int32 is out of range to "
                                           "cast Uint16 or CUshort"));

                            return NEW_VARIANT(
                              LilyCheckedExpr, unknown, &expr->location, expr);
                        } else {
                            literal_data_type =
                              clone__LilyCheckedDataType(defined_data_type);
                        }

                        break;
                    case LILY_CHECKED_DATA_TYPE_KIND_UINT32:
                    case LILY_CHECKED_DATA_TYPE_KIND_CUINT:
                    case LILY_CHECKED_DATA_TYPE_KIND_CULONG:
                        if (expr->literal.int32 < 0) {
                            ANALYSIS_EMIT_DIAGNOSTIC(
                              self,
                              simple_lily_error,
                              (&expr->location),
                              NEW(LilyError,
                                  LILY_ERROR_KIND_COMPTIME_CAST_OVERFLOW),
                              NULL,
                              NULL,
                              from__String(
                                "Int32 is out of range to cast Uint32 "
                                "or CUint or CUlong"));

                            return NEW_VARIANT(
                              LilyCheckedExpr, unknown, &expr->location, expr);
                        } else {
                            literal_data_type =
                              clone__LilyCheckedDataType(defined_data_type);
                        }

                        break;
                    case LILY_CHECKED_DATA_TYPE_KIND_UINT64:
                    case LILY_CHECKED_DATA_TYPE_KIND_USIZE:
                    case LILY_CHECKED_DATA_TYPE_KIND_CULONGLONG:
                        if (expr->literal.int32 < 0) {
                            ANALYSIS_EMIT_DIAGNOSTIC(
                              self,
                              simple_lily_error,
                              (&expr->location),
                              NEW(LilyError,
                                  LILY_ERROR_KIND_COMPTIME_CAST_OVERFLOW),
                              NULL,
                              NULL,
                              from__String(
                                "Int32 is out of range to cast Uint64 "
                                "or Usize or CUlonglong"));

                            return NEW_VARIANT(
                              LilyCheckedExpr, unknown, &expr->location, expr);
                        } else {
                            literal_data_type =
                              clone__LilyCheckedDataType(defined_data_type);
                        }

                        break;
                    default:
                        literal_data_type =
                          NEW(LilyCheckedDataType,
                              LILY_CHECKED_DATA_TYPE_KIND_INT32,
                              NULL);
                }

                literal_data_type->location = &expr->location;
            } else {
                literal_data_type = NEW(LilyCheckedDataType,
                                        LILY_CHECKED_DATA_TYPE_KIND_INT32,
                                        &expr->location);
            }

            return NEW_VARIANT(
              LilyCheckedExpr,
              literal,
              &expr->location,
              literal_data_type,
              expr,
              NEW_VARIANT(LilyCheckedExprLiteral, int32, expr->literal.int32));
        }
        case LILY_AST_EXPR_LITERAL_KIND_INT64: {
            LilyCheckedDataType *literal_data_type = NULL;

            if (defined_data_type) {
                switch (defined_data_type->kind) {
                    case LILY_CHECKED_DATA_TYPE_KIND_INT8:
                        if (expr->literal.int64 > INT8_MAX ||
                            expr->literal.int64 < INT8_MIN) {
                            ANALYSIS_EMIT_DIAGNOSTIC(
                              self,
                              simple_lily_error,
                              (&expr->location),
                              NEW(LilyError,
                                  LILY_ERROR_KIND_COMPTIME_CAST_OVERFLOW),
                              NULL,
                              NULL,
                              from__String(
                                "Int64 is out of range to cast Int8"));

                            return NEW_VARIANT(
                              LilyCheckedExpr, unknown, &expr->location, expr);
                        } else {
                            literal_data_type =
                              clone__LilyCheckedDataType(defined_data_type);
                        }

                        break;
                    case LILY_CHECKED_DATA_TYPE_KIND_INT16:
                    case LILY_CHECKED_DATA_TYPE_KIND_CSHORT:
                        if (expr->literal.int64 > INT16_MAX ||
                            expr->literal.int64 < INT16_MIN) {
                            ANALYSIS_EMIT_DIAGNOSTIC(
                              self,
                              simple_lily_error,
                              (&expr->location),
                              NEW(LilyError,
                                  LILY_ERROR_KIND_COMPTIME_CAST_OVERFLOW),
                              NULL,
                              NULL,
                              from__String("Int64 is out of range to "
                                           "cast Int16 or CShort"));

                            return NEW_VARIANT(
                              LilyCheckedExpr, unknown, &expr->location, expr);
                        } else {
                            literal_data_type =
                              clone__LilyCheckedDataType(defined_data_type);
                        }

                        break;
                    case LILY_CHECKED_DATA_TYPE_KIND_INT32:
#ifdef PLATFORM_32
                    case LILY_CHECKED_DATA_TYPE_KIND_ISIZE:
#endif
                    case LILY_CHECKED_DATA_TYPE_KIND_CINT:
                    case LILY_CHECKED_DATA_TYPE_KIND_CLONG:
                        if (expr->literal.int64 > INT32_MAX ||
                            expr->literal.int64 < INT32_MIN) {
                            ANALYSIS_EMIT_DIAGNOSTIC(
                              self,
                              simple_lily_error,
                              (&expr->location),
                              NEW(LilyError,
                                  LILY_ERROR_KIND_COMPTIME_CAST_OVERFLOW),
                              NULL,
                              NULL,
                              from__String("Int64 is out of range to "
                                           "cast Int32 or Isize (on "
                                           "32-bit) or CInt or CLong"));

                            return NEW_VARIANT(
                              LilyCheckedExpr, unknown, &expr->location, expr);
                        } else {
                            literal_data_type =
                              clone__LilyCheckedDataType(defined_data_type);
                        }

                        break;
#ifdef PLATFORM_64
                    case LILY_CHECKED_DATA_TYPE_KIND_ISIZE:
                    case LILY_CHECKED_DATA_TYPE_KIND_CLONGLONG:
                        literal_data_type =
                          clone__LilyCheckedDataType(defined_data_type);

                        break;
#endif
                    case LILY_CHECKED_DATA_TYPE_KIND_UINT8:
                        if (expr->literal.int64 > UINT8_MAX ||
                            expr->literal.int64 < 0) {
                            ANALYSIS_EMIT_DIAGNOSTIC(
                              self,
                              simple_lily_error,
                              (&expr->location),
                              NEW(LilyError,
                                  LILY_ERROR_KIND_COMPTIME_CAST_OVERFLOW),
                              NULL,
                              NULL,
                              from__String(
                                "Int64 is out of range to cast Uint8"));

                            return NEW_VARIANT(
                              LilyCheckedExpr, unknown, &expr->location, expr);
                        } else {
                            literal_data_type =
                              clone__LilyCheckedDataType(defined_data_type);
                        }

                        break;
                    case LILY_CHECKED_DATA_TYPE_KIND_UINT16:
                    case LILY_CHECKED_DATA_TYPE_KIND_CUSHORT:
                        if (expr->literal.int64 > UINT16_MAX ||
                            expr->literal.int64 < 0) {
                            ANALYSIS_EMIT_DIAGNOSTIC(
                              self,
                              simple_lily_error,
                              (&expr->location),
                              NEW(LilyError,
                                  LILY_ERROR_KIND_COMPTIME_CAST_OVERFLOW),
                              NULL,
                              NULL,
                              from__String("Int64 is out of range to "
                                           "cast Uint16 or CUshort"));

                            return NEW_VARIANT(
                              LilyCheckedExpr, unknown, &expr->location, expr);
                        } else {
                            literal_data_type =
                              clone__LilyCheckedDataType(defined_data_type);
                        }

                        break;
                    case LILY_CHECKED_DATA_TYPE_KIND_UINT32:
                    case LILY_CHECKED_DATA_TYPE_KIND_CUINT:
                    case LILY_CHECKED_DATA_TYPE_KIND_CULONG:
                        if (expr->literal.int64 < 0) {
                            ANALYSIS_EMIT_DIAGNOSTIC(
                              self,
                              simple_lily_error,
                              (&expr->location),
                              NEW(LilyError,
                                  LILY_ERROR_KIND_COMPTIME_CAST_OVERFLOW),
                              NULL,
                              NULL,
                              from__String("Int64 is out of range to "
                                           "cast Uint32 or CUint or CUlong"));

                            return NEW_VARIANT(
                              LilyCheckedExpr, unknown, &expr->location, expr);
                        } else {
                            literal_data_type =
                              clone__LilyCheckedDataType(defined_data_type);
                        }

                        break;
                    case LILY_CHECKED_DATA_TYPE_KIND_UINT64:
                    case LILY_CHECKED_DATA_TYPE_KIND_USIZE:
                    case LILY_CHECKED_DATA_TYPE_KIND_CULONGLONG:
                        if (expr->literal.int64 < 0) {
                            ANALYSIS_EMIT_DIAGNOSTIC(
                              self,
                              simple_lily_error,
                              (&expr->location),
                              NEW(LilyError,
                                  LILY_ERROR_KIND_COMPTIME_CAST_OVERFLOW),
                              NULL,
                              NULL,
                              from__String(
                                "Int64 is out of range to "
                                "cast Uint64 or Usize or CUlonglong"));

                            return NEW_VARIANT(
                              LilyCheckedExpr, unknown, &expr->location, expr);
                        } else {
                            literal_data_type =
                              clone__LilyCheckedDataType(defined_data_type);
                        }

                        break;
                    default:
                        literal_data_type =
                          NEW(LilyCheckedDataType,
                              LILY_CHECKED_DATA_TYPE_KIND_INT64,
                              NULL);
                }

                literal_data_type->location = &expr->location;
            } else {
                literal_data_type = NEW(LilyCheckedDataType,
                                        LILY_CHECKED_DATA_TYPE_KIND_INT64,
                                        &expr->location);
            }

            return NEW_VARIANT(
              LilyCheckedExpr,
              literal,
              &expr->location,
              literal_data_type,
              expr,
              NEW_VARIANT(LilyCheckedExprLiteral, int64, expr->literal.int64));
        }
        case LILY_AST_EXPR_LITERAL_KIND_NIL:
            if (defined_data_type) {
                switch (defined_data_type->kind) {
                    case LILY_CHECKED_DATA_TYPE_KIND_PTR:
                        return NEW_VARIANT(
                          LilyCheckedExpr,
                          literal,
                          &expr->location,
                          NEW_VARIANT(
                            LilyCheckedDataType,
                            ptr,
                            &expr->location,
                            clone__LilyCheckedDataType(defined_data_type->ptr)),
                          expr,
                          NEW(LilyCheckedExprLiteral,
                              LILY_CHECKED_EXPR_LITERAL_KIND_NIL));
                    default:
                        goto get_nil_unknown_dt;
                }
            } else {
            get_nil_unknown_dt : {
                return NEW_VARIANT(
                  LilyCheckedExpr,
                  literal,
                  &expr->location,
                  NEW_VARIANT(LilyCheckedDataType,
                              ptr,
                              &expr->location,
                              NEW(LilyCheckedDataType,
                                  LILY_CHECKED_DATA_TYPE_KIND_UNKNOWN,
                                  &expr->location)),
                  expr,
                  NEW(LilyCheckedExprLiteral,
                      LILY_CHECKED_EXPR_LITERAL_KIND_NIL));
            }
            }
        case LILY_AST_EXPR_LITERAL_KIND_NONE:
            return NEW_VARIANT(
              LilyCheckedExpr,
              literal,
              &expr->location,
              NEW_VARIANT(LilyCheckedDataType,
                          optional,
                          &expr->location,
                          NEW(LilyCheckedDataType,
                              LILY_CHECKED_DATA_TYPE_KIND_UNKNOWN,
                              &expr->location)),
              expr,
              NEW(LilyCheckedExprLiteral, LILY_CHECKED_EXPR_LITERAL_KIND_NONE));
        case LILY_AST_EXPR_LITERAL_KIND_STR:
            return NEW_VARIANT(
              LilyCheckedExpr,
              literal,
              &expr->location,
              NEW_VARIANT(LilyCheckedDataType,
                          str,
                          &expr->location,
                          expr->literal.str->len),
              expr,
              NEW_VARIANT(LilyCheckedExprLiteral, str, expr->literal.str));
        case LILY_AST_EXPR_LITERAL_KIND_SUFFIX_FLOAT32:
            return NEW_VARIANT(LilyCheckedExpr,
                               literal,
                               &expr->location,
                               NEW(LilyCheckedDataType,
                                   LILY_CHECKED_DATA_TYPE_KIND_FLOAT32,
                                   &expr->location),
                               expr,
                               NEW_VARIANT(LilyCheckedExprLiteral,
                                           suffix_float32,
                                           expr->literal.suffix_float32));
        case LILY_AST_EXPR_LITERAL_KIND_SUFFIX_FLOAT64:
            return NEW_VARIANT(LilyCheckedExpr,
                               literal,
                               &expr->location,
                               NEW(LilyCheckedDataType,
                                   LILY_CHECKED_DATA_TYPE_KIND_FLOAT64,
                                   &expr->location),
                               expr,
                               NEW_VARIANT(LilyCheckedExprLiteral,
                                           suffix_float64,
                                           expr->literal.suffix_float64));
        case LILY_AST_EXPR_LITERAL_KIND_SUFFIX_INT8:
            return NEW_VARIANT(LilyCheckedExpr,
                               literal,
                               &expr->location,
                               NEW(LilyCheckedDataType,
                                   LILY_CHECKED_DATA_TYPE_KIND_INT8,
                                   &expr->location),
                               expr,
                               NEW_VARIANT(LilyCheckedExprLiteral,
                                           suffix_int8,
                                           expr->literal.suffix_int8));
        case LILY_AST_EXPR_LITERAL_KIND_SUFFIX_INT16:
            return NEW_VARIANT(LilyCheckedExpr,
                               literal,
                               &expr->location,
                               NEW(LilyCheckedDataType,
                                   LILY_CHECKED_DATA_TYPE_KIND_INT16,
                                   &expr->location),
                               expr,
                               NEW_VARIANT(LilyCheckedExprLiteral,
                                           suffix_int16,
                                           expr->literal.suffix_int16));
        case LILY_AST_EXPR_LITERAL_KIND_SUFFIX_INT32:
            return NEW_VARIANT(LilyCheckedExpr,
                               literal,
                               &expr->location,
                               NEW(LilyCheckedDataType,
                                   LILY_CHECKED_DATA_TYPE_KIND_INT32,
                                   &expr->location),
                               expr,
                               NEW_VARIANT(LilyCheckedExprLiteral,
                                           suffix_int32,
                                           expr->literal.suffix_int32));
        case LILY_AST_EXPR_LITERAL_KIND_SUFFIX_INT64:
            return NEW_VARIANT(LilyCheckedExpr,
                               literal,
                               &expr->location,
                               NEW(LilyCheckedDataType,
                                   LILY_CHECKED_DATA_TYPE_KIND_INT64,
                                   &expr->location),
                               expr,
                               NEW_VARIANT(LilyCheckedExprLiteral,
                                           suffix_int64,
                                           expr->literal.suffix_int64));
        case LILY_AST_EXPR_LITERAL_KIND_SUFFIX_ISIZE:
            return NEW_VARIANT(LilyCheckedExpr,
                               literal,
                               &expr->location,
                               NEW(LilyCheckedDataType,
                                   LILY_CHECKED_DATA_TYPE_KIND_ISIZE,
                                   &expr->location),
                               expr,
                               NEW_VARIANT(LilyCheckedExprLiteral,
                                           suffix_isize,
                                           expr->literal.suffix_isize));
        case LILY_AST_EXPR_LITERAL_KIND_SUFFIX_UINT8:
            return NEW_VARIANT(LilyCheckedExpr,
                               literal,
                               &expr->location,
                               NEW(LilyCheckedDataType,
                                   LILY_CHECKED_DATA_TYPE_KIND_UINT8,
                                   &expr->location),
                               expr,
                               NEW_VARIANT(LilyCheckedExprLiteral,
                                           suffix_uint8,
                                           expr->literal.suffix_uint8));
        case LILY_AST_EXPR_LITERAL_KIND_SUFFIX_UINT16:
            return NEW_VARIANT(LilyCheckedExpr,
                               literal,
                               &expr->location,
                               NEW(LilyCheckedDataType,
                                   LILY_CHECKED_DATA_TYPE_KIND_UINT16,
                                   &expr->location),
                               expr,
                               NEW_VARIANT(LilyCheckedExprLiteral,
                                           suffix_uint16,
                                           expr->literal.suffix_uint16));
        case LILY_AST_EXPR_LITERAL_KIND_SUFFIX_UINT32:
            return NEW_VARIANT(LilyCheckedExpr,
                               literal,
                               &expr->location,
                               NEW(LilyCheckedDataType,
                                   LILY_CHECKED_DATA_TYPE_KIND_UINT32,
                                   &expr->location),
                               expr,
                               NEW_VARIANT(LilyCheckedExprLiteral,
                                           suffix_uint32,
                                           expr->literal.suffix_uint32));
        case LILY_AST_EXPR_LITERAL_KIND_SUFFIX_UINT64:
            return NEW_VARIANT(LilyCheckedExpr,
                               literal,
                               &expr->location,
                               NEW(LilyCheckedDataType,
                                   LILY_CHECKED_DATA_TYPE_KIND_UINT64,
                                   &expr->location),
                               expr,
                               NEW_VARIANT(LilyCheckedExprLiteral,
                                           suffix_uint64,
                                           expr->literal.suffix_uint64));
        case LILY_AST_EXPR_LITERAL_KIND_SUFFIX_USIZE:
            return NEW_VARIANT(LilyCheckedExpr,
                               literal,
                               &expr->location,
                               NEW(LilyCheckedDataType,
                                   LILY_CHECKED_DATA_TYPE_KIND_USIZE,
                                   &expr->location),
                               expr,
                               NEW_VARIANT(LilyCheckedExprLiteral,
                                           suffix_usize,
                                           expr->literal.suffix_usize));
        case LILY_AST_EXPR_LITERAL_KIND_UINT32:
            return NEW_VARIANT(LilyCheckedExpr,
                               literal,
                               &expr->location,
                               NEW(LilyCheckedDataType,
                                   LILY_CHECKED_DATA_TYPE_KIND_UINT32,
                                   &expr->location),
                               expr,
                               NEW_VARIANT(LilyCheckedExprLiteral,
                                           suffix_uint32,
                                           expr->literal.suffix_uint32));
        case LILY_AST_EXPR_LITERAL_KIND_UINT64:
            return NEW_VARIANT(LilyCheckedExpr,
                               literal,
                               &expr->location,
                               NEW(LilyCheckedDataType,
                                   LILY_CHECKED_DATA_TYPE_KIND_UINT64,
                                   &expr->location),
                               expr,
                               NEW_VARIANT(LilyCheckedExprLiteral,
                                           suffix_uint64,
                                           expr->literal.suffix_uint64));
        case LILY_AST_EXPR_LITERAL_KIND_UNDEF:
            return NEW_VARIANT(LilyCheckedExpr,
                               literal,
                               &expr->location,
                               NEW(LilyCheckedDataType,
                                   LILY_CHECKED_DATA_TYPE_KIND_UNKNOWN,
                                   &expr->location),
                               expr,
                               NEW(LilyCheckedExprLiteral,
                                   LILY_CHECKED_EXPR_LITERAL_KIND_UNDEF));
        case LILY_AST_EXPR_LITERAL_KIND_UNIT:
            return NEW_VARIANT(
              LilyCheckedExpr,
              literal,
              &expr->location,
              NEW(LilyCheckedDataType,
                  LILY_CHECKED_DATA_TYPE_KIND_UNIT,
                  &expr->location),
              expr,
              NEW(LilyCheckedExprLiteral, LILY_CHECKED_EXPR_LITERAL_KIND_UNIT));
        default:
            UNREACHABLE("unknown variant");
    }
}

LilyCheckedExpr *
check_tuple_expr__LilyAnalysis(LilyAnalysis *self,
                               LilyAstExpr *expr,
                               LilyCheckedScope *scope,
                               enum LilyCheckedSafetyMode safety_mode,
                               LilyCheckedDataType *defined_data_type)
{
    if (defined_data_type) {
        switch (defined_data_type->kind) {
            case LILY_CHECKED_DATA_TYPE_KIND_TUPLE:
                break;
            default:
                ANALYSIS_EMIT_DIAGNOSTIC(
                  self,
                  simple_lily_error,
                  (&expr->location),
                  NEW(LilyError, LILY_ERROR_KIND_EXPECTED_DATA_TYPE),
                  NULL,
                  NULL,
                  from__String("expected tuple data type"));
        }

        if (defined_data_type->tuple->len != expr->tuple.items->len) {
            ANALYSIS_EMIT_DIAGNOSTIC(
              self,
              simple_lily_error,
              (&expr->location),
              NEW(LilyError, LILY_ERROR_KIND_TUPLES_HAVE_NOT_SAME_SIZE),
              NULL,
              NULL,
              NULL);
        }

        const Usize min_tuple_len =
          defined_data_type->tuple->len > expr->tuple.items->len
            ? defined_data_type->tuple->len
            : expr->tuple.items->len;
        Vec *tuple = NEW(Vec); // Vec<LilyCheckedExpr*>*

        for (Usize i = 0; i < min_tuple_len; ++i) {
            push__Vec(
              tuple,
              check_expr__LilyAnalysis(self,
                                       get__Vec(expr->tuple.items, i),
                                       scope,
                                       safety_mode,
                                       false,
                                       get__Vec(defined_data_type->tuple, i)));
        }

        return NEW_VARIANT(LilyCheckedExpr,
                           tuple,
                           &expr->location,
                           ref__LilyCheckedDataType(defined_data_type),
                           expr,
                           NEW(LilyCheckedExprTuple, tuple));
    } else {
        Vec *tuple_dt = NEW(Vec);   // Vec<LilyCheckedDataType*>*
        Vec *tuple_expr = NEW(Vec); // Vec<LilyCheckedExpr*>*

        for (Usize i = 0; i < expr->tuple.items->len; ++i) {
            LilyCheckedExpr *item =
              check_expr__LilyAnalysis(self,
                                       get__Vec(expr->tuple.items, i),
                                       scope,
                                       safety_mode,
                                       false,
                                       get__Vec(defined_data_type->tuple, i));

            push__Vec(tuple_dt, ref__LilyCheckedDataType(item->data_type));
            push__Vec(tuple_expr, item);
        }

        return NEW_VARIANT(
          LilyCheckedExpr,
          tuple,
          &expr->location,
          NEW_VARIANT(LilyCheckedDataType, tuple, &expr->location, tuple_dt),
          expr,
          NEW(LilyCheckedExprTuple, tuple_expr));
    }
}

LilyCheckedExpr *
check_unary_expr__LilyAnalysis(LilyAnalysis *self,
                               LilyAstExpr *expr,
                               LilyCheckedScope *scope,
                               enum LilyCheckedSafetyMode safety_mode,
                               LilyCheckedDataType *defined_data_type)
{
    LilyCheckedExpr *right = check_expr__LilyAnalysis(
      self, expr->unary.right, scope, safety_mode, false, NULL);

    switch (expr->unary.kind) {
        case LILY_AST_EXPR_UNARY_KIND_DEREFERENCE: {
            switch (right->data_type->kind) {
                case LILY_CHECKED_DATA_TYPE_KIND_PTR:
                    return NEW_VARIANT(
                      LilyCheckedExpr,
                      unary,
                      &expr->location,
                      clone__LilyCheckedDataType(right->data_type->ptr),
                      expr,
                      NEW(LilyCheckedExprUnary,
                          LILY_CHECKED_EXPR_UNARY_KIND_DEREFERENCE,
                          right));
                case LILY_CHECKED_DATA_TYPE_KIND_REF:
                    return NEW_VARIANT(
                      LilyCheckedExpr,
                      unary,
                      &expr->location,
                      clone__LilyCheckedDataType(right->data_type->ref),
                      expr,
                      NEW(LilyCheckedExprUnary,
                          LILY_CHECKED_EXPR_UNARY_KIND_DEREFERENCE,
                          right));
                default:
                    TODO("check if dereference is implemented for this "
                         "type");
            }
        }
        case LILY_AST_EXPR_UNARY_KIND_NEG: {
            if (is_integer_data_type__LilyCheckedDataType(right->data_type) ||
                is_float_data_type__LilyCheckedDataType(right->data_type)) {
                return NEW_VARIANT(LilyCheckedExpr,
                                   unary,
                                   &expr->location,
                                   clone__LilyCheckedDataType(right->data_type),
                                   expr,
                                   NEW(LilyCheckedExprUnary,
                                       LILY_CHECKED_EXPR_UNARY_KIND_NEG,
                                       right));
            } else {
                TODO("check if `-` operator is implemented for this type");
            }
        }
        case LILY_CHECKED_EXPR_UNARY_KIND_NOT: {
            switch (right->data_type->kind) {
                case LILY_CHECKED_DATA_TYPE_KIND_BOOL:
                    return NEW_VARIANT(
                      LilyCheckedExpr,
                      unary,
                      &expr->location,
                      clone__LilyCheckedDataType(right->data_type),
                      expr,
                      NEW(LilyCheckedExprUnary,
                          LILY_CHECKED_EXPR_UNARY_KIND_NOT,
                          right));
                default:
                    TODO("check if `not` operator is implemented for "
                         "this type");
            }
        }
        case LILY_CHECKED_EXPR_UNARY_KIND_REF: {
            if (defined_data_type) {
                switch (defined_data_type->kind) {
                    case LILY_CHECKED_DATA_TYPE_KIND_PTR:
                        return NEW_VARIANT(
                          LilyCheckedExpr,
                          unary,
                          &expr->location,
                          NEW_VARIANT(
                            LilyCheckedDataType,
                            ptr,
                            &expr->location,
                            clone__LilyCheckedDataType(right->data_type)),
                          expr,
                          NEW(LilyCheckedExprUnary,
                              LILY_CHECKED_EXPR_UNARY_KIND_REF,
                              right));
                    case LILY_CHECKED_DATA_TYPE_KIND_REF:
                    get_ref : {
                        return NEW_VARIANT(
                          LilyCheckedExpr,
                          unary,
                          &expr->location,
                          NEW_VARIANT(
                            LilyCheckedDataType,
                            ref,
                            &expr->location,
                            clone__LilyCheckedDataType(right->data_type)),
                          expr,
                          NEW(LilyCheckedExprUnary,
                              LILY_CHECKED_EXPR_UNARY_KIND_REF,
                              right));
                    }
                    default:
                        ANALYSIS_EMIT_DIAGNOSTIC(
                          self,
                          simple_lily_error,
                          defined_data_type->location,
                          NEW(LilyError, LILY_ERROR_KIND_EXPECTED_DATA_TYPE),
                          init__Vec(1, from__String("ref is not overloadable")),
                          NULL,
                          from__String("expected ref or pointer data type"));

                        return NEW_VARIANT(
                          LilyCheckedExpr, unknown, &expr->location, expr);
                }
            } else {
                goto get_ref;
            }
        }
        default:
            TODO("unary expression");
    }
}

LilyCheckedExpr *
check_expr__LilyAnalysis(LilyAnalysis *self,
                         LilyAstExpr *expr,
                         LilyCheckedScope *scope,
                         enum LilyCheckedSafetyMode safety_mode,
                         bool must_mut,
                         LilyCheckedDataType *defined_data_type)
{
    LilyCheckedExpr *res = NULL;

    switch (expr->kind) {
        case LILY_AST_EXPR_KIND_ACCESS:
            switch (expr->access.kind) {
                case LILY_AST_EXPR_ACCESS_KIND_GLOBAL_PATH:
                    res = check_global_path_expr__LilyAnalysis(
                      self, expr, safety_mode, defined_data_type);
                    break;
                case LILY_AST_EXPR_ACCESS_KIND_SELF_PATH:
                    res = check_Self_path_expr__LilyAnalysis(self,
                                                             expr,
                                                             scope,
                                                             safety_mode,
                                                             must_mut,
                                                             defined_data_type);
                    break;
                case LILY_AST_EXPR_ACCESS_KIND_self_PATH:
                    res = check_self_path_expr__LilyAnalysis(self,
                                                             expr,
                                                             scope,
                                                             safety_mode,
                                                             must_mut,
                                                             defined_data_type);
                    break;
                case LILY_AST_EXPR_ACCESS_KIND_HOOK:
                    res =
                      check_hook_access_expr__LilyAnalysis(self,
                                                           expr,
                                                           scope,
                                                           safety_mode,
                                                           must_mut,
                                                           defined_data_type);
                    break;
                case LILY_AST_EXPR_ACCESS_KIND_OBJECT_PATH:
                    UNREACHABLE("Object is not expected in this context");
                case LILY_AST_EXPR_ACCESS_KIND_PROPERTY_INIT:
                    res =
                      check_property_init_expr__LilyAnalysis(self,
                                                             expr,
                                                             scope,
                                                             safety_mode,
                                                             must_mut,
                                                             defined_data_type);
                    break;
                case LILY_AST_EXPR_ACCESS_KIND_PATH:
                    res =
                      check_path_access_expr__LilyAnalysis(self,
                                                           expr,
                                                           scope,
                                                           safety_mode,
                                                           must_mut,
                                                           defined_data_type);
                    break;
                default:
                    UNREACHABLE("unknown variant");
            }

            break;
        case LILY_AST_EXPR_KIND_ARRAY:
            res = check_array_expr__LilyAnalysis(
              self, expr, scope, safety_mode, must_mut, defined_data_type);
            break;
        case LILY_AST_EXPR_KIND_BINARY:
            res = check_binary_expr__LilyAnalysis(
              self, expr, scope, safety_mode, defined_data_type);
            break;
        case LILY_AST_EXPR_KIND_CALL:
            res = check_call_expr__LilyAnalysis(
              self, expr, scope, safety_mode, must_mut, defined_data_type);
            break;
        case LILY_AST_EXPR_KIND_CAST:
            res = check_cast_expr__LilyAnalysis(
              self, expr, scope, safety_mode, defined_data_type);
            break;
        case LILY_AST_EXPR_KIND_GROUPING: {
            LilyCheckedExpr *grouping = check_expr__LilyAnalysis(
              self, expr->grouping, scope, safety_mode, must_mut, NULL);

            res = NEW_VARIANT(LilyCheckedExpr,
                              grouping,
                              &expr->location,
                              ref__LilyCheckedDataType(grouping->data_type),
                              expr,
                              grouping);
            break;
        }
        case LILY_AST_EXPR_KIND_IDENTIFIER:
            res = check_identifier_expr__LilyAnalysis(
              self, expr, scope, defined_data_type, safety_mode, must_mut);
            break;
        case LILY_AST_EXPR_KIND_IDENTIFIER_DOLLAR:
            TODO("identifier dollar expression");
        case LILY_AST_EXPR_KIND_LAMBDA:
            TODO("lambda expression");
        case LILY_AST_EXPR_KIND_LIST:
            res = check_list_expr__LilyAnalysis(
              self, expr, scope, safety_mode, defined_data_type);
            break;
        case LILY_AST_EXPR_KIND_LITERAL:
            res = check_literal_expr__LilyAnalysis(
              self, expr, scope, safety_mode, defined_data_type);
            break;
        case LILY_AST_EXPR_KIND_SELF:
            TODO("self expression");
        case LILY_AST_EXPR_KIND_TRY:
            TODO("try expression");
        case LILY_AST_EXPR_KIND_TUPLE:
            res = check_tuple_expr__LilyAnalysis(
              self, expr, scope, safety_mode, defined_data_type);
            break;
        case LILY_AST_EXPR_KIND_UNARY:
            res = check_unary_expr__LilyAnalysis(
              self, expr, scope, safety_mode, defined_data_type);
            break;
        case LILY_AST_EXPR_KIND_WILDCARD:
            TODO("wildcard expression");
        default:
            UNREACHABLE("unknown variant");
    }

    if (!res->data_type->is_lock) {
        const LilyCheckedScopeDecls *parent =
          get_parent__LilyCheckedScope(scope);

        if (parent) {
            add_unlock_data_type__LilyCheckedScopeDecls(parent, res->data_type);
        }
    }

    return res;
}

LilyCheckedBodyFunItem *
check_asm_stmt__LilyAnalysis(LilyAnalysis *self,
                             const LilyAstStmt *stmt,
                             LilyCheckedScope *scope,
                             enum LilyCheckedSafetyMode safety_mode)
{
    TODO("analysis asm stmt");
}

LilyCheckedBodyFunItem *
check_await_stmt__LilyAnalysis(LilyAnalysis *self,
                               const LilyAstStmt *stmt,
                               LilyCheckedScope *scope,
                               enum LilyCheckedSafetyMode safety_mode)
{
    TODO("analysis await stmt");
}

LilyCheckedBodyFunItem *
check_block_stmt__LilyAnalysis(LilyAnalysis *self,
                               const LilyAstStmt *stmt,
                               LilyCheckedScope *scope,
                               bool in_loop,
                               enum LilyCheckedSafetyMode safety_mode,
                               Vec *end_body,
                               Vec *current_body)
{
    Vec *body = NEW(Vec);
    LilyCheckedScope *scope_block =
      NEW(LilyCheckedScope,
          NEW_VARIANT(LilyCheckedParent, scope, scope, current_body),
          NEW_VARIANT(LilyCheckedScopeDecls, scope, body));

    CHECK_FUN_BODY(stmt->block.body, scope_block, body, safety_mode, in_loop);

    return NEW_VARIANT(
      LilyCheckedBodyFunItem,
      stmt,
      NEW_VARIANT(LilyCheckedStmt,
                  block,
                  &stmt->location,
                  stmt,
                  NEW(LilyCheckedStmtBlock, body, scope_block)));
}

LilyCheckedBodyFunItem *
check_break_stmt__LilyAnalysis(LilyAnalysis *self,
                               const LilyAstStmt *stmt,
                               LilyCheckedScope *scope,
                               bool in_loop,
                               enum LilyCheckedSafetyMode safety_mode)
{
    if (!in_loop) {
        ANALYSIS_EMIT_DIAGNOSTIC(
          self,
          simple_lily_error,
          (&stmt->location),
          NEW(LilyError, LILY_ERROR_KIND_BREAK_IS_NOT_EXPECTED_IN_THIS_CONTEXT),
          NULL,
          NULL,
          NULL);
    }

    return NEW_VARIANT(
      LilyCheckedBodyFunItem,
      stmt,
      NEW_VARIANT(LilyCheckedStmt,
                  break,
                  &stmt->location,
                  stmt,
                  NEW(LilyCheckedStmtBreak, stmt->break_.name)));
}

LilyCheckedBodyFunItem *
check_defer_stmt__LilyAnalysis(LilyAnalysis *self,
                               const LilyAstStmt *stmt,
                               LilyCheckedScope *scope,
                               Usize i,
                               bool in_loop,
                               enum LilyCheckedSafetyMode safety_mode,
                               Vec *end_body,
                               Vec *current_body)
{
    LilyCheckedBodyFunItem *checked_item =
      check_fun_item__LilyAnalysis(self,
                                   stmt->defer.item,
                                   scope,
                                   i,
                                   in_loop,
                                   safety_mode,
                                   end_body,
                                   current_body);

    if (checked_item) {
        push__Vec(end_body, checked_item);
    }

    return NULL;
}

LilyCheckedBodyFunItem *
check_drop_stmt__LilyAnalysis(LilyAnalysis *self,
                              const LilyAstStmt *stmt,
                              LilyCheckedScope *scope,
                              enum LilyCheckedSafetyMode safety_mode)
{
    LilyCheckedExpr *drop_expr = check_expr__LilyAnalysis(
      self, stmt->drop.expr, scope, safety_mode, false, NULL);

    switch (drop_expr->kind) {
        case LILY_CHECKED_EXPR_KIND_CALL:
            switch (drop_expr->call.kind) {
                case LILY_CHECKED_EXPR_CALL_KIND_VARIABLE: {
                    LilyCheckedBodyFunItem *variable =
                      get_variable_from_id__LilyCheckedScope(
                        scope,
                        drop_expr->call.scope.id,
                        drop_expr->call.global_name);

                    // Check if the data type of variable is droppable.
                    switch (variable->stmt.variable.data_type->kind) {
                        case LILY_CHECKED_DATA_TYPE_KIND_CUSTOM:
                            TODO("check if the custom data type is "
                                 "droppable");
                        case LILY_CHECKED_DATA_TYPE_KIND_PTR:
                            TODO("check if the pointer of the data "
                                 "type is droppable");
                        default:
                            ANALYSIS_EMIT_DIAGNOSTIC(
                              self,
                              simple_lily_error,
                              variable->stmt.variable.data_type->location,
                              NEW(
                                LilyError,
                                LILY_ERROR_KIND_THIS_DATA_TYPE_CANNOT_BE_DROPPED),
                              NULL,
                              NULL,
                              NULL);
                    }

                    break;
                }
                case LILY_CHECKED_EXPR_CALL_KIND_FUN_PARAM:
                    TODO("check if the fun param is droppable");
                case LILY_CHECKED_EXPR_CALL_KIND_FUN:
                    TODO("check if the data type of the function is "
                         "droppable");
                case LILY_CHECKED_EXPR_CALL_KIND_RECORD:
                    TODO("check if the data type of the record is "
                         "droppable");
                case LILY_CHECKED_EXPR_CALL_KIND_METHOD:
                    TODO("check if the data type of the method is "
                         "droppable");
                case LILY_CHECKED_EXPR_CALL_KIND_VARIANT:
                    TODO("check if the data type of the variant is "
                         "droppable");
                default:
                    ANALYSIS_EMIT_DIAGNOSTIC(
                      self,
                      simple_lily_error,
                      drop_expr->location,
                      NEW(
                        LilyError,
                        LILY_ERROR_KIND_THIS_KIND_OF_VALUE_IS_NOT_ALLOWED_TO_BE_DROP),
                      NULL,
                      NULL,
                      NULL);
            }

            break;
        default:
            ANALYSIS_EMIT_DIAGNOSTIC(
              self,
              simple_lily_error,
              drop_expr->location,
              NEW(LilyError,
                  LILY_ERROR_KIND_THIS_KIND_OF_EXPR_IS_NOT_ALLOWED_TO_BE_DROP),
              NULL,
              NULL,
              NULL);
    }

    return NEW_VARIANT(LilyCheckedBodyFunItem,
                       stmt,
                       NEW_VARIANT(LilyCheckedStmt,
                                   drop,
                                   &stmt->location,
                                   stmt,
                                   NEW(LilyCheckedStmtDrop, drop_expr)));
}

LilyCheckedBodyFunItem *
check_for_stmt__LilyAnalysis(LilyAnalysis *self,
                             const LilyAstStmt *stmt,
                             LilyCheckedScope *scope,
                             enum LilyCheckedSafetyMode safety_mode)
{
    TODO("check for stmt");
}

LilyCheckedBodyFunItem *
check_if_stmt__LilyAnalysis(LilyAnalysis *self,
                            const LilyAstStmt *stmt,
                            LilyCheckedScope *scope,
                            bool in_loop,
                            enum LilyCheckedSafetyMode safety_mode,
                            Vec *current_body)
{
    LilyCheckedExpr *if_cond = check_expr__LilyAnalysis(
      self, stmt->if_.if_expr, scope, safety_mode, false, NULL);

    EXPECTED_BOOL_EXPR(if_cond);

    Vec *if_body = NEW(Vec);
    LilyCheckedScope *if_scope =
      NEW(LilyCheckedScope,
          NEW_VARIANT(LilyCheckedParent, scope, scope, current_body),
          NEW_VARIANT(LilyCheckedScopeDecls, scope, if_body));

    CHECK_FUN_BODY(stmt->if_.if_body, if_scope, if_body, safety_mode, in_loop);

    Vec *elifs = NULL; // Vec<LilyCheckedStmtIfBranch*>*?

    if (stmt->if_.elif_exprs) {
        elifs = NEW(Vec);

        for (Usize k = 0; k < stmt->if_.elif_exprs->len; ++k) {
            LilyCheckedExpr *elif_cond =
              check_expr__LilyAnalysis(self,
                                       get__Vec(stmt->if_.elif_exprs, k),
                                       scope,
                                       safety_mode,
                                       false,
                                       NULL);

            EXPECTED_BOOL_EXPR(elif_cond);

            Vec *elif_body = NEW(Vec);
            LilyCheckedScope *elif_scope =
              NEW(LilyCheckedScope,
                  NEW_VARIANT(LilyCheckedParent, scope, scope, current_body),
                  NEW_VARIANT(LilyCheckedScopeDecls, scope, elif_body));
            Vec *ast_elif_body = get__Vec(stmt->if_.elif_bodies, k);

            CHECK_FUN_BODY(
              ast_elif_body, elif_scope, elif_body, safety_mode, in_loop);

            push__Vec(
              elifs,
              NEW(LilyCheckedStmtIfBranch, elif_cond, elif_body, elif_scope));
        }
    }

    LilyCheckedStmtElseBranch *else_ = NULL;

    if (stmt->if_.else_body) {
        Vec *else_body = NEW(Vec);
        LilyCheckedScope *else_scope =
          NEW(LilyCheckedScope,
              NEW_VARIANT(LilyCheckedParent, scope, scope, current_body),
              NEW_VARIANT(LilyCheckedScopeDecls, scope, else_body));

        CHECK_FUN_BODY(
          stmt->if_.else_body, else_scope, else_body, safety_mode, in_loop);

        else_ = NEW(LilyCheckedStmtElseBranch, else_body, else_scope);
    }

    return NEW_VARIANT(
      LilyCheckedBodyFunItem,
      stmt,
      NEW_VARIANT(LilyCheckedStmt,
                  if,
                  &stmt->location,
                  stmt,
                  NEW(LilyCheckedStmtIf,
                      NEW(LilyCheckedStmtIfBranch, if_cond, if_body, if_scope),
                      elifs,
                      else_)));
}

LilyCheckedBodyFunItem *
check_match_stmt__LilyAnalysis(LilyAnalysis *self,
                               const LilyAstStmt *stmt,
                               LilyCheckedScope *scope,
                               bool in_loop,
                               enum LilyCheckedSafetyMode safety_mode,
                               Vec *current_body)
{
    TODO("analysis match stmt");
}

LilyCheckedBodyFunItem *
check_next_stmt__LilyAnalysis(LilyAnalysis *self,
                              const LilyAstStmt *stmt,
                              LilyCheckedScope *scope,
                              bool in_loop,
                              enum LilyCheckedSafetyMode safety_mode)
{
    if (!in_loop) {
        ANALYSIS_EMIT_DIAGNOSTIC(
          self,
          simple_lily_error,
          (&stmt->location),
          NEW(LilyError, LILY_ERROR_KIND_NEXT_IS_NOT_EXPECTED_IN_THIS_CONTEXT),
          NULL,
          NULL,
          NULL);
    }

    return NEW_VARIANT(LilyCheckedBodyFunItem,
                       stmt,
                       NEW_VARIANT(LilyCheckedStmt,
                                   next,
                                   &stmt->location,
                                   stmt,
                                   NEW(LilyCheckedStmtNext, stmt->next.name)));
}

LilyCheckedBodyFunItem *
check_raise_stmt__LilyAnalysis(LilyAnalysis *self,
                               const LilyAstStmt *stmt,
                               LilyCheckedScope *scope,
                               bool in_loop,
                               enum LilyCheckedSafetyMode safety_mode)
{
    LilyCheckedScopeResponse raise_response_expr =
      resolve_id__LilyAnalysis(self,
                               stmt->raise.expr,
                               scope,
                               LILY_CHECKED_SCOPE_RESPONSE_KIND_ERROR,
                               safety_mode);

    switch (raise_response_expr.kind) {
        case LILY_CHECKED_SCOPE_RESPONSE_KIND_NOT_FOUND:
            ANALYSIS_EMIT_DIAGNOSTIC(
              self,
              simple_lily_error,
              (&stmt->raise.expr->location),
              NEW(LilyError, LILY_ERROR_KIND_ERROR_DECL_NOT_FOUND),
              NULL,
              NULL,
              NULL);

            return NEW_VARIANT(
              LilyCheckedBodyFunItem,
              stmt,
              NEW_VARIANT(LilyCheckedStmt,
                          raise,
                          &stmt->location,
                          stmt,
                          NEW(LilyCheckedStmtRaise,
                              NEW_VARIANT(LilyCheckedExpr,
                                          unknown,
                                          &stmt->raise.expr->location,
                                          stmt->raise.expr))));
        case LILY_CHECKED_SCOPE_RESPONSE_KIND_ERROR: {
            if (!raise_response_expr.error->is_checked) {
                TODO("check error declaration");
            }

            // TODO: add a support for generics
            LilyCheckedExpr *raise_expr = NEW_VARIANT(
              LilyCheckedExpr,
              call,
              &stmt->raise.expr->location,
              NEW_VARIANT(
                LilyCheckedDataType,
                custom,
                &stmt->raise.expr->location,
                NEW(LilyCheckedDataTypeCustom,
                    raise_response_expr.scope_container.scope_id,
                    (LilyCheckedAccessScope){
                      .id = raise_response_expr.scope_container.error->id },
                    raise_response_expr.error->name,
                    raise_response_expr.error->global_name,
                    NULL,
                    LILY_CHECKED_DATA_TYPE_CUSTOM_KIND_ERROR,
                    false)),
              stmt->raise.expr,
              NEW(LilyCheckedExprCall,
                  LILY_CHECKED_EXPR_CALL_KIND_ERROR,
                  raise_response_expr.error->global_name,
                  (LilyCheckedAccessScope){
                    .id = raise_response_expr.scope_container.scope_id }));

            // TODO: later replace
            // `get_current_fun__LilyCheckedScope` by
            // `get_parent__LilyCheckedScope`, when method
            // or lambda are be available.
            {
                LilyCheckedScopeDecls *fun =
                  get_current_fun__LilyCheckedScope(scope);

                ASSERT(fun);

                add_raise__LilyCheckedDeclFun(
                  &fun->decl->fun, scope, raise_expr->data_type, in_try);
            }

            return NEW_VARIANT(
              LilyCheckedBodyFunItem,
              stmt,
              NEW_VARIANT(LilyCheckedStmt,
                          raise,
                          &stmt->location,
                          stmt,
                          NEW(LilyCheckedStmtRaise, raise_expr)));
        }
        default:
            UNREACHABLE("unknown variant");
    }
}

LilyCheckedBodyFunItem *
check_return_stmt__LilyAnalysis(LilyAnalysis *self,
                                const LilyAstStmt *stmt,
                                LilyCheckedScope *scope,
                                bool in_loop,
                                enum LilyCheckedSafetyMode safety_mode,
                                Vec *end_body,
                                Vec *current_body)
{
    LilyCheckedScopeDecls *current_fun =
      get_current_fun__LilyCheckedScope(scope);
    LilyCheckedExpr *expr = NULL;
    LilyCheckedDataType *fun_return_data_type = NULL;

    ASSERT(current_fun);

    // TODO: add a support for the lambda statement
    switch (current_fun->kind) {
        case LILY_CHECKED_SCOPE_DECLS_KIND_DECL:
            switch (current_fun->decl->kind) {
                case LILY_CHECKED_DECL_KIND_FUN:
                    fun_return_data_type =
                      current_fun->decl->fun.return_data_type;

                    break;
                default:
                    UNREACHABLE("must obtain a function or a lambda function");
            }

            break;
        default:
            UNREACHABLE("must obtain a declaration");
    }

    if (stmt->return_.expr) {
        // Check the data type of the return expression
        expr = check_expr__LilyAnalysis(self,
                                        stmt->return_.expr,
                                        scope,
                                        safety_mode,
                                        false,
                                        fun_return_data_type);

        if (fun_return_data_type) {
            if (fun_return_data_type->kind ==
                  LILY_CHECKED_DATA_TYPE_KIND_UNKNOWN &&
                expr->data_type->kind == LILY_CHECKED_DATA_TYPE_KIND_UNKNOWN) {
                // TODO: add method
                LilyCheckedScopeDecls *current_fun =
                  get_current_fun__LilyCheckedScope(scope);

                ASSERT(current_fun);

                add_compiler_generic__LilyCheckedCompilerGeneric(
                  current_fun->decl->fun.used_compiler_generic);

                fun_return_data_type->kind =
                  LILY_CHECKED_DATA_TYPE_KIND_COMPILER_GENERIC;
                fun_return_data_type->compiler_generic =
                  NEW(LilyCheckedDataTypeCompilerGeneric,
                      last__Vec(current_fun->decl->fun.used_compiler_generic));

                ASSERT(expr->data_type->ref_count == 0);

                update_data_type__LilyCheckedDataType(expr->data_type,
                                                      fun_return_data_type);
            } else if (fun_return_data_type->kind ==
                         LILY_CHECKED_DATA_TYPE_KIND_COMPILER_GENERIC &&
                       expr->data_type->kind ==
                         LILY_CHECKED_DATA_TYPE_KIND_UNKNOWN) {
                ASSERT(expr->data_type->ref_count == 0);

                update_data_type__LilyCheckedDataType(expr->data_type,
                                                      fun_return_data_type);
            } else if (fun_return_data_type->kind ==
                         LILY_CHECKED_DATA_TYPE_KIND_UNKNOWN &&
                       expr->data_type->kind ==
                         LILY_CHECKED_DATA_TYPE_KIND_COMPILER_GENERIC) {
                ASSERT(fun_return_data_type->ref_count == 0);

                update_data_type__LilyCheckedDataType(fun_return_data_type,
                                                      expr->data_type);
            } else if (!eq__LilyCheckedDataType(fun_return_data_type,
                                                expr->data_type)) {
                // TODO: add help on this error
                const Location *location = fun_return_data_type->location
                                             ? fun_return_data_type->location
                                             : expr->data_type->location;

                ANALYSIS_EMIT_DIAGNOSTIC(
                  self,
                  simple_lily_error,
                  location,
                  NEW(
                    LilyError,
                    LILY_ERROR_KIND_DATA_TYPE_DONT_MATCH_WITH_INFER_DATA_TYPE),
                  NULL,
                  NULL,
                  from__String("or the specified return data type "
                               "passed to the function"));
            } else {
                update_data_type__LilyCheckedDataType(fun_return_data_type,
                                                      expr->data_type);
            }
        } else {
            UNREACHABLE("return data type cannot be null");
        }
    } else {
        if (fun_return_data_type) {
            switch (fun_return_data_type->kind) {
                case LILY_CHECKED_DATA_TYPE_KIND_UNIT:
                case LILY_CHECKED_DATA_TYPE_KIND_CVOID:
                    break;
                default:
                    ANALYSIS_EMIT_DIAGNOSTIC(
                      self,
                      simple_lily_error,
                      fun_return_data_type->location,
                      NEW(LilyError, LILY_ERROR_KIND_DATA_TYPE_DONT_MATCH),
                      NULL,
                      NULL,
                      from__String(
                        "expected Unit or CVoid as return data type"));
            }
        } else {
            // TODO:Add the possibility for the compiler to have a list
            // of potential return types (this data type will not be
            // definable by the user).
            if (fun_return_data_type->kind !=
                  LILY_CHECKED_DATA_TYPE_KIND_UNIT &&
                fun_return_data_type->kind !=
                  LILY_CHECKED_DATA_TYPE_KIND_CVOID &&
                fun_return_data_type->kind !=
                  LILY_CHECKED_DATA_TYPE_KIND_UNKNOWN) {
                FAILED("error mismatched data type");
            } else if (fun_return_data_type->kind ==
                       LILY_CHECKED_DATA_TYPE_KIND_UNKNOWN) {
                fun_return_data_type->kind = LILY_CHECKED_DATA_TYPE_KIND_UNIT;
            }
        }
    }

    // TODO: add the `end_body` implicit return.
    // Add the `end_body` before the return statement
    ASSERT(end_body);

    for (Usize i = 0; i < end_body->len; ++i) {
        push__Vec(current_body,
                  ref__LilyCheckedBodyFunItem(get__Vec(end_body, i)));
    }

    current_fun->decl->fun.has_return = true;

    return NEW_VARIANT(LilyCheckedBodyFunItem,
                       stmt,
                       NEW_VARIANT(LilyCheckedStmt,
                                   return,
                                   &stmt->location,
                                   stmt,
                                   NEW(LilyCheckedStmtReturn, expr)));
}

LilyCheckedBodyFunItem *
check_try_stmt__LilyAnalysis(LilyAnalysis *self,
                             const LilyAstStmt *stmt,
                             LilyCheckedScope *scope,
                             bool in_loop,
                             enum LilyCheckedSafetyMode safety_mode,
                             Vec *current_body)
{
    Vec *try_body = NEW(Vec);
    LilyCheckedScope *scope_try =
      NEW(LilyCheckedScope,
          NEW_VARIANT(LilyCheckedParent, scope, scope, current_body),
          NEW_VARIANT(LilyCheckedScopeDecls, scope, try_body));

    in_try = true;

    CHECK_FUN_BODY(stmt->try.try_body,
                   scope_try,
                   try_body,
                   LILY_CHECKED_SAFETY_MODE_SAFE,
                   in_loop);

    in_try = false;

    if (stmt->try.catch_body) {
        Vec *catch_body = NEW(Vec);
        LilyCheckedScope *scope_catch =
          NEW(LilyCheckedScope,
              NEW_VARIANT(LilyCheckedParent, scope, scope, current_body),
              NEW_VARIANT(LilyCheckedScopeDecls, scope, catch_body));

        if (stmt->try.catch_expr) {
            switch (stmt->try.catch_expr->kind) {
                case LILY_AST_EXPR_KIND_IDENTIFIER: {
                    set_catch_name__LilyCheckedScope(
                      scope_catch,
                      stmt->try.catch_expr->identifier.name,
                      &stmt->try.catch_expr->location,
                      scope_try->raises);

                    break;
                }
                default:
                    FAILED("expected identifier");
            }
        }

        CHECK_FUN_BODY(stmt->try.catch_body,
                       scope_catch,
                       catch_body,
                       LILY_CHECKED_SAFETY_MODE_SAFE,
                       in_loop);

        return NEW_VARIANT(LilyCheckedBodyFunItem,
                           stmt,
                           NEW_VARIANT(LilyCheckedStmt,
                                       try,
                                       &stmt->location,
                                       stmt,
                                       NEW(LilyCheckedStmtTry,
                                           try_body,
                                           scope_try,
                                           catch_body,
                                           scope_catch)));
    }

    return NEW_VARIANT(
      LilyCheckedBodyFunItem,
      stmt,
      NEW_VARIANT(LilyCheckedStmt,
                  try,
                  &stmt->location,
                  stmt,
                  NEW(LilyCheckedStmtTry, try_body, scope_try, NULL, NULL)));
}

LilyCheckedBodyFunItem *
check_unsafe_stmt__LilyAnalysis(LilyAnalysis *self,
                                const LilyAstStmt *stmt,
                                LilyCheckedScope *scope,
                                bool in_loop,
                                enum LilyCheckedSafetyMode safety_mode,
                                Vec *current_body)
{
    Vec *body = NEW(Vec);
    LilyCheckedScope *scope_unsafe =
      NEW(LilyCheckedScope,
          NEW_VARIANT(LilyCheckedParent, scope, scope, current_body),
          NEW_VARIANT(LilyCheckedScopeDecls, scope, body));

    CHECK_FUN_BODY(stmt->unsafe.body,
                   scope_unsafe,
                   body,
                   LILY_CHECKED_SAFETY_MODE_UNSAFE,
                   false);

    return NEW_VARIANT(
      LilyCheckedBodyFunItem,
      stmt,
      NEW_VARIANT(LilyCheckedStmt,
                  unsafe,
                  &stmt->location,
                  stmt,
                  NEW(LilyCheckedStmtUnsafe, body, scope_unsafe)));
}

LilyCheckedBodyFunItem *
check_variable_stmt__LilyAnalysis(LilyAnalysis *self,
                                  const LilyAstStmt *stmt,
                                  LilyCheckedScope *scope,
                                  Usize i,
                                  enum LilyCheckedSafetyMode safety_mode,
                                  Vec *end_body)
{
    LilyCheckedScopeContainerVariable *sc_variable =
      NEW(LilyCheckedScopeContainerVariable,
          stmt->variable.name,
          i - end_body->len);
    int status = add_variable__LilyCheckedScope(scope, sc_variable);

    if (status) {
        ANALYSIS_EMIT_DIAGNOSTIC(
          self,
          simple_lily_error,
          (&stmt->location),
          NEW(LilyError, LILY_ERROR_KIND_DUPLICATE_VARIABLE),
          NULL,
          NULL,
          NULL);

        FREE(LilyCheckedScopeContainerVariable, sc_variable);
    }

    LilyCheckedDataType *checked_data_type =
      stmt->variable.data_type
        ? check_data_type__LilyAnalysis(
            self, stmt->variable.data_type, scope, NULL, safety_mode)
        : NULL;
    LilyCheckedExpr *expr = check_expr__LilyAnalysis(
      self, stmt->variable.expr, scope, safety_mode, false, checked_data_type);

    if (checked_data_type) {
        if (!eq__LilyCheckedDataType(checked_data_type, expr->data_type)) {
            ANALYSIS_EMIT_DIAGNOSTIC(
              self,
              simple_lily_error,
              checked_data_type->location,
              NEW(LilyError, LILY_ERROR_KIND_DATA_TYPE_DONT_MATCH),
              NULL,
              NULL,
              NULL);
        }
    }

    return NEW_VARIANT(
      LilyCheckedBodyFunItem,
      stmt,
      NEW_VARIANT(LilyCheckedStmt,
                  variable,
                  &stmt->location,
                  stmt,
                  NEW(LilyCheckedStmtVariable,
                      stmt->variable.name,
                      checked_data_type
                        ? checked_data_type
                        : ref__LilyCheckedDataType(expr->data_type),
                      expr,
                      stmt->variable.is_mut)));
}

LilyCheckedBodyFunItem *
check_while_stmt__LilyAnalysis(LilyAnalysis *self,
                               const LilyAstStmt *stmt,
                               LilyCheckedScope *scope,
                               enum LilyCheckedSafetyMode safety_mode,
                               Vec *current_body)
{
    LilyCheckedExpr *expr = check_expr__LilyAnalysis(
      self, stmt->while_.expr, scope, safety_mode, false, NULL);

    EXPECTED_BOOL_EXPR(expr);

    Vec *body = NEW(Vec);
    LilyCheckedScope *scope_while =
      NEW(LilyCheckedScope,
          NEW_VARIANT(LilyCheckedParent, scope, scope, current_body),
          NEW_VARIANT(LilyCheckedScopeDecls, scope, body));

    CHECK_FUN_BODY(stmt->while_.body, scope_while, body, safety_mode, true);

    return NEW_VARIANT(
      LilyCheckedBodyFunItem,
      stmt,
      NEW_VARIANT(
        LilyCheckedStmt,
        while,
        &stmt->location,
        stmt,
        NEW(LilyCheckedStmtWhile, stmt->while_.name, expr, body, scope_while)));
}

LilyCheckedBodyFunItem *
check_stmt__LilyAnalysis(LilyAnalysis *self,
                         const LilyAstStmt *stmt,
                         LilyCheckedScope *scope,
                         Usize i,
                         bool in_loop,
                         enum LilyCheckedSafetyMode safety_mode,
                         Vec *end_body,
                         Vec *current_body)
{
    switch (stmt->kind) {
        case LILY_AST_STMT_KIND_ASM:
            return check_asm_stmt__LilyAnalysis(self, stmt, scope, safety_mode);
        case LILY_AST_STMT_KIND_AWAIT:
            return check_await_stmt__LilyAnalysis(
              self, stmt, scope, safety_mode);
        case LILY_AST_STMT_KIND_BLOCK:
            return check_block_stmt__LilyAnalysis(
              self, stmt, scope, in_loop, safety_mode, end_body, current_body);
        case LILY_AST_STMT_KIND_BREAK:
            return check_break_stmt__LilyAnalysis(
              self, stmt, scope, in_loop, safety_mode);
        case LILY_AST_STMT_KIND_DEFER:
            return check_defer_stmt__LilyAnalysis(self,
                                                  stmt,
                                                  scope,
                                                  i,
                                                  in_loop,
                                                  safety_mode,
                                                  end_body,
                                                  current_body);
        case LILY_AST_STMT_KIND_DROP:
            return check_drop_stmt__LilyAnalysis(
              self, stmt, scope, safety_mode);
        case LILY_AST_STMT_KIND_FOR:
            return check_for_stmt__LilyAnalysis(self, stmt, scope, safety_mode);
        case LILY_AST_STMT_KIND_IF:
            return check_if_stmt__LilyAnalysis(
              self, stmt, scope, in_loop, safety_mode, current_body);
        case LILY_AST_STMT_KIND_MATCH:
            return check_match_stmt__LilyAnalysis(
              self, stmt, scope, in_loop, safety_mode, current_body);
        case LILY_AST_STMT_KIND_NEXT:
            return check_next_stmt__LilyAnalysis(
              self, stmt, scope, in_loop, safety_mode);
        case LILY_AST_STMT_KIND_RAISE:
            return check_raise_stmt__LilyAnalysis(
              self, stmt, scope, in_loop, safety_mode);
        case LILY_AST_STMT_KIND_RETURN:
            return check_return_stmt__LilyAnalysis(
              self, stmt, scope, in_loop, safety_mode, end_body, current_body);
        case LILY_AST_STMT_KIND_TRY:
            return check_try_stmt__LilyAnalysis(
              self, stmt, scope, in_loop, safety_mode, current_body);
        case LILY_AST_STMT_KIND_UNSAFE:
            return check_unsafe_stmt__LilyAnalysis(
              self, stmt, scope, in_loop, safety_mode, current_body);
        case LILY_AST_STMT_KIND_VARIABLE:
            return check_variable_stmt__LilyAnalysis(
              self, stmt, scope, i, safety_mode, end_body);
        case LILY_AST_STMT_KIND_WHILE:
            return check_while_stmt__LilyAnalysis(
              self, stmt, scope, safety_mode, current_body);
        default:
            UNREACHABLE("unknown variant");
    }
}

LilyCheckedBodyFunItem *
check_fun_item__LilyAnalysis(LilyAnalysis *self,
                             const LilyAstBodyFunItem *ast_item,
                             LilyCheckedScope *scope,
                             Usize i,
                             bool in_loop,
                             enum LilyCheckedSafetyMode safety_mode,
                             Vec *end_body,
                             Vec *current_body)
{
    switch (ast_item->kind) {
        case LILY_AST_BODY_FUN_ITEM_KIND_EXPR:
            return NEW_VARIANT(
              LilyCheckedBodyFunItem,
              expr,
              check_expr__LilyAnalysis(
                self, ast_item->expr, scope, safety_mode, false, NULL));
        case LILY_AST_BODY_FUN_ITEM_KIND_STMT:
            return check_stmt__LilyAnalysis(self,
                                            &ast_item->stmt,
                                            scope,
                                            i,
                                            in_loop,
                                            safety_mode,
                                            end_body,
                                            current_body);
        default:
            UNREACHABLE("unknown variant");
    }
}

Vec *
check_generic_params(LilyAnalysis *self,
                     Vec *ast_generic_params,
                     LilyCheckedScope *scope)
{
    Vec *generic_params =
      ast_generic_params ? NEW(Vec) : NULL; // Vec<LilyCheckedGenericParam*>*?

    if (ast_generic_params) {
        for (Usize i = 0; i < ast_generic_params->len; ++i) {
            LilyAstGenericParam *generic_param =
              get__Vec(ast_generic_params, i);

            switch (generic_param->kind) {
                case LILY_AST_GENERIC_PARAM_KIND_CONSTRAINT:
                    TODO("constraint generic param is not yet implemented");
                case LILY_AST_GENERIC_PARAM_KIND_NORMAL:
                    if (add_generic__LilyCheckedScope(
                          scope,
                          NEW(LilyCheckedScopeContainerGeneric,
                              generic_param->normal,
                              i))) {
                        FAILED("duplicate generic param");
                    }

                    push__Vec(generic_params,
                              NEW_VARIANT(LilyCheckedGenericParam,
                                          normal,
                                          &generic_param->location,
                                          generic_param->normal));

                    break;
                default:
                    UNREACHABLE("unknown variant");
            }
        }
    }

    return generic_params;
}

Vec *
check_fun_params__LilyAnalysis(LilyAnalysis *self,
                               const Vec *params,
                               LilyCheckedScope *scope)
{
    Vec *checked_params = NEW(Vec);

    for (Usize i = 0; i < params->len; ++i) {
        LilyAstDeclFunParam *param = get__Vec(params, i);

        LilyCheckedDataType *checked_param_data_type = NULL;

        if (param->data_type) {
            // TODO: check the safety mode of the function
            checked_param_data_type =
              check_data_type__LilyAnalysis(self,
                                            param->data_type,
                                            scope,
                                            NULL,
                                            LILY_CHECKED_SAFETY_MODE_SAFE);
        } else {
            checked_param_data_type = NEW(LilyCheckedDataType,
                                          LILY_CHECKED_DATA_TYPE_KIND_UNKNOWN,
                                          &param->location);
        }

        switch (param->kind) {
            case LILY_AST_DECL_FUN_PARAM_KIND_DEFAULT: {
                LilyCheckedExpr *default_value =
                  check_expr__LilyAnalysis(self,
                                           param->default_,
                                           scope,
                                           LILY_CHECKED_SAFETY_MODE_SAFE,
                                           false,
                                           checked_param_data_type);

                if (default_value) {
                    push__Vec(checked_params,
                              NEW_VARIANT(LilyCheckedDeclFunParam,
                                          default,
                                          param->name,
                                          checked_param_data_type,
                                          &param->location,
                                          default_value));
                } else {
                    push__Vec(checked_params,
                              NEW_VARIANT(LilyCheckedDeclFunParam,
                                          normal,
                                          param->name,
                                          checked_param_data_type,
                                          &param->location));
                }

                break;
            }
            case LILY_AST_DECL_FUN_PARAM_KIND_NORMAL: {
                push__Vec(checked_params,
                          NEW_VARIANT(LilyCheckedDeclFunParam,
                                      normal,
                                      param->name,
                                      checked_param_data_type,
                                      &param->location));

                break;
            }
            default:
                UNREACHABLE("unknown variant");
        }

        LilyCheckedScopeContainerVariable *sc_variable =
          NEW(LilyCheckedScopeContainerVariable, param->name, i);

        int is_failed = add_param__LilyCheckedScope(scope, sc_variable);

        if (is_failed) {
            ANALYSIS_EMIT_DIAGNOSTIC(
              self,
              simple_lily_error,
              (&param->location),
              NEW(LilyError, LILY_ERROR_KIND_DUPLICATE_PARAM_NAME),
              NULL,
              NULL,
              NULL);

            FREE(LilyCheckedScopeContainerVariable, sc_variable);
        }
    }

    if (checked_params->len > MAX_FUN_PARAMS) {
        FAILED("too many params, expected less or equal to 256");
    }

    return checked_params;
}

void
check_fun_signature__LilyAnalysis(LilyAnalysis *self, LilyCheckedDecl *fun)
{
    // 1. Verify if it's the main function
    if (!strcmp(fun->fun.name->buffer, "main") &&
        !fun->fun.scope->parent->scope->parent &&
        self->package->status == LILY_PACKAGE_STATUS_MAIN) {
        fun->fun.is_main = true;
        self->package->main_is_found = true;
    }

    // 2. Check generic params
    if (fun->ast_decl->fun.generic_params) {
        if (fun->fun.is_main) {
            ANALYSIS_EMIT_DIAGNOSTIC(
              self,
              simple_lily_error,
              fun->location,
              NEW(
                LilyError,
                LILY_ERROR_KIND_GENERIC_PARAMS_ARE_NOT_EXPECTED_IN_MAIN_FUNCTION),
              NULL,
              NULL,
              NULL);
        }

        if (!fun->fun.generic_params) {
            fun->fun.generic_params = check_generic_params(
              self, fun->ast_decl->fun.generic_params, fun->fun.scope);
        }
    }

    // 3. Check params.
    if (fun->ast_decl->fun.params) {
        // Check if the main function have no params.
        if (fun->fun.is_main) {
            ANALYSIS_EMIT_DIAGNOSTIC(
              self,
              simple_lily_error,
              fun->location,
              NEW(
                LilyError,
                LILY_ERROR_KIND_NO_EXPLICIT_PARAMS_ARE_EXPECTED_IN_MAIN_FUNCTION),
              NULL,
              NULL,
              NULL);
        }

        // Configure the params of the function (if not already configured)
        if (!fun->fun.params) {
            fun->fun.params = check_fun_params__LilyAnalysis(
              self, fun->ast_decl->fun.params, fun->fun.scope);
        }

        // Check if the param is known by the compiler (if is an operator)
        if (fun->fun.is_operator) {
            for (Usize i = 0; i < fun->fun.params->len; ++i) {
                LilyCheckedDeclFunParam *param = get__Vec(fun->fun.params, i);

                if (is_compiler_defined__LilyCheckedDataType(
                      param->data_type)) {
                    ANALYSIS_EMIT_DIAGNOSTIC(
                      self,
                      simple_lily_error,
                      param->data_type->location,
                      NEW(
                        LilyError,
                        LILY_ERROR_KIND_OPERATOR_CANNOT_HAVE_COMPILER_DEFINED_DATA_TYPE_AS_PARAMETER),
                      NULL,
                      NULL,
                      NULL);
                }
            }
        }
    }

    // 4. Check return data type.
    if (fun->ast_decl->fun.return_data_type) {
        // TODO: check the safety mode of the function
        if (!fun->fun.return_data_type) {
            fun->fun.return_data_type =
              check_data_type__LilyAnalysis(self,
                                            fun->ast_decl->fun.return_data_type,
                                            fun->fun.scope,
                                            NULL,
                                            LILY_CHECKED_SAFETY_MODE_SAFE);

            // Check the return data type of the main function.
            if (fun->fun.is_main) {
                // This is a special case, because the main function can only
                // return a Unit or Int32 or CVoid data type.
                switch (fun->fun.return_data_type->kind) {
                    case LILY_CHECKED_DATA_TYPE_KIND_INT32:
                    case LILY_CHECKED_DATA_TYPE_KIND_UNIT:
                    case LILY_CHECKED_DATA_TYPE_KIND_CVOID:
                        break;
                    default:
                        ANALYSIS_EMIT_DIAGNOSTIC(
                          self,
                          simple_lily_error,
                          fun->fun.return_data_type->location,
                          NEW(
                            LilyError,
                            LILY_ERROR_KIND_THIS_RETURN_DATA_TYPE_IS_NOT_EXPECTED_FOR_A_MAIN_FUNCTION),
                          NULL,
                          NULL,
                          from__String("expected Int32, Unit or CVoid"));
                }
            }
        } else {
            // If the return type (checked) is already configured, it means that
            // the function is recursive.
            fun->fun.is_recursive = true;
        }
    } else {
        if (!fun->fun.return_data_type) {
            if (fun->fun.is_operator) {
                ANALYSIS_EMIT_DIAGNOSTIC(
                  self,
                  simple_lily_error,
                  fun->location,
                  NEW(LilyError,
                      LILY_ERROR_KIND_OPERATOR_MUST_HAVE_RETURN_DATA_TYPE),
                  NULL,
                  NULL,
                  NULL);
            }

            fun->fun.return_data_type = NEW(
              LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UNKNOWN, NULL);
        } else {
            // If the return type (checked) is already configured, it means that
            // the function is recursive.
            fun->fun.is_recursive = true;
        }
    }

    // Check if the main function is not recursive.
    if (fun->fun.is_main && fun->fun.is_recursive) {
        ANALYSIS_EMIT_DIAGNOSTIC(
          self,
          simple_lily_error,
          fun->location,
          NEW(LilyError, LILY_ERROR_KIND_MAIN_FUNCTION_CANNOT_BE_RECURSIVE),
          NULL,
          NULL,
          NULL);
    }
}

void
check_fun__LilyAnalysis(LilyAnalysis *self, LilyCheckedDecl *fun)
{
    if (fun->fun.is_checked) {
        return;
    }

    // 1. Check if is an operator.
    if (fun->fun.is_operator) {
        if (!valid_operator__LilyCheckedOperator(fun->fun.name)) {
            ANALYSIS_EMIT_DIAGNOSTIC(
              self,
              simple_lily_error,
              fun->location,
              NEW(LilyError, LILY_ERROR_KIND_OPERATOR_IS_NOT_VALID),
              NULL,
              NULL,
              NULL);
        }
    }

    // 2. Check fun signature
    check_fun_signature__LilyAnalysis(self, fun);

    // 3. Init scope of body.
    fun->fun.scope =
      NEW(LilyCheckedScope,
          NEW_VARIANT(LilyCheckedParent, decl, fun->fun.scope, fun),
          NEW_VARIANT(LilyCheckedScopeDecls, scope, fun->fun.body));

    // 4. Push a new signature.
    {
        HashMap *generic_params = NULL;

        if (fun->fun.generic_params) {
            generic_params = NEW(HashMap);

            for (Usize i = 0; i < fun->fun.generic_params->len; ++i) {
                LilyCheckedGenericParam *generic_param =
                  get__Vec(fun->fun.generic_params, i);

                insert__HashMap(
                  generic_params,
                  get_name__LilyCheckedGenericParam(generic_param)->buffer,
                  NEW_VARIANT(LilyCheckedDataType,
                              custom,
                              generic_param->location,
                              NEW(LilyCheckedDataTypeCustom,
                                  fun->fun.scope->id,
                                  (LilyCheckedAccessScope){ .id = i },
                                  generic_param->constraint.name,
                                  generic_param->constraint.name,
                                  NULL,
                                  LILY_CHECKED_DATA_TYPE_CUSTOM_KIND_GENERIC,
                                  false)));
            }
        }

        Vec *types = NEW(Vec);

        if (fun->fun.params) {
            for (Usize i = 0; i < fun->fun.params->len; ++i) {
                push__Vec(
                  types,
                  CAST(LilyCheckedDeclFunParam *, get__Vec(fun->fun.params, i))
                    ->data_type);
            }
        }

        push__Vec(types, ref__LilyCheckedDataType(fun->fun.return_data_type));

        push__Vec(fun->fun.signatures,
                  NEW(LilyCheckedSignatureFun,
                      fun->fun.global_name,
                      types,
                      generic_params));
    }

    // 5. Check body.
    CHECK_FUN_BODY(fun->ast_decl->fun.body,
                   fun->fun.scope,
                   fun->fun.body,
                   LILY_CHECKED_SAFETY_MODE_SAFE,
                   false);

    // 6. Check return data type.
    if (fun->fun.is_main) {
        switch (fun->fun.return_data_type->kind) {
            case LILY_CHECKED_DATA_TYPE_KIND_INT32:
            case LILY_CHECKED_DATA_TYPE_KIND_UNIT:
            case LILY_CHECKED_DATA_TYPE_KIND_CVOID:
                if (!fun->fun.has_return &&
                    (fun->fun.return_data_type->kind !=
                       LILY_CHECKED_DATA_TYPE_KIND_UNIT &&
                     fun->fun.return_data_type->kind !=
                       LILY_CHECKED_DATA_TYPE_KIND_CVOID)) {
                    FAILED("expected Unit or CVoid for the main function");
                }

                break;
            case LILY_CHECKED_DATA_TYPE_KIND_UNKNOWN:
                if (!fun->fun.has_return) {
                    fun->fun.return_data_type->kind =
                      LILY_CHECKED_DATA_TYPE_KIND_UNIT;
                }

                break;
            default:
                FAILED("expected Int32, Unit or CVoid for the main function")
        }
    } else {
        if (!fun->fun.has_return && fun->fun.default_return_dt_is_set) {
            if (fun->fun.return_data_type->kind !=
                  LILY_CHECKED_DATA_TYPE_KIND_UNIT &&
                fun->fun.return_data_type->kind !=
                  LILY_CHECKED_DATA_TYPE_KIND_CVOID) {
                FAILED("expected a return statement");
            }
        }
    }

    // 7. Reload global name on the first signature
    if (fun->fun.used_compiler_generic->len > 0) {
        reload_global_name__LilyCheckedSignatureFun(
          get__Vec(fun->fun.signatures, 0));
    }

    // 8. Add operator to the operator register.
    if (fun->fun.is_operator) {
        LilyCheckedOperator *operator=
          NEW(LilyCheckedOperator,
              fun->fun.name,
              /* Get signature [params, return_data_type] from fun signatures */
              CAST(LilyCheckedSignatureFun *, get__Vec(fun->fun.signatures, 0))
                ->types);

        if (add_operator__LilyCheckedOperatorRegister(
              &self->package->operator_register, operator)) {
            ANALYSIS_EMIT_DIAGNOSTIC(
              self,
              simple_lily_error,
              fun->location,
              NEW(LilyError, LILY_ERROR_KIND_DUPLICATE_OPERATOR),
              NULL,
              NULL,
              NULL);

            FREE(LilyCheckedOperator, operator);
        }
    }

    // 9. Lock all data types
    lock_data_types__LilyCheckedDeclFun(&fun->fun);

    fun->fun.is_checked = true;
}

void
check_constant__LilyAnalysis(LilyAnalysis *self,
                             LilyCheckedDecl *constant,
                             LilyCheckedScope *scope)
{
    if (constant->constant.is_checked) {
        return;
    }

    constant->constant.data_type =
      check_data_type__LilyAnalysis(self,
                                    constant->ast_decl->constant.data_type,
                                    scope,
                                    NULL,
                                    LILY_CHECKED_SAFETY_MODE_SAFE);

    constant->constant.expr =
      check_expr__LilyAnalysis(self,
                               constant->ast_decl->constant.expr,
                               scope,
                               LILY_CHECKED_SAFETY_MODE_SAFE,
                               false,
                               constant->constant.data_type);

    if (!eq__LilyCheckedDataType(constant->constant.expr->data_type,
                                 constant->constant.data_type)) {
        ANALYSIS_EMIT_DIAGNOSTIC(
          self,
          simple_lily_error,
          constant->location,
          NEW(LilyError, LILY_ERROR_KIND_DATA_TYPE_DONT_MATCH),
          NULL,
          NULL,
          NULL);
    }

    constant->constant.is_checked = true;
}

#define ADD_NEW_TYPE_SIGNATURE(decl)                                          \
    {                                                                         \
        OrderedHashMap *generic_params = NULL;                                \
                                                                              \
        if (decl.generic_params) {                                            \
            generic_params = NEW(OrderedHashMap);                             \
                                                                              \
            for (Usize i = 0; i < decl.generic_params->len; ++i) {            \
                LilyCheckedGenericParam *generic_param =                      \
                  get__Vec(decl.generic_params, i);                           \
                                                                              \
                insert__OrderedHashMap(                                       \
                  generic_params,                                             \
                  get_name__LilyCheckedGenericParam(generic_param)->buffer,   \
                  NEW_VARIANT(LilyCheckedDataType,                            \
                              custom,                                         \
                              generic_param->location,                        \
                              NEW(LilyCheckedDataTypeCustom,                  \
                                  decl.scope->id,                             \
                                  (LilyCheckedAccessScope){ .id = i },        \
                                  generic_param->constraint.name,             \
                                  generic_param->constraint.name,             \
                                  NULL,                                       \
                                  LILY_CHECKED_DATA_TYPE_CUSTOM_KIND_GENERIC, \
                                  false)));                                   \
            }                                                                 \
        }                                                                     \
                                                                              \
        push__Vec(                                                            \
          decl.signatures,                                                    \
          NEW(LilyCheckedSignatureType, decl.global_name, generic_params));   \
    }

void
check_alias__LilyAnalysis(LilyAnalysis *self, LilyCheckedDecl *alias)
{
    if (alias->type.alias.is_checked) {
        return;
    }

    // 1. Check generic params.
    if (alias->ast_decl->type.alias.generic_params) {
        alias->type.alias.generic_params =
          check_generic_params(self,
                               alias->ast_decl->type.alias.generic_params,
                               alias->type.alias.scope);
    }

    // 2. Check aliassed data type.
    alias->type.alias.data_type =
      check_data_type__LilyAnalysis(self,
                                    alias->ast_decl->type.alias.data_type,
                                    alias->type.alias.scope,
                                    NULL,
                                    LILY_CHECKED_SAFETY_MODE_SAFE);

    alias->type.alias.is_checked = true;
}

void
check_record_fields__LilyAnalysis(LilyAnalysis *self,
                                  Vec *ast_fields,
                                  LilyCheckedScope *scope,
                                  LilyCheckedDecl *record)
{
    Vec *check_fields = NEW(Vec);

    for (Usize i = 0; i < ast_fields->len; ++i) {
        LilyAstField *ast_field = get__Vec(ast_fields, i);

        // TODO: in the future add a support for unsafe record
        LilyCheckedDataType *data_type =
          check_data_type__LilyAnalysis(self,
                                        ast_field->data_type,
                                        scope,
                                        record->type.deps,
                                        LILY_CHECKED_SAFETY_MODE_SAFE);
        LilyCheckedExpr *optional_expr = NULL;

        if (ast_field->optional_expr) {
            optional_expr =
              check_expr__LilyAnalysis(self,
                                       ast_field->optional_expr,
                                       scope,
                                       LILY_CHECKED_SAFETY_MODE_SAFE,
                                       false,
                                       data_type);
        }

        LilyCheckedScopeContainerVariable *sc_variable =
          NEW(LilyCheckedScopeContainerVariable, ast_field->name, i);
        int is_failed = add_variable__LilyCheckedScope(scope, sc_variable);

        if (is_failed) {
            ANALYSIS_EMIT_DIAGNOSTIC(
              self,
              simple_lily_error,
              (&ast_field->location),
              NEW(LilyError, LILY_ERROR_KIND_DUPLICATE_FIELD),
              NULL,
              NULL,
              NULL);

            FREE(LilyCheckedScopeContainerVariable, sc_variable);
        }

        push__Vec(check_fields,
                  NEW(LilyCheckedField,
                      ast_field->name,
                      data_type,
                      optional_expr,
                      ast_field->is_mut,
                      &ast_field->location));
    }

    record->type.record.fields = check_fields;
}

bool
check_for_recursive_data_type__LilyAnalysis(LilyAnalysis *self,
                                            LilyCheckedDataType *data_type,
                                            String *global_name)
{
    LilyCheckedDataType *field_custom_dt =
      get_direct_custom_data_type__LilyCheckedDataType(data_type);

    if (field_custom_dt) {
        if (!strcmp(field_custom_dt->custom.global_name->buffer,
                    global_name->buffer)) {
            switch (data_type->kind) {
                case LILY_CHECKED_DATA_TYPE_KIND_ARRAY:
                case LILY_CHECKED_DATA_TYPE_KIND_EXCEPTION:
                case LILY_CHECKED_DATA_TYPE_KIND_LIST:
                case LILY_CHECKED_DATA_TYPE_KIND_OPTIONAL:
                case LILY_CHECKED_DATA_TYPE_KIND_PTR:
                    field_custom_dt->custom.is_recursive = true;

                    return true;
                case LILY_CHECKED_DATA_TYPE_KIND_TRACE:
                    return check_for_recursive_data_type__LilyAnalysis(
                      self, data_type->trace, global_name);
                case LILY_CHECKED_DATA_TYPE_KIND_TUPLE: {
                    bool is_recursive = false;

                    for (Usize i = 0; i < data_type->tuple->len; ++i) {
                        is_recursive =
                          check_for_recursive_data_type__LilyAnalysis(
                            self, get__Vec(data_type->tuple, i), global_name);
                    }

                    return is_recursive;
                }
                default:
                    ANALYSIS_EMIT_DIAGNOSTIC(
                      self,
                      simple_lily_error,
                      data_type->location,
                      NEW(LilyError, LILY_ERROR_KIND_INFINITE_DATA_TYPE),
                      NULL,
                      NULL,
                      NULL);
            }
        }
    }

    return false;
}

void
check_record__LilyAnalysis(LilyAnalysis *self, LilyCheckedDecl *record)
{
    if (record->type.record.is_checked) {
        return;
    }

    if (record->ast_decl->type.record.generic_params) {
        record->type.record.generic_params =
          check_generic_params(self,
                               record->ast_decl->type.record.generic_params,
                               record->type.record.scope);
    }

    ADD_NEW_TYPE_SIGNATURE(record->type.record);
    check_record_fields__LilyAnalysis(self,
                                      record->ast_decl->type.record.fields,
                                      record->type.record.scope,
                                      record);

    // Check if the record is recursive
    for (Usize i = 0; i < record->type.record.fields->len; ++i) {
        LilyCheckedField *field = get__Vec(record->type.record.fields, i);

        bool is_recursive = check_for_recursive_data_type__LilyAnalysis(
          self, field->data_type, record->type.record.global_name);

        if (is_recursive && record->type.record.is_recursive) {
            record->type.record.is_recursive = true;
        }
    }

    record->type.record.is_checked = true;
}

void
check_enum_variants__LilyAnalysis(LilyAnalysis *self,
                                  Vec *ast_variants,
                                  LilyCheckedScope *scope,
                                  LilyCheckedDecl *enum_)
{
    Vec *check_variants = NEW(Vec);

    for (Usize i = 0; i < ast_variants->len; ++i) {
        LilyAstVariant *ast_variant = get__Vec(ast_variants, i);

        LilyCheckedDataType *data_type = NULL;

        if (ast_variant->data_type) {
            data_type =
              check_data_type__LilyAnalysis(self,
                                            ast_variant->data_type,
                                            scope,
                                            enum_->type.deps,
                                            LILY_CHECKED_SAFETY_MODE_SAFE);
        }

        LilyCheckedScopeContainerVariable *sc_variable =
          NEW(LilyCheckedScopeContainerVariable, ast_variant->name, i);
        int is_failed = add_variable__LilyCheckedScope(scope, sc_variable);

        if (is_failed) {
            ANALYSIS_EMIT_DIAGNOSTIC(
              self,
              simple_lily_error,
              (&ast_variant->location),
              NEW(LilyError, LILY_ERROR_KIND_DUPLICATE_VARIANT),
              NULL,
              NULL,
              NULL);

            FREE(LilyCheckedScopeContainerVariable, sc_variable);
        }

        push__Vec(check_variants,
                  NEW(LilyCheckedVariant,
                      ast_variant->name,
                      format__String("{S}.{S}",
                                     scope->decls.decl->type.enum_.global_name,
                                     ast_variant->name),
                      data_type,
                      &ast_variant->location,
                      enum_));
    }

    enum_->type.enum_.variants = check_variants;
}

void
check_enum__LilyAnalysis(LilyAnalysis *self, LilyCheckedDecl *enum_)
{
    if (enum_->type.enum_.is_checked) {
        return;
    }

    if (enum_->ast_decl->type.enum_.generic_params) {
        enum_->type.enum_.generic_params =
          check_generic_params(self,
                               enum_->ast_decl->type.enum_.generic_params,
                               enum_->type.enum_.scope);
    }

    ADD_NEW_TYPE_SIGNATURE(enum_->type.enum_)
    check_enum_variants__LilyAnalysis(self,
                                      enum_->ast_decl->type.enum_.variants,
                                      enum_->type.enum_.scope,
                                      enum_);

    // Push a new signature to variant.
    for (Usize i = 0; i < enum_->type.enum_.variants->len; ++i) {
        LilyCheckedVariant *variant = get__Vec(enum_->type.enum_.variants, i);

        if (variant->data_type) {
            add_signature__LilyCheckedSignatureVariant(
              variant->global_name,
              clone__LilyCheckedDataType(variant->data_type),
              variant->signatures);
        } else {
            add_signature__LilyCheckedSignatureVariant(
              variant->global_name, NULL, variant->signatures);
        }
    }

    // Check if the enum is recursive
    for (Usize i = 0; i < enum_->type.enum_.variants->len; ++i) {
        LilyCheckedVariant *variant = get__Vec(enum_->type.enum_.variants, i);

        if (variant->data_type) {
            bool is_recursive = check_for_recursive_data_type__LilyAnalysis(
              self, variant->data_type, enum_->type.enum_.global_name);

            if (is_recursive && enum_->type.enum_.is_recursive) {
                enum_->type.enum_.is_recursive = true;
            }
        }
    }

    enum_->type.enum_.is_checked = true;
}

void
check_error__LilyAnalysis(LilyAnalysis *self, LilyCheckedDecl *error)
{
    if (error->error.is_checked) {
        return;
    }

    if (error->ast_decl->error.generic_params) {
        error->error.generic_params = check_generic_params(
          self, error->ast_decl->error.generic_params, error->error.scope);
    }

    ADD_NEW_TYPE_SIGNATURE(error->error);

    if (error->ast_decl->error.data_type) {
        error->error.data_type =
          check_data_type__LilyAnalysis(self,
                                        error->ast_decl->error.data_type,
                                        error->error.scope,
                                        error->error.deps,
                                        LILY_CHECKED_SAFETY_MODE_SAFE);

        // Check for recursive data type
        error->error.is_recursive = check_for_recursive_data_type__LilyAnalysis(
          self, error->error.data_type, error->error.global_name);
    }

    error->error.is_checked = true;
}

void
check_decls__LilyAnalysis(LilyAnalysis *self,
                          Vec *decls,
                          LilyCheckedScope *scope)
{
    for (Usize i = 0; i < decls->len; ++i) {
        LilyCheckedDecl *decl = get__Vec(decls, i);
        history = NEW(LilyCheckedHistory);

        switch (decl->kind) {
            case LILY_CHECKED_DECL_KIND_FUN:
                check_fun__LilyAnalysis(self, decl);

                break;
            case LILY_CHECKED_DECL_KIND_CONSTANT:
                check_constant__LilyAnalysis(self, decl, scope);

                break;
            case LILY_CHECKED_DECL_KIND_TYPE:
                switch (decl->type.kind) {
                    case LILY_CHECKED_DECL_TYPE_KIND_ALIAS:
                        check_alias__LilyAnalysis(self, decl);

                        break;
                    case LILY_CHECKED_DECL_TYPE_KIND_RECORD:
                        check_record__LilyAnalysis(self, decl);

                        break;
                    case LILY_CHECKED_DECL_TYPE_KIND_ENUM:
                        check_enum__LilyAnalysis(self, decl);

                        break;
                    default:
                        UNREACHABLE("unknown variant");
                }

                break;
            case LILY_CHECKED_DECL_KIND_ERROR:
                check_error__LilyAnalysis(self, decl);

                break;
            default:
                TODO("analysis declaration");
        }

        FREE(LilyCheckedHistory, &history);
    }
}

static void
run_step0__LilyAnalysis(LilyAnalysis *self)
{
}

void
run_step1__LilyAnalysis(LilyAnalysis *self)
{
    push_all_decls__LilyAnalysis(self, self->parser->decls, &self->module);
}

void
run_step2__LilyAnalysis(LilyAnalysis *self)
{
    check_decls__LilyAnalysis(self, self->module.decls, self->module.scope);
}

void
run__LilyAnalysis(LilyAnalysis *self)
{
    self->module.global_name =
      format__String("{S}.global", self->package->global_name);

    self->module.scope =
      NEW(LilyCheckedScope,
          NULL,
          NEW_VARIANT(LilyCheckedScopeDecls, module, &self->module));

    run_step0__LilyAnalysis(self);
    run_step1__LilyAnalysis(self);
    run_step2__LilyAnalysis(self);

    // TODO: add a support to only build a library.
    if (!self->package->main_is_found &&
        self->package->status == LILY_PACKAGE_STATUS_MAIN) {
        Location location_error =
          NEW(Location, self->package->file.name, 1, 1, 1, 1, 0, 0);

        ANALYSIS_EMIT_DIAGNOSTIC(
          self,
          simple_lily_error,
          (&location_error),
          NEW(LilyError, LILY_ERROR_KIND_EXPECTED_MAIN_FUNCTION),
          NULL,
          NULL,
          NULL);
    }

    if (self->package->count_error > 0) {
        exit(1);
    }

#ifdef DEBUG_ANALYSIS
    printf("====Analysis(%s)====\n", self->package->file.name);

    PRINTLN("{Sr}", to_string__Debug__LilyCheckedScope(self->module.scope));

    for (Usize i = 0; i < self->module.decls->len; ++i) {
        PRINTLN(
          "{Sr}",
          to_string__Debug__LilyCheckedDecl(get__Vec(self->module.decls, i)));
    }
#endif
}

DESTRUCTOR(LilyAnalysis, const LilyAnalysis *self)
{
    FREE(String, self->module.name);
    FREE(LilyCheckedDeclModule, &self->module);
}
