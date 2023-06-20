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

#include <base/format.h>
#include <base/new.h>

#include <core/lily/checked/body/fun.h>
#include <core/lily/checked/decl.h>
#include <core/lily/checked/parent.h>
#include <core/lily/checked/scope.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
search_fun_in_current_scope__LilyCheckedScope(LilyCheckedScope *self,
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
search_module__LilyCheckedScope(LilyCheckedScope *self, const String *name)
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

            break;
        default:
            return NEW(LilyCheckedScopeResponse);
    }

    if (self->parent) {
        return search_module__LilyCheckedScope(self->parent->scope, name);
    }

    return NEW(LilyCheckedScopeResponse);
}

LilyCheckedScopeResponse
search_variable__LilyCheckedScope(LilyCheckedScope *self, const String *name)
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

            break;
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

            break;
        default:
            return NEW(LilyCheckedScopeResponse);
    }

    if (self->parent && (self->parent->scope->decls.kind ==
                           LILY_CHECKED_SCOPE_DECLS_KIND_SCOPE ||
                         self->parent->scope->decls.kind ==
                           LILY_CHECKED_SCOPE_DECLS_KIND_DECL)) {
        return search_variable__LilyCheckedScope(self->parent->scope, name);
    }

    return NEW(LilyCheckedScopeResponse);
}

LilyCheckedScopeResponse
search_fun__LilyCheckedScope(LilyCheckedScope *self, const String *name)
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

            break;
        default:
            break;
    }

    if (self->parent) {
        return search_fun__LilyCheckedScope(self->parent->scope, name);
    }

    return NEW(LilyCheckedScopeResponse);
}

LilyCheckedScopeResponse
search_constant__LilyCheckedScope(LilyCheckedScope *self, const String *name)
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

            break;
        default:
            break;
    }

    if (self->parent) {
        return search_constant__LilyCheckedScope(self->parent->scope, name);
    }

    return NEW(LilyCheckedScopeResponse);
}

LilyCheckedScopeResponse
search_error__LilyCheckedScope(LilyCheckedScope *self, const String *name)
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

            break;
        default:
            break;
    }

    if (self->parent) {
        return search_error__LilyCheckedScope(self->parent->scope, name);
    }

    return NEW(LilyCheckedScopeResponse);
}

LilyCheckedScopeResponse
search_alias__LilyCheckedScope(LilyCheckedScope *self, const String *name)
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

            break;
        default:
            break;
    }

    if (self->parent) {
        return search_alias__LilyCheckedScope(self->parent->scope, name);
    }

    return NEW(LilyCheckedScopeResponse);
}

LilyCheckedScopeResponse
search_record__LilyCheckedScope(LilyCheckedScope *self, const String *name)
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

            break;
        default:
            break;
    }

    if (self->parent) {
        return search_record__LilyCheckedScope(self->parent->scope, name);
    }

    return NEW(LilyCheckedScopeResponse);
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
            }
        default:
            break;
    }

    return NEW(LilyCheckedScopeResponse);
}

