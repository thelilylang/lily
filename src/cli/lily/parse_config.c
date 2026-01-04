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

#include <base/assert.h>
#include <base/atoi.h>
#include <base/cli/result.h>
#include <base/macros.h>
#include <base/new.h>

#include <cli/emit.h>
#include <cli/lily/parse_config.h>

#include <stdio.h>
#include <stdlib.h>

// NOTE: The following options, are builtin:
/*
#define H_OPTION 0
#define HELP_OPTION 1
#define V_OPTION 2
#define VERSION_OPTION 3
*/
#define BUILD_COMMAND 0
#define CC_COMMAND 1
#define COMPILE_COMMAND 2
#define CPP_COMMAND 3
#define INIT_COMMAND 4
#define NEW_COMMAND 5
#define RUN_COMMAND 6
#define TEST_COMMAND 7
#define TO_COMMAND 8

// NOTE: The following options, are builtin:
/*
#define BUILD_H_OPTION 0
#define BUILD_HELP_OPTION 1
*/
#define BUILD_VERBOSE_OPTION 2

// NOTE: The following options, are builtin:
/*
#define RUN_H_OPTION 0
#define RUN_HELP_OPTION 1
*/
#define RUN_V_OPTION 2
#define RUN_VERBOSE_OPTION 3
#define RUN_ARGS_OPTION 4
#define RUN_MAX_STACK_OPTION 5
#define RUN_MAX_HEAP_OPTION 6

// NOTE: The following options, are builtin:
/*
#define TO_H_OPTION 0
#define TO_HELP_OPTION 1
*/
#define TO_CC_OPTION 2
#define TO_CPP_OPTION 3
#define TO_JS_OPTION 4

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
            case BUILD_VERBOSE_OPTION:
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
    bool verbose = false;
    char *filename = NULL;
    Vec *args = init__Vec(1, "<app>");
    char *max_stack = NULL, *max_heap = NULL;
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
                    case RUN_V_OPTION:
                    case RUN_VERBOSE_OPTION:
                        verbose = true;
                        break;
                    case RUN_ARGS_OPTION:
                        append__Vec(args, current->option->value->multiple);
                        break;
                    case RUN_MAX_STACK_OPTION:
                        max_stack = current->option->value->single;
                        break;
                    case RUN_MAX_HEAP_OPTION:
                        max_heap = current->option->value->single;
                        break;
                    default:
                        UNREACHABLE("unknown option");
                }

                break;
            default:
                UNREACHABLE("not expected in this context");
        }
    }

    Usize max_stack_capacity = max_stack ? atoi__Usize(max_stack, 10) : 0;
    Usize max_heap_capacity = max_heap ? atoi__Usize(max_heap, 10) : 0;

    // TODO: maybe set a minimum max stack capacity
    if ((max_stack_capacity == 0 && max_stack)) {
        EMIT_ERROR("you cannot set the stack capacity to 0");
    }

    // TODO: maybe set a minimum max heap capacity
    if ((max_heap_capacity == 0 && max_heap)) {
        EMIT_ERROR("you cannot set the heap capacity to 0");
    }

    return NEW_VARIANT(LilyConfig,
                       run,
                       NEW(LilyConfigRun,
                           filename,
                           verbose,
                           args,
                           max_stack_capacity,
                           max_heap_capacity));
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
                    case TO_CC_OPTION:
                        cc = true;
                        break;
                    case TO_CPP_OPTION:
                        cpp = true;
                        break;
                    case TO_JS_OPTION:
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
        case BUILD_COMMAND:
            return parse_build__LilyParseConfig(results);
        case CC_COMMAND:
            return parse_cc__LilyParseConfig(results);
        case COMPILE_COMMAND:
            return NEW_VARIANT(LilyConfig, compile);
        case CPP_COMMAND:
            return parse_cpp__LilyParseConfig(results);
        case INIT_COMMAND:
            return parse_init__LilyParseConfig(results);
        case NEW_COMMAND:
            return parse_new__LilyParseConfig(results);
        case RUN_COMMAND:
            return parse_run__LilyParseConfig(results);
        case TEST_COMMAND:
            return parse_test__LilyParseConfig(results);
        case TO_COMMAND:
            return parse_to__LilyParseConfig(results);
        default:
            UNREACHABLE("unknown command");
    }
}
