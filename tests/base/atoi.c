#include <base/assert.h>
#include <base/atoi.h>
#include <base/new.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void
test_atoi()
{
    ASSERT(atoi__Int32("123") == 123);
    ASSERT(atoi__Int32("-123") == -123);
}
