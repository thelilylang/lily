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
#include <core/lily/checked/expr/literal.h>

#include <core/shared/diagnostic.h>

static inline void
push_constant__LilyAnalysis(LilyAnalysis *self,
                            LilyAstDecl *constant,
                            LilyCheckedDeclModule *module);

static inline void
push_error__LilyAnalysis(LilyAnalysis *self,
                         LilyAstDecl *error,
                         LilyCheckedDeclModule *module);

static inline void
push_fun__LilyAnalysis(LilyAnalysis *self,
                       LilyAstDecl *fun,
                       LilyCheckedDeclModule *module);

static void
push_module__LilyAnalysis(LilyAnalysis *self,
                          LilyAstDecl *module_decl,
                          LilyCheckedDeclModule *module,
                          Usize i);

static inline void
push_class__LilyAnalysis(LilyAnalysis *self,
                         LilyAstDecl *class,
                         LilyCheckedDeclModule *module);

static inline void
push_enum_object__LilyAnalysis(LilyAnalysis *self,
                               LilyAstDecl *enum_object,
                               LilyCheckedDeclModule *module);

static inline void
push_record_object__LilyAnalysis(LilyAnalysis *self,
                                 LilyAstDecl *record_object,
                                 LilyCheckedDeclModule *module);

static inline void
push_trait__LilyAnalysis(LilyAnalysis *self,
                         LilyAstDecl *trait,
                         LilyCheckedDeclModule *module);

static inline void
push_alias__LilyAnalysis(LilyAnalysis *self,
                         LilyAstDecl *alias,
                         LilyCheckedDeclModule *module);

static inline void
push_enum__LilyAnalysis(LilyAnalysis *self,
                        LilyAstDecl *enum_,
                        LilyCheckedDeclModule *module);

static inline void
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
check_expr__LilyAnalysis(LilyAnalysis *self,
                         LilyAstExpr *expr,
                         LilyCheckedScope *scope,
                         enum LilyCheckedSafetyMode safety_mode);

static LilyCheckedStmt *
check_stmt__LilyAnalysis(LilyAnalysis *self,
                         LilyAstStmt *stmt,
                         LilyCheckedScope *scope);

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
            NULL,
            NEW(LilyCheckedScope,
                NEW_VARIANT(LilyCheckedParent, module, module->scope, module)),
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
    LilyCheckedDecl *checked_module = NEW_VARIANT(
      LilyCheckedDecl,
      module,
      &module_decl->location,
      module_decl,
      NEW(LilyCheckedDeclModule,
          module_decl->module.name,
          NEW(Vec),
          NEW(LilyCheckedScope,
              NEW_VARIANT(LilyCheckedParent, module, module->scope, module)),
          NEW(LilyCheckedAccessModule, i),
          module_decl->module.visibility));

    push_all_delcs__LilyAnalysis(
      self, module_decl->module.decls, &checked_module->module);

    push__Vec(module->decls, checked_module);
}

void
push_class__LilyAnalysis(LilyAnalysis *self,
                         LilyAstDecl *class,
                         LilyCheckedDeclModule *module)
{
    push__Vec(module->decls,
              NEW_VARIANT(
                LilyCheckedDecl,
                object,
                &class->location,
                class,
                NEW_VARIANT(
                  LilyCheckedDeclObject,
                  class,
                  NEW(LilyCheckedDeclClass,
                      class->object.class.name,
                      NULL,
                      NULL,
                      NULL,
                      NULL,
                      NEW(LilyCheckedScope,
                          NEW_VARIANT(
                            LilyCheckedParent, module, module->scope, module)),
                      class->object.class.visibility))));
}

void
push_enum_object__LilyAnalysis(LilyAnalysis *self,
                               LilyAstDecl *enum_object,
                               LilyCheckedDeclModule *module)
{
    push__Vec(module->decls,
              NEW_VARIANT(
                LilyCheckedDecl,
                object,
                &enum_object->location,
                enum_object,
                NEW_VARIANT(
                  LilyCheckedDeclObject,
                  enum,
                  NEW(LilyCheckedDeclEnumObject,
                      enum_object->object.enum_.name,
                      NULL,
                      NULL,
                      NULL,
                      NEW(LilyCheckedScope,
                          NEW_VARIANT(
                            LilyCheckedParent, module, module->scope, module)),
                      enum_object->object.enum_.visibility))));
}

void
push_record_object__LilyAnalysis(LilyAnalysis *self,
                                 LilyAstDecl *record_object,
                                 LilyCheckedDeclModule *module)
{
    push__Vec(module->decls,
              NEW_VARIANT(
                LilyCheckedDecl,
                object,
                &record_object->location,
                record_object,
                NEW_VARIANT(
                  LilyCheckedDeclObject,
                  record,
                  NEW(LilyCheckedDeclRecordObject,
                      record_object->object.record.name,
                      NULL,
                      NULL,
                      NULL,
                      NEW(LilyCheckedScope,
                          NEW_VARIANT(
                            LilyCheckedParent, module, module->scope, module)),
                      record_object->object.record.visibility))));
}

void
push_trait__LilyAnalysis(LilyAnalysis *self,
                         LilyAstDecl *trait,
                         LilyCheckedDeclModule *module)
{
    push__Vec(module->decls,
              NEW_VARIANT(
                LilyCheckedDecl,
                object,
                &trait->location,
                trait,
                NEW_VARIANT(
                  LilyCheckedDeclObject,
                  trait,
                  NEW(LilyCheckedDeclTrait,
                      trait->object.trait.name,
                      NULL,
                      NULL,
                      NULL,
                      NEW(LilyCheckedScope,
                          NEW_VARIANT(
                            LilyCheckedParent, module, module->scope, module)),
                      trait->object.trait.visibility))));
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
    push__Vec(module->decls,
              NEW_VARIANT(
                LilyCheckedDecl,
                type,
                &enum_->location,
                enum_,
                NEW_VARIANT(
                  LilyCheckedDeclType,
                  enum,
                  NEW(LilyCheckedDeclEnum,
                      enum_->type.enum_.name,
                      NULL,
                      NULL,
                      NEW(LilyCheckedScope,
                          NEW_VARIANT(
                            LilyCheckedParent, module, module->scope, module)),
                      enum_->type.enum_.visibility))));
}

