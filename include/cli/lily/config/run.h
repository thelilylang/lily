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

#ifndef LILY_CLI_LILY_CONFIG_RUN_H
#define LILY_CLI_LILY_CONFIG_RUN_H

#include <base/macros.h>
#include <base/vec.h>

typedef struct LilyConfigRun
{
    const char *filename;
    bool verbose;
    Vec *args; // Vec<char*>* (&)
    Usize max_stack;
    Usize max_heap;
} LilyConfigRun;

/**
 *
 * @brief Construct LilyConfigRun type.
 */
inline CONSTRUCTOR(LilyConfigRun,
                   LilyConfigRun,
                   const char *filename,
                   bool verbose,
                   Vec *args,
                   Usize max_stack,
                   Usize max_heap)
{
    return (LilyConfigRun){ .filename = filename,
                            .verbose = verbose,
                            .args = args,
                            .max_stack = max_stack,
                            .max_heap = max_heap };
}

#endif // LILY_CLI_LILY_CONFIG_RUN_H
