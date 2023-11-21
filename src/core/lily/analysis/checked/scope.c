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

#include <base/alloc.h>
#include <base/macros.h>

#include <core/lily/analysis/checked/case_verification.h>
#include <core/lily/analysis/checked/decl/module.h>
#include <core/lily/analysis/checked/scope.h>

#include <stdio.h>
#include <stdlib.h>

static LilyCheckedSymbol *
search_variable_in_current_scope__LilyCheckedScopeLocal(
  const LilyCheckedScopeLocal *self,
  char *name);

static LilyCheckedSymbol *
search_param_in_current_scope__LilyCheckedScopeLocal(
  const LilyCheckedScopeLocal *self,
  char *name);

static LilyCheckedSymbol *
search_label_in_current_scope__LilyCheckedScopeLocal(
  const LilyCheckedScopeLocal *self,
  char *name);

static LilyCheckedSymbol *
search_fun_in_current_scope__LilyCheckedScopeLocal(
  const LilyCheckedScopeLocal *self,
  char *name);

static LilyCheckedSymbol *
search_constant_in_current_scope__LilyCheckedScope(const LilyCheckedScope *self,
                                                   char *name);

static LilyCheckedSymbol *
search_enum_in_current_scope__LilyCheckedScope(const LilyCheckedScope *self,
                                               char *name);

static LilyCheckedSymbol *
search_record_in_current_scope__LilyCheckedScope(const LilyCheckedScope *self,
                                                 char *name);

static LilyCheckedSymbol *
search_alias_in_current_scope__LilyCheckedScope(const LilyCheckedScope *self,
                                                char *name);

static LilyCheckedSymbol *
search_error_in_current_scope__LilyCheckedScope(const LilyCheckedScope *self,
                                                char *name);

static LilyCheckedSymbol *
search_enum_object_in_current_scope__LilyCheckedScope(
  const LilyCheckedScope *self,
  char *name);

static LilyCheckedSymbol *
search_module_in_current_scope__LilyCheckedScope(const LilyCheckedScope *self,
                                                 char *name);

static LilyCheckedSymbol *
search_record_object_in_current_scope__LilyCheckedScope(
  const LilyCheckedScope *self,
  char *name);

static LilyCheckedSymbol *
search_class_in_current_scope__LilyCheckedScope(const LilyCheckedScope *self,
                                                char *name);

static LilyCheckedSymbol *
search_trait_in_current_scope__LilyCheckedScope(const LilyCheckedScope *self,
                                                char *name);

static LilyCheckedSymbol *
search_fun_in_current_scope__LilyCheckedScope(const LilyCheckedScope *self,
                                              char *name);

#define SEARCH_IN_CURRENT_SCOPE(hm)    \
    if (hm) {                          \
        return get__HashMap(hm, name); \
    }                                  \
                                       \
    return NULL;

#define SEARCH_LOCAL(k)                                                 \
    LilyCheckedSymbol *symb =                                           \
      search_##k##_in_current_scope__LilyCheckedScopeLocal(self, name); \
                                                                        \
    if (symb) {                                                         \
        return symb;                                                    \
    } else if (self->parent) {                                          \
        return search_##k##__LilyCheckedScopeLocal(self->parent, name); \
    }                                                                   \
                                                                        \
    return NULL;

#define VALID_SNAKE_CASE()                                   \
    if (valid_snake_case__LilyCheckedCaseVerification(name)) \
        return LILY_CHECKED_SCOPE_ADD_STATUS_EXPECTED_SNAKE_CASE;
#define VALID_PASCAL_CASE()                                        \
    if (valid_upper_snake_case__LilyCheckedCaseVerification(name)) \
        return LILY_CHECKED_SCOPE_ADD_STATUS_EXPECTED_PASCAL_CASE;
#define VALID_UPPER_SNAKE_CASE()                                   \
    if (valid_upper_snake_case__LilyCheckedCaseVerification(name)) \
        return LILY_CHECKED_SCOPE_ADD_STATUS_EXPECTED_UPPER_SNAKE_CASE;

#define HASH_MAP_CHECK_IF_EXISTS(hm)                        \
    if (hm) {                                               \
        if (get__HashMap(hm, name)) {                       \
            return LILY_CHECKED_SCOPE_ADD_STATUS_DUPLICATE; \
        }                                                   \
    }

