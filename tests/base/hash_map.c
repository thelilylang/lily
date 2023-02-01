#include <base/assert.h>
#include <base/new.h>
#include <base/hash_map.h>

#include <stdio.h>
#include <stdlib.h>

void test_new__HashMap() {
	HashMap *hm = NEW(HashMap);

	ASSERT(hm->len == 0);

	FREE(HashMap, hm);
}
