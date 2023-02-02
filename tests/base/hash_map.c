#include <base/assert.h>
#include <base/hash_map.h>
#include <base/new.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void
test_new__HashMap()
{
    HashMap *hm = NEW(HashMap);

    ASSERT(hm->len == 0);

    FREE(HashMap, hm);
}

void
test_get__HashMap()
{
    HashMap *hm = NEW(HashMap); // HashMap<int, char*>

    insert__HashMap(hm, (int *)1, "a");
    insert__HashMap(hm, (int *)2, "b");
    insert__HashMap(hm, (int *)3, "c");

    ASSERT(strcmp(get__HashMap(hm, (int *)1), "a") == 0);
    ASSERT(strcmp(get__HashMap(hm, (int *)2), "b") == 0);
    ASSERT(strcmp(get__HashMap(hm, (int *)3), "c") == 0);

    FREE(HashMap, hm);
}

void
test_insert__HashMap()
{
    HashMap *hm = NEW(HashMap); // HashMap<int, char*>

    ASSERT(insert__HashMap(hm, (int *)1, "a") == NULL);
    ASSERT(insert__HashMap(hm, (int *)2, "b") == NULL);
    ASSERT(insert__HashMap(hm, (int *)3, "c") == NULL);

    ASSERT(strcmp(insert__HashMap(hm, (int *)1, "d"), "a") == 0);

    ASSERT(hm->len == 3);

    FREE(HashMap, hm);
}
