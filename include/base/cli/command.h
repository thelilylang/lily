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

#ifndef LILY_BASE_CLI_COMMAND_H
#define LILY_BASE_CLI_COMMAND_H

#include <base/cli/option.h>
#include <base/cli/value.h>
#include <base/ordered_hash_map.h>

typedef struct CliCommand
{
    const char *name;
    OrderedHashMap *options;          // OrderedHashMap<CliOption*>*
    char *help;                       // char*?
    CliValue *value;                  // CliValue*?
    CliDefaultAction *default_action; // CliDefaultAction*?
    struct CliCommand *(*deferred)(
      struct CliCommand *); // struct CliCommand*(struct CliCommand*)*?

    struct CliCommand *(*$option)(struct CliCommand *, CliOption *option);
    struct CliCommand *(*$help)(struct CliCommand *, char *);
    struct CliCommand *(*$value)(struct CliCommand *, CliValue *);
    struct CliCommand *(*$default_action)(struct CliCommand *,
                                          CliDefaultAction *);
    struct CliCommand *(*$defer)(struct CliCommand *,
                                 struct CliCommand *(struct CliCommand *));
} CliCommand;

/**
 *
 * @brief Construct CliCommand type.
 */
CONSTRUCTOR(CliCommand *, CliCommand, const char *name);

/**
 *
 * @brief Free CliCommand type.
 */
DESTRUCTOR(CliCommand, CliCommand *self);

#endif // LILY_BASE_CLI_COMMAND_H
