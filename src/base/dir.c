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
#include <base/dir.h>
#include <base/platform.h>
#include <base/sys.h>

#include <stdio.h>
#include <stdlib.h>

#ifdef LILY_WINDOWS_OS
#include <direct.h>
#include <windows.h>
#else
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#endif

void
create__Dir(const char *path, [[maybe_unused]] enum DirMode mode)
{
#ifdef LILY_WINDOWS_OS
    ASSERT(_mkdir(path) != -1);
#else
    ASSERT(!mkdir(path, mode));
#endif
}

void
remove__Dir(const char *path)
{
#ifdef LILY_WINDOWS_OS
    ASSERT(RemoveDirectoryA(path));
#else
    ASSERT(!rmdir(path));
#endif
}

bool
is__Dir(const char *path)
{
    struct __stat__ st;
    return !__stat__(path, &st) && S_ISDIR(st.st_mode);
}

bool
exists__Dir(const char *path)
{
    DIR *dir = opendir(path);

    if (dir) {
        closedir(dir);

        return true;
    }

    return false;
}