#define HASH_MAP_ADD_TO_SCOPE(hm, kind, symbol_scope)                \
    HASH_MAP_CHECK_IF_EXISTS(hm);                                    \
                                                                     \
    if (!hm) {                                                       \
        hm = NEW(HashMap);                                           \
    }                                                                \
                                                                     \
    insert__HashMap(                                                 \
      hm, name, NEW_VARIANT(LilyCheckedSymbol, kind, symbol_scope)); \
                                                                     \
    return LILY_CHECKED_SCOPE_ADD_STATUS_SUCCESS;

CONSTRUCTOR(LilyCheckedScopeLocal *,
            LilyCheckedScopeLocal,
            const LilyCheckedDeclModule *module,
            Usize id,
            LilyCheckedScopeLocal *parent)
{
    LilyCheckedScopeLocal *self = lily_malloc(sizeof(LilyCheckedScopeLocal));

    self->module = module;
    self->id = id;
    self->variables = NULL;
    self->params = NULL;
    self->labels = NULL;
    self->funs = NULL;
    self->parent = parent;

    return self;
}

enum LilyCheckedScopeAddStatus
add_variable__LilyCheckedScopeLocal(LilyCheckedScopeLocal *self,
                                    char *name,
                                    Usize scope_id,
                                    Usize id)
{
    VALID_SNAKE_CASE();
    HASH_MAP_CHECK_IF_EXISTS(self->params);
    HASH_MAP_CHECK_IF_EXISTS(self->labels);
    HASH_MAP_CHECK_IF_EXISTS(self->funs);
    HASH_MAP_ADD_TO_SCOPE(
      self->variables, variable, NEW(LilyCheckedSymbolVariable, scope_id, id));
}

enum LilyCheckedScopeAddStatus
add_param__LilyCheckedScopeLocal(LilyCheckedScopeLocal *self,
                                 char *name,
                                 Usize id)
{
    VALID_SNAKE_CASE();
    HASH_MAP_CHECK_IF_EXISTS(self->variables);
    HASH_MAP_CHECK_IF_EXISTS(self->labels);
    HASH_MAP_CHECK_IF_EXISTS(self->funs);
    HASH_MAP_ADD_TO_SCOPE(self->params, param, NEW(LilyCheckedSymbolParam, id));
}

enum LilyCheckedScopeAddStatus
add_label__LilyCheckedScopeLocal(LilyCheckedScopeLocal *self,
                                 char *name,
                                 Usize scope_id,
                                 Usize id)
{
    VALID_SNAKE_CASE();
    HASH_MAP_CHECK_IF_EXISTS(self->variables);
    HASH_MAP_CHECK_IF_EXISTS(self->params);
    HASH_MAP_CHECK_IF_EXISTS(self->funs);
    HASH_MAP_ADD_TO_SCOPE(
      self->labels, label, NEW(LilyCheckedSymbolLabel, scope_id, id));
}

enum LilyCheckedScopeAddStatus
add_fun__LilyCheckedScopeLocal(LilyCheckedScopeLocal *self,
                               char *name,
                               Usize package_id,
                               Usize id)
{
    VALID_SNAKE_CASE();
    HASH_MAP_CHECK_IF_EXISTS(self->variables);
    HASH_MAP_CHECK_IF_EXISTS(self->params);
    HASH_MAP_CHECK_IF_EXISTS(self->labels);
    HASH_MAP_ADD_TO_SCOPE(
      self->funs, fun, NEW(LilyCheckedSymbolFun, package_id, id));
}

LilyCheckedSymbol *
search_variable_in_current_scope__LilyCheckedScopeLocal(
  const LilyCheckedScopeLocal *self,
  char *name)
{
    SEARCH_IN_CURRENT_SCOPE(self->variables);
}

LilyCheckedSymbol *
search_param_in_current_scope__LilyCheckedScopeLocal(
  const LilyCheckedScopeLocal *self,
  char *name)
{
    SEARCH_IN_CURRENT_SCOPE(self->params);
}

LilyCheckedSymbol *
search_label_in_current_scope__LilyCheckedScopeLocal(
  const LilyCheckedScopeLocal *self,
  char *name)
{
    SEARCH_IN_CURRENT_SCOPE(self->labels);
}

