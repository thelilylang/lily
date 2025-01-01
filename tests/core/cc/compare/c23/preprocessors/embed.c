#ifdef __CI__
const char readme_content[] = {
#embed "README.md"
, '\0'
};
#else
#warning "GCC and Clang do not yet support the embed preprocessor."
#endif

int main() {
}
