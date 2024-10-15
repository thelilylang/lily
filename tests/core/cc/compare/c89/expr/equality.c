#define bool int

int main() {
	bool a = 20 == 10;
	bool b = 56 == 50;
	bool c = 3.3 == 5.5;
	bool d = 2 != 40;
	bool e = d != c;
	bool f = a != b;

	(void)e;
	(void)f;
}
