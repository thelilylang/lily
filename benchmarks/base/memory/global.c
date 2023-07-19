#include <base/memory/global.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ALLOC_TIME 5.0

int main() {
	printf("Bench global allocator: allocate the most cells in ~5s\n");

    clock_t start = clock();
	MemoryGlobal global = NEW(MemoryGlobal);

	while ((double)(clock() - start) / CLOCKS_PER_SEC < ALLOC_TIME) {
		MemoryBlock *s_block = MEMORY_GLOBAL_ALLOC(char, &global, 100);
		*(char*)s_block->mem = 'h';
	}

	destroy__MemoryGlobal(&global);
	print_stat__MemoryGlobal(&global);

    printf("Time of free: %.2fs\n", ((double)(clock() - start) / CLOCKS_PER_SEC) - ALLOC_TIME);
}
