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
#include <base/cli/option.h>
#include <base/new.h>

CONSTRUCTOR(CliOption *,
            CliOption,
            String *command_usage,
            const char *name,
            const char *help,
            enum CliValueKind has_value)
{
    CliOption *self = lily_malloc(sizeof(CliOption));

    self->usage = format__String("{S} {s}",
                                 command_usage,
                                 has_value == CLI_VALUE_KIND_SINGLE ? "[VALUE]"
                                 : CLI_VALUE_KIND_MULTIPLE ? "[VALUE]..."
                                                           : "");
    self->name = name;
    self->help = help;
    self->has_value = has_value;

    return self;
}

DESTRUCTOR(CliOption, CliOption *self)
{
    FREE(String, self->usage);
    lily_free(self);
}
