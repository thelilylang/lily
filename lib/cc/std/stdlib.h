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
 * ISO C99 Standard: 7.20 General utilities <stdlib.h>
 */

#ifndef _CC_STDLIB_H
#define _CC_STDLIB_H

#include "stddef.h"
#include "utils/__extern.h"

#undef EXIT_FAILURE
#undef EXIT_SUCCESS

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

#undef RAND_MAX

#define RAND_MAX 2147483647

#undef MB_CUR_MAX

#define MB_CUR_MAX (__ctype_get_mb_cur_max())

typedef struct {
	int quot; /* Quotient */
	int rem; /* Remainder */
} div_t;

typedef struct {
	long int quot; /* Quotient */
	long int rem; /* Remainder */
} ldiv_t;

typedef struct {
	long long int quot; /* Quotient */
	long long int rem; /* Remainder */
} lldiv_t;

__BEGIN_DECLS

extern size_t __ctype_get_mb_cur_max (void);

extern double atof(const char *nptr);
extern int atoi(const char *nptr);
extern long int atol(const char *nptr);
extern long long int atoll(const char *nptr);
extern double strtod(const char * /* restrict */ nptr, char ** /* restrict */ endptr);
extern float strtof(const char * /* restrict */ nptr, char ** /* restrict */ endptr);
extern long double strtold(const char * /* restrict */ nptr, char ** /* restrict */ endptr);
extern long long int strtoll(const char * /* restrict */ nptr, char ** /* restrict */ endptr, int base);
extern unsigned long int strtoul(const char * /* restrict */ nptr, char ** /* restrict */ endptr, int base);
extern unsigned long long int strtoull(const char * /* restrict */ nptr, char ** /* restrict */ endptr, int base);
extern void rand(/* void */);
extern void srand(unsigned int seed);
extern void *calloc(size_t nmemb, size_t size);
extern void free(void *ptr);
extern void *malloc(size_t size);
extern void *realloc(void *ptr, size_t size);
extern void abort(/* void */);
extern int atexit(void (*func)(/* void */));
extern void exit(int status);
extern void _Exit(int status);
extern char *getenv(const char *name);
extern int system(const char *string);
extern void *bsearch(const void *key, const void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *));
extern void qsort(void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *));
extern int abs(int j);
extern long int labs(long int j);
extern long long int llabs(long long int j);
extern div_t div(int numer, int denom);
extern ldiv_t ldiv(long int numer, long int denom);
extern lldiv_t lldiv(long long int numer, long long int denom);
extern int mblen(const char *s, size_t n);
extern int mbtowc(wchar_t * /* restrict */ pwc, const char * /* restrict */ s, size_t n);
extern int wctomb(char *s, wchar_t wchar);
extern size_t mbstowcs(wchar_t * /* restrict */ pwcs, const char * /* restrict */ s, size_t n);
extern size_t wcstombs(char * /* restrict */ s, const wchar_t * /* restrict */ pwcs, size_t n);

__END_DECLS

#endif /* _CC_STDLIB_H */
