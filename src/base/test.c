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

#define _GNU_SOURCE

#include <base/fork.h>
#include <base/new.h>
#include <base/print.h>
#include <base/test.h>

#include <stdio.h>
#include <string.h>
#include <time.h>

// TODO: Replace all `printf` by `PRINT` (later...)

// Free TestItem type (TEST_ITEM_KIND_SIMPLE).
static inline VARIANT_DESTRUCTOR(TestItem, simple, TestItem *self);

// Free TestItem type (TEST_ITEM_KIND_SUITE).
static VARIANT_DESTRUCTOR(TestItem, suite, TestItem *self);

static inline void
display_runs_output__Test(char *name);

static inline void
display_pass_case_output__Test(char *name, double time);

static inline void
display_pass_suite_output__Test(char *name, double time);

static inline void
display_pass_simple_output__Test(char *name, double time);

static inline void
display_pass_test_output__Test(char *name);

static inline void
display_skipped_output__Test(char *name, double time);

static void
display_failed_case_or_suite_output__Test(char *name,
                                          double time,
                                          int exit_status,
                                          int kill_signal,
                                          int stop_signal,
                                          const char *kind_s);

static inline void
display_failed_case_output__Test(char *name,
                                 double time,
                                 int exit_status,
                                 int kill_signal,
                                 int stop_signal);

static inline void
display_failed_simple_output__Test(char *name,
                                   double time,
                                   int exit_status,
                                   int kill_signal,
                                   int stop_signal);

static inline void
display_failed_test_output__Test(char *name);

static inline void
display_failed_suite_output__Test(char *name, double time);

#define RUN_OPTION_CASE 0
#define RUN_OPTION_SIMPLE 1

static void
run_case_or_simple__Test(char *name,
                         void (*f)(void),
                         bool *is_parent_failed,
                         Usize *n_skipped,
                         Usize *n_failed,
                         Usize *total,
                         int option);

CONSTRUCTOR(TestCase *, TestCase, char *name, void (*f)(void))
{
    TestCase *self = lily_malloc(sizeof(TestCase));

    self->name = name;
    self->f = f;

    return self;
}

DESTRUCTOR(TestSuite, const TestSuite *self)
{
    FREE_BUFFER_ITEMS(self->cases->buffer, self->cases->len, TestCase);
    FREE(Vec, self->cases);
}

VARIANT_CONSTRUCTOR(TestItem *, TestItem, simple, TestSimple simple)
{
    TestItem *self = lily_malloc(sizeof(TestItem));

    self->kind = TEST_ITEM_KIND_SIMPLE;
    self->simple = simple;

    return self;
}

VARIANT_CONSTRUCTOR(TestItem *, TestItem, suite, TestSuite suite)
{
    TestItem *self = lily_malloc(sizeof(TestItem));

    self->kind = TEST_ITEM_KIND_SUITE;
    self->suite = suite;

    return self;
}

VARIANT_DESTRUCTOR(TestItem, simple, TestItem *self)
{
    lily_free(self);
}

VARIANT_DESTRUCTOR(TestItem, suite, TestItem *self)
{
    FREE(TestSuite, &self->suite);
    lily_free(self);
}

DESTRUCTOR(TestItem, TestItem *self)
{
    switch (self->kind) {
        case TEST_ITEM_KIND_SIMPLE:
            FREE_VARIANT(TestItem, simple, self);
            break;
        case TEST_ITEM_KIND_SUITE:
            FREE_VARIANT(TestItem, suite, self);
            break;
        default:
            UNREACHABLE("unknown variant");
    }
}

void
display_runs_output__Test(char *name)
{
    printf("\r\x1b[37m\x1b[43mRUNS\x1b[0m \x1b[30m%s\x1b[0m", name);
    fflush(stdout);
}

void
display_pass_case_output__Test(char *name, double time)
{
    printf("\r\x1b[37m\x1b[42mCASE\x1b[0m \x1b[30m%s (%.2f ms)\n\x1b[0m",
           name,
           time);
}

void
display_pass_suite_output__Test(char *name, double time)
{
    printf("\r\x1b[37m\x1b[42mSUITE\x1b[0m \x1b[30m%s (%.2f ms)\n\x1b[0m",
           name,
           time);
}

