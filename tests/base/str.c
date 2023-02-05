#include <base/assert.h>
#include <base/macros.h>
#include <base/str.h>
#include <base/types.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void
test_split__Str()
{
    char *s = "Hello\nWorld";
    char **split = split__Str(s, '\n');

    ASSERT(!strcmp(split[0], "Hello"));
    ASSERT(!strcmp(split[1], "World"));

    for (Usize i = 0; i < (Usize)LEN(split, *split); i++)
        free(split[i]);

    free(split);
}