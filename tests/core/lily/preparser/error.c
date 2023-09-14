#include "util.c"

#include <base/test.h>

SIMPLE(error, {
    RUN_PREPARSER(FILE_ERROR);
    FREE_PREPARSER();
});
