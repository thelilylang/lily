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

#include <base/cli/args.h>
#include <base/cli/entry.h>
#include <base/cli/result.h>

#include <cli/ci/ci.h>
#include <cli/ci/parse_config.h>

#include <command/ci/compile.h>
#include <command/ci/self_test.h>

#include <stdlib.h>
#include <stdio.h>

int
main(int argc, char **argv)
{
    CliArgs args = build__CliArgs(argc, argv);

    RUN__CLI_ENTRY(args, build__CliCI, CIConfig, run__CIParseConfig, {
		switch (config.kind) {
			case CI_CONFIG_KIND_COMPILE:
				run__CICompile(&config);

				break;
			case CI_CONFIG_KIND_SELF_TEST:
				run__CISelfTest(&config);

				break;
			default:
				UNREACHABLE("unknown variant");
		}
	});

	return 0;
}
