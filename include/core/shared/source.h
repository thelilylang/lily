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

#ifndef LILY_CORE_SHARED_SOURCE_H
#define LILY_CORE_SHARED_SOURCE_H

#include <base/new.h>

#include <core/shared/cursor.h>
#include <core/shared/file.h>

typedef struct Source
{
    Cursor cursor;
    const File *file; // const File* (&)
} Source;

/**
 *
 * @brief Construct Source type.
 */
inline CONSTRUCTOR(Source, Source, Cursor cursor, const File *file)
{
    return (Source){ .cursor = cursor, .file = file };
}

/**
 *
 * @brief Move next one character.
 */
void
next_char__Source(Source *self);

/**
 *
 * @brief Move back one character.
 */
void
previous_char__Source(Source *self);

#endif // LILY_CORE_SHARED_SOURCE_H
