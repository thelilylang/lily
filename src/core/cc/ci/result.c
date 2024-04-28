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
#include <base/macros.h>

#include <core/cc/ci/result.h>

#include <stdio.h>
#include <stdlib.h>

CONSTRUCTOR(CIResultDefine *,
            CIResultDefine,
            const CITokenPreprocessorDefine *define)
{
    CIResultDefine *self = lily_malloc(sizeof(CIResultDefine));

    self->define = define;
    self->ref_count = 0;

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

CONSTRUCTOR(CIResultFile *,
            CIResultFile,
            const CIResult *result,
            Usize id,
            bool kind,
            enum CIStandard standard,
            String *filename_result,
            File file_input,
            const CIResultFile *builtin)
{
    CIResultFile *self = lily_malloc(sizeof(CIResultFile));

    self->result = result;
    self->id = id;
    self->kind = kind;
    self->filename_result = filename_result;
    self->file_input = file_input;
    self->defines = NEW(HashMap);
    self->includes = NEW(HashMap);
    self->scope_base = NEW(CIScope, NULL, 0, false);
    self->scopes = init__Vec(1, self->scope_base);
    self->enums = NEW(Vec);
    self->functions = NEW(Vec);
    self->structs = NEW(Vec);
    self->typedefs = NEW(Vec);
    self->unions = NEW(Vec);
    self->variables = NEW(Vec);
    self->count_error = 0;
    self->count_warning = 0;
    self->scanner =
      NEW(CIScanner,
          NEW(Source, NEW(Cursor, self->file_input.content), &self->file_input),
          &self->count_error,
          standard);
    self->parser = NEW(CIParser, self, &self->scanner);

    if (builtin) {
        include_builtin__CIResultFile(self, builtin);
    }

    return self;
}

CIScope *
add_scope__CIResultFile(const CIResultFile *self,
                        CIScopeID *parent,
                        bool is_block)
{
    CIScope *scope =
      NEW(CIScope, parent, get_next_scope_id__CIResultFile(self), is_block);

    push__Vec(self->scopes, scope);

    return scope;
}

const CIResultDefine *
add_define__CIResultFile(const CIResultFile *self,
                         CIResultDefine *result_define)
{
    return insert__HashMap(
      self->defines, result_define->define->name->buffer, result_define);
}

const CIResultDefine *
get_define__CIResultFile(const CIResultFile *self, String *name)
{
    return get__HashMap(self->defines, name->buffer);
}

const CIResultDefine *
get_define_from_str__CIResultFile(const CIResultFile *self, char *name)
{
    return get__HashMap(self->defines, name);
}

bool
undef_define__CIResultFile(const CIResultFile *self, String *name)
{
    CIResultDefine *is_exist = remove__HashMap(self->defines, name->buffer);

    if (is_exist) {
        FREE(CIResultDefine, is_exist);

        return true;
    }

    return false;
}

void
include_builtin__CIResultFile(const CIResultFile *self,
                              const CIResultFile *builtin)
{
    HashMapIter iter = NEW(HashMapIter, builtin->defines);
    CIResultDefine *def = NULL;

    while ((def = next__HashMapIter(&iter))) {
        insert__HashMap(
          self->defines, def->define->name->buffer, ref__CIResultDefine(def));
    }
}

void
add_include__CIResultFile(const CIResultFile *self)
{
}

#define CHECK_FOR_SYMBOL_REDEFINITION_DECL(name, search) \
    {                                                    \
        CIDecl *is_exist = search(self, name);           \
                                                         \
        if (is_exist) {                                  \
            return is_exist;                             \
        }                                                \
    }

#define CHECK_FOR_SYMBOL_REDEFINITION_VAR(name, scope)                       \
    {                                                                        \
        CIDecl *is_exist = search_variable__CIResultFile(self, scope, name); \
                                                                             \
        if (is_exist) {                                                      \
            return is_exist;                                                 \
        }                                                                    \
    }

#define CHECK_FOR_SYMBOL_REDEFINITION(name, scope, decl)                       \
    if (!(decl->kind & CI_DECL_KIND_TYPEDEF)) {                                \
        CHECK_FOR_SYMBOL_REDEFINITION_DECL(name, search_enum__CIResultFile);   \
        CHECK_FOR_SYMBOL_REDEFINITION_DECL(name, search_struct__CIResultFile); \
        CHECK_FOR_SYMBOL_REDEFINITION_DECL(name, search_union__CIResultFile);  \
    }                                                                          \
                                                                               \
    if (!(decl->kind & CI_DECL_KIND_ENUM ||                                    \
          decl->kind & CI_DECL_KIND_STRUCT ||                                  \
          decl->kind & CI_DECL_KIND_UNION)) {                                  \
        CHECK_FOR_SYMBOL_REDEFINITION_DECL(name,                               \
                                           search_typedef__CIResultFile);      \
    }                                                                          \
                                                                               \
    CHECK_FOR_SYMBOL_REDEFINITION_DECL(name, search_function__CIResultFile);   \
    CHECK_FOR_SYMBOL_REDEFINITION_VAR(name, scope);

#define ADD_X_DECL(X, scope, add_scope, v)         \
    const String *name = get_name__CIDecl(X);      \
                                                   \
    CHECK_FOR_SYMBOL_REDEFINITION(name, scope, X); \
                                                   \
    ASSERT(!add_scope);                            \
    push__Vec(v, X);                               \
                                                   \
    return NULL;

const CIDecl *
add_enum__CIResultFile(const CIResultFile *self, CIDecl *enum_)
{
    ADD_X_DECL(enum_,
               self->scope_base,
               add_enum__CIScope(
                 self->scope_base, name, NEW(CIFileID, self->id, self->kind)),
               self->enums);
}

const CIDecl *
add_function__CIResultFile(const CIResultFile *self, CIDecl *function)
{
    ADD_X_DECL(function,
               self->scope_base,
               add_function__CIScope(
                 self->scope_base, name, NEW(CIFileID, self->id, self->kind)),
               self->functions);
}

const CIDecl *
add_struct__CIResultFile(const CIResultFile *self, CIDecl *struct_)
{
    ADD_X_DECL(struct_,
               self->scope_base,
               add_struct__CIScope(
                 self->scope_base, name, NEW(CIFileID, self->id, self->kind)),
               self->structs);
}

const CIDecl *
add_typedef__CIResultFile(const CIResultFile *self, CIDecl *typedef_)
{
    ADD_X_DECL(typedef_,
               self->scope_base,
               add_typedef__CIScope(
                 self->scope_base, name, NEW(CIFileID, self->id, self->kind)),
               self->typedefs);
}

const CIDecl *
add_union__CIResultFile(const CIResultFile *self, CIDecl *union_)
{
    ADD_X_DECL(union_,
               self->scope_base,
               add_union__CIScope(
                 self->scope_base, name, NEW(CIFileID, self->id, self->kind)),
               self->unions);
}

const CIDecl *
add_variable__CIResultFile(const CIResultFile *self,
                           const CIScope *scope,
                           CIDecl *variable)
{
    ADD_X_DECL(
      variable,
      scope,
      add_variable__CIScope(
        scope, name, *scope->scope_id, NEW(CIFileID, self->id, self->kind)),
      self->variables);
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
    GET_DECL_FROM_ID__CI_RESULT_FILE(self->enums, enum_id->id);
}

CIDecl *
get_function_from_id__CIResultFile(const CIResultFile *self,
                                   const CIFunctionID *function_id)
{
    GET_DECL_FROM_ID__CI_RESULT_FILE(self->functions, function_id->id);
}

CIDecl *
get_struct_from_id__CIResultFile(const CIResultFile *self,
                                 const CIStructID *struct_id)
{
    GET_DECL_FROM_ID__CI_RESULT_FILE(self->structs, struct_id->id);
}

CIDecl *
get_typedef_from_id__CIResultFile(const CIResultFile *self,
                                  const CITypedefID *typedef_id)
{
    GET_DECL_FROM_ID__CI_RESULT_FILE(self->typedefs, typedef_id->id);
}

CIDecl *
get_union_from_id__CIResultFile(const CIResultFile *self,
                                const CIUnionID *union_id)
{
    GET_DECL_FROM_ID__CI_RESULT_FILE(self->unions, union_id->id);
}

CIDecl *
get_variable_from_id__CIResultFile(const CIResultFile *self,
                                   const CIVariableID *variable_id)
{
    GET_DECL_FROM_ID__CI_RESULT_FILE(self->variables, variable_id->id);
}

#define SEARCH_DECL__CI_RESULT_FILE(ty, search_f, get_from_id_f) \
    const ty *id = search_f(self->scope_base, name);             \
                                                                 \
    if (!id) {                                                   \
        return NULL;                                             \
    }                                                            \
                                                                 \
    return get_from_id_f(self, id);

CIScope *
get_scope_from_id__CIResultFile(const CIResultFile *self,
                                const CIScopeID *scope_id)
{
    return get__Vec(self->scopes, scope_id->id);
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
    while (scope) {
        const CIVariableID *variable_id = search_variable__CIScope(scope, name);

        if (!variable_id) {
            if (scope->parent) {
                scope = get_scope_from_id__CIResultFile(self, scope->parent);
                continue;
            }

            break;
        }

        return get_variable_from_id__CIResultFile(self, variable_id);
    }

    return NULL;
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
    run__CIParser(&self->parser);
}

DESTRUCTOR(CIResultFile, CIResultFile *self)
{
    FREE(String, self->filename_result);
    lily_free(self->file_input.content);
    lily_free(self->file_input.name);
    FREE_HASHMAP_VALUES(self->defines, CIResultDefine);
    FREE(HashMap, self->defines);
    FREE_HASHMAP_VALUES(self->includes, CIResultInclude);
    FREE(HashMap, self->includes);
    FREE_BUFFER_ITEMS(self->scopes->buffer, self->scopes->len, CIScope);
    FREE(Vec, self->scopes);
    FREE_BUFFER_ITEMS(self->enums->buffer, self->enums->len, CIDecl);
    FREE(Vec, self->enums);
    FREE_BUFFER_ITEMS(self->functions->buffer, self->functions->len, CIDecl);
    FREE(Vec, self->functions);
    FREE_BUFFER_ITEMS(self->structs->buffer, self->structs->len, CIDecl);
    FREE(Vec, self->structs);
    FREE_BUFFER_ITEMS(self->typedefs->buffer, self->typedefs->len, CIDecl);
    FREE(Vec, self->typedefs);
    FREE_BUFFER_ITEMS(self->unions->buffer, self->unions->len, CIDecl);
    FREE(Vec, self->unions);
    FREE_BUFFER_ITEMS(self->variables->buffer, self->variables->len, CIDecl);
    FREE(Vec, self->variables);
    FREE(CIScanner, &self->scanner);
    FREE(CIParser, &self->parser);
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

#define ADD_FILE__CI_RESULT(kind)                                 \
    CIResultFile *result_file = NEW(CIResultFile,                 \
                                    self,                         \
                                    self->headers->len,           \
                                    kind,                         \
                                    standard,                     \
                                    filename_result,              \
                                    file_input,                   \
                                    self->builtin);               \
                                                                  \
    if (insert__OrderedHashMap(                                   \
          self->headers, filename_result->buffer, result_file)) { \
        FAILED("duplicate input file");                           \
    }                                                             \
                                                                  \
    return result_file;

CIResultFile *
add_header__CIResult(const CIResult *self,
                     enum CIStandard standard,
                     String *filename_result,
                     File file_input)
{
    ADD_FILE__CI_RESULT(CI_FILE_ID_KIND_HEADER);
}

CIResultFile *
add_source__CIResult(const CIResult *self,
                     enum CIStandard standard,
                     String *filename_result,
                     File file_input)
{
    ADD_FILE__CI_RESULT(CI_FILE_ID_KIND_SOURCE);
}

void
load_builtin__CIResult(CIResult *self, const CIConfig *config)
{
    String *builtin_content = generate_builtin__CIBuiltin(config);
    File builtin_file = { .name = strdup("**<builtin.hci>**"),
                          .content = builtin_content->buffer,
                          .len = builtin_content->len };
    CIResultFile *builtin = NEW(CIResultFile,
                                self,
                                0,
                                CI_FILE_ID_KIND_HEADER,
                                config->standard,
                                from__String("**<builtin.h>**"),
                                builtin_file,
                                NULL);

    set_builtin__CIScanner(&builtin->scanner);
    run__CIResultFile(builtin);

    self->builtin = builtin;

    lily_free(builtin_content);
}

bool
add_and_run__CIResult(const CIResult *self,
                      char *path,
                      enum CIStandard standard)
{
    char *extension = get_extension__File(path);
    String *filename_result = get_filename__File(path);
    bool is_header = false;

    if (!strcmp(extension, ".ci") || !strcmp(extension, ".c")) {
        push_str__String(filename_result, ".c");
    } else if (!strcmp(extension, ".hci") || !strcmp(extension, ".h")) {
        push_str__String(filename_result, ".h");
        is_header = true;
    } else {
        lily_free(extension);
        FREE(String, filename_result);

        FAILED("unknown extension, expected `.ci`, `.c`, `.hci` or `.h`");

        return false;
    }

    const bool has_header =
      is_header ? has_header__CIResult(self, filename_result) : false;
    const bool has_source =
      !is_header ? has_source__CIResult(self, filename_result) : false;

    if (has_header || has_source) {
        lily_free(extension);
        FREE(String, filename_result);

        return false;
    }

    char *file_content = read_file__File(path);
    File file_input = NEW(File, path, file_content);
    CIResultFile *result_file = NULL;

    if (is_header) {
        result_file =
          add_header__CIResult(self, standard, filename_result, file_input);
    } else {
        result_file =
          add_source__CIResult(self, standard, filename_result, file_input);
    }

    run__CIResultFile(result_file);

    lily_free(extension);

    return true;
}

DESTRUCTOR(CIResult, const CIResult *self)
{
    FREE(CIResultFile, self->builtin);
    FREE_ORD_HASHMAP_VALUES(self->headers, CIResultFile);
    FREE(OrderedHashMap, self->headers);
    FREE_ORD_HASHMAP_VALUES(self->sources, CIResultFile);
    FREE(OrderedHashMap, self->sources);
}
