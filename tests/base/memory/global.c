#include <base/assert.h>
#include <base/macros.h>
#include <base/new.h>
#include <base/memory/global.h>

#include <stdio.h>
#include <stdlib.h>

void test_alloc__MemoryGlobal()
{
	MemoryGlobal global = NEW(MemoryGlobal);

	MemoryLayout *s_layout = alloc__MemoryGlobal(&global, 5);

	char *s = s_layout->mem;

	s[0] = 'a';
	s[1] = 'b';
	s[2] = 'c';
	s[3] = 'd';
	s[4] = 'e';

	for (int i = 0; i < 5; ++i) {
		ASSERT_EQ(s[i], 'a' + i);
	}

	resize__MemoryGlobal(&global, s_layout, 10);

	s[5] = 'f';
	s[6] = 'g';
	s[7] = 'h';
	s[8] = 'i';
	s[9] = 'j';

	for (int i = 0; i < 10; ++i) {
		ASSERT_EQ(s[i], 'a' + i);
	}

	free__MemoryGlobal(&global, s_layout);
	destroy__MemoryGlobal(&global);

	// print_stat__MemoryGlobal(&global);
}
