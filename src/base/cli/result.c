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
#include <base/cli/result.h>
#include <base/new.h>

#include <stdio.h>
#include <stdlib.h>

/// @brief Free CliResult type (CLI_RESULT_KIND_COMMAND).
static inline VARIANT_DESTRUCTOR(CliResult, command, CliResult *self);

/// @brief Free CliResult type (CLI_RESULT_KIND_OPTION).
static VARIANT_DESTRUCTOR(CliResult, option, CliResult *self);

/// @brief Free CliResult type (CLI_RESULT_KIND_VALUE).
static VARIANT_DESTRUCTOR(CliResult, value, CliResult *self);

VARIANT_CONSTRUCTOR(CliResult *, CliResult, command, CliResultCommand command)
{
    CliResult *self = lily_malloc(sizeof(CliResult));

    self->kind = CLI_RESULT_KIND_COMMAND;
    self->command = command;

    return self;
}

VARIANT_CONSTRUCTOR(CliResult *, CliResult, option, CliResultOption *option)
{
    CliResult *self = lily_malloc(sizeof(CliResult));

    self->kind = CLI_RESULT_KIND_OPTION;
    self->option = option;

    return self;
}

VARIANT_CONSTRUCTOR(CliResult *, CliResult, value, CliResultValue *value)
{
    CliResult *self = lily_malloc(sizeof(CliResult));

    self->kind = CLI_RESULT_KIND_VALUE;
    self->value = value;

    return self;
}

VARIANT_DESTRUCTOR(CliResult, command, CliResult *self)
{
    FREE(CliResultCommand, &self->command);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CliResult, option, CliResult *self)
{
    FREE(CliResultOption, self->option);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CliResult, value, CliResult *self)
{
    FREE(CliResultValue, self->value);
    lily_free(self);
}

DESTRUCTOR(CliResult, CliResult *self)
{
    switch (self->kind) {
        case CLI_RESULT_KIND_COMMAND:
            FREE_VARIANT(CliResult, command, self);
            break;
        case CLI_RESULT_KIND_OPTION:
            FREE_VARIANT(CliResult, option, self);
            break;
        case CLI_RESULT_KIND_VALUE:
            FREE_VARIANT(CliResult, value, self);
            break;
        default:
            UNREACHABLE("unknown variant");
    }
}
