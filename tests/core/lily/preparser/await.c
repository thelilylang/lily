#include "util.c"

#include <base/test.h>

SIMPLE(await, {
    RUN_PREPARSER(FILE_AWAIT);
    FREE_PREPARSER();
});
