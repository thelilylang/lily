#include "import.c"
#include "macro.c"

#include <base/test.h>

int
main()
{
    NEW_TEST("preparser");
    ADD_SIMPLE(import);
    ADD_SIMPLE(macro);
    RUN_TEST();
}
