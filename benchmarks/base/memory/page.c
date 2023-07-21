#include <base/memory/page.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BYTES 4300000000

int main() {
	printf("Bench page allocator: alloc ~4.0046 GiB\n");

    clock_t start = clock();

	MemoryPage page = NEW(MemoryPage);

	char *s = MEMORY_PAGE_ALLOC(char, &page, BYTES);

	for (Usize i = 0; i < BYTES; ++i) {
		s[i] = i % 127;
	}

	free__MemoryPage(&page);

    printf("Time: %.2fs\n", (double)(clock() - start) / CLOCKS_PER_SEC);
}
