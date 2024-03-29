#include <base/assert.h>
#include <base/atoi.h>
#include <base/itoa.h>
#include <base/test.h>

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ATOI_DEBUG_INT8_RANDOM_TEST
#undef ATOI_DEBUG_INT8_RANDOM_TEST

#define ATOI_DEBUG_INT16_RANDOM_TEST
#undef ATOI_DEBUG_INT16_RANDOM_TEST

#define ATOI_DEBUG_INT32_RANDOM_TEST
#undef ATOI_DEBUG_INT32_RANDOM_TEST

#define ATOI_DEBUG_INT64_RANDOM_TEST
#undef ATOI_DEBUG_INT64_RANDOM_TEST

#define ATOI_DEBUG_UINT8_RANDOM_TEST
#undef ATOI_DEBUG_UINT8_RANDOM_TEST

#define ATOI_DEBUG_UINT16_RANDOM_TEST
#undef ATOI_DEBUG_UINT16_RANDOM_TEST

#define ATOI_DEBUG_UINT32_RANDOM_TEST
#undef ATOI_DEBUG_UINT32_RANDOM_TEST

#define ATOI_DEBUG_UINT64_RANDOM_TEST
#undef ATOI_DEBUG_UINT64_RANDOM_TEST

SUITE(atoi);

CASE(atoi_check_int8_overflow, {
    TEST_ASSERT(CHECK_INT8_OVERFLOW_FROM_STRING("1011101", 2));
    TEST_ASSERT(CHECK_INT8_OVERFLOW_FROM_STRING("175", 8));
    TEST_ASSERT(CHECK_INT8_OVERFLOW_FROM_STRING("120", 10));
    TEST_ASSERT(CHECK_INT8_OVERFLOW_FROM_STRING("6D", 16));

    TEST_ASSERT(!CHECK_INT8_OVERFLOW_FROM_STRING("110010000", 2));
    TEST_ASSERT(!CHECK_INT8_OVERFLOW_FROM_STRING("1440", 8));
    TEST_ASSERT(!CHECK_INT8_OVERFLOW_FROM_STRING("200", 10));
    TEST_ASSERT(!CHECK_INT8_OVERFLOW_FROM_STRING("14A", 16));

    TEST_ASSERT(!CHECK_INT8_OVERFLOW_FROM_STRING("-110010000", 2));
    TEST_ASSERT(!CHECK_INT8_OVERFLOW_FROM_STRING("-1440", 8));
    TEST_ASSERT(!CHECK_INT8_OVERFLOW_FROM_STRING("-200", 10));
    TEST_ASSERT(!CHECK_INT8_OVERFLOW_FROM_STRING("-14A", 16));

    {
        time_t tt;
        srand((unsigned)time(&tt));

        for (int i = 0; i < 1000; i++) {
            Int8 np = rand() % 127;
            Int8 nn = -rand() % 128;

            char *np_s_2 = itoa__Int8(np, 2);
            char *nn_s_2 = itoa__Int8(nn, 2);
            char *np_s_8 = itoa__Int8(np, 8);
            char *nn_s_8 = itoa__Int8(nn, 8);
            char *np_s_10 = itoa__Int8(np, 10);
            char *nn_s_10 = itoa__Int8(nn, 10);
            char *np_s_16 = itoa__Int8(np, 16);
            char *nn_s_16 = itoa__Int8(nn, 16);

#ifdef ATOI_DEBUG_INT8_RANDOM_TEST
            puts("Base 2");
            printf("np_s: %s, nn_s: %s\n", np_s_2, nn_s_2);
            printf("np_s: %d, nn_s: %d\n",
                   CHECK_INT8_OVERFLOW_FROM_STRING(np_s_2, 2),
                   CHECK_INT8_OVERFLOW_FROM_STRING(nn_s_2, 2));

            puts("Base 8");
            printf("np_s: %s, nn_s: %s\n", np_s_8, nn_s_8);
            printf("np_s: %d, nn_s: %d\n",
                   CHECK_INT8_OVERFLOW_FROM_STRING(np_s_8, 8),
                   CHECK_INT8_OVERFLOW_FROM_STRING(nn_s_8, 8));

            puts("Base 10");
            printf("np_s: %s, nn_s: %s\n", np_s_10, nn_s_10);
            printf("np_s: %d, nn_s: %d\n",
                   CHECK_INT8_OVERFLOW_FROM_STRING(np_s_10, 10),
                   CHECK_INT8_OVERFLOW_FROM_STRING(nn_s_10, 10));

            puts("Base 16");
            printf("np_s: %s, nn_s: %s\n", np_s_16, nn_s_16);
            printf("np_s: %d, nn_s: %d\n",
                   CHECK_INT8_OVERFLOW_FROM_STRING(np_s_16, 16),
                   CHECK_INT8_OVERFLOW_FROM_STRING(nn_s_16, 16));
#endif

            TEST_ASSERT(CHECK_INT8_OVERFLOW_FROM_STRING(np_s_2, 2));
            TEST_ASSERT(CHECK_INT8_OVERFLOW_FROM_STRING(nn_s_2, 2));
            TEST_ASSERT(CHECK_INT8_OVERFLOW_FROM_STRING(np_s_8, 8));
            TEST_ASSERT(CHECK_INT8_OVERFLOW_FROM_STRING(nn_s_8, 8));
            TEST_ASSERT(CHECK_INT8_OVERFLOW_FROM_STRING(np_s_10, 10));
            TEST_ASSERT(CHECK_INT8_OVERFLOW_FROM_STRING(nn_s_10, 10));
            TEST_ASSERT(CHECK_INT8_OVERFLOW_FROM_STRING(np_s_16, 16));
            TEST_ASSERT(CHECK_INT8_OVERFLOW_FROM_STRING(nn_s_16, 16));

            lily_free(np_s_2);
            lily_free(nn_s_2);
            lily_free(np_s_8);
            lily_free(nn_s_8);
            lily_free(np_s_10);
            lily_free(nn_s_10);
            lily_free(np_s_16);
            lily_free(nn_s_16);
        }
    }
});

