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

#include <base/new.h>
#include <base/print.h>

#include <core/lily/analysis.h>
#include <core/lily/ast.h>
#include <core/lily/checked/safety_mode.h>
#include <core/lily/lily.h>
#include <core/lily/package.h>

#include <core/shared/diagnostic.h>

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

static inline void
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
push_all_delcs__LilyAnalysis(LilyAnalysis *self,
                             const Vec *decls,
                             LilyCheckedDeclModule *module);

static LilyCheckedDataType *
check_data_type__LilyAnalysis(LilyAnalysis *self,
                              LilyAstDataType *data_type,
                              LilyCheckedScope *scope);

static LilyCheckedExpr *
check_binary_expr__LilyAnalysis(LilyAnalysis *self,
                                LilyAstExpr *expr,
                                LilyCheckedScope *scope,
                                enum LilyCheckedSafetyMode safety_mode);

static LilyCheckedExpr *
check_expr__LilyAnalysis(LilyAnalysis *self,
                         LilyAstExpr *expr,
                         LilyCheckedScope *scope,
                         enum LilyCheckedSafetyMode safety_mode);

static LilyCheckedBodyFunItem *
check_stmt__LilyAnalysis(LilyAnalysis *self,
                         const LilyAstStmt *stmt,
                         LilyCheckedScope *scope,
                         Usize i,
                         bool in_loop,
                         enum LilyCheckedSafetyMode safety_mode,
                         Vec *current_body);

/// @return Vec<LilyCheckedFunParam*>*
static Vec *
check_fun_params__LilyAnalysis(LilyAnalysis *self,
                               const Vec *params,
                               LilyCheckedScope *scope);

static void
check_fun__LilyAnalysis(LilyAnalysis *self, LilyCheckedDecl *fun);

// Apply import.
static void
run_step0__LilyAnalysis(LilyAnalysis *self);

// Push all declarations.
static inline void
run_step1__LilyAnalysis(LilyAnalysis *self);

// Check all declarations.
static void
run_step2__LilyAnalysis(LilyAnalysis *self);

#define CHECK_FUN_BODY(ast_body, scope, body, safety_mode, in_loop)            \
    for (Usize j = 0; j < ast_body->len; ++j) {                                \
        LilyAstBodyFunItem *item = get__Vec(ast_body, j);                      \
                                                                               \
        switch (item->kind) {                                                  \
            case LILY_AST_BODY_FUN_ITEM_KIND_EXPR:                             \
                push__Vec(                                                     \
                  body,                                                        \
                  NEW_VARIANT(LilyCheckedBodyFunItem,                          \
                              expr,                                            \
                              check_expr__LilyAnalysis(                        \
                                self, item->expr, scope, safety_mode)));       \
                                                                               \
                break;                                                         \
            case LILY_AST_BODY_FUN_ITEM_KIND_STMT:                             \
                push__Vec(                                                     \
                  body,                                                        \
                  check_stmt__LilyAnalysis(                                    \
                    self, &item->stmt, scope, j, in_loop, safety_mode, body)); \
                                                                               \
                break;                                                         \
            default:                                                           \
                UNREACHABLE("unknown variant");                                \
        }                                                                      \
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
                              NULL,
                              NULL,
                              constant->constant.visibility)));
}

void
push_error__LilyAnalysis(LilyAnalysis *self,
                         LilyAstDecl *error,
                         LilyCheckedDeclModule *module)
{
    push__Vec(module->decls,
              NEW_VARIANT(LilyCheckedDecl,
                          error,
                          &error->location,
                          error,
                          NEW(LilyCheckedDeclError,
                              error->error.name,
                              NULL,
                              NULL,
                              error->error.visibility)));
}

void
push_fun__LilyAnalysis(LilyAnalysis *self,
                       LilyAstDecl *fun,
                       LilyCheckedDeclModule *module)
{
    Vec *body = NEW(Vec);

    push__Vec(
      module->decls,
      NEW_VARIANT(
        LilyCheckedDecl,
        fun,
        &fun->location,
        fun,
        NEW(LilyCheckedDeclFun,
            fun->fun.name,
            NULL,
            NULL,
            NULL,
            body,
            NEW(LilyCheckedScope,
                NEW_VARIANT(LilyCheckedParent, module, module->scope, module),
                NEW_VARIANT(LilyCheckedScopeDecls, scope, body)),
            fun->fun.visibility,
            fun->fun.is_async,
            fun->fun.is_operator)));
}

