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
#include <base/assert.h>
#include <base/dir.h>
#include <base/macros.h>

#include <core/cc/ci/result.h>

#include <stdio.h>
#include <stdlib.h>

/// @brief This function update prototype, according the declaration.
static void
update_prototype__CIResult(CIDecl *prototype, CIDecl *decl);

/// @param path char* (&)
static void
add_and_run_lib_file__CIResult(const CIResult *self,
                               CIResultLib *lib,
                               char *path);

CONSTRUCTOR(CIResultDefine *,
            CIResultDefine,
            const CITokenPreprocessorDefine *define,
            CIFileID file_id)
{
    CIResultDefine *self = lily_malloc(sizeof(CIResultDefine));

    self->define = define;
    self->ref_count = 0;
    self->file_id = file_id;

    return self;
}

DESTRUCTOR(CIResultDefine, CIResultDefine *self)
{
    if (self->ref_count > 0) {
        --self->ref_count;
        return;
    }

    lily_free(self);
}

CONSTRUCTOR(CIResultInclude *,
            CIResultInclude,
            enum CIResultIncludeKind kind,
            const CIResultFile *path_ref)
{
    CIResultInclude *self = lily_malloc(sizeof(CIResultInclude));

    self->kind = kind;
    self->path_ref = path_ref;
    self->repeat_count = 0;

    return self;
}

void
reset__CIResultEntity(CIResultEntity *self)
{
#define FREE_ENTITY_VECS()                                                    \
    FREE_BUFFER_ITEMS(self->decls->buffer, self->decls->len, CIDecl);         \
    FREE(Vec, self->decls);                                                   \
    FREE_BUFFER_ITEMS(self->enums->buffer, self->enums->len, CIDecl);         \
    FREE(Vec, self->enums);                                                   \
    FREE_BUFFER_ITEMS(self->functions->buffer, self->functions->len, CIDecl); \
    FREE(Vec, self->functions);                                               \
    FREE_BUFFER_ITEMS(self->labels->buffer, self->labels->len, CIDecl);       \
    FREE(Vec, self->labels);                                                  \
    FREE_BUFFER_ITEMS(self->structs->buffer, self->structs->len, CIDecl);     \
    FREE(Vec, self->structs);                                                 \
    FREE_BUFFER_ITEMS(self->typedefs->buffer, self->typedefs->len, CIDecl);   \
    FREE(Vec, self->typedefs);                                                \
    FREE_BUFFER_ITEMS(self->unions->buffer, self->unions->len, CIDecl);       \
    FREE(Vec, self->unions);                                                  \
    FREE_BUFFER_ITEMS(self->variables->buffer, self->variables->len, CIDecl); \
    FREE(Vec, self->variables);

    FREE_ENTITY_VECS();

    self->decls = NEW(Vec);
    self->enums = NEW(Vec);
    self->functions = NEW(Vec);
    self->labels = NEW(Vec);
    self->structs = NEW(Vec);
    self->typedefs = NEW(Vec);
    self->unions = NEW(Vec);
    self->variables = NEW(Vec);
}

DESTRUCTOR(CIResultEntity, const CIResultEntity *self)
{
    FREE_ENTITY_VECS();

    if (self->filename_result) {
        FREE(String, self->filename_result);
    }

#undef FREE_ENTITY_VECS
}

CONSTRUCTOR(CIResultFileAnalysis *, CIResultFileAnalysis, CIResultFile *file)
{
    CIResultFileAnalysis *self = lily_malloc(sizeof(CIResultFileAnalysis));

    self->entity = &file->entity;
    self->scope_base = NEW(CIScope, NULL, 0, false);
    self->scopes = init__Vec(1, self->scope_base);
    self->defines = NEW(HashMap);
    self->includes = NEW(HashMap);
    self->count_error = 0;
    self->count_warning = 0;

    set_file_analysis__CIResultFile(file, self);

    // If predefined is NULL, we are analyzing predefined.
    if (file->entity.result->predefined) {
        include_predefined__CIResultFile(file, file->entity.result->predefined);
    }

    self->parser = NEW(CIParser, file, &file->scanner);

    return self;
}

DESTRUCTOR(CIResultFileAnalysis, CIResultFileAnalysis *self)
{
    FREE_BUFFER_ITEMS(self->scopes->buffer, self->scopes->len, CIScope);
    FREE(Vec, self->scopes);
    FREE_HASHMAP_VALUES(self->defines, CIResultDefine);
    FREE(HashMap, self->defines);
    FREE_HASHMAP_VALUES(self->includes, CIResultInclude);
    FREE(HashMap, self->includes);
    FREE(CIParser, &self->parser);
    lily_free(self);
}

