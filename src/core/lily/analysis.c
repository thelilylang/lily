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

#include <core/lily/analysis.h>
#include <core/lily/ast.h>
#include <core/lily/checked/parent.h>
#include <core/lily/checked/safety_mode.h>
#include <core/lily/lily.h>
#include <core/lily/package.h>

#include <core/shared/diagnostic.h>

// FIXME: You should look for the right File structure in the package because of
// the expansion of a macro.

static inline void
push_constant__LilyAnalysis(LilyAnalysis *self,
                            LilyAstDecl *constant,
                            LilyCheckedDeclModule *module);

static inline void
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

static LilyCheckedDataType *
check_data_type__LilyAnalysis(LilyAnalysis *self,
                              LilyAstDataType *data_type,
                              LilyCheckedScope *scope,
                              enum LilyCheckedSafetyMode safety_mode);

static LilyCheckedExpr *
check_identifier_expr__LilyAnalysis(LilyAnalysis *self,
                                    LilyAstExpr *expr,
                                    LilyCheckedScope *scope,
                                    bool is_moved_expr,
                                    bool must_mut);

static LilyCheckedExpr *
check_binary_expr__LilyAnalysis(LilyAnalysis *self,
                                LilyAstExpr *expr,
                                LilyCheckedScope *scope,
                                enum LilyCheckedSafetyMode safety_mode,
                                bool is_moved_expr,
                                LilyCheckedDataType *defined_data_type);

static void
valid_cast__LilyAnalysis(LilyAnalysis *self,
                         LilyCheckedDataType *src,
                         LilyCheckedDataType *dest,
                         enum LilyCheckedSafetyMode safety_mode);

static LilyCheckedScopeResponse
resolve_id__LilyAnalysis(LilyAnalysis *self,
                         LilyAstExpr *id,
                         LilyCheckedScope *scope,
                         enum LilyCheckedScopeResponseKind res_kind);

static bool
valid_function_signature__LilyAnalysis(LilyAnalysis *self,
                                       Vec *params,
                                       Vec *params_call);

static Vec *
check_fun_params_call__LilyAnalysis(LilyAnalysis *self,
                                    Vec *ast_params,
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
                                 enum LilyCheckedSafetyMode safety_mode,
                                 bool is_moved_expr,
                                 bool must_mut);

static LilyCheckedExpr *
check_field_access__LilyAnalysis(LilyAnalysis *self,
                                 LilyAstExpr *path,
                                 LilyCheckedExpr *first,
                                 LilyCheckedScope *scope,
                                 LilyCheckedScope *record_scope,
                                 enum LilyCheckedSafetyMode safety_mode,
                                 bool is_moved_expr,
                                 bool must_mut);

/// @param is_moved_expr If `is_moved_expr` is false it is when the expression
/// that is passed is not surrounded by a reference or a trace. Also this
/// parameter to know if when the variable is called it should be marked as
/// moved.
/// @param set_data_type LilyCheckedDataType*?
static LilyCheckedExpr *
check_expr__LilyAnalysis(LilyAnalysis *self,
                         LilyAstExpr *expr,
                         LilyCheckedScope *scope,
                         enum LilyCheckedSafetyMode safety_mode,
                         bool is_moved_expr,
                         bool must_mut,
                         LilyCheckedDataType *defined_data_type);

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

static Vec *
check_fields__LilyAnalysis(LilyAnalysis *self,
                           Vec *ast_fields,
                           LilyCheckedScope *scope);

static void
check_record__LilyAnalysis(LilyAnalysis *self, LilyCheckedDecl *record);

static Vec *
check_variants__LilyAnalysis(LilyAnalysis *self,
                             Vec *ast_variants,
                             LilyCheckedScope *scope);

static void
check_enum__LilyAnalysis(LilyAnalysis *self, LilyCheckedDecl *enum_);

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
        FREE(Vec, end_body);                                        \
    }

#define EXPECTED_BOOL_EXPR(expr)                                         \
    if (expr->data_type->kind != LILY_CHECKED_DATA_TYPE_KIND_BOOL) {     \
        emit__Diagnostic(                                                \
          NEW_VARIANT(                                                   \
            Diagnostic,                                                  \
            simple_lily_error,                                           \
            &self->package->file,                                        \
            expr->location,                                              \
            NEW(LilyError, LILY_ERROR_KIND_EXPECTED_BOOLEAN_EXPRESSION), \
            NULL,                                                        \
            NULL,                                                        \
            NULL),                                                       \
          &self->package->count_error);                                  \
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
    push__Vec(
      module->decls,
      NEW_VARIANT(
        LilyCheckedDecl,
        error,
        &error->location,
        error,
        NEW(LilyCheckedDeclError,
            error->error.name,
            format__String("{S}.{S}", module->global_name, error->error.name),
            NULL,
            NULL,
            error->error.visibility)));
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
                    emit__Diagnostic(
                      NEW_VARIANT(
                        Diagnostic,
                        simple_lily_error,
                        &self->package->file,
                        &decl->location,
                        NEW(LilyError, LILY_ERROR_KIND_DUPLICATE_CONSTANT),
                        NULL,
                        NULL,
                        NULL),
                      &self->package->count_error);

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
                    emit__Diagnostic(
                      NEW_VARIANT(
                        Diagnostic,
                        simple_lily_error,
                        &self->package->file,
                        &decl->location,
                        NEW(LilyError, LILY_ERROR_KIND_DUPLICATE_ERROR),
                        NULL,
                        NULL,
                        NULL),
                      &self->package->count_error);

                    FREE(LilyCheckedScopeContainerError, sc_error);
                }

                break;
            }
            case LILY_AST_DECL_KIND_FUN: {
                if (!decl->fun.object_impl) {
                    LilyCheckedScopeContainerFun *overload_fun =
                      search_fun_in_current_scope__LilyCheckedScope(
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
                    emit__Diagnostic(
                      NEW_VARIANT(
                        Diagnostic,
                        simple_lily_error,
                        &self->package->file,
                        &decl->location,
                        NEW(LilyError, LILY_ERROR_KIND_DUPLICATE_MODULE),
                        NULL,
                        NULL,
                        NULL),
                      &self->package->count_error);

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
                            emit__Diagnostic(
                              NEW_VARIANT(
                                Diagnostic,
                                simple_lily_error,
                                &self->package->file,
                                &decl->location,
                                NEW(LilyError, LILY_ERROR_KIND_DUPLICATE_CLASS),
                                NULL,
                                NULL,
                                NULL),
                              &self->package->count_error);

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
                            emit__Diagnostic(
                              NEW_VARIANT(
                                Diagnostic,
                                simple_lily_error,
                                &self->package->file,
                                &decl->location,
                                NEW(LilyError,
                                    LILY_ERROR_KIND_DUPLICATE_ENUM_OBJECT),
                                NULL,
                                NULL,
                                NULL),
                              &self->package->count_error);

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
                            emit__Diagnostic(
                              NEW_VARIANT(
                                Diagnostic,
                                simple_lily_error,
                                &self->package->file,
                                &decl->location,
                                NEW(LilyError,
                                    LILY_ERROR_KIND_DUPLICATE_RECORD_OBJECT),
                                NULL,
                                NULL,
                                NULL),
                              &self->package->count_error);

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
                            emit__Diagnostic(
                              NEW_VARIANT(
                                Diagnostic,
                                simple_lily_error,
                                &self->package->file,
                                &decl->location,
                                NEW(LilyError, LILY_ERROR_KIND_DUPLICATE_TRAIT),
                                NULL,
                                NULL,
                                NULL),
                              &self->package->count_error);

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
                            emit__Diagnostic(
                              NEW_VARIANT(
                                Diagnostic,
                                simple_lily_error,
                                &self->package->file,
                                &decl->location,
                                NEW(LilyError, LILY_ERROR_KIND_DUPLICATE_ALIAS),
                                NULL,
                                NULL,
                                NULL),
                              &self->package->count_error);

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
                            emit__Diagnostic(
                              NEW_VARIANT(
                                Diagnostic,
                                simple_lily_error,
                                &self->package->file,
                                &decl->location,
                                NEW(LilyError, LILY_ERROR_KIND_DUPLICATE_ENUM),
                                NULL,
                                NULL,
                                NULL),
                              &self->package->count_error);

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
                            emit__Diagnostic(
                              NEW_VARIANT(Diagnostic,
                                          simple_lily_error,
                                          &self->package->file,
                                          &decl->location,
                                          NEW(LilyError,
                                              LILY_ERROR_KIND_DUPLICATE_RECORD),
                                          NULL,
                                          NULL,
                                          NULL),
                              &self->package->count_error);

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
                              enum LilyCheckedSafetyMode safety_mode)
{
    switch (data_type->kind) {
        case LILY_AST_DATA_TYPE_KIND_ANY:
            if (safety_mode == LILY_CHECKED_SAFETY_MODE_SAFE) {
                emit__Diagnostic(
                  NEW_VARIANT(
                    Diagnostic,
                    simple_lily_error,
                    &self->package->file,
                    &data_type->location,
                    NEW(LilyError, LILY_ERROR_KIND_CANNOT_USE_ANY_IN_SAFE_MODE),
                    NULL,
                    NULL,
                    NULL),
                  &self->package->count_error);
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
                      NEW_VARIANT(
                        LilyCheckedDataTypeArray,
                        sized,
                        check_data_type__LilyAnalysis(
                          self, data_type->array.data_type, scope, safety_mode),
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
            // TODO: add a support for generic custom data type

            switch (custom_dt_response.kind) {
                case LILY_CHECKED_SCOPE_RESPONSE_KIND_NOT_FOUND:
                    FAILED("the custom type is not found");
                case LILY_CHECKED_SCOPE_RESPONSE_KIND_RECORD:
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
                        NULL,
                        LILY_CHECKED_DATA_TYPE_CUSTOM_KIND_RECORD,
                        custom_dt_response.record->is_recursive));
                case LILY_CHECKED_SCOPE_RESPONSE_KIND_RECORD_OBJECT:
                    TODO("check record object data type");
                case LILY_CHECKED_SCOPE_RESPONSE_KIND_ENUM:
                    TODO("check enum data type");
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
                self, data_type->exception, scope, safety_mode));
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
            return NEW_VARIANT(LilyCheckedDataType,
                               list,
                               &data_type->location,
                               check_data_type__LilyAnalysis(
                                 self, data_type->list, scope, safety_mode));
        case LILY_AST_DATA_TYPE_KIND_MUT:
            return NEW_VARIANT(LilyCheckedDataType,
                               mut,
                               &data_type->location,
                               check_data_type__LilyAnalysis(
                                 self, data_type->mut, scope, safety_mode));
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
                self, data_type->optional, scope, safety_mode));
        case LILY_AST_DATA_TYPE_KIND_PTR:
            return NEW_VARIANT(LilyCheckedDataType,
                               ptr,
                               &data_type->location,
                               check_data_type__LilyAnalysis(
                                 self, data_type->ptr, scope, safety_mode));
        case LILY_AST_DATA_TYPE_KIND_REF:
            return NEW_VARIANT(LilyCheckedDataType,
                               ref,
                               &data_type->location,
                               check_data_type__LilyAnalysis(
                                 self, data_type->ref, scope, safety_mode));
        case LILY_AST_DATA_TYPE_KIND_SELF:
            TODO("Check Self data type");
        case LILY_AST_DATA_TYPE_KIND_STR:
            return NEW_VARIANT(
              LilyCheckedDataType, str, &data_type->location, -1);
        case LILY_AST_DATA_TYPE_KIND_TRACE:
            return NEW_VARIANT(LilyCheckedDataType,
                               trace,
                               &data_type->location,
                               check_data_type__LilyAnalysis(
                                 self, data_type->ref, scope, safety_mode));
        case LILY_AST_DATA_TYPE_KIND_TUPLE:
            TODO("check tuple");
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
                                    bool is_moved_expr,
                                    bool must_mut)
{
    LilyCheckedScopeResponse response =
      search_identifier__LilyCheckedScope(scope, expr->identifier.name);

    if (response.kind == LILY_CHECKED_SCOPE_RESPONSE_KIND_NOT_FOUND) {
        emit__Diagnostic(
          NEW_VARIANT(Diagnostic,
                      simple_lily_error,
                      &self->package->file,
                      &expr->location,
                      NEW(LilyError, LILY_ERROR_KIND_IDENTIFIER_NOT_FOUND),
                      NULL,
                      NULL,
                      NULL),
          &self->package->count_error);

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
                if (response.variable->is_moved) {
                    emit__Diagnostic(
                      NEW_VARIANT(
                        Diagnostic,
                        simple_lily_error,
                        &self->package->file,
                        &expr->location,
                        NEW(LilyError, LILY_ERROR_KIND_VALUE_HAS_BEEN_MOVED),
                        NULL,
                        NULL,
                        NULL),
                      &self->package->count_error);
                }

                // Check if the variable is mutable.
                if (must_mut) {
                    if (!response.variable->is_mut) {
                        emit__Diagnostic(
                          NEW_VARIANT(
                            Diagnostic,
                            simple_lily_error,
                            &self->package->file,
                            &expr->location,
                            NEW(LilyError,
                                LILY_ERROR_KIND_EXPECTED_MUTABLE_VARIABLE),
                            NULL,
                            NULL,
                            NULL),
                          &self->package->count_error);
                    }
                }

                if (is_moved_expr) {
                    response.variable->is_moved = true;
                }

                data_type =
                  clone__LilyCheckedDataType(response.variable->data_type);

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
                  clone__LilyCheckedDataType(response.constant->data_type);

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
                if (response.fun_param->is_moved) {
                    emit__Diagnostic(
                      NEW_VARIANT(
                        Diagnostic,
                        simple_lily_error,
                        &self->package->file,
                        &expr->location,
                        NEW(LilyError, LILY_ERROR_KIND_VALUE_HAS_BEEN_MOVED),
                        NULL,
                        NULL,
                        NULL),
                      &self->package->count_error);
                }

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

                if (is_moved_expr) {
                    response.fun_param->is_moved = true;
                }

                data_type =
                  clone__LilyCheckedDataType(response.fun_param->data_type);

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
            default:
                // TODO: emit a diagnostic
                FAILED("this kind of response is not expected in this "
                       "context");
        }
    }
}

