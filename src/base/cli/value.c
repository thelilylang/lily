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

#include <base/assert.h>
#include <base/cli/value.h>

#include <stdio.h>
#include <stdlib.h>

CONSTRUCTOR(CliValue *,
            CliValue,
            enum CliValueKind kind,
            char *name,
            bool is_required)
{
    CliValue *self = lily_malloc(sizeof(CliValue));

    self->kind = kind;
    self->name = name;
    self->is_required = is_required;

    return self;
}

String *
format__CliValue(CliValue *self)
{
    ASSERT(self);

#define REQUIRED_VALUE_FORMAT "<{s}>{s}"
#define NOT_REQUIRED_VALUE_FORMAT "[{s}]{s}"

    return format__String(self->is_required ? REQUIRED_VALUE_FORMAT
                                            : NOT_REQUIRED_VALUE_FORMAT,
                          self->name,
                          self->kind == CLI_VALUE_KIND_MULTIPLE       ? "..."
                          : self->kind == CLI_VALUE_KIND_MULTIPLE_INF ? "...$"
                                                                      : "");

#undef REQUIRED_VALUE_FORMAT
#undef NOT_REQUIRED_VALUE_FORMAT
}
