/*
 * MIT License
 *
 * Copyright (c) 2022-2025 ArthurPV
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
 * ISO C99 Standard: 7.4 Character handling <ctype.h>
 */

#ifndef _CC_STD_CTYPE_H
#define _CC_STD_CTYPE_H

#include <utils/__extern.h>

__BEGIN_DECLS

extern int
isalnum(int c);
extern int
isalpha(int c);
extern int
isblank(int c);
extern int
iscntrl(int c);
extern int
isdigit(int c);
extern int
isgraph(int c);
extern int
islower(int c);
extern int
isprint(int c);
extern int
ispunct(int c);
extern int
isspace(int c);
extern int
isupper(int c);
extern int
isxdigit(int c);
extern int
tolower(int c);
extern int
toupper(int c);

__END_DECLS

#endif /* _CC_STD_CTYPE_H */