CASE(atoi_check_int16_overflow, {
    TEST_ASSERT(CHECK_INT16_OVERFLOW_FROM_STRING("111111100101011", 2));
    TEST_ASSERT(CHECK_INT16_OVERFLOW_FROM_STRING("5670", 8));
    TEST_ASSERT(CHECK_INT16_OVERFLOW_FROM_STRING("32030", 10));
    TEST_ASSERT(CHECK_INT16_OVERFLOW_FROM_STRING("4B16", 16));
    TEST_ASSERT(CHECK_INT16_OVERFLOW_FROM_STRING("-32768", 10));

    TEST_ASSERT(!CHECK_INT16_OVERFLOW_FROM_STRING("1100011000000000001100", 2));
    TEST_ASSERT(!CHECK_INT16_OVERFLOW_FROM_STRING("251415574", 8));
    TEST_ASSERT(!CHECK_INT16_OVERFLOW_FROM_STRING("33450", 10));
    TEST_ASSERT(!CHECK_INT16_OVERFLOW_FROM_STRING("43CF8C", 16));

    TEST_ASSERT(
      !CHECK_INT16_OVERFLOW_FROM_STRING("-1100011000000000001100", 2));
    TEST_ASSERT(!CHECK_INT16_OVERFLOW_FROM_STRING("-251415574", 8));
    TEST_ASSERT(!CHECK_INT16_OVERFLOW_FROM_STRING("-33450", 10));
    TEST_ASSERT(!CHECK_INT16_OVERFLOW_FROM_STRING("-43CF8C", 16));

    {
        time_t tt;
        srand((unsigned)time(&tt));

        for (int i = 0; i < 1000; i++) {
            Int16 np = rand() % 32767;
            Int16 nn = -rand() % 32768;

            char *np_s_2 = itoa__Int16(np, 2);
            char *nn_s_2 = itoa__Int16(nn, 2);
            char *np_s_8 = itoa__Int16(np, 8);
            char *nn_s_8 = itoa__Int16(nn, 8);
            char *np_s_10 = itoa__Int16(np, 10);
            char *nn_s_10 = itoa__Int16(nn, 10);
            char *np_s_16 = itoa__Int16(np, 16);
            char *nn_s_16 = itoa__Int16(nn, 16);

#ifdef ATOI_DEBUG_INT16_RANDOM_TEST
            puts("Base 2");
            printf("np_s: %s, nn_s: %s\n", np_s_2, nn_s_2);
            printf("np_s: %d, nn_s: %d\n",
                   CHECK_INT16_OVERFLOW_FROM_STRING(np_s_2, 2),
                   CHECK_INT16_OVERFLOW_FROM_STRING(nn_s_2, 2));

            puts("Base 8");
            printf("np_s: %s, nn_s: %s\n", np_s_8, nn_s_8);
            printf("np_s: %d, nn_s: %d\n",
                   CHECK_INT16_OVERFLOW_FROM_STRING(np_s_8, 8),
                   CHECK_INT16_OVERFLOW_FROM_STRING(nn_s_8, 8));

            puts("Base 10");
            printf("np_s: %s, nn_s: %s\n", np_s_10, nn_s_10);
            printf("np_s: %d, nn_s: %d\n",
                   CHECK_INT16_OVERFLOW_FROM_STRING(np_s_10, 10),
                   CHECK_INT16_OVERFLOW_FROM_STRING(nn_s_10, 10));

            puts("Base 16");
            printf("np_s: %s, nn_s: %s\n", np_s_16, nn_s_16);
            printf("np_s: %d, nn_s: %d\n",
                   CHECK_INT16_OVERFLOW_FROM_STRING(np_s_16, 16),
                   CHECK_INT16_OVERFLOW_FROM_STRING(nn_s_16, 16));
#endif

            TEST_ASSERT(CHECK_INT16_OVERFLOW_FROM_STRING(np_s_2, 2));
            TEST_ASSERT(CHECK_INT16_OVERFLOW_FROM_STRING(nn_s_2, 2));
            TEST_ASSERT(CHECK_INT16_OVERFLOW_FROM_STRING(np_s_8, 8));
            TEST_ASSERT(CHECK_INT16_OVERFLOW_FROM_STRING(nn_s_8, 8));
            TEST_ASSERT(CHECK_INT16_OVERFLOW_FROM_STRING(np_s_10, 10));
            TEST_ASSERT(CHECK_INT16_OVERFLOW_FROM_STRING(nn_s_10, 10));
            TEST_ASSERT(CHECK_INT16_OVERFLOW_FROM_STRING(np_s_16, 16));
            TEST_ASSERT(CHECK_INT16_OVERFLOW_FROM_STRING(nn_s_16, 16));

            lily_free(np_s_2);
            lily_free(nn_s_2);
            lily_free(np_s_8);
            lily_free(nn_s_8);
            lily_free(np_s_10);
            lily_free(nn_s_10);
            lily_free(np_s_16);
            lily_free(nn_s_16);
        }
    }
});

