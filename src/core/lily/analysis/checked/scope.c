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
#include <base/format.h>
#include <base/new.h>

#include <core/lily/analysis/checked/body/fun.h>
#include <core/lily/analysis/checked/decl.h>
#include <core/lily/analysis/checked/parent.h>
#include <core/lily/analysis/checked/scope.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static LilyCheckedScopeResponse
search_module_in_current_scope__LilyCheckedScope(LilyCheckedScope *self,
                                                 const String *name);

static LilyCheckedScopeResponse
search_variable_in_current_scope__LilyCheckedScope(LilyCheckedScope *self,
                                                   const String *name);

static LilyCheckedScopeResponse
search_fun_in_current_scope__LilyCheckedScope(LilyCheckedScope *self,
                                              const String *name);

static LilyCheckedScopeResponse
search_constant_in_current_scope__LilyCheckedScope(LilyCheckedScope *self,
                                                   const String *name);

static LilyCheckedScopeResponse
search_error_in_current_scope__LilyCheckedScope(LilyCheckedScope *self,
                                                const String *name);

static LilyCheckedScopeResponse
search_alias_in_current_scope__LilyCheckedScope(LilyCheckedScope *self,
                                                const String *name);

static LilyCheckedScopeResponse
search_record_in_current_scope__LilyCheckedScope(LilyCheckedScope *self,
                                                 const String *name);

static LilyCheckedScopeResponse
search_enum_in_current_scope__LilyCheckedScope(LilyCheckedScope *self,
                                               const String *name);

static LilyCheckedScopeResponse
search_generic_in_current_scope__LilyCheckedScope(LilyCheckedScope *self,
                                                  const String *name);

static LilyCheckedScopeResponse
search_class_in_current_scope__LilyCheckedScope(LilyCheckedScope *self,
                                                const String *name);

static LilyCheckedScopeResponse
search_record_object_in_current_scope__LilyCheckedScope(LilyCheckedScope *self,
                                                        const String *name);

static LilyCheckedScopeResponse
search_enum_object_in_current_scope__LilyCheckedScope(LilyCheckedScope *self,
                                                      const String *name);

static LilyCheckedScopeResponse
search_trait_in_current_scope__LilyCheckedScope(LilyCheckedScope *self,
                                                const String *name);

#define CHECK_IF_EXISTS(container, item, container_name)                    \
    for (Usize i = 0; i < container->len; ++i) {                            \
        if (!strcmp(                                                        \
              CAST(container_name *, get__Vec(container, i))->name->buffer, \
              item->name->buffer)) {                                        \
            return 1;                                                       \
        }                                                                   \
    }

#define ADD_TO_SCOPE(container, item, container_name) \
    CHECK_IF_EXISTS(container, item, container_name); \
                                                      \
    push__Vec(container, item);                       \
                                                      \
    return 0;

CONSTRUCTOR(LilyCheckedScope *,
            LilyCheckedScope,
            LilyCheckedParent *parent,
            LilyCheckedScopeDecls decls)
{
    LilyCheckedScope *self = lily_malloc(sizeof(LilyCheckedScope));

    self->id = parent ? parent->scope->id + 1 : 0;
    self->modules = NEW(Vec);
    self->constants = NEW(Vec);
    self->enums = NEW(Vec);
    self->records = NEW(Vec);
    self->aliases = NEW(Vec);
    self->errors = NEW(Vec);
    self->enums_object = NEW(Vec);
    self->records_object = NEW(Vec);
    self->classes = NEW(Vec);
    self->traits = NEW(Vec);
    self->funs = NEW(Vec);
    self->labels = NEW(Vec);
    self->variables = NEW(Vec);
    self->params = NEW(Vec);
    self->generics = NEW(Vec);
    self->methods = NEW(Vec);
    self->parent = parent;
    self->decls = decls;
    self->drop_table = NEW(LilyCheckedDropTable);

    return self;
}

int
add_module__LilyCheckedScope(LilyCheckedScope *self,
                             LilyCheckedScopeContainerModule *module)
{
    CHECK_IF_EXISTS(self->constants, module, LilyCheckedScopeContainerConstant);
    CHECK_IF_EXISTS(self->funs, module, LilyCheckedScopeContainerFun);
    ADD_TO_SCOPE(self->modules, module, LilyCheckedScopeContainerModule);
}

int
add_constant__LilyCheckedScope(LilyCheckedScope *self,
                               LilyCheckedScopeContainerConstant *constant)
{
    CHECK_IF_EXISTS(self->funs, constant, LilyCheckedScopeContainerFun);
    CHECK_IF_EXISTS(self->modules, constant, LilyCheckedScopeContainerModule);
    ADD_TO_SCOPE(self->constants, constant, LilyCheckedScopeContainerConstant);
}

int
add_enum__LilyCheckedScope(LilyCheckedScope *self,
                           LilyCheckedScopeContainerEnum *enum_)
{
    CHECK_IF_EXISTS(self->records, enum_, LilyCheckedScopeContainerRecord);
    CHECK_IF_EXISTS(self->aliases, enum_, LilyCheckedScopeContainerAlias);
    CHECK_IF_EXISTS(
      self->records_object, enum_, LilyCheckedScopeContainerRecordObject);
    CHECK_IF_EXISTS(
      self->enums_object, enum_, LilyCheckedScopeContainerEnumObject);
    CHECK_IF_EXISTS(self->classes, enum_, LilyCheckedScopeContainerClass);
    CHECK_IF_EXISTS(self->traits, enum_, LilyCheckedScopeContainerTrait);
    ADD_TO_SCOPE(self->enums, enum_, LilyCheckedScopeContainerEnum);
}

int
add_record__LilyCheckedScope(LilyCheckedScope *self,
                             LilyCheckedScopeContainerRecord *record)
{
    CHECK_IF_EXISTS(self->enums, record, LilyCheckedScopeContainerEnum);
    CHECK_IF_EXISTS(self->aliases, record, LilyCheckedScopeContainerAlias);
    CHECK_IF_EXISTS(
      self->records_object, record, LilyCheckedScopeContainerRecordObject);
    CHECK_IF_EXISTS(
      self->enums_object, record, LilyCheckedScopeContainerEnumObject);
    CHECK_IF_EXISTS(self->classes, record, LilyCheckedScopeContainerClass);
    CHECK_IF_EXISTS(self->traits, record, LilyCheckedScopeContainerTrait);
    ADD_TO_SCOPE(self->records, record, LilyCheckedScopeContainerRecord);
}

int
add_alias__LilyCheckedScope(LilyCheckedScope *self,
                            LilyCheckedScopeContainerAlias *alias)
{
    CHECK_IF_EXISTS(self->enums, alias, LilyCheckedScopeContainerEnum);
    CHECK_IF_EXISTS(self->records, alias, LilyCheckedScopeContainerRecord);
    CHECK_IF_EXISTS(
      self->records_object, alias, LilyCheckedScopeContainerRecordObject);
    CHECK_IF_EXISTS(
      self->enums_object, alias, LilyCheckedScopeContainerEnumObject);
    CHECK_IF_EXISTS(self->classes, alias, LilyCheckedScopeContainerClass);
    CHECK_IF_EXISTS(self->traits, alias, LilyCheckedScopeContainerTrait);
    ADD_TO_SCOPE(self->aliases, alias, LilyCheckedScopeContainerAlias);
}

