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
 * ISO C99 Standard: 7.24 Extended multibyte and wide character utilities
 * <wchar.h>
 */

#ifndef _CC_STD_WCHAR_H
#define _CC_STD_WCHAR_H

#include <stdarg.h>
#include <stddef.h>
#include <utils/__extern.h>

__BEGIN_DECLS

/* 7.24.1: an integer type holding any wide character, plus WEOF, which is
   none of them. */
#ifndef __WINT_TYPE__
#define __WINT_TYPE__ unsigned int
#endif /* __WINT_TYPE__ */

#ifndef _CC_STD_WINT_T_DEFINED
#define _CC_STD_WINT_T_DEFINED
typedef __WINT_TYPE__ wint_t;
#endif /* _CC_STD_WINT_T_DEFINED */

#ifndef _CC_STD_MBSTATE_T_DEFINED
#define _CC_STD_MBSTATE_T_DEFINED
/* The conversion state a multibyte sequence is read and written through,
   written the way the C library lays it out. */
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

#define WEOF ((wint_t)-1)

/* 7.24.1: the range wchar_t holds, which is the compiler's to say. */
#ifdef __WCHAR_MAX__
#define WCHAR_MAX __WCHAR_MAX__
#else
#define WCHAR_MAX 0x7fffffff
#endif /* __WCHAR_MAX__ */

#ifdef __WCHAR_MIN__
#define WCHAR_MIN __WCHAR_MIN__
#elif defined(__WCHAR_UNSIGNED__)
#define WCHAR_MIN 0
#else
#define WCHAR_MIN (-WCHAR_MAX - 1)
#endif /* __WCHAR_MAX__ */

/* What wcsftime is written with is only ever pointed to here (7.24.5.1). */
struct tm;

/* 7.24.2 Formatted wide character input/output functions.
   The streams are the ones <stdio.h> declares, which is what a FILE is
   written by, so they are only ever pointed to here as well. */
struct _IO_FILE;

typedef struct _IO_FILE FILE;

extern int
fwprintf(FILE *__restrict stream, const wchar_t *__restrict format, ...);
extern int
fwscanf(FILE *__restrict stream, const wchar_t *__restrict format, ...);
extern int
swprintf(wchar_t *__restrict s,
         size_t n,
         const wchar_t *__restrict format,
         ...);
extern int
swscanf(const wchar_t *__restrict s, const wchar_t *__restrict format, ...);
extern int
vfwprintf(FILE *__restrict stream,
          const wchar_t *__restrict format,
          va_list arg);
extern int
vfwscanf(FILE *__restrict stream,
         const wchar_t *__restrict format,
         va_list arg);
extern int
vswprintf(wchar_t *__restrict s,
          size_t n,
          const wchar_t *__restrict format,
          va_list arg);
extern int
vswscanf(const wchar_t *__restrict s,
         const wchar_t *__restrict format,
         va_list arg);
extern int
vwprintf(const wchar_t *__restrict format, va_list arg);
extern int
vwscanf(const wchar_t *__restrict format, va_list arg);
extern int
wprintf(const wchar_t *__restrict format, ...);
extern int
wscanf(const wchar_t *__restrict format, ...);

/* 7.24.3 Wide character input/output functions */
extern wint_t
fgetwc(FILE *stream);
extern wchar_t *
fgetws(wchar_t *__restrict s, int n, FILE *__restrict stream);
extern wint_t
fputwc(wchar_t c, FILE *stream);
extern int
fputws(const wchar_t *__restrict s, FILE *__restrict stream);
extern int
fwide(FILE *stream, int mode);
extern wint_t
getwc(FILE *stream);
extern wint_t
getwchar(void);
extern wint_t
putwc(wchar_t c, FILE *stream);
extern wint_t
putwchar(wchar_t c);
extern wint_t
ungetwc(wint_t c, FILE *stream);

