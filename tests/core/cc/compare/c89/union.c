union A {
	int x;
};

typedef union B {
	int x;
} B;

int main() {
	union A a = { .x = 10 };
	union A a2 = { 10 };
	union A a3 = (union A){ 10 };
	B b = { .x = 20 };
	B b2 = { 20 };
	B b3 = (B){ 30 };
}