LilyCheckedExpr *
check_binary_expr__LilyAnalysis(LilyAnalysis *self,
                                LilyAstExpr *expr,
                                LilyCheckedScope *scope,
                                enum LilyCheckedSafetyMode safety_mode,
                                bool is_moved_expr,
                                LilyCheckedDataType *defined_data_type)
{
    switch (expr->binary.kind) {
        case LILY_AST_EXPR_BINARY_KIND_BIT_AND:
        case LILY_AST_EXPR_BINARY_KIND_BIT_L_SHIFT:
        case LILY_AST_EXPR_BINARY_KIND_BIT_OR:
        case LILY_AST_EXPR_BINARY_KIND_BIT_R_SHIFT: {
            LilyCheckedExpr *left = check_expr__LilyAnalysis(self,
                                                             expr->binary.left,
                                                             scope,
                                                             safety_mode,
                                                             false,
                                                             false,
                                                             defined_data_type);
            LilyCheckedExpr *right =
              check_expr__LilyAnalysis(self,
                                       expr->binary.right,
                                       scope,
                                       safety_mode,
                                       false,
                                       false,
                                       defined_data_type);

            if (is_integer_data_type__LilyCheckedDataType(left->data_type) &&
                is_integer_data_type__LilyCheckedDataType(right->data_type) &&
                eq__LilyCheckedDataType(left->data_type, right->data_type)) {
                return NEW_VARIANT(
                  LilyCheckedExpr,
                  binary,
                  &expr->location,
                  clone__LilyCheckedDataType(left->data_type),
                  expr,
                  NEW(LilyCheckedExprBinary,
                      (enum LilyCheckedExprBinaryKind)(int)expr->binary.kind,
                      left,
                      right));
            } else if (is_float_data_type__LilyCheckedDataType(
                         left->data_type) ||
                       is_float_data_type__LilyCheckedDataType(
                         right->data_type)) {
                FAILED("float data type is not expected");
            } else {
                TODO("check operator defined");
            }
        }
        case LILY_AST_EXPR_BINARY_KIND_GREATER:
        case LILY_AST_EXPR_BINARY_KIND_GREATER_EQ:
        case LILY_AST_EXPR_BINARY_KIND_LESS:
        case LILY_AST_EXPR_BINARY_KIND_LESS_EQ: {
            LilyCheckedExpr *left = check_expr__LilyAnalysis(self,
                                                             expr->binary.left,
                                                             scope,
                                                             safety_mode,
                                                             false,
                                                             false,
                                                             defined_data_type);
            LilyCheckedExpr *right =
              check_expr__LilyAnalysis(self,
                                       expr->binary.right,
                                       scope,
                                       safety_mode,
                                       false,
                                       false,
                                       defined_data_type);

            if (is_integer_data_type__LilyCheckedDataType(left->data_type) &&
                is_integer_data_type__LilyCheckedDataType(right->data_type) &&
                eq__LilyCheckedDataType(left->data_type, right->data_type)) {
                return NEW_VARIANT(
                  LilyCheckedExpr,
                  binary,
                  &expr->location,
                  NEW(LilyCheckedDataType,
                      LILY_CHECKED_DATA_TYPE_KIND_BOOL,
                      &expr->location),
                  expr,
                  NEW(LilyCheckedExprBinary,
                      (enum LilyCheckedExprBinaryKind)(int)expr->binary.kind,
                      left,
                      right));
            } else if (is_float_data_type__LilyCheckedDataType(
                         left->data_type) &&
                       is_float_data_type__LilyCheckedDataType(
                         right->data_type) &&
                       eq__LilyCheckedDataType(left->data_type,
                                               right->data_type)) {
                return NEW_VARIANT(
                  LilyCheckedExpr,
                  binary,
                  &expr->location,
                  NEW(LilyCheckedDataType,
                      LILY_CHECKED_DATA_TYPE_KIND_BOOL,
                      &expr->location),
                  expr,
                  NEW(LilyCheckedExprBinary,
                      (enum LilyCheckedExprBinaryKind)(int)expr->binary.kind,
                      left,
                      right));
            } else {
                TODO("check operator defined");
            }
        }
        case LILY_AST_EXPR_BINARY_KIND_ADD:
        case LILY_AST_EXPR_BINARY_KIND_DIV:
        case LILY_AST_EXPR_BINARY_KIND_MOD:
        case LILY_AST_EXPR_BINARY_KIND_MUL:
        case LILY_AST_EXPR_BINARY_KIND_EXP:
        case LILY_AST_EXPR_BINARY_KIND_SUB: {
            LilyCheckedExpr *left = check_expr__LilyAnalysis(self,
                                                             expr->binary.left,
                                                             scope,
                                                             safety_mode,
                                                             false,
                                                             false,
                                                             defined_data_type);
            LilyCheckedExpr *right =
              check_expr__LilyAnalysis(self,
                                       expr->binary.right,
                                       scope,
                                       safety_mode,
                                       false,
                                       false,
                                       defined_data_type);

            if (is_integer_data_type__LilyCheckedDataType(left->data_type) &&
                is_integer_data_type__LilyCheckedDataType(right->data_type) &&
                eq__LilyCheckedDataType(left->data_type, right->data_type)) {
                return NEW_VARIANT(
                  LilyCheckedExpr,
                  binary,
                  &expr->location,
                  clone__LilyCheckedDataType(left->data_type),
                  expr,
                  NEW(LilyCheckedExprBinary,
                      (enum LilyCheckedExprBinaryKind)(int)expr->binary.kind,
                      left,
                      right));
            } else if (is_float_data_type__LilyCheckedDataType(
                         left->data_type) &&
                       is_float_data_type__LilyCheckedDataType(
                         right->data_type) &&
                       eq__LilyCheckedDataType(left->data_type,
                                               right->data_type)) {
                return NEW_VARIANT(
                  LilyCheckedExpr,
                  binary,
                  &expr->location,
                  clone__LilyCheckedDataType(left->data_type),
                  expr,
                  NEW(LilyCheckedExprBinary,
                      (enum LilyCheckedExprBinaryKind)(int)expr->binary.kind,
                      left,
                      right));
            } else {
                TODO("check operator defined");
            }
        }
        case LILY_AST_EXPR_BINARY_KIND_AND:
        case LILY_AST_EXPR_BINARY_KIND_OR:
        case LILY_AST_EXPR_BINARY_KIND_XOR: {
            LilyCheckedExpr *left = check_expr__LilyAnalysis(self,
                                                             expr->binary.left,
                                                             scope,
                                                             safety_mode,
                                                             false,
                                                             false,
                                                             defined_data_type);
            LilyCheckedExpr *right =
              check_expr__LilyAnalysis(self,
                                       expr->binary.right,
                                       scope,
                                       safety_mode,
                                       false,
                                       false,
                                       defined_data_type);

            if (left->data_type->kind == LILY_CHECKED_DATA_TYPE_KIND_BOOL &&
                right->data_type->kind == LILY_CHECKED_DATA_TYPE_KIND_BOOL) {
                return NEW_VARIANT(
                  LilyCheckedExpr,
                  binary,
                  &expr->location,
                  NEW(LilyCheckedDataType,
                      LILY_CHECKED_DATA_TYPE_KIND_BOOL,
                      &expr->location),
                  expr,
                  NEW(LilyCheckedExprBinary,
                      (enum LilyCheckedExprBinaryKind)(int)expr->binary.kind,
                      left,
                      right));
            } else {
                TODO("check operator defined");
            }
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
            LilyCheckedExpr *left = NULL;

            switch (expr->binary.left->kind) {
                case LILY_AST_EXPR_KIND_IDENTIFIER: {
                    left = check_expr__LilyAnalysis(self,
                                                    expr->binary.left,
                                                    scope,
                                                    safety_mode,
                                                    false,
                                                    true,
                                                    NULL);

                    break;
                }
                case LILY_AST_EXPR_KIND_ACCESS:
                    TODO("resolve access in assign expression");
                default:
                    FAILED("expected identifier or path");
            }

            LilyCheckedExpr *right =
              check_expr__LilyAnalysis(self,
                                       expr->binary.right,
                                       scope,
                                       safety_mode,
                                       true,
                                       false,
                                       left->data_type);

            if (!eq__LilyCheckedDataType(left->data_type, right->data_type)) {
                emit__Diagnostic(
                  NEW_VARIANT(
                    Diagnostic,
                    simple_lily_error,
                    &self->package->file,
                    left->location,
                    NEW(LilyError, LILY_ERROR_KIND_DATA_TYPE_DONT_MATCH),
                    NULL,
                    NULL,
                    NULL),
                  &self->package->count_error);
            }

            return NEW_VARIANT(
              LilyCheckedExpr,
              binary,
              &expr->location,
              NEW(LilyCheckedDataType,
                  LILY_CHECKED_DATA_TYPE_KIND_UNIT,
                  &expr->location),
              expr,
              NEW(LilyCheckedExprBinary,
                  (enum LilyCheckedExprBinaryKind)(int)expr->binary.kind,
                  left,
                  right));
        }
        case LILY_AST_EXPR_BINARY_KIND_CHAIN:
            TODO("analyze |>");
        case LILY_AST_EXPR_BINARY_KIND_EQ:
        case LILY_AST_EXPR_BINARY_KIND_NOT_EQ: {
            LilyCheckedExpr *left = check_expr__LilyAnalysis(
              self, expr->binary.left, scope, safety_mode, false, false, NULL);
            LilyCheckedExpr *right =
              check_expr__LilyAnalysis(self,
                                       expr->binary.right,
                                       scope,
                                       safety_mode,
                                       false,
                                       false,
                                       left->data_type);

            if (!eq__LilyCheckedDataType(left->data_type, right->data_type)) {
                FAILED("expected same data type on left and right expression");
            }

            return NEW_VARIANT(
              LilyCheckedExpr,
              binary,
              &expr->location,
              NEW(LilyCheckedDataType,
                  LILY_CHECKED_DATA_TYPE_KIND_BOOL,
                  &expr->location),
              expr,
              NEW(LilyCheckedExprBinary,
                  expr->binary.kind == LILY_AST_EXPR_BINARY_KIND_EQ
                    ? LILY_CHECKED_EXPR_BINARY_KIND_EQ
                    : LILY_CHECKED_EXPR_BINARY_KIND_NOT_EQ,
                  left,
                  right));
        }
        case LILY_AST_EXPR_BINARY_KIND_LIST_HEAD:
            TODO("analyze ->");
        case LILY_AST_EXPR_BINARY_KIND_LIST_TAIL:
            TODO("analyze <-");
        case LILY_AST_EXPR_BINARY_KIND_RANGE:
            TODO("analyze ..");
        default:
            UNREACHABLE("unknown variant");
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
        emit__Diagnostic(
          NEW_VARIANT(
            Diagnostic,
            simple_lily_error,
            &self->package->file,
            dest->location,
            NEW(LilyError, LILY_ERROR_KIND_CANNOT_CAST_TO_ANY_IN_SAFE_MODE),
            NULL,
            NULL,
            NULL),
          &self->package->count_error);

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
                        emit__Diagnostic(
                          NEW_VARIANT(
                            Diagnostic,
                            simple_lily_error,
                            &self->package->file,
                            dest->location,
                            NEW(LilyError, LILY_ERROR_KIND_BAD_LITERAL_CAST),
                            NULL,
                            NULL,
                            from__String(
                              "you cannot cast to the same data type")),
                          &self->package->count_error);

                        return;
                    }
                    }

                    return;
                default:
                    emit__Diagnostic(
                      NEW_VARIANT(
                        Diagnostic,
                        simple_lily_error,
                        &self->package->file,
                        dest->location,
                        NEW(LilyError, LILY_ERROR_KIND_BAD_LITERAL_CAST),
                        NULL,
                        NULL,
                        NULL),
                      &self->package->count_error);

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
            emit__Diagnostic(
              NEW_VARIANT(Diagnostic,
                          simple_lily_error,
                          &self->package->file,
                          dest->location,
                          NEW(LilyError, LILY_ERROR_KIND_UNKNOWN_CAST),
                          NULL,
                          NULL,
                          NULL),
              &self->package->count_error);

            return;
    }
}

