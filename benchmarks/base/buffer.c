#include <base/assert.h>
#include <base/buffer.h>
#include <base/new.h>
#include <base/types.h>
#include <base/vec.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef Buffer(Usize) BufferUsize;

int
main()
{
    printf("Bench: push 1'000'000'000 items\n");

    clock_t start = clock();
    Allocator allocator = GLOBAL_ALLOCATOR();
    BufferUsize buffer = NEW(Buffer, &allocator, 4, 2);

    for (Usize i = 0; i < 1000000000; ++i) {
        push__Buffer(buffer, i);
    }

    __free__Buffer(buffer);
    print_stat__MemoryGlobal();

    printf("Time: %.2fs\n", (double)(clock() - start) / CLOCKS_PER_SEC);
}
