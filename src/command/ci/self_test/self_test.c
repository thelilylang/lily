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

#include <pthread.h>
#include <time.h>

static Atomic(Usize) n_test = 0;
static Atomic(Usize) n_test_failed = 0;

#define MAX_THREADS 5

static pthread_t threads[MAX_THREADS] = { 0 };
static Usize threads_count = 0;

/// @brief A thread-function that run and poll the test.
/// @param args void* (String*)
static void *
run_process_thread__CISelfTest(void *args);

static void
wait_threads__CISelfTest();

static void
start_thread__CISelfTest(String *path);

void *
run_process_thread__CISelfTest(void *args)
{
    String *path = args;

    ++n_test;

    CISelfTestProcessUnit process_unit = run__CISelfTestRun(path);

    run__CISelfTestPoll(&process_unit, &n_test_failed);

    FREE(CISelfTestProcessUnit, &process_unit);
    FREE(String, path);

    return NULL;
}

void
wait_threads__CISelfTest()
{
    for (Usize i = 0; i < threads_count; ++i) {
        pthread_join(threads[i], NULL);
    }
}

void
start_thread__CISelfTest(String *path)
{
    if (threads_count >= MAX_THREADS) {
        wait_threads__CISelfTest();
        memset(threads, 0, sizeof(pthread_t) * MAX_THREADS);

        threads_count = 0;
    }

    pthread_create(&threads[threads_count++],
                   NULL,
                   &run_process_thread__CISelfTest,
                   (void *)path);
}

int
run__CISelfTest(const CIConfig *config)
{
    CIProjectConfig project_config;

    if (is__Dir(config->self_test.path)) {
        project_config = parse_yaml__CIProjectConfig(config->self_test.path);
    } else {
        project_config = parse_ci_cli__CIProjectConfig(config);
    }

    if (project_config.self_tests) {
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
                        start_thread__CISelfTest(current);
                    } else {
                        FREE(String, current);
                    }
                }

                FREE(Vec, files);
            } else {
                start_thread__CISelfTest(clone__String(self_test->path));
            }
        }

        wait_threads__CISelfTest();

        PRINT("\n\x1b[30mTest:\x1b[0m \x1b[32m{zu} passed,\x1b[0m \x1b[31m{zu} "
              "failed,\x1b[0m \x1b[30m{zu} total\n\x1b[0m",
              n_test - n_test_failed,
              n_test_failed,
              n_test);

        printf("\x1b[30mTime: %.2fs\n\x1b[0m",
               (double)(clock() - start) / CLOCKS_PER_SEC);
    }

    FREE(CIProjectConfig, &project_config);

    return n_test_failed > 0 ? EXIT_ERR : EXIT_OK;
}
