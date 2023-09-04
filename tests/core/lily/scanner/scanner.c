#include "at_keyword.c"
#include "comment_doc.c"
#include "identifier_dollar.c"
#include "identifier_macro.c"
#include "identifier_normal.c"
#include "identifier_operator.c"
#include "keyword.c"
#include "literal.c"
#include "literal_byte.c"
#include "literal_bytes.c"
#include "literal_char.c"
#include "literal_int_10.c"
#include "literal_int_16.c"
#include "literal_int_2.c"
#include "literal_int_8.c"
#include "literal_string.c"
#include "literal_suffix_float32.c"
#include "literal_suffix_float64.c"
#include "literal_suffix_int16.c"
#include "literal_suffix_int32.c"
#include "literal_suffix_int64.c"
#include "literal_suffix_int8.c"
#include "literal_suffix_isize.c"
#include "literal_suffix_uint16.c"
#include "literal_suffix_uint32.c"
#include "literal_suffix_uint64.c"
#include "literal_suffix_uint8.c"
#include "literal_suffix_usize.c"
#include "operator.c"
#include "other_comment.c"
#include "separator.c"

#include <base/test.h>

int
main()
{
    NEW_TEST("scanner");
    ADD_SIMPLE(at_keyword);
    ADD_SIMPLE(comment_doc);
    ADD_SIMPLE(identifier_dollar);
    ADD_SIMPLE(identifier_macro);
    ADD_SIMPLE(identifier_normal);
    ADD_SIMPLE(identifier_operator);
    ADD_SIMPLE(keyword);
    ADD_SIMPLE(separator);
    ADD_SIMPLE(operator);
    ADD_SUITE(7,
              literal,
              CALL_CASE(literal_byte),
              CALL_CASE(literal_bytes),
              CALL_CASE(literal_char),
              CALL_CASE(literal_int_10),
              CALL_CASE(literal_int_16),
              CALL_CASE(literal_int_2),
              CALL_CASE(literal_int_8));
    RUN_TEST();
}
