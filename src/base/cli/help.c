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

#include <base/assert.h>
#include <base/cli/help.h>
#include <base/format.h>

#include <stdio.h>
#include <stdlib.h>

#define MAX_SPACE 30

#define GENERATE_USAGE(options, cmd, cmd_value)                                  \
    {                                                                            \
        String *usage =                                                          \
          format__String("Usage: {s}{sa}{s}",                                    \
                         cli->name,                                              \
                         cmd                         ? format(" {s}", cmd->name) \
                         : cli->subcommands->len > 0 ? format(" [command]")      \
                                                     : format(""),               \
                         options->len > 0 ? " [options]" : "");                  \
                                                                                 \
        if (cmd_value) {                                                         \
            push__String(usage, ' ');                                            \
                                                                                 \
            {                                                                    \
                String *value = format__CliValue(cmd_value);                     \
                                                                                 \
                APPEND_AND_FREE(usage, value);                                   \
            }                                                                    \
        }                                                                        \
                                                                                 \
        APPEND_AND_FREE(res, usage);                                             \
        push_str__String(res, "\n\n");                                           \
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
            String *value = NULL;                                        \
                                                                         \
            if (current->value) {                                        \
                value = format__CliValue(current->value);                \
            } else {                                                     \
                value = from__String("");                                \
            }                                                            \
                                                                         \
            String *option = format__String(                             \
              "  {s}{s}{s}{s}{S}{Sr}",                                   \
              current->name,                                             \
              current->short_name ? ", " : "",                           \
              current->short_name ? current->short_name : "",            \
              current->value ? " " : "",                                 \
              value,                                                     \
              repeat__String(" ",                                        \
                             MAX_SPACE - strlen(current->name) -         \
                               (value->len == 0 ? 0 : value->len + 1) -  \
                               (current->short_name                      \
                                  ? strlen(current->short_name) + 2      \
                                  : 0)));                                \
                                                                         \
            FREE(String, value);                                         \
                                                                         \
            if (current->help) {                                         \
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
        GENERATE_USAGE(cmd->options, cmd, cmd->value);
        GENERATE_OPTIONS(cmd->options);
    } else {
        GENERATE_USAGE(cli->options, cmd, cli->value);

        if (cli->subcommands->len > 0) {
            String *subcommands = from__String("Commands:\n\n");
            OrderedHashMapIter iter = NEW(OrderedHashMapIter, cli->subcommands);
            CliCommand *current = NULL;

            while ((current = next__OrderedHashMapIter(&iter))) {
                String *subcommand = format__String(
                  "  {s}{Sr}",
                  current->name,
                  repeat__String(" ", MAX_SPACE - strlen(current->name)));

                if (current->help) {
                    push_str__String(subcommand, current->help);
                }

                push__String(subcommand, '\n');
                APPEND_AND_FREE(subcommands, subcommand);
            }

            APPEND_AND_FREE(res, subcommands);
            push_str__String(res, "\n");
        }

        GENERATE_OPTIONS(cli->options);
    }

    return res;
}
