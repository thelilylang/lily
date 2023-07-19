#include <base/assert.h>
#include <base/macros.h>
#include <base/new.h>
#include <base/memory/global.h>

#include <stdio.h>
#include <stdlib.h>

void test_alloc__MemoryGlobal()
{
	MemoryGlobal global = NEW(MemoryGlobal);

	MemoryBlock *s_block = MEMORY_GLOBAL_ALLOC(char, &global, 5);

	char *s = s_block->mem;

	s[0] = 'a';
	s[1] = 'b';
	s[2] = 'c';
	s[3] = 'd';
	s[4] = 'e';

	for (int i = 0; i < 5; ++i) {
		ASSERT_EQ(s[i], 'a' + i);
	}

	MEMORY_GLOBAL_RESIZE(char, &global, s_block, 10);

	s[5] = 'f';
	s[6] = 'g';
	s[7] = 'h';
	s[8] = 'i';
	s[9] = 'j';

	for (int i = 0; i < 10; ++i) {
		ASSERT_EQ(s[i], 'a' + i);
	}

	MEMORY_GLOBAL_FREE(&global, s_block);
	destroy__MemoryGlobal(&global);

	// print_stat__MemoryGlobal(&global);
}
