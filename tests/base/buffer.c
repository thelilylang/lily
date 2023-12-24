#include <base/assert.h>
#include <base/buffer.h>
#include <base/new.h>
#include <base/test.h>

#include <stdio.h>
#include <stdlib.h>

typedef Buffer(int) BufferInt;

SUITE(buffer);

CASE(buffer_push, {
    Allocator allocator = GLOBAL_ALLOCATOR();
    BufferInt buffer = __new__Buffer(&allocator, 4, 2);

    push__Buffer(buffer, 1);
    push__Buffer(buffer, 2);
    push__Buffer(buffer, 3);
    push__Buffer(buffer, 4);
    push__Buffer(buffer, 5);

    for (Usize i = 0; i < buffer.len; ++i) {
        TEST_ASSERT_EQ(buffer.mem[i], i + 1);
    }

    push__Buffer(buffer, 6);
    push__Buffer(buffer, 7);
    push__Buffer(buffer, 8);
    push__Buffer(buffer, 9);
    push__Buffer(buffer, 10);

    for (Usize i = 0; i < buffer.len; ++i) {
        TEST_ASSERT_EQ(buffer.mem[i], i + 1);
    }
});