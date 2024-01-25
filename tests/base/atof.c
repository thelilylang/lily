#include <base/assert.h>
#include <base/atof.h>
#include <base/test.h>

SUITE(atof);

CASE(atof__Float32, {
    TEST_ASSERT(atof__Float32("3.33") == (float)3.33);
    TEST_ASSERT(atof__Float32("2.2") == (float)2.2);
    TEST_ASSERT(atof__Float32("0.33") == (float)0.33);
    TEST_ASSERT(atof__Float32("111.222") == (float)111.222);
});

CASE(atof__Float64, {
    TEST_ASSERT(atof__Float64("3.33") == 3.33);
    TEST_ASSERT(atof__Float64("2.2") == 2.2);
    TEST_ASSERT(atof__Float64("0.33") == 0.33);
    TEST_ASSERT(atof__Float64("111.222") == 111.222);
});
