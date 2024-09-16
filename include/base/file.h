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

#ifndef LILY_BASE_FILE_H
#define LILY_BASE_FILE_H

#include <stdbool.h>

#include <base/string.h>
#include <base/types.h>

/**
 *
 * @brief Get extension of the path.
 */
char *
get_extension__File(const char *path);

/**
 *
 * @brief Check if the path extension has the same of given extension.
 */
bool
has_extension__File(const char *path, const char *extension);

/**
 *
 * @brief Get filename (without extension) of the path.
 * @return String*
 * @example a/b/c/file.txt => file
 */
String *
get_filename__File(const char *path);

/**
 *
 * @brief Get filename of the path.
 * @return String*
 * @example a/b/c/file.txt => file.txt
 */
String *
get_filename_with_extension__File(const char *path);

/**
 *
 * @brief Get directory (e.g. a/b, a, ...) of the path.
 * @return String*
 * @example a/b/c/file.txt => a/b/c
 */
String *
get_dir__File(const char *path);

/**
 *
 * @brief Get size of the file.
 */
Usize
get_size__File(const char *path);

/**
 *
 * @brief Read file content.
 */
char *
read_file__File(const char *path);

/**
 *
 * @brief Read file content in current working directory.
 */
char *
read_file_in_cwd__File(const char *filename);

/**
 *
 * @brief Write content on a file.
 */
void
write_file__File(const char *path, const char *content);

/**
 *
 * @brief Check if the file exists.
 * @return Return true if the file exist otherwise return false.
 */
bool
exists__File(const char *path);

/**
 *
 * @brief Check whether the file exists in the current and previous folders.
 * @return String*?
 */
String *
exists_rec__File(const char *path, const char *filename);

#endif // LILY_BASE_FILE_H
