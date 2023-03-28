#include "data_type.c"

#include <base/test.h>

int
main()
{
    NEW_TEST("parser");
    ADD_SUITE(10,
              data_type,
              CALL_CASE(data_type_array_dynamic),
              CALL_CASE(data_type_array_dynamic2),
              CALL_CASE(data_type_array_sized),
              CALL_CASE(data_type_array_sized2),
              CALL_CASE(data_type_array_multi_pointers),
              CALL_CASE(data_type_array_multi_pointers2),
              CALL_CASE(data_type_array_undetermined),
              CALL_CASE(data_type_array_undetermined2),
			  CALL_CASE(data_type_lambda),
			  CALL_CASE(data_type_tuple));
    RUN_TEST();
}
