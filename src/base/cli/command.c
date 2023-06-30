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
#include <base/cli/command.h>
#include <base/new.h>

CONSTRUCTOR(CliCommand *,
            CliCommand,
            const char *cli_name,
            const char *name,
            bool has_options,
            enum CliValueKind has_value,
            bool has_help)
{
    CliCommand *self = lily_malloc(sizeof(CliCommand));

    self->usage =
      format__String("{s} {s} {s} {s}",
                     cli_name,
                     name,
                     has_options ? "[OPTIONS]" : "",
                     has_value == CLI_VALUE_KIND_SINGLE     ? "[VALUE]"
                     : has_value == CLI_VALUE_KIND_MULTIPLE ? "[VALUE]..."
                                                            : "");
    self->name = name;
    self->options = has_options ? NEW(OrderedHashMap) : NULL;
    self->has_value = has_value;
    self->has_help = has_help;

    return self;
}

DESTRUCTOR(CliCommand, CliCommand *self)
{
    FREE(String, self->usage);

    if (self->options) {
        FREE_ORD_HASHMAP_VALUES(self->options, CliOption);
        FREE(OrderedHashMap, self->options);
    }

    lily_free(self);
}
