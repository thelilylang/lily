/*
 * MIT License
 *
 * Copyright (c) 2022-2025 ArthurPV
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

#include <cli/ci/parse_config.h>
#include <cli/cic/parse_config.h>

#include <stdlib.h>
#include <stdio.h>

// NOTE: The following options, are builtin:
/*
#define H_OPTION 0
#define HELP_OPTION 1
#define V_OPTION 2
#define VERSION_OPTION 3
*/
#define COMPILE_COMMAND 0
#define SELF_TEST_COMMAND 1

/// @brief Parse compile config.
static inline CIConfig 
parse_compile__CIParseConfig(const Vec *results);

/// @brief Parse self-test config.
static CIConfig
parse_self_test__CIParseConfig(const Vec *results);

CIConfig 
parse_compile__CIParseConfig(const Vec *results)
{
	return NEW_VARIANT(CIConfig, compile, run_for_command__CIcParseConfig(results));
}

CIConfig 
parse_self_test__CIParseConfig(const Vec *results)
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
				UNREACHABLE("unknown option");
			default:
                UNREACHABLE("not expected in this context");
		}
	}

	return NEW_VARIANT(CIConfig, self_test, NEW(CIConfigSelfTest, path));
}

CIConfig
run__CIParseConfig(const Vec *results)
{
    CliResult *command = get__Vec(results, 0);

    ASSERT(command->kind == CLI_RESULT_KIND_COMMAND);

    switch (command->command.id) {
		case COMPILE_COMMAND:
			return parse_compile__CIParseConfig(results);
		case SELF_TEST_COMMAND:
			return parse_self_test__CIParseConfig(results);
        default:
            UNREACHABLE("unknown command");
	}
}