CONSTRUCTOR(CIResultFile *,
            CIResultFile,
            File file_input,
            bool kind,
            CIResultFile *owner,
            const CIProjectConfig *config,
            Usize id,
            const CIResult *result,
            enum CIResultEntityKind entity_kind,
            String *filename_result)
{
    CIResultFile *self = lily_malloc(sizeof(CIResultFile));

    self->file_input = file_input;
    self->kind = kind;
    self->file_analysis = NULL;
    self->scope_base = NULL;
    self->owner = owner;
    self->entity =
      NEW(CIResultEntity, id, result, entity_kind, filename_result);
    self->scanner =
      NEW(CIScanner,
          NEW(Source, NEW(Cursor, self->file_input.content), &self->file_input),
          NULL,
          config);

    return self;
}

void
set_file_analysis__CIResultFile(CIResultFile *self,
                                CIResultFileAnalysis *file_analysis)
{
    ASSERT(!self->file_analysis);

    self->file_analysis = file_analysis;
    self->scope_base = self->file_analysis->scope_base;
    self->scanner.base.count_error = &self->file_analysis->count_error;
}

void
destroy_file_analysis__CIResultFile(CIResultFile *self)
{
    ASSERT(self->file_analysis);

    FREE(CIResultFileAnalysis, self->file_analysis);

    self->file_analysis = NULL;
    self->scope_base = NULL;
    self->scanner.base.count_error = NULL;

    reset__CIResultEntity(&self->entity);
}

CIScope *
add_scope__CIResultFile(const CIResultFile *self,
                        CIScopeID *parent,
                        bool is_block)
{
    CIScope *scope =
      NEW(CIScope, parent, get_next_scope_id__CIResultFile(self), is_block);

    push__Vec(self->file_analysis->scopes, scope);

    return scope;
}

const CIResultDefine *
add_define__CIResultFile(const CIResultFile *self,
                         CIResultDefine *result_define)
{
    return insert__HashMap(self->file_analysis->defines,
                           result_define->define->name->buffer,
                           result_define);
}

const CIResultDefine *
get_define__CIResultFile(const CIResultFile *self, String *name)
{
    return get__HashMap(self->file_analysis->defines, name->buffer);
}

const CIResultDefine *
get_define_from_str__CIResultFile(const CIResultFile *self, char *name)
{
    return get__HashMap(self->file_analysis->defines, name);
}

bool
undef_define__CIResultFile(const CIResultFile *self, String *name)
{
    CIResultDefine *is_exist =
      remove__HashMap(self->file_analysis->defines, name->buffer);

    if (is_exist) {
        FREE(CIResultDefine, is_exist);

        return true;
    }

    return false;
}

void
include_predefined__CIResultFile(const CIResultFile *self,
                                 const CIResultFile *predefined)
{
    HashMapIter iter = NEW(HashMapIter, predefined->file_analysis->defines);
    CIResultDefine *def = NULL;

    while ((def = next__HashMapIter(&iter))) {
        insert__HashMap(self->file_analysis->defines,
                        def->define->name->buffer,
                        ref__CIResultDefine(def));
    }
}

void
add_include__CIResultFile(const CIResultFile *self)
{
}

void
update_prototype__CIResult(CIDecl *prototype, CIDecl *decl)
{
    ASSERT(prototype->kind == decl->kind);
    ASSERT(prototype->is_prototype && !decl->is_prototype);

    switch (prototype->kind) {
        case CI_DECL_KIND_ENUM:
            prototype->enum_.variants = decl->enum_.variants;
            decl->enum_.variants = NULL;

            break;
        case CI_DECL_KIND_FUNCTION:
            prototype->function.body = decl->function.body;

            break;
        case CI_DECL_KIND_STRUCT:
            prototype->struct_.fields = decl->struct_.fields;
            decl->struct_.fields = NULL;

            break;
        case CI_DECL_KIND_TYPEDEF:
            break;
        case CI_DECL_KIND_UNION:
            prototype->union_.fields = decl->union_.fields;
            decl->union_.fields = NULL;

            break;
        case CI_DECL_KIND_VARIABLE:
            UNREACHABLE("prototype variable is impossible");
        default:
            UNREACHABLE("the gens variants are not expected");
    }

    prototype->is_prototype = false;
}