LilyCheckedSymbol *
search_fun_in_current_scope__LilyCheckedScopeLocal(
  const LilyCheckedScopeLocal *self,
  char *name)
{
    SEARCH_IN_CURRENT_SCOPE(self->funs);
}

LilyCheckedSymbol *
search_variable__LilyCheckedScopeLocal(const LilyCheckedScopeLocal *self,
                                       char *name)
{
    SEARCH_LOCAL(variable);
}

LilyCheckedSymbol *
search_param__LilyCheckedScopeLocal(const LilyCheckedScopeLocal *self,
                                    char *name)
{
    SEARCH_LOCAL(param);
}

LilyCheckedSymbol *
search_label__LilyCheckedScopeLocal(const LilyCheckedScopeLocal *self,
                                    char *name)
{
    SEARCH_LOCAL(label);
}

LilyCheckedSymbol *
search_fun__LilyCheckedScopeLocal(const LilyCheckedScopeLocal *self, char *name)
{
    SEARCH_LOCAL(fun);
}

LilyCheckedSymbol *
search_identifier__LilyCheckedScopeLocal(const LilyCheckedScopeLocal *self,
                                         char *name)
{
    LilyCheckedSymbol *variable_symbol =
      search_variable_in_current_scope__LilyCheckedScopeLocal(self, name);
    LilyCheckedSymbol *param_symbol =
      search_param_in_current_scope__LilyCheckedScopeLocal(self, name);
    LilyCheckedSymbol *label_symbol =
      search_label_in_current_scope__LilyCheckedScopeLocal(self, name);
    LilyCheckedSymbol *fun_symbol =
      search_fun_in_current_scope__LilyCheckedScopeLocal(self, name);

#define SYMBOLS_LOCAL_IDENTIFIER_LEN 4
    LilyCheckedSymbol *symbols[SYMBOLS_LOCAL_IDENTIFIER_LEN] =
      (LilyCheckedSymbol *[SYMBOLS_LOCAL_IDENTIFIER_LEN]){
          variable_symbol, param_symbol, label_symbol, fun_symbol
      };
    int symbol_index = -1;

    for (Usize i = 0; i < SYMBOLS_LOCAL_IDENTIFIER_LEN; ++i) {
        LilyCheckedSymbol *symbol = symbols[i];

        if (symbol) {
            switch (symbol->kind) {
                case LILY_CHECKED_SYMBOL_KIND_FUN:
                case LILY_CHECKED_SYMBOL_KIND_LABEL:
                case LILY_CHECKED_SYMBOL_KIND_PARAM:
                case LILY_CHECKED_SYMBOL_KIND_VARIABLE:
                    if (symbol_index == -1) {
                        symbol_index = i;
                        goto loop_exit;
                    }

                    continue;
                default:
                    UNREACHABLE("this variant is not expected");
            }
        }
    }

loop_exit : {
    return symbol_index == -1
             ? self->parent
                 ? search_identifier__LilyCheckedScopeLocal(self->parent, name)
                 : /* TODO: Search in module */ NULL
             : symbols[symbol_index];
}
}

DESTRUCTOR(LilyCheckedScopeLocal, LilyCheckedScopeLocal *self)
{
    if (self->variables) {
        FREE_HASHMAP_VALUES(self->variables, LilyCheckedSymbol);
        FREE(HashMap, self->variables);
    }

    if (self->params) {
        FREE_HASHMAP_VALUES(self->params, LilyCheckedSymbol);
        FREE(HashMap, self->params);
    }

    if (self->labels) {
        FREE_HASHMAP_VALUES(self->labels, LilyCheckedSymbol);
        FREE(HashMap, self->labels);
    }

    if (self->funs) {
        FREE_HASHMAP_VALUES(self->funs, LilyCheckedSymbol);
        FREE(HashMap, self->funs);
    }

    lily_free(self);
}

CONSTRUCTOR(LilyCheckedScope *, LilyCheckedScope, LilyCheckedSymbol *parent)
{
    LilyCheckedScope *self = lily_malloc(sizeof(LilyCheckedScope));

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
    self->parent = parent;
    self->children = NULL;

    return self;
}

