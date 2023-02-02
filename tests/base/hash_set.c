#include <base/assert.h>
#include <base/hash_set.h>
#include <base/new.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void
test_new__HashSet()
{
    HashSet *hs = NEW(HashSet);

    ASSERT(hs->len == 0);

    FREE(HashSet, hs);
}
