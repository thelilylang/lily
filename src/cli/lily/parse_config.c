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

#include <base/assert.h>
#include <base/cli/result.h>
#include <base/macros.h>
#include <base/new.h>

#include <cli/emit.h>
#include <cli/lily/parse_config.h>

#include <stdio.h>
#include <stdlib.h>

// Parse build config.
static LilyConfig
parse_build__LilyParseConfig(const Vec *results);

// Parse cc config.
static LilyConfig
parse_cc__LilyParseConfig(const Vec *results);

// Parse cpp config.
static LilyConfig
parse_cpp__LilyParseConfig(const Vec *results);

// Parse init config.
static LilyConfig
parse_init__LilyParseConfig(const Vec *results);

// Parse new config.
static LilyConfig
parse_new__LilyParseConfig(const Vec *results);

// Parse run config.
static LilyConfig
parse_run__LilyParseConfig(const Vec *results);

// Parse test config.
static LilyConfig
parse_test__LilyParseConfig(const Vec *results);

// Parse to config.
static LilyConfig
parse_to__LilyParseConfig(const Vec *results);

LilyConfig
parse_build__LilyParseConfig(const Vec *results)
{
    bool verbose = false;
    VecIter iter = NEW(VecIter, results);
    CliResult *current = NULL;

    // Skip command
    next__VecIter(&iter);

    while ((current = next__VecIter(&iter))) {
        ASSERT(current->kind == CLI_RESULT_KIND_OPTION);

        switch (current->option->id) {
            case 2:
                verbose = true;
                break;
            default:
                UNREACHABLE("unknown option");
        }
    }

    return NEW_VARIANT(LilyConfig, build, NEW(LilyConfigBuild, verbose));
}

LilyConfig
parse_cc__LilyParseConfig(const Vec *results)
{
    char *filename = NULL;
    VecIter iter = NEW(VecIter, results);
    CliResult *current = NULL;

    while ((current = next__VecIter(&iter))) {
        switch (current->kind) {
            case CLI_RESULT_KIND_COMMAND:
                ASSERT(current->command.value);
                ASSERT(current->command.value->kind ==
                       CLI_RESULT_VALUE_KIND_SINGLE);

                filename = current->command.value->single;

                break;
            case CLI_RESULT_KIND_OPTION:
                switch (current->option->id) {
                    default:
                        UNREACHABLE("unknown option");
                }

                break;
            default:
                UNREACHABLE("not expected in this context");
        }
    }

    return NEW_VARIANT(LilyConfig, cc, NEW(LilyConfigCc, filename));
}

LilyConfig
parse_cpp__LilyParseConfig(const Vec *results)
{
    char *filename = NULL;
    VecIter iter = NEW(VecIter, results);
    CliResult *current = NULL;

    while ((current = next__VecIter(&iter))) {
        switch (current->kind) {
            case CLI_RESULT_KIND_COMMAND:
                ASSERT(current->command.value);
                ASSERT(current->command.value->kind ==
                       CLI_RESULT_VALUE_KIND_SINGLE);

                filename = current->command.value->single;

                break;
            case CLI_RESULT_KIND_OPTION:
                switch (current->option->id) {
                    default:
                        UNREACHABLE("unknown option");
                }

                break;
            default:
                UNREACHABLE("not expected in this context");
        }
    }

    return NEW_VARIANT(LilyConfig, cpp, NEW(LilyConfigCpp, filename));
}

LilyConfig
parse_init__LilyParseConfig(const Vec *results)
{
    char *path = NULL;
    VecIter iter = NEW(VecIter, results);
    CliResult *current = NULL;

    while ((current = next__VecIter(&iter))) {
        switch (current->kind) {
            case CLI_RESULT_KIND_COMMAND:
                ASSERT(current->command.value);
                ASSERT(current->command.value->kind ==
                       CLI_RESULT_VALUE_KIND_SINGLE);

                path = current->command.value->single;

                break;
            case CLI_RESULT_KIND_OPTION:
                switch (current->option->id) {
                    default:
                        UNREACHABLE("unknown option");
                }

                break;
            default:
                UNREACHABLE("not expected in this context");
        }
    }

    return NEW_VARIANT(LilyConfig, init, NEW(LilyConfigInit, path));
}

