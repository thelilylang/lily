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

#include <cli/ci/ci.h>
#include <cli/cic/cic.h>
#include <cli/version.h>

static CliCommand *
compile_command__CliCI();

static CliCommand *
self_test_command__CliCI();

static CliCommand *
compile_options__CliCI(CliCommand *cmd);

static CliCommand *
self_test_options__CliCI(CliCommand *cmd);

CliCommand *
compile_command__CliCI()
{
    CliCommand *cmd = NEW(CliCommand, CI_COMMAND_ID_COMPILE, "compile");

    help__CliCommand(cmd, "Compile a file or a project");
    value__CliCommand(
      cmd,
      NEW(CliValue, CLI_VALUE_KIND_SINGLE, "PROJECT_PATH | FILE_PATH", true));
    defer__CliCommand(cmd, &compile_options__CliCI);

    return cmd;
}

CliCommand *
self_test_command__CliCI()
{
    CliCommand *cmd = NEW(CliCommand, CI_COMMAND_ID_SELF_TEST, "self-test");

    help__CliCommand(cmd, "Self test the compiler");
    value__CliCommand(cmd,
                      NEW(CliValue, CLI_VALUE_KIND_SINGLE, "FILE_PATH", true));
    defer__CliCommand(cmd, &self_test_options__CliCI);

    return cmd;
}

CliCommand *
compile_options__CliCI(CliCommand *cmd)
{
    CIC_OPTIONS(cmd, option__CliCommand);

    return cmd;
}

CliCommand *
self_test_options__CliCI(CliCommand *cmd)
{
    return cmd;
}

Cli
build__CliCI(Vec *args)
{
    Cli cli = NEW(Cli, args, "ci");

    version__Cli(&cli, VERSION);
    author__Cli(&cli, "ArthurPV");
    about__Cli(&cli, "The CI programming language");
    subcommand__Cli(&cli, compile_command__CliCI());
    subcommand__Cli(&cli, self_test_command__CliCI());

    return cli;
}
