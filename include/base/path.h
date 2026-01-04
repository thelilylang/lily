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

#ifndef LILY_BASE_PATH_H
#define LILY_BASE_PATH_H

#include <base/dir_separator.h>
#include <base/platform.h>
#include <base/string.h>

/**
 *
 * @brief Check if the given path is relative.
 */
inline bool
is_relative__Path(const char *path)
{
#ifdef LILY_WINDOWS_OS
    return isalpha(path[0]) && path[1] == ':' && path[2] == DIR_SEPARATOR;
#else
    return path[0] != DIR_SEPARATOR;
#endif
}

/**
 *
 * @brief Convert relative to absolute path.
 * @return String*
 */
String *
convert_to_absolute_path__Path(const char *path);

#endif // LILY_BASE_PATH_H
