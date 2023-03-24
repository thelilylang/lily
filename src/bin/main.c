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

#include <base/macros.h>
#include <base/new.h>
#include <base/platform.h>

#include <cli/emit.h>
#include <cli/help.h>
#include <cli/parse_command.h>
#include <cli/parse_config.h>

#include <command/compile.h>

#include <stdio.h>

int
main(int argc, char **argv)
{
    if (argc > 1) {
        const char *command = argv[1];

#ifdef MSVC_VERSION
        Vec *options = NEW(Vec); // Vec<char*>*

        // 1. Get the rest of argv
        for (int i = 2; i < argc; i++)
            push__Vec(options, argv[i]);

        // 2. Parse comand
        const ParseCommand parse_command =
          NEW(ParseCommand, command, (const char **)options->buffer, argc - 2);
#else
        char *options[argc - 2];

        // 1. Get the rest of argv
        for (int i = argc; i >= 2; --i)
            options[argc - i - 1] = argv[i];

        // 2. Parse comand
        const ParseCommand parse_command =
          NEW(ParseCommand, command, (const char **)options, argc - 2);
#endif

        const Option option = run__ParseCommand(&parse_command);

        // 3. Parse config
        const Config config = run__ParseConfig(&option);

        // 4. Run command
        switch (config.kind) {
            case CONFIG_KIND_BUILD:
                break;
            case CONFIG_KIND_CC:
                break;
            case CONFIG_KIND_COMPILE:
                run__Compile(&config.compile);
                break;
            case CONFIG_KIND_CPP:
                break;
            case CONFIG_KIND_INIT:
                break;
            case CONFIG_KIND_NEW:
                break;
            case CONFIG_KIND_RUN:
                break;
            case CONFIG_KIND_TEST:
                break;
            case CONFIG_KIND_TO:
                break;
        }

#ifdef MSVC_VERSION
        FREE(Vec, options);
#endif

        FREE(Option, option);
    } else {
        EMIT_ERROR("expected command");
        puts(HELP);

        return 1;
    }

    return 0;
}
