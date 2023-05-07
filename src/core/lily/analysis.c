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

static inline void
push_fun__LilyAnalysis(LilyAnalysis *self,
                       LilyAstDecl *fun,
                       LilyCheckedDeclModule *module);

static inline void
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

static void
run_step1__LilyAnalysis(LilyAnalysis *self);

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
    push__Vec(module->decls,
              NEW_VARIANT(LilyCheckedDecl,
                          fun,
                          &fun->location,
                          fun,
                          NEW(LilyCheckedDeclFun,
                              fun->fun.name,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
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
    push__Vec(
      module->decls,
      NEW_VARIANT(
        LilyCheckedDecl,
        module,
        &module_decl->location,
        module_decl,
        NEW(LilyCheckedDeclModule,
            module_decl->module.name,
            NULL,
            NEW(LilyCheckedScope,
                NEW_VARIANT(LilyCheckedParent, module, module->scope, module)),
            NEW(LilyCheckedAccessModule, i),
            module_decl->module.visibility)));
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
                TODO("analysis step 1");
        }
    }
}

void
run_step1__LilyAnalysis(LilyAnalysis *self)
{
    push_all_delcs__LilyAnalysis(self, self->parser->decls, &self->module);
}

void
run__LilyAnalysis(LilyAnalysis *self)
{
    run_step1__LilyAnalysis(self);

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