LilyCheckedScopeResponse
search_identifier__LilyCheckedScope(LilyCheckedScope *self, const String *name)
{
    LilyCheckedScopeResponse variable =
      search_variable__LilyCheckedScope(self, name);
    LilyCheckedScopeResponse fun = search_fun__LilyCheckedScope(self, name);
    LilyCheckedScopeResponse module =
      search_module__LilyCheckedScope(self, name);
    LilyCheckedScopeResponse constant =
      search_constant__LilyCheckedScope(self, name);

    // [variable, fun, module, constant]
    // [0, 1, 2, 3]
    Vec *responses = init__Vec(4, &variable, &fun, &module, &constant);

    bool variable_is_found = true;
    bool fun_is_found = true;
    bool module_is_found = true;
    bool constant_is_found = true;

    for (Usize i = 0; i < responses->len; ++i) {
        LilyCheckedScopeResponse *response = get__Vec(responses, i);

        switch (response->kind) {
            case LILY_CHECKED_SCOPE_RESPONSE_KIND_NOT_FOUND:
                if (i == 0) {
                    variable_is_found = false;
                } else if (i == 1) {
                    fun_is_found = false;
                } else if (i == 2) {
                    module_is_found = false;
                } else if (i == 3) {
                    constant_is_found = false;
                }

                break;
            default:
                break;
        }
    }

    FREE(Vec, responses);

    switch (variable_is_found + fun_is_found + module_is_found +
            constant_is_found) {
        case 4:
        found_4 : {
            // The scope of the variable or function is in all situations higher
            // than that of the module.
            if (fun.scope_container.scope_id >
                variable.scope_container.scope_id) {
                return fun;
            } else {
                FREE(LilyCheckedScopeResponse, &fun);

                return variable;
            }
        }
        case 3:
            if ((variable_is_found && fun_is_found && constant_is_found) ||
                (variable_is_found && fun_is_found && module_is_found) ||
                (module_is_found && constant_is_found && variable_is_found)) {
                goto found_4;
            } else if (fun_is_found && constant_is_found && module_is_found) {
                if (fun.scope_container.scope_id >
                      constant.scope_container.scope_id &&
                    fun.scope_container.scope_id >
                      module.scope_container.scope_id) {
                    return fun;
                } else if (constant.scope_container.scope_id >
                             fun.scope_container.scope_id &&
                           constant.scope_container.scope_id >
                             module.scope_container.scope_id) {
                    FREE(LilyCheckedScopeResponse, &fun);

                    return constant;
                } else if (module.scope_container.scope_id >
                             fun.scope_container.scope_id &&
                           module.scope_container.scope_id >
                             constant.scope_container.scope_id) {
                    FREE(LilyCheckedScopeResponse, &fun);

                    return module;
                } else {
                    UNREACHABLE("the scope have a bug!!");
                }
            } else {
                UNREACHABLE("the scope have a bug!!");
            }

            break;
        case 2:
            if (variable_is_found && fun_is_found) {
                goto found_4;
            } else if ((variable_is_found && constant_is_found) ||
                       (variable_is_found && module_is_found)) {
                return variable;
            } else if (fun_is_found && constant_is_found) {
                if (fun.scope_container.scope_id >
                    constant.scope_container.scope_id) {
                    return fun;
                } else if (fun.scope_container.scope_id <
                           constant.scope_container.scope_id) {
                    FREE(LilyCheckedScopeResponse, &fun);

                    return constant;
                } else {
                    UNREACHABLE("the scope have a bug!!");
                }
            } else if (fun_is_found && module_is_found) {
                if (fun.scope_container.scope_id >
                    module.scope_container.scope_id) {
                    return fun;
                } else if (fun.scope_container.scope_id <
                           module.scope_container.scope_id) {
                    return module;
                } else {
                    UNREACHABLE("the scope have a bug!!");
                }
            } else if (constant_is_found && module_is_found) {
                if (constant.scope_container.scope_id >
                    module.scope_container.scope_id) {
                    return constant;
                } else if (constant.scope_container.scope_id <
                           module.scope_container.scope_id) {
                    return module;
                } else {
                    UNREACHABLE("the scope have a bug!!");
                }
            } else {
                UNREACHABLE("the scope have a bug!!");
            }

            break;
        case 1:
            if (variable_is_found) {
                return variable;
            } else if (fun_is_found) {
                return fun;
            } else if (constant_is_found) {
                return constant;
            } else {
                return module;
            }

            break;
        case 0:
            return NEW(LilyCheckedScopeResponse);
        default:
            UNREACHABLE("this situation is impossible");
    }
}

LilyCheckedScopeResponse
search_custom_type__LilyCheckedScope(LilyCheckedScope *self, const String *name)
{
    // TODO: search other custom data type
    return search_record__LilyCheckedScope(self, name);
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
