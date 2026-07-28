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

#ifndef LILY_CLI_LILY_H
#define LILY_CLI_LILY_H

#include <base/cli.h>

// Identifiers of the `lily` commands and of the options of each command. They
// are attached at construction and returned as-is in the parse results. Option
// identifiers only have to be unique within their own command.
enum LilyCommandId
{
    LILY_COMMAND_ID_BUILD,
    LILY_COMMAND_ID_CC,
    LILY_COMMAND_ID_CPP,
    LILY_COMMAND_ID_COMPILE,
    LILY_COMMAND_ID_INIT,
    LILY_COMMAND_ID_NEW,
    LILY_COMMAND_ID_RUN,
    LILY_COMMAND_ID_TEST,
    LILY_COMMAND_ID_TO
};

enum LilyBuildOptionId
{
    LILY_BUILD_OPTION_ID_VERBOSE
};

enum LilyRunOptionId
{
    LILY_RUN_OPTION_ID_VERBOSE,
    LILY_RUN_OPTION_ID_ARGS,
    LILY_RUN_OPTION_ID_MAX_STACK,
    LILY_RUN_OPTION_ID_MAX_HEAP
};

enum LilyToOptionId
{
    LILY_TO_OPTION_ID_CC,
    LILY_TO_OPTION_ID_CPP,
    LILY_TO_OPTION_ID_JS
};

Cli
build__CliLily(Vec *args);

#endif // LILY_CLI_LILY_H
