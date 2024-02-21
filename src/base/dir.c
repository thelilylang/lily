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

#include <base/alloc.h>
#include <base/assert.h>
#include <base/dir.h>
#include <base/dir_separator.h>
#include <base/new.h>
#include <base/string.h>
#include <base/sys.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef LILY_WINDOWS_OS
#include <direct.h>
#include <windows.h>
#else
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#endif

#define PATH_MAX 4096

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
create_recursive_dir__Dir(const char *path, [[maybe_unused]] enum DirMode mode)
{
    char *current = (char *)path;
    char *dir_separator = strchr(path, DIR_SEPARATOR);
    String *dir = NEW(String);

    while (*current) {
        // Get one directory
        if (dir_separator) {
            while (current != dir_separator) {
                push__String(dir, *(current++));
            }
        } else {
            while (*current) {
                push__String(dir, *(current++));
            }
        }

        if (!exists__Dir(dir->buffer)) {
            create__Dir(dir->buffer, mode);
        }

        push__String(dir, DIR_SEPARATOR);

        if (*current) {
            dir_separator = strchr(++current, DIR_SEPARATOR);
        }
    }

    FREE(String, dir);
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

char *
get_cwd__Dir()
{
    char cwd[PATH_MAX];

#ifdef LILY_WINDOWS_OS
    if (_getcwd(cwd, sizeof(cwd)))
#else
    if (getcwd(cwd, sizeof(cwd)))
#endif
    {
        char *res = lily_malloc(strlen(cwd) + 1);

        strcpy(res, cwd);

        return res;
    }

    return NULL;
}

Vec *
get_files_rec__Dir(const char *path)
{
    DIR *dir = opendir(path);

    if (!dir) {
        return NULL;
    }

    struct dirent *dp;
    Vec *res = NEW(Vec);
    char current_path[PATH_MAX];

    while ((dp = readdir(dir))) {
        if (!strcmp(dp->d_name, ".") && !strcmp(dp->d_name, "..")) {
            strcpy(current_path, path);
            strcat(current_path, "/");
            strcat(current_path, dp->d_name);

            struct __stat__ path_stat;

            __stat__(current_path, &path_stat);

            if (S_ISDIR(path_stat.st_mode)) {
                Vec *res_current_dir = get_files_rec__Dir(path);

                ASSERT(res_current_dir);

                append__Vec(res, res_current_dir);

                FREE(Vec, res_current_dir);
            } else {
                push__Vec(res, from__String(current_path));
            }
        }
    }

    return res;
}
