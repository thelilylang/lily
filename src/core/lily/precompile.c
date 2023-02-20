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

#include <core/lily/package/package.h>
#include <core/lily/precompile.h>

// Free LilyImportValue type (LILY_IMPORT_VALUE_KIND_FILE).
static VARIANT_DESTRUCTOR(LilyImportValue, file, LilyImportValue *self);

// Free LilyImportValue type (LILY_IMPORT_VALUE_KIND_LIBRARY).
static VARIANT_DESTRUCTOR(LilyImportValue, library, LilyImportValue *self);

// Free LilyImportValue type (LILY_IMPORT_VALUE_KIND_PACKAGE).
static VARIANT_DESTRUCTOR(LilyImportValue, package, LilyImportValue *self);

// Free LilyImportValue type (LILY_IMPORT_VALUE_KIND_SELECT).
static VARIANT_DESTRUCTOR(LilyImportValue, select, LilyImportValue *self);

CONSTRUCTOR(LilyImportValue *, LilyImportValue, enum LilyImportValueKind kind)
{
    LilyImportValue *self = lily_malloc(sizeof(LilyImportValue));

    self->kind = kind;

    return self;
}

VARIANT_CONSTRUCTOR(LilyImportValue *, LilyImportValue, file, String *file)
{
    LilyImportValue *self = lily_malloc(sizeof(LilyImportValue));

    self->kind = LILY_IMPORT_VALUE_KIND_FILE;
    self->file = file;

    return self;
}

VARIANT_CONSTRUCTOR(LilyImportValue *,
                    LilyImportValue,
                    library,
                    String *library)
{
    LilyImportValue *self = lily_malloc(sizeof(LilyImportValue));

    self->kind = LILY_IMPORT_VALUE_KIND_LIBRARY;
    self->library = library;

    return self;
}

VARIANT_CONSTRUCTOR(LilyImportValue *,
                    LilyImportValue,
                    package,
                    String *package)
{
    LilyImportValue *self = lily_malloc(sizeof(LilyImportValue));

    self->kind = LILY_IMPORT_VALUE_KIND_PACKAGE;
    self->package = package;

    return self;
}

VARIANT_CONSTRUCTOR(LilyImportValue *, LilyImportValue, select, Vec *select)
{
    LilyImportValue *self = lily_malloc(sizeof(LilyImportValue));

    self->kind = LILY_IMPORT_VALUE_KIND_SELECT;
    self->select = select;

    return self;
}

VARIANT_DESTRUCTOR(LilyImportValue, file, LilyImportValue *self)
{
    FREE(String, self->file);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyImportValue, library, LilyImportValue *self)
{
    FREE(String, self->library);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyImportValue, package, LilyImportValue *self)
{
    FREE(String, self->package);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyImportValue, select, LilyImportValue *self)
{
    FREE_BUFFER_ITEMS(self->select->buffer, self->select->len, LilyImportValue);
    FREE(Vec, self->select);
    lily_free(self);
}

DESTRUCTOR(LilyImportValue, LilyImportValue *self)
{
    switch (self->kind) {
        case LILY_IMPORT_VALUE_KIND_FILE:
            FREE_VARIANT(LilyImportValue, file, self);
            break;
        case LILY_IMPORT_VALUE_KIND_LIBRARY:
            FREE_VARIANT(LilyImportValue, library, self);
            break;
        case LILY_IMPORT_VALUE_KIND_PACKAGE:
            FREE_VARIANT(LilyImportValue, package, self);
            break;
        case LILY_IMPORT_VALUE_KIND_SELECT:
            FREE_VARIANT(LilyImportValue, select, self);
            break;
        default:
            lily_free(self);
    }
}

CONSTRUCTOR(LilyImport *, LilyImport, Vec *values, String *as)
{
    LilyImport *self = lily_malloc(sizeof(LilyImport));

    self->values = values;
    self->as = as;

    return self;
}

DESTRUCTOR(LilyImport, LilyImport *self)
{
    FREE_BUFFER_ITEMS(self->values->buffer, self->values->len, LilyImportValue);
    FREE(Vec, self->values);
    FREE(String, self->as);
    lily_free(self);
}

CONSTRUCTOR(LilyPrecompile,
            LilyPrecompile,
            const LilyPreparser *preparser,
            LilyPackage *package)
{
    return (LilyPrecompile){ .preparser = preparser, .package = package };
}

void
run__LilyPrecompile(LilyPrecompile *self)
{
}