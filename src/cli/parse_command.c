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

#include <base/format.h>
#include <base/new.h>

#include <cli/emit.h>
#include <cli/help.h>
#include <cli/parse_command.h>
#include <cli/version.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void
help();
static void
version();
static void
error(const ParseCommand *self);

CONSTRUCTOR(ParseCommand,
            ParseCommand,
            const char *command,
            const char **options,
            const Usize options_size)
{
    enum Command command_kind;

    if (!(strcmp(command, "build")))
        command_kind = COMMAND_BUILD;
    else if (!strcmp(command, "cc"))
        command_kind = COMMAND_CC;
    else if (!strcmp(command, "compile"))
        command_kind = COMMAND_COMPILE;
    else if (!strcmp(command, "cpp"))
        command_kind = COMMAND_CPP;
    else if (!strcmp(command, "help") || !strcmp(command, "--help") ||
             !strcmp(command, "-h"))
        command_kind = COMMAND_HELP;
    else if (!strcmp(command, "init"))
        command_kind = COMMAND_INIT;
    else if (!strcmp(command, "new"))
        command_kind = COMMAND_NEW;
    else if (!strcmp(command, "run"))
        command_kind = COMMAND_RUN;
    else if (!strcmp(command, "to"))
        command_kind = COMMAND_TO;
    else if (!strcmp(command, "version") || !strcmp(command, "--version") ||
             !strcmp(command, "-v"))
        command_kind = COMMAND_VERSION;
    else
        command_kind = COMMAND_ERROR;

    return (ParseCommand){
        .command = command,
        .options = options,
        .options_size = options_size,
        .command_kind = command_kind,
    };
}

#define PARSE_COMMAND(option_type, parse, l_name, u_name)                      \
    static Option parse_##l_name##__ParseCommand(const ParseCommand *self)     \
    {                                                                          \
        if (self->options_size == 0) {                                         \
            puts(u_name##_HELP);                                               \
            exit(0);                                                           \
        } else {                                                               \
            Vec *options = parse(self->options, self->options_size);           \
            for (Usize i = 0; i < options->len; i++) {                         \
                option_type op = get__Vec(options, i);                         \
                switch (op->kind) {                                            \
                    case u_name##_OPTION_KIND_ERROR: {                         \
                        char *msg = format("unknown option `{s}`", op->error); \
                        EMIT_ERROR(msg);                                       \
                        free(msg);                                             \
                        break;                                                 \
                    }                                                          \
                    case u_name##_OPTION_KIND_HELP:                            \
                        puts(u_name##_HELP);                                   \
                        exit(0);                                               \
                        break;                                                 \
                    default:                                                   \
                        break;                                                 \
                }                                                              \
            }                                                                  \
            return NEW_VARIANT(Option, l_name, options);                       \
        }                                                                      \
    }

PARSE_COMMAND(BuildOption *, parse__BuildOption, build, BUILD);
PARSE_COMMAND(CcOption *, parse__CcOption, cc, CC);
PARSE_COMMAND(CompileOption *, parse__CompileOption, compile, COMPILE);
PARSE_COMMAND(CppOption *, parse__CppOption, cpp, CPP);
PARSE_COMMAND(InitOption *, parse__InitOption, init, INIT);
PARSE_COMMAND(NewOption *, parse__NewOption, new, NEW);
PARSE_COMMAND(RunOption *, parse__RunOption, run, RUN);
PARSE_COMMAND(TestOption *, parse__TestOption, test, TEST);
PARSE_COMMAND(ToOption *, parse__ToOption, to, TO);

void
help()
{
    puts(HELP);
    exit(0);
}

void
version()
{
    printf("lily v%s", VERSION);
    exit(0);
}

void
error(const ParseCommand *self)
{
    char *msg = format("unknown command `{s}`", self->command);

    EMIT_ERROR(msg);

    free(msg);
    exit(1);
}

Option
run__ParseCommand(const ParseCommand *self)
{
    switch (self->command_kind) {
        case COMMAND_BUILD:
            return parse_build__ParseCommand(self);
        case COMMAND_CC:
            return parse_cc__ParseCommand(self);
        case COMMAND_COMPILE:
            return parse_compile__ParseCommand(self);
        case COMMAND_CPP:
            return parse_cpp__ParseCommand(self);
        case COMMAND_ERROR:
            error(self);
        case COMMAND_HELP:
            help();
        case COMMAND_INIT:
            return parse_init__ParseCommand(self);
        case COMMAND_NEW:
            return parse_new__ParseCommand(self);
        case COMMAND_RUN:
            return parse_run__ParseCommand(self);
        case COMMAND_TEST:
            return parse_test__ParseCommand(self);
        case COMMAND_TO:
            return parse_to__ParseCommand(self);
        case COMMAND_VERSION:
            version();
        default:
            UNREACHABLE("unknown variant");
    }
}
