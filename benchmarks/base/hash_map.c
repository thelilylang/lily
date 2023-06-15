#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <base/hash/fnv.h>
#include <base/hash_map.h>
#include <base/new.h>
#include <base/types.h>

int
main()
{
    printf("Bench: push 1'000'000 items\n");

    {
        clock_t start = clock();
        HashMap *hm = NEW(HashMap);

        for (Usize i = 0; i < 1000000; i++) {
            char *key = malloc(7);
            snprintf(key, 7, "%zu", i);

            insert__HashMap(hm, key, (Usize *)i);
        }

        // We have some memory leaks, because the keys are not free.
        FREE(HashMap, hm);

        printf("Time: %.2fs\n", (double)(clock() - start) / CLOCKS_PER_SEC);
    }
}
