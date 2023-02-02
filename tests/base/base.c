#include "atoi.c"
#include "format.c"
#include "hash_map.c"
#include "hash_set.c"
#include "itoa.c"
#include "string.c"
#include "vec.c"

#include <stdio.h>

int
main()
{
    // Test atoi
    test_atoi();

    // Test format
    test_format_s_specifier();
    test_format_sa_specifier();

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

    // Test String
    test_new__String();
    test_from__String();
    test_pop__String();
    test_push__String();

    // Test Vec
    test_new__Vec();
    test_from__Vec();
    test_get__Vec();
    test_pop__Vec();
    test_push__Vec();
    test_remove__Vec();
    test_reverse__Vec();

    puts("\x1b[32mAll tests pass\x1b[0m");
}
