#include <stdio.h>
#include <time.h>

#include <base/memory/arena.h>
#include <base/new.h>
#include <base/types.h>
#include <base/vec.h>

typedef struct Buffer
{
    Usize *mem;
    MemoryArena *arena;
    Usize len;
    Usize capacity;
} Buffer;

static struct MemoryApi api = {
    .align = __align__,
    .alloc = __alloc__,
    .resize = __resize__,
    .free = __free__,
};

void
push(Buffer *self, Usize item)
{
    if (!self->mem) {
        self->mem = api.alloc(self->capacity * sizeof(Usize), sizeof(Usize));
        // self->mem = alloc__MemoryArena(self->arena, sizeof(Usize) *
        // self->capacity);
    } else if (self->len == self->capacity) {
        Usize new_capacity = self->capacity * 2;

        self->mem = api.resize(self->mem,
                               self->capacity * sizeof(Usize),
                               new_capacity * sizeof(Usize),
                               sizeof(Usize));
        // self->mem = resize__MemoryArena(self->arena, self->mem, sizeof(Usize)
        // * new_capacity);

        self->capacity = new_capacity;
    }

    self->mem[self->len++] = item;
}

#define CAPACITY 23000000000

int
main()
{
    printf("Bench: push 1'000'000'000 items\n");

    MemoryArena arena = NEW(MemoryArena, CAPACITY);
    Buffer buffer = (Buffer){
        .mem = NULL,
        .arena = &arena,
        .len = 0,
        .capacity = 8,
    };

    for (Usize i = 0; i < 1000000000; ++i) {
        push(&buffer, i);
    }

    // for (Usize i = 0; i < 1000000000; ++i) {
    // printf("%zu\n", buffer.mem[i]);
    // }

    api.free((void **)&buffer.mem, buffer.capacity, sizeof(Usize));

    // print_stat__MemoryArena(&arena);
    // destroy__MemoryArena(&arena);

    // {
    //     clock_t start = clock();
    //     Vec *v = NEW(Vec);

    //     for (Usize i = 0; i < 1000000000; i++) {
    //         push__Vec(v, (Usize *)i);
    //     }

    //     FREE(Vec, v);

    //     printf("Time: %.2fs\n", (double)(clock() - start) / CLOCKS_PER_SEC);
    // }

    // printf("Bench: push 1'000'000'000 items and remove 1'000'000'000\n");

    // {
    //     clock_t start = clock();
    //     Vec *v = NEW(Vec);

    //     for (Usize i = 0; i < 1000000000; i++) {
    //         push__Vec(v, (Usize *)i);
    //     }

    //     for (Usize i = 0; i < 1000000000; i++) {
    //         pop__Vec(v);
    //     }

    //     FREE(Vec, v);

    //     printf("Time: %.2fs\n", (double)(clock() - start) / CLOCKS_PER_SEC);
    // }
}
