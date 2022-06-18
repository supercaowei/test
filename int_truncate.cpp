#include <stdio.h>

int main()
{
	long long ll;
	scanf("%lld", &ll);
	int truncated = (int)ll;
	printf("%d\n", truncated);
	return 0;
}
