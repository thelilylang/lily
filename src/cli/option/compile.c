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

#include <cli/option/compile.h>

#include <stdlib.h>
#include <string.h>

// Convert option in CompileOption struct.
static CompileOption *
get__CompileOption(const char *option);

CONSTRUCTOR(CompileOption *, CompileOption, enum CompileOptionKind kind)
{
    CompileOption *self = lily_malloc(sizeof(CompileOption));

    self->kind = kind;

    return self;
}

VARIANT_CONSTRUCTOR(CompileOption *, CompileOption, error, const char *error)
{
    CompileOption *self = lily_malloc(sizeof(CompileOption));

    self->kind = COMPILE_OPTION_KIND_ERROR;
    self->error = error;

    return self;
}

VARIANT_CONSTRUCTOR(CompileOption *,
                    CompileOption,
                    filename,
                    const char *filename)
{
    CompileOption *self = lily_malloc(sizeof(CompileOption));

    self->kind = COMPILE_OPTION_KIND_FILENAME;
    self->filename = filename;

    return self;
}

static CompileOption *
get__CompileOption(const char *option)
{
    if (!strcmp(option, "--dump-ir"))
        return NEW(CompileOption, COMPILE_OPTION_KIND_DUMP_IR);
    else if (!strcmp(option, "--dump-parser"))
        return NEW(CompileOption, COMPILE_OPTION_KIND_DUMP_PARSER);
    else if (!strcmp(option, "--dump-scanner"))
        return NEW(CompileOption, COMPILE_OPTION_KIND_DUMP_SCANNER);
    else if (!strcmp(option, "--dump-tc"))
        return NEW(CompileOption, COMPILE_OPTION_KIND_DUMP_TYPECHECK);
    else if (!strcmp(option, "-h") || !strcmp(option, "--help"))
        return NEW(CompileOption, COMPILE_OPTION_KIND_HELP);
    else if (!strcmp(option, "--run-ir"))
        return NEW(CompileOption, COMPILE_OPTION_KIND_RUN_IR);
    else if (!strcmp(option, "--run-parser"))
        return NEW(CompileOption, COMPILE_OPTION_KIND_RUN_PARSER);
    else if (!strcmp(option, "--run-scanner"))
        return NEW(CompileOption, COMPILE_OPTION_KIND_RUN_SCANNER);
    else if (!strcmp(option, "--run-tc"))
        return NEW(CompileOption, COMPILE_OPTION_KIND_RUN_TYPECHECK);
    else
        return NEW_VARIANT(CompileOption, error, option);
}

Vec *
parse__CompileOption(const char **options, const Usize options_size)
{
    Vec *res = NEW(Vec);

    for (Usize i = 0; i < options_size; i++) {
        if (options[i][0] == '-') {
            push__Vec(res, get__CompileOption(options[i]));
        } else {
            push__Vec(res, NEW_VARIANT(CompileOption, filename, options[i]));
        }
    }

    return res;
}

DESTRUCTOR(CompileOption, CompileOption *self)
{
    lily_free(self);
}
