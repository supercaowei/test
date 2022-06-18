
#include "test_make_shared.h"

int main()
{
	int i = 0;
	int j = 0;
	CC c;	
	auto d = std::make_shared<Derived>(c, j);
	return 0;
}