#define CHECK_FOR_SYMBOL_REDEFINITION_DECL(name, search, decl)                \
    {                                                                         \
        CIDecl *is_exist = search(self, name);                                \
                                                                              \
        if (is_exist) {                                                       \
            /* Manage prototype update */                                     \
            if (!decl->is_prototype) {                                        \
                if (is_exist->is_prototype && is_exist->kind == decl->kind) { \
                    update_prototype__CIResult(is_exist, decl);               \
                                                                              \
                    return decl;                                              \
                }                                                             \
                                                                              \
                return is_exist;                                              \
            } else {                                                          \
                /* No need to update anything in this case. */                \
                return decl;                                                  \
            }                                                                 \
        }                                                                     \
    }

#define CHECK_FOR_SYMBOL_REDEFINITION_DECL_WITH_SCOPE( \
  name, search, scope, decl)                           \
    {                                                  \
        CIDecl *is_exist = search(self, scope, name);  \
                                                       \
        if (is_exist) {                                \
            return is_exist;                           \
        }                                              \
    }

#define CHECK_FOR_SYMBOL_REDEFINITION(name, scope, decl)     \
    if (decl->kind & CI_DECL_KIND_LABEL) {                   \
        CHECK_FOR_SYMBOL_REDEFINITION_DECL_WITH_SCOPE(       \
          name, search_label__CIResultFile, scope, decl);    \
    } else if (decl->kind & CI_DECL_KIND_FUNCTION ||         \
               decl->kind & CI_DECL_KIND_VARIABLE) {         \
        CHECK_FOR_SYMBOL_REDEFINITION_DECL(                  \
          name, search_function__CIResultFile, decl);        \
        CHECK_FOR_SYMBOL_REDEFINITION_DECL_WITH_SCOPE(       \
          name, search_variable__CIResultFile, scope, decl); \
    } else {                                                 \
        if (!(decl->kind & CI_DECL_KIND_TYPEDEF)) {          \
            CHECK_FOR_SYMBOL_REDEFINITION_DECL(              \
              name, search_enum__CIResultFile, decl);        \
            CHECK_FOR_SYMBOL_REDEFINITION_DECL(              \
              name, search_struct__CIResultFile, decl);      \
            CHECK_FOR_SYMBOL_REDEFINITION_DECL(              \
              name, search_union__CIResultFile, decl);       \
        }                                                    \
                                                             \
        if (!(decl->kind & CI_DECL_KIND_ENUM ||              \
              decl->kind & CI_DECL_KIND_STRUCT ||            \
              decl->kind & CI_DECL_KIND_UNION)) {            \
            CHECK_FOR_SYMBOL_REDEFINITION_DECL(              \
              name, search_typedef__CIResultFile, decl);     \
        }                                                    \
    }

#define ADD_X_DECL(X, scope, add_scope, v, add_to_owner)                      \
    const String *name = get_name__CIDecl(X);                                 \
                                                                              \
    if (!name) {                                                              \
        return NULL;                                                          \
    }                                                                         \
                                                                              \
    CHECK_FOR_SYMBOL_REDEFINITION(name, scope, X);                            \
                                                                              \
    if (add_scope) {                                                          \
        return X;                                                             \
    }                                                                         \
                                                                              \
    push__Vec(v, X);                                                          \
                                                                              \
    if (!is_local__CIDecl(X)) {                                               \
        push__Vec(self->file_analysis->entity->decls, ref__CIDecl(X));        \
    }                                                                         \
                                                                              \
    return self->owner &&                                                     \
               (X->kind != CI_DECL_KIND_VARIABLE || !X->variable.is_local) && \
               X->kind != CI_DECL_KIND_LABEL                                  \
             ? add_to_owner                                                   \
             : NULL;

const CIDecl *
add_enum__CIResultFile(const CIResultFile *self, CIDecl *enum_)
{
    ADD_X_DECL(enum_,
               self->file_analysis->scope_base,
               add_enum__CIScope(
                 self->file_analysis->scope_base,
                 name,
                 NEW(CIFileID, self->file_analysis->entity->id, self->kind)),
               self->file_analysis->entity->enums,
               add_enum__CIResultFile(self->owner, ref__CIDecl(enum_)));
}

const CIDecl *
add_function__CIResultFile(const CIResultFile *self, CIDecl *function)
{
    ADD_X_DECL(function,
               self->file_analysis->scope_base,
               add_function__CIScope(
                 self->file_analysis->scope_base,
                 name,
                 NEW(CIFileID, self->file_analysis->entity->id, self->kind)),
               self->file_analysis->entity->functions,
               add_function__CIResultFile(self->owner, ref__CIDecl(function)));
}

const CIDecl *
add_label__CIResultFile(const CIResultFile *self,
                        const CIScope *scope,
                        CIDecl *label)
{
    ADD_X_DECL(label,
               scope,
               add_label__CIScope(
                 scope,
                 name,
                 *scope->scope_id,
                 NEW(CIFileID, self->file_analysis->entity->id, self->kind)),
               self->file_analysis->entity->labels,
               add_label__CIResultFile(
                 self->owner, self->owner->scope_base, ref__CIDecl(label)));
}

