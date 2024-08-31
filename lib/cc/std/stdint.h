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

/*
 * ISO C99 Standard: 7.18 Integer types <stdint.h> 
 */

#ifndef _CC_STDINT_H
#define _CC_STDINT_H

/* 7.18.1.1 Exact-width integer types */

#ifdef __INT8_TYPE__
typedef __INT8_TYPE__ int8_t;
#endif /* __INT8_TYPE__ */

#ifdef __INT16_TYPE__
typedef __INT16_TYPE__ int16_t;
#endif /* __INT16_TYPE__ */

#ifdef __INT32_TYPE__
typedef __INT32_TYPE__ int32_t;
#endif /* __INT32_TYPE__ */

#ifdef __INT64_TYPE__
typedef __INT64_TYPE__ int64_t;
#endif /* __INT64_TYPE__ */

#ifdef __UINT8_TYPE__
typedef __UINT8_TYPE__ uint8_t;
#endif /* __UINT8_TYPE__ */

#ifdef __UINT16_TYPE__
typedef __UINT16_TYPE__ uint16_t;
#endif /* __UINT16_TYPE__ */

#ifdef __UINT32_TYPE__
typedef __UINT32_TYPE__ uint32_t;
#endif /* __UINT32_TYPE__ */

#ifdef __UINT64_TYPE__
typedef __UINT64_TYPE__ uint64_t;
#endif /* __UINT64_TYPE__ */

/* 7.18.1.2 Minimum-width integer types */

#ifdef __INT_LEAST8_TYPE__
typedef __INT_LEAST8_TYPE__ int_least8_t;
#endif /* __INT_LEAST8_TYPE__ */

#ifdef __INT_LEAST16_TYPE__
typedef __INT_LEAST16_TYPE__ int_least16_t;
#endif /* __INT_LEAST16_TYPE__ */

#ifdef __INT_LEAST32_TYPE__
typedef __INT_LEAST32_TYPE__ int_least32_t;
#endif /* __INT_LEAST32_TYPE__ */

#ifdef __INT_LEAST64_TYPE__
typedef __INT_LEAST64_TYPE__ int_least64_t;
#endif /* __INT_LEAST64_TYPE__ */

#ifdef __UINT_LEAST8_TYPE__
typedef __UINT_LEAST8_TYPE__ uint_least8_t;
#endif /* __UINT_LEAST8_TYPE__ */

#ifdef __UINT_LEAST16_TYPE__
typedef __UINT_LEAST16_TYPE__ uint_least16_t;
#endif /* __UINT_LEAST16_TYPE__ */

#ifdef __UINT_LEAST32_TYPE__
typedef __UINT_LEAST32_TYPE__ uint_least32_t;
#endif /* __UINT_LEAST32_TYPE__ */

#ifdef __UINT_LEAST64_TYPE__
typedef __UINT_LEAST64_TYPE__ uint_least64_t;
#endif /* __UINT_LEAST64_TYPE__ */

/* 7.18.1.3 Fastest minimum-width integer types */

#ifdef __INT_FAST8_TYPE__
typedef __INT_FAST8_TYPE__ int_fast8_t;
#endif /* __INT_FAST8_TYPE__ */

#ifdef __INT_FAST16_TYPE__
typedef __INT_FAST16_TYPE__ int_fast16_t;
#endif /* __INT_FAST16_TYPE__ */

#ifdef __INT_FAST32_TYPE__
typedef __INT_FAST32_TYPE__ int_fast32_t;
#endif /* __INT_FAST32_TYPE__ */

#ifdef __INT_FAST64_TYPE__
typedef __INT_FAST64_TYPE__ int_fast64_t;
#endif /* __INT_FAST64_TYPE__ */

#ifdef __UINT_FAST8_TYPE__
typedef __UINT_FAST8_TYPE__ uint_fast8_t;
#endif /* __UINT_FAST8_TYPE__ */

