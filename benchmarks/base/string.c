#include <time.h>
#include <stdio.h>

#include <base/string.h>
#include <base/new.h>
#include <base/types.h>

int main() {
	printf("Bench: push 1'000'000'000 characters\n");

	{
		clock_t start = clock();
		String *s = NEW(String);

		for (Usize i = 0; i < 200000000; i++) {
			push__String(s, 'a');
			push__String(s, 'b');
			push__String(s, 'c');
			push__String(s, 'd');
			push__String(s, 'e');
		}

		FREE(String, s);

		printf("Time: %.2fs\n", (double)(clock() - start) / CLOCKS_PER_SEC);
	}
}
