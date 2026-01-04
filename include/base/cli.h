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

#ifndef LILY_BASE_CLI_H
#define LILY_BASE_CLI_H

#include <base/cli/command.h>
#include <base/cli/option.h>
#include <base/cli/value.h>
#include <base/ordered_hash_map.h>
#include <base/string.h>
#include <base/vec.h>

typedef struct Cli
{
    const char *name;
    OrderedHashMap *subcommands; // OrderedHashMap<CliCommand*>*
    OrderedHashMap *options;     // OrderedHashMap<CliOption*>*
    char *author;                // char*?
    CliValue *value;             // CliValue*?
    char *about;                 // char*?
    char *version;               // char*?
    String *full_command;
    const Vec *args; // const Vec<char*>* (&)
    VecIter args_iter;

    struct Cli *(*$author)(struct Cli *, char *);
    struct Cli *(*$about)(struct Cli *, char *);
    struct Cli *(*$version)(struct Cli *, char *);
    struct Cli *(*$subcommand)(struct Cli *, CliCommand *);
    struct Cli *(*$option)(struct Cli *, CliOption *);
    struct Cli *(*$single_value)(struct Cli *, char *, bool);
    struct Cli *(*$multiple_value)(struct Cli *, char *, bool);
    struct Cli *(*$multiple_inf_value)(struct Cli *, char *, bool);
    struct Vec *(*$parse)(struct Cli *);
} Cli;

/**
 *
 * @brief Construct Cli type.
 */
CONSTRUCTOR(Cli, Cli, const Vec *args, const char *name);

/**
 *
 * @brief Free Cli type.
 */
DESTRUCTOR(Cli, const Cli *self);

#endif // LILY_BASE_CLI_H
