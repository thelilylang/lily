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

#include <cli/lily/lily.h>
#include <cli/lilyc/lilyc.h>
#include <cli/version.h>

static CliCommand *
build_command__CliLily();

static CliCommand *
cc_command__CliLily();

static CliCommand *
cpp_command__CliLily();

static CliCommand *
compile_command__CliLily();

static CliCommand *
init_command__CliLily();

static CliCommand *
new_command__CliLily();

static CliCommand *
run_command__CliLily();

static CliCommand *
test_command__CliLily();

static CliCommand *
to_command__CliLily();

static CliCommand *
build_options__CliLily(CliCommand *cmd);

static CliCommand *
cc_options__CliLily(CliCommand *cmd);

static CliCommand *
cpp_options__CliLily(CliCommand *cmd);

static CliCommand *
compile_options__CliLily(CliCommand *cmd);

static CliCommand *
init_options__CliLily(CliCommand *cmd);

static CliCommand *
new_options__CliLily(CliCommand *cmd);

static CliCommand *
run_options__CliLily(CliCommand *cmd);

static CliCommand *
test_options__CliLily(CliCommand *cmd);

static CliCommand *
to_options__CliLily(CliCommand *cmd);

CliCommand *
build_command__CliLily()
{
    CliCommand *cmd = NEW(CliCommand, "build");

    cmd->$help(cmd, "Build project")->$defer(cmd, &build_options__CliLily);

    return cmd;
}

CliCommand *
cc_command__CliLily()
{
    CliCommand *cmd = NEW(CliCommand, "cc");

    cmd->$help(cmd, "C compiler")
      ->$value(cmd, NEW(CliValue, CLI_VALUE_KIND_SINGLE, "FILE", true))
      ->$defer(cmd, &cc_options__CliLily);

    return cmd;
}

CliCommand *
cpp_command__CliLily()
{
    CliCommand *cmd = NEW(CliCommand, "c++");

    cmd->$help(cmd, "C++ compiler")
      ->$value(cmd, NEW(CliValue, CLI_VALUE_KIND_SINGLE, "FILE", true))
      ->$defer(cmd, &cpp_options__CliLily);

    return cmd;
}

CliCommand *
compile_command__CliLily()
{
    CliCommand *cmd = NEW(CliCommand, "compile");

    cmd->$help(cmd, "Compile file")
      ->$value(cmd, NEW(CliValue, CLI_VALUE_KIND_SINGLE, "FILE", true))
      ->$defer(cmd, &compile_options__CliLily);

    return cmd;
}

CliCommand *
init_command__CliLily()
{
    CliCommand *cmd = NEW(CliCommand, "init");

    cmd->$help(cmd, "Init project")
      ->$value(cmd, NEW(CliValue, CLI_VALUE_KIND_SINGLE, "PATH", true))
      ->$defer(cmd, &init_options__CliLily);

    return cmd;
}

CliCommand *
new_command__CliLily()
{
    CliCommand *cmd = NEW(CliCommand, "new");

    cmd->$help(cmd, "New project")
      ->$value(cmd, NEW(CliValue, CLI_VALUE_KIND_SINGLE, "NAME", true))
      ->$defer(cmd, &new_options__CliLily);

    return cmd;
}

CliCommand *
run_command__CliLily()
{
    CliCommand *cmd = NEW(CliCommand, "run");

    cmd->$help(cmd, "Run file (interpreter)")
      ->$value(cmd, NEW(CliValue, CLI_VALUE_KIND_SINGLE, "FILE", true))
      ->$defer(cmd, &run_options__CliLily);

    return cmd;
}

CliCommand *
test_command__CliLily()
{
    CliCommand *cmd = NEW(CliCommand, "test");

    cmd->$help(cmd, "Test a file")
      ->$value(cmd, NEW(CliValue, CLI_VALUE_KIND_SINGLE, "FILE", true))
      ->$defer(cmd, &test_options__CliLily);

    return cmd;
}

CliCommand *
to_command__CliLily()
{
    CliCommand *cmd = NEW(CliCommand, "to");

    cmd->$help(cmd, "Transpile to...")
      ->$value(cmd, NEW(CliValue, CLI_VALUE_KIND_SINGLE, "FILE", true))
      ->$defer(cmd, &to_options__CliLily);

    return cmd;
}

CliCommand *
build_options__CliLily(CliCommand *cmd)
{
    CliOption *verbose = NEW(CliOption, "--verbose");

    verbose->$short_name(verbose, "-v");

    return cmd->$option(cmd, verbose);
}

CliCommand *
cc_options__CliLily(CliCommand *cmd)
{
    return cmd;
}

CliCommand *
cpp_options__CliLily(CliCommand *cmd)
{
    return cmd;
}

CliCommand *
compile_options__CliLily(CliCommand *cmd)
{
    LILYC_OPTIONS(cmd);

    return cmd;
}

CliCommand *
init_options__CliLily(CliCommand *cmd)
{
    return cmd;
}

CliCommand *
new_options__CliLily(CliCommand *cmd)
{
    return cmd;
}

CliCommand *
run_options__CliLily(CliCommand *cmd)
{
    CliOption *verbose = NEW(CliOption, "--verbose");
    CliOption *args = NEW(CliOption, "---");
    CliOption *max_stack = NEW(CliOption, "--max-stack");
    CliOption *max_heap = NEW(CliOption, "--max-heap");

    verbose->$short_name(verbose, "-v")
      ->$help(verbose, "Enable log step of the interpreter");
    args
      ->$value(args, NEW(CliValue, CLI_VALUE_KIND_MULTIPLE_INF, "ARGS", false))
      ->$help(args, "Pass argument(s) to the program");
    max_stack
      ->$value(max_stack,
               NEW(CliValue, CLI_VALUE_KIND_SINGLE, "CAPACITY", false))
      ->$help(max_stack, "Set a max stack capacity in BYTES");
    max_heap
      ->$value(max_heap,
               NEW(CliValue, CLI_VALUE_KIND_SINGLE, "CAPACITY", false))
      ->$help(max_heap, "Set a max heap capacity in BYTES");

    return cmd->$option(cmd, verbose)
      ->$option(cmd, args)
      ->$option(cmd, max_stack)
      ->$option(cmd, max_heap);
}

CliCommand *
test_options__CliLily(CliCommand *cmd)
{
    return cmd;
}

CliCommand *
to_options__CliLily(CliCommand *cmd)
{
    CliOption *cc = NEW(CliOption, "--cc");
    CliOption *cpp = NEW(CliOption, "--cpp");
    CliOption *js = NEW(CliOption, "--js");

    cc->$help(cc, "Transpile to C");
    cpp->$help(cpp, "Transpile to C++");
    js->$help(js, "Transpile to JS");

    return cmd->$option(cmd, cc)->$option(cmd, cpp)->$option(cmd, js);
}

Cli
build__CliLily(Vec *args)
{
    Cli cli = NEW(Cli, args, "lily");

    cli.$version(&cli, VERSION)
      ->$author(&cli, "ArthurPV")
      ->$about(&cli, "The Lily programming language")
      ->$subcommand(&cli, build_command__CliLily())
      ->$subcommand(&cli, cc_command__CliLily())
      ->$subcommand(&cli, compile_command__CliLily())
      ->$subcommand(&cli, cpp_command__CliLily())
      ->$subcommand(&cli, init_command__CliLily())
      ->$subcommand(&cli, new_command__CliLily())
      ->$subcommand(&cli, run_command__CliLily())
      ->$subcommand(&cli, test_command__CliLily())
      ->$subcommand(&cli, to_command__CliLily());

    return cli;
}
