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

#ifndef LILY_CLI_CIC_H
#define LILY_CLI_CIC_H

#include <base/cli.h>

// Identifiers of the `cic` options. Because `CIC_OPTIONS` is shared between the
// `cic` program (which owns `--help` and `--version`) and the `ci compile`
// command (which only owns `--help`), these must not depend on how many options
// the library adds by itself - hence the explicit identifiers.
enum CIcOptionId
{
    CIC_OPTION_ID_MODE,
    CIC_OPTION_ID_FILE,
    CIC_OPTION_ID_STD,
    CIC_OPTION_ID_INCLUDE,
    CIC_OPTION_ID_INCLUDE0,
    CIC_OPTION_ID_NO_STATE_CHECK,
    CIC_OPTION_ID_METHOD_CONVENTION,
    CIC_OPTION_ID_TYPE_CONVENTION
};

#define CIC_OPTIONS(self, add_option)                                          \
    CliOption *mode = NEW(CliOption, CIC_OPTION_ID_MODE, "--mode");            \
    CliOption *file = NEW(CliOption, CIC_OPTION_ID_FILE, "--file");            \
    CliOption *standard = NEW(CliOption, CIC_OPTION_ID_STD, "--std");          \
    CliOption *include = NEW(CliOption, CIC_OPTION_ID_INCLUDE, "--include");   \
    CliOption *include0 =                                                      \
      NEW(CliOption, CIC_OPTION_ID_INCLUDE0, "--include0");                    \
    CliOption *no_state_check =                                                \
      NEW(CliOption, CIC_OPTION_ID_NO_STATE_CHECK, "--no-state-check");        \
    CliOption *method_convention =                                             \
      NEW(CliOption, CIC_OPTION_ID_METHOD_CONVENTION, "--method-convention");  \
    CliOption *type_convention =                                               \
      NEW(CliOption, CIC_OPTION_ID_TYPE_CONVENTION, "--type-convention");      \
                                                                               \
    value__CliOption(                                                          \
      help__CliOption(mode, "Specify transpilation mode (DEBUG | RELEASE)"),   \
      NEW(CliValue, CLI_VALUE_KIND_SINGLE, "MODE", true));                     \
    short_name__CliOption(                                                     \
      help__CliOption(file, "Allow to pass a file instead of a project path"), \
      "-f");                                                                   \
    help__CliOption(                                                           \
      standard,                                                                \
      "Pass the standard to use (values: kr | c89 | c95 | c99 | c11 | "        \
      "c17 | c23, default: c99)");                                             \
    value__CliOption(standard,                                                 \
                     NEW(CliValue, CLI_VALUE_KIND_SINGLE, "STD", true));       \
    short_name__CliOption(standard, "-s");                                     \
    help__CliOption(include,                                                   \
                    "Add directory to the end of the list of include search "  \
                    "paths");                                                  \
    value__CliOption(include,                                                  \
                     NEW(CliValue, CLI_VALUE_KIND_SINGLE, "DIR", true));       \
    short_name__CliOption(include, "-I");                                      \
    help__CliOption(include0,                                                  \
                    "Add directory to the begin of the list of include "       \
                    "search paths");                                           \
    value__CliOption(include0,                                                 \
                     NEW(CliValue, CLI_VALUE_KIND_SINGLE, "DIR", true));       \
    help__CliOption(no_state_check, "Disable the state checker");              \
    help__CliOption(method_convention,                                         \
                    "Name convention a function follows to be callable as a "  \
                    "method (e.g. `$type_$name`)");                            \
    value__CliOption(                                                          \
      method_convention,                                                       \
      NEW(CliValue, CLI_VALUE_KIND_SINGLE, "CONVENTION", true));               \
    help__CliOption(type_convention,                                           \
                    "Name convention a type follows to be given methods "      \
                    "(e.g. `$name_t`, default: any name)");                    \
    value__CliOption(                                                          \
      type_convention,                                                         \
      NEW(CliValue, CLI_VALUE_KIND_SINGLE, "CONVENTION", true));               \
                                                                               \
    add_option(self, mode);                                                    \
    add_option(self, file);                                                    \
    add_option(self, standard);                                                \
    add_option(self, include);                                                 \
    add_option(self, include0);                                                \
    add_option(self, no_state_check);                                          \
    add_option(self, method_convention);                                       \
    add_option(self, type_convention);

Cli
build__CliCIc(Vec *args);

#endif // LILY_CLI_CIC_H
