#define MERGE(a, b) a##b

int main() {
	int xy = 10;
	int a = MERGE(x, y);
	int b = a + MERGE(1, 0);

	return 0;
}
