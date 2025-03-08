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

#include <base/dir.h>
#include <base/fork.h>
#include <base/new.h>
#include <base/pipe.h>
#include <base/print.h>

#include <command/ci/self_test/poll.h>
#include <command/ci/self_test/process_unit.h>
#include <command/ci/self_test/run.h>
#include <command/ci/self_test/self_test.h>

#include <core/cc/ci/file.h>
#include <core/cc/ci/project_config.h>

#include <time.h>

void
run__CISelfTest(const CIConfig *config)
{
    CIProjectConfig project_config;

    if (is__Dir(config->self_test.path)) {
        project_config = parse_yaml__CIProjectConfig(config->self_test.path);
    } else {
        project_config = parse_ci_cli__CIProjectConfig(config);
    }

    if (project_config.self_tests) {
        Vec *process_units = NEW(Vec); // Vec<CISelfTestProcessUnit*>*
        Usize n_test = 0;
        Usize n_test_failed = 0;
        clock_t start = clock();

        for (Usize i = 0; i < project_config.self_tests->len; ++i) {
            const CIProjectConfigSelfTest *self_test =
              get__Vec(project_config.self_tests, i);

            if (is__Dir(self_test->path->buffer)) {
                Vec *files = get_files_rec__Dir(self_test->path->buffer);
                VecIter iter = NEW(VecIter, files);
                String *current = NULL;

                while ((current = next__VecIter(&iter))) {
                    if (is_source__CIFile(current->buffer)) {
                        ++n_test;

                        CISelfTestProcessUnit *process_unit =
                          run__CISelfTestRun(current);

                        push__Vec(process_units, process_unit);
                    } else {
                        FREE(String, current);
                    }
                }

                FREE(Vec, files);
            } else {
                ++n_test;

                CISelfTestProcessUnit *process_unit =
                  run__CISelfTestRun(clone__String(self_test->path));

                push__Vec(process_units, process_unit);
            }
        }

        run__CISelfTestPoll(process_units, &n_test_failed);

        PRINT("\n\x1b[30mTest:\x1b[0m \x1b[32m{zu} passed,\x1b[0m \x1b[31m{zu} "
              "failed,\x1b[0m \x1b[30m{zu} total\n\x1b[0m",
              n_test - n_test_failed,
              n_test_failed,
              n_test);

        printf("\x1b[30mTime: %.2fs\n\x1b[0m",
               (double)(clock() - start) / CLOCKS_PER_SEC);

        FREE_BUFFER_ITEMS(
          process_units->buffer, process_units->len, CISelfTestProcessUnit);
        FREE(Vec, process_units);
    }

    FREE(CIProjectConfig, &project_config);
}
