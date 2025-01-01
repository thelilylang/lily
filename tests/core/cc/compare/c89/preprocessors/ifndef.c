int main() {
#ifndef X
	int y = 100;
#else
#error "Expected to enter in the if branch"
	int y = 1000;
#endif
}
