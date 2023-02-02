#include <base/assert.h>
#include <base/format.h>
#include <base/new.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void
test_format_s_specifier()
{
    char *s = format("{s}", "Hello");

    ASSERT(strcmp(s, "Hello") == 0);

    free(s);
}

void
test_format_sa_specifier()
{
    char *s = format("{sa}", format("{s}", "Hello"));

    ASSERT(strcmp(s, "Hello") == 0);

    free(s);
}
