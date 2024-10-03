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

#ifndef LILY_BASE_TEST_H
#define LILY_BASE_TEST_H

#include <base/alloc.h>
#include <base/macros.h>
#include <base/new.h>
#include <base/types.h>
#include <base/vec.h>

#include <stdarg.h>

// This macro enables the use of fork(...) (or equivalent if we're not talking
// about a Unix-like OS) during test execution.
#define TEST_USE_FORK

// Here we define non-standard return status codes, which will only be available
// in this test library.
#define TEST_PASS EXIT_OK
#define TEST_FAIL EXIT_ERR
#define TEST_SKIP 10

#define TEST_ASSERT_EQ_BASE(x, y, e) \
    if ((x) != (y))                  \
        e(TEST_FAIL);

#ifdef TEST_USE_FORK
#define TEST_ASSERT_EQ(x, y) TEST_ASSERT_EQ_BASE(x, y, exit)
#else
#define TEST_ASSERT_EQ(x, y) TEST_ASSERT_EQ_BASE(x, y, return)
#endif

#define TEST_ASSERT_NE_BASE(x, y, e) \
    if ((x) == (y))                  \
        e(TEST_FAIL);

#ifdef TEST_USE_FORK
#define TEST_ASSERT_NE(x, y) TEST_ASSERT_NE_BASE(x, y, exit)
#else
#define TEST_ASSERT_NE(x, y) TEST_ASSERT_NE_BASE(x, y, return)
#endif

#define TEST_ASSERT_BASE(x, e) \
    if (!(x))                  \
        e(TEST_FAIL);

#ifdef TEST_USE_FORK
#define TEST_ASSERT(x) TEST_ASSERT_BASE(x, exit)
#else
#define TEST_ASSERT(x) TEST_ASSERT_BASE(x, return)
#endif

#define CASE_BASE(T, name) T test_case__##name()

#ifdef TEST_USE_FORK
#define CASE(name, block) \
    CASE_BASE(void, name) \
    {                     \
        block;            \
        exit(TEST_PASS);  \
    }
#else
#define CASE(name, block) \
    CASE_BASE(int, name)  \
    {                     \
        block;            \
        return TEST_PASS; \
    }
#endif

#define SKIP_CASE_BASE(T, name) T test_case__##name()

#ifdef TEST_USE_FORK
#define SKIP_CASE(name, block) \
    SKIP_CASE_BASE(void, name) \
    {                          \
        do {                   \
            exit(TEST_SKIP);   \
        } while (0);           \
        block;                 \
    }
#else
#define SKIP_CASE(name, block) \
    SKIP_CASE_BASE(int, name)  \
    {                          \
        do {                   \
            return TEST_SKIP;  \
        } while (0);           \
        block;                 \
    }
#endif

#define SIMPLE_BASE(T, name) T test_simple__##name()

#ifdef TEST_USE_FORK
#define SIMPLE(name, block) \
    SIMPLE_BASE(void, name) \
    {                       \
        block;              \
        exit(TEST_PASS);    \
    }
#else
#define SIMPLE(name, block) \
    SIMPLE_BASE(int, name)  \
    {                       \
        block;              \
        return TEST_PASS;   \
    }
#endif

#define SKIP_SIMPLE_BASE(T, name) T test_simple__##name()

#ifdef TEST_USE_FORK
#define SKIP_SIMPLE(name, block) \
    SKIP_SIMPLE_BASE(void, name) \
    {                            \
        do {                     \
            exit(TEST_SKIP);     \
        } while (0);             \
        block;                   \
    }
#else
#define SKIP_SIMPLE(name, block) \
    SKIP_SIMPLE_BASE(int, name)  \
    {                            \
        do {                     \
            return TEST_SKIP;    \
        } while (0);             \
        block;                   \
    }
#endif

#define SUITE(name)                                   \
    TestSuite test_suite__##name(int n_case, ...)     \
    {                                                 \
        Vec *cases = NEW(Vec);                        \
        va_list vl;                                   \
        va_start(vl, n_case);                         \
                                                      \
        for (Usize i = 0; i < n_case; i++) {          \
            push__Vec(cases, va_arg(vl, TestCase *)); \
        }                                             \
                                                      \
        va_end(vl);                                   \
                                                      \
        return NEW(TestSuite, #name, cases);          \
    }

#define ADD_SUITE(n_case, name, ...) \
    push__Vec(                       \
      test.items,                    \
      NEW_VARIANT(TestItem, suite, test_suite__##name(n_case, __VA_ARGS__)))

#define ADD_SIMPLE(name)            \
    push__Vec(test.items,           \
              NEW_VARIANT(TestItem, \
                          simple,   \
                          NEW(TestSimple, #name, &test_simple__##name)))

#define NEW_TEST(name) Test test = NEW(Test, name);

#define CALL_CASE(name) NEW(TestCase, #name, &test_case__##name)

#define CALL_SIMPLE(name) NEW(TestSimple, #name, &test_simple__##name)

#define RUN_TEST()                   \
    int test_res = run__Test(&test); \
    FREE(Test, &test);               \
    return test_res;

typedef struct TestSuite TestSuite;

typedef struct TestCase
{
    char *name;
#ifdef TEST_USE_FORK
    void (*f)(void);
#else
    int (*f)(void);
#endif
} TestCase;

/**
 *
 * @brief Construct TestCase type.
 */
#ifdef TEST_USE_FORK
CONSTRUCTOR(TestCase *, TestCase, char *name, void (*f)(void));
#else
CONSTRUCTOR(TestCase *, TestCase, char *name, int (*f)(void));
#endif

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
#ifdef TEST_USE_FORK
    void (*f)(void);
#else
    int (*f)(void);
#endif
} TestSimple;

/**
 *
 * @brief Construct TestSimple type.
 */
#ifdef TEST_USE_FORK
inline CONSTRUCTOR(TestSimple, TestSimple, char *name, void (*f)(void))
#else
inline CONSTRUCTOR(TestSimple, TestSimple, char *name, int (*f)(void))
#endif
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

/**
 *
 * @brief Free TestItem type.
 */
DESTRUCTOR(TestItem, TestItem *self);

typedef struct Test
{
    Vec *items; // Vec<TestItem*>*
    char *name;
} Test;

/**
 *
 * @brief Construct Test type.
 */
inline CONSTRUCTOR(Test, Test, char *name)
{
    return (Test){ .items = NEW(Vec), .name = name };
}

/**
 *
 * @brief Run test.
 */
int
run__Test(const Test *self);

/**
 *
 * @brief Free Test type.
 */
DESTRUCTOR(Test, const Test *self);

#endif // LILY_BASE_TEST_H
