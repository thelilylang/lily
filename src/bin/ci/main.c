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

#include <core/cc/ci/parser.h>
#include <core/cc/ci/result.h>
#include <core/cc/ci/scanner.h>

#include <stdio.h>

int
main(int argc, char **argv)
{
    // TODO: implement a real CLI, like in bin/lily/main.c, bin/lilyc/main.c
    if (argc >= 3) {
        enum CIStandard standard;

        if (!strcmp(argv[1], "c89")) {
            standard = CI_STANDARD_89;
        } else if (!strcmp(argv[1], "c95")) {
            standard = CI_STANDARD_95;
        } else if (!strcmp(argv[1], "c99")) {
            standard = CI_STANDARD_99;
        } else if (!strcmp(argv[1], "c11")) {
            standard = CI_STANDARD_11;
        } else if (!strcmp(argv[1], "c17")) {
            standard = CI_STANDARD_17;
        } else if (!strcmp(argv[1], "c23")) {
            standard = CI_STANDARD_23;
        } else {
            UNREACHABLE("unknown standard");
        }

        CIResult result = NEW(CIResult);

        for (Usize i = 2; i < argc; ++i) {
            char *extension = get_extension__File(argv[i]);
            String *filename_result = get_filename__File(argv[i]);
            bool is_header = false;

            if (!strcmp(extension, ".ci")) {
                push_str__String(filename_result, ".c");
            } else if (!strcmp(extension, ".hci")) {
                push_str__String(filename_result, ".h");
                is_header = true;
            } else {
                lily_free(extension);
                FREE(String, filename_result);
                FREE(CIResult, &result);

                FAILED("unknown extension, expected `.ci` or `.hci`");
            }

            const bool has_header =
              is_header ? has_header__CIResult(&result, filename_result)
                        : false;
            const bool has_source =
              !is_header ? has_source__CIResult(&result, filename_result)
                         : false;

            if (has_header || has_source) {
                lily_free(extension);
                FREE(String, filename_result);

                continue;
            }

            char *file_content = read_file__File(argv[i]);
            File file_input = NEW(File, argv[i], file_content);
            CIResultFile *result_file =
              NEW(CIResultFile, standard, filename_result, file_input);

            run__CIResultFile(result_file);

            if (is_header) {
                add_header__CIResult(&result, result_file);
            } else {
                add_source__CIResult(&result, result_file);
            }

            lily_free(extension);
        }

        FREE(CIResult, &result);
    } else {
        printf("ci [standard] [ci_files...]\n");
    }
}
