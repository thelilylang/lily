#include "util.c"

#include <base/test.h>

SIMPLE(try, {
    RUN_PREPARSER(FILE_TRY);
    FREE_PREPARSER();
});
