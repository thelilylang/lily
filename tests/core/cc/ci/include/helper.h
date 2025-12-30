#ifndef LILY_TESTS_CORE_CC_CI_HELPER_H
#define LILY_TESTS_CORE_CC_CI_HELPER_H

#ifndef NULL
#define NULL (void*)0
#endif /* NULL */

typedef __SIZE_TYPE__ size_t;

extern int puts(const char *s);
extern int printf(const char *s, ...);

extern void *malloc(size_t size);
extern void free(void *ptr); 

extern void exit(int status);

#endif /* LILY_TESTS_CORE_CC_CI_HELPER_H */