enum LilyCheckedScopeAddStatus
add_constant__LilyCheckedScope(LilyCheckedScope *self,
                               char *name,
                               Usize package_id,
                               Usize id,
                               enum LilyVisibility visibility)
{
    VALID_UPPER_SNAKE_CASE();
    HASH_MAP_CHECK_IF_EXISTS(self->funs);
    HASH_MAP_ADD_TO_SCOPE(self->constants,
                          constant,
                          NEW(LilyCheckedSymbolConstant, package_id, id));
}

enum LilyCheckedScopeAddStatus
add_enum__LilyCheckedScope(LilyCheckedScope *self,
                           char *name,
                           Usize id,
                           enum LilyVisibility visibility)
{
    VALID_PASCAL_CASE();
    HASH_MAP_CHECK_IF_EXISTS(self->records);
    HASH_MAP_CHECK_IF_EXISTS(self->aliases);
    HASH_MAP_CHECK_IF_EXISTS(self->errors);
    HASH_MAP_CHECK_IF_EXISTS(self->enums_object);
    HASH_MAP_CHECK_IF_EXISTS(self->modules);
    HASH_MAP_CHECK_IF_EXISTS(self->records_object);
    HASH_MAP_CHECK_IF_EXISTS(self->classes);
    HASH_MAP_CHECK_IF_EXISTS(self->traits);
    HASH_MAP_ADD_TO_SCOPE(
      self->enums,
      LILY_CHECKED_SYMBOL_KIND_ENUM,
      visibility,
      NEW_VARIANT(LilyCheckedSymbolScope,
                  module,
                  NEW(LilyCheckedSymbolScopeModule, self->id, id)));
}

enum LilyCheckedScopeAddStatus
add_record__LilyCheckedScope(LilyCheckedScope *self,
                             char *name,
                             Usize id,
                             enum LilyVisibility visibility)
{
    VALID_PASCAL_CASE();
    HASH_MAP_CHECK_IF_EXISTS(self->enums);
    HASH_MAP_CHECK_IF_EXISTS(self->aliases);
    HASH_MAP_CHECK_IF_EXISTS(self->errors);
    HASH_MAP_CHECK_IF_EXISTS(self->enums_object);
    HASH_MAP_CHECK_IF_EXISTS(self->modules);
    HASH_MAP_CHECK_IF_EXISTS(self->records_object);
    HASH_MAP_CHECK_IF_EXISTS(self->classes);
    HASH_MAP_CHECK_IF_EXISTS(self->traits);
    HASH_MAP_ADD_TO_SCOPE(
      self->records,
      LILY_CHECKED_SYMBOL_KIND_RECORD,
      visibility,
      NEW_VARIANT(LilyCheckedSymbolScope,
                  module,
                  NEW(LilyCheckedSymbolScopeModule, self->id, id)));
}

enum LilyCheckedScopeAddStatus
add_alias__LilyCheckedScope(LilyCheckedScope *self,
                            char *name,
                            Usize id,
                            enum LilyVisibility visibility)
{
    VALID_PASCAL_CASE();
    HASH_MAP_CHECK_IF_EXISTS(self->enums);
    HASH_MAP_CHECK_IF_EXISTS(self->records);
    HASH_MAP_CHECK_IF_EXISTS(self->errors);
    HASH_MAP_CHECK_IF_EXISTS(self->enums_object);
    HASH_MAP_CHECK_IF_EXISTS(self->modules);
    HASH_MAP_CHECK_IF_EXISTS(self->records_object);
    HASH_MAP_CHECK_IF_EXISTS(self->classes);
    HASH_MAP_CHECK_IF_EXISTS(self->traits);
    HASH_MAP_ADD_TO_SCOPE(
      self->aliases,
      LILY_CHECKED_SYMBOL_KIND_ALIAS,
      visibility,
      NEW_VARIANT(LilyCheckedSymbolScope,
                  module,
                  NEW(LilyCheckedSymbolScopeModule, self->id, id)));
}

enum LilyCheckedScopeAddStatus
add_error__LilyCheckedScope(LilyCheckedScope *self,
                            char *name,
                            Usize id,
                            enum LilyVisibility visibility)
{
    VALID_PASCAL_CASE();
    HASH_MAP_CHECK_IF_EXISTS(self->enums);
    HASH_MAP_CHECK_IF_EXISTS(self->records);
    HASH_MAP_CHECK_IF_EXISTS(self->aliases);
    HASH_MAP_CHECK_IF_EXISTS(self->enums_object);
    HASH_MAP_CHECK_IF_EXISTS(self->modules);
    HASH_MAP_CHECK_IF_EXISTS(self->records_object);
    HASH_MAP_CHECK_IF_EXISTS(self->classes);
    HASH_MAP_CHECK_IF_EXISTS(self->traits);
    HASH_MAP_ADD_TO_SCOPE(
      self->errors,
      LILY_CHECKED_SYMBOL_KIND_ERROR,
      visibility,
      NEW_VARIANT(LilyCheckedSymbolScope,
                  module,
                  NEW(LilyCheckedSymbolScopeModule, self->id, id)));
}

