#define X 10

int main() {
	int x = X;

#undef X

#if X
#error "Expected to enter in the else branch"
#else
	int y = 120;
#endif
}
