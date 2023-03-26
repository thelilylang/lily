#include "comment_doc.c"
#include "identifier_dollar.c"
#include "identifier_macro.c"
#include "identifier_normal.c"
#include "identifier_operator.c"
#include "keyword.c"
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
    ADD_SIMPLE(comment_doc);
    ADD_SIMPLE(identifier_dollar);
    ADD_SIMPLE(identifier_macro);
    ADD_SIMPLE(identifier_normal);
    ADD_SIMPLE(separator);
    ADD_SIMPLE(operator);
    RUN_TEST();
}
