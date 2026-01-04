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

#ifndef LILY_CORE_LILY_PACKAGE_LIBRARY_H
#define LILY_CORE_LILY_PACKAGE_LIBRARY_H

#include <base/macros.h>
#include <base/string.h>

#include <core/lily/compiler/ar/ar.h>

typedef struct LilyPackage LilyPackage;

typedef struct LilyLibrary
{
    String *name;      // String* (&)
    String *version;   // String*?
    String *url;       // String*?; URL to download the library
    String *path;      // String*? path (in the PC) to find the library
    char *output_path; // char*? e.g. *.a, *.lib, ...
    enum LilyArKind ar;
    LilyPackage *package;
} LilyLibrary;

/**
 *
 * @brief Construct LilyLibrary type.
 */
CONSTRUCTOR(LilyLibrary *,
            LilyLibrary,
            String *version,
            String *url,
            String *path,
            LilyPackage *package);

/**
 *
 * @brief Finish to set all values (name and ar).
 * @param name String* (&)
 */
inline void
finish_set__LilyLibrary(LilyLibrary *self, String *name, enum LilyArKind ar)
{
    self->name = name;
    self->ar = ar;
}

/**
 *
 * @brief Free LilyLibrary type.
 */
DESTRUCTOR(LilyLibrary, LilyLibrary *self);

#endif // LILY_CORE_LILY_PACKAGE_LIBRARY_H
