#include "string.c"
#include "vec.c"

#include <stdio.h>

int main() {
	test_new__Vec();
	test_from__Vec();
	test_get__Vec();
	test_pop__Vec();
	test_push__Vec();
	test_remove__Vec();
	test_reverse__Vec();

	test_new__String();
	test_from__String();
	test_push__String();

	puts("\x1b[32mAll tests pass\x1b[0m");
}
