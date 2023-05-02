#include <stdio.h>

// #derive: Eq
typedef struct Person {
	const char *name;
} Person;

int main() {
	Person p = {
		.name = "S"
	};

	Person p2 = {
		.name = "S"
	};

	printf("%d", EQ(Person, p, p2));
}
