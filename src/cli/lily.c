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

#include <cli/lily.h>
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

    cmd->$help(cmd, "C compiler")->$defer(cmd, &cc_options__CliLily);

    return cmd;
}

CliCommand *
cpp_command__CliLily()
{
    CliCommand *cmd = NEW(CliCommand, "c++");

    cmd->$help(cmd, "C++ compiler")->$defer(cmd, &cpp_options__CliLily);

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

    cmd->$help(cmd, "Init project")->$defer(cmd, &init_options__CliLily);

    return cmd;
}

CliCommand *
new_command__CliLily()
{
    CliCommand *cmd = NEW(CliCommand, "new");

    cmd->$help(cmd, "New project")->$defer(cmd, &new_options__CliLily);

    return cmd;
}

CliCommand *
run_command__CliLily()
{
    CliCommand *cmd = NEW(CliCommand, "run");

    cmd->$help(cmd, "Run file")->$defer(cmd, &run_options__CliLily);

    return cmd;
}

CliCommand *
to_command__CliLily()
{
    CliCommand *cmd = NEW(CliCommand, "to");

    cmd->$help(cmd, "Transpile to...")->$defer(cmd, &to_options__CliLily);

    return cmd;
}

CliCommand *
build_options__CliLily(CliCommand *cmd)
{
    return cmd;
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
    CliOption *dump_scanner = NEW(CliOption, "--dump-scanner");
    CliOption *dump_parser = NEW(CliOption, "--dump-parser");
    CliOption *dump_tc = NEW(CliOption, "--dump-tc");
    CliOption *dump_mir = NEW(CliOption, "--dump-mir");
    CliOption *dump_ir = NEW(CliOption, "--dump-ir");
    CliOption *llvm_ir = NEW(CliOption, "--llvm-ir");
    CliOption *cc_ir = NEW(CliOption, "--cc-ir");
    CliOption *cpp_ir = NEW(CliOption, "--cpp-ir");
    CliOption *js_ir = NEW(CliOption, "--js-ir");
    CliOption *wasm_ir = NEW(CliOption, "--wasm-ir");
    CliOption *target = NEW(CliOption, "--target");

    dump_scanner->$help(dump_scanner, "Dump scanner output");
    dump_parser->$help(dump_parser, "Dump parser output");
    dump_tc->$help(dump_tc, "Dump typecheck output");
    dump_mir->$help(dump_mir, "Dump MIR output");
    dump_ir->$help(dump_ir, "Dump IR output");
    llvm_ir->$help(llvm_ir, "Run LLVM as IR (by default)");
    cc_ir->$help(cc_ir, "Use C as IR");
    cpp_ir->$help(cpp_ir, "Use C++ as IR");
    js_ir->$help(js_ir, "Use JS as IR");
    wasm_ir->$help(wasm_ir, "Use WASM as IR");
    target
      ->$help(target,
              "Specify the target of the compilation (<arch>-<os>-<abi>)")
      ->$value(target, NEW(CliValue, CLI_VALUE_KIND_SINGLE, "TARGET", true));

    return cmd->$option(cmd, dump_scanner)
      ->$option(cmd, dump_parser)
      ->$option(cmd, dump_tc)
      ->$option(cmd, dump_mir)
      ->$option(cmd, dump_ir)
      ->$option(cmd, llvm_ir)
      ->$option(cmd, cc_ir)
      ->$option(cmd, cpp_ir)
      ->$option(cmd, js_ir)
      ->$option(cmd, wasm_ir)
      ->$option(cmd, target);
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
    return cmd;
}

CliCommand *
to_options__CliLily(CliCommand *cmd)
{
    return cmd;
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
      ->$subcommand(&cli, cpp_command__CliLily())
      ->$subcommand(&cli, compile_command__CliLily())
      ->$subcommand(&cli, init_command__CliLily())
      ->$subcommand(&cli, new_command__CliLily())
      ->$subcommand(&cli, run_command__CliLily())
      ->$subcommand(&cli, to_command__CliLily());

    return cli;
}
