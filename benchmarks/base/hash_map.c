#include <stdio.h>
#include <time.h>
#include <string.h>

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
            char key[7];
            snprintf(key, 7, "%zu", i);

            insert__HashMap(hm, key, (Usize*)i);
        }

		printf("%zu\n", (Usize)(Uptr)get__HashMap(hm, "2"));

        FREE(HashMap, hm);

        printf("Time: %.2fs\n", (double)(clock() - start) / CLOCKS_PER_SEC);
    }
}
