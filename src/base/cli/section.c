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
#include <base/cli/section.h>
#include <base/new.h>

#include <stdio.h>
#include <stdlib.h>

/// @brief Free CliSection type (CLI_SECTION_KIND_AUTHOR).
static VARIANT_DESTRUCTOR(CliSection, author, CliSection *self);

/// @brief Free CliSection type (CLI_SECTION_KIND_BUG).
static VARIANT_DESTRUCTOR(CliSection, bug, CliSection *self);

/// @brief Free CliSection type (CLI_SECTION_KIND_DESCRIPTION).
static VARIANT_DESTRUCTOR(CliSection, description, CliSection *self);

/// @brief Free CliSection type (CLI_SECTION_KIND_HELP).
static VARIANT_DESTRUCTOR(CliSection, help, CliSection *self);

/// @brief Free CliSection type (CLI_SECTION_KIND_USAGE).
static inline VARIANT_DESTRUCTOR(CliSection, usage, CliSection *self);

VARIANT_CONSTRUCTOR(CliSection *, CliSection, author, String *author)
{
    CliSection *self = lily_malloc(sizeof(CliSection));

    self->kind = CLI_SECTION_KIND_AUTHOR;
    self->author = author;

    return self;
}

VARIANT_CONSTRUCTOR(CliSection *, CliSection, bug, String *bug)
{
    CliSection *self = lily_malloc(sizeof(CliSection));

    self->kind = CLI_SECTION_KIND_BUG;
    self->bug = bug;

    return self;
}

VARIANT_CONSTRUCTOR(CliSection *, CliSection, description, String *description)
{
    CliSection *self = lily_malloc(sizeof(CliSection));

    self->kind = CLI_SECTION_KIND_DESCRIPTION;
    self->description = description;

    return self;
}

VARIANT_CONSTRUCTOR(CliSection *, CliSection, help, String *help)
{
    CliSection *self = lily_malloc(sizeof(CliSection));

    self->kind = CLI_SECTION_KIND_HELP;
    self->help = help;

    return self;
}

VARIANT_CONSTRUCTOR(CliSection *, CliSection, usage, String *usage)
{
    CliSection *self = lily_malloc(sizeof(CliSection));

    self->kind = CLI_SECTION_KIND_USAGE;
    self->usage = usage;

    return self;
}

VARIANT_DESTRUCTOR(CliSection, author, CliSection *self)
{
    FREE(String, self->author);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CliSection, bug, CliSection *self)
{
    FREE(String, self->bug);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CliSection, description, CliSection *self)
{
    FREE(String, self->description);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CliSection, help, CliSection *self)
{
    FREE(String, self->help);
    lily_free(self);
}

VARIANT_DESTRUCTOR(CliSection, usage, CliSection *self)
{
    lily_free(self);
}

DESTRUCTOR(CliSection, CliSection *self)
{
    switch (self->kind) {
        case CLI_SECTION_KIND_AUTHOR:
            FREE_VARIANT(CliSection, author, self);
            break;
        case CLI_SECTION_KIND_BUG:
            FREE_VARIANT(CliSection, bug, self);
            break;
        case CLI_SECTION_KIND_DESCRIPTION:
            FREE_VARIANT(CliSection, description, self);
            break;
        case CLI_SECTION_KIND_HELP:
            FREE_VARIANT(CliSection, help, self);
            break;
        case CLI_SECTION_KIND_USAGE:
            FREE_VARIANT(CliSection, usage, self);
            break;
        default:
            UNREACHABLE("unknown variant");
    }
}
