#include <base/assert.h>
#include <base/new.h>
#include <base/string.h>
#include <base/test.h>
#include <base/vec_bit.h>

SUITE(vec_bit);

CASE(vec_bit_add, {
    VecBit *vec_bit = NEW(VecBit);

    for (Usize i = 0; i < 1000; ++i) {
        add__VecBit(vec_bit, i);
    }

    FREE(VecBit, vec_bit);
});

CASE(vec_bit_has, {
    VecBit *vec_bit = NEW(VecBit);

    add__VecBit(vec_bit, 10);

    TEST_ASSERT(has__VecBit(vec_bit, 10));
    TEST_ASSERT(!has__VecBit(vec_bit, 8));
    TEST_ASSERT(!has__VecBit(vec_bit, 50));

    add__VecBit(vec_bit, 50);

    TEST_ASSERT(has__VecBit(vec_bit, 50));

    FREE(VecBit, vec_bit);
});

CASE(vec_bit_remove, {
    VecBit *vec_bit = NEW(VecBit);

    add__VecBit(vec_bit, 10);

    TEST_ASSERT(has__VecBit(vec_bit, 10));

    remove__VecBit(vec_bit, 10);

    TEST_ASSERT(!has__VecBit(vec_bit, 10));

    FREE(VecBit, vec_bit);
});