const CIDecl *
add_struct__CIResultFile(const CIResultFile *self, CIDecl *struct_)
{
    ADD_X_DECL(struct_,
               self->file_analysis->scope_base,
               add_struct__CIScope(
                 self->file_analysis->scope_base,
                 name,
                 NEW(CIFileID, self->file_analysis->entity->id, self->kind)),
               self->file_analysis->entity->structs,
               add_struct__CIResultFile(self->owner, ref__CIDecl(struct_)));
}

const CIDecl *
add_typedef__CIResultFile(const CIResultFile *self, CIDecl *typedef_)
{
    ADD_X_DECL(typedef_,
               self->file_analysis->scope_base,
               add_typedef__CIScope(
                 self->file_analysis->scope_base,
                 name,
                 NEW(CIFileID, self->file_analysis->entity->id, self->kind)),
               self->file_analysis->entity->typedefs,
               add_typedef__CIResultFile(self->owner, ref__CIDecl(typedef_)));
}

const CIDecl *
add_union__CIResultFile(const CIResultFile *self, CIDecl *union_)
{
    ADD_X_DECL(union_,
               self->file_analysis->scope_base,
               add_union__CIScope(
                 self->file_analysis->scope_base,
                 name,
                 NEW(CIFileID, self->file_analysis->entity->id, self->kind)),
               self->file_analysis->entity->unions,
               add_union__CIResultFile(self->owner, ref__CIDecl(union_)));
}

const CIDecl *
add_variable__CIResultFile(const CIResultFile *self,
                           const CIScope *scope,
                           CIDecl *variable)
{
    ADD_X_DECL(variable,
               scope,
               add_variable__CIScope(
                 scope,
                 name,
                 *scope->scope_id,
                 NEW(CIFileID, self->file_analysis->entity->id, self->kind)),
               self->file_analysis->entity->variables,
               add_variable__CIResultFile(
                 self->owner, self->owner->scope_base, ref__CIDecl(variable)));
}

#define GET_DECL_FROM_ID__CI_RESULT_FILE(vec, id) return get__Vec(vec, id);

const CIResultFile *
get_from_file_id__CIResult(const CIResult *self, const CIFileID *file_id)
{
    switch (file_id->kind) {
        case CI_FILE_ID_KIND_HEADER:
            return get_from_id__OrderedHashMap(self->headers, file_id->id);
        case CI_FILE_ID_KIND_SOURCE:
            return get_from_id__OrderedHashMap(self->sources, file_id->id);
        default:
            UNREACHABLE("expected header or source file");
    }
}

CIDecl *
get_enum_from_id__CIResultFile(const CIResultFile *self,
                               const CIEnumID *enum_id)
{
    GET_DECL_FROM_ID__CI_RESULT_FILE(self->file_analysis->entity->enums,
                                     enum_id->id);
}

CIDecl *
get_function_from_id__CIResultFile(const CIResultFile *self,
                                   const CIFunctionID *function_id)
{
    GET_DECL_FROM_ID__CI_RESULT_FILE(self->file_analysis->entity->functions,
                                     function_id->id);
}

CIDecl *
get_label_from_id__CIResultFile(const CIResultFile *self,
                                const CILabelID *label_id)
{
    GET_DECL_FROM_ID__CI_RESULT_FILE(self->file_analysis->entity->labels,
                                     label_id->id);
}

CIDecl *
get_struct_from_id__CIResultFile(const CIResultFile *self,
                                 const CIStructID *struct_id)
{
    GET_DECL_FROM_ID__CI_RESULT_FILE(self->file_analysis->entity->structs,
                                     struct_id->id);
}

CIDecl *
get_typedef_from_id__CIResultFile(const CIResultFile *self,
                                  const CITypedefID *typedef_id)
{
    GET_DECL_FROM_ID__CI_RESULT_FILE(self->file_analysis->entity->typedefs,
                                     typedef_id->id);
}

CIDecl *
get_union_from_id__CIResultFile(const CIResultFile *self,
                                const CIUnionID *union_id)
{
    GET_DECL_FROM_ID__CI_RESULT_FILE(self->file_analysis->entity->unions,
                                     union_id->id);
}

CIDecl *
get_variable_from_id__CIResultFile(const CIResultFile *self,
                                   const CIVariableID *variable_id)
{
    GET_DECL_FROM_ID__CI_RESULT_FILE(self->file_analysis->entity->variables,
                                     variable_id->id);
}

