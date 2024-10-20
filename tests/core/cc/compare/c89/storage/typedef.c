typedef int X;

typedef struct {
	int x;
} S;

typedef int (*add_t2)(int lhs, int rhs);
typedef int add_t(int lhs, int rhs);
typedef int array_integer_t[10];

int main() {
	X a = 30;
	S s = {2};

	add_t f;
	add_t2 f2;
	array_integer_t arr = {1,2,3,4,5,6,7,8,9,10};
}