enum LilyCheckedScopeAddStatus
add_enum_object__LilyCheckedScope(LilyCheckedScope *self,
                                  char *name,
                                  Usize id,
                                  enum LilyVisibility visibility)
{
    VALID_PASCAL_CASE();
    HASH_MAP_CHECK_IF_EXISTS(self->enums);
    HASH_MAP_CHECK_IF_EXISTS(self->records);
    HASH_MAP_CHECK_IF_EXISTS(self->aliases);
    HASH_MAP_CHECK_IF_EXISTS(self->errors);
    HASH_MAP_CHECK_IF_EXISTS(self->modules);
    HASH_MAP_CHECK_IF_EXISTS(self->records_object);
    HASH_MAP_CHECK_IF_EXISTS(self->classes);
    HASH_MAP_CHECK_IF_EXISTS(self->traits);
    HASH_MAP_ADD_TO_SCOPE(
      self->enums_object,
      LILY_CHECKED_SYMBOL_KIND_ENUM_OBJECT,
      visibility,
      NEW_VARIANT(LilyCheckedSymbolScope,
                  module,
                  NEW(LilyCheckedSymbolScopeModule, self->id, id)));
}

enum LilyCheckedScopeAddStatus
add_module__LilyCheckedScope(LilyCheckedScope *self,
                             char *name,
                             Usize id,
                             enum LilyVisibility visibility)
{
    VALID_PASCAL_CASE();
    HASH_MAP_CHECK_IF_EXISTS(self->enums);
    HASH_MAP_CHECK_IF_EXISTS(self->records);
    HASH_MAP_CHECK_IF_EXISTS(self->aliases);
    HASH_MAP_CHECK_IF_EXISTS(self->errors);
    HASH_MAP_CHECK_IF_EXISTS(self->enums_object);
    HASH_MAP_CHECK_IF_EXISTS(self->records_object);
    HASH_MAP_CHECK_IF_EXISTS(self->classes);
    HASH_MAP_CHECK_IF_EXISTS(self->traits);
    HASH_MAP_ADD_TO_SCOPE(
      self->modules,
      LILY_CHECKED_SYMBOL_KIND_MODULE,
      visibility,
      NEW_VARIANT(LilyCheckedSymbolScope,
                  module,
                  NEW(LilyCheckedSymbolScopeModule, self->id, id)));
}

enum LilyCheckedScopeAddStatus
add_record_object__LilyCheckedScope(LilyCheckedScope *self,
                                    char *name,
                                    Usize id,
                                    enum LilyVisibility visibility)
{
    VALID_PASCAL_CASE();
    HASH_MAP_CHECK_IF_EXISTS(self->enums);
    HASH_MAP_CHECK_IF_EXISTS(self->records);
    HASH_MAP_CHECK_IF_EXISTS(self->aliases);
    HASH_MAP_CHECK_IF_EXISTS(self->errors);
    HASH_MAP_CHECK_IF_EXISTS(self->enums_object);
    HASH_MAP_CHECK_IF_EXISTS(self->modules);
    HASH_MAP_CHECK_IF_EXISTS(self->classes);
    HASH_MAP_CHECK_IF_EXISTS(self->traits);
    HASH_MAP_ADD_TO_SCOPE(
      self->records_object,
      LILY_CHECKED_SYMBOL_KIND_RECORD_OBJECT,
      visibility,
      NEW_VARIANT(LilyCheckedSymbolScope,
                  module,
                  NEW(LilyCheckedSymbolScopeModule, self->id, id)));
}

