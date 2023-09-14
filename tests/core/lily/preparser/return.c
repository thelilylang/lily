#include "util.c"

#include <base/test.h>

SIMPLE(return, {
    RUN_PREPARSER(FILE_RETURN);
    FREE_PREPARSER();
});