#ifdef __UINT_FAST16_TYPE__
typedef __UINT_FAST16_TYPE__ uint_fast16_t;
#endif /* __UINT_FAST16_TYPE__ */

#ifdef __UINT_FAST32_TYPE__
typedef __UINT_FAST32_TYPE__ uint_fast32_t;
#endif /* __UINT_FAST32_TYPE__ */

#ifdef __UINT_FAST64_TYPE__
typedef __UINT_FAST64_TYPE__ uint_fast64_t;
#endif /* __UINT_FAST64_TYPE__ */

/* 7.18.1.4 Integer types capable of holding object pointers */

#ifdef __INTPTR_TYPE__
typedef __INTPTR_TYPE__ intptr_t;
#endif /* __INTPTR_TYPE__ */

#ifdef __UINTPTR_TYPE__
typedef __UINTPTR_TYPE__ uintptr_t;
#endif /* __UINTPTR_TYPE__ */

/* 7.18.1.5 Greatest-width integer types */

#ifdef __INTMAX_TYPE__
typedef __INTMAX_TYPE__ intmax_t;
#endif /* __INTMAX_TYPE__ */

#ifdef __UINTMAX_TYPE__
typedef __UINTMAX_TYPE__ uintmax_t;
#endif /* __UINTMAX_TYPE__ */

/* 7.18.2.1 Limits of exact-width integer types */

#ifdef __INT8_MAX__
#undef INT8_MAX
#undef INT8_MIN

#define INT8_MAX __INT8_MAX__
#define INT8_MIN (-INT8_MAX - 1)
#endif /* __INT8_MAX__ */

#ifdef __INT16_MAX__
#undef INT16_MAX
#undef INT16_MIN

#define INT16_MAX __INT16_MAX__
#define INT16_MIN (-INT16_MAX - 1)
#endif /* __INT16_MAX__ */

#ifdef __INT32_MAX__
#undef INT32_MAX
#undef INT32_MIN

#define INT32_MAX __INT32_MAX__
#define INT32_MIN (-INT32_MAX - 1)
#endif /* __INT32_MAX__ */

#ifdef __INT64_MAX__
#undef INT64_MAX
#undef INT64_MIN

#define INT64_MAX __INT64_MAX__
#define INT64_MIN (-INT64_MAX - 1)
#endif /* __INT64_MAX__ */

#ifdef __UINT8_MAX__
#undef UINT8_MAX 

#define UINT8_MAX __UINT8_MAX__
#endif /* __UINT8_MAX__ */

#ifdef __UINT16_MAX__
#undef UINT16_MAX 

#define UINT16_MAX __UINT16_MAX__
#endif /* __UINT16_MAX__ */

#ifdef __UINT32_MAX__
#undef UINT32_MAX 

#define UINT32_MAX __UINT32_MAX__
#endif /* __UINT32_MAX__ */

#ifdef __UINT64_MAX__
#undef UINT64_MAX 

#define UINT64_MAX __UINT64_MAX__
#endif /* __UINT64_MAX__ */

/* 7.18.2.2 Limits of minimum-width integer types */

#ifdef __INT_LEAST8_MAX__
#undef INT_LEAST8_MAX
#undef INT_LEAST8_MIN

#define INT_LEAST8_MAX __INT_LEAST8_MAX__
#define INT_LEAST8_MIN (-INT_LEAST8_MAX - 1)
#endif /* __INT_LEAST8_MAX__ */

#ifdef __INT_LEAST16_MAX__
#undef INT_LEAST16_MAX
#undef INT_LEAST16_MIN

#define INT_LEAST16_MAX __INT_LEAST16_MAX__
#define INT_LEAST16_MIN (-INT_LEAST16_MAX - 1)
#endif /* __INT_LEAST16_MAX__ */

#ifdef __INT_LEAST16_MAX__
#undef INT_LEAST16_MAX
#undef INT_LEAST16_MIN

