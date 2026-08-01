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
 * ISO C99 Standard: 7.25 Wide character classification and mapping utilities
 * <wctype.h>
 */

#ifndef _CC_STD_WCTYPE_H
#define _CC_STD_WCTYPE_H

#include <utils/__extern.h>

__BEGIN_DECLS

/* 7.25.1: an integer type holding any wide character, plus WEOF, which is
   none of them. */
#ifndef __WINT_TYPE__
#define __WINT_TYPE__ unsigned int
#endif /* __WINT_TYPE__ */

#ifndef _CC_STD_WINT_T_DEFINED
#define _CC_STD_WINT_T_DEFINED
typedef __WINT_TYPE__ wint_t;
#endif /* _CC_STD_WINT_T_DEFINED */

/* A scalar type holding what a classification and a mapping are looked up
   by, which is what wctype and wctrans give back. */
typedef unsigned long int wctype_t;
typedef const int *wctrans_t;

/* 7.25.1: a value that is no member of the extended character set. */
#define WEOF ((wint_t)-1)

/* 7.25.2.1 Wide character classification functions */
extern int
iswalnum(wint_t wc);
extern int
iswalpha(wint_t wc);
extern int
iswblank(wint_t wc);
extern int
iswcntrl(wint_t wc);
extern int
iswdigit(wint_t wc);
extern int
iswgraph(wint_t wc);
extern int
iswlower(wint_t wc);
extern int
iswprint(wint_t wc);
extern int
iswpunct(wint_t wc);
extern int
iswspace(wint_t wc);
extern int
iswupper(wint_t wc);
extern int
iswxdigit(wint_t wc);

/* 7.25.2.2: a classification the locale names, which iswctype then reads a
   character against. */
extern int
iswctype(wint_t wc, wctype_t desc);
extern wctype_t
wctype(const char *property);

/* 7.25.3.1 Wide character case mapping functions */
extern wint_t
towlower(wint_t wc);
extern wint_t
towupper(wint_t wc);

/* 7.25.3.2: a mapping the locale names, which towctrans then reads a
   character through. */
extern wint_t
towctrans(wint_t wc, wctrans_t desc);
extern wctrans_t
wctrans(const char *property);

__END_DECLS

#endif /* _CC_STD_WCTYPE_H */
