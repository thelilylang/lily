#include "atoi.c"
#include "format.c"
#include "hash_map.c"
#include "hash_set.c"
#include "itoa.c"
#include "str.c"
#include "string.c"
#include "vec.c"

#include <stdio.h>

int
main()
{
    // Test atoi
    test_check_int8_overflow__Atoi();
    test_check_int16_overflow__Atoi();
    test_check_int32_overflow__Atoi();
    test_check_int64_overflow__Atoi();
    test_check_uint8_overflow__Atoi();
    test_check_uint16_overflow__Atoi();
    test_check_uint32_overflow__Atoi();
    test_check_uint64_overflow__Atoi();
    test_atoi__Atoi();
    test_atoi_safe__Atoi();

    // Test format
    test_s_specifier__Format();
    test_sa_specifier__Format();
    test_d_specifier__Format();
    test_d_binary_sepecifier__Format();
    test_d_octal_specifier__Format();
    test_f_specifier__Format();
    test_S_specifier__Format();
    test_Sr_specifier__Format();

    // Test HashMap
    test_new__HashMap();
    test_get__HashMap();
    test_insert__HashMap();

    // Test HashSet
    test_new__HashSet();

    // Test itoa
    test_itoa_base_10();
    test_itoa_base_2();
    test_itoa_base_8();
    test_itoa_base_16();

    // Test Str
    test_split__Str();

    // Test String
    test_new__String();
    test_from__String();
    test_split__String();
    test_pop__String();
    test_push__String();

    // Test Vec
    test_init__Vec();
    test_new__Vec();
    test_from__Vec();
    test_get__Vec();
    test_pop__Vec();
    test_push__Vec();
    test_remove__Vec();
    test_reverse__Vec();

    puts("\x1b[32mAll tests pass\x1b[0m");
}
