#include <stdio.h>

int main()
{
	double f = 0.00000000000000000000001;
	printf("%s\n", f == 0.f ? "true" : "false");
}