enum LilyCheckedScopeAddStatus
add_class__LilyCheckedScope(LilyCheckedScope *self,
                            char *name,
                            Usize id,
                            enum LilyVisibility visibility)
{
    VALID_PASCAL_CASE();
    HASH_MAP_CHECK_IF_EXISTS(self->enums);
    HASH_MAP_CHECK_IF_EXISTS(self->records);
    HASH_MAP_CHECK_IF_EXISTS(self->aliases);
    HASH_MAP_CHECK_IF_EXISTS(self->errors);
    HASH_MAP_CHECK_IF_EXISTS(self->enums_object);
    HASH_MAP_CHECK_IF_EXISTS(self->modules);
    HASH_MAP_CHECK_IF_EXISTS(self->records_object);
    HASH_MAP_CHECK_IF_EXISTS(self->traits);
    HASH_MAP_ADD_TO_SCOPE(
      self->classes,
      LILY_CHECKED_SYMBOL_KIND_CLASS,
      visibility,
      NEW_VARIANT(LilyCheckedSymbolScope,
                  module,
                  NEW(LilyCheckedSymbolScopeModule, self->id, id)));
}

enum LilyCheckedScopeAddStatus
add_trait__LilyCheckedScope(LilyCheckedScope *self,
                            char *name,
                            Usize id,
                            enum LilyVisibility visibility)
{
    VALID_PASCAL_CASE();
    HASH_MAP_CHECK_IF_EXISTS(self->enums);
    HASH_MAP_CHECK_IF_EXISTS(self->records);
    HASH_MAP_CHECK_IF_EXISTS(self->aliases);
    HASH_MAP_CHECK_IF_EXISTS(self->errors);
    HASH_MAP_CHECK_IF_EXISTS(self->enums_object);
    HASH_MAP_CHECK_IF_EXISTS(self->modules);
    HASH_MAP_CHECK_IF_EXISTS(self->records_object);
    HASH_MAP_CHECK_IF_EXISTS(self->classes);
    HASH_MAP_ADD_TO_SCOPE(
      self->traits,
      LILY_CHECKED_SYMBOL_KIND_TRAIT,
      visibility,
      NEW_VARIANT(LilyCheckedSymbolScope,
                  module,
                  NEW(LilyCheckedSymbolScopeModule, self->id, id)));
}

enum LilyCheckedScopeAddStatus
add_fun__LilyCheckedScope(LilyCheckedScope *self,
                          char *name,
                          Usize id,
                          enum LilyVisibility visibility)
{
    VALID_SNAKE_CASE();
    HASH_MAP_CHECK_IF_EXISTS(self->constants);
    HASH_MAP_ADD_TO_SCOPE(
      self->funs,
      LILY_CHECKED_SYMBOL_KIND_FUN,
      visibility,
      NEW_VARIANT(LilyCheckedSymbolScope,
                  module,
                  NEW(LilyCheckedSymbolScopeModule, self->id, id)));
}

LilyCheckedSymbol *
search_constant_in_current_scope__LilyCheckedScope(const LilyCheckedScope *self,
                                                   char *name)
{
    SEARCH_IN_CURRENT_SCOPE(self->constants);
}

LilyCheckedSymbol *
search_enum_in_current_scope__LilyCheckedScope(const LilyCheckedScope *self,
                                               char *name)
{
    SEARCH_IN_CURRENT_SCOPE(self->enums);
}

LilyCheckedSymbol *
search_record_in_current_scope__LilyCheckedScope(const LilyCheckedScope *self,
                                                 char *name)
{
    SEARCH_IN_CURRENT_SCOPE(self->records);
}

LilyCheckedSymbol *
search_alias_in_current_scope__LilyCheckedScope(const LilyCheckedScope *self,
                                                char *name)
{
    SEARCH_IN_CURRENT_SCOPE(self->aliases);
}

LilyCheckedSymbol *
search_error_in_current_scope__LilyCheckedScope(const LilyCheckedScope *self,
                                                char *name)
{
    SEARCH_IN_CURRENT_SCOPE(self->errors);
}

LilyCheckedSymbol *
search_enum_object_in_current_scope__LilyCheckedScope(
  const LilyCheckedScope *self,
  char *name)
{
    SEARCH_IN_CURRENT_SCOPE(self->enums_object);
}

LilyCheckedSymbol *
search_module_in_current_scope__LilyCheckedScope(const LilyCheckedScope *self,
                                                 char *name)
{
    SEARCH_IN_CURRENT_SCOPE(self->modules);
}

