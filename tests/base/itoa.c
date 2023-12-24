#include <base/assert.h>
#include <base/itoa.h>
#include <base/test.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

SUITE(itoa);

CASE(itoa_base_10, {
    char *res = itoa__Int8(0, 10);
    char *res2 = itoa__Int8(20, 10);

    TEST_ASSERT(strcmp(res, "0") == 0);
    TEST_ASSERT(strcmp(res2, "20") == 0);

    lily_free(res);
    lily_free(res2);
});

CASE(itoa_base_2, {
    char *res = itoa__Int8(0, 2);
    char *res2 = itoa__Int8(20, 2);

    TEST_ASSERT(strcmp(res, "0") == 0);
    TEST_ASSERT(strcmp(res2, "10100") == 0);

    lily_free(res);
    lily_free(res2);
});

CASE(itoa_base_8, {
    char *res = itoa__Int8(0, 8);
    char *res2 = itoa__Int8(20, 8);

    TEST_ASSERT(strcmp(res, "0") == 0);
    TEST_ASSERT(strcmp(res2, "24") == 0);

    lily_free(res);
    lily_free(res2);
});

CASE(itoa_base_16, {
    char *res = itoa__Int8(0, 16);
    char *res2 = itoa__Int8(20, 16);
    char *res3 = itoa__Int8(90, 16);

    TEST_ASSERT(strcmp(res, "0") == 0);
    TEST_ASSERT(strcmp(res2, "14") == 0);
    TEST_ASSERT(strcmp(res3, "5A") == 0);

    lily_free(res);
    lily_free(res2);
    lily_free(res3);
});
