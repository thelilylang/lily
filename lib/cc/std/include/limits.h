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

/*
 * ISO C99 Standard: 5.2.4.2.1 Sizes of integer types <limits.h>
 * ISO C23 Standard: 5.2.5.3.2 Characteristics of integer types <limits.h>
 */

#ifndef _CC_STD_LIMITS_H
#define _CC_STD_LIMITS_H

/* The values the standard gives are the smallest ones an implementation may
   define, so what the compiler says about the target is what is used, and the
   standard's own minimum is only the fallback. */

/* Number of bits for the smallest object that is not a bit-field (byte). */
#ifdef __CHAR_BIT__
#define CHAR_BIT __CHAR_BIT__
#else
#define CHAR_BIT 8
#endif /* __CHAR_BIT__ */

#ifdef __SCHAR_MAX__
#define SCHAR_MAX __SCHAR_MAX__
#else
#define SCHAR_MAX 127
#endif /* __SCHAR_MAX__ */

#define SCHAR_MIN (-SCHAR_MAX - 1)
#define UCHAR_MAX (SCHAR_MAX * 2 + 1)

/* C99 footnote: CHAR_MIN is either 0 or SCHAR_MIN, which is what tells a plain
   char that holds a sign from one that does not. */
#ifdef __CHAR_UNSIGNED__
#define CHAR_MIN 0
#define CHAR_MAX UCHAR_MAX
#else
#define CHAR_MIN SCHAR_MIN
#define CHAR_MAX SCHAR_MAX
#endif /* __CHAR_UNSIGNED__ */

/* Maximum number of bytes in a multibyte character, for any supported locale.
   Only the "C" locale is supported, where a character is a single byte. */
#ifdef __MB_LEN_MAX__
#define MB_LEN_MAX __MB_LEN_MAX__
#else
#define MB_LEN_MAX 1
#endif /* __MB_LEN_MAX__ */

#ifdef __SHRT_MAX__
#define SHRT_MAX __SHRT_MAX__
#else
#define SHRT_MAX 32767
#endif /* __SHRT_MAX__ */

#define SHRT_MIN (-SHRT_MAX - 1)
#define USHRT_MAX (SHRT_MAX * 2 + 1)

#ifdef __INT_MAX__
#define INT_MAX __INT_MAX__
#else
#define INT_MAX 32767
#endif /* __INT_MAX__ */

#define INT_MIN (-INT_MAX - 1)
#define UINT_MAX (INT_MAX * 2U + 1U)

#ifdef __LONG_MAX__
#define LONG_MAX __LONG_MAX__
#else
#define LONG_MAX 2147483647L
#endif /* __LONG_MAX__ */

#define LONG_MIN (-LONG_MAX - 1L)
#define ULONG_MAX (LONG_MAX * 2UL + 1UL)

/* long long is written since C99. */
#if !defined(__STDC_VERSION__) || __STDC_VERSION__ >= 199901L
#ifdef __LONG_LONG_MAX__
#define LLONG_MAX __LONG_LONG_MAX__
#else
#define LLONG_MAX 9223372036854775807LL
#endif /* __LONG_LONG_MAX__ */

#define LLONG_MIN (-LLONG_MAX - 1LL)
#define ULLONG_MAX (LLONG_MAX * 2ULL + 1ULL)
#endif /* !defined(__STDC_VERSION__) || __STDC_VERSION__ >= 199901L */

/* The widths, and the width of a bit-precise integer, are written since C23.
   Each _WIDTH is the width of the type it names, and the ones of the character
   types are all CHAR_BIT. */
#if defined(__STDC_VERSION__) && __STDC_VERSION__ > 201710L
#ifdef __BOOL_WIDTH__
#define BOOL_WIDTH __BOOL_WIDTH__
#else
#define BOOL_WIDTH 1
#endif /* __BOOL_WIDTH__ */

#define BOOL_MAX 1

#define CHAR_WIDTH CHAR_BIT
#define SCHAR_WIDTH CHAR_BIT
#define UCHAR_WIDTH CHAR_BIT

#ifdef __SHRT_WIDTH__
#define SHRT_WIDTH __SHRT_WIDTH__
#else
#define SHRT_WIDTH 16
#endif /* __SHRT_WIDTH__ */

#define USHRT_WIDTH SHRT_WIDTH

#ifdef __INT_WIDTH__
#define INT_WIDTH __INT_WIDTH__
#else
#define INT_WIDTH 16
#endif /* __INT_WIDTH__ */

#define UINT_WIDTH INT_WIDTH

#ifdef __LONG_WIDTH__
#define LONG_WIDTH __LONG_WIDTH__
#else
#define LONG_WIDTH 32
#endif /* __LONG_WIDTH__ */

#define ULONG_WIDTH LONG_WIDTH

#ifdef __LONG_LONG_WIDTH__
#define LLONG_WIDTH __LONG_LONG_WIDTH__
#else
#define LLONG_WIDTH 64
#endif /* __LONG_LONG_WIDTH__ */

#define ULLONG_WIDTH LLONG_WIDTH

#ifdef __BITINT_MAXWIDTH__
#define BITINT_MAXWIDTH __BITINT_MAXWIDTH__
#else
#define BITINT_MAXWIDTH ULLONG_WIDTH
#endif /* __BITINT_MAXWIDTH__ */
#endif /* defined(__STDC_VERSION__) && __STDC_VERSION__ > 201710L */

#endif /* _CC_STD_LIMITS_H */