LilyConfig
parse_new__LilyParseConfig(const Vec *results)
{
    char *name = NULL;
    VecIter iter = NEW(VecIter, results);
    CliResult *current = NULL;

    while ((current = next__VecIter(&iter))) {
        switch (current->kind) {
            case CLI_RESULT_KIND_COMMAND:
                ASSERT(current->command.value);
                ASSERT(current->command.value->kind ==
                       CLI_RESULT_VALUE_KIND_SINGLE);

                name = current->command.value->single;

                break;
            case CLI_RESULT_KIND_OPTION:
                switch (current->option->id) {
                    default:
                        UNREACHABLE("unknown option");
                }

                break;
            default:
                UNREACHABLE("not expected in this context");
        }
    }

    return NEW_VARIANT(LilyConfig, new, NEW(LilyConfigNew, name));
}

LilyConfig
parse_run__LilyParseConfig(const Vec *results)
{
    char *filename = NULL;
    VecIter iter = NEW(VecIter, results);
    CliResult *current = NULL;

    while ((current = next__VecIter(&iter))) {
        switch (current->kind) {
            case CLI_RESULT_KIND_COMMAND:
                ASSERT(current->command.value);
                ASSERT(current->command.value->kind ==
                       CLI_RESULT_VALUE_KIND_SINGLE);

                filename = current->command.value->single;

                break;
            case CLI_RESULT_KIND_OPTION:
                switch (current->option->id) {
                    default:
                        UNREACHABLE("unknown option");
                }

                break;
            default:
                UNREACHABLE("not expected in this context");
        }
    }

    return NEW_VARIANT(LilyConfig, run, NEW(LilyConfigRun, filename));
}

LilyConfig
parse_test__LilyParseConfig(const Vec *results)
{
    char *filename = NULL;
    VecIter iter = NEW(VecIter, results);
    CliResult *current = NULL;

    while ((current = next__VecIter(&iter))) {
        switch (current->kind) {
            case CLI_RESULT_KIND_COMMAND:
                ASSERT(current->command.value);
                ASSERT(current->command.value->kind ==
                       CLI_RESULT_VALUE_KIND_SINGLE);

                filename = current->command.value->single;

                break;
            case CLI_RESULT_KIND_OPTION:
                switch (current->option->id) {
                    default:
                        UNREACHABLE("unknown option");
                }

                break;
            default:
                UNREACHABLE("not expected in this context");
        }
    }

    return NEW_VARIANT(LilyConfig, test, NEW(LilyConfigTest, filename));
}

LilyConfig
parse_to__LilyParseConfig(const Vec *results)
{
    char *filename = NULL;
    bool cc = false, cpp = false, js = false;
    VecIter iter = NEW(VecIter, results);
    CliResult *current = NULL;

    while ((current = next__VecIter(&iter))) {
        switch (current->kind) {
            case CLI_RESULT_KIND_COMMAND:
                ASSERT(current->command.value);
                ASSERT(current->command.value->kind ==
                       CLI_RESULT_VALUE_KIND_SINGLE);

                filename = current->command.value->single;

                break;
            case CLI_RESULT_KIND_OPTION:
                switch (current->option->id) {
                    // 0 and 1 id are used by help option
                    case 2:
                        cc = true;
                        break;
                    case 3:
                        cpp = true;
                        break;
                    case 4:
                        js = true;
                        break;
                    default:
                        UNREACHABLE("unknown option");
                }

                break;
            default:
                UNREACHABLE("not expected in this context");
        }
    }

    return NEW_VARIANT(
      LilyConfig, to, NEW(LilyConfigTo, filename, cc, cpp, js));
}

LilyConfig
run__LilyParseConfig(const Vec *results)
{
    CliResult *command = get__Vec(results, 0);

    ASSERT(command->kind == CLI_RESULT_KIND_COMMAND);

    switch (command->command.id) {
        case 0:
            return parse_build__LilyParseConfig(results);
        case 1:
            return parse_cc__LilyParseConfig(results);
        case 2:
            return NEW_VARIANT(LilyConfig, compile);
        case 3:
            return parse_cpp__LilyParseConfig(results);
        case 4:
            return parse_init__LilyParseConfig(results);
        case 5:
            return parse_new__LilyParseConfig(results);
        case 6:
            return parse_run__LilyParseConfig(results);
        case 7:
            return parse_test__LilyParseConfig(results);
        case 8:
            return parse_to__LilyParseConfig(results);
        default:
            UNREACHABLE("unknown command");
    }
}