void
display_pass_simple_output__Test(char *name, double time)
{
    printf("\r\x1b[37m\x1b[42mSIMPLE\x1b[0m \x1b[30m%s (%.2f ms)\n\x1b[0m",
           name,
           time);
}

void
display_pass_test_output__Test(char *name)
{
    printf("\r\x1b[37m\x1b[42mTEST\x1b[0m \x1b[30m%s\n\x1b[0m", name);
}

void
display_skipped_output__Test(char *name, double time)
{
    printf("\r\x1b[37m\x1b[43mSKIPPED\x1b[0m \x1b[30m%s (%.2f ms)\n\x1b[0m",
           name,
           time);
}

void
display_failed_case_or_suite_output__Test(char *name,
                                          double time,
                                          int exit_status,
                                          int kill_signal,
                                          int stop_signal,
                                          const char *kind_s)

{
    char *exit_status_msg = exit_status != -1
                              ? format("\x1b[31m[Exit={d}]\x1b[0m", exit_status)
                              : NULL;
    char *signal_msg =
      kill_signal != -1 || stop_signal != -1
        ? format("\x1b[31m[Signal={s}]\x1b[0m",
                 strsignal(kill_signal == -1 ? stop_signal : kill_signal))
        : NULL;

    printf("\r\x1b[37m\x1b[41m%s\x1b[0m \x1b[30m%s (%.2f ms)\x1b[0m%s%s%s\n",
           kind_s,
           name,
           time,
           exit_status == -1 && kill_signal == -1 && stop_signal == -1 ? ""
                                                                       : " ",
           exit_status_msg ? exit_status_msg : "",
           signal_msg ? signal_msg : "");

    if (exit_status_msg) {
        lily_free(exit_status_msg);
    }

    if (signal_msg) {
        lily_free(signal_msg);
    }
}

void
display_failed_case_output__Test(char *name,
                                 double time,
                                 int exit_status,
                                 int kill_signal,
                                 int stop_signal)
{
    display_failed_case_or_suite_output__Test(
      name, time, exit_status, kill_signal, stop_signal, "CASE");
}

void
display_failed_simple_output__Test(char *name,
                                   double time,
                                   int exit_status,
                                   int kill_signal,
                                   int stop_signal)
{
    display_failed_case_or_suite_output__Test(
      name, time, exit_status, kill_signal, stop_signal, "SIMPLE");
}

static inline void
display_failed_test_output__Test(char *name)
{
    printf("\r\x1b[37m\x1b[41mTEST\x1b[0m \x1b[30m%s\n\x1b[0m", name);
}

void
display_failed_suite_output__Test(char *name, double time)
{
    printf("\r\x1b[37m\x1b[41mSUITE\x1b[0m \x1b[30m%s (%.2f ms)\n\x1b[0m",
           name,
           time);
}

#define DISPLAY_RESULT(                                           \
  name, kind, f_res, time, exit_status, kill_signal, stop_signal) \
    switch (f_res) {                                              \
        case TEST_PASS:                                           \
            display_pass_##kind##_output__Test(name, time);       \
                                                                  \
            break;                                                \
        case TEST_SKIP:                                           \
            display_skipped_output__Test(name, time);             \
                                                                  \
            break;                                                \
        case TEST_FAIL:                                           \
            display_failed_##kind##_output__Test(                 \
              name, time, exit_status, kill_signal, stop_signal); \
                                                                  \
            break;                                                \
        default:                                                  \
            UNREACHABLE("unknown status");                        \
    }

void
run_case_or_simple__Test(char *name,
                         void (*f)(void),
                         bool *is_parent_failed,
                         Usize *n_skipped,
                         Usize *n_failed,
                         Usize *total,
                         int option)
{
    display_runs_output__Test(name);

    clock_t time_simple = clock();
    int exit_status = -1;
    int kill_signal = -1;
    int stop_signal = -1;
    int f_res = 0;
    Fork fork_pid = run__Fork();

    use__Fork(fork_pid, f, &exit_status, &kill_signal, &stop_signal);

    if (exit_status == TEST_SKIP) {
        ++(*n_skipped);
        f_res = TEST_SKIP;
    } else if (exit_status == TEST_FAIL || exit_status != TEST_PASS ||
               kill_signal != -1 || stop_signal != -1) {
        ++(*n_failed);
        *is_parent_failed = true;
        f_res = TEST_FAIL;
    } else {
        f_res = TEST_PASS;
    }

    switch (option) {
        case RUN_OPTION_CASE:
            DISPLAY_RESULT(name,
                           case,
                           f_res,
                           (double)(clock() - time_simple),
                           exit_status,
                           kill_signal,
                           stop_signal);
            break;
        case RUN_OPTION_SIMPLE:
            DISPLAY_RESULT(name,
                           simple,
                           f_res,
                           (double)(clock() - time_simple),
                           exit_status,
                           kill_signal,
                           stop_signal);
            break;
        default:
            UNREACHABLE("unknown option");
    }

    ++(*total);
}

