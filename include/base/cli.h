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
#include <base/cli/section.h>
#include <base/cli/value.h>
#include <base/ordered_hash_map.h>
#include <base/sized_array.h>
#include <base/string.h>
#include <base/vec.h>

typedef struct Cli
{
    String *full_command;
    String *usage;
    const char *name;
    OrderedHashMap *commands; // OrderedHashMap<CliCommand*>*?
    OrderedHashMap *options;  // OrderedHashMap<CliOption*>*?
    Vec *sections;            // Vec<CliSection*>*?
    enum CliValueKind has_value;
    bool enable_suggestions;
} Cli;

/**
 *
 * @brief Construct Cli type.
 */
CONSTRUCTOR(Cli,
            Cli,
            const SizedArray *args,
            const char *name,
            bool has_commands,
            bool has_options);

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
 * @brief Set `has_value` to `CLI_VALUE_KIND_SINGLE` or
 * `CLI_VALUE_KIND_MULTIPLE`.
 */
inline void
set_value__Cli(Cli *self, enum CliValueKind has_value)
{
    self->has_value = has_value;
}

/**
 *
 * @brief Free Cli type.
 */
DESTRUCTOR(Cli, const Cli *self);

#endif // LILY_BASE_CLI_H
