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

#ifndef LILY_CORE_LILY_COMPILER_OUTPUT_CACHE_H
#define LILY_CORE_LILY_COMPILER_OUTPUT_CACHE_H

#include <base/macros.h>
#include <base/platform.h>

#ifdef LILY_WINDOWS_OS
#define DIR_CACHE_NAME "out.lily\\"
#define DIR_CACHE_BIN DIR_CACHE_NAME "bin\\"
#define DIR_CACHE_LIB DIR_CACHE_NAME "lib\\"
#define DIR_CACHE_OBJ DIR_CACHE_NAME "obj\\"
#else
#define DIR_CACHE_NAME "out.lily/"
#define DIR_CACHE_BIN DIR_CACHE_NAME "bin/"
#define DIR_CACHE_LIB DIR_CACHE_NAME "lib/"
#define DIR_CACHE_OBJ DIR_CACHE_NAME "obj/"
#endif

/**
 *
 * @brief Create cache.
 *
 * e.g.
 * out.lily/
 * ├── bin
 * ├── lib
 * └── obj
 */
void
create_cache__LilyCompilerOutputCache();

#endif // LILY_CORE_LILY_COMPILER_OUTPUT_CACHE_H