CASE(atoi_check_int32_overflow, {
    TEST_ASSERT(CHECK_INT32_OVERFLOW_FROM_STRING("111111111111111", 2));
    TEST_ASSERT(CHECK_INT32_OVERFLOW_FROM_STRING("14556325", 8));
    TEST_ASSERT(CHECK_INT32_OVERFLOW_FROM_STRING("222222220", 10));
    TEST_ASSERT(CHECK_INT32_OVERFLOW_FROM_STRING("7903F847", 16));

    TEST_ASSERT(!CHECK_INT32_OVERFLOW_FROM_STRING(
      "11111111110101110110111111111111111111111111111111", 2));
    TEST_ASSERT(!CHECK_INT32_OVERFLOW_FROM_STRING("2223334425553344", 8));
    TEST_ASSERT(!CHECK_INT32_OVERFLOW_FROM_STRING("2147483649", 10));
    TEST_ASSERT(!CHECK_INT32_OVERFLOW_FROM_STRING("7903F84723A", 16));

    TEST_ASSERT(!CHECK_INT32_OVERFLOW_FROM_STRING(
      "-11111111110101110110111111111111111111111111111111", 2));
    TEST_ASSERT(!CHECK_INT32_OVERFLOW_FROM_STRING("-2223334425553344", 8));
    TEST_ASSERT(!CHECK_INT32_OVERFLOW_FROM_STRING("-2147483649", 10));
    TEST_ASSERT(!CHECK_INT32_OVERFLOW_FROM_STRING("-7903F84723A", 16));

    {
        time_t tt;
        srand((unsigned)time(&tt));

        for (int i = 0; i < 1000; i++) {
            Int32 np = rand() % INT32_MAX;
            Int32 nn = -rand() % INT32_MIN + 1;

            char *np_s_2 = itoa__Int32(np, 2);
            char *nn_s_2 = itoa__Int32(nn, 2);
            char *np_s_8 = itoa__Int32(np, 8);
            char *nn_s_8 = itoa__Int32(nn, 8);
            char *np_s_10 = itoa__Int32(np, 10);
            char *nn_s_10 = itoa__Int32(nn, 10);
            char *np_s_16 = itoa__Int32(np, 16);
            char *nn_s_16 = itoa__Int32(nn, 16);

#ifdef ATOI_DEBUG_INT32_RANDOM_TEST
            puts("Base 2");
            printf("np_s: %s, nn_s: %s\n", np_s_2, nn_s_2);
            printf("np_s: %d, nn_s: %d\n",
                   CHECK_INT32_OVERFLOW_FROM_STRING(np_s_2, 2),
                   CHECK_INT32_OVERFLOW_FROM_STRING(nn_s_2, 2));

            puts("Base 8");
            printf("np_s: %s, nn_s: %s\n", np_s_8, nn_s_8);
            printf("np_s: %d, nn_s: %d\n",
                   CHECK_INT32_OVERFLOW_FROM_STRING(np_s_8, 8),
                   CHECK_INT32_OVERFLOW_FROM_STRING(nn_s_8, 8));

            puts("Base 10");
            printf("np_s: %s, nn_s: %s\n", np_s_10, nn_s_10);
            printf("np_s: %d, nn_s: %d\n",
                   CHECK_INT32_OVERFLOW_FROM_STRING(np_s_10, 10),
                   CHECK_INT32_OVERFLOW_FROM_STRING(nn_s_10, 10));

            puts("Base 16");
            printf("np_s: %s, nn_s: %s\n", np_s_16, nn_s_16);
            printf("np_s: %d, nn_s: %d\n",
                   CHECK_INT32_OVERFLOW_FROM_STRING(np_s_16, 16),
                   CHECK_INT32_OVERFLOW_FROM_STRING(nn_s_16, 16));
#endif

            TEST_ASSERT(CHECK_INT32_OVERFLOW_FROM_STRING(np_s_2, 2));
            TEST_ASSERT(CHECK_INT32_OVERFLOW_FROM_STRING(nn_s_2, 2));
            TEST_ASSERT(CHECK_INT32_OVERFLOW_FROM_STRING(np_s_8, 8));
            TEST_ASSERT(CHECK_INT32_OVERFLOW_FROM_STRING(nn_s_8, 8));
            TEST_ASSERT(CHECK_INT32_OVERFLOW_FROM_STRING(np_s_10, 10));
            TEST_ASSERT(CHECK_INT32_OVERFLOW_FROM_STRING(nn_s_10, 10));
            TEST_ASSERT(CHECK_INT32_OVERFLOW_FROM_STRING(np_s_16, 16));
            TEST_ASSERT(CHECK_INT32_OVERFLOW_FROM_STRING(nn_s_16, 16));

            lily_free(np_s_2);
            lily_free(nn_s_2);
            lily_free(np_s_8);
            lily_free(nn_s_8);
            lily_free(np_s_10);
            lily_free(nn_s_10);
            lily_free(np_s_16);
            lily_free(nn_s_16);
        }
    }
})

