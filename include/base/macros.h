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

// Vec<T>
// FREE_BUFFER_ITEMS(buffer, len, type)
#define FREE_BUFFER_ITEMS(b, l, t)     \
    for (Usize x1 = 0; x1 < l; x1++) { \
        __free__##t(b[x1]);            \
    }

// Vec<Vec<T>>
// FREE_BUFFER_ITEMS_2(buffer, len, type)
#define FREE_BUFFER_ITEMS_2(b, l, t)                   \
    for (Usize x2 = 0; x2 < l; x2++) {                 \
        Vec *item = b[x2];                             \
        FREE_BUFFER_ITEMS(item->buffer, item->len, t); \
        FREE(Vec, item);                               \
    }

#define CAST(type, expr) ((type)(expr))

#define CONSTRUCTOR(type, name, ...) type __new__##name(__VA_ARGS__)
#define DESTRUCTOR(name, ...) void __free__##name(__VA_ARGS__)

#define VARIANT_CONSTRUCTOR(type, name, variant, ...) \
    type __new__##name##__##variant(__VA_ARGS__)

#define VARIANT_DESTRUCTOR(name, variant, ...) \
    void __free__##name##__##variant(__VA_ARGS__)

#define LEN(arr, item) (((int)sizeof(arr) / sizeof(item)))

#define MOVE(p1, p2) \
	p2 = p1; \
	p1 = NULL

#ifdef ENV_DEBUG
#define IMPL_FOR_DEBUG(name, struct_name, ...) \
    name##__Debug__##struct_name(__VA_ARGS__)
#define CALL_DEBUG_IMPL(name, struct_name, ...) \
    name##__Debug__##struct_name(__VA_ARGS__)
#define CALL_DEBUG(name, ...) debug__Debug__##name(__VA_ARGS__)
#endif

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
