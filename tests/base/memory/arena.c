#include <base/assert.h>
#include <base/macros.h>
#include <base/new.h>
#include <base/memory/arena.h>

#include <stdio.h>
#include <stdlib.h>

void
test_alloc__MemoryArena()
{
	MemoryArena arena = NEW(MemoryArena, 1024);

	int *arr = MEMORY_ARENA_ALLOC(int, &arena, 5);

	arr[0] = 1;
	arr[1] = 2;
	arr[2] = 3;
	arr[3] = 4;
	arr[4] = 5;

	for (int i = 0; i < 5; ++i) {
		ASSERT_EQ(i + 1, arr[i]);
	}

	arr = MEMORY_ARENA_RESIZE(int, &arena, arr, 10);

	arr[5] = 6;
	arr[6] = 7;
	arr[7] = 8;
	arr[8] = 9;
	arr[9] = 10;

	for (int i = 0; i < 10; ++i) {
		ASSERT_EQ(i + 1, arr[i]);
	}

	destroy__MemoryArena(&arena);

	// print_stat__MemoryArena(&arena);
}
