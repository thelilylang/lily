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

#include <base/assert.h>
#include <base/cli.h>

#include <stdio.h>
#include <stdlib.h>

CONSTRUCTOR(Cli, Cli, const SizedArray *args, const char *name)
{
    String *full_command = NEW(String);

    {
        SizedArrayIter iter = NEW(SizedArrayIter, args);
        char *current = NULL;

        while ((current = next__SizedArrayIter(&iter))) {
            push_str__String(full_command, current);
            push__String(full_command, ' ');
        }
    }

    if (full_command->len > 0) {
        pop__String(full_command);
    }

    return (Cli){ .full_command = full_command,
                  .usage = NULL,
                  .name = name,
                  .commands = NEW(OrderedHashMap),
                  .options = NEW(OrderedHashMap),
                  .sections = NULL,
                  .has_value = CLI_VALUE_KIND_NONE };
}

void
add_command__Cli(Cli *self, CliCommand *command)
{
    ASSERT(
      !insert__OrderedHashMap(self->commands, (char *)command->name, command));
}

void
add_option__Cli(Cli *self, CliOption *option)
{
    ASSERT(
      !insert__OrderedHashMap(self->options, (char *)option->name, option));
}

DESTRUCTOR(Cli, const Cli *self)
{
    FREE(String, self->full_command);
    FREE(String, self->usage);

    FREE_ORD_HASHMAP_VALUES(self->commands, CliCommand);
    FREE(OrderedHashMap, self->commands);

    FREE_ORD_HASHMAP_VALUES(self->options, CliOption);
    FREE(OrderedHashMap, self->options);

    if (self->sections) {
        FREE_BUFFER_ITEMS(
          self->sections->buffer, self->sections->len, CliSection);
        FREE(Vec, self->sections);
    }
}
