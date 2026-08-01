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
 * ISO C11 Standard: 7.28 Unicode utilities <uchar.h>
 */

#ifndef _CC_STD_UCHAR_H
#define _CC_STD_UCHAR_H

#include <stddef.h>
#include <stdint.h>
#include <utils/__extern.h>

__BEGIN_DECLS

/* 7.28: the conversion state a multibyte sequence is read and written
   through. It is written the way the C library lays it out, since it is that
   one the object is handed to. */
#ifndef __WINT_TYPE__
#define __WINT_TYPE__ unsigned int
#endif /* __WINT_TYPE__ */

#ifndef _CC_STD_MBSTATE_T_DEFINED
#define _CC_STD_MBSTATE_T_DEFINED
typedef struct
{
    int __count;
    union
    {
        __WINT_TYPE__ __wch;
        char __wchb[4];
    } __value;
} mbstate_t;
#endif /* _CC_STD_MBSTATE_T_DEFINED */

/* 7.28: char16_t and char32_t are the types a UTF-16 and a UTF-32 code unit
   are held in. Since C23 the two are keywords of their own. */
#if !defined(__STDC_VERSION__) || __STDC_VERSION__ <= 201710L
typedef uint_least16_t char16_t;
typedef uint_least32_t char32_t;
#endif /* !defined(__STDC_VERSION__) || __STDC_VERSION__ <= 201710L */

/* 7.28.1: what the encoding a code unit is written in is, where the
   implementation says so. */
#define __STDC_UTF_16__ 1
#define __STDC_UTF_32__ 1

/* 7.28.1.1 - 7.28.1.4: a multibyte character is read into a code unit, and a
   code unit is written back out as one. */
extern size_t
mbrtoc16(char16_t *__restrict pc16,
         const char *__restrict s,
         size_t n,
         mbstate_t *__restrict ps);
extern size_t
c16rtomb(char *__restrict s, char16_t c16, mbstate_t *__restrict ps);
extern size_t
mbrtoc32(char32_t *__restrict pc32,
         const char *__restrict s,
         size_t n,
         mbstate_t *__restrict ps);
extern size_t
c32rtomb(char *__restrict s, char32_t c32, mbstate_t *__restrict ps);

__END_DECLS

#endif /* _CC_STD_UCHAR_H */
