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
#include <base/assert.h>
#include <base/cli/command.h>
#include <base/cli/help.h>
#include <base/new.h>

#include <stdio.h>
#include <stdlib.h>

CONSTRUCTOR(CliCommand *, CliCommand, Usize id, const char *name)
{
    CliCommand *self = lily_malloc(sizeof(CliCommand));

    self->id = id;
    self->name = name;
    self->options = NEW(OrderedHashMap);
    self->help = NULL;
    self->value = NULL;
    self->default_action = NULL;
    self->deferred = NULL;

    // Add default option
    {
        CliOption *help = NEW(CliOption, CLI_OPTION_ID_HELP, "--help");

        help__CliOption(help, "Print the command help");
        default_action__CliOption(
          help, NEW_VARIANT(CliDefaultAction, help, &generate_help__CliHelp));
        short_name__CliOption(help, "-h");

        option__CliCommand(self, help);
    }

    return self;
}

CliCommand *
option__CliCommand(CliCommand *self, CliOption *option)
{
    ASSERT(option);

    if (option->short_name) {
        insert__OrderedHashMap(
          self->options, option->short_name, ref__CliOption(option));
    }

    insert__OrderedHashMap(self->options, (char *)option->name, option);

    return self;
}

CliCommand *
help__CliCommand(CliCommand *self, char *help)
{
    ASSERT(!self->help && help);

    self->help = help;
    return self;
}

CliCommand *
value__CliCommand(CliCommand *self, CliValue *value)
{
    ASSERT(!self->value && value);

    self->value = value;
    return self;
}

CliCommand *
default_action__CliCommand(CliCommand *self, CliDefaultAction *default_action)
{
    ASSERT(!self->default_action && default_action);

    self->default_action = default_action;
    return self;
}

CliCommand *
defer__CliCommand(CliCommand *self, CliCommand *(*deferred)(CliCommand *))
{
    ASSERT(!self->deferred && deferred);

    self->deferred = deferred;
    return self;
}

DESTRUCTOR(CliCommand, CliCommand *self)
{
    FREE_ORD_HASHMAP_VALUES(self->options, CliOption);
    FREE(OrderedHashMap, self->options);

    if (self->value) {
        FREE(CliValue, self->value);
    }

    if (self->default_action) {
        FREE(CliDefaultAction, self->default_action);
    }

    lily_free(self);
}
