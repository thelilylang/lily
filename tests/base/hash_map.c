#include <base/assert.h>
#include <base/hash_map.h>
#include <base/new.h>
#include <base/test.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

SUITE(hash_map);

CASE(hash_map_new, {
    HashMap *hm = NEW(HashMap);

    TEST_ASSERT(hm->len == 0);

    FREE(HashMap, hm);
});

CASE(hash_map_get, {
    HashMap *hm = NEW(HashMap); // HashMap<char*>

    insert__HashMap(hm, "1", "a");
    insert__HashMap(hm, "2", "b");
    insert__HashMap(hm, "3", "c");

    TEST_ASSERT(!strcmp(get__HashMap(hm, "1"), "a"));
    TEST_ASSERT(!strcmp(get__HashMap(hm, "2"), "b"));
    TEST_ASSERT(!strcmp(get__HashMap(hm, "3"), "c"));

    FREE(HashMap, hm);
});

CASE(hash_map_insert, {
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
});
