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

#include <base/memory/api.h>

#include <stddef.h>

void *
__align__(void *mem, Usize align)
{
    return __align__$Alloc(mem, align);
}

void *
__alloc__(Usize size, Usize align)
{
#ifdef USE_C_MEMORY_API
    return malloc(size);
#else
    return __alloc__$Alloc(size, align);
#endif
}

void *
__resize__(void *old_mem, Usize old_size, Usize new_size, Usize align)
{
#ifdef USE_C_MEMORY_API
    return realloc(old_mem, new_size);
#else
    return __resize__$Alloc(old_mem, old_size, new_size, align);
#endif
}

void
__free__(void **mem, Usize size, Usize align)
{
#ifdef USE_C_MEMORY_API
    return free(*mem);
#else
    return __free__$Alloc(mem, size, align);
#endif
}
