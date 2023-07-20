#include <base/memory/global.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ALLOC_TIME 5.0

int main() {
	printf("Bench global allocator: allocate the most cells in ~5s\n");

    clock_t start = clock();

	while ((double)(clock() - start) / CLOCKS_PER_SEC < ALLOC_TIME) {
		void *s_block = MEMORY_GLOBAL_ALLOC(char, 100);
		*(char*)s_block = 'h';
	}

	print_stat__MemoryGlobal();

    printf("Time of free: %.2fs\n", ((double)(clock() - start) / CLOCKS_PER_SEC) - ALLOC_TIME);
}
