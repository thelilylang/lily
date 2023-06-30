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

#ifndef LILY_BASE_CLI_H
#define LILY_BASE_CLI_H

#include <base/cli/command.h>
#include <base/cli/option.h>
#include <base/ordered_hash_map.h>
#include <base/string.h>

typedef struct Cli
{
    const char *name;
    String *description;      // String*?
    OrderedHashMap *commands; // OrderedHashMap<CliCommand*>*?
    OrderedHashMap *options;  // OrderedHashMap<CliOption*>*?
    bool has_value;
} Cli;

/**
 *
 * @brief Construct Cli type.
 */
inline CONSTRUCTOR(Cli,
                   Cli,
                   const char *name,
                   String *description,
                   bool has_commands,
                   bool has_options)
{
    return (Cli){ .name = name,
                  .description = description,
                  .commands = has_commands ? NEW(OrderedHashMap) : NULL,
                  .options = has_options ? NEW(OrderedHashMap) : NULL,
                  .has_value = false };
}

/**
 *
 * @brief Add command to commands.
 */
void
add_command__Cli(Cli *self, CliCommand *command);

/**
 *
 * @brief Add option to options.
 */
void
add_option__Cli(Cli *self, CliOption *option);

/**
 *
 * @brief Set `has_value` to true.
 */
inline void
set_value__Cli(Cli *self)
{
    self->has_value = true;
}

/**
 *
 * @brief Free Cli type.
 */
DESTRUCTOR(Cli, const Cli *self);

#endif // LILY_BASE_CLI_H
