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

#include <core/cc/ci/result.h>

typedef Vec *CIResultDefineVec;

DESTRUCTOR(CIResultDefineVec, CIResultDefineVec self);

DESTRUCTOR(CIResultDefineVec, CIResultDefineVec self)
{
    FREE_BUFFER_ITEMS(self->buffer, self->len, CIResultDefine);
    FREE(Vec, self);
}

CONSTRUCTOR(CIResultDefine *,
            CIResultDefine,
            const CITokenPreprocessorDefine *define,
            const Location *undef_location)
{
    CIResultDefine *self = lily_malloc(sizeof(CIResultDefine));

    self->define = define;
    self->undef_location = undef_location;

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
            enum CIStandard standard,
            String *filename_result,
            File file_input)
{
    CIResultFile *self = lily_malloc(sizeof(CIResultFile));

    self->filename_result = filename_result;
    self->file_input = file_input;
    self->defines = NEW(HashMap);
    self->includes = NEW(HashMap);
    self->enums = NEW(OrderedHashMap);
    self->functions = NEW(OrderedHashMap);
    self->structs = NEW(OrderedHashMap);
    self->unions = NEW(OrderedHashMap);
    self->variables = NEW(OrderedHashMap);
    self->count_error = 0;
    self->scanner =
      NEW(CIScanner,
          NEW(Source, NEW(Cursor, self->file_input.content), &self->file_input),
          &self->count_error,
          standard);
    self->parser = NEW(CIParser, self, &self->scanner);

    return self;
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

    return insert__OrderedHashMap(self->enums, enum_name->buffer, enum_);
}

const CIDecl *
add_function__CIResultFile(const CIResultFile *self, CIDecl *function)
{
    const String *function_name = get_name__CIDecl(function);
    const CIDecl *is_variable_exist =
      search_variable__CIResultFile(self, function_name);

    if (is_variable_exist) {
        return is_variable_exist;
    }

    return insert__OrderedHashMap(
      self->functions, function_name->buffer, function);
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

    return insert__OrderedHashMap(self->structs, struct_name->buffer, struct_);
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

    return insert__OrderedHashMap(self->unions, union_name->buffer, union_);
}

const CIDecl *
add_variable__CIResultFile(const CIResultFile *self, CIDecl *variable)
{
    const String *variable_name = get_name__CIDecl(variable);
    const CIDecl *is_function_exist =
      search_function__CIResultFile(self, variable_name);

    if (is_function_exist) {
        return is_function_exist;
    }

    return insert__OrderedHashMap(
      self->variables, get_name__CIDecl(variable)->buffer, variable);
}

CIDecl *
search_enum__CIResultFile(const CIResultFile *self, const String *name)
{
    return get__OrderedHashMap(self->enums, name->buffer);
}

CIDecl *
search_function__CIResultFile(const CIResultFile *self, const String *name)
{
    return get__OrderedHashMap(self->functions, name->buffer);
}

CIDecl *
search_struct__CIResultFile(const CIResultFile *self, const String *name)
{
    return get__OrderedHashMap(self->structs, name->buffer);
}

CIDecl *
search_union__CIResultFile(const CIResultFile *self, const String *name)
{
    return get__OrderedHashMap(self->unions, name->buffer);
}

CIDecl *
search_variable__CIResultFile(const CIResultFile *self, const String *name)
{
    return get__OrderedHashMap(self->variables, name->buffer);
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
    FREE_ORD_HASHMAP_VALUES(self->enums, CIDecl);
    FREE(OrderedHashMap, self->enums);
    FREE_ORD_HASHMAP_VALUES(self->functions, CIDecl);
    FREE(OrderedHashMap, self->functions);
    FREE_ORD_HASHMAP_VALUES(self->structs, CIDecl);
    FREE(OrderedHashMap, self->structs);
    FREE_ORD_HASHMAP_VALUES(self->unions, CIDecl);
    FREE(OrderedHashMap, self->unions);
    FREE_ORD_HASHMAP_VALUES(self->variables, CIDecl);
    FREE(OrderedHashMap, self->variables);
    FREE(CIScanner, &self->scanner);
    FREE(CIParser, &self->parser);
    lily_free(self);
}

DESTRUCTOR(CIResult, const CIResult *self)
{
    FREE_HASHMAP_VALUES(self->headers, CIResultFile);
    FREE(HashMap, self->headers);
    FREE_HASHMAP_VALUES(self->sources, CIResultFile);
    FREE(HashMap, self->sources);
}
