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

#include <base/assert.h>
#include <base/cli/result.h>

#include <cli/ci/parse_config.h>
#include <cli/emit.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// NOTE: The following options, are builtin:
/*
#define H_OPTION 0
#define HELP_OPTION 1
#define V_OPTION 2
#define VERSION_OPTION 3
*/
#define MODE_OPTION 4
#define FILE_OPTION 5

CIConfig
run__CIParseConfig(const Vec *results)
{
    enum CIConfigMode mode = CI_CONFIG_MODE_NONE;
    bool file = false;
    char *path = NULL;

    VecIter iter = NEW(VecIter, results);
    CliResult *current = NULL;

    while ((current = next__VecIter(&iter))) {
        switch (current->kind) {
            case CLI_RESULT_KIND_VALUE:
                ASSERT(current->value);
                ASSERT(current->value->kind == CLI_RESULT_VALUE_KIND_SINGLE);

                path = current->value->single;

                break;
            case CLI_RESULT_KIND_OPTION:
                switch (current->option->id) {
                    case MODE_OPTION:
                        ASSERT(current->option->value);
                        ASSERT(current->option->value->kind ==
                               CLI_RESULT_VALUE_KIND_SINGLE);

                        if (!strcmp(current->option->value->single, "DEBUG")) {
                            mode = CI_CONFIG_MODE_DEBUG;
                        } else if (!strcmp(current->option->value->single,
                                           "RELEASE")) {
                            mode = CI_CONFIG_MODE_RELEASE;
                        } else {
                            EMIT_ERROR(
                              "expected DEBUG or RELEASE as value of `--mode`");
                            exit(1);
                        }

                        break;
                    case FILE_OPTION:
                        file = true;

                        break;
                    default:
                        UNREACHABLE("unknown option");
                }

                break;
            default:
                UNREACHABLE("not expected in this context");
        }
    }

    return NEW(CIConfig, path, mode, file);
}
