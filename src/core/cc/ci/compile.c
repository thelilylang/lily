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

#include <base/dir.h>
#include <base/file.h>
#include <base/platform.h>

#include <core/cc/ci/compile.h>
#include <core/cc/ci/result.h>

#ifdef LILY_UNIX_OS
#include <sys/wait.h>
#endif

static String *
build_bin_compile_command__CICompile(const CIResultBin *bin,
                                     const CIResultFile *file);

static String *
build_lib_compile_command__CICompile(const CIResultLib *lib,
                                     const CIResultFile *file);

/// @brief Run a compile command, and read what the compiler exited with.
/// @return true when the compiler was run and accepted what it was given.
static bool
run_command__CICompile(const char *command);

/// @param file const CIResultFile* (&)
/// @param other_args void* (&) (bool* (&)) Set when a compile failed, so that
/// the caller of `pass_through_result__CIResult` is told about a compiler the
/// handler itself cannot report to.
static void
handler__CICompile(void *entity, const CIResultFile *file, void *other_args);

static const char *standard_options[] = {
    [CI_STANDARD_NONE] = "",        [CI_STANDARD_KR] = "",
    [CI_STANDARD_89] = " -std=c89", [CI_STANDARD_95] = " -std=c90",
    [CI_STANDARD_99] = " -std=c99", [CI_STANDARD_11] = " -std=c11",
    [CI_STANDARD_17] = " -std=c17", [CI_STANDARD_23] = " -std=c23"
};

String *
build_bin_compile_command__CICompile(const CIResultBin *bin,
                                     const CIResultFile *file)
{
    String *bin_dir_result =
      get_dir_result__CIResultFile(file, CI_DIR_RESULT_PURPOSE_BIN);

    create_recursive_dir__Dir(bin_dir_result->buffer,
                              DIR_MODE_RWXU | DIR_MODE_RWXG | DIR_MODE_RWXO);

    String *bin_file = format__String("{S}/{s}", bin_dir_result, bin->name);

    // NOTE: The binary the last run left behind is removed, so that a compile
    // which fails leaves nothing to be taken for what it was asked to build.
    remove__File(bin_file->buffer);
    FREE(String, bin_file);

    String *gen_c_dir_result =
      get_dir_result__CIResultFile(file, CI_DIR_RESULT_PURPOSE_C_GEN);
    String *gen_file = format__String(
      "{Sr}/{S}", gen_c_dir_result, file->entity.filename_result);
    String *command = format__String("{S} {Sr}{s} -o {Sr}/{s}",
                                     file->config->compiler.command,
                                     gen_file,
                                     standard_options[file->config->standard],
                                     bin_dir_result,
                                     bin->name);

    return command;
}

String *
build_lib_compile_command__CICompile(const CIResultLib *lib,
                                     const CIResultFile *file)
{
    String *lib_dir_result =
      get_dir_result__CIResultFile(file, CI_DIR_RESULT_PURPOSE_LIB);

    create_recursive_dir__Dir(lib_dir_result->buffer,
                              DIR_MODE_RWXU | DIR_MODE_RWXG | DIR_MODE_RWXO);

    String *lib_file = format__String("{S}/{s}", lib_dir_result, lib->name);

    // NOTE: The library the last run left behind is removed, for the same
    // reason the binary is.
    remove__File(lib_file->buffer);
    FREE(String, lib_file);

    String *gen_c_dir_result =
      get_dir_result__CIResultFile(file, CI_DIR_RESULT_PURPOSE_C_GEN);
    String *gen_file = format__String(
      "{Sr}/{S}", gen_c_dir_result, file->entity.filename_result);
    String *command = format__String("{S} {Sr}{s} -static -o {Sr}/{s}",
                                     file->config->compiler.command,
                                     gen_file,
                                     standard_options[file->config->standard],
                                     lib_dir_result,
                                     lib->name);

    return command;
}

bool
run_command__CICompile(const char *command)
{
    int status = system(command);

    if (status == -1) {
        return false;
    }

#ifdef LILY_UNIX_OS
    // A shell is what `system` runs, so what the compiler exited with is read
    // out of the wait status it hands back. A compiler killed by a signal did
    // not produce what it was asked for either.
    return WIFEXITED(status) && WEXITSTATUS(status) == 0;
#else
    return status == 0;
#endif
}

void
handler__CICompile(void *entity, const CIResultFile *file, void *other_args)
{
    bool *has_failed = other_args;

    switch (file->entity.kind) {
        case CI_RESULT_ENTITY_KIND_BIN: {
            const CIResultBin *bin = entity;
            String *command = build_bin_compile_command__CICompile(bin, file);

            if (!run_command__CICompile(command->buffer)) {
                *has_failed = true;
            }

            FREE(String, command);

            break;
        }
        case CI_RESULT_ENTITY_KIND_LIB: {
            const CIResultLib *lib = entity;
            String *command = build_lib_compile_command__CICompile(lib, file);

            if (!run_command__CICompile(command->buffer)) {
                *has_failed = true;
            }

            FREE(String, command);

            break;
        }
        case CI_RESULT_ENTITY_KIND_FILE:
            UNREACHABLE("file entity is not expected in this context");
        default:
            UNREACHABLE("unknown variant");
    }
}

bool
exec__CICompile(const CIResult *result)
{
    bool has_failed = false;

    pass_through_result__CIResult(result, &handler__CICompile, &has_failed);

    return !has_failed;
}