void
push_record__LilyAnalysis(LilyAnalysis *self,
                          LilyAstDecl *record,
                          LilyCheckedDeclModule *module)
{
    push__Vec(module->decls,
              NEW_VARIANT(
                LilyCheckedDecl,
                type,
                &record->location,
                record,
                NEW_VARIANT(
                  LilyCheckedDeclType,
                  record,
                  NEW(LilyCheckedDeclRecord,
                      record->type.record.name,
                      NULL,
                      NULL,
                      NEW(LilyCheckedScope,
                          NEW_VARIANT(
                            LilyCheckedParent, module, module->scope, module)),
                      record->type.record.visibility))));
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

                LilyCheckedScopeContainerConstant *sc_constant =
                  NEW(LilyCheckedScopeContainerConstant,
                      decl->constant.name,
                      NEW(LilyCheckedAccessConstant, module->access, i));

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
                  NEW(LilyCheckedScopeContainerError,
                      decl->error.name,
                      NEW(LilyCheckedAccessError, module->access, i));

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
                      search_fun__LilyCheckedScope(module->scope,
                                                   decl->fun.name);

                    push_fun__LilyAnalysis(self, decl, module);

                    if (overload_fun) {
                        push__Vec(overload_fun->accesses,
                                  NEW(LilyCheckedAccessFun, module->access, i));
                    } else {
                        add_fun__LilyCheckedScope(
                          module->scope,
                          NEW(LilyCheckedScopeContainerFun,
                              decl->fun.name,
                              init__Vec(
                                1,
                                NEW(LilyCheckedAccessFun, module->access, i))));
                    }
                }

                break;
            }
            case LILY_AST_DECL_KIND_MODULE: {
                push_module__LilyAnalysis(self, decl, module, i);

                LilyCheckedScopeContainerModule *sc_module =
                  NEW(LilyCheckedScopeContainerModule,
                      decl->module.name,
                      NEW(LilyCheckedAccessModule, i));

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
                              NEW(LilyCheckedAccessClass, module->access, i));

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
                              NEW(LilyCheckedAccessEnumObject,
                                  module->access,
                                  i));

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
                                NEW(LilyCheckedAccessRecordObject,
                                    module->access,
                                    i));

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
                              NEW(LilyCheckedAccessTrait, module->access, i));

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
                              NEW(LilyCheckedAccessAlias, module->access, i));

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
                              NEW(LilyCheckedAccessEnum, module->access, i));

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
                              NEW(LilyCheckedAccessRecord, module->access, i));

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
            TODO("binary expression");
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
                                           NULL),
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
                                           NULL),
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
                                           NULL),
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
                                           NULL),
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
                                           NULL),
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
                                           NULL),
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
                                           NULL),
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
                                           NULL),
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
                                  NULL,
                                  NEW(LilyCheckedDataType,
                                      LILY_CHECKED_DATA_TYPE_KIND_UNKNOWN,
                                      NULL)),
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
                                  NULL,
                                  NEW(LilyCheckedDataType,
                                      LILY_CHECKED_DATA_TYPE_KIND_UNKNOWN,
                                      NULL)),
                      expr,
                      NEW(LilyCheckedExprLiteral,
                          LILY_CHECKED_EXPR_LITERAL_KIND_NONE));
                case LILY_AST_EXPR_LITERAL_KIND_STR:
                    return NEW_VARIANT(LilyCheckedExpr,
                                       literal,
                                       &expr->location,
                                       NEW(LilyCheckedDataType,
                                           LILY_CHECKED_DATA_TYPE_KIND_STR,
                                           NULL),
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
                          NULL),
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
                          NULL),
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
                                           NULL),
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
                                           NULL),
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
                                           NULL),
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
                                           NULL),
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
                                           NULL),
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
                                           NULL),
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
                          NULL),
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
                          NULL),
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
                          NULL),
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
                                           NULL),
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
                          NULL),
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
                          NULL),
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
                          NULL),
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
                          NULL),
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

LilyCheckedStmt *
check_stmt__LilyAnalysis(LilyAnalysis *self,
                         LilyAstStmt *stmt,
                         LilyCheckedScope *scope)
{
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
          NEW(LilyCheckedScopeContainerVariable,
              param->name,
              (LilyCheckedAccessScope){
                .module = (LilyCheckedAccessModule){ .id = 0 }, .id = i });

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

    for (Usize i = 0; i < fun->ast_decl->fun.body->len; ++i) {
        LilyAstBodyFunItem *item = get__Vec(fun->ast_decl->fun.body, i);

        switch (item->kind) {
            case LILY_AST_BODY_FUN_ITEM_KIND_EXPR:
                push__Vec(fun->fun.body,
                          NEW_VARIANT(LilyCheckedBodyFunItem,
                                      expr,
                                      check_expr__LilyAnalysis(
                                        self,
                                        item->expr,
                                        fun->fun.scope,
                                        LILY_CHECKED_SAFETY_MODE_SAFE)));

                break;
            case LILY_AST_BODY_FUN_ITEM_KIND_STMT:
                TODO("check statement");
        }
    }

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
    // FREE(LilyCheckedDeclModule, &self->module);
}
