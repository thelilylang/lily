#include "util.c"

#include <base/test.h>

SIMPLE(record, {
    RUN_PREPARSER(FILE_RECORD);
    FREE_PREPARSER();
});