#define SEARCH_DECL__CI_RESULT_FILE(ty, search_f, get_from_id_f)    \
    const ty *id = search_f(self->file_analysis->scope_base, name); \
                                                                    \
    if (!id) {                                                      \
        return NULL;                                                \
    }                                                               \
                                                                    \
    return get_from_id_f(self, id);

CIScope *
get_scope_from_id__CIResultFile(const CIResultFile *self,
                                const CIScopeID *scope_id)
{
    ASSERT(scope_id);

    return get__Vec(self->file_analysis->scopes, scope_id->id);
}

CIDecl *
search_enum__CIResultFile(const CIResultFile *self, const String *name)
{
    SEARCH_DECL__CI_RESULT_FILE(
      CIEnumID, search_enum__CIScope, get_enum_from_id__CIResultFile);
}

CIDecl *
search_function__CIResultFile(const CIResultFile *self, const String *name)
{
    SEARCH_DECL__CI_RESULT_FILE(CIFunctionID,
                                search_function__CIScope,
                                get_function_from_id__CIResultFile);
}

#define SEARCH_DECL_WITH_SCOPE__CIRESULT_FILE(                                \
  ty, search_f, scope, get_from_id_f)                                         \
    while (scope) {                                                           \
        const ty *id = search_f(scope, name);                                 \
                                                                              \
        if (!id) {                                                            \
            if (scope->parent) {                                              \
                scope = get_scope_from_id__CIResultFile(self, scope->parent); \
                continue;                                                     \
            }                                                                 \
                                                                              \
            break;                                                            \
        }                                                                     \
                                                                              \
        return get_from_id_f(self, id);                                       \
    }                                                                         \
                                                                              \
    return NULL;

CIDecl *
search_label__CIResultFile(const CIResultFile *self,
                           const CIScope *scope,
                           const String *name)
{
    SEARCH_DECL_WITH_SCOPE__CIRESULT_FILE(
      CILabelID, search_label__CIScope, scope, get_label_from_id__CIResultFile);
}

CIDecl *
search_struct__CIResultFile(const CIResultFile *self, const String *name)
{
    SEARCH_DECL__CI_RESULT_FILE(
      CIStructID, search_struct__CIScope, get_struct_from_id__CIResultFile);
}

CIDecl *
search_union__CIResultFile(const CIResultFile *self, const String *name)
{
    SEARCH_DECL__CI_RESULT_FILE(
      CIUnionID, search_union__CIScope, get_union_from_id__CIResultFile);
}

CIDecl *
search_variable__CIResultFile(const CIResultFile *self,
                              const CIScope *scope,
                              const String *name)
{
    SEARCH_DECL_WITH_SCOPE__CIRESULT_FILE(CIVariableID,
                                          search_variable__CIScope,
                                          scope,
                                          get_variable_from_id__CIResultFile);
}

CIDecl *
search_typedef__CIResultFile(const CIResultFile *self, const String *name)
{
    SEARCH_DECL__CI_RESULT_FILE(
      CITypedefID, search_typedef__CIScope, get_typedef_from_id__CIResultFile);
}

CIDecl *
search_data_type__CIResultFile(const CIResultFile *self, const String *name)
{
    {
        CIDecl *enum_ = search_enum__CIResultFile(self, name);

        if (enum_) {
            return enum_;
        }
    }

    {
        CIDecl *struct_ = search_struct__CIResultFile(self, name);

        if (struct_) {
            return struct_;
        }
    }

    {
        CIDecl *union_ = search_union__CIResultFile(self, name);

        if (union_) {
            return union_;
        }
    }

    {
        CIDecl *typedef_ = search_typedef__CIResultFile(self, name);

        if (typedef_) {
            return typedef_;
        }
    }

    return NULL;
}

void
run__CIResultFile(CIResultFile *self)
{
    run__CIScanner(&self->scanner, false);
    run__CIParser(&self->file_analysis->parser);
}

#define ADD_Xs_DECL(add_f, t, v, scope_base_hm, cond, ...)    \
    HashMapIter enums_iter = NEW(HashMapIter, scope_base_hm); \
    t *current = NULL;                                        \
                                                              \
    while ((current = next__HashMapIter(&enums_iter))) {      \
        if (current->file_id.id == self->entity.id            \
              ? current->file_id.kind != self->kind           \
              : true) {                                       \
            CIDecl *decl = get__Vec(v, current->id);          \
                                                              \
            if (cond) {                                       \
                add_f(__VA_ARGS__);                           \
            }                                                 \
        }                                                     \
    }

void
add_enums__CIResultFile(const CIResultFile *self, const CIResultFile *other)
{
    ADD_Xs_DECL(add_enum__CIResultFile,
                CIEnumID,
                other->entity.enums,
                other->scope_base->enums,
                true,
                self,
                ref__CIDecl(decl));
}

