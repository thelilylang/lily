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

#ifndef LILY_BASE_ALLOC_H
#define LILY_BASE_ALLOC_H

#include <base/macros.h>
#include <base/types.h>

#ifndef ENV_SAFE
#include <stdlib.h>
#endif

#ifdef ENV_SAFE
/**
 *
 * @brief Safe calloc function.
 */
void *
ecalloc__Alloc(Usize n, Usize size);

/**
 *
 * @brief Safe malloc function.
 */
void *
emalloc__Alloc(Usize size);

/**
 *
 * @brief Safe realloc function.
 */
void *
erealloc__Alloc(void *p, Usize size);

/**
 *
 * @brief Safe free function.
 */
void
efree__Alloc(void *p);
#endif

#ifdef ENV_SAFE
#define lily_calloc(n, size) ecalloc__Alloc(n, size)
#define lily_malloc(size) emalloc__Alloc(size)
#define lily_realloc(p, size) erealloc__Alloc(p, size)
#define lily_free(p) efree__Alloc(p)
#else
#define lily_calloc(n, size) calloc(n, size)
#define lily_malloc(size) malloc(size)
#define lily_realloc(p, size) realloc(p, size)
#define lily_free(p) free(p)
#endif

#endif // LILY_BASE_ALLOC_H