void
push_module__LilyAnalysis(LilyAnalysis *self,
                          LilyAstDecl *module_decl,
                          LilyCheckedDeclModule *module,
                          Usize i)
{
    LilyCheckedDecl *checked_module =
      NEW_VARIANT(LilyCheckedDecl,
                  module,
                  &module_decl->location,
                  module_decl,
                  NEW(LilyCheckedDeclModule,
                      module_decl->module.name,
                      NEW(Vec),
                      NULL,
                      module_decl->module.visibility));

    checked_module->module.scope =
      NEW(LilyCheckedScope,
          NEW_VARIANT(LilyCheckedParent, module, module->scope, module),
          NEW_VARIANT(LilyCheckedScopeDecls, module, &checked_module->module));

    push_all_delcs__LilyAnalysis(
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
    LilyCheckedDecl *checked_enum_object =
      NEW_VARIANT(LilyCheckedDecl,
                  object,
                  &enum_object->location,
                  enum_object,
                  NEW_VARIANT(LilyCheckedDeclObject,
                              enum,
                              NEW(LilyCheckedDeclEnumObject,
                                  enum_object->object.enum_.name,
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
    LilyCheckedDecl *checked_record_object =
      NEW_VARIANT(LilyCheckedDecl,
                  object,
                  &record_object->location,
                  record_object,
                  NEW_VARIANT(LilyCheckedDeclObject,
                              record,
                              NEW(LilyCheckedDeclRecordObject,
                                  record_object->object.record.name,
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
    push__Vec(module->decls,
              NEW_VARIANT(LilyCheckedDecl,
                          type,
                          &alias->location,
                          alias,
                          NEW_VARIANT(LilyCheckedDeclType,
                                      alias,
                                      NEW(LilyCheckedDeclAlias,
                                          alias->type.alias.name,
                                          NULL,
                                          NULL,
                                          alias->type.alias.visibility))));
}

void
push_enum__LilyAnalysis(LilyAnalysis *self,
                        LilyAstDecl *enum_,
                        LilyCheckedDeclModule *module)
{
    LilyCheckedDecl *checked_enum =
      NEW_VARIANT(LilyCheckedDecl,
                  type,
                  &enum_->location,
                  enum_,
                  NEW_VARIANT(LilyCheckedDeclType,
                              enum,
                              NEW(LilyCheckedDeclEnum,
                                  enum_->type.enum_.name,
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
push_all_delcs__LilyAnalysis(LilyAnalysis *self,
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
                              LilyCheckedScope *scope)
{
    switch (data_type->kind) {
        case LILY_AST_DATA_TYPE_KIND_ANY:
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
                                    self, data_type->array.data_type, scope),
                                  data_type->array.size));
                default:
                    return NEW_VARIANT(
                      LilyCheckedDataType,
                      array,
                      &data_type->location,
                      NEW(LilyCheckedDataTypeArray,
                          (enum LilyCheckedDataTypeArrayKind)(
                            int)data_type->array.kind,
                          check_data_type__LilyAnalysis(
                            self, data_type->array.data_type, scope)));
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
            return NEW(LilyCheckedDataType,
                       LILY_CHECKED_DATA_TYPE_KIND_BYTES,
                       &data_type->location);
        case LILY_AST_DATA_TYPE_KIND_CHAR:
            return NEW(LilyCheckedDataType,
                       LILY_CHECKED_DATA_TYPE_KIND_CHAR,
                       &data_type->location);
        case LILY_AST_DATA_TYPE_KIND_CUSTOM:
            TODO("check custom data type");
        case LILY_AST_DATA_TYPE_KIND_EXCEPTION:
            return NEW_VARIANT(
              LilyCheckedDataType,
              exception,
              &data_type->location,
              check_data_type__LilyAnalysis(self, data_type->exception, scope));
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
              check_data_type__LilyAnalysis(self, data_type->list, scope));
        case LILY_AST_DATA_TYPE_KIND_MUT:
            return NEW_VARIANT(
              LilyCheckedDataType,
              mut,
              &data_type->location,
              check_data_type__LilyAnalysis(self, data_type->mut, scope));
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
              check_data_type__LilyAnalysis(self, data_type->optional, scope));
        case LILY_AST_DATA_TYPE_KIND_PTR:
            return NEW_VARIANT(
              LilyCheckedDataType,
              ptr,
              &data_type->location,
              check_data_type__LilyAnalysis(self, data_type->ptr, scope));
        case LILY_AST_DATA_TYPE_KIND_REF:
            return NEW_VARIANT(
              LilyCheckedDataType,
              ref,
              &data_type->location,
              check_data_type__LilyAnalysis(self, data_type->ref, scope));
        case LILY_AST_DATA_TYPE_KIND_SELF:
            TODO("Check Self data type");
        case LILY_AST_DATA_TYPE_KIND_STR:
            return NEW(LilyCheckedDataType,
                       LILY_CHECKED_DATA_TYPE_KIND_STR,
                       &data_type->location);
        case LILY_AST_DATA_TYPE_KIND_TRACE:
            return NEW_VARIANT(
              LilyCheckedDataType,
              trace,
              &data_type->location,
              check_data_type__LilyAnalysis(self, data_type->ref, scope));
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

LilyCheckedExpr *
check_binary_expr__LilyAnalysis(LilyAnalysis *self,
                                LilyAstExpr *expr,
                                LilyCheckedScope *scope,
                                enum LilyCheckedSafetyMode safety_mode)
{
    switch (expr->binary.kind) {
        case LILY_AST_EXPR_BINARY_KIND_ADD:
            TODO("analyze +");
        case LILY_AST_EXPR_BINARY_KIND_AND:
            TODO("analyze and");
        case LILY_AST_EXPR_BINARY_KIND_ASSIGN: {
            LilyCheckedExpr *left = check_expr__LilyAnalysis(
              self, expr->binary.left, scope, safety_mode);
            LilyCheckedExpr *right = check_expr__LilyAnalysis(
              self, expr->binary.right, scope, safety_mode);

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
              NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UNIT, NULL),
              expr,
              NEW(LilyCheckedExprBinary,
                  (enum LilyCheckedExprBinaryKind)(int)expr->binary.kind,
                  left,
                  right));
        }
        case LILY_AST_EXPR_BINARY_KIND_ASSIGN_ADD:
            TODO("analyze +=");
        case LILY_AST_EXPR_BINARY_KIND_ASSIGN_BIT_AND:
            TODO("analyze &=");
        case LILY_AST_EXPR_BINARY_KIND_ASSIGN_BIT_L_SHIFT:
            TODO("analyze <<=");
        case LILY_AST_EXPR_BINARY_KIND_ASSIGN_BIT_OR:
            TODO("analyze |=");
        case LILY_AST_EXPR_BINARY_KIND_ASSIGN_BIT_R_SHIFT:
            TODO("analyze >>=");
        case LILY_AST_EXPR_BINARY_KIND_ASSIGN_DIV:
            TODO("analyze /=");
        case LILY_AST_EXPR_BINARY_KIND_ASSIGN_EXP:
            TODO("analyze **=");
        case LILY_AST_EXPR_BINARY_KIND_ASSIGN_MOD:
            TODO("analyze %=");
        case LILY_AST_EXPR_BINARY_KIND_ASSIGN_MUL:
            TODO("analyze *=");
        case LILY_AST_EXPR_BINARY_KIND_ASSIGN_SUB:
            TODO("analyze -=");
        case LILY_AST_EXPR_BINARY_KIND_ASSIGN_XOR:
            TODO("analyze xor=");
        case LILY_AST_EXPR_BINARY_KIND_BIT_AND:
            TODO("analyze &");
        case LILY_AST_EXPR_BINARY_KIND_BIT_L_SHIFT:
            TODO("analyze <<");
        case LILY_AST_EXPR_BINARY_KIND_BIT_OR:
            TODO("analyze |");
        case LILY_AST_EXPR_BINARY_KIND_BIT_R_SHIFT:
            TODO("analyze >>");
        case LILY_AST_EXPR_BINARY_KIND_CHAIN:
            TODO("analyze |>");
        case LILY_AST_EXPR_BINARY_KIND_DIV:
            TODO("analyze /");
        case LILY_AST_EXPR_BINARY_KIND_EQ:
            TODO("analyze ==");
        case LILY_AST_EXPR_BINARY_KIND_EXP:
            TODO("analyze **");
        case LILY_AST_EXPR_BINARY_KIND_GREATER:
            TODO("analyze >");
        case LILY_AST_EXPR_BINARY_KIND_GREATER_EQ:
            TODO("analyze >=");
        case LILY_AST_EXPR_BINARY_KIND_LESS:
            TODO("analyze <");
        case LILY_AST_EXPR_BINARY_KIND_LESS_EQ:
            TODO("analyze <=");
        case LILY_AST_EXPR_BINARY_KIND_LIST_HEAD:
            TODO("analyze ->");
        case LILY_AST_EXPR_BINARY_KIND_LIST_TAIL:
            TODO("analyze <-");
        case LILY_AST_EXPR_BINARY_KIND_MOD:
            TODO("analyze %");
        case LILY_AST_EXPR_BINARY_KIND_MUL:
            TODO("analyze *");
        case LILY_AST_EXPR_BINARY_KIND_NOT_EQ:
            TODO("analyze not=");
        case LILY_AST_EXPR_BINARY_KIND_OR:
            TODO("analyze or");
        case LILY_AST_EXPR_BINARY_KIND_RANGE:
            TODO("analyze ..");
        case LILY_AST_EXPR_BINARY_KIND_SUB:
            TODO("analyze -");
        case LILY_AST_EXPR_BINARY_KIND_XOR:
            TODO("analyze xor");
        default:
            UNREACHABLE("unknown variant");
    }
}

LilyCheckedExpr *
check_expr__LilyAnalysis(LilyAnalysis *self,
                         LilyAstExpr *expr,
                         LilyCheckedScope *scope,
                         enum LilyCheckedSafetyMode safety_mode)
{
    switch (expr->kind) {
        case LILY_AST_EXPR_KIND_ACCESS:
            TODO("access expression");
        case LILY_AST_EXPR_KIND_ARRAY:
            TODO("array expression");
        case LILY_AST_EXPR_KIND_BINARY:
            return check_binary_expr__LilyAnalysis(
              self, expr, scope, safety_mode);
        case LILY_AST_EXPR_KIND_CALL:
            TODO("call expression");
        case LILY_AST_EXPR_KIND_CAST:
            TODO("cast expression");
        case LILY_AST_EXPR_KIND_GROUPING:
            TODO("grouping expression");
        case LILY_AST_EXPR_KIND_IDENTIFIER:
            TODO("identifier expression");
        case LILY_AST_EXPR_KIND_IDENTIFIER_DOLLAR:
            TODO("identifier dollar expression");
        case LILY_AST_EXPR_KIND_LAMBDA:
            TODO("lambda expression");
        case LILY_AST_EXPR_KIND_LIST:
            TODO("list expression");
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
                    return NEW_VARIANT(LilyCheckedExpr,
                                       literal,
                                       &expr->location,
                                       NEW(LilyCheckedDataType,
                                           LILY_CHECKED_DATA_TYPE_KIND_BYTES,
                                           &expr->location),
                                       expr,
                                       NEW_VARIANT(LilyCheckedExprLiteral,
                                                   bytes,
                                                   expr->literal.bytes));
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
                case LILY_AST_EXPR_LITERAL_KIND_FLOAT64:
                    return NEW_VARIANT(LilyCheckedExpr,
                                       literal,
                                       &expr->location,
                                       NEW(LilyCheckedDataType,
                                           LILY_CHECKED_DATA_TYPE_KIND_FLOAT64,
                                           &expr->location),
                                       expr,
                                       NEW_VARIANT(LilyCheckedExprLiteral,
                                                   float64,
                                                   expr->literal.float64));
                case LILY_AST_EXPR_LITERAL_KIND_INT32:
                    return NEW_VARIANT(LilyCheckedExpr,
                                       literal,
                                       &expr->location,
                                       NEW(LilyCheckedDataType,
                                           LILY_CHECKED_DATA_TYPE_KIND_INT32,
                                           &expr->location),
                                       expr,
                                       NEW_VARIANT(LilyCheckedExprLiteral,
                                                   int32,
                                                   expr->literal.int32));
                case LILY_AST_EXPR_LITERAL_KIND_INT64:
                    return NEW_VARIANT(LilyCheckedExpr,
                                       literal,
                                       &expr->location,
                                       NEW(LilyCheckedDataType,
                                           LILY_CHECKED_DATA_TYPE_KIND_INT64,
                                           &expr->location),
                                       expr,
                                       NEW_VARIANT(LilyCheckedExprLiteral,
                                                   int64,
                                                   expr->literal.int64));
                case LILY_AST_EXPR_LITERAL_KIND_NIL:
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
                                       NEW(LilyCheckedDataType,
                                           LILY_CHECKED_DATA_TYPE_KIND_STR,
                                           &expr->location),
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
        case LILY_AST_EXPR_KIND_UNARY:
            TODO("unary expression");
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
        case LILY_AST_STMT_KIND_DEFER:
            TODO("analysis defer stmt");
        case LILY_AST_STMT_KIND_DROP:
            TODO("analysis drop stmt");
        case LILY_AST_STMT_KIND_FOR:
            TODO("analysis for stmt");
        case LILY_AST_STMT_KIND_IF: {
            LilyCheckedExpr *if_cond = check_expr__LilyAnalysis(
              self, stmt->if_.if_expr, scope, safety_mode);
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
                      safety_mode);
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
            LilyCheckedExpr *expr = NULL;

            if (stmt->return_.expr) {
                expr = check_expr__LilyAnalysis(
                  self, stmt->return_.expr, scope, safety_mode);
            }

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
            LilyCheckedExpr *expr = check_expr__LilyAnalysis(
              self, stmt->variable.expr, scope, safety_mode);

            LilyCheckedScopeContainerVariable *sc_variable =
              NEW(LilyCheckedScopeContainerVariable, stmt->variable.name, i);
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

            return NEW_VARIANT(LilyCheckedBodyFunItem,
                               stmt,
                               NEW_VARIANT(LilyCheckedStmt,
                                           variable,
                                           &stmt->location,
                                           stmt,
                                           NEW(LilyCheckedStmtVariable,
                                               stmt->variable.name,
                                               expr->data_type,
                                               expr,
                                               stmt->variable.is_mut)));
        }
        case LILY_AST_STMT_KIND_WHILE: {
            LilyCheckedExpr *expr = check_expr__LilyAnalysis(
              self, stmt->while_.expr, scope, safety_mode);
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
            checked_param_data_type =
              check_data_type__LilyAnalysis(self, param->data_type, scope);
        } else {
            checked_param_data_type = NEW(
              LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UNKNOWN, NULL);
        }

        switch (param->kind) {
            case LILY_AST_DECL_FUN_PARAM_KIND_DEFAULT: {
                LilyCheckedExpr *default_value = check_expr__LilyAnalysis(
                  self, param->default_, scope, LILY_CHECKED_SAFETY_MODE_SAFE);

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

        int is_failed = add_variable__LilyCheckedScope(scope, sc_variable);

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
check_fun__LilyAnalysis(LilyAnalysis *self, LilyCheckedDecl *fun)
{
    // 1. Check params.
    if (fun->ast_decl->fun.params) {
        fun->fun.params = check_fun_params__LilyAnalysis(
          self, fun->ast_decl->fun.params, fun->fun.scope);
    }

    // 2. Check return data type.
    if (fun->ast_decl->fun.return_data_type) {
        fun->fun.return_data_type = check_data_type__LilyAnalysis(
          self, fun->ast_decl->fun.return_data_type, fun->fun.scope);
    } else {
        fun->fun.return_data_type =
          NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UNKNOWN, NULL);
    }

    // 3. Check body.
    fun->fun.body = NEW(Vec);

    CHECK_FUN_BODY(fun->ast_decl->fun.body,
                   fun->fun.scope,
                   fun->fun.body,
                   LILY_CHECKED_SAFETY_MODE_SAFE,
                   false);

    fun->is_checked = true;
}

static void
run_step0__LilyAnalysis(LilyAnalysis *self)
{
}

void
run_step1__LilyAnalysis(LilyAnalysis *self)
{
    push_all_delcs__LilyAnalysis(self, self->parser->decls, &self->module);
}

void
run_step2__LilyAnalysis(LilyAnalysis *self)
{
    for (Usize i = 0; i < self->module.decls->len; ++i) {
        LilyCheckedDecl *decl = get__Vec(self->module.decls, i);

        if (!decl->is_checked) {
            switch (decl->kind) {
                case LILY_CHECKED_DECL_KIND_FUN:
                    check_fun__LilyAnalysis(self, decl);

                    break;
                default:
                    TODO("analysis declaration");
            }
        }
    }
}

void
run__LilyAnalysis(LilyAnalysis *self)
{
    self->module.scope =
      NEW(LilyCheckedScope,
          NULL,
          NEW_VARIANT(LilyCheckedScopeDecls, module, &self->module));

    run_step0__LilyAnalysis(self);
    run_step1__LilyAnalysis(self);
    run_step2__LilyAnalysis(self);

    if (self->package->count_error > 0) {
        exit(1);
    }

#ifdef DEBUG_ANALYSIS
    printf("====Analysis(%s)====\n", self->package->file.name);

    PRINTLN("{Sr}", to_string__Debug__LilyCheckedScope(self->module.scope));
#endif
}

DESTRUCTOR(LilyAnalysis, const LilyAnalysis *self)
{
    FREE(String, self->module.name);
    // FREE(LilyCheckedDeclModule, &self->module);
}
