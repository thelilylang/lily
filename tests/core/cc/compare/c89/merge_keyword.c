#define MERGE(a, b) a##b
#define MERGE2(a, b, c) a##b##c

int main() {
	int MERGE(if, 0) = 20;
	MERGE2(l, o, ng) xx = 40;
}