/* 7.24.4.1 Wide string numeric conversion functions */
extern double
wcstod(const wchar_t *__restrict nptr, wchar_t **__restrict endptr);
extern float
wcstof(const wchar_t *__restrict nptr, wchar_t **__restrict endptr);
extern long double
wcstold(const wchar_t *__restrict nptr, wchar_t **__restrict endptr);
extern long int
wcstol(const wchar_t *__restrict nptr, wchar_t **__restrict endptr, int base);
extern long long int
wcstoll(const wchar_t *__restrict nptr, wchar_t **__restrict endptr, int base);
extern unsigned long int
wcstoul(const wchar_t *__restrict nptr, wchar_t **__restrict endptr, int base);
extern unsigned long long int
wcstoull(const wchar_t *__restrict nptr, wchar_t **__restrict endptr, int base);

/* 7.24.4.2 Wide string copying functions */
extern wchar_t *
wcscpy(wchar_t *__restrict s1, const wchar_t *__restrict s2);
extern wchar_t *
wcsncpy(wchar_t *__restrict s1, const wchar_t *__restrict s2, size_t n);
extern wchar_t *
wmemcpy(wchar_t *__restrict s1, const wchar_t *__restrict s2, size_t n);
extern wchar_t *
wmemmove(wchar_t *s1, const wchar_t *s2, size_t n);

/* 7.24.4.3 Wide string concatenation functions */
extern wchar_t *
wcscat(wchar_t *__restrict s1, const wchar_t *__restrict s2);
extern wchar_t *
wcsncat(wchar_t *__restrict s1, const wchar_t *__restrict s2, size_t n);

/* 7.24.4.4 Wide string comparison functions */
extern int
wcscmp(const wchar_t *s1, const wchar_t *s2);
extern int
wcscoll(const wchar_t *s1, const wchar_t *s2);
extern int
wcsncmp(const wchar_t *s1, const wchar_t *s2, size_t n);
extern size_t
wcsxfrm(wchar_t *__restrict s1, const wchar_t *__restrict s2, size_t n);
extern int
wmemcmp(const wchar_t *s1, const wchar_t *s2, size_t n);

/* 7.24.4.5 Wide string search functions */
extern wchar_t *
wcschr(const wchar_t *s, wchar_t c);
extern size_t
wcscspn(const wchar_t *s1, const wchar_t *s2);
extern wchar_t *
wcspbrk(const wchar_t *s1, const wchar_t *s2);
extern wchar_t *
wcsrchr(const wchar_t *s, wchar_t c);
extern size_t
wcsspn(const wchar_t *s1, const wchar_t *s2);
extern wchar_t *
wcsstr(const wchar_t *s1, const wchar_t *s2);
extern wchar_t *
wcstok(wchar_t *__restrict s1,
       const wchar_t *__restrict s2,
       wchar_t **__restrict ptr);
extern wchar_t *
wmemchr(const wchar_t *s, wchar_t c, size_t n);

/* 7.24.4.6 Miscellaneous functions */
extern size_t
wcslen(const wchar_t *s);
extern wchar_t *
wmemset(wchar_t *s, wchar_t c, size_t n);

/* 7.24.5 Wide character time conversion functions */
extern size_t
wcsftime(wchar_t *__restrict s,
         size_t maxsize,
         const wchar_t *__restrict format,
         const struct tm *__restrict timeptr);

/* 7.24.6.1 Single byte/wide character conversion functions */
extern wint_t
btowc(int c);
extern int
wctob(wint_t c);

/* 7.24.6.2 Conversion state functions */
extern int
mbsinit(const mbstate_t *ps);

/* 7.24.6.3 Restartable multibyte/wide character conversion functions */
extern size_t
mbrlen(const char *__restrict s, size_t n, mbstate_t *__restrict ps);
extern size_t
mbrtowc(wchar_t *__restrict pwc,
        const char *__restrict s,
        size_t n,
        mbstate_t *__restrict ps);
extern size_t
wcrtomb(char *__restrict s, wchar_t wc, mbstate_t *__restrict ps);

/* 7.24.6.4 Restartable multibyte/wide string conversion functions */
extern size_t
mbsrtowcs(wchar_t *__restrict dst,
          const char **__restrict src,
          size_t len,
          mbstate_t *__restrict ps);
extern size_t
wcsrtombs(char *__restrict dst,
          const wchar_t **__restrict src,
          size_t len,
          mbstate_t *__restrict ps);

__END_DECLS

#endif /* _CC_STD_WCHAR_H */
