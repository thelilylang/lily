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

#ifndef LILY_BASE_CLI_OPTION_H
#define LILY_BASE_CLI_OPTION_H

#include <base/cli/default_action.h>
#include <base/cli/value.h>
#include <base/macros.h>
#include <base/string.h>
#include <base/types.h>

// Identifiers of the options added by the library itself. User identifiers are
// expected to start at 0, so reserving the top of the range keeps both sets
// disjoint without the user having to know how many builtins there are.
#define CLI_OPTION_ID_BUILTIN_BASE ((Usize) - 16)
#define CLI_OPTION_ID_HELP (CLI_OPTION_ID_BUILTIN_BASE + 0)
#define CLI_OPTION_ID_VERSION (CLI_OPTION_ID_BUILTIN_BASE + 1)

typedef struct CliOption
{
    // Caller-defined identifier, forwarded as-is to `CliResultOption`. It is
    // never derived from the declaration order, so options can be reordered or
    // given a short name without changing how results are matched.
    Usize id;
    const char *name;                 // NOTE: this is the long name (--<name>)
    char *short_name;                 // char*? (-<letter>)
    CliValue *value;                  // CliValue*?
    char *help;                       // char*?
    CliDefaultAction *default_action; // CliDefaultAction*?
    Usize ref_count;
} CliOption;

/**
 *
 * @brief Construct CliOption type.
 */
CONSTRUCTOR(CliOption *, CliOption, Usize id, const char *name);

/**
 *
 * @brief Set the short name (-<letter>) of the option.
 * @return CliOption* (&) self, to allow the calls to be nested.
 */
CliOption *
short_name__CliOption(CliOption *self, char *name);

/**
 *
 * @brief Set the value expected by the option.
 * @return CliOption* (&) self, to allow the calls to be nested.
 */
CliOption *
value__CliOption(CliOption *self, CliValue *value);

/**
 *
 * @brief Set the help description of the option.
 * @return CliOption* (&) self, to allow the calls to be nested.
 */
CliOption *
help__CliOption(CliOption *self, char *help);

/**
 *
 * @brief Set the action run when the option is passed.
 * @return CliOption* (&) self, to allow the calls to be nested.
 */
CliOption *
default_action__CliOption(CliOption *self, CliDefaultAction *default_action);

/**
 *
 * @brief Pass to ref the CliOption and increment the `ref_count`.
 * @return CliOption*
 */
inline CliOption *
ref__CliOption(CliOption *self)
{
    ++self->ref_count;
    return self;
}

/**
 *
 * @brief Free CliOption type.
 */
DESTRUCTOR(CliOption, CliOption *self);

#endif // LILY_BASE_CLI_OPTION_H
