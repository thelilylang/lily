#include "array.c"
#include "data_type.c"
#include "expr.c"

#include <base/test.h>

int
main()
{
    NEW_TEST("parser");
    ADD_SUITE(11,
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
              CALL_CASE(data_type_tuple),
              CALL_CASE(data_type_custom));
    ADD_SUITE(2, expr, CALL_CASE(expr_array), CALL_CASE(expr_array2));
    RUN_TEST();
}
