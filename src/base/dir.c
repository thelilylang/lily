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

#ifdef LILY_WINDOWS_OS
// https://learn.microsoft.com/en-us/windows/win32/fileio/maximum-file-path-limitation?tabs=registry
#define PATH_MAX 260
#elifdef LILY_APPLE_OS
// On MacOS:
// https://developer.apple.com/documentation/applicationservices/1560101-maximum_path_size/cs_max_path
#define PATH_MAX 1024
#elifdef LILY_BSD_OS
// On NetBSD and probably on other BSDs, the limit path is defined in the header
// <limits.h>: #define _POSIX_PATH_MAX			256
#define PATH_MAX 256
#elifdef LILY_LINUX_OS
// On Linux the path limit is defined in the header <linux/limits.h>:
// #define PATH_MAX        4096    /* # chars in a path name including nul */
#define PATH_MAX 4096
#else
#error "this OS is not yet supported"
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

#ifdef LILY_WINDOWS_OS
Vec *
get_files_rec__Dir(const char *path)
{
    char current_path[PATH_MAX];

    sprintf(current_path, "%s\\%s", path, "*");

    WIN32_FIND_DATA find_file_data;
    HANDLE h_find_file = FindFirstFile(current_path, &find_file_data);

    if (h_find_file != INVALID_HANDLE_VALUE) {
        Vec *res = NEW(Vec); // Vec<String*>*

        do {
            if (find_file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                if (strcmp(find_file_data.cFileName, ".") != 0 &&
                    strcmp(find_file_data.cFileName, "..") != 0) {
                    sprintf(
                      current_path, "%s\\%s", path, find_file_data.cFileName);

                    Vec *res_current_dir = get_files_rec__Dir(current_path);

                    ASSERT(res);

                    append__Vec(res, res_current_dir);

                    FREE(Vec, res_current_dir);
                }
            } else {
                push__Vec(
                  res,
                  format__String("{s}\\{s}", path, find_file_data.cFileName));
            }
        } while (FindNextFile(h_find_file, &find_file_data) != 0);

        FindClose(h_find_file);

        return res;
    }

    return NULL;
}
#else
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
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {
            strcpy(current_path, path);
            strcat(current_path, "/");
            strcat(current_path, dp->d_name);

            struct __stat__ path_stat;

            __stat__(current_path, &path_stat);

            if (S_ISDIR(path_stat.st_mode)) {
                Vec *res_current_dir = get_files_rec__Dir(current_path);

                ASSERT(res_current_dir);

                append__Vec(res, res_current_dir);

                FREE(Vec, res_current_dir);
            } else {
                push__Vec(res, from__String(current_path));
            }
        }
    }

    closedir(dir);

    return res;
}
#endif
