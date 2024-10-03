#include "allocator.c"
#include "atof.c"
#include "atoi.c"
#include "buffer.c"
#include "format.c"
#include "hash_map.c"
#include "hash_set.c"
#include "itoa.c"
#include "memory/arena.c"
#include "memory/global.c"
#include "memory/page.c"
#include "stack.c"
#include "str.c"
#include "string.c"
#include "vec.c"

#include <base/test.h>

#include <stdio.h>

int
main()
{
    NEW_TEST("base");
    ADD_SUITE(1, allocator, CALL_CASE(allocator_alloc));
    ADD_SUITE(10,
              atoi,
              CALL_CASE(atoi_check_int8_overflow),
              CALL_CASE(atoi_check_int16_overflow),
              CALL_CASE(atoi_check_int32_overflow),
              CALL_CASE(atoi_check_int64_overflow),
              CALL_CASE(atoi_check_uint8_overflow),
              CALL_CASE(atoi_check_uint16_overflow),
              CALL_CASE(atoi_check_uint32_overflow),
              CALL_CASE(atoi_check_uint64_overflow),
              CALL_CASE(atoi),
              CALL_CASE(atoi_safe));
    ADD_SUITE(2, atof, CALL_CASE(atof__Float32), CALL_CASE(atof__Float64));
    ADD_SUITE(1, buffer, CALL_CASE(buffer_push));
    ADD_SUITE(9,
              format,
              CALL_CASE(format_s_specifier),
              CALL_CASE(format_sa_specifier),
              CALL_CASE(format_d_specifier),
              CALL_CASE(format_d_binary_specifier),
              CALL_CASE(format_d_octal_specifier),
              CALL_CASE(format_d_hex_specifier),
              CALL_CASE(format_f_specifier),
              CALL_CASE(format_S_specifier),
              CALL_CASE(format_Sr_specifier));
    ADD_SUITE(3,
              hash_map,
              CALL_CASE(hash_map_new),
              CALL_CASE(hash_map_get),
              CALL_CASE(hash_map_insert));
    ADD_SUITE(1, hash_set, CALL_CASE(hash_set_new));
    ADD_SUITE(4,
              itoa,
              CALL_CASE(itoa_base_10),
              CALL_CASE(itoa_base_2),
              CALL_CASE(itoa_base_8),
              CALL_CASE(itoa_base_16));
    ADD_SUITE(1, memory_arena, CALL_CASE(memory_arena_alloc));
    ADD_SUITE(1, memory_global, CALL_CASE(memory_global_alloc));
    ADD_SUITE(1, memory_page, CALL_CASE(memory_page_alloc));
    ADD_SUITE(4,
              stack,
              CALL_CASE(stack_new),
              CALL_CASE(stack_push),
              CALL_CASE(stack_pop),
              CALL_CASE(stack_empty));
    ADD_SUITE(2, str, CALL_CASE(str_split), CALL_CASE(str_get_slice));
    ADD_SUITE(7,
              string,
              CALL_CASE(string_new),
              CALL_CASE(string_clone),
              CALL_CASE(string_from),
              CALL_CASE(string_get_slice),
              CALL_CASE(string_split),
              CALL_CASE(string_pop),
              CALL_CASE(string_push));
    ADD_SUITE(10,
              vec,
              CALL_CASE(vec_init),
              CALL_CASE(vec_new),
              CALL_CASE(vec_from),
              CALL_CASE(vec_get),
              CALL_CASE(vec_insert),
              CALL_CASE(vec_insert_after),
              CALL_CASE(vec_pop),
              CALL_CASE(vec_push),
              CALL_CASE(vec_remove),
              CALL_CASE(vec_reverse));
    RUN_TEST();
}
