#include <base/assert.h>
#include <base/hash_set.h>
#include <base/new.h>
#include <base/test.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

SUITE(hash_set);

CASE(hash_set_new, {
    HashSet *hs = NEW(HashSet);

    TEST_ASSERT(hs->len == 0);

    FREE(HashSet, hs);
});