int
add_error__LilyCheckedScope(LilyCheckedScope *self,
                            LilyCheckedScopeContainerError *error)
{
    ADD_TO_SCOPE(self->errors, error, LilyCheckedScopeContainerError);
}

int
add_enum_object__LilyCheckedScope(
  LilyCheckedScope *self,
  LilyCheckedScopeContainerEnumObject *enum_object)
{
    CHECK_IF_EXISTS(self->enums, enum_object, LilyCheckedScopeContainerEnum);
    CHECK_IF_EXISTS(
      self->records, enum_object, LilyCheckedScopeContainerRecord);
    CHECK_IF_EXISTS(
      self->records_object, enum_object, LilyCheckedScopeContainerRecordObject);
    CHECK_IF_EXISTS(self->aliases, enum_object, LilyCheckedScopeContainerAlias);
    CHECK_IF_EXISTS(self->classes, enum_object, LilyCheckedScopeContainerClass);
    CHECK_IF_EXISTS(self->traits, enum_object, LilyCheckedScopeContainerTrait);
    ADD_TO_SCOPE(
      self->enums_object, enum_object, LilyCheckedScopeContainerEnumObject);
}

int
add_record_object__LilyCheckedScope(
  LilyCheckedScope *self,
  LilyCheckedScopeContainerRecordObject *record_object)
{
    CHECK_IF_EXISTS(self->enums, record_object, LilyCheckedScopeContainerEnum);
    CHECK_IF_EXISTS(
      self->records, record_object, LilyCheckedScopeContainerRecord);
    CHECK_IF_EXISTS(
      self->aliases, record_object, LilyCheckedScopeContainerAlias);
    CHECK_IF_EXISTS(
      self->enums_object, record_object, LilyCheckedScopeContainerEnumObject);
    CHECK_IF_EXISTS(
      self->classes, record_object, LilyCheckedScopeContainerClass);
    CHECK_IF_EXISTS(
      self->traits, record_object, LilyCheckedScopeContainerTrait);
    ADD_TO_SCOPE(self->records_object,
                 record_object,
                 LilyCheckedScopeContainerRecordObject);
}

int
add_class__LilyCheckedScope(LilyCheckedScope *self,
                            LilyCheckedScopeContainerClass *class)
{
    CHECK_IF_EXISTS(self->enums, class, LilyCheckedScopeContainerEnum);
    CHECK_IF_EXISTS(self->records, class, LilyCheckedScopeContainerRecord);
    CHECK_IF_EXISTS(
      self->records_object, class, LilyCheckedScopeContainerRecordObject);
    CHECK_IF_EXISTS(
      self->enums_object, class, LilyCheckedScopeContainerEnumObject);
    CHECK_IF_EXISTS(self->aliases, class, LilyCheckedScopeContainerAlias);
    CHECK_IF_EXISTS(self->traits, class, LilyCheckedScopeContainerTrait);
    ADD_TO_SCOPE(self->classes, class, LilyCheckedScopeContainerClass);
}

int
add_trait__LilyCheckedScope(LilyCheckedScope *self,
                            LilyCheckedScopeContainerTrait *trait)
{
    CHECK_IF_EXISTS(self->enums, trait, LilyCheckedScopeContainerEnum);
    CHECK_IF_EXISTS(self->records, trait, LilyCheckedScopeContainerRecord);
    CHECK_IF_EXISTS(
      self->records_object, trait, LilyCheckedScopeContainerRecordObject);
    CHECK_IF_EXISTS(
      self->enums_object, trait, LilyCheckedScopeContainerEnumObject);
    CHECK_IF_EXISTS(self->classes, trait, LilyCheckedScopeContainerClass);
    CHECK_IF_EXISTS(self->aliases, trait, LilyCheckedScopeContainerAlias);
    ADD_TO_SCOPE(self->traits, trait, LilyCheckedScopeContainerTrait);
}

int
add_fun__LilyCheckedScope(LilyCheckedScope *self,
                          LilyCheckedScopeContainerFun *fun)
{
    CHECK_IF_EXISTS(self->variables, fun, LilyCheckedScopeContainerVariable);
    CHECK_IF_EXISTS(self->modules, fun, LilyCheckedScopeContainerModule);
    ADD_TO_SCOPE(self->funs, fun, LilyCheckedScopeContainerFun);
}

int
add_label__LilyCheckedScope(LilyCheckedScope *self,
                            LilyCheckedScopeContainerLabel *label)
{
    CHECK_IF_EXISTS(self->variables, label, LilyCheckedScopeContainerVariable);
    ADD_TO_SCOPE(self->labels, label, LilyCheckedScopeContainerLabel);
}

int
add_variable__LilyCheckedScope(LilyCheckedScope *self,
                               LilyCheckedScopeContainerVariable *variable)
{
    CHECK_IF_EXISTS(self->labels, variable, LilyCheckedScopeContainerLabel);
    ADD_TO_SCOPE(self->variables, variable, LilyCheckedScopeContainerVariable);
}

int
add_param__LilyCheckedScope(LilyCheckedScope *self,
                            LilyCheckedScopeContainerVariable *param)
{
    ADD_TO_SCOPE(self->params, param, LilyCheckedScopeContainerVariable);
}

int
add_generic__LilyCheckedScope(LilyCheckedScope *self,
                              LilyCheckedScopeContainerGeneric *generic)
{
    ADD_TO_SCOPE(self->generics, generic, LilyCheckedScopeContainerGeneric);
}

LilyCheckedScopeContainerFun *
get_fun_in_current_scope__LilyCheckedScope(LilyCheckedScope *self,
                                           const String *name)
{
    for (Usize i = 0; i < self->funs->len; ++i) {
        LilyCheckedScopeContainerFun *fun = get__Vec(self->funs, i);

        if (!strcmp(fun->name->buffer, name->buffer)) {
            return fun;
        }
    }

    return NULL;
}

LilyCheckedScopeResponse
search_module_in_current_scope__LilyCheckedScope(LilyCheckedScope *self,
                                                 const String *name)
{
    switch (self->decls.kind) {
        case LILY_CHECKED_SCOPE_DECLS_KIND_MODULE:
            for (Usize i = 0; i < self->modules->len; ++i) {
                LilyCheckedScopeContainerModule *module =
                  get__Vec(self->modules, i);

                if (!strcmp(module->name->buffer, name->buffer)) {
                    LilyCheckedDecl *m =
                      get__Vec(self->decls.module->decls, module->id);

                    return NEW_VARIANT(
                      LilyCheckedScopeResponse,
                      module,
                      m->location,
                      NEW_VARIANT(
                        LilyCheckedScopeContainer, module, self->id, module),
                      &m->module);
                }
            }

            return NEW(LilyCheckedScopeResponse);
        default:
            return NEW(LilyCheckedScopeResponse);
    }
}

