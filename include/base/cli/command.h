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

#ifndef LILY_BASE_CLI_COMMAND_H
#define LILY_BASE_CLI_COMMAND_H

#include <base/cli/option.h>
#include <base/ordered_hash_map.h>

typedef struct CliCommand
{
    String *usage;
    const char *name;
    OrderedHashMap *options; // OrderedHashMap<CliOption*>*
    bool has_value;
} CliCommand;

/**
 *
 * @brief Construct CliCommand type.
 */
CONSTRUCTOR(CliCommand *,
            CliCommand,
            const char *cli_name,
            const char *name,
            bool has_value);

/**
 *
 * @brief Add option to command.
 */
inline void
add_option__CliCommand(CliCommand *self, CliOption *option)
{
    insert__OrderedHashMap(self->options, (char *)option->name, option);
}

/**
 *
 * @brief Free CliCommand type.
 */
DESTRUCTOR(CliCommand, CliCommand *self);

#endif // LILY_BASE_CLI_COMMAND_H
