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
    HashMap *hm = NEW(HashMap); // HashMap<char*>

    insert__HashMap(hm, "1", "a");
    insert__HashMap(hm, "2", "b");
    insert__HashMap(hm, "3", "c");

    ASSERT(!strcmp(get__HashMap(hm, "1"), "a"));
    ASSERT(!strcmp(get__HashMap(hm, "2"), "b"));
    ASSERT(!strcmp(get__HashMap(hm, "3"), "c"));

    FREE(HashMap, hm);
}

void
test_insert__HashMap()
{
    HashMap *hm = NEW(HashMap); // HashMap<char*>

    insert__HashMap(hm, "1", "a");
    insert__HashMap(hm, "2", "b");
    insert__HashMap(hm, "3", "c");
    insert__HashMap(hm, "4", "d");
    insert__HashMap(hm, "5", "e");
    insert__HashMap(hm, "6", "f");
    insert__HashMap(hm, "7", "g");
    insert__HashMap(hm, "8", "h");
    insert__HashMap(hm, "9", "i");

    FREE(HashMap, hm);
}
