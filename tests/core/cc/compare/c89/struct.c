struct Q {
	int x;
	int y;
};

typedef struct R {
	int x;
	int y;
} R;

int main() {
	struct Q q = { .x = 20, .y = 10 };
	struct Q q2 = {20, 10};

	R r = {10, 10};
	R r2 = { .x = 10, .y = 10 };
}
