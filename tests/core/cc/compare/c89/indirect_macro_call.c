#define USE_A 1
#define DEF(x) USE_##x

#if DEF(A) == 1
int add(int x, int y) {
	return x + y;
}
#endif

int main() {
	int x = DEF(A);
}