LilyCheckedScopeResponse
search_variable_in_current_scope__LilyCheckedScope(LilyCheckedScope *self,
                                                   const String *name)
{
    switch (self->decls.kind) {
        case LILY_CHECKED_SCOPE_DECLS_KIND_SCOPE:
            for (Usize i = 0; i < self->variables->len; ++i) {
                LilyCheckedScopeContainerVariable *variable =
                  get__Vec(self->variables, i);

                if (!strcmp(variable->name->buffer, name->buffer)) {
                    LilyCheckedBodyFunItem *item =
                      get__Vec(self->decls.scope, variable->id);

                    return NEW_VARIANT(LilyCheckedScopeResponse,
                                       variable,
                                       item->stmt.location,
                                       NEW_VARIANT(LilyCheckedScopeContainer,
                                                   variable,
                                                   self->id,
                                                   variable),
                                       &item->stmt.variable);
                }
            }

            return NEW(LilyCheckedScopeResponse);
        case LILY_CHECKED_SCOPE_DECLS_KIND_DECL:
            if (self->decls.decl->kind == LILY_CHECKED_DECL_KIND_FUN) {
                for (Usize i = 0; i < self->params->len; ++i) {
                    LilyCheckedScopeContainerVariable *variable =
                      get__Vec(self->params, i);

                    if (!strcmp(variable->name->buffer, name->buffer)) {
                        LilyCheckedDeclFunParam *param =
                          get__Vec(self->decls.decl->fun.params, variable->id);

                        return NEW_VARIANT(
                          LilyCheckedScopeResponse,
                          fun_param,
                          param->location,
                          NEW_VARIANT(LilyCheckedScopeContainer,
                                      variable,
                                      self->id,
                                      variable),
                          param);
                    }
                }
            } else if (self->decls.decl->kind == LILY_CHECKED_DECL_KIND_TYPE) {
                switch (self->decls.decl->type.kind) {
                    case LILY_CHECKED_DECL_TYPE_KIND_RECORD:
                        for (Usize i = 0; i < self->variables->len; ++i) {
                            LilyCheckedScopeContainerVariable *variable =
                              get__Vec(self->variables, i);

                            if (!strcmp(variable->name->buffer, name->buffer)) {
                                LilyCheckedField *field =
                                  get__Vec(self->decls.decl->type.record.fields,
                                           variable->id);

                                return NEW_VARIANT(
                                  LilyCheckedScopeResponse,
                                  record_field,
                                  field->location,
                                  NEW_VARIANT(LilyCheckedScopeContainer,
                                              variable,
                                              self->id,
                                              variable),
                                  field);
                            }
                        }

                        break;
                    default:
                        break;
                }
            }

            return NEW(LilyCheckedScopeResponse);
        default:
            return NEW(LilyCheckedScopeResponse);
    }
}

LilyCheckedScopeResponse
search_fun_in_current_scope__LilyCheckedScope(LilyCheckedScope *self,
                                              const String *name)
{
    switch (self->decls.kind) {
        case LILY_CHECKED_SCOPE_DECLS_KIND_MODULE:
            for (Usize i = 0; i < self->funs->len; ++i) {
                LilyCheckedScopeContainerFun *fun = get__Vec(self->funs, i);

                if (!strcmp(fun->name->buffer, name->buffer)) {
                    Vec *f = NEW(Vec);

                    for (Usize j = 0; j < fun->ids->len; ++j) {
                        push__Vec(f,
                                  get__Vec(self->decls.module->decls,
                                           (Usize)(Uptr)(Usize *)get__Vec(
                                             fun->ids, j)));
                    }

                    return NEW_VARIANT(
                      LilyCheckedScopeResponse,
                      fun,
                      NULL,
                      NEW_VARIANT(
                        LilyCheckedScopeContainer, fun, self->id, fun),
                      f);
                }
            }

            return NEW(LilyCheckedScopeResponse);
        default:
            return NEW(LilyCheckedScopeResponse);
    }
}

LilyCheckedScopeResponse
search_constant_in_current_scope__LilyCheckedScope(LilyCheckedScope *self,
                                                   const String *name)
{
    switch (self->decls.kind) {
        case LILY_CHECKED_SCOPE_DECLS_KIND_MODULE:
            for (Usize i = 0; i < self->constants->len; ++i) {
                LilyCheckedScopeContainerConstant *constant =
                  get__Vec(self->constants, i);

                if (!strcmp(constant->name->buffer, name->buffer)) {
                    LilyCheckedDecl *c =
                      get__Vec(self->decls.module->decls, constant->id);

                    return NEW_VARIANT(LilyCheckedScopeResponse,
                                       constant,
                                       c->location,
                                       NEW_VARIANT(LilyCheckedScopeContainer,
                                                   constant,
                                                   self->id,
                                                   constant),
                                       &c->constant);
                }
            }

            return NEW(LilyCheckedScopeResponse);
        default:
            return NEW(LilyCheckedScopeResponse);
    }
}

LilyCheckedScopeResponse
search_error_in_current_scope__LilyCheckedScope(LilyCheckedScope *self,
                                                const String *name)
{
    switch (self->decls.kind) {
        case LILY_CHECKED_SCOPE_DECLS_KIND_MODULE:
            for (Usize i = 0; i < self->constants->len; ++i) {
                LilyCheckedScopeContainerError *error =
                  get__Vec(self->errors, i);

                if (!strcmp(error->name->buffer, name->buffer)) {
                    LilyCheckedDecl *e =
                      get__Vec(self->decls.module->decls, error->id);

                    return NEW_VARIANT(
                      LilyCheckedScopeResponse,
                      error,
                      e->location,
                      NEW_VARIANT(
                        LilyCheckedScopeContainer, error, self->id, error),
                      e,
                      &e->error);
                }
            }

            return NEW(LilyCheckedScopeResponse);
        default:
            return NEW(LilyCheckedScopeResponse);
    }
}

LilyCheckedScopeResponse
search_alias_in_current_scope__LilyCheckedScope(LilyCheckedScope *self,
                                                const String *name)
{
    switch (self->decls.kind) {
        case LILY_CHECKED_SCOPE_DECLS_KIND_MODULE:
            for (Usize i = 0; i < self->aliases->len; ++i) {
                LilyCheckedScopeContainerAlias *alias =
                  get__Vec(self->aliases, i);

                if (!strcmp(alias->name->buffer, name->buffer)) {
                    LilyCheckedDecl *a =
                      get__Vec(self->decls.module->decls, alias->id);

                    return NEW_VARIANT(
                      LilyCheckedScopeResponse,
                      alias,
                      a->location,
                      NEW_VARIANT(
                        LilyCheckedScopeContainer, alias, self->id, alias),
                      a,
                      &a->type.alias);
                }
            }

            return NEW(LilyCheckedScopeResponse);
        default:
            return NEW(LilyCheckedScopeResponse);
    }
}

LilyCheckedScopeResponse
search_record_in_current_scope__LilyCheckedScope(LilyCheckedScope *self,
                                                 const String *name)
{
    switch (self->decls.kind) {
        case LILY_CHECKED_SCOPE_DECLS_KIND_MODULE:
            for (Usize i = 0; i < self->records->len; ++i) {
                LilyCheckedScopeContainerRecord *record =
                  get__Vec(self->records, i);

                if (!strcmp(record->name->buffer, name->buffer)) {
                    LilyCheckedDecl *r =
                      get__Vec(self->decls.module->decls, record->id);

                    return NEW_VARIANT(
                      LilyCheckedScopeResponse,
                      record,
                      r->location,
                      NEW_VARIANT(
                        LilyCheckedScopeContainer, record, self->id, record),
                      r,
                      &r->type.record);
                }
            }

            return NEW(LilyCheckedScopeResponse);
        default:
            return NEW(LilyCheckedScopeResponse);
    }
}

