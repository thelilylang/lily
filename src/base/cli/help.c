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

#include <base/assert.h>
#include <base/cli/help.h>

#include <stdio.h>
#include <stdlib.h>

#define GENERATE_USAGE(options, value)                                       \
    {                                                                        \
        String *usage = format__String(                                      \
          "Usage: {s}{s}", cli->name, options->len > 0 ? " [options]" : ""); \
                                                                             \
        if (value) {                                                         \
            push_str__String(usage, " [");                                   \
            push_str__String(usage, (char *)value->name);                    \
            push__String(usage, ']');                                        \
                                                                             \
            if (value->kind == CLI_VALUE_KIND_MULTIPLE) {                    \
                push_str__String(usage, "...");                              \
            }                                                                \
        }                                                                    \
                                                                             \
        APPEND_AND_FREE(res, usage);                                         \
        push_str__String(res, "\n\n");                                       \
    }

#define GENERATE_OPTIONS(ops)                                            \
    if (ops->len > 0) {                                                  \
        String *options = from__String("Options:\n\n");                  \
        OrderedHashMapIter iter = NEW(OrderedHashMapIter, ops);          \
        CliOption *current = NULL;                                       \
                                                                         \
        while ((current = next__OrderedHashMapIter(&iter))) {            \
            /* Determine if is a short option (to avoid duplication) */  \
            {                                                            \
                OrderedHashMapPair *pair =                               \
                  get_pair_from_id__OrderedHashMap(ops, iter.count - 1); \
                                                                         \
                ASSERT(pair);                                            \
                                                                         \
                if (strlen(pair->key) == 2) {                            \
                    continue;                                            \
                }                                                        \
            }                                                            \
                                                                         \
            String *option = format__String("  {s}", current->name);     \
                                                                         \
            if (current->short_name) {                                   \
                push_str__String(option, ", ");                          \
                push_str__String(option, current->short_name);           \
            }                                                            \
                                                                         \
            if (current->help) {                                         \
                push_str__String(option, "\t\t");                        \
                push_str__String(option, current->help);                 \
                push__String(option, '\n');                              \
            }                                                            \
                                                                         \
            APPEND_AND_FREE(options, option);                            \
        }                                                                \
                                                                         \
        APPEND_AND_FREE(res, options);                                   \
    }

String *
generate_help__CliHelp(const Cli *cli, const CliCommand *cmd)
{
    ASSERT((cli && !cmd) || (cli && cmd));

    String *res = NEW(String);

    if (cmd) {
        GENERATE_USAGE(cmd->options, cmd->value);
        GENERATE_OPTIONS(cmd->options);
    } else {
        GENERATE_USAGE(cli->options, cli->value);

        if (cli->subcommands->len > 0) {
            String *subcommands = from__String("Commands:\n\n");
            OrderedHashMapIter iter = NEW(OrderedHashMapIter, cli->subcommands);
            CliCommand *current = NULL;

            while ((current = next__OrderedHashMapIter(&iter))) {
                String *subcommand = format__String("  {s}", current->name);

                if (current->help) {
                    push_str__String(subcommand, "\t\t");
                    push_str__String(subcommand, current->help);
                    push__String(subcommand, '\n');
                }

                APPEND_AND_FREE(subcommands, subcommand);
            }

            APPEND_AND_FREE(res, subcommands);
            push_str__String(res, "\n");
        }

        GENERATE_OPTIONS(cli->options);
    }

    return res;
}
