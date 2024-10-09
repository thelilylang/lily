/*
 * MIT License
 *
 * Copyright (c) 2022-2024 ArthurPV
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

Cli
build__CliCI(Vec *args)
{
    Cli cli = NEW(Cli, args, "ci");
    CliOption *mode = NEW(CliOption, "--mode");
    CliOption *file = NEW(CliOption, "--file");
    CliOption *standard = NEW(CliOption, "--std");
    CliOption *include = NEW(CliOption, "--include");
    CliOption *include0 = NEW(CliOption, "--include0");

    mode->$help(mode, "Specify transpilation mode (DEBUG | RELEASE)")
      ->$value(mode, NEW(CliValue, CLI_VALUE_KIND_SINGLE, "MODE", true));

    file->$help(file, "Allow to pass a file instead of a project path")
      ->$short_name(file, "-f");

    standard
      ->$help(standard,
              "Pass the standard to use (values: kr | c89 | c95 | c99 | c11 | "
              "c17 | c2x, default: c99)")
      ->$value(standard, NEW(CliValue, CLI_VALUE_KIND_SINGLE, "STD", true))
      ->$short_name(standard, "-s");

    include
      ->$help(include,
              "Add directory to the end of the list of include search paths")
      ->$value(include, NEW(CliValue, CLI_VALUE_KIND_SINGLE, "DIR", true))
      ->$short_name(include, "-I");

    include0
      ->$help(include0,
              "Add directory to the begin of the list of include search paths")
      ->$value(include0, NEW(CliValue, CLI_VALUE_KIND_SINGLE, "DIR", true));

    cli.$version(&cli, VERSION)
      ->$author(&cli, "ArthurPV")
      ->$about(&cli, "The CI programming language")
      ->$option(&cli, mode)
      ->$option(&cli, file)
      ->$option(&cli, standard)
      ->$option(&cli, include)
      ->$option(&cli, include0)
      ->$single_value(&cli, "PROJECT_PATH | FILE_PATH", true);

    return cli;
}
