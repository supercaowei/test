#include <stdio.h>

typedef struct {
    unsigned char c;
} B;

int main()
{
    B b;
	printf("%lu\n", sizeof(b));
	return 0;
}
