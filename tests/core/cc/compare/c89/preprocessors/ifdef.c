int main() {
#ifdef X
#error "Expected to enter in the else branch"
	int x = 10;
#else
	int y = 400;
#endif
}