#define INT_LEAST16_MAX __INT_LEAST16_MAX__
#define INT_LEAST16_MIN (-INT_LEAST16_MAX - 1)
#endif /* __INT_LEAST16_MAX__ */

#ifdef __INT_LEAST32_MAX__
#undef INT_LEAST32_MAX
#undef INT_LEAST32_MIN

#define INT_LEAST32_MAX __INT_LEAST32_MAX__
#define INT_LEAST32_MIN (-INT_LEAST32_MAX - 1)
#endif /* __INT_LEAST32_MAX__ */

#ifdef __INT_LEAST64_MAX__
#undef INT_LEAST64_MAX
#undef INT_LEAST64_MIN

#define INT_LEAST64_MAX __INT_LEAST64_MAX__
#define INT_LEAST64_MIN (-INT_LEAST64_MAX - 1)
#endif /* __INT_LEAST64_MAX__ */

#ifdef __UINT_LEAST8_MAX__
#undef UINT_LEAST8_MAX

#define UINT_LEAST8_MAX __UINT_LEAST8_MAX__
#endif /* __UINT_LEAST8_MAX__ */

#ifdef __UINT_LEAST16_MAX__
#undef UINT_LEAST16_MAX

#define UINT_LEAST16_MAX __UINT_LEAST16_MAX__
#endif /* __UINT_LEAST16_MAX__ */

#ifdef __UINT_LEAST32_MAX__
#undef UINT_LEAST32_MAX

#define UINT_LEAST32_MAX __UINT_LEAST32_MAX__
#endif /* __UINT_LEAST32_MAX__ */

#ifdef __UINT_LEAST64_MAX__
#undef UINT_LEAST64_MAX

#define UINT_LEAST64_MAX __UINT_LEAST64_MAX__
#endif /* __UINT_LEAST64_MAX__ */

/* 7.18.2.3 Limits of fastest minimum-width integer types */

#ifdef __INT_FAST8_MAX__
#undef INT_FAST8_MAX
#undef INT_FAST8_MIN

#define INT_FAST8_MAX __INT_FAST8_MAX__
#define INT_FAST8_MIN (-INT_FAST8_MAX - 1)
#endif /* __INT_FAST8_MAX__ */

#ifdef __INT_FAST16_MAX__
#undef INT_FAST16_MAX
#undef INT_FAST16_MIN

#define INT_FAST16_MAX __INT_FAST16_MAX__
#define INT_FAST16_MIN (-INT_FAST16_MAX - 1)
#endif /* __INT_FAST16_MAX__ */

#ifdef __INT_FAST32_MAX__
#undef INT_FAST32_MAX
#undef INT_FAST32_MIN

#define INT_FAST32_MAX __INT_FAST32_MAX__
#define INT_FAST32_MIN (-INT_FAST32_MAX - 1)
#endif /* __INT_FAST32_MAX__ */

#ifdef __INT_FAST64_MAX__
#undef INT_FAST64_MAX
#undef INT_FAST64_MIN

#define INT_FAST64_MAX __INT_FAST64_MAX__
#define INT_FAST64_MIN (-INT_FAST64_MAX - 1)
#endif /* __INT_FAST64_MAX__ */

#ifdef __UINT_FAST8_MAX__
#undef UINT_FAST8_MAX

#define UINT_FAST8_MAX __UINT_FAST8_MAX__
#endif /* __UINT_FAST8_MAX__ */

#ifdef __UINT_FAST16_MAX__
#undef UINT_FAST16_MAX

#define UINT_FAST16_MAX __UINT_FAST16_MAX__
#endif /* __UINT_FAST16_MAX__ */

#ifdef __UINT_FAST32_MAX__
#undef UINT_FAST32_MAX

#define UINT_FAST32_MAX __UINT_FAST32_MAX__
#endif /* __UINT_FAST32_MAX__ */

#ifdef __UINT_FAST64_MAX__
#undef UINT_FAST64_MAX

#define UINT_FAST64_MAX __UINT_FAST64_MAX__
#endif /* __UINT_FAST64_MAX__ */