CASE(atoi_check_int64_overflow, {
    TEST_ASSERT(CHECK_INT64_OVERFLOW_FROM_STRING("1100111100000111", 2));
    TEST_ASSERT(CHECK_INT64_OVERFLOW_FROM_STRING("3333333333266333322", 8));
    TEST_ASSERT(CHECK_INT64_OVERFLOW_FROM_STRING("22222222222222222", 10));
    TEST_ASSERT(CHECK_INT64_OVERFLOW_FROM_STRING("aaaaaaaaaaa", 16));

    TEST_ASSERT(!CHECK_INT64_OVERFLOW_FROM_STRING(
      "1000000000000000000000000000000000000000000000000000000000000001", 2));
    TEST_ASSERT(!CHECK_INT64_OVERFLOW_FROM_STRING("1000000000000000000000", 8));
    TEST_ASSERT(!CHECK_INT64_OVERFLOW_FROM_STRING(
      "10000000000000000000002020202020030", 10));
    TEST_ASSERT(!CHECK_INT64_OVERFLOW_FROM_STRING("10000000000FFFFF", 16));

    TEST_ASSERT(
      !CHECK_INT64_OVERFLOW_FROM_STRING("-333333333333333333333333", 10));
    TEST_ASSERT(
      !CHECK_INT64_OVERFLOW_FROM_STRING("-aaaaaaaaaaaaaaaaaaaaaaa", 16));

    {
        time_t tt;
        srand((unsigned)time(&tt));

        for (int i = 0; i < 1000; i++) {
            Int64 np = rand() % INT64_MAX;
            Int64 nn = -rand() % INT64_MAX;

            char *np_s_2 = itoa__Int64(np, 2);
            char *nn_s_2 = itoa__Int64(nn, 2);
            char *np_s_8 = itoa__Int64(np, 8);
            char *nn_s_8 = itoa__Int64(nn, 8);
            char *np_s_10 = itoa__Int64(np, 10);
            char *nn_s_10 = itoa__Int64(nn, 10);
            char *np_s_16 = itoa__Int64(np, 16);
            char *nn_s_16 = itoa__Int64(nn, 16);

#ifdef ATOI_DEBUG_INT64_RANDOM_TEST
            puts("Base 2");
            printf("np_s: %s, nn_s: %s\n", np_s_2, nn_s_2);
            printf("np_s: %d, nn_s: %d\n",
                   CHECK_INT64_OVERFLOW_FROM_STRING(np_s_2, 2),
                   CHECK_INT64_OVERFLOW_FROM_STRING(nn_s_2, 2));

            puts("Base 8");
            printf("np_s: %s, nn_s: %s\n", np_s_8, nn_s_8);
            printf("np_s: %d, nn_s: %d\n",
                   CHECK_INT64_OVERFLOW_FROM_STRING(np_s_8, 8),
                   CHECK_INT64_OVERFLOW_FROM_STRING(nn_s_8, 8));

            puts("Base 10");
            printf("np_s: %s, nn_s: %s\n", np_s_10, nn_s_10);
            printf("np_s: %d, nn_s: %d\n",
                   CHECK_INT64_OVERFLOW_FROM_STRING(np_s_10, 10),
                   CHECK_INT64_OVERFLOW_FROM_STRING(nn_s_10, 10));

            puts("Base 16");
            printf("np_s: %s, nn_s: %s\n", np_s_16, nn_s_16);
            printf("np_s: %d, nn_s: %d\n",
                   CHECK_INT64_OVERFLOW_FROM_STRING(np_s_16, 16),
                   CHECK_INT64_OVERFLOW_FROM_STRING(nn_s_16, 16));
#endif

            TEST_ASSERT(CHECK_INT64_OVERFLOW_FROM_STRING(np_s_2, 2));
            TEST_ASSERT(CHECK_INT64_OVERFLOW_FROM_STRING(nn_s_2, 2));
            TEST_ASSERT(CHECK_INT64_OVERFLOW_FROM_STRING(np_s_8, 8));
            TEST_ASSERT(CHECK_INT64_OVERFLOW_FROM_STRING(nn_s_8, 8));
            TEST_ASSERT(CHECK_INT64_OVERFLOW_FROM_STRING(np_s_10, 10));
            TEST_ASSERT(CHECK_INT64_OVERFLOW_FROM_STRING(nn_s_10, 10));
            TEST_ASSERT(CHECK_INT64_OVERFLOW_FROM_STRING(np_s_16, 16));
            TEST_ASSERT(CHECK_INT64_OVERFLOW_FROM_STRING(nn_s_16, 16));

            lily_free(np_s_2);
            lily_free(nn_s_2);
            lily_free(np_s_8);
            lily_free(nn_s_8);
            lily_free(np_s_10);
            lily_free(nn_s_10);
            lily_free(np_s_16);
            lily_free(nn_s_16);
        }
    }
});

