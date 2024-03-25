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

typedef Vec *CIResultDefineVec;

DESTRUCTOR(CIResultDefineVec, CIResultDefineVec self);

DESTRUCTOR(CIResultDefineVec, CIResultDefineVec self)
{
    FREE_BUFFER_ITEMS(self->buffer, self->len, CIResultDefine);
    FREE(Vec, self);
}

CONSTRUCTOR(CIResultDefine *,
            CIResultDefine,
            const CITokenPreprocessorDefine *define)
{
    CIResultDefine *self = lily_malloc(sizeof(CIResultDefine));

    self->define = define;

    return self;
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
            Usize id,
            bool kind,
            enum CIStandard standard,
            String *filename_result,
            File file_input)
{
    CIResultFile *self = lily_malloc(sizeof(CIResultFile));

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

bool
undef_define__CIResultFile(const CIResultFile *self, String *name)
{
    CIResultDefine *is_exist = remove__HashMap(self->defines, name->buffer);
    bool res = is_exist;

    FREE(CIResultDefine, is_exist);

    return res;
}

const CIDecl *
add_enum__CIResultFile(const CIResultFile *self, CIDecl *enum_)
{
    const String *enum_name = get_name__CIDecl(enum_);
    const CIDecl *is_struct_exist =
      search_struct__CIResultFile(self, enum_name);

    if (is_struct_exist) {
        return is_struct_exist;
    }

    const CIDecl *is_union_exist = search_union__CIResultFile(self, enum_name);

    if (is_union_exist) {
        return is_union_exist;
    }

    const CIEnumID *is_enum_exist = add_enum__CIScope(
      self->scope_base, enum_name, NEW(CIFileID, self->id, self->kind));

    if (!is_enum_exist) {
        push__Vec(self->enums, enum_);

        return NULL;
    }

    return enum_;
}

const CIDecl *
add_function__CIResultFile(const CIResultFile *self, CIDecl *function)
{
    const String *function_name = get_name__CIDecl(function);
    const CIDecl *is_variable_exist =
      search_variable__CIResultFile(self, self->scope_base, function_name);

    if (is_variable_exist) {
        return is_variable_exist;
    }

    const CIFunctionID *is_function_exist = add_function__CIScope(
      self->scope_base, function_name, NEW(CIFileID, self->id, self->kind));

    if (!is_function_exist) {
        push__Vec(self->functions, function);

        return NULL;
    }

    return function;
}

const CIDecl *
add_struct__CIResultFile(const CIResultFile *self, CIDecl *struct_)
{
    const String *struct_name = get_name__CIDecl(struct_);
    const CIDecl *is_enum_exist = search_enum__CIResultFile(self, struct_name);

    if (is_enum_exist) {
        return is_enum_exist;
    }

    const CIDecl *is_union_exist =
      search_union__CIResultFile(self, struct_name);

    if (is_union_exist) {
        return is_union_exist;
    }

    const CIStructID *is_struct_exist = add_struct__CIScope(
      self->scope_base, struct_name, NEW(CIFileID, self->id, self->kind));

    if (!is_struct_exist) {
        push__Vec(self->structs, struct_);

        return NULL;
    }

    return struct_;
}

const CIDecl *
add_union__CIResultFile(const CIResultFile *self, CIDecl *union_)
{
    const String *union_name = get_name__CIDecl(union_);
    const CIDecl *is_struct_exist =
      search_struct__CIResultFile(self, union_name);

    if (is_struct_exist) {
        return is_struct_exist;
    }

    const CIDecl *is_enum_exist = search_enum__CIResultFile(self, union_name);

    if (is_enum_exist) {
        return is_enum_exist;
    }

    const CIUnionID *is_union_exist = add_union__CIScope(
      self->scope_base, union_name, NEW(CIFileID, self->id, self->kind));

    if (!is_union_exist) {
        push__Vec(self->unions, union_);

        return NULL;
    }

    return union_;
}

const CIDecl *
add_variable__CIResultFile(const CIResultFile *self,
                           const CIScope *scope,
                           CIDecl *variable)
{
    const String *variable_name = get_name__CIDecl(variable);
    const CIDecl *is_function_exist =
      search_function__CIResultFile(self, variable_name);

    if (is_function_exist) {
        return is_function_exist;
    }

    const CIVariableID *is_variable_exist =
      add_variable__CIScope(scope,
                            variable_name,
                            *scope->scope_id,
                            NEW(CIFileID, self->id, self->kind));

    if (!is_variable_exist) {
        push__Vec(self->variables, variable);

        return NULL;
    }

    return variable;
}

#define GET_DECL_FROM_ID__CI_RESULT_FILE(vec, id) return get__Vec(vec, id);

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

#define SEARCH_TYPEDEF(search)                                       \
    CIDecl *res = search(self, name);                                \
                                                                     \
    if (res && res->storage_class_flag & CI_STORAGE_CLASS_TYPEDEF) { \
        return res;                                                  \
    }                                                                \
                                                                     \
    return NULL;

CIDecl *
search_typedef_enum__CIResultFile(const CIResultFile *self, const String *name)
{
    SEARCH_TYPEDEF(search_enum__CIResultFile);
}

CIDecl *
search_typedef_struct__CIResultFile(const CIResultFile *self,
                                    const String *name)
{
    SEARCH_TYPEDEF(search_struct__CIResultFile);
}

CIDecl *
search_typedef_union__CIResultFile(const CIResultFile *self, const String *name)
{
    SEARCH_TYPEDEF(search_union__CIResultFile);
}

CIDecl *
search_typedef__CIResultFile(const CIResultFile *self, const String *name)
{
    CIDecl *res = search_data_type__CIResultFile(self, name);

    if (res && res->storage_class_flag & CI_STORAGE_CLASS_TYPEDEF) {
        return res;
    }

    return NULL;
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
    FREE_HASHMAP_VALUES(self->defines, CIResultDefineVec);
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
                                    self->headers->len,           \
                                    kind,                         \
                                    standard,                     \
                                    filename_result,              \
                                    file_input);                  \
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

DESTRUCTOR(CIResult, const CIResult *self)
{
    FREE_ORD_HASHMAP_VALUES(self->headers, CIResultFile);
    FREE(OrderedHashMap, self->headers);
    FREE_ORD_HASHMAP_VALUES(self->sources, CIResultFile);
    FREE(OrderedHashMap, self->sources);
}
