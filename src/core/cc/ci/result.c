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
    self->decls = NEW(OrderedHashMap);
    self->count_error = 0;
    self->scanner =
      NEW(CIScanner,
          NEW(Source, NEW(Cursor, self->file_input.content), &self->file_input),
          &self->count_error,
          standard);
    self->parser = NEW(CIParser, self, &self->scanner);

    return self;
}

bool
add_decl__CIResultFile(const CIResultFile *self, CIDecl *decl)
{
    return insert__OrderedHashMap(
      self->decls, get_name__CIDecl(decl)->buffer, decl);
}

CIDecl *
search_typedef__CIResultFile(const CIResultFile *self, const String *name)
{
    CIDecl *res = get__OrderedHashMap(self->decls, name->buffer);

    if (res && res->storage_class_flag & CI_STORAGE_CLASS_TYPEDEF) {
        return res;
    }

    return NULL;
}

CIDecl *
search_data_type__CIResultFile(const CIResultFile *self, const String *name)
{
    CIDecl *res = get__OrderedHashMap(self->decls, name->buffer);

    if (res) {
        switch (res->kind) {
            case CI_DECL_KIND_ENUM:
            case CI_DECL_KIND_STRUCT:
            case CI_DECL_KIND_UNION:
                return res;
            default:
                return NULL;
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
    FREE_ORD_HASHMAP_VALUES(self->decls, CIDecl);
    FREE(OrderedHashMap, self->decls);
    FREE(CIScanner, &self->scanner);
    lily_free(self);
}

DESTRUCTOR(CIResult, const CIResult *self)
{
    FREE_HASHMAP_VALUES(self->headers, CIResultFile);
    FREE(HashMap, self->headers);
    FREE_HASHMAP_VALUES(self->sources, CIResultFile);
    FREE(HashMap, self->sources);
}