LilyCheckedScopeResponse
resolve_id__LilyAnalysis(LilyAnalysis *self,
                         LilyAstExpr *id,
                         LilyCheckedScope *scope,
                         enum LilyCheckedScopeResponseKind res_kind)
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
                case LILY_CHECKED_SCOPE_RESPONSE_KIND_ALIAS:
                    return search_alias__LilyCheckedScope(scope,
                                                          id->identifier.name);
                case LILY_CHECKED_SCOPE_RESPONSE_KIND_ERROR:
                    return search_error__LilyCheckedScope(scope,
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
                case LILY_AST_EXPR_ACCESS_KIND_GLOBAL:
                    UNREACHABLE("global is not expected in this context");
                default:
                    TODO("...");
            }

            TODO("resolve access");
        }
        default:
            UNREACHABLE("this expression is not an id");
    }
}

bool
valid_function_signature__LilyAnalysis(LilyAnalysis *self,
                                       Vec *params,
                                       Vec *params_call)
{
    if (params) {
        if (params_call->len > params->len) {
            FAILED("error: too many params");
        }

        for (Usize i = 0; i < params->len; ++i) {
            LilyCheckedDeclFunParam *param = get__Vec(params, i);

            if (i < params_call->len) {
                LilyCheckedExprCallFunParam *param_call =
                  get__Vec(params_call, i);

                if (!eq__LilyCheckedDataType(param->data_type,
                                             param_call->value->data_type)) {
                    FAILED("data type of param don't match");

                    return false;
                }
            } else {
                switch (param->kind) {
                    case LILY_CHECKED_DECL_FUN_PARAM_KIND_DEFAULT:
                        break;
                    default:
                        // TODO: emit a diagnostic
                        FAILED("error mismatched the number of params");

                        return false;
                }
            }
        }
    } else if (!params && params_call) {
        FAILED("too many params");
    }

    return true;
}

Vec *
check_fun_params_call__LilyAnalysis(LilyAnalysis *self,
                                    Vec *ast_params,
                                    LilyCheckedDeclFun *called_fun,
                                    LilyCheckedScope *scope,
                                    enum LilyCheckedSafetyMode safety_mode)
{
    Vec *checked_params = ast_params->len > 0 ? NEW(Vec) : NULL;