CASE(atoi_check_uint8_overflow, {
    TEST_ASSERT(CHECK_UINT8_OVERFLOW_FROM_STRING("1111", 2));
    TEST_ASSERT(CHECK_UINT8_OVERFLOW_FROM_STRING("22", 8));
    TEST_ASSERT(CHECK_UINT8_OVERFLOW_FROM_STRING("288", 8));
    TEST_ASSERT(CHECK_UINT8_OVERFLOW_FROM_STRING("254", 10));
    TEST_ASSERT(CHECK_UINT8_OVERFLOW_FROM_STRING("AA", 16));

    TEST_ASSERT(!CHECK_UINT8_OVERFLOW_FROM_STRING("111111111", 2));
    TEST_ASSERT(!CHECK_UINT8_OVERFLOW_FROM_STRING("2777", 8));
    TEST_ASSERT(!CHECK_UINT8_OVERFLOW_FROM_STRING("-1", 10));
    TEST_ASSERT(!CHECK_UINT8_OVERFLOW_FROM_STRING("FFF", 16));

    {
        time_t tt;
        srand((unsigned)time(&tt));

        for (int i = 0; i < 1000; i++) {
            Uint8 np = rand() % 255;

            char *np_s_2 = itoa__Uint8(np, 2);
            char *np_s_8 = itoa__Uint8(np, 8);
            char *np_s_10 = itoa__Uint8(np, 10);
            char *np_s_16 = itoa__Uint8(np, 16);

#ifdef ATOI_DEBUG_UINT8_RANDOM_TEST
            puts("Base 2");
            printf("np_s: %s\n", np_s_2);
            printf("np_s: %d\n", CHECK_UINT8_OVERFLOW_FROM_STRING(np_s_2, 2));

            puts("Base 8");
            printf("np_s: %s\n", np_s_8);
            printf("np_s: %d\n", CHECK_UINT8_OVERFLOW_FROM_STRING(np_s_8, 8));

            puts("Base 10");
            printf("np_s: %s\n", np_s_10);
            printf("np_s: %d\n", CHECK_UINT8_OVERFLOW_FROM_STRING(np_s_10, 10));

            puts("Base 16");
            printf("np_s: %s\n", np_s_16);
            printf("np_s: %d\n", CHECK_UINT8_OVERFLOW_FROM_STRING(np_s_16, 16));
#endif

            TEST_ASSERT(CHECK_UINT8_OVERFLOW_FROM_STRING(np_s_2, 2));
            TEST_ASSERT(CHECK_UINT8_OVERFLOW_FROM_STRING(np_s_8, 8));
            TEST_ASSERT(CHECK_UINT8_OVERFLOW_FROM_STRING(np_s_10, 10));
            TEST_ASSERT(CHECK_UINT8_OVERFLOW_FROM_STRING(np_s_16, 16));

            lily_free(np_s_2);
            lily_free(np_s_8);
            lily_free(np_s_10);
            lily_free(np_s_16);
        }
    }
})

