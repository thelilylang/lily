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

#ifndef LILY_BASE_DIR_H
#define LILY_BASE_DIR_H

#include <base/vec.h>

#include <stdbool.h>

// NOTE: Only useful on Unix operating systems
enum DirMode
{
    DIR_MODE_RWXU = 0700,
    DIR_MODE_RUSR = 0400,
    DIR_MODE_WUSR = 0200,
    DIR_MODE_XUSR = 0100,
    DIR_MODE_RWXG = 0070,
    DIR_MODE_RGRP = 0040,
    DIR_MODE_WGRP = 0020,
    DIR_MODE_XGRP = 0010,
    DIR_MODE_RWXO = 0007,
    DIR_MODE_ROTH = 0004,
    DIR_MODE_WOTH = 0002,
    DIR_MODE_XOTH = 0001,
    DIR_MODE_SUID = 4000,
    DIR_MODE_SGID = 2000,
    DIR_MODE_SVTX = 1000
};

/**
 *
 * @brief Create directory according the mode.
 */
void
create__Dir(const char *path, [[maybe_unused]] enum DirMode mode);

/**
 *
 * @brief Create directories (recursive) according the mode.
 */
void
create_recursive_dir__Dir(const char *path, [[maybe_unused]] enum DirMode mode);

/**
 *
 * @brief Remove directory.
 */
void
remove__Dir(const char *path);

/**
 *
 * @brief Check if the path is a directory.
 */
bool
is__Dir(const char *path);

/**
 *
 * @brief Check if the directory exists.
 */
bool
exists__Dir(const char *path);

/**
 *
 * @brief Get current working directory.
 * @return char*?
 */
char *
get_cwd__Dir();

/**
 *
 * @brief Gets all files in the folder recursively.
 * @return Vec<String*>*?
 */
Vec *
get_files_rec__Dir(const char *path);

#endif // LILY_BASE_DIR_H
