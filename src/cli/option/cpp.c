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
#include <base/new.h>

#include <cli/option/cpp.h>

#include <stdlib.h>
#include <string.h>

// Convert option in CppOption struct.
static CppOption *
get__CppOption(const char *option);

CONSTRUCTOR(CppOption *, CppOption, enum CppOptionKind kind)
{
    CppOption *self = lily_malloc(sizeof(CppOption));

    self->kind = kind;

    return self;
}

VARIANT_CONSTRUCTOR(CppOption *, CppOption, error, const char *error)
{
    CppOption *self = lily_malloc(sizeof(CppOption));

    self->kind = CPP_OPTION_KIND_ERROR;
    self->error = error;

    return self;
}

VARIANT_CONSTRUCTOR(CppOption *, CppOption, filename, const char *filename)
{
    CppOption *self = lily_malloc(sizeof(CppOption));

    self->kind = CPP_OPTION_KIND_FILENAME;
    self->filename = filename;

    return self;
}

CppOption *
get__CppOption(const char *option)
{
    if (!strcmp(option, "-h") || !strcmp(option, "--help"))
        return NEW(CppOption, CPP_OPTION_KIND_HELP);
    else
        return NEW_VARIANT(CppOption, error, option);
}

Vec *
parse__CppOption(const char **options, const Usize options_size)
{
    Vec *res = NEW(Vec);

    for (Usize i = 0; i < options_size; ++i) {
        if (options[i][0] == '-') {
            push__Vec(res, get__CppOption(options[i]));
        } else {
            push__Vec(res, NEW_VARIANT(CppOption, filename, options[i]));
        }
    }

    return res;
}

DESTRUCTOR(CppOption, CppOption *self)
{
    lily_free(self);
}