    for (Usize i = 0; i < ast_params->len; ++i) {
        LilyCheckedDeclFunParam *fun_param = get__Vec(called_fun->params, i);
        LilyAstExprFunParamCall *call_param = get__Vec(ast_params, i);

        LilyCheckedExpr *value = check_expr__LilyAnalysis(self,
                                                          call_param->value,
                                                          scope,
                                                          safety_mode,
                                                          false,
                                                          false,
                                                          fun_param->data_type);

        switch (call_param->kind) {
            case LILY_AST_EXPR_FUN_PARAM_CALL_KIND_DEFAULT:
                break;
            case LILY_AST_EXPR_FUN_PARAM_CALL_KIND_NORMAL:
                // TODO: pass &param->location
                push__Vec(checked_params,
                          NEW_VARIANT(LilyCheckedExprCallFunParam,
                                      normal,
                                      value,
                                      call_param->location));

                break;
            default:
                break;
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
          self, call_param->value, scope, safety_mode, false, false, NULL);

        switch (call_param->kind) {
            case LILY_AST_EXPR_FUN_PARAM_CALL_KIND_DEFAULT:
                FAILED("default param is not expected in sys function");
            case LILY_AST_EXPR_FUN_PARAM_CALL_KIND_NORMAL:
                // TODO: pass &param->location
                push__Vec(checked_params,
                          NEW_VARIANT(LilyCheckedExprCallFunParam,
                                      normal,
                                      value,
                                      call_param->location));

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

        LilyCheckedExpr *value = check_expr__LilyAnalysis(self,
                                                          call_param->value,
                                                          scope,
                                                          safety_mode,
                                                          false,
                                                          false,
                                                          param_data_type);

        if (!eq__LilyCheckedDataType(param_data_type, value->data_type)) {
            FAILED("data type not expected");
        }

        switch (call_param->kind) {
            case LILY_AST_EXPR_FUN_PARAM_CALL_KIND_DEFAULT:
                FAILED("default param is not expected in sys function");
            case LILY_AST_EXPR_FUN_PARAM_CALL_KIND_NORMAL:
                // TODO: pass &param->location
                push__Vec(checked_params,
                          NEW_VARIANT(LilyCheckedExprCallFunParam,
                                      normal,
                                      value,
                                      call_param->location));

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
                                 enum LilyCheckedSafetyMode safety_mode,
                                 bool is_moved_expr,
                                 bool must_mut)
{
    switch (expr->kind) {
        case LILY_AST_EXPR_KIND_ACCESS:
            TODO("resolve access");
        case LILY_AST_EXPR_KIND_IDENTIFIER:
            return check_identifier_expr__LilyAnalysis(
              self, expr, scope, is_moved_expr, must_mut);
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
                                 bool is_moved_expr,
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
                        FAILED("field is not found");
                    case LILY_CHECKED_SCOPE_RESPONSE_KIND_RECORD_FIELD: {
                        LilyCheckedExpr *field = NEW_VARIANT(
                          LilyCheckedExpr,
                          call,
                          &path_item->location,
                          clone__LilyCheckedDataType(
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
                                    FAILED("cannot access to trait in "
                                           "field access");
                                default:
                                    UNREACHABLE(
                                      "expected a custom data type. "
                                      "maybe get_direct_custom_data_type__"
                                      "LilyCheckedDataType have a bug!!");
                            }
                        } else {
                            if (i + 1 != path->access.path->len) {
                                FAILED("expected a custom data type");
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
                FAILED("no expected in this context");
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
                      clone__LilyCheckedDataType(last->data_type),
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

LilyCheckedExpr *
check_expr__LilyAnalysis(LilyAnalysis *self,
                         LilyAstExpr *expr,
                         LilyCheckedScope *scope,
                         enum LilyCheckedSafetyMode safety_mode,
                         bool is_moved_expr,
                         bool must_mut,
                         LilyCheckedDataType *defined_data_type)
{
    switch (expr->kind) {
        case LILY_AST_EXPR_KIND_ACCESS:
            // TODO: maybe remove variant access.
            switch (expr->access.kind) {
                case LILY_AST_EXPR_ACCESS_KIND_GLOBAL:
                    return check_expr__LilyAnalysis(self,
                                                    expr->access.global,
                                                    self->module.scope,
                                                    safety_mode,
                                                    is_moved_expr,
                                                    must_mut,
                                                    NULL);
                case LILY_AST_EXPR_ACCESS_KIND_SELF: {
                    LilyCheckedDecl *object =
                      get_current_object__LilyCheckedScope(scope);

                    if (!object) {
                        FAILED("expected object declaration");
                    }

                    TODO("resolve Self access");
                }
                case LILY_AST_EXPR_ACCESS_KIND_self: {
                    LilyCheckedDecl *method =
                      get_current_method__LilyCheckedScope(scope);

                    if (!method) {
                        FAILED("self is not expected in function");
                    }

                    TODO("resolve self access");
                }
                case LILY_AST_EXPR_ACCESS_KIND_HOOK:
                    TODO("resolve hook access");
                case LILY_AST_EXPR_ACCESS_KIND_OBJECT: {
                    LilyCheckedDecl *object =
                      get_current_object__LilyCheckedScope(scope);

                    if (!object) {
                        FAILED("expected object declaration");
                    }

                    UNREACHABLE("Object is not expected in this context");
                }
                case LILY_AST_EXPR_ACCESS_KIND_PROPERTY_INIT: {
                    LilyCheckedDecl *method =
                      get_current_method__LilyCheckedScope(scope);

                    if (!method) {
                        FAILED("property init is not expected in function");
                    }

                    TODO("resolve property init access");
                }
                case LILY_AST_EXPR_ACCESS_KIND_PATH: {
                    LilyAstExpr *first_ast_expr =
                      get__Vec(expr->access.path, 0);

                    // Analysis the first expression of the path
                    LilyCheckedExpr *first = get_call_from_expr__LilyAnalysis(
                      self, first_ast_expr, scope, safety_mode, false, false);

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
                                          is_moved_expr,
                                          must_mut);
                                    }
                                    case LILY_CHECKED_DATA_TYPE_CUSTOM_KIND_RECORD_OBJECT:
                                        TODO("field access record object!!");
                                    case LILY_CHECKED_DATA_TYPE_CUSTOM_KIND_CLASS:
                                        TODO("attribute access!!");
                                    default:
                                        FAILED("this kind of data type is "
                                               "not expected");
                                }
                            } else {
                                FAILED("this data type is not expected");
                            }
                        }
                        case LILY_CHECKED_EXPR_CALL_KIND_FUN:
                            TODO("fun call!! (path access)");
                        case LILY_CHECKED_EXPR_CALL_KIND_MODULE:
                            TODO("module call!! (path access)");
                        case LILY_CHECKED_EXPR_CALL_KIND_UNKNOWN:
                            FAILED("unknown call");
                        default:
                            // TODO: emit a diagnostic
                            FAILED("no expected in this context or not yet "
                                   "implemented");
                    }

                    TODO("resolve path access");
                }
                default:
                    UNREACHABLE("unknown variant");
            }
        case LILY_AST_EXPR_KIND_ARRAY: {
            Vec *items = NEW(Vec);

            for (Usize i = 0; i < expr->array.items->len; ++i) {
                push__Vec(
                  items,
                  check_expr__LilyAnalysis(self,
                                           get__Vec(expr->array.items, i),
                                           scope,
                                           safety_mode,
                                           false,
                                           false,
                                           NULL));
            }

            LilyCheckedDataType *data_type_item =
              CAST(LilyCheckedExpr *, get__Vec(items, 0))->data_type;

            for (Usize i = 1; i < expr->array.items->len; ++i) {
                if (!eq__LilyCheckedDataType(
                      data_type_item,
                      CAST(LilyCheckedExpr *, get__Vec(items, i))->data_type)) {
                    FAILED("data type does not match to the first item");
                }
            }

            if (defined_data_type) {
                switch (defined_data_type->kind) {
                    case LILY_CHECKED_DATA_TYPE_KIND_ARRAY:
                        if (!eq__LilyCheckedDataType(
                              data_type_item,
                              defined_data_type->array.data_type)) {
                            FAILED("array item data type does not match");
                        }

                        return NEW_VARIANT(
                          LilyCheckedExpr,
                          array,
                          &expr->location,
                          NEW_VARIANT(
                            LilyCheckedDataType,
                            array,
                            &expr->location,
                            NEW(LilyCheckedDataTypeArray,
                                defined_data_type->array.kind,
                                clone__LilyCheckedDataType(
                                  defined_data_type->array.data_type))),
                          expr,
                          NEW(LilyCheckedExprArray, items));
                    default:
                        FAILED("expected array data type");
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
        case LILY_AST_EXPR_KIND_BINARY:
            return check_binary_expr__LilyAnalysis(
              self, expr, scope, safety_mode, is_moved_expr, defined_data_type);
        case LILY_AST_EXPR_KIND_CALL:
            switch (expr->call.kind) {
                case LILY_AST_EXPR_CALL_KIND_FUN: {
                    LilyCheckedScopeResponse response =
                      resolve_id__LilyAnalysis(
                        self,
                        expr->call.fun.id,
                        scope,
                        LILY_CHECKED_SCOPE_RESPONSE_KIND_FUN);

                    switch (response.kind) {
                        case LILY_CHECKED_SCOPE_RESPONSE_KIND_NOT_FOUND:
                            emit__Diagnostic(
                              NEW_VARIANT(
                                Diagnostic,
                                simple_lily_error,
                                &self->package->file,
                                &expr->location,
                                NEW(LilyError,
                                    LILY_ERROR_KIND_FUNCTION_IS_NOT_FOUND),
                                NULL,
                                NULL,
                                from__String("unknown function in this scope")),
                              &self->package->count_error);

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
                                LilyCheckedDecl *fun =
                                  get__Vec(response.fun, 0);

                                if (!fun->fun.is_checked) {
                                    check_fun_signature__LilyAnalysis(self,
                                                                      fun);
                                }

                                if (fun->fun.is_main) {
                                    FAILED("you cannot call the main function");
                                }

                                Vec *checked_params =
                                  check_fun_params_call__LilyAnalysis(
                                    self,
                                    expr->call.fun.params,
                                    &fun->fun,
                                    scope,
                                    safety_mode);

                                if (!valid_function_signature__LilyAnalysis(
                                      self, fun->fun.params, checked_params)) {
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
                                    LilyCheckedExpr *fun_call = NEW_VARIANT(
                                      LilyCheckedExpr,
                                      call,
                                      &expr->location,
                                      clone__LilyCheckedDataType(
                                        fun->fun.return_data_type),
                                      expr,
                                      NEW_VARIANT(
                                        LilyCheckedExprCall,
                                        fun,
                                        (LilyCheckedAccessScope){
                                          .id =
                                            response.scope_container.scope_id },
                                        fun->fun.global_name,
                                        NEW(LilyCheckedExprCallFun,
                                            checked_params)));

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
                        emit__Diagnostic(
                          NEW_VARIANT(
                            Diagnostic,
                            simple_lily_error,
                            &self->package->file,
                            &expr->location,
                            NEW(LilyError,
                                LILY_ERROR_KIND_IMPORT_BUILTIN_REQUIRED),
                            NULL,
                            NULL,
                            from__String("please import `@builtin`")),
                          &self->package->count_error);

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

                    if (!is_builtin_function__LilyBuiltin(
                          expr->call.fun_builtin.name->buffer)) {
                    bad_builtin_function : {
                        emit__Diagnostic(
                          NEW_VARIANT(Diagnostic,
                                      simple_lily_error,
                                      &self->package->file,
                                      &expr->location,
                                      NEW(LilyError,
                                          LILY_ERROR_KIND_BAD_BUILTIN_FUNCTION),
                                      NULL,
                                      NULL,
                                      from__String("unknown builtin function")),
                          &self->package->count_error);

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

                    Vec *check_params_call =
                      check_builtin_fun_params_call__LilyAnalysis(
                        self,
                        expr->call.fun_builtin.params,
                        scope,
                        safety_mode); // Vec<LilyCheckedExprCallFunParam*>*?
                    Vec *check_params_call_dt =
                      NEW(Vec); // Vec<LilyCheckedDataType*>*

                    if (check_params_call) {
                        for (Usize i = 0; i < check_params_call->len; ++i) {
                            push__Vec(check_params_call_dt,
                                      CAST(LilyCheckedExprCallFunParam *,
                                           get__Vec(check_params_call, i))
                                        ->value->data_type);
                        }
                    }

                    const LilyBuiltinFun *builtin_signature =
                      get_builtin__LilyBuiltin(
                        self->root_package->builtins,
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
                      clone__LilyCheckedDataType(
                        builtin_signature->return_data_type),
                      expr,
                      NEW_VARIANT(LilyCheckedExprCall,
                                  fun_builtin,
                                  NEW(LilyCheckedExprCallFunBuiltin,
                                      check_params_call,
                                      builtin_signature)));
                }
                case LILY_AST_EXPR_CALL_KIND_FUN_SYS: {
                    if (!self->package->sys_is_loaded) {
                        emit__Diagnostic(
                          NEW_VARIANT(
                            Diagnostic,
                            simple_lily_error,
                            &self->package->file,
                            &expr->location,
                            NEW(LilyError, LILY_ERROR_KIND_IMPORT_SYS_REQUIRED),
                            NULL,
                            NULL,
                            from__String("please import `@sys`")),
                          &self->package->count_error);

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

                    if (!is_sys_function__LilySys(
                          expr->call.fun_sys.name->buffer)) {
                        emit__Diagnostic(
                          NEW_VARIANT(
                            Diagnostic,
                            simple_lily_error,
                            &self->package->file,
                            &expr->location,
                            NEW(LilyError, LILY_ERROR_KIND_BAD_SYS_FUNCTION),
                            NULL,
                            NULL,
                            from__String("unknown sys function")),
                          &self->package->count_error);

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

                    const LilySysFun *sys_signature =
                      get_sys__LilySys(self->root_package->syss,
                                       expr->call.fun_sys.name->buffer);

                    Vec *check_params_call =
                      check_sys_fun_params_call__LilyAnalysis(
                        self,
                        expr->call.fun_sys.params,
                        sys_signature->params,
                        scope,
                        safety_mode); // Vec<LilyCheckedExprCallFunParam*>*?

                    // Add sys to sys_signature to know which prototype to
                    // define for IR.
                    add_sys_fun_to_sys_usage__LilyPackage(
                      self->package, (LilySysFun *)sys_signature);

                    return NEW_VARIANT(
                      LilyCheckedExpr,
                      call,
                      &expr->location,
                      clone__LilyCheckedDataType(
                        sys_signature->return_data_type),
                      expr,
                      NEW_VARIANT(LilyCheckedExprCall,
                                  fun_sys,
                                  NEW(LilyCheckedExprCallFunSys,
                                      check_params_call,
                                      sys_signature)));
                }
                case LILY_AST_EXPR_CALL_KIND_LEN: {
                    LilyCheckedExpr *len_expr =
                      check_expr__LilyAnalysis(self,
                                               expr->call.len,
                                               scope,
                                               safety_mode,
                                               false,
                                               false,
                                               NULL);

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
                                      NEW_VARIANT(
                                        LilyCheckedExprLiteral,
                                        int64,
                                        strlen(len_expr->literal.cstr)));

                                    FREE(LilyCheckedExpr, len_expr);

                                    return res;
                                }
                                default:
                                    TODO(
                                      "do for the rest of compatible literal");
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
                              NEW_VARIANT(
                                LilyCheckedExprCall, str_len, len_expr));
                        case LILY_CHECKED_DATA_TYPE_KIND_CSTR: {
                            LilyBuiltinFun *len_cstr_fun =
                              &self->root_package->builtins[__len__$CStr];

                            add_builtin_fun_to_builtin_usage__LilyPackage(
                              self->package, len_cstr_fun);

                            return NEW_VARIANT(
                              LilyCheckedExpr,
                              call,
                              &expr->location,
                              NEW(LilyCheckedDataType,
                                  LILY_CHECKED_DATA_TYPE_KIND_USIZE,
                                  &expr->location),
                              expr,
                              NEW_VARIANT(
                                LilyCheckedExprCall, cstr_len, len_expr));
                        }
                        default:
                            FAILED("expected Str, CStr, Bytes, array or list "
                                   "data type");
                    }
                }
                case LILY_AST_EXPR_CALL_KIND_RECORD: {
                    LilyCheckedScopeResponse response =
                      resolve_id__LilyAnalysis(
                        self,
                        expr->call.record.id,
                        scope,
                        LILY_CHECKED_SCOPE_RESPONSE_KIND_RECORD);

                    switch (response.kind) {
                        case LILY_CHECKED_SCOPE_RESPONSE_KIND_NOT_FOUND:
                            emit__Diagnostic(
                              NEW_VARIANT(
                                Diagnostic,
                                simple_lily_error,
                                &self->package->file,
                                &expr->location,
                                NEW(LilyError, LILY_ERROR_KIND_UNKNOWN_TYPE),
                                NULL,
                                NULL,
                                from__String("unknown record in this scope")),
                              &self->package->count_error);

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
                            if (!response.record->is_checked) {
                                check_record__LilyAnalysis(
                                  self, response.record->scope->decls.decl);
                            }

                            Vec *checked_record_params = NEW(Vec);

                            for (Usize i = 0; i < expr->call.record.params->len;
                                 ++i) {
                                LilyAstExprRecordParamCall *param =
                                  get__Vec(expr->call.record.params, i);

                                LilyCheckedScopeResponse field_response =
                                  search_field__LilyCheckedScope(
                                    response.record->scope, param->name);

                                Usize field_index = 0;

                                switch (field_response.kind) {
                                    case LILY_CHECKED_SCOPE_RESPONSE_KIND_NOT_FOUND:
                                        emit__Diagnostic(
                                          NEW_VARIANT(
                                            Diagnostic,
                                            simple_lily_error,
                                            &self->package->file,
                                            &expr->location,
                                            NEW(
                                              LilyError,
                                              LILY_ERROR_KIND_FIELD_IS_NOT_FOUND),
                                            NULL,
                                            NULL,
                                            from__String(
                                              "unknown record in this scope")),
                                          &self->package->count_error);

                                        break;
                                    default:
                                        field_index =
                                          field_response.scope_container
                                            .variable->id;

                                        break;
                                }

                                LilyCheckedExpr *checked_value =
                                  check_expr__LilyAnalysis(
                                    self,
                                    param->value,
                                    scope,
                                    safety_mode,
                                    false,
                                    must_mut,
                                    field_response.record_field->data_type);

                                if (!eq__LilyCheckedDataType(
                                      checked_value->data_type,
                                      field_response.record_field->data_type)) {
                                    FAILED("data type do not match");
                                }

                                push__Vec(checked_record_params,
                                          NEW(LilyCheckedExprCallRecordParam,
                                              param->name,
                                              checked_value,
                                              field_index));
                            }

                            // TODO: add support for call generic record.
                            return NEW_VARIANT(
                              LilyCheckedExpr,
                              call,
                              &expr->location,
                              NEW_VARIANT(
                                LilyCheckedDataType,
                                custom,
                                &expr->location,
                                NEW(
                                  LilyCheckedDataTypeCustom,
                                  response.scope_container.scope_id,
                                  (LilyCheckedAccessScope){
                                    .id = response.scope_container.record->id },
                                  response.record->name,
                                  response.record->global_name,
                                  NULL,
                                  LILY_CHECKED_DATA_TYPE_CUSTOM_KIND_RECORD,
                                  response.record->is_recursive)),
                              expr,
                              NEW_VARIANT(
                                LilyCheckedExprCall,
                                record,
                                (LilyCheckedAccessScope){
                                  .id = response.scope_container.scope_id },
                                response.record->global_name,
                                NEW(LilyCheckedExprCallRecord,
                                    checked_record_params)));
                        }
                    }
                }
                case LILY_AST_EXPR_CALL_KIND_VARIANT: {
                    LilyCheckedScopeResponse response =
                      resolve_id__LilyAnalysis(
                        self,
                        expr->call.variant.id,
                        scope,
                        LILY_CHECKED_SCOPE_RESPONSE_KIND_ENUM_VARIANT);

                    switch (response.kind) {
                        case LILY_CHECKED_SCOPE_RESPONSE_KIND_NOT_FOUND:
                            emit__Diagnostic(
                              NEW_VARIANT(
                                Diagnostic,
                                simple_lily_error,
                                &self->package->file,
                                &expr->location,
                                NEW(LilyError, LILY_ERROR_KIND_UNKNOWN_TYPE),
                                NULL,
                                NULL,
                                from__String("unknown enum in this scope")),
                              &self->package->count_error);

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
                            if (!response.enum_->is_checked) {
                                TODO("check enum");
                            }
                        }
                    }

                    TODO("check variant call");
                }
                default:
                    UNREACHABLE("unknown variant");
            }
        case LILY_AST_EXPR_KIND_CAST: {
            LilyCheckedExpr *left = check_expr__LilyAnalysis(self,
                                                             expr->cast.expr,
                                                             scope,
                                                             safety_mode,
                                                             is_moved_expr,
                                                             must_mut,
                                                             NULL);
            LilyCheckedDataType *dest = check_data_type__LilyAnalysis(
              self, expr->cast.dest_data_type, scope, safety_mode);
            enum LilyCheckedExprCastKind kind;

            valid_cast__LilyAnalysis(self, left->data_type, dest, safety_mode);

            if (is_string_data_type__LilyCheckedDataType(left->data_type)) {
                kind = LILY_CHECKED_EXPR_CAST_KIND_STRING;
            } else if (is_literal_data_type__LilyCheckedDataType(
                         left->data_type)) {
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
        case LILY_AST_EXPR_KIND_GROUPING: {
            LilyCheckedExpr *grouping = check_expr__LilyAnalysis(self,
                                                                 expr->grouping,
                                                                 scope,
                                                                 safety_mode,
                                                                 is_moved_expr,
                                                                 must_mut,
                                                                 NULL);

            return NEW_VARIANT(LilyCheckedExpr,
                               grouping,
                               &expr->location,
                               clone__LilyCheckedDataType(grouping->data_type),
                               expr,
                               grouping);
        }
        case LILY_AST_EXPR_KIND_IDENTIFIER:
            return check_identifier_expr__LilyAnalysis(
              self, expr, scope, is_moved_expr, must_mut);
        case LILY_AST_EXPR_KIND_IDENTIFIER_DOLLAR:
            TODO("identifier dollar expression");
        case LILY_AST_EXPR_KIND_LAMBDA:
            TODO("lambda expression");
        case LILY_AST_EXPR_KIND_LIST: {
            LilyCheckedDataType *defined_list_data_type =
              defined_data_type ? defined_data_type->list : NULL;
            Vec *list = NEW(Vec);

            for (Usize i = 0; i < expr->list.items->len; ++i) {
                push__Vec(
                  list,
                  check_expr__LilyAnalysis(self,
                                           get__Vec(expr->list.items, i),
                                           scope,
                                           safety_mode,
                                           is_moved_expr,
                                           must_mut,
                                           defined_list_data_type));
            }

            TODO("list expression");
        }
        case LILY_AST_EXPR_KIND_LITERAL:
            switch (expr->literal.kind) {
                case LILY_AST_EXPR_LITERAL_KIND_BOOL:
                    return NEW_VARIANT(LilyCheckedExpr,
                                       literal,
                                       &expr->location,
                                       NEW(LilyCheckedDataType,
                                           LILY_CHECKED_DATA_TYPE_KIND_BOOL,
                                           &expr->location),
                                       expr,
                                       NEW_VARIANT(LilyCheckedExprLiteral,
                                                   bool_,
                                                   expr->literal.bool_));
                case LILY_AST_EXPR_LITERAL_KIND_BYTE:
                    return NEW_VARIANT(LilyCheckedExpr,
                                       literal,
                                       &expr->location,
                                       NEW(LilyCheckedDataType,
                                           LILY_CHECKED_DATA_TYPE_KIND_BYTE,
                                           &expr->location),
                                       expr,
                                       NEW_VARIANT(LilyCheckedExprLiteral,
                                                   byte,
                                                   expr->literal.byte));
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
                      NEW_VARIANT(
                        LilyCheckedExprLiteral, bytes, expr->literal.bytes));
                case LILY_AST_EXPR_LITERAL_KIND_CHAR:
                    return NEW_VARIANT(LilyCheckedExpr,
                                       literal,
                                       &expr->location,
                                       NEW(LilyCheckedDataType,
                                           LILY_CHECKED_DATA_TYPE_KIND_CHAR,
                                           &expr->location),
                                       expr,
                                       NEW_VARIANT(LilyCheckedExprLiteral,
                                                   char,
                                                   expr->literal.char_));
                case LILY_AST_EXPR_LITERAL_KIND_CSTR:
                    return NEW_VARIANT(LilyCheckedExpr,
                                       literal,
                                       &expr->location,
                                       NEW(LilyCheckedDataType,
                                           LILY_CHECKED_DATA_TYPE_KIND_CSTR,
                                           &expr->location),
                                       expr,
                                       NEW_VARIANT(LilyCheckedExprLiteral,
                                                   cstr,
                                                   expr->literal.cstr));
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
                        literal_data_type =
                          NEW(LilyCheckedDataType,
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
                                    FAILED("comptime cast overflow");
                                } else {
                                    literal_data_type =
                                      clone__LilyCheckedDataType(
                                        defined_data_type);
                                }

                                break;
                            case LILY_CHECKED_DATA_TYPE_KIND_INT16:
                            case LILY_CHECKED_DATA_TYPE_KIND_CSHORT:
                                if (expr->literal.int32 > INT16_MAX ||
                                    expr->literal.int32 < INT16_MIN) {
                                    FAILED("comptime cast overflow");
                                } else {
                                    literal_data_type =
                                      clone__LilyCheckedDataType(
                                        defined_data_type);
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
                                    FAILED("comptime cast overflow");
                                } else {
                                    literal_data_type =
                                      clone__LilyCheckedDataType(
                                        defined_data_type);
                                }

                                break;
                            case LILY_CHECKED_DATA_TYPE_KIND_UINT16:
                            case LILY_CHECKED_DATA_TYPE_KIND_CUSHORT:
                                if (expr->literal.int32 > UINT16_MAX ||
                                    expr->literal.int32 < 0) {
                                    FAILED("comptime cast overflow");
                                } else {
                                    literal_data_type =
                                      clone__LilyCheckedDataType(
                                        defined_data_type);
                                }

                                break;
                            case LILY_CHECKED_DATA_TYPE_KIND_UINT32:
                            case LILY_CHECKED_DATA_TYPE_KIND_CUINT:
                            case LILY_CHECKED_DATA_TYPE_KIND_CULONG:
                                if (expr->literal.int32 < 0) {
                                    FAILED("comptime cast overflow");
                                } else {
                                    literal_data_type =
                                      clone__LilyCheckedDataType(
                                        defined_data_type);
                                }

                                break;
                            case LILY_CHECKED_DATA_TYPE_KIND_UINT64:
                            case LILY_CHECKED_DATA_TYPE_KIND_USIZE:
                            case LILY_CHECKED_DATA_TYPE_KIND_CULONGLONG:
                                if (expr->literal.int32 < 0) {
                                    FAILED("comptime cast overflow");
                                } else {
                                    literal_data_type =
                                      clone__LilyCheckedDataType(
                                        defined_data_type);
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
                        literal_data_type =
                          NEW(LilyCheckedDataType,
                              LILY_CHECKED_DATA_TYPE_KIND_INT32,
                              &expr->location);
                    }

                    return NEW_VARIANT(LilyCheckedExpr,
                                       literal,
                                       &expr->location,
                                       literal_data_type,
                                       expr,
                                       NEW_VARIANT(LilyCheckedExprLiteral,
                                                   int32,
                                                   expr->literal.int32));
                }
                case LILY_AST_EXPR_LITERAL_KIND_INT64: {
                    LilyCheckedDataType *literal_data_type = NULL;

                    if (defined_data_type) {
                        switch (defined_data_type->kind) {
                            case LILY_CHECKED_DATA_TYPE_KIND_INT8:
                                if (expr->literal.int64 > INT8_MAX ||
                                    expr->literal.int64 < INT8_MIN) {
                                    FAILED("comptime cast overflow");
                                } else {
                                    literal_data_type =
                                      clone__LilyCheckedDataType(
                                        defined_data_type);
                                }

                                break;
                            case LILY_CHECKED_DATA_TYPE_KIND_INT16:
                            case LILY_CHECKED_DATA_TYPE_KIND_CSHORT:
                                if (expr->literal.int64 > INT16_MAX ||
                                    expr->literal.int64 < INT16_MIN) {
                                    FAILED("comptime cast overflow");
                                } else {
                                    literal_data_type =
                                      clone__LilyCheckedDataType(
                                        defined_data_type);
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
                                    FAILED("comptime cast overflow");
                                } else {
                                    literal_data_type =
                                      clone__LilyCheckedDataType(
                                        defined_data_type);
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
                                    FAILED("comptime cast overflow");
                                } else {
                                    literal_data_type =
                                      clone__LilyCheckedDataType(
                                        defined_data_type);
                                }

                                break;
                            case LILY_CHECKED_DATA_TYPE_KIND_UINT16:
                            case LILY_CHECKED_DATA_TYPE_KIND_CUSHORT:
                                if (expr->literal.int64 > UINT16_MAX ||
                                    expr->literal.int64 < 0) {
                                    FAILED("comptime cast overflow");
                                } else {
                                    literal_data_type =
                                      clone__LilyCheckedDataType(
                                        defined_data_type);
                                }

                                break;
                            case LILY_CHECKED_DATA_TYPE_KIND_UINT32:
                            case LILY_CHECKED_DATA_TYPE_KIND_CUINT:
                            case LILY_CHECKED_DATA_TYPE_KIND_CULONG:
                                if (expr->literal.int64 < 0) {
                                    FAILED("comptime cast overflow");
                                } else {
                                    literal_data_type =
                                      clone__LilyCheckedDataType(
                                        defined_data_type);
                                }

                                break;
                            case LILY_CHECKED_DATA_TYPE_KIND_UINT64:
                            case LILY_CHECKED_DATA_TYPE_KIND_USIZE:
                            case LILY_CHECKED_DATA_TYPE_KIND_CULONGLONG:
                                if (expr->literal.int64 < 0) {
                                    FAILED("comptime cast overflow");
                                } else {
                                    literal_data_type =
                                      clone__LilyCheckedDataType(
                                        defined_data_type);
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
                        literal_data_type =
                          NEW(LilyCheckedDataType,
                              LILY_CHECKED_DATA_TYPE_KIND_INT64,
                              &expr->location);
                    }

                    return NEW_VARIANT(LilyCheckedExpr,
                                       literal,
                                       &expr->location,
                                       literal_data_type,
                                       expr,
                                       NEW_VARIANT(LilyCheckedExprLiteral,
                                                   int64,
                                                   expr->literal.int64));
                }
                case LILY_AST_EXPR_LITERAL_KIND_NIL:
                    if (defined_data_type) {
                        switch (defined_data_type->kind) {
                            case LILY_CHECKED_DATA_TYPE_KIND_PTR:
                                return NEW_VARIANT(
                                  LilyCheckedExpr,
                                  literal,
                                  &expr->location,
                                  NEW_VARIANT(LilyCheckedDataType,
                                              ptr,
                                              &expr->location,
                                              clone__LilyCheckedDataType(
                                                defined_data_type->ptr)),
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
                      NEW(LilyCheckedExprLiteral,
                          LILY_CHECKED_EXPR_LITERAL_KIND_NONE));
                case LILY_AST_EXPR_LITERAL_KIND_STR:
                    return NEW_VARIANT(LilyCheckedExpr,
                                       literal,
                                       &expr->location,
                                       NEW_VARIANT(LilyCheckedDataType,
                                                   str,
                                                   &expr->location,
                                                   expr->literal.str->len),
                                       expr,
                                       NEW_VARIANT(LilyCheckedExprLiteral,
                                                   str,
                                                   expr->literal.str));
                case LILY_AST_EXPR_LITERAL_KIND_SUFFIX_FLOAT32:
                    return NEW_VARIANT(
                      LilyCheckedExpr,
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
                    return NEW_VARIANT(
                      LilyCheckedExpr,
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
                    return NEW_VARIANT(
                      LilyCheckedExpr,
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
                    return NEW_VARIANT(
                      LilyCheckedExpr,
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
                    return NEW_VARIANT(
                      LilyCheckedExpr,
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
                    return NEW_VARIANT(
                      LilyCheckedExpr,
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
                    return NEW_VARIANT(
                      LilyCheckedExpr,
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
                    return NEW_VARIANT(
                      LilyCheckedExpr,
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
                      NEW(LilyCheckedExprLiteral,
                          LILY_CHECKED_EXPR_LITERAL_KIND_UNIT));
                default:
                    UNREACHABLE("unknown variant");
            }
        case LILY_AST_EXPR_KIND_SELF:
            TODO("self expression");
        case LILY_AST_EXPR_KIND_TRY:
            TODO("try expression");
        case LILY_AST_EXPR_KIND_TUPLE:
            TODO("tuple expression");
        case LILY_AST_EXPR_KIND_UNARY: {
            LilyCheckedExpr *right = check_expr__LilyAnalysis(self,
                                                              expr->unary.right,
                                                              scope,
                                                              safety_mode,
                                                              is_moved_expr,
                                                              false,
                                                              NULL);

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
                    if (is_integer_data_type__LilyCheckedDataType(
                          right->data_type) ||
                        is_float_data_type__LilyCheckedDataType(
                          right->data_type)) {
                        return NEW_VARIANT(
                          LilyCheckedExpr,
                          unary,
                          &expr->location,
                          clone__LilyCheckedDataType(right->data_type),
                          expr,
                          NEW(LilyCheckedExprUnary,
                              LILY_CHECKED_EXPR_UNARY_KIND_NEG,
                              right));
                    } else {
                        TODO(
                          "check if `-` operator is implemented for this type");
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
                                  NEW_VARIANT(LilyCheckedDataType,
                                              ptr,
                                              &expr->location,
                                              clone__LilyCheckedDataType(
                                                right->data_type)),
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
                                  NEW_VARIANT(LilyCheckedDataType,
                                              ref,
                                              &expr->location,
                                              clone__LilyCheckedDataType(
                                                right->data_type)),
                                  expr,
                                  NEW(LilyCheckedExprUnary,
                                      LILY_CHECKED_EXPR_UNARY_KIND_REF,
                                      right));
                            }
                            default:
                                FAILED("expected ref or ptr data type (ref is "
                                       "not overloadable)");
                        }
                    } else {
                        goto get_ref;
                    }
                }
                default:
                    TODO("unary expression");
            }
        }
        case LILY_AST_EXPR_KIND_WILDCARD:
            TODO("wildcard expression");
        default:
            UNREACHABLE("unknown variant");
    }
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
            TODO("analysis asm stmt");
        case LILY_AST_STMT_KIND_AWAIT:
            TODO("analysis await stmt");
        case LILY_AST_STMT_KIND_BLOCK: {
            Vec *body = NEW(Vec);
            LilyCheckedScope *scope_block =
              NEW(LilyCheckedScope,
                  NEW_VARIANT(LilyCheckedParent, scope, scope, current_body),
                  NEW_VARIANT(LilyCheckedScopeDecls, scope, body));

            CHECK_FUN_BODY(
              stmt->block.body, scope_block, body, safety_mode, false);

            return NEW_VARIANT(
              LilyCheckedBodyFunItem,
              stmt,
              NEW_VARIANT(LilyCheckedStmt,
                          block,
                          &stmt->location,
                          stmt,
                          NEW(LilyCheckedStmtBlock, body, scope_block)));
        }
        case LILY_AST_STMT_KIND_BREAK:
            if (!in_loop) {
                emit__Diagnostic(
                  NEW_VARIANT(
                    Diagnostic,
                    simple_lily_error,
                    &self->package->file,
                    &stmt->location,
                    NEW(LilyError,
                        LILY_ERROR_KIND_BREAK_IS_NOT_EXPECTED_IN_THIS_CONTEXT),
                    NULL,
                    NULL,
                    NULL),
                  &self->package->count_error);
            }

            return NEW_VARIANT(
              LilyCheckedBodyFunItem,
              stmt,
              NEW_VARIANT(LilyCheckedStmt,
                          break,
                          &stmt->location,
                          stmt,
                          NEW(LilyCheckedStmtBreak, stmt->break_.name)));
        case LILY_AST_STMT_KIND_DEFER: {
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
        case LILY_AST_STMT_KIND_DROP:
            TODO("analysis drop stmt");
        case LILY_AST_STMT_KIND_FOR:
            TODO("analysis for stmt");
        case LILY_AST_STMT_KIND_IF: {
            LilyCheckedExpr *if_cond = check_expr__LilyAnalysis(
              self, stmt->if_.if_expr, scope, safety_mode, true, false, NULL);

            EXPECTED_BOOL_EXPR(if_cond);

            Vec *if_body = NEW(Vec);
            LilyCheckedScope *if_scope =
              NEW(LilyCheckedScope,
                  NEW_VARIANT(LilyCheckedParent, scope, scope, current_body),
                  NEW_VARIANT(LilyCheckedScopeDecls, scope, if_body));

            CHECK_FUN_BODY(
              stmt->if_.if_body, if_scope, if_body, safety_mode, false);

            Vec *elifs = NULL; // Vec<LilyCheckedStmtIfBranch*>*?

            if (stmt->if_.elif_exprs) {
                elifs = NEW(Vec);

                for (Usize k = 0; k < stmt->if_.elif_exprs->len; ++k) {
                    LilyCheckedExpr *elif_cond = check_expr__LilyAnalysis(
                      self,
                      get__Vec(stmt->if_.elif_exprs, k),
                      scope,
                      safety_mode,
                      true,
                      false,
                      NULL);

                    EXPECTED_BOOL_EXPR(elif_cond);

                    Vec *elif_body = NEW(Vec);
                    LilyCheckedScope *elif_scope =
                      NEW(LilyCheckedScope,
                          NEW_VARIANT(
                            LilyCheckedParent, scope, scope, current_body),
                          NEW_VARIANT(LilyCheckedScopeDecls, scope, elif_body));
                    Vec *ast_elif_body = get__Vec(stmt->if_.elif_bodies, k);

                    CHECK_FUN_BODY(
                      ast_elif_body, elif_scope, elif_body, safety_mode, false);

                    push__Vec(elifs,
                              NEW(LilyCheckedStmtIfBranch,
                                  elif_cond,
                                  elif_body,
                                  elif_scope));
                }
            }

            LilyCheckedStmtElseBranch *else_ = NULL;

            if (stmt->if_.else_body) {
                Vec *else_body = NEW(Vec);
                LilyCheckedScope *else_scope = NEW(
                  LilyCheckedScope,
                  NEW_VARIANT(LilyCheckedParent, scope, scope, current_body),
                  NEW_VARIANT(LilyCheckedScopeDecls, scope, else_body));

                CHECK_FUN_BODY(stmt->if_.else_body,
                               else_scope,
                               else_body,
                               safety_mode,
                               false);

                else_ = NEW(LilyCheckedStmtElseBranch, else_body, else_scope);
            }

            return NEW_VARIANT(
              LilyCheckedBodyFunItem,
              stmt,
              NEW_VARIANT(
                LilyCheckedStmt,
                if,
                &stmt->location,
                stmt,
                NEW(LilyCheckedStmtIf,
                    NEW(LilyCheckedStmtIfBranch, if_cond, if_body, if_scope),
                    elifs,
                    else_)));
        }
        case LILY_AST_STMT_KIND_MATCH:
            TODO("analysis match stmt");
        case LILY_AST_STMT_KIND_NEXT:
            if (!in_loop) {
                emit__Diagnostic(
                  NEW_VARIANT(
                    Diagnostic,
                    simple_lily_error,
                    &self->package->file,
                    &stmt->location,
                    NEW(LilyError,
                        LILY_ERROR_KIND_NEXT_IS_NOT_EXPECTED_IN_THIS_CONTEXT),
                    NULL,
                    NULL,
                    NULL),
                  &self->package->count_error);
            }

            return NEW_VARIANT(
              LilyCheckedBodyFunItem,
              stmt,
              NEW_VARIANT(LilyCheckedStmt,
                          next,
                          &stmt->location,
                          stmt,
                          NEW(LilyCheckedStmtNext, stmt->next.name)));
        case LILY_AST_STMT_KIND_RAISE:
            TODO("analysis raise stmt");
        case LILY_AST_STMT_KIND_RETURN: {
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
                            UNREACHABLE(
                              "must obtain a function or a lambda function");
                    }

                    break;
                default:
                    UNREACHABLE("must obtain a declaration");
            }

            if (stmt->return_.expr) {
                expr = check_expr__LilyAnalysis(self,
                                                stmt->return_.expr,
                                                scope,
                                                safety_mode,
                                                true,
                                                false,
                                                NULL);

                // Check the data type of the return expression
                if (fun_return_data_type) {
                    if (!eq__LilyCheckedDataType(fun_return_data_type,
                                                 expr->data_type)) {
                        FAILED("the data type doesn't match with the inferred "
                               "type or the specified return data type passed "
                               "to the function");
                    }
                } else {
                    fun_return_data_type =
                      clone__LilyCheckedDataType(expr->data_type);
                }
            } else {
                if (fun_return_data_type) {
                    switch (fun_return_data_type->kind) {
                        case LILY_CHECKED_DATA_TYPE_KIND_UNIT:
                        case LILY_CHECKED_DATA_TYPE_KIND_CVOID:
                            break;
                        default:
                            FAILED(
                              "expected Unit or CVoid as return data type");
                    }
                } else {
                    // TODO:Add the possibility for the compiler to have a list
                    // of potential return types (this data type will not be
                    // definable by the user).
                    fun_return_data_type = NEW(LilyCheckedDataType,
                                               LILY_CHECKED_DATA_TYPE_KIND_UNIT,
                                               &stmt->location);
                }
            }

            // TODO: add the `end_body` implicit return.
            // Add the `end_body` before the return statement
            append__Vec(current_body, end_body);

            return NEW_VARIANT(LilyCheckedBodyFunItem,
                               stmt,
                               NEW_VARIANT(LilyCheckedStmt,
                                           return,
                                           &stmt->location,
                                           stmt,
                                           NEW(LilyCheckedStmtReturn, expr)));
        }
        case LILY_AST_STMT_KIND_TRY:
            TODO("analysis try stmt");
        case LILY_AST_STMT_KIND_UNSAFE: {
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
        case LILY_AST_STMT_KIND_VARIABLE: {
            LilyCheckedScopeContainerVariable *sc_variable =
              NEW(LilyCheckedScopeContainerVariable,
                  stmt->variable.name,
                  i - end_body->len);
            int status = add_variable__LilyCheckedScope(scope, sc_variable);

            if (status) {
                emit__Diagnostic(
                  NEW_VARIANT(
                    Diagnostic,
                    simple_lily_error,
                    &self->package->file,
                    &stmt->location,
                    NEW(LilyError, LILY_ERROR_KIND_DUPLICATE_VARIABLE),
                    NULL,
                    NULL,
                    NULL),
                  &self->package->count_error);

                FREE(LilyCheckedScopeContainerVariable, sc_variable);
            }

            LilyCheckedDataType *checked_data_type =
              stmt->variable.data_type
                ? check_data_type__LilyAnalysis(
                    self, stmt->variable.data_type, scope, safety_mode)
                : NULL;
            LilyCheckedExpr *expr =
              check_expr__LilyAnalysis(self,
                                       stmt->variable.expr,
                                       scope,
                                       safety_mode,
                                       true,
                                       false,
                                       checked_data_type);

            if (checked_data_type) {
                if (!eq__LilyCheckedDataType(checked_data_type,
                                             expr->data_type)) {
                    FAILED("the data type doesn't match");
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
                                : clone__LilyCheckedDataType(expr->data_type),
                              expr,
                              stmt->variable.is_mut)));
        }
        case LILY_AST_STMT_KIND_WHILE: {
            LilyCheckedExpr *expr = check_expr__LilyAnalysis(
              self, stmt->while_.expr, scope, safety_mode, true, false, NULL);

            EXPECTED_BOOL_EXPR(expr);

            Vec *body = NEW(Vec);
            LilyCheckedScope *scope_while =
              NEW(LilyCheckedScope,
                  NEW_VARIANT(LilyCheckedParent, scope, scope, current_body),
                  NEW_VARIANT(LilyCheckedScopeDecls, scope, body));

            CHECK_FUN_BODY(
              stmt->while_.body, scope_while, body, safety_mode, true);

            return NEW_VARIANT(LilyCheckedBodyFunItem,
                               stmt,
                               NEW_VARIANT(LilyCheckedStmt,
                                           while,
                                           &stmt->location,
                                           stmt,
                                           NEW(LilyCheckedStmtWhile,
                                               stmt->while_.name,
                                               expr,
                                               body,
                                               scope_while)));
        }
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
                self, ast_item->expr, scope, safety_mode, true, false, NULL));
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
    TODO("check generic params");
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
            checked_param_data_type = check_data_type__LilyAnalysis(
              self, param->data_type, scope, LILY_CHECKED_SAFETY_MODE_SAFE);
        } else {
            checked_param_data_type = NEW(
              LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UNKNOWN, NULL);
        }

        switch (param->kind) {
            case LILY_AST_DECL_FUN_PARAM_KIND_DEFAULT: {
                LilyCheckedExpr *default_value =
                  check_expr__LilyAnalysis(self,
                                           param->default_,
                                           scope,
                                           LILY_CHECKED_SAFETY_MODE_SAFE,
                                           true,
                                           false,
                                           checked_param_data_type);

                if (default_value) {
                    push__Vec(checked_params,
                              NEW_VARIANT(LilyCheckedDeclFunParam,
                                          default,
                                          param->name,
                                          checked_param_data_type,
                                          param->location,
                                          default_value));
                } else {
                    push__Vec(checked_params,
                              NEW_VARIANT(LilyCheckedDeclFunParam,
                                          normal,
                                          param->name,
                                          checked_param_data_type,
                                          param->location));
                }

                break;
            }
            case LILY_AST_DECL_FUN_PARAM_KIND_NORMAL: {
                push__Vec(checked_params,
                          NEW_VARIANT(LilyCheckedDeclFunParam,
                                      normal,
                                      param->name,
                                      checked_param_data_type,
                                      param->location));

                break;
            }
            default:
                UNREACHABLE("unknown variant");
        }

        LilyCheckedScopeContainerVariable *sc_variable =
          NEW(LilyCheckedScopeContainerVariable, param->name, i);

        int is_failed = add_param__LilyCheckedScope(scope, sc_variable);

        if (is_failed) {
            emit__Diagnostic(
              NEW_VARIANT(Diagnostic,
                          simple_lily_error,
                          &self->package->file,
                          &param->location,
                          NEW(LilyError, LILY_ERROR_KIND_DUPLICATE_PARAM_NAME),
                          NULL,
                          NULL,
                          NULL),
              &self->package->count_error);

            FREE(LilyCheckedScopeContainerVariable, sc_variable);
        }
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

    if (fun->fun.is_main && fun->fun.is_recursive) {
        FAILED("the main function cannot be recursive");
    }

    // 2. Check generic params
    if (fun->ast_decl->fun.generic_params) {
        if (fun->fun.is_main) {
            FAILED("generic params is not expected in main function");
        }

        if (!fun->fun.generic_params) {
            fun->fun.generic_params = check_generic_params(
              self, fun->ast_decl->fun.generic_params, fun->fun.scope);
        }
    }

    // 3. Check params.
    if (fun->ast_decl->fun.params) {
        if (fun->fun.is_main) {
            FAILED(
              "no (explicit) parameters are expected in the main function");
        }

        if (!fun->fun.params) {
            fun->fun.params = check_fun_params__LilyAnalysis(
              self, fun->ast_decl->fun.params, fun->fun.scope);
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
                                            LILY_CHECKED_SAFETY_MODE_SAFE);

            // Check the return data type of the main function.
            if (fun->fun.is_main) {
                switch (fun->fun.return_data_type->kind) {
                    case LILY_CHECKED_DATA_TYPE_KIND_INT32:
                    case LILY_CHECKED_DATA_TYPE_KIND_UNIT:
                    case LILY_CHECKED_DATA_TYPE_KIND_CVOID:
                        break;
                    default:
                        FAILED("this return data type is not expected for a "
                               "main function");
                }
            }
        } else {
            // If the return type (checked) is already configured, it means that
            // the function is recursive.
            fun->fun.is_recursive = true;
        }
    } else {
        if (!fun->fun.return_data_type) {
            fun->fun.return_data_type = NEW(
              LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UNKNOWN, NULL);
        } else {
            // If the return type (checked) is already configured, it means that
            // the function is recursive.
            fun->fun.is_recursive = true;
        }
    }
}

void
check_fun__LilyAnalysis(LilyAnalysis *self, LilyCheckedDecl *fun)
{
    if (fun->fun.is_checked) {
        return;
    }

    // 1. Check fun signature
    check_fun_signature__LilyAnalysis(self, fun);

    // 2. Init scope of body.
    fun->fun.scope =
      NEW(LilyCheckedScope,
          NEW_VARIANT(LilyCheckedParent, decl, fun->fun.scope, fun),
          NEW_VARIANT(LilyCheckedScopeDecls, scope, fun->fun.body));

    // 3. Check body.
    CHECK_FUN_BODY(fun->ast_decl->fun.body,
                   fun->fun.scope,
                   fun->fun.body,
                   LILY_CHECKED_SAFETY_MODE_SAFE,
                   false);

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
                                    LILY_CHECKED_SAFETY_MODE_SAFE);

    constant->constant.expr =
      check_expr__LilyAnalysis(self,
                               constant->ast_decl->constant.expr,
                               scope,
                               LILY_CHECKED_SAFETY_MODE_SAFE,
                               true,
                               false,
                               constant->constant.data_type);

    if (!eq__LilyCheckedDataType(constant->constant.expr->data_type,
                                 constant->constant.data_type)) {
        FAILED("data type don't match");
    }

    constant->constant.is_checked = true;
}

void
check_alias__LilyAnalysis(LilyAnalysis *self, LilyCheckedDecl *alias)
{
    if (alias->type.alias.is_checked) {
        return;
    }

    if (alias->ast_decl->type.alias.generic_params) {
        alias->type.alias.generic_params =
          check_generic_params(self,
                               alias->ast_decl->type.alias.generic_params,
                               alias->type.alias.scope);
    }

    alias->type.alias.data_type =
      check_data_type__LilyAnalysis(self,
                                    alias->ast_decl->type.alias.data_type,
                                    alias->type.alias.scope,
                                    LILY_CHECKED_SAFETY_MODE_SAFE);

    alias->type.alias.is_checked = true;
}

Vec *
check_fields__LilyAnalysis(LilyAnalysis *self,
                           Vec *ast_fields,
                           LilyCheckedScope *scope)
{
    Vec *check_fields = NEW(Vec);

    for (Usize i = 0; i < ast_fields->len; ++i) {
        LilyAstField *ast_field = get__Vec(ast_fields, i);

        // TODO: in the future add a support for unsafe record
        LilyCheckedDataType *data_type = check_data_type__LilyAnalysis(
          self, ast_field->data_type, scope, LILY_CHECKED_SAFETY_MODE_SAFE);
        LilyCheckedExpr *optional_expr = NULL;

        if (ast_field->optional_expr) {
            optional_expr =
              check_expr__LilyAnalysis(self,
                                       ast_field->optional_expr,
                                       scope,
                                       LILY_CHECKED_SAFETY_MODE_SAFE,
                                       false,
                                       false,
                                       data_type);
        }

        LilyCheckedScopeContainerVariable *sc_variable =
          NEW(LilyCheckedScopeContainerVariable, ast_field->name, i);
        int is_failed = add_variable__LilyCheckedScope(scope, sc_variable);

        if (is_failed) {
            emit__Diagnostic(
              NEW_VARIANT(Diagnostic,
                          simple_lily_error,
                          &self->package->file,
                          &ast_field->location,
                          NEW(LilyError, LILY_ERROR_KIND_DUPLICATE_FIELD),
                          NULL,
                          NULL,
                          NULL),
              &self->package->count_error);

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

    return check_fields;
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
                    FAILED("infinite data type");
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

    record->type.record.fields = check_fields__LilyAnalysis(
      self, record->ast_decl->type.record.fields, record->type.record.scope);

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

Vec *
check_variants__LilyAnalysis(LilyAnalysis *self,
                             Vec *ast_variants,
                             LilyCheckedScope *scope)
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
                                            LILY_CHECKED_SAFETY_MODE_SAFE);
        }

        LilyCheckedScopeContainerVariable *sc_variable =
          NEW(LilyCheckedScopeContainerVariable, ast_variant->name, i);
        int is_failed = add_variable__LilyCheckedScope(scope, sc_variable);

        if (is_failed) {
            emit__Diagnostic(
              NEW_VARIANT(Diagnostic,
                          simple_lily_error,
                          &self->package->file,
                          &ast_variant->location,
                          NEW(LilyError, LILY_ERROR_KIND_DUPLICATE_VARIANT),
                          NULL,
                          NULL,
                          NULL),
              &self->package->count_error);

            FREE(LilyCheckedScopeContainerVariable, sc_variable);
        }

        push__Vec(check_variants,
                  NEW(LilyCheckedVariant,
                      ast_variant->name,
                      format__String("{S}.{S}",
                                     scope->decls.decl->type.enum_.global_name,
                                     ast_variant->name),
                      data_type,
                      &ast_variant->location));
    }

    return check_variants;
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

    enum_->type.enum_.variants = check_variants__LilyAnalysis(
      self, enum_->ast_decl->type.enum_.variants, enum_->type.enum_.scope);

    // Check if the enum is recursive
    for (Usize i = 0; i < enum_->type.enum_.variants->len; ++i) {
        LilyCheckedField *variant = get__Vec(enum_->type.enum_.variants, i);

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
check_decls__LilyAnalysis(LilyAnalysis *self,
                          Vec *decls,
                          LilyCheckedScope *scope)
{
    for (Usize i = 0; i < decls->len; ++i) {
        LilyCheckedDecl *decl = get__Vec(decls, i);

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
                        TODO("check type");
                }

                break;
            default:
                TODO("analysis declaration");
        }
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

        emit__Diagnostic(
          NEW_VARIANT(Diagnostic,
                      simple_lily_error,
                      &self->package->file,
                      &location_error,
                      NEW(LilyError, LILY_ERROR_KIND_EXPECTED_MAIN_FUNCTION),
                      NULL,
                      NULL,
                      NULL),
          &self->package->count_error);
    }

    if (self->package->count_error > 0) {
        exit(1);
    }

#ifdef DEBUG_ANALYSIS
    printf("====Analysis(%s)====\n", self->package->file.name);

    PRINTLN("{Sr}", to_string__Debug__LilyCheckedScope(self->module.scope));

    // for (Usize i = 0; i < self->module.decls->len; ++i) {
    // 	PRINTLN("{Sr}",
    // to_string__Debug__LilyCheckedDecl(get__Vec(self->module.decls, i)));
    // }
#endif
}

DESTRUCTOR(LilyAnalysis, const LilyAnalysis *self)
{
    FREE(String, self->module.name);
    // FREE(LilyCheckedDeclModule, &self->module);
}
