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

#ifndef LILY_CLI_HELP_H
#define LILY_CLI_HELP_H

#define BUILD_HELP                               \
    "Usage: lily build [options] [FILENAME]\n\n" \
    "Options:\n\n"                               \
    "  -h, --help      Print build's help"

#define CC_HELP                               \
    "Usage: lily cc [options] [FILENAME]\n\n" \
    "Options:\n\n"                            \
    "  -h, --help      Print cc's help"

#define COMPILE_HELP                               \
    "Usage: lily compile [options] [FILENAME]\n\n" \
    "Options:\n\n"                                 \
    "  --dump-scanner  Dump scanner output\n"      \
    "  --dump-parser   Dump parser output\n"       \
    "  --dump-tc       Dump typecheck output\n"    \
    "  --run-scanner   Run only the scanner\n"     \
    "  --run-parser    Run only the parser\n"      \
    "  --run-tc        Run only the typecheck\n"   \
    "  -h, --help      Print compile's help\n"

#define CPP_HELP                               \
    "Usage: lily c++ [options] [FILENAME]\n\n" \
    "Options:\n\n"                             \
    "  -h, --help      Print c++'s help"

#define INIT_HELP                           \
    "Usage: lily init [options] [PATH]\n\n" \
    "Options:\n\n"                          \
    "  -h, --help      Print init's help"

#define NEW_HELP                           \
    "Usage: lily new [options] [NAME]\n\n" \
    "Options:\n\n"                         \
    "  -h, --help      Print new's help"

#define RUN_HELP                               \
    "Usage: lily run [options] [FILENAME]\n\n" \
    "Options:\n\n"                             \
    "  -h, --help      Print run's help"

#define TEST_HELP                               \
    "Usage: lily test [options] [FILENAME]\n\n" \
    "Options:\n\n"                              \
    "  -h, --help      Print test's help"

#define TO_HELP                               \
    "Usage: lily to [options] [FILENAME]\n\n" \
    "Options:\n\n"                            \
    "  -h, --help      Print to's help"

#define HELP                                       \
    "Usage: lily [command] [options]\n\n"          \
    "Commands:\n\n"                                \
    "  build           Build project\n"            \
    "  cc              C compiler\n"               \
    "  c++             C++ compiler\n"             \
    "  compile         Compile file\n"             \
    "  help            Print help command usage\n" \
    "  init            Init project\n"             \
    "  new             New project\n"              \
    "  run             Run file\n"                 \
    "  to              Transpile to...\n"          \
    "  version         Print version\n\n"          \
    "General Options:\n\n"                         \
    "  -h, --help      Print help command usage\n" \
    "  -v, --version   Print version"

#endif // LILY_CLI_HELP_H
