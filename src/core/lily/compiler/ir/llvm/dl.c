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

#include <core/lily/compiler/ir/llvm/dl.h>

#if defined(LILY_LINUX_OS) || defined(LILY_BSD_OS)

#include <base/macros.h>

#define _GNU_SOURCE

#include <dlfcn.h>
#include <link.h>
#include <stdio.h>
#include <stdlib.h>

const char *
get_library_path__LilyIrLlvmLinker(const char *library)
{
    void *loaded;
    struct link_map *map;
    int rc;

    loaded = dlopen(library, RTLD_LAZY);

    if (!loaded) {
        FAILED("dlopen failed");
    }

    rc = dlinfo(loaded, RTLD_DI_LINKMAP, &map);

    if (rc) {
        dlclose(loaded);
        FAILED("dlinfo failed");
    }

    const char *library_path = map->l_name;

    dlclose(loaded);

    return library_path;
}

#endif