LilyCheckedScopeResponse
search_enum_in_current_scope__LilyCheckedScope(LilyCheckedScope *self,
                                               const String *name)
{
    switch (self->decls.kind) {
        case LILY_CHECKED_SCOPE_DECLS_KIND_MODULE:
            for (Usize i = 0; i < self->enums->len; ++i) {
                LilyCheckedScopeContainerEnum *enum_ = get__Vec(self->enums, i);

                if (!strcmp(enum_->name->buffer, name->buffer)) {
                    LilyCheckedDecl *e =
                      get__Vec(self->decls.module->decls, enum_->id);

                    return NEW_VARIANT(
                      LilyCheckedScopeResponse,
                      enum_,
                      e->location,
                      NEW_VARIANT(
                        LilyCheckedScopeContainer, enum_, self->id, enum_),
                      e,
                      &e->type.enum_);
                }
            }

            return NEW(LilyCheckedScopeResponse);
        default:
            return NEW(LilyCheckedScopeResponse);
    }
}

LilyCheckedScopeResponse
search_generic_in_current_scope__LilyCheckedScope(LilyCheckedScope *self,
                                                  const String *name)
{
    switch (self->decls.kind) {
        case LILY_CHECKED_SCOPE_DECLS_KIND_DECL:
            for (Usize i = 0; i < self->generics->len; ++i) {
                LilyCheckedScopeContainerGeneric *generic =
                  get__Vec(self->generics, i);

                if (!strcmp(generic->name->buffer, name->buffer)) {
                    LilyCheckedGenericParam *g = NULL;

                    switch (self->decls.decl->kind) {
                        case LILY_CHECKED_DECL_KIND_FUN:
                            ASSERT(self->decls.decl->fun.generic_params);
                            ASSERT(generic->id <
                                   self->decls.decl->fun.generic_params->len);

                            g = get__Vec(self->decls.decl->fun.generic_params,
                                         generic->id);

                            break;
                        case LILY_CHECKED_DECL_KIND_ERROR:
                            ASSERT(self->decls.decl->error.generic_params);
                            ASSERT(generic->id <
                                   self->decls.decl->error.generic_params->len);

                            g = get__Vec(self->decls.decl->error.generic_params,
                                         generic->id);

                            break;
                        case LILY_CHECKED_DECL_KIND_METHOD:
                            ASSERT(self->decls.decl->method.generic_params);
                            ASSERT(
                              generic->id <
                              self->decls.decl->method.generic_params->len);

                            g =
                              get__Vec(self->decls.decl->method.generic_params,
                                       generic->id);

                            break;
                        case LILY_CHECKED_DECL_KIND_OBJECT:
                            switch (self->decls.decl->object.kind) {
                                case LILY_CHECKED_DECL_OBJECT_KIND_CLASS:
                                    ASSERT(self->decls.decl->object
                                             .class.generic_params);
                                    ASSERT(generic->id <
                                           self->decls.decl->object
                                             .class.generic_params->len);

                                    g = get__Vec(self->decls.decl->object
                                                   .class.generic_params,
                                                 generic->id);

                                    break;
                                case LILY_CHECKED_DECL_OBJECT_KIND_ENUM:
                                    ASSERT(self->decls.decl->object.enum_
                                             .generic_params);
                                    ASSERT(generic->id <
                                           self->decls.decl->object.enum_
                                             .generic_params->len);

                                    g = get__Vec(self->decls.decl->object.enum_
                                                   .generic_params,
                                                 generic->id);

                                    break;
                                case LILY_CHECKED_DECL_OBJECT_KIND_RECORD:
                                    ASSERT(self->decls.decl->object.record
                                             .generic_params);
                                    ASSERT(generic->id <
                                           self->decls.decl->object.record
                                             .generic_params->len);

                                    g = get__Vec(self->decls.decl->object.record
                                                   .generic_params,
                                                 generic->id);

                                    break;
                                case LILY_CHECKED_DECL_OBJECT_KIND_TRAIT:
                                    ASSERT(self->decls.decl->object.trait
                                             .generic_params);
                                    ASSERT(generic->id <
                                           self->decls.decl->object.trait
                                             .generic_params->len);

                                    g = get__Vec(self->decls.decl->object.trait
                                                   .generic_params,
                                                 generic->id);

                                    break;
                                default:
                                    UNREACHABLE("unknown variant");
                            }

                            break;
                        case LILY_CHECKED_DECL_KIND_TYPE:
                            switch (self->decls.decl->type.kind) {
                                case LILY_CHECKED_DECL_TYPE_KIND_ALIAS:
                                    ASSERT(self->decls.decl->type.alias
                                             .generic_params);
                                    ASSERT(generic->id <
                                           self->decls.decl->type.alias
                                             .generic_params->len);

                                    g = get__Vec(self->decls.decl->type.alias
                                                   .generic_params,
                                                 generic->id);

                                    break;
                                case LILY_CHECKED_DECL_TYPE_KIND_ENUM:
                                    ASSERT(self->decls.decl->type.enum_
                                             .generic_params);
                                    ASSERT(generic->id <
                                           self->decls.decl->type.enum_
                                             .generic_params->len);

                                    g = get__Vec(self->decls.decl->type.enum_
                                                   .generic_params,
                                                 generic->id);

                                    break;
                                case LILY_CHECKED_DECL_TYPE_KIND_RECORD:
                                    ASSERT(self->decls.decl->type.record
                                             .generic_params);
                                    ASSERT(generic->id <
                                           self->decls.decl->type.record
                                             .generic_params->len);

                                    g = get__Vec(self->decls.decl->type.record
                                                   .generic_params,
                                                 generic->id);

                                    break;
                                default:
                                    UNREACHABLE("unknown variant");
                            }

                            break;
                        default:
                            UNREACHABLE("generic params are not expected in "
                                        "this context");
                    }

                    return NEW_VARIANT(
                      LilyCheckedScopeResponse,
                      generic,
                      g->location,
                      NEW_VARIANT(
                        LilyCheckedScopeContainer, generic, self->id, generic),
                      g);
                }
            }

            return NEW(LilyCheckedScopeResponse);
        default:
            return NEW(LilyCheckedScopeResponse);
    }
}

LilyCheckedScopeResponse
search_class_in_current_scope__LilyCheckedScope(LilyCheckedScope *self,
                                                const String *name)
{
    switch (self->decls.kind) {
        case LILY_CHECKED_SCOPE_DECLS_KIND_MODULE:
            for (Usize i = 0; i < self->classes->len; ++i) {
                LilyCheckedScopeContainerClass *class =
                  get__Vec(self->classes, i);

                if (!strcmp(class->name->buffer, name->buffer)) {
                    LilyCheckedDecl *c =
                      get__Vec(self->decls.module->decls, class->id);

                    return NEW_VARIANT(
                      LilyCheckedScopeResponse,
                      class,
                      c->location,
                      NEW_VARIANT(
                        LilyCheckedScopeContainer, class, self->id, class),
                      c,
                      &c->object.class);
                }
            }

            return NEW(LilyCheckedScopeResponse);
        default:
            return NEW(LilyCheckedScopeResponse);
    }
}

