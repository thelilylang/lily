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

#include <base/macros.h>

#include <core/lily/package/package.h>
#include <core/lily/precompile.h>

#include <ctype.h>
#include <string.h>

#ifdef ENV_DEBUG
#include <base/print.h>
#endif

// Free LilyImportValue type (LILY_IMPORT_VALUE_KIND_ACCESS).
static VARIANT_DESTRUCTOR(LilyImportValue, access, LilyImportValue *self);

// Free LilyImportValue type (LILY_IMPORT_VALUE_KIND_FILE).
static VARIANT_DESTRUCTOR(LilyImportValue, file, LilyImportValue *self);

// Free LilyImportValue type (LILY_IMPORT_VALUE_KIND_LIBRARY).
static VARIANT_DESTRUCTOR(LilyImportValue, library, LilyImportValue *self);

// Free LilyImportValue type (LILY_IMPORT_VALUE_KIND_PACKAGE).
static VARIANT_DESTRUCTOR(LilyImportValue, package, LilyImportValue *self);

// Free LilyImportValue type (LILY_IMPORT_VALUE_KIND_SELECT).
static VARIANT_DESTRUCTOR(LilyImportValue, select, LilyImportValue *self);

/// @return Vec<LilyImportValue>
static Vec *
precompile_import_access__LilyPrecompile(LilyPrecompile *self,
                                         const String *import_value,
                                         const Location *location,
                                         Usize *position,
                                         bool parse_selector);

static LilyImport *
precompile_import__LilyPrecompile(LilyPrecompile *self,
                                  const LilyPreparserImport *import);

CONSTRUCTOR(LilyImportValue *, LilyImportValue, enum LilyImportValueKind kind)
{
    LilyImportValue *self = lily_malloc(sizeof(LilyImportValue));

    self->kind = kind;

    return self;
}

VARIANT_CONSTRUCTOR(LilyImportValue *, LilyImportValue, access, String *access)
{
    LilyImportValue *self = lily_malloc(sizeof(LilyImportValue));

    self->kind = LILY_IMPORT_VALUE_KIND_ACCCESS;
    self->access = access;

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

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, LilyImportValueKind, enum LilyImportValueKind self)
{
    switch (self) {
        case LILY_IMPORT_VALUE_KIND_ACCCESS:
            return "LILY_IMPORT_VALUE_KIND_ACCESS";
        case LILY_IMPORT_VALUE_KIND_BUILTIN:
            return "LILY_IMPORT_VALUE_KIND_BUILTIN";
        case LILY_IMPORT_VALUE_KIND_CORE:
            return "LILY_IMPORT_VALUE_KIND_CORE";
        case LILY_IMPORT_VALUE_KIND_FILE:
            return "LILY_IMPORT_VALUE_KIND_FILE";
        case LILY_IMPORT_VALUE_KIND_LIBRARY:
            return "LILY_IMPORT_VALUE_KIND_LIBRARY";
        case LILY_IMPORT_VALUE_KIND_PACKAGE:
            return "LILY_IMPORT_VALUE_KIND_PACKAGE";
        case LILY_IMPORT_VALUE_KIND_SELECT_ALL:
            return "LILY_IMPORT_VALUE_KIND_SELECT_ALL";
        case LILY_IMPORT_VALUE_KIND_SELECT:
            return "LILY_IMPORT_VALUE_KIND_SELECT";
        case LILY_IMPORT_VALUE_KIND_STD:
            return "LILY_IMPORT_VALUE_KIND_STD";
        case LILY_IMPORT_VALUE_KIND_URL:
            return "LILY_IMPORT_VALUE_KIND_URL";
        default:
            UNREACHABLE("unknown variant");
    }
}

