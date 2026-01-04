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

#include <base/cli/args.h>
#include <base/cli/entry.h>
#include <base/cli/result.h>
#include <base/macros.h>
#include <base/new.h>
#include <base/platform.h>

#include <cli/lily/lily.h>
#include <cli/lily/parse_config.h>

#include <command/lily/compile.h>
#include <command/lily/run.h>

#include <llvm-c/Core.h>

#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char **argv)
{
    CliArgs args = build__CliArgs(argc, argv);

    RUN__CLI_ENTRY(args, build__CliLily, LilyConfig, run__LilyParseConfig, {
        switch (config.kind) {
            case LILY_CONFIG_KIND_BUILD:
                break;
            case LILY_CONFIG_KIND_CC:
                break;
            case LILY_CONFIG_KIND_COMPILE:
                run__LilyCompile(args);

                break;
            case LILY_CONFIG_KIND_CPP:
                break;
            case LILY_CONFIG_KIND_INIT:
                break;
            case LILY_CONFIG_KIND_NEW:
                break;
            case LILY_CONFIG_KIND_RUN:
                run__LilyRun(&config);

                break;
            case LILY_CONFIG_KIND_TEST:
                break;
            case LILY_CONFIG_KIND_TO:
                break;
            default:
                UNREACHABLE("unknown variant");
        }
    });

    LLVMShutdown();

    return 0;
}
