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
 * ISO C99 Standard: 7.21 String handling <string.h>
 */

#ifndef _CC_STRING_H
#define _CC_STRING_H

#include "stddef.h"
#include "utils/__extern.h"

__BEGIN_DECLS

extern void *memcpy(void * /* restrict */ s1, const void * /* restrict */ s2, size_t n);
extern void *memmove(void *s1, const void *s2, size_t n);
extern char *strcpy(char * /* restrict */ s1, const char * /* restrict */ s2);
extern char *strncpy(char * /* restrict */ s1, const char * /* restrict */ s2, size_t n);
extern char *strcat(char * /* restrict */ s1, const char * /* restrict */ s2);
extern char *strncat(char * /* restrict */ s1, const char * /* restrict */ s2, size_t n);
extern int memcmp(const void *s1, const void *s2, size_t n);
extern int strcmp(const char *s1, const char *s2);
extern int strcoll(const char *s1, const char *s2);
extern int strncmp(const char *s1, const char *s2, size_t n);
extern size_t strxfrm(char * /* restrict */ s1, const char * /* restrict */ s2, size_t n);
extern void *memchr(const void *s, int c, size_t n);
extern char *strchr(const char *s, int c);
extern size_t strcspn(const char *s1, const char *s2);
extern char *strpbrk(const char *s1, const char *s2);
extern char *strrchr(const char *s, int c);
extern size_t strspn(const char *s1, const char *s2);
extern char *strstr(const char *s1, const char *s2);
extern char *strok(char * /* restrict */ s1, const char * /* restrict */ s2);
extern void *memset(void *s, int c, size_t n);
extern char *strerror(int errnum);
extern size_t strlen(const char *s);

__END_DECLS

#endif /* _CC_STRING_H */
