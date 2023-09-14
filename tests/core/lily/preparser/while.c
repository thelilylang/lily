#include "util.c"

#include <base/test.h>

SIMPLE(while, {
    RUN_PREPARSER(FILE_WHILE);
    FREE_PREPARSER();
});
