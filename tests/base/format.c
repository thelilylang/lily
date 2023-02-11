#include <base/assert.h>
#include <base/format.h>
#include <base/new.h>
#include <base/string.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void
test_s_specifier__Format()
{
    char *s = format("{s}", "Hello");

    ASSERT(strcmp(s, "Hello") == 0);

    lily_free(s);
}

void
test_sa_specifier__Format()
{
    char *s = format("{sa}", format("{s}", "Hello"));

    ASSERT(strcmp(s, "Hello") == 0);

    lily_free(s);
}

void
test_d_specifier__Format()
{
    char *s = format("{d}", 40);

    ASSERT(strcmp(s, "40") == 0);

    lily_free(s);
}

void
test_d_binary_sepecifier__Format()
{
    char *s = format("{d:b}", 20);

    ASSERT(strcmp(s, "10100") == 0);

    lily_free(s);
}

void
test_d_octal_specifier__Format()
{
    char *s = format("{d:o}", 20);

    ASSERT(strcmp(s, "24") == 0);

    lily_free(s);
}

void
test_d_hex_specifier__Format()
{
    char *s = format("{d:x}", 20);

    ASSERT(strcmp(s, "14") == 0);

    lily_free(s);
}

void
test_f_specifier__Format()
{
    char *s = format("{f}", 2.2);

    ASSERT(strcmp(s, "2.200000") == 0);

    lily_free(s);
}

void
test_S_specifier__Format()
{
    String *string = from__String("hello");
    char *s = format("{S} world", string);

    ASSERT(strcmp(s, "hello world") == 0);

    FREE(String, string);
    lily_free(s);
}

void
test_Sr_specifier__Format()
{
    String *string = from__String("hello");
    char *s = format("{Sr} world", string);

    ASSERT(strcmp(s, "hello world") == 0);

    lily_free(s);
}