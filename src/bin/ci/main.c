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

#include <core/cc/ci/scanner.h>

#include <stdio.h>

int
main(int argc, char **argv)
{
    // TODO: implement a real CLI, like in bin/lily/main.c, bin/lilyc/main.c
    if (argc == 3) {
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

        char *file_content = read_file__File(argv[2]);
        Usize count_error = 0;
        File file = NEW(File, argv[2], file_content);
        CIScanner scanner = NEW(CIScanner,
                                NEW(Source, NEW(Cursor, file_content), &file),
                                &count_error,
                                standard);

        run__CIScanner(&scanner, true);

        FREE(CIScanner, &scanner);
        FREE(File, &file);
    } else {
        printf("ci [standard] [ci_file]\n");
    }
}
