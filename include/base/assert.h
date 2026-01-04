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

#ifndef LILY_BASE_ASSERT_H
#define LILY_BASE_ASSERT_H

#include <base/macros.h>

#if defined(ENV_DEBUG) || defined(ENV_ASSERT) || defined(ENV_SAFE)
#define ASSERT_EQ(a, b)                                                \
    do {                                                               \
        if (a != b) {                                                  \
            printf("ASSERT_EQ failed: (%s:%d)\n", __FILE__, __LINE__); \
            exit(1);                                                   \
        }                                                              \
    } while (0)
#else
#define ASSERT_EQ(a, b) \
    (void)(a);          \
    (void)(b)
#endif

#if defined(ENV_DEBUG) || defined(ENV_ASSERT) || defined(ENV_SAFE)
#define ASSERT_NE(a, b)                                                \
    do {                                                               \
        if (a == b) {                                                  \
            printf("ASSERT_NE failed: (%s:%d)\n", __FILE__, __LINE__); \
            exit(1);                                                   \
        }                                                              \
    } while (0)
#else
#define ASSERT_NE(a, b) \
    (void)(a);          \
    (void)(b)
#endif

#if defined(ENV_DEBUG) || defined(ENV_ASSERT) || defined(ENV_SAFE)
#define ASSERT(a)                                                   \
    do {                                                            \
        if (!(a)) {                                                 \
            printf("ASSERT failed: (%s:%d)\n", __FILE__, __LINE__); \
            exit(1);                                                \
        }                                                           \
    } while (0)
#else
#define ASSERT(a) (void)(a)
#endif

#endif // LILY_BASE_ASSERT_H