LilyCheckedScopeResponse
search_record_object_in_current_scope__LilyCheckedScope(LilyCheckedScope *self,
                                                        const String *name)
{
    switch (self->decls.kind) {
        case LILY_CHECKED_SCOPE_DECLS_KIND_MODULE:
            for (Usize i = 0; i < self->records_object->len; ++i) {
                LilyCheckedScopeContainerRecordObject *record_object =
                  get__Vec(self->records_object, i);

                if (!strcmp(record_object->name->buffer, name->buffer)) {
                    LilyCheckedDecl *r =
                      get__Vec(self->decls.module->decls, record_object->id);

                    return NEW_VARIANT(LilyCheckedScopeResponse,
                                       record_object,
                                       r->location,
                                       NEW_VARIANT(LilyCheckedScopeContainer,
                                                   record_object,
                                                   self->id,
                                                   record_object),
                                       r,
                                       &r->object.record);
                }
            }

            return NEW(LilyCheckedScopeResponse);
        default:
            return NEW(LilyCheckedScopeResponse);
    }
}

LilyCheckedScopeResponse
search_enum_object_in_current_scope__LilyCheckedScope(LilyCheckedScope *self,
                                                      const String *name)
{
    switch (self->decls.kind) {
        case LILY_CHECKED_SCOPE_DECLS_KIND_MODULE:
            for (Usize i = 0; i < self->enums_object->len; ++i) {
                LilyCheckedScopeContainerEnumObject *enum_object =
                  get__Vec(self->enums_object, i);

                if (!strcmp(enum_object->name->buffer, name->buffer)) {
                    LilyCheckedDecl *e =
                      get__Vec(self->decls.module->decls, enum_object->id);

                    return NEW_VARIANT(LilyCheckedScopeResponse,
                                       enum_object,
                                       e->location,
                                       NEW_VARIANT(LilyCheckedScopeContainer,
                                                   enum_object,
                                                   self->id,
                                                   enum_object),
                                       e,
                                       &e->object.enum_);
                }
            }

            return NEW(LilyCheckedScopeResponse);
        default:
            return NEW(LilyCheckedScopeResponse);
    }
}

LilyCheckedScopeResponse
search_trait_in_current_scope__LilyCheckedScope(LilyCheckedScope *self,
                                                const String *name)
{
    switch (self->decls.kind) {
        case LILY_CHECKED_SCOPE_DECLS_KIND_MODULE:
            for (Usize i = 0; i < self->traits->len; ++i) {
                LilyCheckedScopeContainerTrait *trait =
                  get__Vec(self->traits, i);

                if (!strcmp(trait->name->buffer, name->buffer)) {
                    LilyCheckedDecl *t =
                      get__Vec(self->decls.module->decls, trait->id);

                    return NEW_VARIANT(
                      LilyCheckedScopeResponse,
                      trait,
                      t->location,
                      NEW_VARIANT(
                        LilyCheckedScopeContainer, trait, self->id, trait),
                      t,
                      &t->object.trait);
                }
            }

            return NEW(LilyCheckedScopeResponse);
        default:
            return NEW(LilyCheckedScopeResponse);
    }
}

LilyCheckedScopeResponse
search_module__LilyCheckedScope(LilyCheckedScope *self, const String *name)
{
    LilyCheckedScopeResponse response =
      search_module_in_current_scope__LilyCheckedScope(self, name);

    switch (response.kind) {
        case LILY_CHECKED_SCOPE_RESPONSE_KIND_NOT_FOUND:
            if (self->parent) {
                return search_module__LilyCheckedScope(self->parent->scope,
                                                       name);
            }

            return NEW(LilyCheckedScopeResponse);
        default:
            return response;
    }
}

LilyCheckedScopeResponse
search_variable__LilyCheckedScope(LilyCheckedScope *self, const String *name)
{
    LilyCheckedScopeResponse response =
      search_variable_in_current_scope__LilyCheckedScope(self, name);

    switch (response.kind) {
        case LILY_CHECKED_SCOPE_RESPONSE_KIND_NOT_FOUND:
            if (self->parent && (self->parent->scope->decls.kind ==
                                   LILY_CHECKED_SCOPE_DECLS_KIND_SCOPE ||
                                 self->parent->scope->decls.kind ==
                                   LILY_CHECKED_SCOPE_DECLS_KIND_DECL)) {
                return search_variable__LilyCheckedScope(self->parent->scope,
                                                         name);
            }

            return NEW(LilyCheckedScopeResponse);
        default:
            return response;
    }
}

LilyCheckedScopeResponse
search_fun__LilyCheckedScope(LilyCheckedScope *self, const String *name)
{
    LilyCheckedScopeResponse response =
      search_fun_in_current_scope__LilyCheckedScope(self, name);

    switch (response.kind) {
        case LILY_CHECKED_SCOPE_RESPONSE_KIND_NOT_FOUND:
            if (self->parent) {
                return search_fun__LilyCheckedScope(self->parent->scope, name);
            }

            return NEW(LilyCheckedScopeResponse);
        default:
            return response;
    }
}

LilyCheckedScopeResponse
search_constant__LilyCheckedScope(LilyCheckedScope *self, const String *name)
{
    LilyCheckedScopeResponse response =
      search_constant_in_current_scope__LilyCheckedScope(self, name);

    switch (response.kind) {
        case LILY_CHECKED_SCOPE_RESPONSE_KIND_NOT_FOUND:
            if (self->parent) {
                return search_constant__LilyCheckedScope(self->parent->scope,
                                                         name);
            }

            return NEW(LilyCheckedScopeResponse);
        default:
            return response;
    }
}

LilyCheckedScopeResponse
search_error__LilyCheckedScope(LilyCheckedScope *self, const String *name)
{
    LilyCheckedScopeResponse response =
      search_error_in_current_scope__LilyCheckedScope(self, name);

    switch (response.kind) {
        case LILY_CHECKED_SCOPE_RESPONSE_KIND_NOT_FOUND:
            if (self->parent) {
                return search_error__LilyCheckedScope(self->parent->scope,
                                                      name);
            }

            return NEW(LilyCheckedScopeResponse);
        default:
            return response;
    }
}

LilyCheckedScopeResponse
search_alias__LilyCheckedScope(LilyCheckedScope *self, const String *name)
{
    LilyCheckedScopeResponse response =
      search_alias_in_current_scope__LilyCheckedScope(self, name);

    switch (response.kind) {
        case LILY_CHECKED_SCOPE_RESPONSE_KIND_NOT_FOUND:
            if (self->parent) {
                return search_alias__LilyCheckedScope(self->parent->scope,
                                                      name);
            }

            return NEW(LilyCheckedScopeResponse);
        default:
            return response;
    }
}

LilyCheckedScopeResponse
search_record__LilyCheckedScope(LilyCheckedScope *self, const String *name)
{
    LilyCheckedScopeResponse response =
      search_record_in_current_scope__LilyCheckedScope(self, name);

    switch (response.kind) {
        case LILY_CHECKED_SCOPE_RESPONSE_KIND_NOT_FOUND:
            if (self->parent) {
                return search_record__LilyCheckedScope(self->parent->scope,
                                                       name);
            }

            return NEW(LilyCheckedScopeResponse);
        default:
            return response;
    }
}

