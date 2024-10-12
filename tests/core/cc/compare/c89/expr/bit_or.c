#include <stdbool.h>

int main() {
	(void)(true | false);
	(void)(true | true);
	(void)(1 | 0);
	(void)(1LL | 1UL);
}
