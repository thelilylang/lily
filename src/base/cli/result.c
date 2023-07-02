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

#include <base/cli/result.h>
#include <base/new.h>

#include <stdio.h>
#include <stdlib.h>

/// @brief Free CliResult type (CLI_RESULT_KIND_COMMAND).
static VARIANT_DESTRUCTOR(CliResult, command, const CliResult *self);

/// @brief Free CliResult type (CLI_RESULT_KIND_OPTIONS).
static VARIANT_DESTRUCTOR(CliResult, options, const CliResult *self);

VARIANT_DESTRUCTOR(CliResult, command, const CliResult *self)
{
    if (self->values) {
        FREE(Vec, self->values);
    }
}

VARIANT_DESTRUCTOR(CliResult, options, const CliResult *self)
{
    if (self->values) {
        FREE(Vec, self->values);
    }

    FREE_BUFFER_ITEMS(
      self->options->buffer, self->options->len, CliResultOption);
    FREE(Vec, self->options);
}

DESTRUCTOR(CliResult, const CliResult *self)
{
    switch (self->kind) {
        case CLI_RESULT_KIND_COMMAND:
            FREE_VARIANT(CliResult, command, self);
            break;
        case CLI_RESULT_KIND_OPTIONS:
            FREE_VARIANT(CliResult, options, self);
            break;
        default:
            UNREACHABLE("unknown variant");
    }
}
