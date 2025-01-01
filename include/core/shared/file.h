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

#ifndef LILY_CORE_SHARED_FILE_H
#define LILY_CORE_SHARED_FILE_H

#include <base/alloc.h>
#include <base/file.h>
#include <base/macros.h>
#include <base/types.h>

#include <stdlib.h>
#include <string.h>

typedef struct File
{
    char *name;
    char *content;
    Usize len; // length of the content
} File;

/**
 *
 * @brief Construct File type.
 */
inline CONSTRUCTOR(File, File, char *name, char *content)
{
    return (File){ .name = name,
                   .content = content,
                   .len = get_size__File(name) + 1 };
}

/**
 *
 * @brief Free File type.
 */
inline DESTRUCTOR(File, const File *self)
{
    lily_free(self->content);
}

#endif // LILY_CORE_SHARED_FILE_H