int
run__Test(const Test *self)
{
    clock_t start = clock();
    bool is_test_failed = false;
    Usize n_simple = 0;
    Usize n_suite = 0;
    Usize n_case = 0;
    Usize n_simple_skipped = 0;
    Usize n_simple_failed = 0;
    Usize n_suite_failed = 0;
    Usize n_case_skipped = 0;
    Usize n_case_failed = 0;

    // Run all items.
    for (Usize i = 0; i < self->items->len; i++) {
        TestItem *item = get__Vec(self->items, i);

        switch (item->kind) {
            case TEST_ITEM_KIND_SIMPLE: {
                for (; i < self->items->len; i++) {
                    item = get__Vec(self->items, i);

                    if (item->kind == TEST_ITEM_KIND_SIMPLE) {
                        run_case_or_simple__Test(item->simple.name,
                                                 item->simple.f,
                                                 &is_test_failed,
                                                 &n_simple_skipped,
                                                 &n_simple_failed,
                                                 &n_simple,
                                                 RUN_OPTION_SIMPLE);
                    } else {
                        break;
                    }
                }

                i--;
                puts("");

                break;
            }
            case TEST_ITEM_KIND_SUITE: {
                bool is_suite_failed = false;
                clock_t time_suite = clock();

                for (Usize i = 0; i < item->suite.cases->len; i++) {
                    TestCase *test_case = get__Vec(item->suite.cases, i);

                    run_case_or_simple__Test(test_case->name,
                                             test_case->f,
                                             &is_suite_failed,
                                             &n_case_skipped,
                                             &n_case_failed,
                                             &n_case,
                                             RUN_OPTION_CASE);
                }

                if (is_suite_failed) {
                    is_test_failed = true;
                    ++n_suite_failed;
                    display_failed_suite_output__Test(
                      item->suite.name, (double)(clock() - time_suite));
                } else {
                    display_pass_suite_output__Test(
                      item->suite.name, (double)(clock() - time_suite));
                }

                puts("");

                ++n_suite;

                break;
            }
        }
    }

    if (is_test_failed) {
        display_failed_test_output__Test(self->name);
    } else {
        display_pass_test_output__Test(self->name);
    }

    PRINT("\n\x1b[30mSuite:\x1b[0m \x1b[32m{zu} passed,\x1b[0m \x1b[31m{zu} "
          "failed,\x1b[0m "
          "\x1b[30m{zu} total\n\x1b[0m",
          n_suite - n_suite_failed,
          n_suite_failed,
          n_suite);

    PRINT("\x1b[30mSimple:\x1b[0m \x1b[32m{zu} passed,\x1b[0m \x1b[31m{zu} "
          "failed,\x1b[0m \x1b[33m{zu} skipped,\x1b[0m "
          "\x1b[30m{zu} total\n\x1b[0m",
          n_simple - n_simple_failed - n_simple_skipped,
          n_simple_failed,
          n_simple_skipped,
          n_simple);

    PRINT("\x1b[30mCase:\x1b[0m \x1b[32m{zu} passed,\x1b[0m \x1b[31m{zu} "
          "failed,\x1b[0m \x1b[33m{zu} skipped,\x1b[0m "
          "\x1b[30m{zu} total\n\x1b[0m",
          n_case - n_case_failed - n_case_skipped,
          n_case_failed,
          n_case_skipped,
          n_case);

    printf("\x1b[30mTime: %.2fs\n\x1b[0m",
           (double)(clock() - start) / CLOCKS_PER_SEC);

    if (is_test_failed) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

DESTRUCTOR(Test, const Test *self)
{
    FREE_BUFFER_ITEMS(self->items->buffer, self->items->len, TestItem);
    FREE(Vec, self->items);
}
