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

#include <base/alloc.h>

#ifdef ENV_SAFE
#include <stdio.h>
#include <stdlib.h>

void *
ecalloc__Alloc(Usize n, Usize size)
{
    void *p = calloc(n, size);

    if (!p) {
        perror("(ecalloc): Try to allocate");
    }

    return p;
}

void *
emalloc__Alloc(Usize size)
{
    void *p = malloc(size);

    if (!p) {
        perror("(emalloc): Try to allocate");
    }

    return p;
}

void *
erealloc__Alloc(void *p, Usize size)
{
    void *new_p = realloc(p, size);

    if (!new_p) {
        perror("(erealloc): Try to allocate");
    }

    return new_p;
}

void
efree__Alloc(void *p)
{
    if (!p) {
        perror("(efree): Try to free");
    }

    free(p);
    p = NULL;
}
#endif