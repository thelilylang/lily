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

#include <core/cc/ci/compile.h>
#include <core/cc/ci/result.h>

static String *
build_bin_compile_command__CICompile(const CIResultBin *bin,
                                     const CIResultFile *file);

static String *
build_lib_compile_command__CICompile(const CIResultLib *lib,
                                     const CIResultFile *file);

/// @param file const CIResultFile* (&)
static void
handler__CICompile(void *entity,
                   const CIResultFile *file,
                   [[maybe_unused]] void *other_args);

String *
build_bin_compile_command__CICompile(const CIResultBin *bin,
                                     const CIResultFile *file)
{
    String *bin_dir_result =
      get_dir_result__CIResultFile(file, CI_DIR_RESULT_PURPOSE_BIN);

    create_recursive_dir__Dir(bin_dir_result->buffer,
                              DIR_MODE_RWXU | DIR_MODE_RWXG | DIR_MODE_RWXO);

    String *gen_c_dir_result =
      get_dir_result__CIResultFile(file, CI_DIR_RESULT_PURPOSE_C_GEN);
    String *gen_file = format__String(
      "{Sr}/{S}", gen_c_dir_result, file->entity.filename_result);
    String *command = format__String("{S} {Sr} -o {Sr}/{s}",
                                     file->config->compiler.command,
                                     gen_file,
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

    String *gen_c_dir_result =
      get_dir_result__CIResultFile(file, CI_DIR_RESULT_PURPOSE_C_GEN);
    String *gen_file = format__String(
      "{Sr}/{S}", gen_c_dir_result, file->entity.filename_result);
    String *command = format__String("{S} {Sr} -static -o {Sr}/{s}",
                                     file->config->compiler.command,
                                     gen_file,
                                     lib_dir_result,
                                     lib->name);

    return command;
}

void
handler__CICompile(void *entity,
                   const CIResultFile *file,
                   [[maybe_unused]] void *other_args)
{
    switch (file->entity.kind) {
        case CI_RESULT_ENTITY_KIND_BIN: {
            const CIResultBin *bin = entity;
            String *command = build_bin_compile_command__CICompile(bin, file);

            system(command->buffer);

            FREE(String, command);

            break;
        }
        case CI_RESULT_ENTITY_KIND_LIB: {
            const CIResultLib *lib = entity;
            String *command = build_lib_compile_command__CICompile(lib, file);

            system(command->buffer);

            FREE(String, command);

            break;
        }
        case CI_RESULT_ENTITY_KIND_FILE:
            UNREACHABLE("file entity is not expected in this context");
        default:
            UNREACHABLE("unknown variant");
    }
}

void
exec__CICompile(const CIResult *result)
{
    pass_through_result__CIResult(result, &handler__CICompile, NULL);
}
