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

#include <base/dir.h>

#include <core/lily/compiler/output/cache.h>

void
create_cache__LilyCompilerOutputCache()
{
    if (!exists__Dir(DIR_CACHE_NAME)) {
        create__Dir(DIR_CACHE_NAME,
                    DIR_MODE_RWXU | DIR_MODE_RWXG | DIR_MODE_RWXO);
    }

    if (!exists__Dir(DIR_CACHE_BIN)) {
        create__Dir(DIR_CACHE_BIN,
                    DIR_MODE_RWXU | DIR_MODE_RWXG | DIR_MODE_RWXO);
    }

    if (!exists__Dir(DIR_CACHE_LIB)) {
        create__Dir(DIR_CACHE_LIB,
                    DIR_MODE_RWXU | DIR_MODE_RWXG | DIR_MODE_RWXO);
    }

    if (!exists__Dir(DIR_CACHE_OBJ)) {
        create__Dir(DIR_CACHE_OBJ,
                    DIR_MODE_RWXU | DIR_MODE_RWXG | DIR_MODE_RWXO);
    }
}
