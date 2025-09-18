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
#include <base/dup.h>
#include <base/fork.h>
#include <base/new.h>
#include <base/print.h>
#include <base/std_fileno.h>
#include <base/string.h>

#include <bin/bin.h>

#include <cli/cic/cic.h>
#include <cli/cic/config.h>
#include <cli/cic/parse_config.h>

#include <command/ci/self_test/diagnostic.h>
#include <command/ci/self_test/metadata/scanner.h>
#include <command/ci/self_test/run.h>
#include <command/cic/cic.h>

#include <time.h>

struct CIHandlerOtherArgs
{
    const CISelfTestMetadata *metadata; // const CISelfTestMetadata* (&)
    clock_t start;
};

/// @brief Construct CIHandlerOtherArgs type.
static inline CONSTRUCTOR(struct CIHandlerOtherArgs,
                          CIHandlerOtherArgs,
                          const CISelfTestMetadata *metadata,
                          clock_t start);

/// @brief Handler of pass_through_result__CIResult`` function.
/// @param other_args  void* (&) (const struct CIHandlerOtherArgs* (&))
static void
handler_result__CISelfTestRun(void *entity,
                              const CIResultFile *file,
                              void *other_args);

/// @brief Handler of `run__CIc` function.
/// @param other_args void* (&) (const struct CIHandlerOtherArgs* (&))
static void
handler__CISelfTestRun(const CIResult *result, void *other_args);

/// @brief Run cic command with a custom handler.
/// @param path const String* (&)
static void
run_cic__CISelfTestRun(const String *path, const CISelfTestMetadata *metadata);

CONSTRUCTOR(struct CIHandlerOtherArgs,
            CIHandlerOtherArgs,
            const CISelfTestMetadata *metadata,
            clock_t start)
{
    return (struct CIHandlerOtherArgs){ .metadata = metadata, .start = start };
}

void
handler_result__CISelfTestRun(void *entity,
                              const CIResultFile *file,
                              void *other_args)
{
    ASSERT(file->entity.kind == CI_RESULT_ENTITY_KIND_BIN);

    const CIResultBin *bin = entity;
    const struct CIHandlerOtherArgs *other_args_casted = other_args;
    const CISelfTestMetadata *metadata = other_args_casted->metadata;
    const clock_t start = other_args_casted->start;
    String *bin_dir_result =
      get_dir_result__CIResultFile(file, CI_DIR_RESULT_PURPOSE_BIN);
    String *command = format__String("{Sr}/{s}", bin_dir_result, bin->name);
    String *command_output = NULL;

    if (!exists__File(command->buffer)) {
        display_failed_binary_not_exist__CISelfTestDiagnostic(
          command, file->file_input.name);

        goto exit;
    }

    command_output = save__Command(command->buffer, NULL);

    if (metadata->expected_stdout && strcmp(metadata->expected_stdout->buffer,
                                            command_output->buffer) != 0) {
        display_failed_expected_stdout_assertion_output__CISelfTestDiagnostic(
          metadata->expected_stdout, command_output, file->file_input.name);
    } else {
        display_pass_test_output__CISelfTestDiagnostic(
          file->file_input.name, (double)(clock() - start) / CLOCKS_PER_SEC);
    }

exit:
    FREE(String, command);

    if (command_output) {
        FREE(String, command_output);
    }
}

void
handler__CISelfTestRun(const CIResult *result, void *other_args)
{
    pass_through_result__CIResult(
      result, &handler_result__CISelfTestRun, other_args);
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
    struct CIHandlerOtherArgs other_args =
      NEW(CIHandlerOtherArgs, metadata, clock());

    RUN__CLI_ENTRY(args, build__CliCIc, CIcConfig, run__CIcParseConfig, {
        run__CIc(&config, &handler__CISelfTestRun, &other_args);
    });

    FREE_BUFFER_ITEMS(args_string->buffer, args_string->len, String);
    FREE(Vec, args_string);
}

CISelfTestProcessUnit *
run__CISelfTestRun(String *path)
{
    Pipefd pipefd;

    create__Pipe(
      pipefd); // Two pipes? One for redirect stderr and stdout for debug at
               // least. And one for diagnostic, sounds a good plan.

    const Fork pid = run__Fork();

    switch (pid) {
        case 0: {
            // Redirect stdout and stderr to the pipe.
            run2__Dup(pipefd[PIPE_WRITE_FD], LILY_STDOUT_FILENO);
            run2__Dup(pipefd[PIPE_WRITE_FD], LILY_STDERR_FILENO);
            close__Pipe(pipefd);

            CISelfTestMetadata metadata = NEW(CISelfTestMetadata);

            run__CISelfTestMetadataScanner(path, &metadata);
            run_cic__CISelfTestRun(path, &metadata);

            FREE(CISelfTestMetadata, &metadata);

            fflush(stdout);

            exit(EXIT_OK);
        }
        case -1:
            UNREACHABLE("failed to fork process");
        default:
            close_write__Pipe(pipefd);

            return NEW(CISelfTestProcessUnit, pid, path, pipefd[PIPE_READ_FD]);
    }
}