LilyCheckedScopeResponse
search_enum__LilyCheckedScope(LilyCheckedScope *self, const String *name)
{
    LilyCheckedScopeResponse response =
      search_enum_in_current_scope__LilyCheckedScope(self, name);

    switch (response.kind) {
        case LILY_CHECKED_SCOPE_RESPONSE_KIND_NOT_FOUND:
            if (self->parent) {
                return search_enum__LilyCheckedScope(self->parent->scope, name);
            }

            return NEW(LilyCheckedScopeResponse);
        default:
            return response;
    }
}

LilyCheckedScopeResponse
search_generic__LilyCheckedScope(LilyCheckedScope *self, const String *name)
{
    LilyCheckedScopeResponse response =
      search_generic_in_current_scope__LilyCheckedScope(self, name);

    switch (response.kind) {
        case LILY_CHECKED_SCOPE_RESPONSE_KIND_NOT_FOUND:
            if (self->parent) {
                return search_generic__LilyCheckedScope(self->parent->scope,
                                                        name);
            }

            return NEW(LilyCheckedScopeResponse);
        default:
            return response;
    }
}

LilyCheckedScopeResponse
search_class__LilyCheckedScope(LilyCheckedScope *self, const String *name)
{
    LilyCheckedScopeResponse response =
      search_class_in_current_scope__LilyCheckedScope(self, name);

    switch (response.kind) {
        case LILY_CHECKED_SCOPE_RESPONSE_KIND_NOT_FOUND:
            if (self->parent) {
                return search_class__LilyCheckedScope(self->parent->scope,
                                                      name);
            }

            return NEW(LilyCheckedScopeResponse);
        default:
            return response;
    }
}

LilyCheckedScopeResponse
search_record_object__LilyCheckedScope(LilyCheckedScope *self,
                                       const String *name)
{
    LilyCheckedScopeResponse response =
      search_record_object_in_current_scope__LilyCheckedScope(self, name);

    switch (response.kind) {
        case LILY_CHECKED_SCOPE_RESPONSE_KIND_NOT_FOUND:

            if (self->parent) {
                return search_record_object__LilyCheckedScope(
                  self->parent->scope, name);
            }

            return NEW(LilyCheckedScopeResponse);
        default:
            return response;
    }
}

LilyCheckedScopeResponse
search_enum_object__LilyCheckedScope(LilyCheckedScope *self, const String *name)
{
    LilyCheckedScopeResponse response =
      search_enum_object_in_current_scope__LilyCheckedScope(self, name);

    switch (response.kind) {
        case LILY_CHECKED_SCOPE_RESPONSE_KIND_NOT_FOUND:

            if (self->parent) {
                return search_enum_object__LilyCheckedScope(self->parent->scope,
                                                            name);
            }

            return NEW(LilyCheckedScopeResponse);
        default:
            return response;
    }
}

LilyCheckedScopeResponse
search_trait__LilyCheckedScope(LilyCheckedScope *self, const String *name)
{
    LilyCheckedScopeResponse response =
      search_trait_in_current_scope__LilyCheckedScope(self, name);

    switch (response.kind) {
        case LILY_CHECKED_SCOPE_RESPONSE_KIND_NOT_FOUND:
            if (self->parent) {
                return search_trait__LilyCheckedScope(self->parent->scope,
                                                      name);
            }

            return NEW(LilyCheckedScopeResponse);
        default:
            return response;
    }
}

LilyCheckedScopeResponse
search_field__LilyCheckedScope(LilyCheckedScope *self, const String *name)
{
    switch (self->decls.kind) {
        case LILY_CHECKED_SCOPE_DECLS_KIND_DECL:
            if (self->decls.decl->kind == LILY_CHECKED_DECL_KIND_TYPE) {
                switch (self->decls.decl->type.kind) {
                    case LILY_CHECKED_DECL_TYPE_KIND_RECORD:
                        for (Usize i = 0; i < self->variables->len; ++i) {
                            LilyCheckedScopeContainerVariable *variable =
                              get__Vec(self->variables, i);

                            if (!strcmp(variable->name->buffer, name->buffer)) {
                                LilyCheckedField *field =
                                  get__Vec(self->decls.decl->type.record.fields,
                                           variable->id);

                                return NEW_VARIANT(
                                  LilyCheckedScopeResponse,
                                  record_field,
                                  field->location,
                                  NEW_VARIANT(LilyCheckedScopeContainer,
                                              variable,
                                              self->id,
                                              variable),
                                  field);
                            }
                        }

                        break;
                    default:
                        break;
                }
            } else if (self->decls.decl->kind ==
                       LILY_CHECKED_DECL_KIND_OBJECT) {
                switch (self->decls.decl->object.kind) {
                    case LILY_CHECKED_DECL_OBJECT_KIND_RECORD:
                        for (Usize i = 0; i < self->variables->len; ++i) {
                            LilyCheckedScopeContainerVariable *variable =
                              get__Vec(self->variables, i);

                            if (!strcmp(variable->name->buffer, name->buffer)) {
                                LilyCheckedBodyRecordObjectItem *field =
                                  get__Vec(self->decls.decl->object.record.body,
                                           variable->id);

                                ASSERT(
                                  field->kind ==
                                  LILY_CHECKED_BODY_RECORD_OBJECT_ITEM_KIND_FIELD);

                                return NEW_VARIANT(
                                  LilyCheckedScopeResponse,
                                  record_field_object,
                                  field->location,
                                  NEW_VARIANT(LilyCheckedScopeContainer,
                                              variable,
                                              self->id,
                                              variable),
                                  &field->field);
                            }
                        }

                        break;
                    default:
                        break;
                }
            }

            return NEW(LilyCheckedScopeResponse);
        default:
            return NEW(LilyCheckedScopeResponse);
    }
}

