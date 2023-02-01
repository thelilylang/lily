#include <time.h>
#include <stdio.h>

#include <base/vec.h>
#include <base/new.h>
#include <base/types.h>

int main() {
	printf("Bench: push 1'000'000'000 items\n");

	{
		clock_t start = clock();
		Vec *v = NEW(Vec);

		for (Usize i = 0; i < 1'000'000'000; i++) {
			push__Vec(v, (Usize*)i);
		}

		FREE(Vec, v);

		printf("Time: %.2fs\n", (double)(clock() - start) / CLOCKS_PER_SEC);
	}

	printf("Bench: push 1'000'000'000 items and remove 1'000'000'000\n");

	{
		clock_t start = clock();
		Vec *v = NEW(Vec);

		for (Usize i = 0; i < 1'000'000'000; i++) {
			push__Vec(v, (Usize*)i);
		}

		for (Usize i = 0; i < 1'000'000'000; i++) {
			pop__Vec(v);
		}

		FREE(Vec, v);

		printf("Time: %.2fs\n", (double)(clock() - start) / CLOCKS_PER_SEC);
	}
}
