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

#include <cli/option/init.h>

#include <stdlib.h>
#include <string.h>

// Convert option in InitOption struct.
static InitOption *
get__InitOption(const char *option);

CONSTRUCTOR(InitOption *, InitOption, enum InitOptionKind kind)
{
    InitOption *self = lily_malloc(sizeof(InitOption));

    self->kind = kind;

    return self;
}

VARIANT_CONSTRUCTOR(InitOption *, InitOption, error, const char *error)
{
    InitOption *self = lily_malloc(sizeof(InitOption));

    self->kind = INIT_OPTION_KIND_ERROR;
    self->error = error;

    return self;
}

VARIANT_CONSTRUCTOR(InitOption *, InitOption, path, const char *path)
{
    InitOption *self = lily_malloc(sizeof(InitOption));

    self->kind = INIT_OPTION_KIND_PATH;
    self->path = path;

    return self;
}

InitOption *
get__InitOption(const char *option)
{
    if (!strcmp(option, "-h") || !strcmp(option, "--help"))
        return NEW(InitOption, INIT_OPTION_KIND_HELP);
    else
        return NEW_VARIANT(InitOption, error, option);
}

Vec *
parse__InitOption(const char **options, const Usize options_size)
{
    Vec *res = NEW(Vec);

    for (Usize i = 0; i < options_size; ++i) {
        if (options[i][0] == '-') {
            push__Vec(res, get__InitOption(options[i]));
        } else {
            push__Vec(res, NEW_VARIANT(InitOption, path, options[i]));
        }
    }

    return res;
}

DESTRUCTOR(InitOption, InitOption *self)
{
    lily_free(self);
}
