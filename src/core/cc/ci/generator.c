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

#include <base/dir.h>

#include <core/cc/ci/generator.h>

static void
run_file__CIGenerator(const CIResultFile *file_result);

static void
generate_decl__CIGenerator();

// NOTE: If the program is multi-threaded, you'll need to adapt these variables
// to multi-threading.
static String *current_result_content = NULL;

void
run__CIGenerator(const CIResult *result)
{
    HashMapIter iter_headers = NEW(HashMapIter, result->headers);
    HashMapIter iter_sources = NEW(HashMapIter, result->sources);
    CIResultFile *current_header = NULL;
    CIResultFile *current_source = NULL;

    while ((current_header = next__HashMapIter(&iter_headers))) {
        run_file__CIGenerator(current_header);
    }

    while ((current_source = next__HashMapIter(&iter_sources))) {
        run_file__CIGenerator(current_source);
    }
}

void
run_file__CIGenerator(const CIResultFile *file_result)
{
    current_result_content = NEW(String);
    // TODO: Add a possibly to the user to "create its custom output director"
    const char *output_dir = "out.ci";
    String *dir_result = format__String(
      "{s}/{Sr}", output_dir, get_dir__File(file_result->file_input.name));
    String *path_result =
      format__String("{S}/{S}", dir_result, file_result->filename_result);

    create_recursive_dir__Dir(dir_result->buffer,
                              DIR_MODE_RWXU | DIR_MODE_RWXG | DIR_MODE_RWXO);
    write_file__File(path_result->buffer, current_result_content->buffer);

    FREE(String, current_result_content);
    FREE(String, dir_result);
    FREE(String, path_result);
}
