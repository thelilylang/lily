#include <base/assert.h>
#include <base/format.h>
#include <base/new.h>
#include <base/string.h>
#include <base/test.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

SUITE(format);

CASE(format_s_specifier, {
    char *s = format("{s}", "Hello");

    TEST_ASSERT(strcmp(s, "Hello") == 0);

    lily_free(s);
});

CASE(format_sa_specifier, {
    char *s = format("{sa}", format("{s}", "Hello"));

    TEST_ASSERT(strcmp(s, "Hello") == 0);

    lily_free(s);
});

CASE(format_d_specifier, {
    char *s = format("{d}", 40);

    TEST_ASSERT(strcmp(s, "40") == 0);

    lily_free(s);
});

CASE(format_d_binary_specifier, {
    char *s = format("{d:b}", 20);

    TEST_ASSERT(strcmp(s, "10100") == 0);

    lily_free(s);
});

CASE(format_d_octal_specifier, {
    char *s = format("{d:o}", 20);

    TEST_ASSERT(strcmp(s, "24") == 0);

    lily_free(s);
});

CASE(format_d_hex_specifier, {
    char *s = format("{d:x}", 20);

    TEST_ASSERT(strcmp(s, "14") == 0);

    lily_free(s);
});

CASE(format_f_specifier, {
    char *s = format("{f}", 2.2);

    TEST_ASSERT(strcmp(s, "2.200000") == 0);

    lily_free(s);
});

CASE(format_S_specifier, {
    String *string = from__String("hello");
    char *s = format("{S} world", string);

    TEST_ASSERT(strcmp(s, "hello world") == 0);

    FREE(String, string);
    lily_free(s);
});

CASE(format_Sr_specifier, {
    String *string = from__String("hello");
    char *s = format("{Sr} world", string);

    TEST_ASSERT(strcmp(s, "hello world") == 0);

    lily_free(s);
});