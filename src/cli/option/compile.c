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
#include <base/str.h>

#include <cli/option/compile.h>

#include <stdlib.h>
#include <string.h>

// Convert option in CompileOption struct.
static CompileOption *
get__CompileOption(const char *option);

static VARIANT_DESTRUCTOR(CompileOption, target, CompileOption *self);

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

VARIANT_CONSTRUCTOR(CompileOption *, CompileOption, target, String *target)
{
    CompileOption *self = lily_malloc(sizeof(CompileOption));

    self->kind = COMPILE_OPTION_KIND_TARGET;
    self->target = target;

    return self;
}

static CompileOption *
get__CompileOption(const char *option)
{
    if (!strcmp(option, "--cc-ir"))
        return NEW(CompileOption, COMPILE_OPTION_KIND_CC_IR);
    else if (!strcmp(option, "--cpp-ir"))
        return NEW(CompileOption, COMPILE_OPTION_KIND_CPP_IR);
    else if (!strcmp(option, "--dump-ir"))
        return NEW(CompileOption, COMPILE_OPTION_KIND_DUMP_IR);
    else if (!strcmp(option, "--dump-parser"))
        return NEW(CompileOption, COMPILE_OPTION_KIND_DUMP_PARSER);
    else if (!strcmp(option, "--dump-scanner"))
        return NEW(CompileOption, COMPILE_OPTION_KIND_DUMP_SCANNER);
    else if (!strcmp(option, "--dump-tc"))
        return NEW(CompileOption, COMPILE_OPTION_KIND_DUMP_TYPECHECK);
    else if (!strcmp(option, "-h") || !strcmp(option, "--help"))
        return NEW(CompileOption, COMPILE_OPTION_KIND_HELP);
    else if (!strcmp(option, "--js-ir"))
        return NEW(CompileOption, COMPILE_OPTION_KIND_JS_IR);
    else if (!strcmp(option, "--llvm-ir"))
        return NEW(CompileOption, COMPILE_OPTION_KIND_LLVM_IR);
    else if (!strcmp(option, "--run-ir"))
        return NEW(CompileOption, COMPILE_OPTION_KIND_RUN_IR);
    else if (!strcmp(option, "--run-parser"))
        return NEW(CompileOption, COMPILE_OPTION_KIND_RUN_PARSER);
    else if (!strcmp(option, "--run-scanner"))
        return NEW(CompileOption, COMPILE_OPTION_KIND_RUN_SCANNER);
    else if (!strcmp(option, "--run-tc"))
        return NEW(CompileOption, COMPILE_OPTION_KIND_RUN_TYPECHECK);
    else if (option[0] == 't' && option[1] == 'a' && option[2] == 'r' &&
             option[3] == 'g' && option[4] == 'e' && option[5] == 't' &&
             option[6] == '=') {
        String *target = NEW(String);

        for (Usize i = 0; option[i]; ++i) {
            push__String(target, option[i]);
        }

        return NEW_VARIANT(CompileOption, target, target);
    } else if (!strcmp(option, "--wasm-ir"))
        return NEW(CompileOption, COMPILE_OPTION_KIND_WASM_IR);
    else
        return NEW_VARIANT(CompileOption, error, option);
}

Vec *
parse__CompileOption(const char **options, const Usize options_size)
{
    Vec *res = NEW(Vec);

    for (Usize i = 0; i < options_size; ++i) {
        if (options[i][0] == '-') {
            push__Vec(res, get__CompileOption(options[i]));
        } else {
            push__Vec(res, NEW_VARIANT(CompileOption, filename, options[i]));
        }
    }

    return res;
}

VARIANT_DESTRUCTOR(CompileOption, target, CompileOption *self)
{
    FREE(String, self->target);
    lily_free(self);
}

DESTRUCTOR(CompileOption, CompileOption *self)
{
    switch (self->kind) {
        case COMPILE_OPTION_KIND_TARGET:
            FREE_VARIANT(CompileOption, target, self);
            break;
        default:
            lily_free(self);
    }
}
