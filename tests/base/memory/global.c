#include <base/assert.h>
#include <base/macros.h>
#include <base/memory/global.h>
#include <base/new.h>

#include <stdio.h>
#include <stdlib.h>

void
test_alloc__MemoryGlobal()
{
    char *s = MEMORY_GLOBAL_ALLOC(char, 5);

    s[0] = 'a';
    s[1] = 'b';
    s[2] = 'c';
    s[3] = 'd';
    s[4] = 'e';

    for (int i = 0; i < 5; ++i) {
        ASSERT_EQ(s[i], 'a' + i);
    }

    // s = MEMORY_GLOBAL_RESIZE(char, s, 10);

    // s[5] = 'f';
    // s[6] = 'g';
    // s[7] = 'h';
    // s[8] = 'i';
    // s[9] = 'j';

    // for (int i = 0; i < 10; ++i) {
    // 	ASSERT_EQ(s[i], 'a' + i);
    // }

    // char *s2 = MEMORY_GLOBAL_ALLOC(char, 6);

    // s2[0] = 'h';
    // s2[1] = 'e';
    // s2[2] = 'l';
    // s2[3] = 'l';
    // s2[4] = 'o';

    // MEMORY_GLOBAL_FREE(s);
    // MEMORY_GLOBAL_FREE(s2);

    // print_stat__MemoryGlobal(&global);
}
