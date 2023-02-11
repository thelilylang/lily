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

#include <cli/emit.h>
#include <cli/option/build.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Convert option in BuildOption struct.
static BuildOption *
get__BuildOption(const char *option);

CONSTRUCTOR(BuildOption *, BuildOption, enum BuildOptionKind kind)
{
    BuildOption *self = lily_malloc(sizeof(BuildOption));

    self->kind = kind;

    return self;
}

VARIANT_CONSTRUCTOR(BuildOption *, BuildOption, error, const char *error)
{
    BuildOption *self = lily_malloc(sizeof(BuildOption));

    self->kind = BUILD_OPTION_KIND_ERROR;
    self->error = error;

    return self;
}

BuildOption *
get__BuildOption(const char *option)
{
    if (!strcmp(option, "-h") || !strcmp(option, "--help"))
        return NEW(BuildOption, BUILD_OPTION_KIND_HELP);
    else if (!strcmp(option, "-v"))
        return NEW(BuildOption, BUILD_OPTION_KIND_VERBOSE);
    else
        return NEW_VARIANT(BuildOption, error, option);
}

Vec *
parse__BuildOption(const char **options, const Usize options_size)
{
    Vec *res = NEW(Vec);

    for (Usize i = 0; i < options_size; i++) {
        if (options[i][0] == '-') {
            push__Vec(res, get__BuildOption(options[i]));
        } else {
            EMIT_ERROR("expected option");
            EMIT_NOTE("an option must be start by `-`");
            EMIT_HELP("please see the help of build command");
            exit(1);
        }
    }

    return res;
}

DESTRUCTOR(BuildOption, BuildOption *self)
{
    lily_free(self);
}