LilyCheckedScopeResponse
search_variant__LilyCheckedScope(LilyCheckedScope *self, const String *name)
{
    switch (self->decls.kind) {
        case LILY_CHECKED_SCOPE_DECLS_KIND_DECL:
            if (self->decls.decl->kind == LILY_CHECKED_DECL_KIND_TYPE) {
                switch (self->decls.decl->type.kind) {
                    case LILY_CHECKED_DECL_TYPE_KIND_ENUM:
                        for (Usize i = 0; i < self->variables->len; ++i) {
                            LilyCheckedScopeContainerVariable *variable =
                              get__Vec(self->variables, i);

                            if (!strcmp(variable->name->buffer, name->buffer)) {
                                LilyCheckedVariant *enum_variant = get__Vec(
                                  self->decls.decl->type.enum_.variants,
                                  variable->id);

                                return NEW_VARIANT(
                                  LilyCheckedScopeResponse,
                                  enum_variant,
                                  enum_variant->location,
                                  NEW_VARIANT(LilyCheckedScopeContainer,
                                              variable,
                                              self->id,
                                              variable),
                                  enum_variant);
                            }
                        }

                        break;
                    default:
                        break;
                }
            } else if (self->decls.decl->kind ==
                       LILY_CHECKED_DECL_KIND_OBJECT) {
                switch (self->decls.decl->object.kind) {
                    case LILY_CHECKED_DECL_OBJECT_KIND_ENUM:
                        for (Usize i = 0; i < self->variables->len; ++i) {
                            LilyCheckedScopeContainerVariable *variable =
                              get__Vec(self->variables, i);

                            if (!strcmp(variable->name->buffer, name->buffer)) {
                                LilyCheckedBodyEnumObjectItem *variant =
                                  get__Vec(self->decls.decl->object.enum_.body,
                                           variable->id);

                                ASSERT(
                                  variant->kind ==
                                  LILY_CHECKED_BODY_ENUM_OBJECT_ITEM_KIND_VARIANT);

                                return NEW_VARIANT(
                                  LilyCheckedScopeResponse,
                                  enum_variant_object,
                                  variant->location,
                                  NEW_VARIANT(LilyCheckedScopeContainer,
                                              variable,
                                              self->id,
                                              variable),
                                  variant->variant);
                            }
                        }

                        break;
                    default:
                        break;
                }
            }

            return NEW(LilyCheckedScopeResponse);
        default:
            return NEW(LilyCheckedScopeResponse);
    }
}

LilyCheckedScopeResponse
search_identifier__LilyCheckedScope(LilyCheckedScope *self, const String *name)
{
    LilyCheckedScopeResponse variable =
      search_variable_in_current_scope__LilyCheckedScope(self, name);
    LilyCheckedScopeResponse fun =
      search_fun_in_current_scope__LilyCheckedScope(self, name);
    LilyCheckedScopeResponse module =
      search_module_in_current_scope__LilyCheckedScope(self, name);
    LilyCheckedScopeResponse constant =
      search_constant_in_current_scope__LilyCheckedScope(self, name);
    LilyCheckedScopeResponse field = search_field__LilyCheckedScope(self, name);
    LilyCheckedScopeResponse variant =
      search_variant__LilyCheckedScope(self, name);
    LilyCheckedScopeResponse enum_ =
      search_enum_in_current_scope__LilyCheckedScope(self, name);

    // [variable, fun, module, constant, field, variant, enum]
#define RESPONSES_IDENTIFIER_LEN 7
    LilyCheckedScopeResponse *responses[RESPONSES_IDENTIFIER_LEN] =
      (LilyCheckedScopeResponse *[RESPONSES_IDENTIFIER_LEN]){
          &variable, &fun, &module, &constant, &field, &variant, &enum_
      };
    int index_with_largest_id = -1;

    for (Usize i = 0; i < RESPONSES_IDENTIFIER_LEN; ++i) {
        LilyCheckedScopeResponse *response = responses[i];

        switch (response->kind) {
            case LILY_CHECKED_SCOPE_RESPONSE_KIND_NOT_FOUND:
                continue;
            default:
                if (index_with_largest_id == -1) {
                    index_with_largest_id = i;
                } else {
                    if (response->scope_container.scope_id >
                        responses[index_with_largest_id]
                          ->scope_container.scope_id) {
                        index_with_largest_id = i;
                    }
                }
        }
    }

    if (self->parent && index_with_largest_id == -1) {
        return search_identifier__LilyCheckedScope(self->parent->scope, name);
    }

    return index_with_largest_id == -1 ? NEW(LilyCheckedScopeResponse)
                                       : *responses[index_with_largest_id];
}

LilyCheckedScopeResponse
search_custom_type__LilyCheckedScope(LilyCheckedScope *self, const String *name)
{
    LilyCheckedScopeResponse record =
      search_record_in_current_scope__LilyCheckedScope(self, name);
    LilyCheckedScopeResponse enum_ =
      search_enum_in_current_scope__LilyCheckedScope(self, name);
    LilyCheckedScopeResponse generic =
      search_generic_in_current_scope__LilyCheckedScope(self, name);
    LilyCheckedScopeResponse class = search_class__LilyCheckedScope(self, name);
    LilyCheckedScopeResponse record_object =
      search_record_object_in_current_scope__LilyCheckedScope(self, name);
    LilyCheckedScopeResponse enum_object =
      search_enum_object_in_current_scope__LilyCheckedScope(self, name);
    LilyCheckedScopeResponse trait =
      search_trait_in_current_scope__LilyCheckedScope(self, name);

    // [record, enum, generic, class, record_object, enum_object, trait]
#define RESPONSES_CUSTOM_TYPE_LEN 7
    LilyCheckedScopeResponse *responses[RESPONSES_CUSTOM_TYPE_LEN] =
      (LilyCheckedScopeResponse *[RESPONSES_CUSTOM_TYPE_LEN]){
          &record,        &enum_,       &generic, &class,
          &record_object, &enum_object, &trait
      };
    int index_with_largest_id = -1;

    for (Usize i = 0; i < RESPONSES_CUSTOM_TYPE_LEN; ++i) {
        LilyCheckedScopeResponse *response = responses[i];

        switch (response->kind) {
            case LILY_CHECKED_SCOPE_RESPONSE_KIND_NOT_FOUND:
                continue;
            default:
                if (index_with_largest_id == -1) {
                    index_with_largest_id = i;
                } else {
                    if (response->scope_container.scope_id >
                        responses[index_with_largest_id]
                          ->scope_container.scope_id) {
                        index_with_largest_id = i;
                    }
                }
        }
    }

    if (self->parent && index_with_largest_id == -1) {
        return search_custom_type__LilyCheckedScope(self->parent->scope, name);
    }

    return index_with_largest_id == -1 ? NEW(LilyCheckedScopeResponse)
                                       : *responses[index_with_largest_id];
}

LilyCheckedScope *
get_scope_from_id__LilyCheckedScope(LilyCheckedScope *self, Usize id)
{
    if (id > self->id) {
        UNREACHABLE("the analysis have a bug!!");
    } else if (id == self->id) {
        return self;
    } else {
        return get_scope_from_id__LilyCheckedScope(self->parent->scope, id);
    }
}

LilyCheckedScope *
safe_get_scope_from_id__LilyCheckedScope(LilyCheckedScope *self, Usize id)
{
    if (id > self->id) {
        return NULL;
    } else if (id == self->id) {
        return self;
    } else {
        return get_scope_from_id__LilyCheckedScope(self->parent->scope, id);
    }
}

LilyCheckedDecl *
get_decl_from_id__LilyCheckedScope(LilyCheckedScope *self, Usize id, Usize pos)
{
    LilyCheckedScope *scope = get_scope_from_id__LilyCheckedScope(self, id);

    switch (scope->decls.kind) {
        case LILY_CHECKED_SCOPE_DECLS_KIND_MODULE:
            return get__Vec(scope->decls.module->decls, pos);
        default:
            return NULL;
    }
}

LilyCheckedBodyFunItem *
get_variable_from_id__LilyCheckedScope(LilyCheckedScope *self,
                                       Usize id,
                                       String *name)
{
    LilyCheckedScope *scope = get_scope_from_id__LilyCheckedScope(self, id);

    switch (scope->decls.kind) {
        case LILY_CHECKED_SCOPE_DECLS_KIND_SCOPE:
            for (Usize i = 0; i < scope->variables->len; ++i) {
                LilyCheckedScopeContainerVariable *variable =
                  get__Vec(scope->variables, i);

                if (!strcmp(variable->name->buffer, name->buffer)) {
                    return get__Vec(scope->decls.scope, variable->id);
                }
            }

            UNREACHABLE("the scope have a bug!!");
        default:
            return NULL;
    }
}