String *
IMPL_FOR_DEBUG(to_string, LilyImportValue, const LilyImportValue *self)
{
    switch (self->kind) {
        case LILY_IMPORT_VALUE_KIND_ACCCESS:
            return format__String(
              "LilyImportValue{{ kind = {s}, access = {S} }",
              to_string__Debug__LilyImportValueKind(self->kind),
              self->access);
        case LILY_IMPORT_VALUE_KIND_FILE:
            return format__String(
              "LilyImportValue{{ kind = {s}, file = {S} }",
              to_string__Debug__LilyImportValueKind(self->kind),
              self->file);
        case LILY_IMPORT_VALUE_KIND_LIBRARY:
            return format__String(
              "LilyImportValue{{ kind = {s}, library = {S} }",
              to_string__Debug__LilyImportValueKind(self->kind),
              self->library);
        case LILY_IMPORT_VALUE_KIND_PACKAGE:
            return format__String(
              "LilyImportValue{{ kind = {s}, package = {S} }",
              to_string__Debug__LilyImportValueKind(self->kind),
              self->package);
        case LILY_IMPORT_VALUE_KIND_SELECT: {
            String *res =
              format__String("LilyImportValue{{ kind = {s}, select = {{ ",
                             to_string__Debug__LilyImportValueKind(self->kind));

            for (Usize i = 0; i < self->select->len; i++) {
                Vec *item = get__Vec(self->select, i);

                push_str__String(res, " { ");

                for (Usize j = 0; j < item->len; j++) {
                    String *s =
                      to_string__Debug__LilyImportValue(get__Vec(item, j));

                    APPEND_AND_FREE(res, s);

                    if (j != item->len - 1) {
                        push_str__String(res, ", ");
                    }
                }

                push_str__String(res, " } ");

                if (i != self->select->len - 1) {
                    push_str__String(res, ", ");
                }
            }

            push_str__String(res, " }");

            return res;
        }
        default:
            return format__String(
              "LilyImportValue{{ kind = {s} }",
              to_string__Debug__LilyImportValueKind(self->kind));
    }
}

void
IMPL_FOR_DEBUG(debug, LilyImportValue, const LilyImportValue *self)
{
    PRINTLN("{Sr}", to_string__Debug__LilyImportValue(self));
}
#endif

VARIANT_DESTRUCTOR(LilyImportValue, access, LilyImportValue *self)
{
    FREE(String, self->access);
    lily_free(self);
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
    for (Usize i = 0; i < self->select->len; i++) {
        Vec *item = self->select->buffer[i];

        FREE_BUFFER_ITEMS(item->buffer, item->len, LilyImportValue);
        FREE(Vec, item);
    }

    FREE(Vec, self->select);
    lily_free(self);
}

