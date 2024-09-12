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

#define _GNU_SOURCE

#include <base/assert.h>
#include <base/macros.h>
#include <base/platform.h>

#include <builtin/alloc.h>

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(LILY_LINUX_OS) || defined(LILY_APPLE_OS) || defined(LILY_BSD_OS)
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#elif defined(LILY_WINDOWS_OS)
#include <windows.h>
#else
#error "This OS is not yet supported"
#endif

Usize
__max_capacity__$Alloc()
{
#if defined(LILY_LINUX_OS) || defined(LILY_APPLE_OS) || defined(LILY_BSD_OS)
    return sysconf(_SC_PHYS_PAGES) * sysconf(_SC_PAGESIZE);
#elif defined(LILY_WINDOWS_OS)
    MEMORYSTATUSEX status;
    status.dwLength = sizeof(status);
    GlobalMemoryStatusEx(&status);
    return status.ullTotalPhys;
#else
#error "This OS is not yet supported"
#endif
}

void *
__align__$Alloc(void *mem, Usize align)
{
    ASSERT(align % ALIGNMENT_COEFF == 0);

    return (void *)ALIGN(mem, align);
}

void *
__alloc__$Alloc(Usize size, Usize align)
{
#ifdef ENV_SAFE
    Usize max_capacity = __max_capacity__$Alloc();

    if (size > max_capacity) {
        perror("Lily(Fail): too much memory allocation allocated");
        exit(1);
    }
#endif

#if defined(LILY_LINUX_OS) || defined(LILY_APPLE_OS) || defined(LILY_BSD_OS)
    void *mem = mmap(
      NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (mem == MAP_FAILED) {
        perror("Lily(Fail): fail to allocate memory");
        exit(1);
    }
#elif defined(LILY_WINDOWS_OS)
    LPVOID mem =
      VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    if (!mem) {
        perror("Lily(Fail): fail to allocate memory");
        exit(1);
    }
#else
#error "This OS is not yet supported"
#endif

    if (align > 0) {
        mem = __align__$Alloc(mem, align);
    }

    return mem;
}

void *
__resize__$Alloc(void *old_mem, Usize old_size, Usize new_size, Usize align)
{
    if (new_size == 0) {
        return old_mem;
    } else if (!old_mem) {
        return __alloc__$Alloc(new_size, align);
    } else if (new_size <= old_size) {
        return old_mem;
    }

#if defined(LILY_LINUX_OS)
    void *new_mem = mremap(old_mem, old_size, new_size, MREMAP_MAYMOVE);

    if (new_mem == MAP_FAILED) {
        new_mem = __alloc__$Alloc(new_size, align);

        memcpy(new_mem, old_mem, old_size);
        // NOTE: We assume that the alignment does not change
        __free__$Alloc(&old_mem, old_size, align);
    }
#elif defined(LILY_APPLE_OS) || defined(LILY_BSD_OS)
    void *new_mem = mmap(NULL,
                         new_size,
                         PROT_READ | PROT_WRITE,
                         MAP_PRIVATE | MAP_ANONYMOUS,
                         -1,
                         0);

    if (new_mem == MAP_FAILED) {
        perror("Lily(Fail): fail to allocate memory");
        exit(1);
    }

    memcpy(new_mem, old_mem, old_size);
    // NOTE: We assume that the alignment does not change
    __free__$Alloc(&old_mem, old_size, align);
#elif defined(LILY_WINDOWS_OS)
    LPVOID new_mem =
      VirtualAlloc(NULL, new_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    if (!new_mem) {
        perror("Lily(Fail): fail to allocate memory");
        exit(1);
    }

    memcpy(new_mem, old_mem, old_size);
    // NOTE: We assume that the alignment does not change
    __free__$Alloc(&old_mem, old_size, align);
#else
#error "This is not yet supported"
#endif

    if (align > 0) {
        new_mem = __align__$Alloc(new_mem, align);
    }

    return new_mem;
}

void
__free__$Alloc(void **mem, Usize size, Usize align)
{
    if (!(*mem) || !mem) {
        perror("Lily(Fail): fail to free a pointer, because the value is NULL");
        exit(1);
    }

    // Calculate the aligned starting address based on the alignment
    void *aligned_mem =
      align == 0 ? *mem : (void *)((uintptr_t)(*mem) & ~(align - 1));

    // Calculate the size to unmap, taking into account the alignment
    size_t aligned_size = size + ((uintptr_t)(*mem) - (uintptr_t)(aligned_mem));

#if defined(LILY_LINUX_OS) || defined(LILY_APPLE_OS) || defined(LILY_BSD_OS)
    if (munmap(aligned_mem, aligned_size) == -1) {
        perror("Lily(Fail): fail to free memory");
        exit(1);
    }
#elif defined(LILY_WINDOWS_OS)
    if (!VirtualFree(aligned_mem, aligned_size, MEM_RELEASE)) {
        perror("Lily(Fail): fail to free memory");
        exit(1);
    }
#else
#error "This OS is not yet supported"
#endif

    *mem = 0;
}
