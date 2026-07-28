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

#ifndef LILY_CLI_ENTRY_H
#define LILY_CLI_ENTRY_H

#include <base/cli.h>
#include <base/cli/args.h>
#include <base/cli/result.h>

/**
 *
 * @brief Build the CLI, parse `args` and run `block` on the resulting config.
 * @param status An `int` lvalue. It is set to the status the program is
 * expected to exit with when the parse does not produce a config (an error was
 * emitted, or `--help`/`--version` was printed). It is left untouched when
 * `block` runs, so `block` is free to set it itself.
 */
#define RUN__CLI_ENTRY(args, build_cli, config_t, run_config, status, block) \
    {                                                                        \
        Cli cli = build_cli(args);                                           \
        Vec *res = parse__Cli(&cli);                                         \
                                                                             \
        if (res) {                                                           \
            config_t config = run_config(res);                               \
                                                                             \
            FREE_BUFFER_ITEMS(res->buffer, res->len, CliResult);             \
            FREE(Vec, res);                                                  \
            FREE(Cli, &cli);                                                 \
                                                                             \
            block;                                                           \
                                                                             \
            FREE(CliArgs, args);                                             \
            FREE(config_t, &config);                                         \
        } else {                                                             \
            status = exit_status__Cli(&cli);                                 \
                                                                             \
            FREE(Cli, &cli);                                                 \
            FREE(CliArgs, args);                                             \
        }                                                                    \
    }

#endif // LILY_CLI_ENTRY_H
