/*
 * MIT License
 *
 * Copyright (c) 2022-2026 ArthurPV
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
#include <base/cli/result/value.h>
#include <base/new.h>

#include <stdio.h>
#include <stdlib.h>

static inline VARIANT_DESTRUCTOR(CliResultValue, single, CliResultValue *self);

static VARIANT_DESTRUCTOR(CliResultValue, multiple, CliResultValue *self);

static VARIANT_DESTRUCTOR(CliResultValue, multiple_inf, CliResultValue *self);

VARIANT_CONSTRUCTOR(CliResultValue *,
                    CliResultValue,
                    single,
                    const char *name,
                    char *single)
{
    CliResultValue *self = lily_malloc(sizeof(CliResultValue));

    self->kind = CLI_RESULT_VALUE_KIND_SINGLE;
    self->name = name;
    self->single = single;

    return self;
}

VARIANT_CONSTRUCTOR(CliResultValue *,
                    CliResultValue,
                    multiple,
                    const char *name,
                    Vec *multiple)
{
    CliResultValue *self = lily_malloc(sizeof(CliResultValue));

    self->kind = CLI_RESULT_VALUE_KIND_MULTIPLE;
    self->name = name;
    self->multiple = multiple;

    return self;
}

VARIANT_CONSTRUCTOR(CliResultValue *,
                    CliResultValue,
                    multiple_inf,
                    const char *name,
                    Vec *multiple_inf)
{
    CliResultValue *self = lily_malloc(sizeof(CliResultValue));

    self->kind = CLI_RESULT_VALUE_KIND_MULTIPLE_INF;
    self->name = name;
    self->multiple_inf = multiple_inf;

    return self;
}

VARIANT_DESTRUCTOR(CliResultValue, single, CliResultValue *self)
{
    lily_free(self);
}

VARIANT_DESTRUCTOR(CliResultValue, multiple, CliResultValue *self)
{
    FREE(Vec, self->multiple);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CliResultValue, multiple_inf, CliResultValue *self)
{
    FREE(Vec, self->multiple_inf);
    lily_free(self);
}

DESTRUCTOR(CliResultValue, CliResultValue *self)
{
    switch (self->kind) {
        case CLI_RESULT_VALUE_KIND_SINGLE:
            FREE_VARIANT(CliResultValue, single, self);
            break;
        case CLI_RESULT_VALUE_KIND_MULTIPLE:
            FREE_VARIANT(CliResultValue, multiple, self);
            break;
        case CLI_RESULT_VALUE_KIND_MULTIPLE_INF:
            FREE_VARIANT(CliResultValue, multiple_inf, self);
            break;
        default:
            UNREACHABLE("unknown variant");
    }
}