/* 7.18.2.4 Limits of integer types capable of holding object pointers */

#ifdef __INTPTR_MAX__
#undef INTPTR_MAX
#undef INTPTR_MIN

#define INTPTR_MAX __INTPTR_MAX__
#define INTPTR_MIN (-INTPTR_MAX - 1)
#endif /* __INTPTR_MAX__ */

#ifdef __UINTPTR_MAX__
#undef UINTPTR_MAX

#define UINTPTR_MAX __UINTPTR_MAX__
#endif /* __UINTPTR_MAX__ */

/* 7.18.2.5 Limits of greatest-width integer types */

#ifdef __INTMAX_MAX__
#undef INTMAX_MAX
#undef INTMAX_MIN

#define INTMAX_MAX __INTMAX_MAX__
#define INTMAX_MIN (-INTMAX_MAX - 1)
#endif /* __INTMAX_MAX__ */

#ifdef __UINTMAX_MAX__
#undef UINTMAX_MAX

#define UINTMAX_MAX __UINTMAX_MAX__
#endif /* __UINTMAX_MAX__ */

/* 7.18.3 Limits of other integer types */

#ifdef __PTRDIFF_MAX__
#undef PTRDIFF_MAX
#undef PTRDIFF_MIN

#define PTRDIFF_MAX __PTRDIFF_MAX__
#define PTRDIFF_MIN (-PTRDIFF_MAX - 1)
#endif /* __PTRDIFF_MAX__ */

#ifdef __SIG_ATOMIC_MAX__
#undef SIG_ATOMIC_MAX
#undef SIG_ATOMIC_MIN

#define SIG_ATOMIC_MAX __SIG_ATOMIC_MAX__
#define SIG_ATOMIC_MIN (-SIG_ATOMIC_MAX - 1)
#endif /* __SIG_ATOMIC_MAX__ */

#ifdef __SIZE_MAX__
#undef SIZE_MAX

#define SIZE_MAX __SIZE_MAX__
#endif /* __SIZE_MAX__ */

#ifdef __WCHAR_MAX__
#undef WCHAR_MAX
#undef WCHAR_MIN

#define WCHAR_MAX __WCHAR_MAX__
#define WCHAR_MIN (-WCHAR_MAX - 1)
#endif /* __WCHAR_MAX__ */

#ifdef __WINT_MAX__
#undef WINT_MAX
#undef WINT_MIN

#define WINT_MAX __WINT_MAX__
#define WINT_MIN (-WINT_MAX - 1)
#endif /* __WINT_MAX__ */

/* 7.18.4.1 Macros for minimum-width integer constants */

#ifdef __INT8_C
#define INT8_C(c) __INT8_C(c)
#endif /* __INT8_C */

#ifdef __INT16_C
#define INT16_C(c) __INT16_C(c)
#endif /* __INT16_C */

#ifdef __INT32_C
#define INT32_C(c) __INT32_C(c)
#endif /* __INT32_C */

#ifdef __INT64_C
#define INT64_C(c) __INT64_C(c)
#endif /* __INT64_C */

#ifdef __UINT8_C
#define UINT8_C(c) __UINT8_C(c)
#endif /* __UINT8_C */

#ifdef __UINT16_C
#define UINT16_C(c) __UINT16_C(c)
#endif /* __UINT16_C */

#ifdef __UINT32_C
#define UINT32_C(c) __UINT32_C(c)
#endif /* __UINT32_C */

#ifdef __UINT64_C
#define UINT64_C(c) __UINT64_C(c)
#endif /* __UINT64_C */

/* 7.18.4.2 Macros for greatest-width integer constants */

#ifdef __INTMAX_C
#define INTMAX_C(c) __INTMAX_C(c)
#endif /* __INTMAX_C */

#ifdef __UINTMAX_C
#define UINTMAX_C(c) __UINTMAX_C(c)
#endif /* __UINTMAX_C */

#endif /* _CC_STDINT_H */
