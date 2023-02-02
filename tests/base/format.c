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
