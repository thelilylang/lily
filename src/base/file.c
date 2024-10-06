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
#include <base/file.h>
#include <base/format.h>
#include <base/macros.h>
#include <base/new.h>
#include <base/path.h>
#include <base/str.h>
#include <base/sys.h>
#include <base/types.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef LILY_WINDOWS_OS
#include <io.h>
#include <windows.h>
#else
#include <unistd.h>
#endif

#define FILE_EXTENSION_SEPARATOR '.'

/// @brief Return a pointer to the beginning of the extension, if one is found,
/// otherwise returns NULL.
/// @return char*? (&)
static char *
select_extension__File(const char *path);

char *
select_extension__File(const char *path)
{
    char *res = (char *)path;

    while (true) {
        // app.lily
        //    ^
        // ./a/b/c/app.ci
        // ^
        // ../a/b/c/d.c
        // ^
        // a/b/c/d.c.txt
        //        ^
        res = strchr(res, FILE_EXTENSION_SEPARATOR);

        if (res && strchr(res, DIR_SEPARATOR)) {
            ++res;

            continue;
        }

        return res;
    }
}

char *
get_extension__File(const char *path)
{
    const char *path_filename = select_extension__File(path);
    Usize size = 1;
    char *extension = lily_malloc(1);

    extension[0] = '\0';

    if (!path_filename) {
        return extension;
    }

    while (*path_filename) {
        extension = lily_realloc(extension, size + 1);
        extension[size - 1] = *(path_filename++);
        extension[size++] = '\0';
    }

    return extension;
}

bool
has_extension__File(const char *path, const char *extension)
{
    const char *path_filename = select_extension__File(path);

    return path_filename && !strcmp(path_filename, extension);
}

String *
get_filename__File(const char *path)
{
    // Extract the filename
    const char *path_filename = strrchr(path, DIR_SEPARATOR);

    if (path_filename) {
        ++path_filename;
    } else {
        path_filename = path;
    }

    String *filename = NEW(String);

    while (*path_filename != FILE_EXTENSION_SEPARATOR) {
        push__String(filename, *(path_filename++));
    }

    return filename;
}

String *
get_filename_with_extension__File(const char *path)
{
    // Extract the filename
    const char *path_filename = strrchr(path, DIR_SEPARATOR);

    if (path_filename) {
        ++path_filename;
    } else {
        path_filename = path;
    }

    String *filename = NEW(String);

    while (*path_filename) {
        push__String(filename, *(path_filename++));
    }

    return filename;
}

String *
get_dir__File(const char *path)
{
    // Extract the directory (path)
    const char *path_dir = strrchr(path, DIR_SEPARATOR);

    if (path_dir) {
        ++path_dir;
    } else {
        path_dir = path;
    }

    String *dir = NEW(String);

    for (char *current = (char *)path; current != path_dir; ++current) {
        push__String(dir, *current);
    }

    return dir;
}

String *
get_absolute_dir__File(const char *path)
{
    ASSERT(path);

    String *absolute_path = convert_to_absolute_path__Path(path);

    ASSERT(absolute_path);

    String *absolute_path_dir = get_dir__File(absolute_path->buffer);

    FREE(String, absolute_path);

    return absolute_path_dir;
}

Usize
get_size__File(const char *path)
{
    struct __stat__ st;
    __stat__(path, &st);
    return st.st_size;
}

#ifdef LILY_WINDOWS_OS
char *
read_file__File(const char *path)
{
    if (is_directory__File(path)) {
        printf("\x1b[31merror\x1b[0m: the file is a directory: `%s`\n", path);
        exit(1);
    }

    FILE *file = fopen(path, "r");

    if (!file) {
        printf("\x1b[31merror\x1b[0m: could not open file: `%s`\n", path);
        exit(1);
    }

    char *content = lily_malloc(1);
    content[0] = '\0';
    char ch;

    size_t len = 0;

    while ((ch = fgetc(file)) != EOF) {
        content = lily_realloc(content, len + 2);
        content[len++] = ch;
        content[len] = '\0';
    }

    fclose(file);

    content = lily_realloc(content, len + 2);
    strcat(content, "\n\0");

    return content;
}
#else
char *
read_file__File(const char *path)
{
    if (is__Dir(path)) {
        printf("\x1b[31merror\x1b[0m: the file is a directory: `%s`\n", path);
        exit(1);
    }

    FILE *file = fopen(path, "r");

    if (!file) {
        printf("\x1b[31merror\x1b[0m: could not open file: `%s`\n", path);
        exit(1);
    }

    struct __stat__ st;

    __stat__(path, &st);

    Usize size = st.st_size + 1;
    char *content = lily_malloc(size);

    memset(content, 0, size);
    fread(content, size, 1, file);

    ASSERT(feof(file));

    fclose(file);

    return content;
}
#endif

char *
read_file_in_cwd__File(const char *filename)
{
    char *cwd = get_cwd__Dir();

    if (!cwd) {
        printf("\x1b[31merror\x1b[0m: cannot get current working directory\n");
        exit(1);
    }

    char *path = format("{sa}/{s}", cwd, filename);
    char *content = read_file__File(path);

    lily_free(path);

    return content;
}

#ifdef LILY_WINDOWS_OS
void
write_file__File(const char *path, const char *content)
{
    // TODO: create a function to write on a file on Windows.
    return;
}
#else
void
write_file__File(const char *path, const char *content)
{
    FILE *file = fopen(path, "w");

    if (!file) {
        printf("\x1b[31merror\x1b[0m: could not open file: `%s`\n", path);
        exit(1);
    }

    fprintf(file, "%s", content);

    fclose(file);
}
#endif

bool
exists__File(const char *path)
{
#ifdef LILY_WINDOWS_OS
    return !_access(path, 0);
#else
    return access(path, F_OK) != -1;
#endif
}

String *
exists_rec__File(const char *path, const char *filename)
{
    Usize count_folder = count_c__Str(path, DIR_SEPARATOR);

    for (Usize i = 0; i < count_folder; ++i) {
        String *path_s =
          format__String("{s}" DIR_SEPARATOR_S "{Sr}" DIR_SEPARATOR_S "{s}",
                         path,
                         repeat__String(".." DIR_SEPARATOR_S, i),
                         filename);

        if (exists__File(path_s->buffer)) {
            return path_s;
        }

        FREE(String, path_s);
    }

    return NULL;
}
