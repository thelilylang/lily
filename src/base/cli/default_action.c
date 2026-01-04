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

#include <base/cli.h>
#include <base/cli/default_action.h>
#include <base/print.h>

#include <stdio.h>

VARIANT_CONSTRUCTOR(CliDefaultAction *,
                    CliDefaultAction,
                    help,
                    String *(*help)(const Cli *, const CliCommand *))
{
    CliDefaultAction *self = lily_malloc(sizeof(CliDefaultAction));

    self->kind = CLI_DEFAULT_ACTION_KIND_HELP;
    self->help = help;

    return self;
}

VARIANT_CONSTRUCTOR(CliDefaultAction *,
                    CliDefaultAction,
                    version,
                    char *version)
{
    CliDefaultAction *self = lily_malloc(sizeof(CliDefaultAction));

    self->kind = CLI_DEFAULT_ACTION_KIND_VERSION;
    self->version = version;

    return self;
}

void
print__CliDefaultAction(const CliDefaultAction *self,
                        const Cli *cli,
                        const CliCommand *cmd)
{
    switch (self->kind) {
        case CLI_DEFAULT_ACTION_KIND_HELP:
            if (cmd) {
                PRINTLN("{Sr}", self->help(cli, cmd));
            } else {
                PRINTLN("{Sr}", self->help(cli, NULL));
            }

            break;
        case CLI_DEFAULT_ACTION_KIND_VERSION:
            PRINTLN("{s} v{s}", cli->name, self->version);
            break;
        default:
            UNREACHABLE("unknown variant");
    }

    exit(0);
}
