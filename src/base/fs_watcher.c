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

#include <base/platform.h>

#ifdef LILY_LINUX_OS

#include <base/assert.h>
#include <base/fs_watcher.h>

#include <sys/inotify.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

CONSTRUCTOR(FsWatcher, FsWatcher, Int32 flags)
{
    Int32 fd = inotify_init1(flags);

    ASSERT(fd != -1);

    return (FsWatcher){ .fd = fd, .wd = { .content = NULL, .len = 0 } };
}

void
add__FsWatcher(FsWatcher *self, const char *path, Int32 mask)
{
    Int32 new_wd_item = inotify_add_watch(self->fd, path, mask);

    ASSERT(new_wd_item != -1);

    if (!self->wd.content) {
        self->wd.content = malloc(sizeof(Int32));
        *self->wd.content = new_wd_item;
        ++self->wd.len;
        return;
    }

    self->wd.content =
      realloc(self->wd.content, sizeof(Int32) * (self->wd.len + 1));
    self->wd.content[self->wd.len++] = new_wd_item;
}

Int32
get_fd__FsWatcher(const FsWatcher *self)
{
    ASSERT(self->fd != -1);

    return self->fd;
}

DESTRUCTOR(FsWatcher, FsWatcher, const FsWatcher *self)
{
    close(self->fd);

    if (self->wd.content) {
        free(self->wd.content);
    }
}

#else
#warning "not yet implemented for this OS"
#endif // LILY_LINUX_OS