CASE(atoi_check_uint16_overflow, {
    TEST_ASSERT(CHECK_UINT16_OVERFLOW_FROM_STRING("1111111111111100", 2));
    TEST_ASSERT(CHECK_UINT16_OVERFLOW_FROM_STRING("1777", 8));
    TEST_ASSERT(CHECK_UINT16_OVERFLOW_FROM_STRING("63893", 10));
    TEST_ASSERT(CHECK_UINT16_OVERFLOW_FROM_STRING("ABCD", 16));

    TEST_ASSERT(!CHECK_UINT16_OVERFLOW_FROM_STRING("111111111111111111111", 2));
    TEST_ASSERT(!CHECK_UINT16_OVERFLOW_FROM_STRING("222222", 8));
    TEST_ASSERT(!CHECK_UINT16_OVERFLOW_FROM_STRING("-100", 10));
    TEST_ASSERT(!CHECK_UINT16_OVERFLOW_FROM_STRING("ABCDEFABC", 16));

    {
        time_t tt;
        srand((unsigned)time(&tt));

        for (int i = 0; i < 1000; i++) {
            Uint16 np = rand() % 65535;

            char *np_s_2 = itoa__Uint16(np, 2);
            char *np_s_8 = itoa__Uint16(np, 8);
            char *np_s_10 = itoa__Uint16(np, 10);
            char *np_s_16 = itoa__Uint16(np, 16);

#ifdef ATOI_DEBUG_UINT16_RANDOM_TEST
            puts("Base 2");
            printf("np_s: %s\n", np_s_2);
            printf("np_s: %d\n", CHECK_UINT16_OVERFLOW_FROM_STRING(np_s_2, 2));

            puts("Base 8");
            printf("np_s: %s\n", np_s_8);
            printf("np_s: %d\n", CHECK_UINT16_OVERFLOW_FROM_STRING(np_s_8, 8));

            puts("Base 10");
            printf("np_s: %s\n", np_s_10);
            printf("np_s: %d\n",
                   CHECK_UINT16_OVERFLOW_FROM_STRING(np_s_10, 10));

            puts("Base 16");
            printf("np_s: %s\n", np_s_16);
            printf("np_s: %d\n",
                   CHECK_UINT16_OVERFLOW_FROM_STRING(np_s_16, 16));
#endif

            TEST_ASSERT(CHECK_UINT16_OVERFLOW_FROM_STRING(np_s_2, 2));
            TEST_ASSERT(CHECK_UINT16_OVERFLOW_FROM_STRING(np_s_8, 8));
            TEST_ASSERT(CHECK_UINT16_OVERFLOW_FROM_STRING(np_s_10, 10));
            TEST_ASSERT(CHECK_UINT16_OVERFLOW_FROM_STRING(np_s_16, 16));

            lily_free(np_s_2);
            lily_free(np_s_8);
            lily_free(np_s_10);
            lily_free(np_s_16);
        }
    }
});

