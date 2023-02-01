#include <base/assert.h>
#include <base/new.h>
#include <base/hash_map.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_new__HashMap() {
	HashMap *hm = NEW(HashMap);

	ASSERT(hm->len == 0);

	FREE(HashMap, hm);
}

void test_insert__HashMap() {
	HashMap *hm = NEW(HashMap); // HashMap<int, char*>

	ASSERT(insert__HashMap(hm, (int*)1, "a") == NULL);
	ASSERT(insert__HashMap(hm, (int*)2, "b") == NULL);
	ASSERT(insert__HashMap(hm, (int*)3, "c") == NULL);

	ASSERT(strcmp(insert__HashMap(hm, (int*)1, "d"), "a") == 0);

	ASSERT(hm->len == 3);

	FREE(HashMap, hm);
}
