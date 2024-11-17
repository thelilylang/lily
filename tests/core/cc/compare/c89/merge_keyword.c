#define MERGE(a, b) a##b
#define MERGE2(a, b, c) a##b##c
#define MERGE3(a, b, c, d) a##b##c##d

int main() {
	int MERGE(if, 0) = 20;
	MERGE2(l, o, ng) xx = 40;
	MERGE3(l, o, n, g) abc = 100;
}
