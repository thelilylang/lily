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
#include <base/cli/option.h>
#include <base/new.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static CliOption *
short_name__CliOption(CliOption *self, char *name);

static CliOption *
value__CliOption(CliOption *self, CliValue *value);

static CliOption *
help__CliOption(CliOption *self, char *help);

static CliOption *
default_action__CliOption(CliOption *self, CliDefaultAction *default_action);

CONSTRUCTOR(CliOption *, CliOption, const char *name)
{
    ASSERT(name[0] == '-' && strlen(name) > 2);

    CliOption *self = lily_malloc(sizeof(CliOption));

    self->name = name;
    self->short_name = NULL;
    self->value = NULL;
    self->help = NULL;
    self->default_action = NULL;
    self->ref_count = 0;

    self->$short_name = &short_name__CliOption;
    self->$value = &value__CliOption;
    self->$help = &help__CliOption;
    self->$default_action = &default_action__CliOption;

    return self;
}

CliOption *
short_name__CliOption(CliOption *self, char *name)
{
    ASSERT(!self->short_name && name);
    ASSERT(strlen(name) == 2);

    self->short_name = name;
    return self;
}

CliOption *
value__CliOption(CliOption *self, CliValue *value)
{
    ASSERT(!self->value && value);

    self->value = value;
    return self;
}

CliOption *
help__CliOption(CliOption *self, char *help)
{
    ASSERT(!self->help && help);

    self->help = help;
    return self;
}

CliOption *
default_action__CliOption(CliOption *self, CliDefaultAction *default_action)
{
    ASSERT(!self->default_action && default_action);

    self->default_action = default_action;
    return self;
}

DESTRUCTOR(CliOption, CliOption *self)
{
    if (self->ref_count > 0) {
        --self->ref_count;
        return;
    }

    if (self->value) {
        FREE(CliValue, self->value);
    }

    if (self->default_action) {
        FREE(CliDefaultAction, self->default_action);
    }

    lily_free(self);
}