LilyCheckedSymbol *
search_record_object_in_current_scope__LilyCheckedScope(
  const LilyCheckedScope *self,
  char *name)
{
    SEARCH_IN_CURRENT_SCOPE(self->records_object);
}

LilyCheckedSymbol *
search_class_in_current_scope__LilyCheckedScope(const LilyCheckedScope *self,
                                                char *name)
{
    SEARCH_IN_CURRENT_SCOPE(self->classes);
}

LilyCheckedSymbol *
search_trait_in_current_scope__LilyCheckedScope(const LilyCheckedScope *self,
                                                char *name)
{
    SEARCH_IN_CURRENT_SCOPE(self->traits);
}

LilyCheckedSymbol *
search_fun_in_current_scope__LilyCheckedScope(const LilyCheckedScope *self,
                                              char *name)
{
    SEARCH_IN_CURRENT_SCOPE(self->funs);
}

LilyCheckedSearchStatus
search_constant__LilyCheckedScope(const LilyCheckedDeclModule *module,
                                  char *name,
                                  bool must_pub)
{
    LilyCheckedSymbol *symb =
      search_constant_in_current_scope__LilyCheckedScope(module->scope, name);

    if (symb) {
        if ((must_pub && symb->visibility == LILY_VISIBILITY_PUBLIC) ||
            !must_pub) {
            return NEW_VARIANT(LilyCheckedSearchStatus, success, symb);
        }

        return NEW_VARIANT(LilyCheckedSearchStatus, private_symbol);
    }

    return module->parent
             ? search_constant__LilyCheckedScope(module->parent, name, must_pub)
             : NEW_VARIANT(LilyCheckedSearchStatus, symbol_not_found);
}

LilyCheckedSearchStatus
search_identifier__LilyCheckedScope(const LilyCheckedDeclModule *module,
                                    char *name,
                                    bool must_pub)
{
    LilyCheckedSymbol *constant_symbol =
      search_constant_in_current_scope__LilyCheckedScope(module->scope, name);

    return NEW_VARIANT(LilyCheckedSearchStatus, symbol_not_found);
}

LilyCheckedSearchStatus
search_data_type__LilyCheckedScope(const LilyCheckedDeclModule *module,
                                   char *name,
                                   bool must_pub)
{
    return NEW_VARIANT(LilyCheckedSearchStatus, symbol_not_found);
}

DESTRUCTOR(LilyCheckedScope, LilyCheckedScope *self)
{
    if (self->constants) {
        FREE_HASHMAP_VALUES(self->constants, LilyCheckedSymbol);
        FREE(HashMap, self->constants);
    }

    if (self->enums) {
        FREE_HASHMAP_VALUES(self->enums, LilyCheckedSymbol);
        FREE(HashMap, self->enums);
    }

    if (self->records) {
        FREE_HASHMAP_VALUES(self->records, LilyCheckedSymbol);
        FREE(HashMap, self->records);
    }

    if (self->aliases) {
        FREE_HASHMAP_VALUES(self->aliases, LilyCheckedSymbol);
        FREE(HashMap, self->aliases);
    }

    if (self->errors) {
        FREE_HASHMAP_VALUES(self->errors, LilyCheckedSymbol);
        FREE(HashMap, self->errors);
    }

    if (self->enums_object) {
        FREE_HASHMAP_VALUES(self->enums_object, LilyCheckedSymbol);
        FREE(HashMap, self->enums_object);
    }

    if (self->modules) {
        FREE_HASHMAP_VALUES(self->modules, LilyCheckedSymbol);
        FREE(HashMap, self->modules);
    }

    if (self->records_object) {
        FREE_HASHMAP_VALUES(self->records_object, LilyCheckedSymbol);
        FREE(HashMap, self->records_object);
    }

    if (self->classes) {
        FREE_HASHMAP_VALUES(self->classes, LilyCheckedSymbol);
        FREE(HashMap, self->classes);
    }

    if (self->traits) {
        FREE_HASHMAP_VALUES(self->traits, LilyCheckedSymbol);
        FREE(HashMap, self->traits);
    }

    if (self->funs) {
        FREE_HASHMAP_VALUES(self->funs, LilyCheckedSymbol);
        FREE(HashMap, self->funs);
    }

    if (self->children) {
        FREE(Vec, self->children);
    }

    lily_free(self);
}
