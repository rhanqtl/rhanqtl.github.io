#define OUTER(x) CONCAT(INNER_, x)
#define INNER_123 456
#define xyz 123
#define CONCAT(x, y) CONCAT_INTERNAL(x, y)
#define CONCAT_INTERNAL(x, y) x##y

int main() { }
