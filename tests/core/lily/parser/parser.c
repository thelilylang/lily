#include "access.c"
#include "array.c"
#include "binary.c"
#include "call.c"
#include "cast.c"
#include "data_type.c"
#include "expr.c"
#include "identifier.c"
#include "literal.c"
#include "tuple.c"
#include "unary.c"

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
    // NOTE: For the moment, we're not trying to test the lambda (already tested
    // in the sample).
    ADD_SUITE(30,
              expr,
              CALL_CASE(expr_array),
              CALL_CASE(expr_array2),
              CALL_CASE(expr_array3),
              CALL_CASE(expr_array4),
              CALL_CASE(expr_binary),
              CALL_CASE(expr_binary2),
              CALL_CASE(expr_binary3),
              CALL_CASE(expr_access),
              CALL_CASE(expr_access2),
              CALL_CASE(expr_access3),
              CALL_CASE(expr_access4),
              CALL_CASE(expr_access5),
              CALL_CASE(expr_call),
              CALL_CASE(expr_call2),
              CALL_CASE(expr_call3),
              CALL_CASE(expr_call4),
              CALL_CASE(expr_call5),
              CALL_CASE(expr_cast),
              CALL_CASE(expr_identifier),
              CALL_CASE(expr_literal),
              CALL_CASE(expr_literal2),
              CALL_CASE(expr_literal3),
              CALL_CASE(expr_literal4),
              CALL_CASE(expr_literal5),
              CALL_CASE(expr_literal6),
              CALL_CASE(expr_tuple),
              CALL_CASE(expr_unary),
              CALL_CASE(expr_unary2),
              CALL_CASE(expr_unary3),
              CALL_CASE(expr_unary4));
    RUN_TEST();
}