LilyCheckedScopeDecls *
get_current_fun__LilyCheckedScope(LilyCheckedScope *self)
{
    // TODO: add a support for the lambda statement
    switch (self->decls.kind) {
        case LILY_CHECKED_SCOPE_DECLS_KIND_DECL:
            switch (self->decls.decl->kind) {
                case LILY_CHECKED_DECL_KIND_FUN:
                    return &self->decls;
                default:
                    break;
            }

            break;
        default:
            break;
    }

    return self->parent ? get_current_fun__LilyCheckedScope(self->parent->scope)
                        : NULL;
}

LilyCheckedDecl *
get_current_method__LilyCheckedScope(LilyCheckedScope *self)
{
    switch (self->decls.kind) {
        case LILY_CHECKED_SCOPE_DECLS_KIND_DECL:
            switch (self->decls.decl->kind) {
                case LILY_CHECKED_DECL_KIND_METHOD:
                    return self->decls.decl;
                default:
                    break;
            }

            break;
        default:
            break;
    }

    return self->parent
             ? get_current_method__LilyCheckedScope(self->parent->scope)
             : NULL;
}

LilyCheckedDecl *
get_current_object__LilyCheckedScope(LilyCheckedScope *self)
{
    switch (self->decls.kind) {
        case LILY_CHECKED_SCOPE_DECLS_KIND_DECL:
            switch (self->decls.decl->kind) {
                case LILY_CHECKED_DECL_KIND_OBJECT:
                    return self->decls.decl;
                default:
                    break;
            }

            break;
        default:
            break;
    }

    return self->parent
             ? get_current_object__LilyCheckedScope(self->parent->scope)
             : NULL;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyCheckedScope, const LilyCheckedScope *self)
{
    String *res =
      format__String("LilyCheckedScope{{ id = {d}, modules =", self->id);

    DEBUG_VEC_STR(self->modules, res, LilyCheckedScopeContainerModule);

    push_str__String(res, ", constants =");

    DEBUG_VEC_STR(self->constants, res, LilyCheckedScopeContainerConstant);

    push_str__String(res, ", enums =");

    DEBUG_VEC_STR(self->enums, res, LilyCheckedScopeContainerEnum);

    push_str__String(res, ", records =");

    DEBUG_VEC_STR(self->records, res, LilyCheckedScopeContainerRecord);

    push_str__String(res, ", aliases =");

    DEBUG_VEC_STR(self->aliases, res, LilyCheckedScopeContainerAlias);

    push_str__String(res, ", errors =");

    DEBUG_VEC_STR(self->errors, res, LilyCheckedScopeContainerError);

    push_str__String(res, ", enums_object =");

    DEBUG_VEC_STR(self->enums_object, res, LilyCheckedScopeContainerEnumObject);

    push_str__String(res, ", records_object =");

    DEBUG_VEC_STR(
      self->records_object, res, LilyCheckedScopeContainerRecordObject);

    push_str__String(res, ", classes =");

    DEBUG_VEC_STR(self->classes, res, LilyCheckedScopeContainerClass);

    push_str__String(res, ", traits =");

    DEBUG_VEC_STR(self->traits, res, LilyCheckedScopeContainerTrait);

    push_str__String(res, ", funs =");

    DEBUG_VEC_STRING(self->funs, res, LilyCheckedScopeContainerFun);

    push_str__String(res, ", labels =");

    DEBUG_VEC_STR(self->labels, res, LilyCheckedScopeContainerLabel);

    push_str__String(res, ", variables =");

    DEBUG_VEC_STR(self->variables, res, LilyCheckedScopeContainerVariable);

    push_str__String(res, ", parent = ");

    if (self->parent) {
        String *s = to_string__Debug__LilyCheckedParent(self->parent);

        APPEND_AND_FREE(res, s);
    } else {
        push_str__String(res, "NULL");
    }

    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(LilyCheckedScope, LilyCheckedScope *self)
{
    FREE_BUFFER_ITEMS(self->modules->buffer,
                      self->modules->len,
                      LilyCheckedScopeContainerModule);
    FREE(Vec, self->modules);

    FREE_BUFFER_ITEMS(self->constants->buffer,
                      self->constants->len,
                      LilyCheckedScopeContainerConstant);
    FREE(Vec, self->constants);

    FREE_BUFFER_ITEMS(
      self->enums->buffer, self->enums->len, LilyCheckedScopeContainerEnum);
    FREE(Vec, self->enums);

    FREE_BUFFER_ITEMS(self->records->buffer,
                      self->records->len,
                      LilyCheckedScopeContainerRecord);
    FREE(Vec, self->records);

    FREE_BUFFER_ITEMS(self->aliases->buffer,
                      self->aliases->len,
                      LilyCheckedScopeContainerAlias);
    FREE(Vec, self->aliases);

    FREE_BUFFER_ITEMS(
      self->errors->buffer, self->errors->len, LilyCheckedScopeContainerError);
    FREE(Vec, self->errors);

    FREE_BUFFER_ITEMS(self->enums_object->buffer,
                      self->enums_object->len,
                      LilyCheckedScopeContainerEnumObject);
    FREE(Vec, self->enums_object);

    FREE_BUFFER_ITEMS(self->records_object->buffer,
                      self->records_object->len,
                      LilyCheckedScopeContainerRecordObject);
    FREE(Vec, self->records_object);

    FREE_BUFFER_ITEMS(self->classes->buffer,
                      self->classes->len,
                      LilyCheckedScopeContainerClass);
    FREE(Vec, self->classes);

    FREE_BUFFER_ITEMS(
      self->traits->buffer, self->traits->len, LilyCheckedScopeContainerTrait);
    FREE(Vec, self->traits);

    FREE_BUFFER_ITEMS(
      self->labels->buffer, self->labels->len, LilyCheckedScopeContainerLabel);
    FREE(Vec, self->labels);

    FREE_BUFFER_ITEMS(
      self->funs->buffer, self->funs->len, LilyCheckedScopeContainerFun);
    FREE(Vec, self->funs);

    FREE_BUFFER_ITEMS(self->variables->buffer,
                      self->variables->len,
                      LilyCheckedScopeContainerVariable);
    FREE(Vec, self->variables);

    FREE_BUFFER_ITEMS(self->params->buffer,
                      self->params->len,
                      LilyCheckedScopeContainerVariable);
    FREE(Vec, self->params);

    FREE_BUFFER_ITEMS(self->generics->buffer,
                      self->generics->len,
                      LilyCheckedScopeContainerGeneric);
    FREE(Vec, self->generics);

    FREE_BUFFER_ITEMS(self->methods->buffer,
                      self->methods->len,
                      LilyCheckedScopeContainerMethod);
    FREE(Vec, self->methods);

    if (self->parent) {
        FREE(LilyCheckedParent, self->parent);
    }

    FREE(LilyCheckedDropTable, &self->drop_table);

    lily_free(self);
}
