/*
 * MIT License
 *
 * Copyright (c) 2022-2025 ArthurPV
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
search_captured_variable_in_current_scope__LilyCheckedScope(
  LilyCheckedScope *self,
  const String *name);

static LilyCheckedScopeResponse
search_module_in_current_scope__LilyCheckedScope(LilyCheckedScope *self,
                                                 const String *name);

static LilyCheckedScopeResponse
search_variable_in_current_scope__LilyCheckedScope(LilyCheckedScope *self,
                                                   const String *name);

static LilyCheckedScopeResponse
search_param_in_current_scope__LilyCheckedScope(LilyCheckedScope *self,
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

#define HASH_MAP_CHECK_IF_EXISTS(container, item)          \
    if (container) {                                       \
        if (get__HashMap(container, item->name->buffer)) { \
            return 1;                                      \
        }                                                  \
    }

#define VEC_CHECK_IF_EXISTS(container, item, container_name)           \
    if (container) {                                                   \
        for (Usize i = 0; i < container->len; ++i) {                   \
            if (!strcmp(CAST(container_name *, get__Vec(container, i)) \
                          ->name->buffer,                              \
                        item->name->buffer)) {                         \
                return 1;                                              \
            }                                                          \
        }                                                              \
    }

#define HASH_MAP_ADD_TO_SCOPE(container, item)            \
    HASH_MAP_CHECK_IF_EXISTS(container, item);            \
                                                          \
    if (!container) {                                     \
        container = NEW(HashMap);                         \
    }                                                     \
                                                          \
    insert__HashMap(container, item->name->buffer, item); \
                                                          \
    return 0;

#define VEC_ADD_TO_SCOPE(container, item, container_name) \
    VEC_CHECK_IF_EXISTS(container, item, container_name); \
                                                          \
    if (!container) {                                     \
        container = NEW(Vec);                             \
    }                                                     \
                                                          \
    push__Vec(container, item);                           \
                                                          \
    return 0;

CONSTRUCTOR(LilyCheckedScopeCatch *,
            LilyCheckedScopeCatch,
            String *name,
            const Location *location,
            HashMap *raises)
{
    LilyCheckedScopeCatch *self = lily_malloc(sizeof(LilyCheckedScopeCatch));

    self->name = name;
    self->location = location;
    self->raises = raises;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedScopeCatch,
               const LilyCheckedScopeCatch *self)
{
    String *res = format__String(
      "LilyCheckedScopeCatch{{ name = {S}, location = {sa}, raises =",
      self->name,
      to_string__Debug__Location(self->location));

    DEBUG_HASH_MAP_STRING(self->raises, res, LilyCheckedDataType);

    push_str__String(res, " }");

    return res;
}
#endif

CONSTRUCTOR(LilyCheckedScope *,
            LilyCheckedScope,
            LilyCheckedParent *parent,
            LilyCheckedScopeDecls decls)
{
    LilyCheckedScope *self = lily_malloc(sizeof(LilyCheckedScope));

    self->id = parent ? parent->scope->id + 1 : 0;
    // TODO: add support for lambda
    self->raises = NULL;
    self->catch = NULL;
    self->captured_variables = NULL;
    self->modules = NULL;
    self->constants = NULL;
    self->enums = NULL;
    self->records = NULL;
    self->aliases = NULL;
    self->errors = NULL;
    self->enums_object = NULL;
    self->records_object = NULL;
    self->classes = NULL;
    self->traits = NULL;
    self->funs = NULL;
    self->labels = NULL;
    self->variables = NULL;
    self->params = NULL;
    self->generics = NULL;
    self->methods = NULL;
    self->parent = parent;
    self->decls = decls;
    self->has_return = false;

    return self;
}

int
add_captured_variable__LilyCheckedScope(
  LilyCheckedScope *self,
  LilyCheckedScopeContainerCapturedVariable *captured_variable)
{
    HASH_MAP_ADD_TO_SCOPE(self->captured_variables, captured_variable);
}

int
add_module__LilyCheckedScope(LilyCheckedScope *self,
                             LilyCheckedScopeContainerModule *module)
{
    HASH_MAP_CHECK_IF_EXISTS(self->constants, module);
    VEC_CHECK_IF_EXISTS(self->funs, module, LilyCheckedScopeContainerFun);
    HASH_MAP_ADD_TO_SCOPE(self->modules, module);
}

int
add_constant__LilyCheckedScope(LilyCheckedScope *self,
                               LilyCheckedScopeContainerConstant *constant)
{
    VEC_CHECK_IF_EXISTS(self->funs, constant, LilyCheckedScopeContainerFun);
    HASH_MAP_CHECK_IF_EXISTS(self->modules, constant);
    HASH_MAP_ADD_TO_SCOPE(self->constants, constant);
}

int
add_enum__LilyCheckedScope(LilyCheckedScope *self,
                           LilyCheckedScopeContainerEnum *enum_)
{
    HASH_MAP_CHECK_IF_EXISTS(self->records, enum_);
    HASH_MAP_CHECK_IF_EXISTS(self->aliases, enum_);
    HASH_MAP_CHECK_IF_EXISTS(self->records_object, enum_);
    HASH_MAP_CHECK_IF_EXISTS(self->enums_object, enum_);
    HASH_MAP_CHECK_IF_EXISTS(self->classes, enum_);
    HASH_MAP_CHECK_IF_EXISTS(self->traits, enum_);
    HASH_MAP_ADD_TO_SCOPE(self->enums, enum_);
}

int
add_record__LilyCheckedScope(LilyCheckedScope *self,
                             LilyCheckedScopeContainerRecord *record)
{
    HASH_MAP_CHECK_IF_EXISTS(self->enums, record);
    HASH_MAP_CHECK_IF_EXISTS(self->aliases, record);
    HASH_MAP_CHECK_IF_EXISTS(self->records_object, record);
    HASH_MAP_CHECK_IF_EXISTS(self->enums_object, record);
    HASH_MAP_CHECK_IF_EXISTS(self->classes, record);
    HASH_MAP_CHECK_IF_EXISTS(self->traits, record);
    HASH_MAP_ADD_TO_SCOPE(self->records, record);
}

int
add_alias__LilyCheckedScope(LilyCheckedScope *self,
                            LilyCheckedScopeContainerAlias *alias)
{
    HASH_MAP_CHECK_IF_EXISTS(self->enums, alias);
    HASH_MAP_CHECK_IF_EXISTS(self->records, alias);
    HASH_MAP_CHECK_IF_EXISTS(self->records_object, alias);
    HASH_MAP_CHECK_IF_EXISTS(self->enums_object, alias);
    HASH_MAP_CHECK_IF_EXISTS(self->classes, alias);
    HASH_MAP_CHECK_IF_EXISTS(self->traits, alias);
    HASH_MAP_ADD_TO_SCOPE(self->aliases, alias);
}

int
add_error__LilyCheckedScope(LilyCheckedScope *self,
                            LilyCheckedScopeContainerError *error)
{
    HASH_MAP_ADD_TO_SCOPE(self->errors, error);
}

int
add_enum_object__LilyCheckedScope(
  LilyCheckedScope *self,
  LilyCheckedScopeContainerEnumObject *enum_object)
{
    HASH_MAP_CHECK_IF_EXISTS(self->enums, enum_object);
    HASH_MAP_CHECK_IF_EXISTS(self->records, enum_object);
    HASH_MAP_CHECK_IF_EXISTS(self->records_object, enum_object);
    HASH_MAP_CHECK_IF_EXISTS(self->aliases, enum_object);
    HASH_MAP_CHECK_IF_EXISTS(self->classes, enum_object);
    HASH_MAP_CHECK_IF_EXISTS(self->traits, enum_object);
    HASH_MAP_ADD_TO_SCOPE(self->enums_object, enum_object);
}

int
add_record_object__LilyCheckedScope(
  LilyCheckedScope *self,
  LilyCheckedScopeContainerRecordObject *record_object)
{
    HASH_MAP_CHECK_IF_EXISTS(self->enums, record_object);
    HASH_MAP_CHECK_IF_EXISTS(self->records, record_object);
    HASH_MAP_CHECK_IF_EXISTS(self->aliases, record_object);
    HASH_MAP_CHECK_IF_EXISTS(self->enums_object, record_object);
    HASH_MAP_CHECK_IF_EXISTS(self->classes, record_object);
    HASH_MAP_CHECK_IF_EXISTS(self->traits, record_object);
    HASH_MAP_ADD_TO_SCOPE(self->records_object, record_object);
}

int
add_class__LilyCheckedScope(LilyCheckedScope *self,
                            LilyCheckedScopeContainerClass *class)
{
    HASH_MAP_CHECK_IF_EXISTS(self->enums, class);
    HASH_MAP_CHECK_IF_EXISTS(self->records, class);
    HASH_MAP_CHECK_IF_EXISTS(self->records_object, class);
    HASH_MAP_CHECK_IF_EXISTS(self->enums_object, class);
    HASH_MAP_CHECK_IF_EXISTS(self->aliases, class);
    HASH_MAP_CHECK_IF_EXISTS(self->traits, class);
    HASH_MAP_ADD_TO_SCOPE(self->classes, class);
}

int
add_trait__LilyCheckedScope(LilyCheckedScope *self,
                            LilyCheckedScopeContainerTrait *trait)
{
    HASH_MAP_CHECK_IF_EXISTS(self->enums, trait);
    HASH_MAP_CHECK_IF_EXISTS(self->records, trait);
    HASH_MAP_CHECK_IF_EXISTS(self->records_object, trait);
    HASH_MAP_CHECK_IF_EXISTS(self->enums_object, trait);
    HASH_MAP_CHECK_IF_EXISTS(self->classes, trait);
    HASH_MAP_CHECK_IF_EXISTS(self->aliases, trait);
    HASH_MAP_ADD_TO_SCOPE(self->traits, trait);
}

int
add_fun__LilyCheckedScope(LilyCheckedScope *self,
                          LilyCheckedScopeContainerFun *fun)
{
    HASH_MAP_CHECK_IF_EXISTS(self->variables, fun);
    HASH_MAP_CHECK_IF_EXISTS(self->modules, fun);
    VEC_ADD_TO_SCOPE(self->funs, fun, LilyCheckedScopeContainerFun);
}

int
add_label__LilyCheckedScope(LilyCheckedScope *self,
                            LilyCheckedScopeContainerLabel *label)
{
    HASH_MAP_CHECK_IF_EXISTS(self->variables, label);
    HASH_MAP_ADD_TO_SCOPE(self->labels, label);
}

int
add_variable__LilyCheckedScope(LilyCheckedScope *self,
                               LilyCheckedScopeContainerVariable *variable)
{
    HASH_MAP_CHECK_IF_EXISTS(self->labels, variable);
    HASH_MAP_ADD_TO_SCOPE(self->variables, variable);
}

int
add_param__LilyCheckedScope(LilyCheckedScope *self,
                            LilyCheckedScopeContainerVariable *param)
{
    HASH_MAP_ADD_TO_SCOPE(self->params, param);
}

int
add_generic__LilyCheckedScope(LilyCheckedScope *self,
                              LilyCheckedScopeContainerGeneric *generic)
{
    HASH_MAP_ADD_TO_SCOPE(self->generics, generic);
}

LilyCheckedScopeContainerFun *
get_fun_in_current_scope__LilyCheckedScope(LilyCheckedScope *self,
                                           const String *name)
{
    if (self->funs) {
        for (Usize i = 0; i < self->funs->len; ++i) {
            LilyCheckedScopeContainerFun *fun = get__Vec(self->funs, i);

            if (!strcmp(fun->name->buffer, name->buffer)) {
                return fun;
            }
        }
    }

    return NULL;
}

const LilyCheckedScopeDecls *
get_parent__LilyCheckedScope(const LilyCheckedScope *self)
{
    switch (self->decls.kind) {
        case LILY_CHECKED_SCOPE_DECLS_KIND_MODULE:
            return NULL;
        case LILY_CHECKED_SCOPE_DECLS_KIND_DECL:
            switch (self->decls.decl->kind) {
                case LILY_CHECKED_DECL_KIND_FUN:
                case LILY_CHECKED_DECL_KIND_METHOD:
                    return &self->decls;
                default:
                    return NULL;
            }
        case LILY_CHECKED_SCOPE_DECLS_KIND_SCOPE:
            return self->parent
                     ? get_parent__LilyCheckedScope(self->parent->scope)
                     : NULL;
        case LILY_CHECKED_SCOPE_DECLS_KIND_STMT:
            return NULL;
        default:
            UNREACHABLE("unknown variant");
    }
}

LilyCheckedScopeResponse
search_captured_variable_in_current_scope__LilyCheckedScope(
  LilyCheckedScope *self,
  const String *name)
{
    if (self->captured_variables) {
        switch (self->decls.kind) {
            case LILY_CHECKED_SCOPE_DECLS_KIND_STMT: {
                LilyCheckedScopeContainerCapturedVariable *captured_variable =
                  get__HashMap(self->captured_variables, name->buffer);

                if (captured_variable) {
                    LilyCheckedCapturedVariable *cv =
                      get_from_id__OrderedHashMap(
                        get_captured_variables__LilyCheckedScopeStmt(
                          &self->decls.stmt),
                        captured_variable->id);

                    ASSERT(cv);

                    return NEW_VARIANT(LilyCheckedScopeResponse,
                                       captured_variable,
                                       cv->location,
                                       NEW_VARIANT(LilyCheckedScopeContainer,
                                                   captured_variable,
                                                   self->id,
                                                   captured_variable),
                                       cv);
                }

                break;
            }
            default:
                break;
        }
    }

    return NEW(LilyCheckedScopeResponse);
}

LilyCheckedScopeResponse
search_module_in_current_scope__LilyCheckedScope(LilyCheckedScope *self,
                                                 const String *name)
{
    if (self->modules) {
        switch (self->decls.kind) {
            case LILY_CHECKED_SCOPE_DECLS_KIND_MODULE: {
                LilyCheckedScopeContainerModule *module =
                  get__HashMap(self->modules, name->buffer);

                if (module) {
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

                return NEW(LilyCheckedScopeResponse);
            }
            default:
                return NEW(LilyCheckedScopeResponse);
        }
    }

    return NEW(LilyCheckedScopeResponse);
}

LilyCheckedScopeResponse
search_variable_in_current_scope__LilyCheckedScope(LilyCheckedScope *self,
                                                   const String *name)
{
    if (self->variables) {
        switch (self->decls.kind) {
            case LILY_CHECKED_SCOPE_DECLS_KIND_SCOPE: {
                LilyCheckedScopeContainerVariable *variable =
                  get__HashMap(self->variables, name->buffer);

                if (variable) {
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

                return NEW(LilyCheckedScopeResponse);
            }
            default:
                return NEW(LilyCheckedScopeResponse);
        }
    }

    return NEW(LilyCheckedScopeResponse);
}

LilyCheckedScopeResponse
search_param_in_current_scope__LilyCheckedScope(LilyCheckedScope *self,
                                                const String *name)
{
    if (self->params) {
        switch (self->decls.kind) {
            case LILY_CHECKED_SCOPE_DECLS_KIND_DECL:
                if (self->decls.decl->kind == LILY_CHECKED_DECL_KIND_FUN ||
                    self->decls.decl->kind == LILY_CHECKED_DECL_KIND_METHOD) {
                    LilyCheckedScopeContainerVariable *variable =
                      get__HashMap(self->params, name->buffer);

                    if (variable) {
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

                return NEW(LilyCheckedScopeResponse);
            default:
                return NEW(LilyCheckedScopeResponse);
        }
    }

    return NEW(LilyCheckedScopeResponse);
}

LilyCheckedScopeResponse
search_fun_in_current_scope__LilyCheckedScope(LilyCheckedScope *self,
                                              const String *name)
{
    if (self->funs) {
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

    return NEW(LilyCheckedScopeResponse);
}

LilyCheckedScopeResponse
search_constant_in_current_scope__LilyCheckedScope(LilyCheckedScope *self,
                                                   const String *name)
{
    if (self->constants) {
        switch (self->decls.kind) {
            case LILY_CHECKED_SCOPE_DECLS_KIND_MODULE: {
                LilyCheckedScopeContainerConstant *constant =
                  get__HashMap(self->constants, name->buffer);

                if (constant) {
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

                return NEW(LilyCheckedScopeResponse);
            }
            default:
                return NEW(LilyCheckedScopeResponse);
        }
    }

    return NEW(LilyCheckedScopeResponse);
}

LilyCheckedScopeResponse
search_error_in_current_scope__LilyCheckedScope(LilyCheckedScope *self,
                                                const String *name)
{
    if (self->errors) {
        switch (self->decls.kind) {
            case LILY_CHECKED_SCOPE_DECLS_KIND_MODULE: {
                LilyCheckedScopeContainerError *error =
                  get__HashMap(self->errors, name->buffer);

                if (error) {
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

                return NEW(LilyCheckedScopeResponse);
            }
            default:
                return NEW(LilyCheckedScopeResponse);
        }
    }

    return NEW(LilyCheckedScopeResponse);
}

LilyCheckedScopeResponse
search_alias_in_current_scope__LilyCheckedScope(LilyCheckedScope *self,
                                                const String *name)
{
    if (self->aliases) {
        switch (self->decls.kind) {
            case LILY_CHECKED_SCOPE_DECLS_KIND_MODULE: {
                LilyCheckedScopeContainerAlias *alias =
                  get__HashMap(self->aliases, name->buffer);

                if (alias) {
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

                return NEW(LilyCheckedScopeResponse);
            }
            default:
                return NEW(LilyCheckedScopeResponse);
        }
    }

    return NEW(LilyCheckedScopeResponse);
}

LilyCheckedScopeResponse
search_record_in_current_scope__LilyCheckedScope(LilyCheckedScope *self,
                                                 const String *name)
{
    if (self->records) {
        switch (self->decls.kind) {
            case LILY_CHECKED_SCOPE_DECLS_KIND_MODULE: {
                LilyCheckedScopeContainerRecord *record =
                  get__HashMap(self->records, name->buffer);

                if (record) {
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

                return NEW(LilyCheckedScopeResponse);
            }
            default:
                return NEW(LilyCheckedScopeResponse);
        }
    }

    return NEW(LilyCheckedScopeResponse);
}

LilyCheckedScopeResponse
search_enum_in_current_scope__LilyCheckedScope(LilyCheckedScope *self,
                                               const String *name)
{
    if (self->enums) {
        switch (self->decls.kind) {
            case LILY_CHECKED_SCOPE_DECLS_KIND_MODULE: {
                LilyCheckedScopeContainerEnum *enum_ =
                  get__HashMap(self->enums, name->buffer);

                if (enum_) {
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

                return NEW(LilyCheckedScopeResponse);
            }
            default:
                return NEW(LilyCheckedScopeResponse);
        }
    }

    return NEW(LilyCheckedScopeResponse);
}

LilyCheckedScopeResponse
search_generic_in_current_scope__LilyCheckedScope(LilyCheckedScope *self,
                                                  const String *name)
{
    if (self->generics) {
        switch (self->decls.kind) {
            case LILY_CHECKED_SCOPE_DECLS_KIND_DECL: {
                LilyCheckedScopeContainerGeneric *generic =
                  get__HashMap(self->generics, name->buffer);

                if (generic) {
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

                return NEW(LilyCheckedScopeResponse);
            }
            default:
                return NEW(LilyCheckedScopeResponse);
        }
    }

    return NEW(LilyCheckedScopeResponse);
}

LilyCheckedScopeResponse
search_class_in_current_scope__LilyCheckedScope(LilyCheckedScope *self,
                                                const String *name)
{
    if (self->classes) {
        switch (self->decls.kind) {
            case LILY_CHECKED_SCOPE_DECLS_KIND_MODULE: {
                LilyCheckedScopeContainerClass *class =
                  get__HashMap(self->classes, name->buffer);

                if (class) {
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

                return NEW(LilyCheckedScopeResponse);
            }
            default:
                return NEW(LilyCheckedScopeResponse);
        }
    }

    return NEW(LilyCheckedScopeResponse);
}

LilyCheckedScopeResponse
search_record_object_in_current_scope__LilyCheckedScope(LilyCheckedScope *self,
                                                        const String *name)
{
    if (self->records_object) {
        switch (self->decls.kind) {
            case LILY_CHECKED_SCOPE_DECLS_KIND_MODULE: {
                LilyCheckedScopeContainerRecordObject *record_object =
                  get__HashMap(self->records_object, name->buffer);

                if (record_object) {
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

                return NEW(LilyCheckedScopeResponse);
            }
            default:
                return NEW(LilyCheckedScopeResponse);
        }
    }

    return NEW(LilyCheckedScopeResponse);
}

LilyCheckedScopeResponse
search_enum_object_in_current_scope__LilyCheckedScope(LilyCheckedScope *self,
                                                      const String *name)
{
    if (self->enums_object) {
        switch (self->decls.kind) {
            case LILY_CHECKED_SCOPE_DECLS_KIND_MODULE: {
                LilyCheckedScopeContainerEnumObject *enum_object =
                  get__HashMap(self->enums_object, name->buffer);

                if (enum_object) {
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

                return NEW(LilyCheckedScopeResponse);
            }
            default:
                return NEW(LilyCheckedScopeResponse);
        }
    }

    return NEW(LilyCheckedScopeResponse);
}

LilyCheckedScopeResponse
search_trait_in_current_scope__LilyCheckedScope(LilyCheckedScope *self,
                                                const String *name)
{
    if (self->traits) {
        switch (self->decls.kind) {
            case LILY_CHECKED_SCOPE_DECLS_KIND_MODULE: {
                LilyCheckedScopeContainerTrait *trait =
                  get__HashMap(self->traits, name->buffer);

                if (trait) {
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

                return NEW(LilyCheckedScopeResponse);
            }
            default:
                return NEW(LilyCheckedScopeResponse);
        }
    }

    return NEW(LilyCheckedScopeResponse);
}

LilyCheckedScopeResponse
search_module__LilyCheckedScope(LilyCheckedScope *self, const String *name)
{
    LilyCheckedScopeResponse response =
      search_module_in_current_scope__LilyCheckedScope(self, name);

    switch (response.kind) {
        case LILY_CHECKED_SCOPE_RESPONSE_KIND_NOT_FOUND:
            return self->parent ? search_module__LilyCheckedScope(
                                    self->parent->scope, name)
                                : NEW(LilyCheckedScopeResponse);
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
            return self->parent && (self->parent->scope->decls.kind ==
                                    LILY_CHECKED_SCOPE_DECLS_KIND_SCOPE)
                     ? search_variable__LilyCheckedScope(self->parent->scope,
                                                         name)
                     : NEW(LilyCheckedScopeResponse);
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
            return self->parent
                     ? search_fun__LilyCheckedScope(self->parent->scope, name)
                     : NEW(LilyCheckedScopeResponse);
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
            return self->parent ? search_constant__LilyCheckedScope(
                                    self->parent->scope, name)
                                : NEW(LilyCheckedScopeResponse);
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
            return self->parent
                     ? search_error__LilyCheckedScope(self->parent->scope, name)
                     : NEW(LilyCheckedScopeResponse);
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
            return self->parent
                     ? search_alias__LilyCheckedScope(self->parent->scope, name)
                     : NEW(LilyCheckedScopeResponse);
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
            return self->parent ? search_record__LilyCheckedScope(
                                    self->parent->scope, name)
                                : NEW(LilyCheckedScopeResponse);
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
            return self->parent
                     ? search_enum__LilyCheckedScope(self->parent->scope, name)
                     : NEW(LilyCheckedScopeResponse);
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
            return self->parent ? search_generic__LilyCheckedScope(
                                    self->parent->scope, name)
                                : NEW(LilyCheckedScopeResponse);
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
            return self->parent
                     ? search_class__LilyCheckedScope(self->parent->scope, name)
                     : NEW(LilyCheckedScopeResponse);
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
            return self->parent ? search_record_object__LilyCheckedScope(
                                    self->parent->scope, name)
                                : NEW(LilyCheckedScopeResponse);
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
            return self->parent ? search_enum_object__LilyCheckedScope(
                                    self->parent->scope, name)
                                : NEW(LilyCheckedScopeResponse);
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
            return self->parent
                     ? search_trait__LilyCheckedScope(self->parent->scope, name)
                     : NEW(LilyCheckedScopeResponse);
        default:
            return response;
    }
}

LilyCheckedScopeResponse
search_field__LilyCheckedScope(LilyCheckedScope *self, const String *name)
{
    if (self->variables) {
        switch (self->decls.kind) {
            case LILY_CHECKED_SCOPE_DECLS_KIND_DECL:
                if (self->decls.decl->kind == LILY_CHECKED_DECL_KIND_TYPE) {
                    switch (self->decls.decl->type.kind) {
                        case LILY_CHECKED_DECL_TYPE_KIND_RECORD: {
                            LilyCheckedScopeContainerVariable *variable =
                              get__HashMap(self->variables, name->buffer);

                            if (variable) {
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

                            break;
                        }
                        default:
                            break;
                    }
                } else if (self->decls.decl->kind ==
                           LILY_CHECKED_DECL_KIND_OBJECT) {
                    switch (self->decls.decl->object.kind) {
                        case LILY_CHECKED_DECL_OBJECT_KIND_RECORD: {
                            LilyCheckedScopeContainerVariable *variable =
                              get__HashMap(self->variables, name->buffer);

                            if (variable) {
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

                            break;
                        }
                        default:
                            break;
                    }
                }

                return NEW(LilyCheckedScopeResponse);
            default:
                return NEW(LilyCheckedScopeResponse);
        }
    }

    return NEW(LilyCheckedScopeResponse);
}

LilyCheckedScopeResponse
search_variant__LilyCheckedScope(LilyCheckedScope *self, const String *name)
{
    if (self->variables) {
        switch (self->decls.kind) {
            case LILY_CHECKED_SCOPE_DECLS_KIND_DECL:
                if (self->decls.decl->kind == LILY_CHECKED_DECL_KIND_TYPE) {
                    switch (self->decls.decl->type.kind) {
                        case LILY_CHECKED_DECL_TYPE_KIND_ENUM: {
                            LilyCheckedScopeContainerVariable *variable =
                              get__HashMap(self->variables, name->buffer);

                            if (variable) {
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

                            break;
                        }
                        default:
                            break;
                    }
                } else if (self->decls.decl->kind ==
                           LILY_CHECKED_DECL_KIND_OBJECT) {
                    switch (self->decls.decl->object.kind) {
                        case LILY_CHECKED_DECL_OBJECT_KIND_ENUM: {
                            LilyCheckedScopeContainerVariable *variable =
                              get__HashMap(self->variables, name->buffer);

                            if (variable) {
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

                            break;
                        }
                        default:
                            break;
                    }
                }

                return NEW(LilyCheckedScopeResponse);
            default:
                return NEW(LilyCheckedScopeResponse);
        }
    }

    return NEW(LilyCheckedScopeResponse);
}

LilyCheckedScopeResponse
search_identifier__LilyCheckedScope(LilyCheckedScope *self, const String *name)
{
    LilyCheckedScopeResponse variable =
      search_variable_in_current_scope__LilyCheckedScope(self, name);
    LilyCheckedScopeResponse param =
      search_param_in_current_scope__LilyCheckedScope(self, name);
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
    LilyCheckedScopeResponse error =
      search_error_in_current_scope__LilyCheckedScope(self, name);
    LilyCheckedScopeResponse captured_variable =
      search_captured_variable_in_current_scope__LilyCheckedScope(self, name);

    // [variable, param, fun, module, constant, field, variant, enum, error,
    // captured_variable]
#define RESPONSES_IDENTIFIER_LEN 10
    LilyCheckedScopeResponse *responses[RESPONSES_IDENTIFIER_LEN] =
      (LilyCheckedScopeResponse *[RESPONSES_IDENTIFIER_LEN]){
          &variable, &param,   &fun,   &module, &constant,
          &field,    &variant, &enum_, &error,  &captured_variable
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

    if (self->catch && index_with_largest_id == -1) {
        if (!strcmp(name->buffer, self->catch->name->buffer)) {
            return NEW_VARIANT(LilyCheckedScopeResponse,
                               catch_variable,
                               self->catch->location,
                               self->catch->raises);
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
    LilyCheckedScopeResponse alias =
      search_alias_in_current_scope__LilyCheckedScope(self, name);

    // [record, enum, generic, class, record_object, enum_object, trait, alias]
#define RESPONSES_CUSTOM_TYPE_LEN 8
    LilyCheckedScopeResponse *responses[RESPONSES_CUSTOM_TYPE_LEN] =
      (LilyCheckedScopeResponse *[RESPONSES_CUSTOM_TYPE_LEN]){
          &record,        &enum_,       &generic, &class,
          &record_object, &enum_object, &trait,   &alias
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

LilyCheckedScopeResponse
search_from_scope_id__LilyCheckedScope(
  LilyCheckedScope *self,
  Usize id,
  const String *name,
  enum LilyCheckedScopeResponseKind res_kind)
{
    LilyCheckedScope *current_scope =
      safe_get_scope_from_id__LilyCheckedScope(self, id);

    if (!current_scope) {
        return NEW(LilyCheckedScopeResponse);
    }

    switch (res_kind) {
        case LILY_CHECKED_SCOPE_RESPONSE_KIND_MODULE:
            return search_module_in_current_scope__LilyCheckedScope(
              current_scope, name);
        case LILY_CHECKED_SCOPE_RESPONSE_KIND_CONSTANT:
            return search_constant_in_current_scope__LilyCheckedScope(
              current_scope, name);
        case LILY_CHECKED_SCOPE_RESPONSE_KIND_CATCH_VARIABLE:
            TODO("catch variable");
        case LILY_CHECKED_SCOPE_RESPONSE_KIND_CAPTURED_VARIABLE:
            return search_captured_variable_in_current_scope__LilyCheckedScope(
              current_scope, name);
        case LILY_CHECKED_SCOPE_RESPONSE_KIND_ENUM:
            return search_enum_in_current_scope__LilyCheckedScope(current_scope,
                                                                  name);
        case LILY_CHECKED_SCOPE_RESPONSE_KIND_ENUM_VARIANT:
        case LILY_CHECKED_SCOPE_RESPONSE_KIND_ENUM_VARIANT_OBJECT:
            return search_variant__LilyCheckedScope(current_scope, name);
        case LILY_CHECKED_SCOPE_RESPONSE_KIND_RECORD:
            return search_record_in_current_scope__LilyCheckedScope(
              current_scope, name);
        case LILY_CHECKED_SCOPE_RESPONSE_KIND_RECORD_FIELD:
        case LILY_CHECKED_SCOPE_RESPONSE_KIND_RECORD_FIELD_OBJECT:
            return search_field__LilyCheckedScope(current_scope, name);
        case LILY_CHECKED_SCOPE_RESPONSE_KIND_ALIAS:
            return search_alias_in_current_scope__LilyCheckedScope(
              current_scope, name);
        case LILY_CHECKED_SCOPE_RESPONSE_KIND_ERROR:
            return search_error_in_current_scope__LilyCheckedScope(
              current_scope, name);
        case LILY_CHECKED_SCOPE_RESPONSE_KIND_ENUM_OBJECT:
            return search_enum_object_in_current_scope__LilyCheckedScope(
              current_scope, name);
        case LILY_CHECKED_SCOPE_RESPONSE_KIND_RECORD_OBJECT:
            return search_record_object_in_current_scope__LilyCheckedScope(
              current_scope, name);
        case LILY_CHECKED_SCOPE_RESPONSE_KIND_CLASS:
            return search_class_in_current_scope__LilyCheckedScope(
              current_scope, name);
        case LILY_CHECKED_SCOPE_RESPONSE_KIND_TRAIT:
            return search_trait_in_current_scope__LilyCheckedScope(
              current_scope, name);
        case LILY_CHECKED_SCOPE_RESPONSE_KIND_FUN:
            return search_fun_in_current_scope__LilyCheckedScope(current_scope,
                                                                 name);
        case LILY_CHECKED_SCOPE_RESPONSE_KIND_LABEL:
            TODO("label");
        case LILY_CHECKED_SCOPE_RESPONSE_KIND_VARIABLE:
            return search_variable_in_current_scope__LilyCheckedScope(
              current_scope, name);
        case LILY_CHECKED_SCOPE_RESPONSE_KIND_FUN_PARAM:
        case LILY_CHECKED_SCOPE_RESPONSE_KIND_METHOD_PARAM:
            return search_param_in_current_scope__LilyCheckedScope(
              current_scope, name);
        case LILY_CHECKED_SCOPE_RESPONSE_KIND_GENERIC:
            return search_generic_in_current_scope__LilyCheckedScope(
              current_scope, name);
        case LILY_CHECKED_SCOPE_RESPONSE_KIND_NOT_FOUND:
            return NEW(LilyCheckedScopeResponse);
        default:
            UNREACHABLE("unknown variant");
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

    if (self->variables) {
        switch (scope->decls.kind) {
            case LILY_CHECKED_SCOPE_DECLS_KIND_SCOPE: {
                LilyCheckedScopeContainerVariable *variable =
                  get__HashMap(self->variables, name->buffer);

                if (variable) {
                    return get__Vec(scope->decls.scope, variable->id);
                }

                UNREACHABLE("the scope have a bug!!");
            }
            default:
                return NULL;
        }
    }

    return NULL;
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

void
set_catch_name__LilyCheckedScope(LilyCheckedScope *self,
                                 String *catch_name,
                                 const Location *location,
                                 HashMap *raises)
{
    ASSERT(!self->catch);

    self->catch = NEW(LilyCheckedScopeCatch, catch_name, location, raises);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyCheckedScope, const LilyCheckedScope *self)
{
    String *res =
      format__String("LilyCheckedScope{{ id = {d}, raises =", self->id);

    if (self->raises) {
        DEBUG_HASH_MAP_STRING(self->raises, res, LilyCheckedDataType);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, ", catch = ");

    if (self->catch) {
        String *s = to_string__Debug__LilyCheckedScopeCatch(self->catch);

        APPEND_AND_FREE(res, s);
    } else {
        push_str__String(res, "NULL");
    }

    push_str__String(res, ", captured_variables =");

    if (self->captured_variables) {
        DEBUG_HASH_MAP_STR(self->captured_variables,
                           res,
                           LilyCheckedScopeContainerCapturedVariable);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, ", modules =");

    if (self->modules) {
        DEBUG_HASH_MAP_STR(self->modules, res, LilyCheckedScopeContainerModule);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, ", constants =");

    if (self->constants) {
        DEBUG_HASH_MAP_STR(
          self->constants, res, LilyCheckedScopeContainerConstant);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, ", enums =");

    if (self->enums) {
        DEBUG_HASH_MAP_STR(self->enums, res, LilyCheckedScopeContainerEnum);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, ", records =");

    if (self->records) {
        DEBUG_HASH_MAP_STR(self->records, res, LilyCheckedScopeContainerRecord);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, ", aliases =");

    if (self->aliases) {
        DEBUG_HASH_MAP_STR(self->aliases, res, LilyCheckedScopeContainerAlias);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, ", errors =");

    if (self->errors) {
        DEBUG_HASH_MAP_STR(self->errors, res, LilyCheckedScopeContainerError);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, ", enums_object =");

    if (self->enums_object) {
        DEBUG_HASH_MAP_STR(
          self->enums_object, res, LilyCheckedScopeContainerEnumObject);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, ", records_object =");

    if (self->records_object) {
        DEBUG_HASH_MAP_STR(
          self->records_object, res, LilyCheckedScopeContainerRecordObject);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, ", classes =");

    if (self->classes) {
        DEBUG_HASH_MAP_STR(self->classes, res, LilyCheckedScopeContainerClass);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, ", traits =");

    if (self->traits) {
        DEBUG_HASH_MAP_STR(self->traits, res, LilyCheckedScopeContainerTrait);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, ", funs =");

    if (self->funs) {
        DEBUG_VEC_STRING(self->funs, res, LilyCheckedScopeContainerFun);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, ", labels =");

    if (self->labels) {
        DEBUG_HASH_MAP_STR(self->labels, res, LilyCheckedScopeContainerLabel);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, ", variables =");

    if (self->variables) {
        DEBUG_HASH_MAP_STR(
          self->variables, res, LilyCheckedScopeContainerVariable);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, ", params =");

    if (self->params) {
        DEBUG_HASH_MAP_STR(
          self->params, res, LilyCheckedScopeContainerVariable);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, ", generics =");

    if (self->params) {
        DEBUG_HASH_MAP_STR(self->params, res, LilyCheckedScopeContainerGeneric);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, ", methods =");

    if (self->methods) {
        DEBUG_VEC_STRING(self->methods, res, LilyCheckedScopeContainerMethod);
    } else {
        push_str__String(res, " NULL");
    }

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
    if (self->raises) {
        FREE_HASHMAP_VALUES(self->raises, LilyCheckedDataType);
        FREE(HashMap, self->raises);
    }

    if (self->catch) {
        FREE(LilyCheckedScopeCatch, self->catch);
    }

    if (self->captured_variables) {
        FREE_HASHMAP_VALUES(self->captured_variables,
                            LilyCheckedScopeContainerCapturedVariable);
        FREE(HashMap, self->captured_variables);
    }

    if (self->modules) {
        FREE_HASHMAP_VALUES(self->modules, LilyCheckedScopeContainerModule);
        FREE(HashMap, self->modules);
    }

    if (self->constants) {
        FREE_HASHMAP_VALUES(self->constants, LilyCheckedScopeContainerConstant);
        FREE(HashMap, self->constants);
    }

    if (self->enums) {
        FREE_HASHMAP_VALUES(self->enums, LilyCheckedScopeContainerEnum);
        FREE(HashMap, self->enums);
    }

    if (self->records) {
        FREE_HASHMAP_VALUES(self->records, LilyCheckedScopeContainerRecord);
        FREE(HashMap, self->records);
    }

    if (self->aliases) {
        FREE_HASHMAP_VALUES(self->aliases, LilyCheckedScopeContainerAlias);
        FREE(HashMap, self->aliases);
    }

    if (self->errors) {
        FREE_HASHMAP_VALUES(self->errors, LilyCheckedScopeContainerError);
        FREE(HashMap, self->errors);
    }

    if (self->enums_object) {
        FREE_HASHMAP_VALUES(self->enums_object,
                            LilyCheckedScopeContainerEnumObject);
        FREE(HashMap, self->enums_object);
    }

    if (self->records_object) {
        FREE_HASHMAP_VALUES(self->records_object,
                            LilyCheckedScopeContainerRecordObject);
        FREE(HashMap, self->records_object);
    }

    if (self->classes) {
        FREE_HASHMAP_VALUES(self->classes, LilyCheckedScopeContainerClass);
        FREE(HashMap, self->classes);
    }

    if (self->traits) {
        FREE_HASHMAP_VALUES(self->traits, LilyCheckedScopeContainerTrait);
        FREE(HashMap, self->traits);
    }

    if (self->labels) {
        FREE_HASHMAP_VALUES(self->labels, LilyCheckedScopeContainerLabel);
        FREE(HashMap, self->labels);
    }

    if (self->funs) {
        FREE_BUFFER_ITEMS(
          self->funs->buffer, self->funs->len, LilyCheckedScopeContainerFun);
        FREE(Vec, self->funs);
    }

    if (self->variables) {
        FREE_HASHMAP_VALUES(self->variables, LilyCheckedScopeContainerVariable);
        FREE(HashMap, self->variables);
    }

    if (self->params) {
        FREE_HASHMAP_VALUES(self->params, LilyCheckedScopeContainerVariable);
        FREE(HashMap, self->params);
    }

    if (self->generics) {
        FREE_HASHMAP_VALUES(self->generics, LilyCheckedScopeContainerGeneric);
        FREE(HashMap, self->generics);
    }

    if (self->methods) {
        FREE_BUFFER_ITEMS(self->methods->buffer,
                          self->methods->len,
                          LilyCheckedScopeContainerMethod);
        FREE(Vec, self->methods);
    }

    if (self->parent) {
        FREE(LilyCheckedParent, self->parent);
    }

    lily_free(self);
}