void
add_functions__CIResultFile(const CIResultFile *self, const CIResultFile *other)
{
    ADD_Xs_DECL(add_function__CIResultFile,
                CIFunctionID,
                other->entity.functions,
                other->scope_base->functions,
                true,
                self,
                ref__CIDecl(decl));
}

void
add_labels__CIResultFile(const CIResultFile *self, const CIResultFile *other)
{
    /* Empty, because labels are always local. */
}

void
add_structs__CIResultFile(const CIResultFile *self, const CIResultFile *other)
{
    ADD_Xs_DECL(add_struct__CIResultFile,
                CIStructID,
                other->entity.structs,
                other->scope_base->structs,
                true,
                self,
                ref__CIDecl(decl));
}

void
add_typedefs__CIResultFile(const CIResultFile *self, const CIResultFile *other)
{
    ADD_Xs_DECL(add_typedef__CIResultFile,
                CITypedefID,
                other->entity.typedefs,
                other->scope_base->typedefs,
                true,
                self,
                ref__CIDecl(decl));
}

void
add_unions__CIResultFile(const CIResultFile *self, const CIResultFile *other)
{
    ADD_Xs_DECL(add_union__CIResultFile,
                CIUnionID,
                other->entity.unions,
                other->scope_base->unions,
                true,
                self,
                ref__CIDecl(decl));
}

void
add_variables__CIResultFile(const CIResultFile *self, const CIResultFile *other)
{
    // Only include global variable.
    ADD_Xs_DECL(add_variable__CIResultFile,
                CIVariableID,
                other->entity.variables,
                other->scope_base->variables,
                !decl->variable.is_local,
                self,
                self->scope_base,
                ref__CIDecl(decl));
}

void
merge_content__CIResultFile(const CIResultFile *self, CIResultFile *other)
{
    add_enums__CIResultFile(self, other);
    add_functions__CIResultFile(self, other);
    add_structs__CIResultFile(self, other);
    add_unions__CIResultFile(self, other);
    add_variables__CIResultFile(self, other);
    add_labels__CIResultFile(self, other);
}

void
include_content__CIResultFile(const CIResultFile *self, CIResultFile *other)
{
#define HEADER_FILE_BUILTIN_ID 0
    HashMapIter defines_iter = NEW(HashMapIter, other->file_analysis->defines);
    CIResultDefine *current_define = NULL;

    while ((current_define = next__HashMapIter(&defines_iter))) {
        if ((current_define->file_id.id == HEADER_FILE_BUILTIN_ID
               ? current_define->file_id.kind != CI_FILE_ID_KIND_HEADER
               : true) /* Don't include predefined file content */
            && (current_define->file_id.id == self->entity.id
                  ? current_define->file_id.kind != self->kind
                  : true) &&
            add_define__CIResultFile(self,
                                     ref__CIResultDefine(current_define))) {
            FAILED("duplicate #define name");
        }
    }

    merge_content__CIResultFile(self, other);
}

DESTRUCTOR(CIResultFile, CIResultFile *self)
{
    if (self->file_input.content) {
        lily_free(self->file_input.content);
    }

    if (self->file_input.name) {
        lily_free(self->file_input.name);
    }

    FREE(CIScanner, &self->scanner);
    FREE(CIResultEntity, &self->entity);

    if (self->file_analysis) {
        FREE(CIResultFileAnalysis, self->file_analysis);
    }

    lily_free(self);
}

CONSTRUCTOR(CIResultLib *,
            CIResultLib,
            const char *name,
            Usize id,
            const CIResult *result,
            const CIProjectConfig *config)
{
    CIResultLib *self = lily_malloc(sizeof(CIResultLib));

    self->name = name;
    self->sources = NEW(OrderedHashMap);
    self->file =
      NEW(CIResultFile,
          (File){ .name = NULL, .content = NULL, .len = 0 } /* No input file */,
          CI_FILE_ID_KIND_SOURCE,
          NULL,
          config,
          id,
          result,
          CI_RESULT_ENTITY_KIND_LIB,
          format__String("lib{s}.c", self->name));

    return self;
}

void
build__CIResultLib(const CIResultLib *self)
{
    OrderedHashMapIter iter = NEW(OrderedHashMapIter, self->sources);
    CIResultFile *current_lib_file = NULL;

    // Build library file
    while ((current_lib_file = next__OrderedHashMapIter(&iter))) {
        merge_content__CIResultFile(self->file, current_lib_file);
    }
}

