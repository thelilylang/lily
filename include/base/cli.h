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

enum CliParseStatus
{
    // The arguments were parsed, the results are usable.
    CLI_PARSE_STATUS_OK,
    // A default action (`--help`, `--version`) has been printed. There is
    // nothing left to run, but this is not a failure.
    CLI_PARSE_STATUS_DONE,
    // A diagnostic has been emitted on stderr.
    CLI_PARSE_STATUS_ERROR
};

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
    enum CliParseStatus status;
} Cli;

/**
 *
 * @brief Construct Cli type.
 */
CONSTRUCTOR(Cli, Cli, const Vec *args, const char *name);

/**
 *
 * @brief Set the author of the program.
 * @return Cli* (&) self, to allow the calls to be nested.
 */
Cli *
author__Cli(Cli *self, char *author);

/**
 *
 * @brief Set the description of the program.
 * @return Cli* (&) self, to allow the calls to be nested.
 */
Cli *
about__Cli(Cli *self, char *about);

/**
 *
 * @brief Set the version of the program. This also adds the `--version`
 * option.
 * @return Cli* (&) self, to allow the calls to be nested.
 */
Cli *
version__Cli(Cli *self, char *version);

/**
 *
 * @brief Add a subcommand to the program.
 * @return Cli* (&) self, to allow the calls to be nested.
 */
Cli *
subcommand__Cli(Cli *self, CliCommand *subcommand);

/**
 *
 * @brief Add an option to the program.
 * @return Cli* (&) self, to allow the calls to be nested.
 */
Cli *
option__Cli(Cli *self, CliOption *option);

/**
 *
 * @brief Expect a single value.
 * @return Cli* (&) self, to allow the calls to be nested.
 */
Cli *
single_value__Cli(Cli *self, char *name, bool is_required);

/**
 *
 * @brief Expect one or more values.
 * @return Cli* (&) self, to allow the calls to be nested.
 */
Cli *
multiple_value__Cli(Cli *self, char *name, bool is_required);

/**
 *
 * @brief Expect one or more values, options included.
 * @return Cli* (&) self, to allow the calls to be nested.
 */
Cli *
multiple_inf_value__Cli(Cli *self, char *name, bool is_required);

/**
 *
 * @brief Parse the arguments given to the constructor.
 * @note This never terminates the process. When NULL is returned,
 * `self->status` tells whether that is an error or a default action that has
 * already been printed.
 * @return Vec<CliResult*>*? - NULL when `self->status != CLI_PARSE_STATUS_OK`.
 */
Vec *
parse__Cli(Cli *self);

/**
 *
 * @brief Get the status code the program is expected to exit with, based on the
 * result of the last parse.
 */
inline int
exit_status__Cli(const Cli *self)
{
    return self->status == CLI_PARSE_STATUS_ERROR ? 1 : 0;
}

/**
 *
 * @brief Free Cli type.
 */
DESTRUCTOR(Cli, const Cli *self);

#endif // LILY_BASE_CLI_H
