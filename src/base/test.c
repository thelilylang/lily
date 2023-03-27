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

#include <base/new.h>
#include <base/test.h>

#include <stdio.h>
#include <time.h>

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

static inline void
display_failed_case_output__Test(char *name, double time);

static inline void
display_failed_simple_output__Test(char *name, double time);

static inline void
display_failed_test_output__Test(char *name);

static inline void
display_failed_suite_output__Test(char *name, double time);

CONSTRUCTOR(TestCase *, TestCase, char *name, int (*f)(void))
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
display_failed_case_output__Test(char *name, double time)
{
    printf("\r\x1b[37m\x1b[41mCASE\x1b[0m \x1b[30m%s (%.2f ms)\n\x1b[0m",
           name,
           time);
}

void
display_failed_simple_output__Test(char *name, double time)
{
    printf("\r\x1b[37m\x1b[41mSIMPLE\x1b[0m \x1b[30m%s (%.2f ms)\n\x1b[0m",
           name,
           time);
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

#define DISPLAY_RESULT(name, kind, f_res, time)               \
    switch (f_res) {                                          \
        case TEST_PASS:                                       \
            display_pass_##kind##_output__Test(name, time);   \
                                                              \
            break;                                            \
        case TEST_SKIP:                                       \
            display_skipped_output__Test(name, time);         \
                                                              \
            break;                                            \
        case TEST_FAIL:                                       \
            display_failed_##kind##_output__Test(name, time); \
                                                              \
            break;                                            \
        default:                                              \
            UNREACHABLE("unknown status");                    \
    }

int
run__Test(const Test *self)
{
    clock_t start = clock();
    bool is_failed = false;
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
                    if (item->kind == TEST_ITEM_KIND_SIMPLE) {
                        display_runs_output__Test(item->simple.name);

                        clock_t time_simple = clock();
                        int f_res = item->simple.f();

                        if (f_res == TEST_FAIL) {
                            ++n_simple_failed;
                            is_failed = true;
                        } else if (f_res == TEST_SKIP) {
                            ++n_simple_skipped;
                        }

                        DISPLAY_RESULT(item->simple.name,
                                       simple,
                                       f_res,
                                       (double)(clock() - time_simple));

						if (i + 1 < self->items->len) {
							item = self->items->buffer[i + 1];
						}

                        ++n_simple;
                    } else {
                        break;
                    }
                }

                i--;
                puts("");

                break;
            }
            case TEST_ITEM_KIND_SUITE: {
                int is_failed = false;

                clock_t time_suite = clock();

                for (Usize i = 0; i < item->suite.cases->len; i++) {
                    TestCase *test_case = get__Vec(item->suite.cases, i);

                    display_runs_output__Test(test_case->name);

                    clock_t time_case = clock();
                    int f_res = test_case->f();

                    if (f_res == TEST_FAIL) {
                        ++n_case_failed;
                        is_failed = true;
                    } else if (f_res == TEST_SKIP) {
                        ++n_case_skipped;
                    }

                    DISPLAY_RESULT(test_case->name,
                                   case,
                                   f_res,
                                   (double)(clock() - time_case));

                    ++n_case;
                }

                if (is_failed) {
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

    if (is_failed) {
        display_failed_test_output__Test(self->name);
    } else {
        display_pass_test_output__Test(self->name);
    }

    printf("\n\x1b[30mSuite:\x1b[0m \x1b[32m%zu passed,\x1b[0m \x1b[31m%zu "
           "failed,\x1b[0m "
           "\x1b[30m%zu total\n\x1b[0m",
           n_suite - n_suite_failed,
           n_suite_failed,
           n_suite);

    printf("\x1b[30mSimple:\x1b[0m \x1b[32m%zu passed,\x1b[0m \x1b[31m%zu "
           "failed,\x1b[0m \x1b[33m%zu skipped,\x1b[0m "
           "\x1b[30m%zu total\n\x1b[0m",
           n_simple - n_simple_failed - n_simple_skipped,
           n_simple_failed,
           n_simple_skipped,
           n_simple);

    printf("\x1b[30mCase:\x1b[0m \x1b[32m%zu passed,\x1b[0m \x1b[31m%zu "
           "failed,\x1b[0m \x1b[33m%zu skipped,\x1b[0m "
           "\x1b[30m%zu total\n\x1b[0m",
           n_case - n_case_failed - n_case_skipped,
           n_case_failed,
           n_case_skipped,
           n_case);

    printf("\x1b[30mTime: %.2fs\n\x1b[0m",
           (double)(clock() - start) / CLOCKS_PER_SEC);

    if (is_failed) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

DESTRUCTOR(Test, const Test *self)
{
    FREE_BUFFER_ITEMS(self->items->buffer, self->items->len, TestItem);
    FREE(Vec, self->items);
}
