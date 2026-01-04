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

#ifndef LILY_BASE_CLI_DEFAULT_ACTION_H
#define LILY_BASE_CLI_DEFAULT_ACTION_H

#include <base/alloc.h>
#include <base/macros.h>
#include <base/string.h>

typedef struct Cli Cli;
typedef struct CliCommand CliCommand;

enum CliDefaultActionKind
{
    CLI_DEFAULT_ACTION_KIND_HELP,
    CLI_DEFAULT_ACTION_KIND_VERSION
};

typedef struct CliDefaultAction
{
    enum CliDefaultActionKind kind;
    union
    {
        String *(*help)(const Cli *, const CliCommand *);
        char *version;
    };
} CliDefaultAction;

/**
 *
 * @brief Construct CliDefaultAction type (CLI_DEFAULT_ACTION_KIND_HELP).
 */
VARIANT_CONSTRUCTOR(CliDefaultAction *,
                    CliDefaultAction,
                    help,
                    String *(*help)(const Cli *, const CliCommand *));

/**
 *
 * @brief Construct CliDefaultAction type (CLI_DEFAULT_ACTION_KIND_VERSION).
 */
VARIANT_CONSTRUCTOR(CliDefaultAction *,
                    CliDefaultAction,
                    version,
                    char *version);

/**
 *
 * @brief Print something based on the action.
 * @param cli const Cli*? (&)
 * @param cmd const CliCommand*? (&)
 * @note Run exit(0)
 */
void
print__CliDefaultAction(const CliDefaultAction *self,
                        const Cli *cli,
                        const CliCommand *cmd);

/**
 *
 * @brief Free CliDefaultAction type.
 */
inline DESTRUCTOR(CliDefaultAction, CliDefaultAction *self)
{
    lily_free(self);
}

#endif // LILY_BASE_CLI_DEFAULT_ACTION_H
