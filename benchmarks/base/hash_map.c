#include <time.h>
#include <stdio.h>

#include <base/hash_map.h>
#include <base/new.h>
#include <base/types.h>

int main() {
	printf("Bench: push 1'000'000'000 items\n");

	{
		clock_t start = clock();
		HashMap *hm = NEW(HashMap);

		for (Usize i = 0; i < 1'000'000'000; i++) {
			insert__HashMap(hm, (Usize*)i, "a");
		}

		FREE(HashMap, hm);

		printf("Time: %.2fs\n", (double)(clock() - start) / CLOCKS_PER_SEC);
	}
}
