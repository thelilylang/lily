#include <base/assert.h>
#include <base/macros.h>
#include <base/memory/page.h>
#include <base/new.h>

#include <stdio.h>
#include <stdlib.h>

void
test_alloc__MemoryPage()
{
    MemoryPage page = NEW(MemoryPage);

    char *s = MEMORY_PAGE_ALLOC(char, &page, 5);

    s[0] = 'a';
    s[1] = 'b';
    s[2] = 'c';
    s[3] = 'd';
    s[4] = 'e';

    for (int i = 0; i < 5; ++i) {
        ASSERT_EQ(s[i], 'a' + i);
    }

    s = MEMORY_PAGE_RESIZE(char, &page, 10);

    s[5] = 'f';
    s[6] = 'g';
    s[7] = 'h';
    s[8] = 'i';
    s[9] = 'j';

    for (int i = 0; i < 10; ++i) {
        ASSERT_EQ(s[i], 'a' + i);
    }
}
