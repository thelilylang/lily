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
 * ISO C99 Standard: 7.23 Date and time <time.h>
 */

#ifndef _CC_STD_TIME_H
#define _CC_STD_TIME_H

#include <platform.h>
#include <stddef.h>
#include <utils/__extern.h>

__BEGIN_DECLS

/* 7.23.1: the two arithmetic types times are held in, and how many of what
   clock gives back stand for a second. Both are the platform's to say. */
#ifdef _CC_STD_LINUX
#ifdef _CC_STD_X86_64
typedef long int clock_t;
typedef long int time_t;
#else
typedef long int clock_t;
typedef long int time_t;
#endif /* _CC_STD_X86_64 */

#define CLOCKS_PER_SEC ((clock_t)1000000)
#endif /* _CC_STD_LINUX */

/* 7.23.1: a calendar time broken down into its parts. The members after
   tm_isdst are the platform's own, and are written here so that what is
   allocated is the size the C library reads and writes. */
struct tm
{
    int tm_sec;   /* seconds after the minute [0, 60] */
    int tm_min;   /* minutes after the hour [0, 59] */
    int tm_hour;  /* hours since midnight [0, 23] */
    int tm_mday;  /* day of the month [1, 31] */
    int tm_mon;   /* months since January [0, 11] */
    int tm_year;  /* years since 1900 */
    int tm_wday;  /* days since Sunday [0, 6] */
    int tm_yday;  /* days since January 1 [0, 365] */
    int tm_isdst; /* daylight saving time flag */
#ifdef _CC_STD_LINUX
    long int __tm_gmtoff;
    const char *__tm_zone;
#endif /* _CC_STD_LINUX */
};

/* 7.23.2 Time manipulation functions */
extern clock_t
clock(void);
extern double
difftime(time_t time1, time_t time0);
extern time_t
mktime(struct tm *timeptr);
extern time_t
time(time_t *timer);

/* 7.23.3 Time conversion functions */
extern char *
asctime(const struct tm *timeptr);
extern char *
ctime(const time_t *timer);
extern struct tm *
gmtime(const time_t *timer);
extern struct tm *
localtime(const time_t *timer);
extern size_t
strftime(char *__restrict s,
         size_t maxsize,
         const char *__restrict format,
         const struct tm *__restrict timeptr);

__END_DECLS

#endif /* _CC_STD_TIME_H */
