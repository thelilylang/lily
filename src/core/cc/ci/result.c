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

CONSTRUCTOR(CIResultFile *, CIResultFile, String *filename)
{
    CIResultFile *self = lily_malloc(sizeof(CIResultFile));

    self->filename = filename;
    self->defines = NEW(HashMap);
    self->includes = NEW(HashMap);
    self->decls = NEW(HashMap);

    return self;
}

DESTRUCTOR(CIResultFile, CIResultFile *self)
{
    FREE(String, self->filename);
    FREE_HASHMAP_VALUES(self->defines, CIResultDefineVec);
    FREE(HashMap, self->defines);
    FREE_HASHMAP_VALUES(self->includes, CIResultInclude);
    FREE(HashMap, self->includes);
    FREE_HASHMAP_VALUES(self->decls, CIDecl);
    FREE(HashMap, self->decls);
    lily_free(self);
}

DESTRUCTOR(CIResult, const CIResult *self)
{
    FREE_HASHMAP_VALUES(self->headers, CIResultFile);
    FREE(HashMap, self->headers);
    FREE_HASHMAP_VALUES(self->sources, CIResultFile);
    FREE(HashMap, self->sources);
}