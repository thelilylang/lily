#include <stdnoreturn.h>
#include <stdlib.h>

noreturn void bye() {
	exit(0);
}

int main() {
	bye();
}
