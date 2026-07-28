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
    CliCommand *cmd = NEW(CliCommand, LILY_COMMAND_ID_BUILD, "build");

    help__CliCommand(cmd, "Build project");
    defer__CliCommand(cmd, &build_options__CliLily);

    return cmd;
}

CliCommand *
cc_command__CliLily()
{
    CliCommand *cmd = NEW(CliCommand, LILY_COMMAND_ID_CC, "cc");

    help__CliCommand(cmd, "C compiler");
    value__CliCommand(cmd, NEW(CliValue, CLI_VALUE_KIND_SINGLE, "FILE", true));
    defer__CliCommand(cmd, &cc_options__CliLily);

    return cmd;
}

CliCommand *
cpp_command__CliLily()
{
    CliCommand *cmd = NEW(CliCommand, LILY_COMMAND_ID_CPP, "c++");

    help__CliCommand(cmd, "C++ compiler");
    value__CliCommand(cmd, NEW(CliValue, CLI_VALUE_KIND_SINGLE, "FILE", true));
    defer__CliCommand(cmd, &cpp_options__CliLily);

    return cmd;
}

CliCommand *
compile_command__CliLily()
{
    CliCommand *cmd = NEW(CliCommand, LILY_COMMAND_ID_COMPILE, "compile");

    help__CliCommand(cmd, "Compile file");
    value__CliCommand(cmd, NEW(CliValue, CLI_VALUE_KIND_SINGLE, "FILE", true));
    defer__CliCommand(cmd, &compile_options__CliLily);

    return cmd;
}

CliCommand *
init_command__CliLily()
{
    CliCommand *cmd = NEW(CliCommand, LILY_COMMAND_ID_INIT, "init");

    help__CliCommand(cmd, "Init project");
    value__CliCommand(cmd, NEW(CliValue, CLI_VALUE_KIND_SINGLE, "PATH", true));
    defer__CliCommand(cmd, &init_options__CliLily);

    return cmd;
}

CliCommand *
new_command__CliLily()
{
    CliCommand *cmd = NEW(CliCommand, LILY_COMMAND_ID_NEW, "new");

    help__CliCommand(cmd, "New project");
    value__CliCommand(cmd, NEW(CliValue, CLI_VALUE_KIND_SINGLE, "NAME", true));
    defer__CliCommand(cmd, &new_options__CliLily);

    return cmd;
}

CliCommand *
run_command__CliLily()
{
    CliCommand *cmd = NEW(CliCommand, LILY_COMMAND_ID_RUN, "run");

    help__CliCommand(cmd, "Run file (interpreter)");
    value__CliCommand(cmd, NEW(CliValue, CLI_VALUE_KIND_SINGLE, "FILE", true));
    defer__CliCommand(cmd, &run_options__CliLily);

    return cmd;
}

CliCommand *
test_command__CliLily()
{
    CliCommand *cmd = NEW(CliCommand, LILY_COMMAND_ID_TEST, "test");

    help__CliCommand(cmd, "Test a file");
    value__CliCommand(cmd, NEW(CliValue, CLI_VALUE_KIND_SINGLE, "FILE", true));
    defer__CliCommand(cmd, &test_options__CliLily);

    return cmd;
}

CliCommand *
to_command__CliLily()
{
    CliCommand *cmd = NEW(CliCommand, LILY_COMMAND_ID_TO, "to");

    help__CliCommand(cmd, "Transpile to...");
    value__CliCommand(cmd, NEW(CliValue, CLI_VALUE_KIND_SINGLE, "FILE", true));
    defer__CliCommand(cmd, &to_options__CliLily);

    return cmd;
}

CliCommand *
build_options__CliLily(CliCommand *cmd)
{
    CliOption *verbose =
      NEW(CliOption, LILY_BUILD_OPTION_ID_VERBOSE, "--verbose");

    short_name__CliOption(verbose, "-v");

    return option__CliCommand(cmd, verbose);
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
    LILYC_OPTIONS(cmd, option__CliCommand);

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
    CliOption *verbose =
      NEW(CliOption, LILY_RUN_OPTION_ID_VERBOSE, "--verbose");
    CliOption *args = NEW(CliOption, LILY_RUN_OPTION_ID_ARGS, "---");
    CliOption *max_stack =
      NEW(CliOption, LILY_RUN_OPTION_ID_MAX_STACK, "--max-stack");
    CliOption *max_heap =
      NEW(CliOption, LILY_RUN_OPTION_ID_MAX_HEAP, "--max-heap");

    short_name__CliOption(verbose, "-v");
    help__CliOption(verbose, "Enable log step of the interpreter");
    value__CliOption(args,
                     NEW(CliValue, CLI_VALUE_KIND_MULTIPLE_INF, "ARGS", false));
    help__CliOption(args, "Pass argument(s) to the program");
    value__CliOption(max_stack,
                     NEW(CliValue, CLI_VALUE_KIND_SINGLE, "CAPACITY", false));
    help__CliOption(max_stack, "Set a max stack capacity in BYTES");
    value__CliOption(max_heap,
                     NEW(CliValue, CLI_VALUE_KIND_SINGLE, "CAPACITY", false));
    help__CliOption(max_heap, "Set a max heap capacity in BYTES");

    option__CliCommand(cmd, verbose);
    option__CliCommand(cmd, args);
    option__CliCommand(cmd, max_stack);
    option__CliCommand(cmd, max_heap);

    return cmd;
}

CliCommand *
test_options__CliLily(CliCommand *cmd)
{
    return cmd;
}

CliCommand *
to_options__CliLily(CliCommand *cmd)
{
    CliOption *cc = NEW(CliOption, LILY_TO_OPTION_ID_CC, "--cc");
    CliOption *cpp = NEW(CliOption, LILY_TO_OPTION_ID_CPP, "--cpp");
    CliOption *js = NEW(CliOption, LILY_TO_OPTION_ID_JS, "--js");

    help__CliOption(cc, "Transpile to C");
    help__CliOption(cpp, "Transpile to C++");
    help__CliOption(js, "Transpile to JS");

    option__CliCommand(cmd, cc);
    option__CliCommand(cmd, cpp);
    option__CliCommand(cmd, js);

    return cmd;
}

Cli
build__CliLily(Vec *args)
{
    Cli cli = NEW(Cli, args, "lily");

    version__Cli(&cli, VERSION);
    author__Cli(&cli, "ArthurPV");
    about__Cli(&cli, "The Lily programming language");

    subcommand__Cli(&cli, build_command__CliLily());
    subcommand__Cli(&cli, cc_command__CliLily());
    subcommand__Cli(&cli, compile_command__CliLily());
    subcommand__Cli(&cli, cpp_command__CliLily());
    subcommand__Cli(&cli, init_command__CliLily());
    subcommand__Cli(&cli, new_command__CliLily());
    subcommand__Cli(&cli, run_command__CliLily());
    subcommand__Cli(&cli, test_command__CliLily());
    subcommand__Cli(&cli, to_command__CliLily());

    return cli;
}
