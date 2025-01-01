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

#ifndef LILY_BASE_FS_WATCHER_H
#define LILY_BASE_FS_WATCHER_H

#include <base/platform.h>

#ifdef LILY_LINUX_OS

#include <base/macros.h>
#include <base/types.h>

typedef struct FsWatcher
{
    Int32 fd;
    struct
    {
        Int32 *content; // Int32*?
        Usize len;
    } wd;
} FsWatcher;

/**
 *
 * @brief Construct FsWatcher type.
 */
CONSTRUCTOR(FsWatcher, FsWatcher, Int32 flags);

/**
 *
 * @brief Add to watch.
 * @param path directory or filename path.
 * @param mask On Linux see <sys/inotify.h> header (IN_MODIFY, IN_OPEN, ...).
 */
void
add__FsWatcher(FsWatcher *self, const char *path, Int32 mask);

/**
 *
 * @brief Get file descriptor.
 */
Int32
get_fd__FsWatcher(const FsWatcher *self);

/**
 *
 * @brief Free FsWatcher type.
 */
DESTRUCTOR(FsWatcher, FsWatcher, const FsWatcher *self);

#else
#warning "not yet implemented for this OS"
#endif // LILY_LINUX_OS

#endif // LILY_BASE_FS_WATCHER_H
