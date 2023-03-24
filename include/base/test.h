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

#include <base/vec.h>

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

typedef struct TestSimpleCase
{
    char *name;
    int (*f)(Test *);
} TestSimpleCase;

struct TestSuite
{
    char *name;
    Vec *cases; // Vec<TestCase*>*
};

struct Test
{
    Vec *suites;
};

#endif // LILY_BASE_TEST_H