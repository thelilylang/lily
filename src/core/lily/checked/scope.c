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

#ifdef ENV_DEBUG
#include <stdio.h>
#include <stdlib.h>
#endif

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
    self->parent = parent;
    self->decls = decls;

    return self;
}

int
add_module__LilyCheckedScope(LilyCheckedScope *self,
                             LilyCheckedScopeContainerModule *module)
{
    ADD_TO_SCOPE(self->modules, module, LilyCheckedScopeContainerModule);
}

int
add_constant__LilyCheckedScope(LilyCheckedScope *self,
                               LilyCheckedScopeContainerConstant *constant)
{
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

    if (self->parent && (self->parent->scope->decls.kind ==
                         LILY_CHECKED_SCOPE_DECLS_KIND_MODULE)) {
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
                          &param->location,
                          NEW_VARIANT(LilyCheckedScopeContainer,
                                      variable,
                                      self->id,
                                      variable),
                          param);
                    }
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
search_identifier__LilyCheckedScope(LilyCheckedScope *self, const String *name)
{
    LilyCheckedScopeResponse module =
      search_module__LilyCheckedScope(self, name);
    LilyCheckedScopeResponse variable =
      search_variable__LilyCheckedScope(self, name);

    if (module.kind == LILY_CHECKED_SCOPE_RESPONSE_KIND_NOT_FOUND &&
        variable.kind == LILY_CHECKED_SCOPE_RESPONSE_KIND_NOT_FOUND) {
        return NEW(LilyCheckedScopeResponse);
    }

    if (module.kind == LILY_CHECKED_SCOPE_RESPONSE_KIND_NOT_FOUND) {
        return variable;
    } else if (variable.kind == LILY_CHECKED_SCOPE_RESPONSE_KIND_NOT_FOUND) {
        return module;
    }

    if (module.scope_container.module->id >
        variable.scope_container.variable->id) {
        return module;
    }

    return variable;
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
}
