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

#include <base/alloc.h>
#include <base/assert.h>
#include <base/dir.h>
#include <base/file.h>
#include <base/macros.h>
#include <base/new.h>
#include <base/platform.h>
#include <base/sys.h>
#include <base/types.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef LILY_WINDOWS_OS
#include <windows.h>
#endif

char *
get_extension__File(const char *path)
{
    bool extension_started = false;
    Usize size = 1;
    char *extension = lily_malloc(1);
    Usize len = strlen(path);

    for (Usize i = 0; i < len; i++) {
        if (path[i] == '.' && path[i + 1 < len ? i + 1 : i] != '.' &&
            path[i + 1 < len ? i + 1 : i] != '/' &&
            path[i + 1 < len ? i + 1 : i] != '\\') {
            extension_started = true;
        }

        if (extension_started) {
            extension = lily_realloc(extension, size + 1);
            extension[size - 1] = path[i];
            extension[size++] = '\0';
        }
    }

    return extension;
}

String *
get_filename__File(const char *path)
{
    // Extract the filename
#ifdef LILY_WINDOWS_OS
    const char *filename_with_ext = strrchr(path, '\\');
#else
    const char *filename_with_ext = strrchr(path, '/');
#endif

    if (filename_with_ext) {
        ++filename_with_ext;
    }

    String *filename = NEW(String);

    while (*filename_with_ext != '.') {
        push__String(filename, *(filename_with_ext++));
    }

    return filename;
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

    Usize size = st.st_size + 2;
    char *content = lily_malloc(size);

    memset(content, 0, size);
    fread(content, size, 1, file);

    content[size - 1] = '\n';

    fclose(file);

    return content;
}
#endif

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