DESTRUCTOR(CIResultLib, CIResultLib *self)
{
    FREE_ORD_HASHMAP_VALUES(self->sources, CIResultFile);
    FREE(OrderedHashMap, self->sources);
    FREE(CIResultFile, self->file);
    lily_free(self);
}

CONSTRUCTOR(CIResultBin *, CIResultBin, const char *name)
{
    CIResultBin *self = lily_malloc(sizeof(CIResultBin));

    self->name = name;
    self->file = NULL;

    return self;
}

void
set_file__CIResultBin(CIResultBin *self, CIResultFile *file)
{
    ASSERT(!self->file);

    self->file = file;
}

DESTRUCTOR(CIResultBin, CIResultBin *self)
{
    if (self->file) {
        FREE(CIResultFile, self->file);
    }

    lily_free(self);
}

const CIResultFile *
search_from_file_id__CIResult(const CIResult *self, const CIFileID *file_id)
{
    switch (file_id->kind) {
        case CI_FILE_ID_KIND_HEADER:
            return get_from_id__OrderedHashMap(self->headers, file_id->id);
        case CI_FILE_ID_KIND_SOURCE:
            return get_from_id__OrderedHashMap(self->sources, file_id->id);
        default:
            UNREACHABLE("unknown variant");
    }
}

const CIDecl *
search_enum_from_id__CIResult(const CIResult *self, const CIEnumID *enum_id)
{
    const CIResultFile *file =
      search_from_file_id__CIResult(self, &enum_id->file_id);

    ASSERT(file);

    return get_enum_from_id__CIResultFile(file, enum_id);
}

#define ADD_FILE__CI_RESULT(kind, hm, s)                                       \
    CIResultFile *result_file = NEW(CIResultFile, file_input, kind, standard); \
                                                                               \
    if (insert__OrderedHashMap(hm, filename_result->buffer, result_file)) {    \
        FAILED("duplicate input "s);                                           \
    }                                                                          \
                                                                               \
    return result_file;

CIResultFile *
add_header__CIResult(const CIResult *self,
                     String *filename_result,
                     File file_input)
{
    CIResultFile *result_file = NEW(CIResultFile,
                                    file_input,
                                    CI_FILE_ID_KIND_HEADER,
                                    NULL,
                                    self->config,
                                    self->headers->len,
                                    self,
                                    CI_RESULT_ENTITY_KIND_FILE,
                                    filename_result);

    if (insert__OrderedHashMap(
          self->headers, filename_result->buffer, result_file)) {
        FAILED("duplicate input header");
    }

    return result_file;
}

CIResultBin *
add_bin__CIResult(const CIResult *self, char *name)
{
    CIResultBin *bin = NEW(CIResultBin, name);

    if (insert__OrderedHashMap(self->bins, name, bin)) {
        FAILED("duplicate input binary");
    }

    return bin;
}

CIResultLib *
add_lib__CIResult(const CIResult *self, char *name)
{
    CIResultLib *lib =
      NEW(CIResultLib, name, self->libs->len, self, self->config);

    if (insert__OrderedHashMap(self->libs, name, lib)) {
        FAILED("duplicate input library");
    }

    return lib;
}

void
load_predefined__CIResult(CIResult *self)
{
    String *predefined_content = generate__CIPreDefined(self->config);
    File predefined_file = { .name = strdup("**<predefined.hci>**"),
                             .content = predefined_content->buffer,
                             .len = predefined_content->len };
    CIResultFile *predefined = NEW(CIResultFile,
                                   predefined_file,
                                   CI_FILE_ID_KIND_HEADER,
                                   NULL,
                                   self->config,
                                   HEADER_FILE_BUILTIN_ID,
                                   self,
                                   CI_RESULT_ENTITY_KIND_FILE,
                                   NULL);
    (void)NEW(CIResultFileAnalysis, predefined);

    insert__OrderedHashMap(self->headers, predefined_file.name, predefined);
    set_predefined__CIScanner(&predefined->scanner);
    set__CIPreDefined(predefined);
    run__CIResultFile(predefined);

    self->predefined = predefined;

    lily_free(predefined_content);
}