CASE(atoi_check_uint32_overflow, {
    TEST_ASSERT(
      CHECK_UINT32_OVERFLOW_FROM_STRING("111111111111111111111111100", 2));
    TEST_ASSERT(CHECK_UINT32_OVERFLOW_FROM_STRING("177777", 8));
    TEST_ASSERT(CHECK_UINT32_OVERFLOW_FROM_STRING("5000000", 10));
    TEST_ASSERT(CHECK_UINT32_OVERFLOW_FROM_STRING("FFFA", 16));

    TEST_ASSERT(!CHECK_UINT32_OVERFLOW_FROM_STRING(
      "11111111111111111111111111111111111111111111111111", 2));
    TEST_ASSERT(!CHECK_UINT32_OVERFLOW_FROM_STRING("222222222222222222222", 8));
    TEST_ASSERT(!CHECK_UINT32_OVERFLOW_FROM_STRING("-100", 10));
    TEST_ASSERT(
      !CHECK_UINT32_OVERFLOW_FROM_STRING("FFFFFFFFFFFFFFFFFFFFFFFFFF", 16));

    {
        time_t tt;
        srand((unsigned)time(&tt));

        for (int i = 0; i < 1000; i++) {
            Uint32 np = rand() % UINT32_MAX;

            char *np_s_2 = itoa__Uint32(np, 2);
            char *np_s_8 = itoa__Uint32(np, 8);
            char *np_s_10 = itoa__Uint32(np, 10);
            char *np_s_16 = itoa__Uint32(np, 16);

#ifdef ATOI_DEBUG_UINT32_RANDOM_TEST
            puts("Base 2");
            printf("np_s: %s\n", np_s_2);
            printf("np_s: %d\n", CHECK_UINT32_OVERFLOW_FROM_STRING(np_s_2, 2));

            puts("Base 8");
            printf("np_s: %s\n", np_s_8);
            printf("np_s: %d\n", CHECK_UINT32_OVERFLOW_FROM_STRING(np_s_8, 8));

            puts("Base 10");
            printf("np_s: %s\n", np_s_10);
            printf("np_s: %d\n",
                   CHECK_UINT32_OVERFLOW_FROM_STRING(np_s_10, 10));

            puts("Base 16");
            printf("np_s: %s\n", np_s_16);
            printf("np_s: %d\n",
                   CHECK_UINT32_OVERFLOW_FROM_STRING(np_s_16, 16));
#endif

            TEST_ASSERT(CHECK_UINT32_OVERFLOW_FROM_STRING(np_s_2, 2));
            TEST_ASSERT(CHECK_UINT32_OVERFLOW_FROM_STRING(np_s_8, 8));
            TEST_ASSERT(CHECK_UINT32_OVERFLOW_FROM_STRING(np_s_10, 10));
            TEST_ASSERT(CHECK_UINT32_OVERFLOW_FROM_STRING(np_s_16, 16));

            lily_free(np_s_2);
            lily_free(np_s_8);
            lily_free(np_s_10);
            lily_free(np_s_16);
        }
    }
});

