#include <base/allocator.h>
#include <base/assert.h>
#include <base/macros.h>
#include <base/new.h>

#include <stdio.h>
#include <stdlib.h>

void
test_alloc__Allocator()
{
    Allocator alloc = ARENA_ALLOCATOR();

    MemoryBlock *s_block = A_ALLOC(char, alloc, 5);
    char *s = s_block->mem;

    s[0] = 'a';
    s[1] = 'b';
    s[2] = 'c';
    s[3] = 'd';
    s[4] = 'e';

    for (int i = 0; i < 5; ++i) {
        ASSERT_EQ(s[i], 'a' + i);
    }

    A_RESIZE(char, alloc, s_block, 10);

    s[5] = 'f';
    s[6] = 'g';
    s[7] = 'h';
    s[8] = 'i';
    s[9] = 'j';

    for (int i = 0; i < 10; ++i) {
        ASSERT_EQ(s[i], 'a' + i);
    }

    destroy__Allocator(&alloc);

    // A_PRINT_STAT(alloc);
}