CIResultFile *
scan_file__CIResult(const CIResult *self,
                    CIResultFile *owner,
                    CIResultFile *file_parent,
                    char *path,
                    Usize id)
{
    char *extension = get_extension__File(path);
    String *filename_result = get_filename__File(path);
    bool kind;

    if (!strcmp(extension, ".ci") || !strcmp(extension, ".c")) {
        push_str__String(filename_result, ".c");
        kind = CI_FILE_ID_KIND_SOURCE;
    } else if (!strcmp(extension, ".hci") || !strcmp(extension, ".h")) {
        push_str__String(filename_result, ".h");
        kind = CI_FILE_ID_KIND_HEADER;
    } else {
        lily_free(extension);
        FREE(String, filename_result);

        FAILED("unknown extension, expected `.ci`, `.c`, `.hci` or `.h`");

        return NULL;
    }

    char *file_content = read_file__File(path);
    File file_input = NEW(File, strdup(path), file_content);
    CIResultFile *result_file = NEW(CIResultFile,
                                    file_input,
                                    kind,
                                    owner,
                                    self->config,
                                    id,
                                    self,
                                    CI_RESULT_ENTITY_KIND_FILE,
                                    filename_result);
    (void)NEW(CIResultFileAnalysis, result_file);

    if (kind == CI_FILE_ID_KIND_SOURCE) {
        insert__OrderedHashMap(
          self->sources, result_file->file_input.name, result_file);
    } else {
        insert__OrderedHashMap(
          self->headers, result_file->file_input.name, result_file);
    }

    run__CIScanner(&result_file->scanner, false);

    lily_free(extension);

    return result_file;
}

CIResultFile *
run_file__CIResult(const CIResult *self,
                   CIResultFile *owner,
                   CIResultFile *file_parent,
                   char *path,
                   Usize id)
{
    CIResultFile *result_file =
      scan_file__CIResult(self, owner, file_parent, path, id);

    run__CIParser(&result_file->file_analysis->parser);

    // if (file_parent) {
    //     include_content__CIResultFile(result_file, file_parent);
    // }

    return result_file;
}

void
add_and_run_lib_file__CIResult(const CIResult *self,
                               CIResultLib *lib,
                               char *path)
{
    CIResultFile *lib_file =
      run_file__CIResult(self, lib->file, NULL, path, self->sources->len);

    ASSERT(lib_file->entity.filename_result);

    insert__OrderedHashMap(lib->sources, path, lib_file);
}

void
add_and_run_lib__CIResult(const CIResult *self,
                          const CIProjectConfigLibrary *lib)
{
    CIResultLib *result_lib = add_lib__CIResult(self, (char *)lib->name);

    // Run (Scan & Parse) all source files of the library.
    for (Usize i = 0; i < lib->paths->len; ++i) {
        char *path = CAST(String *, get__Vec(lib->paths, i))->buffer;

        if (is__Dir(path)) {
            Vec *dir_paths = get_files_rec__Dir(path);

            for (Usize j = 0; j < dir_paths->len; ++j) {
                add_and_run_lib_file__CIResult(
                  self, result_lib, get__Vec(dir_paths, j));
            }

            FREE_BUFFER_ITEMS(dir_paths->buffer, dir_paths->len, String);
            FREE(Vec, dir_paths);
        } else {
            add_and_run_lib_file__CIResult(self, result_lib, path);
        }
    }

    build__CIResultLib(result_lib);
}

void
add_and_run_bin__CIResult(const CIResult *self, const CIProjectConfigBin *bin)
{
    CIResultBin *result_bin = add_bin__CIResult(self, (char *)bin->name);
    char *path = bin->path->buffer;
    CIResultFile *bin_file =
      run_file__CIResult(self, NULL, NULL, path, self->sources->len);

    ASSERT(bin_file->entity.filename_result);

    set_file__CIResultBin(result_bin, bin_file);
}

CIResultFile *
add_and_run_header__CIResult(const CIResult *self,
                             CIResultFile *file_parent,
                             char *path,
                             const CIProjectConfig *config)
{
    ASSERT(file_parent);

    CIResultFile *header = get__OrderedHashMap(self->headers, path);

    if (!header) {
        header = scan_file__CIResult(
          self, NULL, file_parent, path, self->headers->len);
    }

    return header;
}

void
build__CIResult(CIResult *self)
{
    load_predefined__CIResult(self);

    for (Usize i = 0; i < self->config->libraries->len; ++i) {
        add_and_run_lib__CIResult(self, get__Vec(self->config->libraries, i));
    }

    for (Usize i = 0; i < self->config->bins->len; ++i) {
        add_and_run_bin__CIResult(self, get__Vec(self->config->bins, i));
    }
}

DESTRUCTOR(CIResult, const CIResult *self)
{
    FREE_ORD_HASHMAP_VALUES(self->headers, CIResultFile);
    FREE(OrderedHashMap, self->headers);
    FREE(OrderedHashMap, self->sources);
    FREE_ORD_HASHMAP_VALUES(self->bins, CIResultBin);
    FREE(OrderedHashMap, self->bins);
    FREE_ORD_HASHMAP_VALUES(self->libs, CIResultLib);
    FREE(OrderedHashMap, self->libs);
}
