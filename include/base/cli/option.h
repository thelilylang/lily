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

typedef struct CliOption
{
    const char *name;                 // NOTE: this is the long name (--<name>)
    char *short_name;                 // char*? (-<letter>)
    CliValue *value;                  // CliValue*?
    char *help;                       // char*?
    CliDefaultAction *default_action; // CliDefaultAction*?
    Usize ref_count;

    struct CliOption *(*$short_name)(struct CliOption *, char *);
    struct CliOption *(*$value)(struct CliOption *, CliValue *value);
    struct CliOption *(*$help)(struct CliOption *, char *);
    struct CliOption *(*$default_action)(struct CliOption *,
                                         CliDefaultAction *);
} CliOption;

/**
 *
 * @brief Construct CliOption type.
 */
CONSTRUCTOR(CliOption *, CliOption, const char *name);

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
