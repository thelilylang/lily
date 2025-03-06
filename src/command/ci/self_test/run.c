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
#include <base/color.h>
#include <base/command.h>
#include <base/fork.h>
#include <base/new.h>
#include <base/print.h>
#include <base/string.h>

#include <bin/bin.h>

#include <cli/cic/cic.h>
#include <cli/cic/config.h>
#include <cli/cic/parse_config.h>

#include <command/ci/self_test/metadata/scanner.h>
#include <command/ci/self_test/run.h>
#include <command/cic/cic.h>

/// @brief Handler of pass_through_result__CIResult`` function.
/// @param other_args  void* (&) (const CISelfTestMetadata* (&))
static void
handler_result__CISelfTestRun(void *entity,
                              const CIResultFile *file,
                              void *other_args);

/// @brief Handler of `run__CIc` function.
/// @param other_args void* (&) (const CISelfTestMetadata* (&))
static void
handler__CISelfTestRun(const CIResult *result, void *other_args);

/// @brief Run cic command with a custom handler.
/// @param path const String* (&)
static void
run_cic__CISelfTestRun(const String *path, const CISelfTestMetadata *metadata);

void
handler_result__CISelfTestRun(void *entity,
                              const CIResultFile *file,
                              void *other_args)
{
    ASSERT(file->entity.kind == CI_RESULT_ENTITY_KIND_BIN);

    const CIResultBin *bin = entity;
    const CISelfTestMetadata *metadata = other_args;
    String *bin_dir_result =
      get_dir_result__CIResultFile(file, CI_DIR_RESULT_PURPOSE_BIN);
    String *command = format__String("{Sr}/{s}", bin_dir_result, bin->name);
    String *command_output = save__Command(command->buffer);

    if (metadata->expected_stdout && strcmp(metadata->expected_stdout->buffer,
                                            command_output->buffer) != 0) {
        PRINTLN("{sa}", RED("ERR"));

        return;
    }

    FREE(String, command);
    FREE(String, command_output);

    PRINTLN("{sa}", GREEN("OK"));
}

void
handler__CISelfTestRun(const CIResult *result, void *other_args)
{
    const CISelfTestMetadata *metadata = other_args;

    pass_through_result__CIResult(
      result, &handler_result__CISelfTestRun, (void *)metadata);
}

void
run_cic__CISelfTestRun(const String *path, const CISelfTestMetadata *metadata)
{
    Vec *args_string =
      init__Vec(6,
                from__String(CIC_BIN_PATH),
                from__String("--no-state-check"), // NOTE: Disable the state
                                                  // check as is not yet ready
                from__String("-I"),
                from__String("tests/core/cc/ci/include"),
                from__String("-f"),
                clone__String((String *)path)); // Vec<String*>*

    if (metadata->compile_options) {
        Vec *compile_options_string =
          split__String(metadata->compile_options, ' '); // Vec<String*>*

        append__Vec(args_string, compile_options_string);
        FREE(Vec, compile_options_string);
    }

    CliArgs args = build_from_string_args__CliArgs(args_string);

    RUN__CLI_ENTRY(args, build__CliCIc, CIcConfig, run__CIcParseConfig, {
        run__CIc(&config, &handler__CISelfTestRun, (void *)metadata);
    });

    FREE_BUFFER_ITEMS(args_string->buffer, args_string->len, String);
    FREE(Vec, args_string);
}

void
run__CISelfTestRun(const String *path)
{
    const Fork pid = run__Fork();

    switch (pid) {
        case 0: {
            CISelfTestMetadata metadata = NEW(CISelfTestMetadata);

            run__CISelfTestMetadataScanner(path, &metadata);
            run_cic__CISelfTestRun(path, &metadata);

            FREE(CISelfTestMetadata, &metadata);

            exit(EXIT_OK);
        }
        case -1:
            UNREACHABLE("failed to fork process");
        default:
            break;
    }
}
