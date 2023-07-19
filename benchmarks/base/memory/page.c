#include <base/memory/page.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BYTES 4300000000

int main() {
	printf("Bench page allocator: alloc ~4.0046 GiB\n");

    clock_t start = clock();

	MemoryPage page = NEW(MemoryPage);

	MemoryBlock *s_block = MEMORY_PAGE_ALLOC(char, &page, BYTES);
	char *s = s_block->mem;

	for (Usize i = 0; i < BYTES; ++i) {
		s[i] = i % 127;
	}

	destroy__MemoryPage(&page);
	print_stat__MemoryPage(&page);

    printf("Time: %.2fs\n", (double)(clock() - start) / CLOCKS_PER_SEC);
}
