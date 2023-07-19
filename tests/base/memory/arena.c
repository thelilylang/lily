#include <base/assert.h>
#include <base/macros.h>
#include <base/new.h>
#include <base/memory/arena.h>

#include <stdio.h>
#include <stdlib.h>

void
test_alloc__MemoryArena()
{
	MemoryArena arena = from_capacity__MemoryArena(1024);

	MemoryBlock block = alloc__MemoryArena(&arena, sizeof(int) * 5);
	int *arr = block.mem;

	arr[0] = 1;
	arr[1] = 2;
	arr[2] = 3;
	arr[3] = 4;
	arr[4] = 5;

	for (int i = 0; i < 5; ++i) {
		ASSERT_EQ(i + 1, arr[i]);
	}

	block = resize__MemoryArena(&arena, &block, sizeof(int) * 10);
	arr = block.mem;

	arr[5] = 6;
	arr[6] = 7;
	arr[7] = 8;
	arr[8] = 9;
	arr[9] = 10;

	for (int i = 0; i < 10; ++i) {
		ASSERT_EQ(i + 1, arr[i]);
	}

	// print_stat__MemoryArena(&arena);

	destroy__MemoryArena(&arena);
}
