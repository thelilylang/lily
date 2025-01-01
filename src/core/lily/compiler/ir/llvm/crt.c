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

#include <core/lily/compiler/ir/llvm/crt.h>

#if defined(LILY_LINUX_OS) || defined(LILY_BSD_OS)

#include <base/alloc.h>
#include <base/file.h>

#include <pthread.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define DIRECTORIES_LEN 5

#define SEARCH_CRT0_OR_CRT1(file, path, has)                  \
    pthread_mutex_lock(&crt_mutex);                           \
                                                              \
    for (Usize i = 0; i < DIRECTORIES_LEN && !path; ++i) {    \
        String *res = exists_rec__File(directories[i], file); \
                                                              \
        if (res) {                                            \
            path = res->buffer;                               \
            has = true;                                       \
                                                              \
            lily_free(res);                                   \
                                                              \
            break;                                            \
        }                                                     \
    }                                                         \
                                                              \
    pthread_mutex_unlock(&crt_mutex);                         \
                                                              \
    return path;

#define SEARCH_OTHER_CRT(file, path)                          \
    pthread_mutex_lock(&crt_mutex);                           \
                                                              \
    for (Usize i = 0; i < DIRECTORIES_LEN && !path; ++i) {    \
        String *res = exists_rec__File(directories[i], file); \
                                                              \
        if (res) {                                            \
            path = res->buffer;                               \
                                                              \
            lily_free(res);                                   \
                                                              \
            break;                                            \
        }                                                     \
    }                                                         \
                                                              \
    pthread_mutex_unlock(&crt_mutex);                         \
                                                              \
    if (!path) {                                              \
        FAILED(file " is not found");                         \
    }                                                         \
                                                              \
    return path;

static const char *directories[DIRECTORIES_LEN] = { "/usr/lib",
                                                    "/usr/lib/x86_64-linux-gnu",
                                                    "/lib",
                                                    "/usr/local/lib",
                                                    "/usr/lib/gcc" };

// This mutex prevents crt files (crt0.o, crt1.o, crti.o, crtn.o) from being
// searched twice in two different threads.
static pthread_mutex_t crt_mutex = PTHREAD_MUTEX_INITIALIZER;

static char *crt0_path = NULL;
static char *crt1_path = NULL;
static char *crti_path = NULL;
static char *crtn_path = NULL;

static bool has_crt0 = false;
static bool has_crt1 = false;

const char *
get_crt0_library_path__LilyIrLlvmLinker()
{
    if (crt0_path) {
        return crt0_path;
    } else if (has_crt1) {
        return NULL;
    }

    SEARCH_CRT0_OR_CRT1("crt0.o", crt0_path, has_crt0);
}

const char *
get_crt1_library_path__LilyIrLlvmLinker()
{
    if (crt1_path) {
        return crt1_path;
    } else if (has_crt0) {
        return NULL;
    }

    SEARCH_CRT0_OR_CRT1("crt1.o", crt1_path, has_crt1);
}

const char *
get_crti_library_path__LilyIrLlvmLinker()
{
    if (crti_path) {
        return crti_path;
    }

    SEARCH_OTHER_CRT("crti.o", crti_path);
}

const char *
get_crtn_library_path__LilyIrLlvmLinker()
{
    if (crtn_path) {
        return crtn_path;
    }

    SEARCH_OTHER_CRT("crtn.o", crtn_path);
}

void
destroy_crt__LilyIrLlvmLinker()
{
    pthread_mutex_destroy(&crt_mutex);

    if (crt0_path) {
        lily_free(crt0_path);
    }

    if (crt1_path) {
        lily_free(crt1_path);
    }

    if (crti_path) {
        lily_free(crti_path);
    }

    if (crtn_path) {
        lily_free(crtn_path);
    }
}

#endif
