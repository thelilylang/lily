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
    HashMap *hm = NEW(HashMap); // HashMap<char*>*

    TEST_ASSERT(!insert__HashMap(hm, "1", "a"));
    TEST_ASSERT(!insert__HashMap(hm, "2", "b"));
    TEST_ASSERT(!insert__HashMap(hm, "3", "c"));

    TEST_ASSERT(!strcmp(get__HashMap(hm, "1"), "a"));
    TEST_ASSERT(!strcmp(get__HashMap(hm, "2"), "b"));
    TEST_ASSERT(!strcmp(get__HashMap(hm, "3"), "c"));

    FREE(HashMap, hm);
});

CASE(hash_map_insert, {
    HashMap *hm = NEW(HashMap); // HashMap<char*>*

    TEST_ASSERT(!insert__HashMap(hm, "1", "a"));
    TEST_ASSERT(!insert__HashMap(hm, "2", "b"));
    TEST_ASSERT(!insert__HashMap(hm, "3", "c"));
    TEST_ASSERT(!insert__HashMap(hm, "4", "d"));
    TEST_ASSERT(!insert__HashMap(hm, "5", "e"));
    TEST_ASSERT(!insert__HashMap(hm, "6", "f"));
    TEST_ASSERT(!insert__HashMap(hm, "7", "g"));
    TEST_ASSERT(!insert__HashMap(hm, "8", "h"));
    TEST_ASSERT(!insert__HashMap(hm, "9", "i"));

    FREE(HashMap, hm);
});

CASE(hash_map_remove, {
    HashMap *hm = NEW(HashMap); // HashMap<char*>*

    TEST_ASSERT(!insert__HashMap(hm, "1", "a"));
    TEST_ASSERT(!insert__HashMap(hm, "2", "b"));
    TEST_ASSERT(!insert__HashMap(hm, "3", "c"));

    TEST_ASSERT(remove__HashMap(hm, "1"));
    TEST_ASSERT(remove__HashMap(hm, "2"));
    TEST_ASSERT(remove__HashMap(hm, "3"));
    TEST_ASSERT(!remove__HashMap(hm, "4"));
    TEST_ASSERT(!remove__HashMap(hm, "5"));

    TEST_ASSERT(hm->len == 0);

    FREE(HashMap, hm);
});

SUITE(hash_map_iter);

CASE(hash_map_iter_next, {
    HashMap *hm = NEW(HashMap); // HashMap<char*>*

    TEST_ASSERT(!insert__HashMap(hm, "1", "a"));
    TEST_ASSERT(!insert__HashMap(hm, "2", "b"));
    TEST_ASSERT(!insert__HashMap(hm, "3", "c"));

    HashMapIter iter = NEW(HashMapIter, hm);
    char *current = NULL;
    bool enter_in_loop = false;

    while ((current = next__HashMapIter(&iter))) {
        enter_in_loop = true;

        TEST_ASSERT(!strcmp(current, "a") || !strcmp(current, "b") ||
                    !strcmp(current, "c"));
    }

    TEST_ASSERT(enter_in_loop);
    TEST_ASSERT(!next__HashMapIter(&iter));

    FREE(HashMap, hm);
});

CASE(hash_map_iter_next_pair, {
    HashMap *hm = NEW(HashMap); // HashMap<char*>*

    TEST_ASSERT(!insert__HashMap(hm, "1", "a"));
    TEST_ASSERT(!insert__HashMap(hm, "2", "b"));
    TEST_ASSERT(!insert__HashMap(hm, "3", "c"));

    HashMapIter iter = NEW(HashMapIter, hm);
    HashMapIterPair pair;
    bool enter_in_loop = false;

    while (!HASH_MAP_ITER_PAIR_IS_NULL(pair = next_pair__HashMapIter(&iter))) {
        enter_in_loop = true;

        TEST_ASSERT(!strcmp(pair.key, "1") || !strcmp(pair.key, "2") ||
                    !strcmp(pair.key, "3"));
        TEST_ASSERT(!strcmp(pair.value, "a") || !strcmp(pair.value, "b") ||
                    !strcmp(pair.value, "c"));
    }

    TEST_ASSERT(enter_in_loop);
    TEST_ASSERT(HASH_MAP_ITER_PAIR_IS_NULL(next_pair__HashMapIter(&iter)));

    FREE(HashMap, hm);
});
