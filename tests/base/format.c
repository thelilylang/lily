#include <base/assert.h>
#include <base/format.h>
#include <base/new.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void
test_s_specifier__Format()
{
    char *s = format("{s}", "Hello");

    ASSERT(strcmp(s, "Hello") == 0);

    free(s);
}

void
test_sa_specifier__Format()
{
    char *s = format("{sa}", format("{s}", "Hello"));

    ASSERT(strcmp(s, "Hello") == 0);

    free(s);
}

void
test_d_specifier__Format()
{
    char *s = format("{d}", 40);

    ASSERT(strcmp(s, "40") == 0);

    free(s);
}

void
test_d_binary_sepecifier__Format()
{
    char *s = format("{d:b}", 20);

    ASSERT(strcmp(s, "10100") == 0);

    free(s);
}

void
test_d_octal_specifier__Format()
{
    char *s = format("{d:o}", 20);

    ASSERT(strcmp(s, "24") == 0);

    free(s);
}

void
test_d_hex_specifier__Format()
{
    char *s = format("{d:x}", 20);

    ASSERT(strcmp(s, "14") == 0);

    free(s);
}