DESTRUCTOR(LilyImportValue, LilyImportValue *self)
{
    switch (self->kind) {
        case LILY_IMPORT_VALUE_KIND_ACCCESS:
            FREE_VARIANT(LilyImportValue, access, self);
            break;
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
    return (LilyPrecompile){ .preparser = preparser,
                             .package = package,
                             .count_error = 0 };
}

#include <stdio.h>

Vec *
precompile_import_access__LilyPrecompile(LilyPrecompile *self,
                                         const String *import_value,
                                         const Location *location,
                                         Usize *position,
                                         bool parse_selector)
{
    Vec *values = NEW(Vec);

    while (*position < import_value->len) {
        if (isalpha(import_value->buffer[*position]) ||
            import_value->buffer[*position] == '_') {
            String *access = NEW(String);

            while (
              (isalpha(import_value->buffer[*position]) ||
               import_value->buffer[*position] == '_') &&
              (import_value->buffer[*position] != '.' ||
               (import_value->buffer[*position] != ',' && parse_selector)) &&
              import_value->buffer[*position]) {
                push__String(access, import_value->buffer[*position]);
                *position += 1;
            }

            push__Vec(values, NEW_VARIANT(LilyImportValue, access, access));
        } else if (import_value->buffer[*position] == '{') {
            *position += 1;

            Vec *selector_res = NEW(Vec); // Vec<Vec<LilyImportValue*>*>*
            Usize selector_position = 0;

            while (import_value->buffer[*position] != '}' &&
                   import_value->buffer[*position]) {
                String *selector_value = NEW(String);

                while ((import_value->buffer[*position] != '}' &&
                        import_value->buffer[*position] != ',') &&
                       import_value->buffer[*position]) {
                    push__String(selector_value,
                                 import_value->buffer[*position]);
                    *position += 1;
                }

                if (!import_value->buffer[*position]) {
                    goto expected_r_brace;
                }

                if (import_value->buffer[*position] != '}') {
                    *position += 1;
                }

                Vec *selector_item = precompile_import_access__LilyPrecompile(
                  self, selector_value, location, &selector_position, true);

                if (selector_item) {
                    push__Vec(selector_res, selector_item);
                    FREE(String, selector_value);
                } else {
                    FREE(String, selector_value);

                    return NULL;
                }
            }

            if (!import_value->buffer[*position]) {
                goto expected_r_brace;
            } else {
                *position += 1;
                goto select_success_way;
            }

        expected_r_brace : {
            if (!import_value->buffer[*position]) {
                emit__Diagnostic(
                  NEW_VARIANT(
                    Diagnostic,
                    simple_lily_error,
                    self->preparser->scanner->source.file,
                    location,
                    NEW(LilyError,
                        LILY_ERROR_KIND_UNEXPECTED_CHARACTER_IN_IMPORT_VALUE),
                    NULL,
                    NULL,
                    from__String("expected `}` to close the selector")),
                  &self->count_error);

                return NULL;
            }
        }

        select_success_way : {
            push__Vec(values,
                      NEW_VARIANT(LilyImportValue, select, selector_res));
        }

        } else if (import_value->buffer[*position] == '.' &&
                   import_value->buffer[*position + 1] == '*') {
            *position += 2;
            push__Vec(values,
                      NEW(LilyImportValue, LILY_IMPORT_VALUE_KIND_SELECT_ALL));
        } else if (import_value->buffer[*position] == '.') {
            *position += 1;
        } else if (isspace(import_value->buffer[*position])) {
            while (isspace(import_value->buffer[*position])) {
                *position += 1;
            }

            continue;
        } else {
            emit__Diagnostic(
              NEW_VARIANT(
                Diagnostic,
                simple_lily_error,
                self->preparser->scanner->source.file,
                location,
                NEW(LilyError,
                    LILY_ERROR_KIND_UNEXPECTED_CHARACTER_IN_IMPORT_VALUE),
                NULL,
                NULL,
                NULL),
              &self->count_error);

            FREE_BUFFER_ITEMS(values->buffer, values->len, LilyImportValue);
            FREE(Vec, values);

            return NULL;
        }
    }

    return values;
}

LilyImport *
precompile_import__LilyPrecompile(LilyPrecompile *self,
                                  const LilyPreparserImport *import)
{
    Vec *values = NEW(Vec);
    Usize position = 0;

    switch (get__String(import->value, position++)) {
        case '@':
            break;
        default: {
            emit__Diagnostic(
              NEW_VARIANT(Diagnostic,
                          simple_lily_error,
                          self->preparser->scanner->source.file,
                          &import->location,
                          NEW(LilyError, LILY_ERROR_KIND_BAD_IMPORT_VALUE),
                          NULL,
                          NULL,
                          from__String("import value must be start by `@`")),
              &self->count_error);

            return NULL;
        }
    }

    {
        String *name = NEW(String);

        while (import->value->buffer[position] != '(' &&
               import->value->buffer[position] != '.' &&
               import->value->buffer[position]) {
            push__String(name, import->value->buffer[position++]);
        }

        if (!strcmp(name->buffer, "builtin")) {
            push__Vec(values,
                      NEW(LilyImportValue, LILY_IMPORT_VALUE_KIND_BUILTIN));
        } else if (!strcmp(name->buffer, "core")) {
            push__Vec(values,
                      NEW(LilyImportValue, LILY_IMPORT_VALUE_KIND_CORE));
        } else if (!strcmp(name->buffer, "std")) {
            push__Vec(values, NEW(LilyImportValue, LILY_IMPORT_VALUE_KIND_STD));
        } else if (strcmp(name->buffer, "file") &&
                   strcmp(name->buffer, "library") &&
                   strcmp(name->buffer, "package")) {
            emit__Diagnostic(
              NEW_VARIANT(
                Diagnostic,
                simple_lily_error,
                self->preparser->scanner->source.file,
                &import->location,
                NEW(LilyError, LILY_ERROR_KIND_UNKNOWN_IMPORT_AT_FLAG),
                init__Vec(1,
                          from__String("expected `@builtin`, `@core`, `@file`, "
                                       "`@library`, `@package`, `@std`")),
                NULL,
                NULL),
              &self->count_error);

            FREE(String, name);
            FREE(Vec, values);

            return NULL;
        } else {
            if (!import->value->buffer[position++]) {
                emit__Diagnostic(
                  NEW_VARIANT(
                    Diagnostic,
                    simple_lily_error,
                    self->preparser->scanner->source.file,
                    &import->location,
                    NEW(LilyError,
                        LILY_ERROR_KIND_UNEXPECTED_CHARACTER_IN_IMPORT_VALUE),
                    NULL,
                    NULL,
                    from__String(
                      "expected `(` after `@file`, `@library` or `@package`")),
                  &self->count_error);

                FREE(Vec, values);
                FREE(String, name);

                return NULL;
            }

            String *flag_value = NEW(String);

            while (import->value->buffer[position] != ')' &&
                   import->value->buffer[position]) {
                push__String(flag_value, import->value->buffer[position++]);
            }

            if (!import->value->buffer[position++]) {
                emit__Diagnostic(
                  NEW_VARIANT(
                    Diagnostic,
                    simple_lily_error,
                    self->preparser->scanner->source.file,
                    &import->location,
                    NEW(LilyError,
                        LILY_ERROR_KIND_UNEXPECTED_CHARACTER_IN_IMPORT_VALUE),
                    NULL,
                    NULL,
                    from__String(
                      "expected `)` after @file, @library, @package value")),
                  &self->count_error);

                FREE(String, flag_value);
                FREE(String, name);
                FREE(Vec, values);

                return NULL;
            }

            if (!strcmp(name->buffer, "file")) {
                push__Vec(values,
                          NEW_VARIANT(LilyImportValue, file, flag_value));
            } else if (!strcmp(name->buffer, "library")) {
                push__Vec(values,
                          NEW_VARIANT(LilyImportValue, library, flag_value));
            } else {
                push__Vec(values,
                          NEW_VARIANT(LilyImportValue, package, flag_value));
            }
        }

        FREE(String, name);
    }

    if (position < import->value->len) {
        String *rest_import_value = take_slice__String(import->value, position);
        Usize position = 0;

        Vec *access = precompile_import_access__LilyPrecompile(
          self, rest_import_value, &import->location, &position, false);

        if (access) {
            append__Vec(values, access);
            FREE(Vec, access);
        } else {
            FREE(String, rest_import_value);
            return NULL;
        }

        FREE(String, rest_import_value);
    }

    return NEW(LilyImport,
               values,
               clone__String(import->as)); // TODO: avoid clone__String
}

void
run__LilyPrecompile(LilyPrecompile *self)
{
    // 1. Precompile all imports
    for (Usize i = 0; i < self->preparser->public_imports->len; i++) {
        LilyImport *import = precompile_import__LilyPrecompile(
          self, get__Vec(self->preparser->public_imports, i));

        if (import) {
            push__Vec(self->package->public_imports, import);
        }
    }

    for (Usize i = 0; i < self->preparser->private_imports->len; i++) {
        LilyImport *import = precompile_import__LilyPrecompile(
          self, get__Vec(self->preparser->private_imports, i));

        if (import) {
            push__Vec(self->package->private_imports, import);
        }
    }
}