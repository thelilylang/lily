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

#ifndef LILY_BASE_MEMORY_API_H
#define LILY_BASE_MEMORY_API_H

#define USE_C_MEMORY_API
#undef USE_C_MEMORY_API

#include <builtin/alloc.h>

/**
 *
 * @brief Align pointer with the given size alignment.
 */
inline void *
align__MemoryApi(void *mem, Usize align)
{
    return __align__$Alloc(mem, align);
}

/**
 *
 * @brief Alloc to a pointer with the given size + align the allocated pointer.
 */
inline void *
alloc__MemoryApi(Usize size, Usize align)
{
    return __alloc__$Alloc(size, align);
}

/**
 *
 * @brief Resize the given pointer with the new_size + align the resized
 * pointer.
 */
inline void *
resize__MemoryApi(void *mem, Usize new_size, Usize align)
{
    return __resize__$Alloc(mem, new_size, align);
}

/**
 *
 * @brief Free the given pointer according the given size and alignment.
 */
inline void
free__MemoryApi(void **mem, Usize size, Usize align)
{
    return __free__$Alloc(mem, size, align);
}

#endif // LILY_BASE_MEMORY_API_H
