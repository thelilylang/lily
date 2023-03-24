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

#ifndef LILY_BASE_TEST_H
#define LILY_BASE_TEST_H

#include <base/new.h>
#include <base/types.h>
#include <base/vec.h>

#include <stdarg.h>

#define TEST_SUCCESS 0
#define TEST_FAILED 1
#define TEST_SKIPPED -1

#define TEST_ASSERT_EQ(x, y) \
    if (x != y)              \
        return TEST_FAILED;

#define TEST_ASSERT_NE(x, y) \
    if (x == y)              \
        return TEST_FAILED;

#define TEST_ASSERT(x) \
    if (!x)            \
        return TEST_FAILED;

#define CASE(name, block)                   \
    int test_case__##name(TestSuite *suite) \
    {                                       \
        block;                              \
        return TEST_SUCCESS;                \
    }

#define SKIP_CASE(name, block)              \
    int test_case__##name(TestSuite *suite) \
    {                                       \
        do {                                \
            return TEST_SKIPPED;            \
        } while (0);                        \
        block;                              \
    }

#define SIMPLE(name, block)             \
    int test_simple__##name(Test *test) \
    {                                   \
        block;                          \
        return TEST_SUCCESS;            \
    }

#define SKIP_SIMPLE(name, block)        \
    int test_simple__##name(Test *test) \
    {                                   \
        do {                            \
            return TEST_SKIPPED;        \
        } while (0);                    \
        block;                          \
    }

#define SUITE(name, n_case, ...)                      \
    Suite test_suite__##name(Test *test)              \
    {                                                 \
        Vec *cases = NEW(Vec);                        \
        va_arg vl;                                    \
        va_start(vl, n_case);                         \
                                                      \
        for (Usiz i = 0; i < n_case; i++) {           \
            push__Vec(cases, va_arg(vl, TestCase *)); \
        }                                             \
                                                      \
        return NEW(TestSuite, name, cases);           \
    }

typedef struct TestSuite TestSuite;
typedef struct Test Test;

typedef struct TestCase
{
    char *name;
    int (*f)(TestSuite *);
} TestCase;

/**
 *
 * @brief Construct TestCase type.
 */
CONSTRUCTOR(TestCase *, TestCase, char *name, int (*f)(TestSuite *));

/**
 *
 * @brief Free TestCase type.
 */
inline DESTRUCTOR(TestCase, TestCase *self)
{
    lily_free(self);
}

typedef struct TestSimple
{
    char *name;
    int (*f)(Test *);
} TestSimple;

/**
 *
 * @brief Construct TestSimple type.
 */
inline CONSTRUCTOR(TestSimple, TestSimple, char *name, int (*f)(Test *))
{
    return (TestSimple){ .name = name, .f = f };
}

struct TestSuite
{
    char *name;
    Vec *cases; // Vec<TestCase*>*
};

/**
 *
 * @brief Construct TestSuite type.
 */
inline CONSTRUCTOR(TestSuite, TestSuite, char *name, Vec *cases)
{
    return (TestSuite){ .name = name, .cases = cases };
}

/**
 *
 * @brief Free TestSuite type.
 */
DESTRUCTOR(TestSuite, const TestSuite *self);

enum TestItemKind
{
    TEST_ITEM_KIND_SIMPLE,
    TEST_ITEM_KIND_SUITE,
};

typedef struct TestItem
{
    enum TestItemKind kind;
    union
    {
        TestSimple simple;
        TestSuite suite;
    };
} TestItem;

/**
 *
 * @brief Construct TestItem type (TEST_ITEM_KIND_SIMPLE).
 */
VARIANT_CONSTRUCTOR(TestItem *, TestItem, simple, TestSimple simple);

/**
 *
 * @brief Construct TestItem type (TEST_ITEM_KIND_SUITE).
 */
VARIANT_CONSTRUCTOR(TestItem *, TestItem, suite, TestSuite suite);

struct Test
{
    Vec *items; // Vec<TestItem*>*
};

/**
 *
 * @brief Construct Test type.
 */
inline CONSTRUCTOR(Test, Test, Vec *items)
{
    return (Test){ .items = items };
}

/**
 *
 * @brief Free Test type.
 */
DESTRUCTOR(Test, const Test *self);

#endif // LILY_BASE_TEST_H