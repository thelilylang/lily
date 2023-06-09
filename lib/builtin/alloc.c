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

#include <base/assert.h>
#include <base/platform.h>

#include <builtin/alloc.h>

#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#if defined(LILY_LINUX_OS) || defined(LILY_APPLE_OS)
#include <sys/mman.h>
#include <unistd.h>
#elif defined(LILY_WINDOWS_OS)
#include <windows.h>
#else
#error "This OS is not supported"
#endif

static volatile Heap heap = (Heap){
    .head =
      (HeapChild){
        .block = (Block){ .size = 0, .is_free = 0, .is_used = 0, .ptr = NULL },
        .next = NULL,
        .prev = NULL },
    .tail = NULL,
    .capacity = 0,
    .total_size = 0,
    .total_block = 0,
    .total_free = 0,
};

static volatile HeapChild *
__get_prev__$Alloc(Usize count)
{
    volatile HeapChild *current = &heap.head;

    for (Usize i = 0; i < count; ++i) {
        current = current->next;
    }

    return current;
}

static volatile Block *
__add_block__$Alloc()
{
    if (!heap.head.block.is_used) {
        ++heap.total_block;
        heap.head.block.is_used = 1;

        return &heap.head.block;
    }

    if (heap.tail) {
        if (!heap.tail->block.is_used) {
            heap.tail->block.is_used = 1;

            return &heap.tail->block;
        } else {
            heap.tail = &(HeapChild){
                .block =
                  (Block){ .size = 0, .is_free = 0, .is_used = 1, .ptr = NULL },
                .next = NULL,
                .prev = heap.tail
            };
            ++heap.total_block;

            return &heap.tail->block;
        }
    }

    heap.tail = &(HeapChild){
        .block = (Block){ .size = 0, .is_free = 0, .is_used = 1, .ptr = NULL },
        .next = NULL,
        .prev = &heap.head
    };
    heap.head.next = heap.tail;

    ++heap.total_block;

    return &heap.tail->block;
}

void
__init__$Alloc()
{
#if defined(LILY_LINUX_OS) || defined(LILY_APPLE_OS)
    heap.capacity = sysconf(_SC_PHYS_PAGES) * sysconf(_SC_PAGESIZE);
#elif defined(LILY_WINDOWS_OS)
    MEMORYSTATUSEX status;
    status.dwLength = sizeof(status);
    GlobalMemoryStatusEx(&status);
    heap.capacity = status.ullTotalPhys;
#else
#error "This OS is not supported"
#endif
}

Block *
__align__$Alloc(void *ptr, Usize align)
{
    ASSERT(align % 2 == 0);

    Uptr addr = (Uptr)ptr;
    Uptr aligned_addr = (addr + align - 1) & ~(align - 1);

    return (void *)aligned_addr;
}

volatile Block *
__alloc__$Alloc(Usize size, Usize align)
{
    volatile Block *block = __add_block__$Alloc();

    block->size += size;
    heap.total_size += size;

    int fd = open("/dev/zero", O_RDWR);

    if (fd == -1) {
        perror("Lily(Fail): fail to open `/dev/zero`");
        exit(1);
    }

    if (heap.total_size > heap.capacity) {
        perror("Lily(Fail): too much memory allocation allocated");
        exit(1);
    }

    block->ptr =
      mmap(NULL, block->size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);

    if (block->ptr == MAP_FAILED) {
        perror("Lily(Fail): fail to allocate memory");
        exit(1);
    }

    if (align > 0) {
        block->ptr = __align__$Alloc(block->ptr, align);
    }

    return block;
}

volatile Block *
__resize__$Alloc(volatile Block *block, Usize new_size, Usize align)
{
    if (!block || !block->ptr) {
        perror("Lily(Fail): fail to free a pointer, because the value is NULL");
        exit(1);
    } else if (block->is_free) {
        perror("Lily(Fail): this block is free");
        exit(1);
    }

    if (new_size < block->size) {
        heap.total_size -= block->size - new_size;
    } else {
        heap.total_size += new_size - block->size;
    }

    block->size = new_size;

    int fd = open("/dev/zero", O_RDWR);

    if (fd == -1) {
        perror("Lily(Fail): fail to open `/dev/zero`");
        exit(1);
    }

    if (heap.total_size > heap.capacity) {
        perror("Lily(Fail): too much memory allocation allocated");
        exit(1);
    }

    block->ptr =
      mmap(block->ptr, block->size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);

    if (block->ptr == MAP_FAILED) {
        perror("Lily(Fail): fail to allocate memory");
        exit(1);
    }

    if (align > 0) {
        block->ptr = __align__$Alloc(block->ptr, align);
    }

    return block;
}

void
__free__$Alloc(volatile Block *block)
{
    if (!block->ptr || !block) {
        perror("Lily(Fail): fail to free a pointer, because the value is NULL");
        exit(1);
    } else if (block->is_free) {
        perror("Lily(Fail): this block is already free");
        exit(1);
    }

    munmap(block->ptr, block->size);

    block->is_free = 1;
}

void *
__get_ptr__$Alloc(volatile Block *block)
{
    return block->ptr;
}
