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
 * ISO C11 Standard: 7.26 Threads <threads.h>
 */

#ifndef _CC_STD_THREADS_H
#define _CC_STD_THREADS_H

#include <platform.h>
#include <time.h>
#include <utils/__extern.h>

__BEGIN_DECLS

/* Since C23 `thread_local` is a keyword of its own. */
#if !defined(__STDC_VERSION__) || __STDC_VERSION__ <= 201710L
#define thread_local _Thread_local
#endif /* !defined(__STDC_VERSION__) || __STDC_VERSION__ <= 201710L */

/* 7.26.1: how many times a thread-specific storage is written back to zero
   before the destructors are given up on. */
#define TSS_DTOR_ITERATIONS 4

#ifdef _CC_STD_LINUX
/* 7.26.1: what a thread, a mutex, a condition and a thread-specific storage
   are held in. A mutex and a condition are only ever handed to the C library,
   so what is written here is the room it needs, with the alignment it reads
   them under. */
#ifdef _CC_STD_X86_64
#define __CC_STD_SIZEOF_MTX_T 40
#else
#define __CC_STD_SIZEOF_MTX_T 24
#endif /* _CC_STD_X86_64 */

#define __CC_STD_SIZEOF_CND_T 48

typedef unsigned long int thrd_t;
typedef unsigned int tss_t;

typedef union
{
    char __size[__CC_STD_SIZEOF_MTX_T];
    long int __align;
} mtx_t;

typedef union
{
    char __size[__CC_STD_SIZEOF_CND_T];
    long long int __align;
} cnd_t;

/* 7.26.2.1: a flag a call is made through only once, written with the value
   the standard names. */
typedef struct
{
    int __data;
} once_flag;

#define ONCE_FLAG_INIT \
    {                  \
        0              \
    }
#endif /* _CC_STD_LINUX */

typedef int (*thrd_start_t)(void *);
typedef void (*tss_dtor_t)(void *);

/* 7.26.1: what a function of this header gives back. The enumeration is
   written with a tag of its own, in the namespace an implementation is given,
   since what the standard asks for is only the constants. */
enum __cc_std_thrd_status
{
    thrd_success = 0,
    thrd_busy = 1,
    thrd_error = 2,
    thrd_nomem = 3,
    thrd_timedout = 4
};

/* 7.26.4.1: how a mutex is written to behave. */
enum __cc_std_mtx_type
{
    mtx_plain = 0,
    mtx_recursive = 1,
    mtx_timed = 2
};

/* 7.26.2 Initialization functions */
extern void
call_once(once_flag *flag, void (*func)(void));

/* 7.26.3 Condition variable functions */
extern int
cnd_broadcast(cnd_t *cond);
extern void
cnd_destroy(cnd_t *cond);
extern int
cnd_init(cnd_t *cond);
extern int
cnd_signal(cnd_t *cond);
extern int
cnd_timedwait(cnd_t *__restrict cond,
              mtx_t *__restrict mtx,
              const struct timespec *__restrict ts);
extern int
cnd_wait(cnd_t *cond, mtx_t *mtx);

/* 7.26.4 Mutex functions */
extern void
mtx_destroy(mtx_t *mtx);
extern int
mtx_init(mtx_t *mtx, int type);
extern int
mtx_lock(mtx_t *mtx);
extern int
mtx_timedlock(mtx_t *__restrict mtx, const struct timespec *__restrict ts);
extern int
mtx_trylock(mtx_t *mtx);
extern int
mtx_unlock(mtx_t *mtx);

/* 7.26.5 Thread functions */
extern int
thrd_create(thrd_t *thr, thrd_start_t func, void *arg);
extern thrd_t
thrd_current(void);
extern int
thrd_detach(thrd_t thr);
extern int
thrd_equal(thrd_t thr0, thrd_t thr1);
extern void
thrd_exit(int res);
extern int
thrd_join(thrd_t thr, int *res);
extern int
thrd_sleep(const struct timespec *duration, struct timespec *remaining);
extern void
thrd_yield(void);

/* 7.26.6 Thread-specific storage functions */
extern int
tss_create(tss_t *key, tss_dtor_t dtor);
extern void
tss_delete(tss_t key);
extern void *
tss_get(tss_t key);
extern int
tss_set(tss_t key, void *val);

__END_DECLS

#endif /* _CC_STD_THREADS_H */
