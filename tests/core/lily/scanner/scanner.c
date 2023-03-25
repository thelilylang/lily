#include "operator.c"
#include "separator.c"

#include <base/test.h>

int
main()
{
    NEW_TEST("scanner");
    ADD_SIMPLE(separator);
    ADD_SIMPLE(operator);
    RUN_TEST();
}