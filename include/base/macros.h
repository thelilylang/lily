/*
 * MIT License
 *
 * Copyright (c) 2022-2023 ArthurPV
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

#ifndef LILY_BASE_MACROS_H
#define LILY_BASE_MACROS_H

#define ENV_DEBUG
#undef ENV_DEBUG

#define ENV_INSTALL
#undef ENV_INSTALL

#define ENV_LOCAL
#undef ENV_LOCAL

#define ENV_SAFE
#undef ENV_SAFE

#if defined(__GNUC__)
#define GCC_VERSION \
    (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#elif defined(__clang_major__)
#define CLANG_VERSION \
    (__clang_major__ * 1000 + __clang_minor__ * 100 + __clang_patchlevel__)
#else
#error "unknown compiler, please add a support for this compiler."
#endif

#define PTR_SIZE sizeof(void *)

#define FREE_BUFFER_ITEMS(buffer, len, type) \
    for (Usize i = 0; i < len; i++) {        \
        __free__##type(buffer[i]);           \
    }

#define CAST(type, expr) ((type)(expr))

#define CONSTRUCTOR(type, name, ...) type __new__##name(__VA_ARGS__)
#define DESTRUCTOR(name, ...) void __free__##name(__VA_ARGS__)

#define VARIANT_CONSTRUCTOR(type, name, variant, ...) \
    type __new__##name##__##variant(__VA_ARGS__)

#define VARIANT_DESTRUCTOR(name, variant, ...) \
    void __free__##name##__##variant(__VA_ARGS__)

#define LEN(arr, item) (((int)sizeof(arr) / sizeof(item)) + 1)

#define DBG(msg)                                                \
    {                                                           \
        printf("DEBUG: %s (%s:%d)\n", msg, __FILE__, __LINE__); \
        exit(1);                                                \
    }

#define TODO(msg)                                              \
    {                                                          \
        printf("TODO: %s (%s:%d)\n", msg, __FILE__, __LINE__); \
        exit(1);                                               \
    }

#define FAILED(msg)                                              \
    {                                                            \
        printf("FAILED: %s (%s:%d)\n", msg, __FILE__, __LINE__); \
        exit(1);                                                 \
    }

#define UNREACHABLE(msg)                                              \
    {                                                                 \
        printf("UNREACHABLE: %s (%s:%d)\n", msg, __FILE__, __LINE__); \
        exit(1);                                                      \
    }

#endif // LILY_BASE_MACROS_H
