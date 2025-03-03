/*
 * MIT License
 *
 * Copyright (c) 2022-2025 ArthurPV
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
    CliCommand *cmd = NEW(CliCommand, "compile");

    cmd->$help(cmd, "Compile a file or a project")
      ->$value(
        cmd,
        NEW(CliValue, CLI_VALUE_KIND_SINGLE, "PROJECT_PATH | FILE_PATH", true))
      ->$defer(cmd, &compile_options__CliCI);

    return cmd;
}

CliCommand *
self_test_command__CliCI()
{
    CliCommand *cmd = NEW(CliCommand, "self-test");

    cmd->$help(cmd, "Self test the compiler")
      ->$value(cmd, NEW(CliValue, CLI_VALUE_KIND_SINGLE, "FILE_PATH", true))
      ->$defer(cmd, &self_test_options__CliCI);

    return cmd;
}

CliCommand *
compile_options__CliCI(CliCommand *cmd)
{
    CIC_OPTIONS(cmd);

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

    cli.$version(&cli, VERSION)
      ->$author(&cli, "ArthurPV")
      ->$about(&cli, "The CI programming language")
      ->$subcommand(&cli, compile_command__CliCI())
      ->$subcommand(&cli, self_test_command__CliCI());

    return cli;
}
