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
#include <base/cli/result.h>

#include <cli/cic/parse_config.h>
#include <cli/emit.h>

#include <core/cc/ci/features.h>
#include <core/shared/search.h>

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
#define MODE_OPTION 0           // (4 or 2)
#define F_OPTION 1              // (5 or 3)
#define FILE_OPTION 2           // (6 or 4)
#define S_OPTION 3              // (7 or 5)
#define STD_OPTION 4            // (8 or 6)
#define I_OPTION 5              // (9 or 7)
#define INCLUDE_OPTION 6        // (10 or 8)
#define INCLUDE0_OPTION 7       // (11 or 9)
#define NO_STATE_CHECK_OPTION 8 // (12 or 10)

/// @brief Get offset, given the purpose.
static int
get_offset__CIcParseConfigPurpose(enum CIcParseConfigPurpose self);

/// @brief Parse cic configuration (base).
static CIcConfig
run_base__CIcParseConfig(const Vec *results,
                         enum CIcParseConfigPurpose purpose);

int
get_offset__CIcParseConfigPurpose(enum CIcParseConfigPurpose self)
{
    switch (self) {
        case CIC_PARSE_CONFIG_PURPOSE_CLI:
            // H_OPTION + HELP_OPTION + V_OPTION + VERSION_OPTION = 4
            return 4;
        case CIC_PARSE_CONFIG_PURPOSE_CMD:
            // H_OPTION + HELP_OPTION = 2
            return 2;
        default:
            UNREACHABLE("unknown variant");
    }
}

CIcConfig
run_base__CIcParseConfig(const Vec *results, enum CIcParseConfigPurpose purpose)
{
    int option_offset = get_offset__CIcParseConfigPurpose(purpose);
    enum CIcConfigMode mode = CIC_CONFIG_MODE_NONE;
    bool file = false;
    char *path = NULL;
    enum CIStandard standard = CI_STANDARD_NONE;
    Vec *includes = NEW(Vec);  // Vec<char* (&)>*
    Vec *includes0 = NEW(Vec); // Vec<char* (&)>*
    bool no_state_check = false;

    VecIter iter = NEW(VecIter, results);
    CliResult *current = NULL;

    while ((current = next__VecIter(&iter))) {
        switch (current->kind) {
            case CLI_RESULT_KIND_VALUE:
                if (purpose == CIC_PARSE_CONFIG_PURPOSE_CLI) {
                    ASSERT(current->value);
                    ASSERT(current->value->kind ==
                           CLI_RESULT_VALUE_KIND_SINGLE);

                    path = current->value->single;

                    break;
                }

                UNREACHABLE("not expected in this context");
            case CLI_RESULT_KIND_OPTION:
                ASSERT(current->option->id >= option_offset);

                switch (current->option->id - option_offset) {
                    case MODE_OPTION:
                        ASSERT(current->option->value);
                        ASSERT(current->option->value->kind ==
                               CLI_RESULT_VALUE_KIND_SINGLE);

                        if (!strcmp(current->option->value->single, "DEBUG")) {
                            mode = CIC_CONFIG_MODE_DEBUG;
                        } else if (!strcmp(current->option->value->single,
                                           "RELEASE")) {
                            mode = CIC_CONFIG_MODE_RELEASE;
                        } else {
                            EMIT_ERROR(
                              "expected DEBUG or RELEASE as value of `--mode`");
                            exit(1);
                        }

                        break;
                    case F_OPTION:
                    case FILE_OPTION:
                        file = true;

                        break;
                    case S_OPTION:
                    case STD_OPTION: {
                        ASSERT(current->option->value);
                        ASSERT(current->option->value->kind ==
                               CLI_RESULT_VALUE_KIND_SINGLE);

                        String *option_s =
                          from__String(current->option->value->single);
                        Int32 res =
                          get_id__Search(option_s,
                                         get_standards__CIStandard(),
                                         get_standard_ids__CIStandard(),
                                         CI_N_STANDARD);

                        if (res == -1) {
                            FAILED("this value is not expected to be a valid "
                                   "standard value");
                        } else {
                            standard = res;
                        }

                        FREE(String, option_s);

                        break;
                    }
                    case I_OPTION:
                    case INCLUDE_OPTION:
                        ASSERT(current->option->value);
                        ASSERT(current->option->value->kind ==
                               CLI_RESULT_VALUE_KIND_SINGLE);

                        push__Vec(includes, current->option->value->single);

                        break;
                    case INCLUDE0_OPTION:
                        ASSERT(current->option->value);
                        ASSERT(current->option->value->kind ==
                               CLI_RESULT_VALUE_KIND_SINGLE);

                        push__Vec(includes0, current->option->value->single);

                        break;
                    case NO_STATE_CHECK_OPTION:
                        no_state_check = true;

                        break;
                    default:
                        UNREACHABLE("unknown option");
                }

                break;
            case CLI_RESULT_KIND_COMMAND:
                if (purpose == CIC_PARSE_CONFIG_PURPOSE_CMD) {
                    ASSERT(current->command.value);
                    ASSERT(current->command.value->kind ==
                           CLI_RESULT_VALUE_KIND_SINGLE);

                    path = current->command.value->single;

                    break;
                }

                UNREACHABLE("not expected in this context");
            default:
                UNREACHABLE("unknown variant");
        }
    }

    if (standard == CI_STANDARD_NONE) {
        standard = CI_STANDARD_99;
    }

    return NEW(CIcConfig,
               path,
               mode,
               file,
               standard,
               includes,
               includes0,
               no_state_check);
}

CIcConfig
run_for_command__CIcParseConfig(const Vec *results)
{
    return run_base__CIcParseConfig(results, CIC_PARSE_CONFIG_PURPOSE_CMD);
}

CIcConfig
run__CIcParseConfig(const Vec *results)
{
    return run_base__CIcParseConfig(results, CIC_PARSE_CONFIG_PURPOSE_CLI);
}
