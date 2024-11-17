#define MERGE(a, b) a##b
#define J a
#define MERGE_WITH_J(a) a##J

int main() {
	int xy = 10;
	int a = MERGE(x, y);
	int b = a + MERGE(1, 0);
	int MERGE_WITH_J(x) = 30;
	int *xJp = &xJ;

	return 0;
}