CASE(atoi_check_uint64_overflow, {
    TEST_ASSERT(
      CHECK_UINT64_OVERFLOW_FROM_STRING("111111111111111111111111100", 2));
    TEST_ASSERT(CHECK_UINT64_OVERFLOW_FROM_STRING("177777", 8));
    TEST_ASSERT(CHECK_UINT64_OVERFLOW_FROM_STRING("5000000", 10));
    TEST_ASSERT(CHECK_UINT64_OVERFLOW_FROM_STRING("FFFA", 16));

    TEST_ASSERT(!CHECK_UINT64_OVERFLOW_FROM_STRING(
      "111111111111111111111111111111111111111111111111111111111111111111111111"
      "11111111111111111111111111111111111111111111111111111111111111111111111"
      "1",
      2));
    TEST_ASSERT(
      !CHECK_UINT64_OVERFLOW_FROM_STRING("2222222222222222222222222222222222222"
                                         "222222222222222222222222222222222222",
                                         8));
    TEST_ASSERT(!CHECK_UINT64_OVERFLOW_FROM_STRING("-100", 10));
    TEST_ASSERT(!CHECK_UINT64_OVERFLOW_FROM_STRING(
      "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
      "FFFFFFFFFF",
      16));

    {
        time_t tt;
        srand((unsigned)time(&tt));

        for (int i = 0; i < 1000; i++) {
            Uint64 np = rand() % UINT64_MAX;

            char *np_s_2 = itoa__Uint64(np, 2);
            char *np_s_8 = itoa__Uint64(np, 8);
            char *np_s_10 = itoa__Uint64(np, 10);
            char *np_s_16 = itoa__Uint64(np, 16);

#ifdef ATOI_DEBUG_UINT64_RANDOM_TEST
            puts("Base 2");
            printf("np_s: %s\n", np_s_2);
            printf("np_s: %d\n", CHECK_UINT64_OVERFLOW_FROM_STRING(np_s_2, 2));

            puts("Base 8");
            printf("np_s: %s\n", np_s_8);
            printf("np_s: %d\n", CHECK_UINT64_OVERFLOW_FROM_STRING(np_s_8, 8));

            puts("Base 10");
            printf("np_s: %s\n", np_s_10);
            printf("np_s: %d\n",
                   CHECK_UINT64_OVERFLOW_FROM_STRING(np_s_10, 10));

            puts("Base 16");
            printf("np_s: %s\n", np_s_16);
            printf("np_s: %d\n",
                   CHECK_UINT64_OVERFLOW_FROM_STRING(np_s_16, 16));
#endif

            TEST_ASSERT(CHECK_UINT64_OVERFLOW_FROM_STRING(np_s_2, 2));
            TEST_ASSERT(CHECK_UINT64_OVERFLOW_FROM_STRING(np_s_8, 8));
            TEST_ASSERT(CHECK_UINT64_OVERFLOW_FROM_STRING(np_s_10, 10));
            TEST_ASSERT(CHECK_UINT64_OVERFLOW_FROM_STRING(np_s_16, 16));

            lily_free(np_s_2);
            lily_free(np_s_8);
            lily_free(np_s_10);
            lily_free(np_s_16);
        }
    }
});

CASE(atoi, {
    TEST_ASSERT(atoi__Int32("11", 2) == 3);
    TEST_ASSERT(atoi__Int32("77", 8) == 63);
    TEST_ASSERT(atoi__Int32("123", 10) == 123);
    TEST_ASSERT(atoi__Int32("-123", 10) == -123);
    TEST_ASSERT(atoi__Int32("FF", 16) == 255);
});

CASE(atoi_safe, {
    {
        Optional *op = atoi_safe__Int8("500", 10);

        TEST_ASSERT(is_none__Optional(op));

        FREE(Optional, op);
}

{
    Optional *op = atoi_safe__Int8("100", 10);

    TEST_ASSERT(is_some__Optional(op));
    TEST_ASSERT(op->some == (Int8 *)100);

    FREE(Optional, op);
}
});