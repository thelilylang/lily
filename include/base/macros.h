/*
 * MIT License
 *
 * Copyright (c) 2022-2024 ArthurPV
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

#define ENV_ASSERT
#undef ENV_ASSERT

#define ENV_DEBUG
#undef ENV_DEBUG

#define ENV_INSTALL
#undef ENV_INSTALL

#define ENV_LOCAL
#undef ENV_LOCAL

#define ENV_SAFE
#undef ENV_SAFE

#if defined(__GNUC__) && !defined(__clang_major__)
#define GCC_VERSION \
    (__GNUC__ * 1000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#elif defined(__clang_major__)
#define CLANG_VERSION \
    (__clang_major__ * 1000 + __clang_minor__ * 100 + __clang_patchlevel__)
#elif defined(_MSC_VER)
#define MSVC_VERSION _MSC_VER
#else
#error \
  "unknown compiler, please add a support for this compiler or open issue at 'https://github.com/thelilylang/lily/tree/main'."
#endif

#if defined(GCC_VERSION) || defined(CLANG_VERSION) || defined(MSVC_VERSION)

#if defined(GCC_VERSION)
#if GCC_VERSION < 4600
#error "this version of GCC is not yet supported."
#endif

#elif defined(CLANG_VERSION)
#if CLANG_VERSION < 3000
#error "this version of clang is not yet supported."
#endif

#elif defined(MSVC_VERSION)
#if MSVC_VERSION < 1928
#error "this version of MSCV is not yet supported."
#endif

#else
#error \
  "unknown compiler, please add a support for this compiler or open issue at 'https://github.com/thelilylang/lily/tree/main'."
#endif

#endif

#ifndef CLANG_VERSION
#warning "Only Clang version >= 15 is fully supported."
#endif

#define PTR_SIZE sizeof(void *)
#define DEFAULT_ALIGNMENT 0

// Vec<T>
// FREE_BUFFER_ITEMS(buffer, len, type)
// NOTE: Please do not add parentheses where parameters are passed, as this is a
// critical macro and could cause bugs.
#define FREE_BUFFER_ITEMS(b, l, t)     \
    for (Usize x1 = 0; x1 < l; ++x1) { \
        __free__##t(b[x1]);            \
    }

// Vec<Vec<T>>
// FREE_BUFFER_ITEMS_2(buffer, len, type)
// NOTE: Please do not add parentheses where parameters are passed, as this is a
// critical macro and could cause bugs.
#define FREE_BUFFER_ITEMS_2(b, l, t)                   \
    for (Usize x2 = 0; x2 < l; ++x2) {                 \
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

#define ARRAY(...) { __VA_ARGS__ }
#define LEN(arr, item) (((int)sizeof(arr) / sizeof(item)))

// MOVE(pointer1, pointer2)
#define MOVE(p1, p2) \
    p2 = p1;         \
    p1 = NULL

// FREE_MOVE(pointer, free)
#define FREE_MOVE(p, f) \
    if (p) {            \
        f;              \
    }

#define SWAP(x, y)   \
    {                \
        void *p = x; \
        x = y;       \
        y = p;       \
    }

#define STR(s) #s

#ifdef ENV_DEBUG
#define IMPL_FOR_DEBUG(name, struct_name, ...) \
    name##__Debug__##struct_name(__VA_ARGS__)
#define CALL_DEBUG_IMPL(name, struct_name, ...) \
    name##__Debug__##struct_name(__VA_ARGS__)
#define CALL_DEBUG(name, ...) debug__Debug__##name(__VA_ARGS__)
#endif

#define TEST(type, name, ...) type name##__Test__(__VA_ARGS__)
#define CALL_TEST(name, ...) name##__Test__(__VA_ARGS__)

#define DBG(msg)                                                \
    {                                                           \
        printf("DEBUG: %s (%s:%d)\n", msg, __FILE__, __LINE__); \
        exit(1);                                                \
    }

#define TODO(msg)                                              \
    {                                                          \
        printf("TODO: %s (%s:%d)\n", msg, __FILE__, __LINE__); \
        exit(0);                                               \
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

#define ALIGNMENT_COEFF 2

#if defined(CLANG_VERSION) || defined(MSCV_VERSION)
#define alignof(T) _Alignof(T)
#endif

#define ALIGN(mem, align) (((Uptr)mem + (align - 1)) & ~(align - 1));

#define threadlocal _Thread_local

#define EXIT_OK EXIT_SUCCESS
#define EXIT_ERR EXIT_FAILURE

#endif // LILY_BASE_MACROS_H
