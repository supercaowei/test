#ifndef TTTTTTTT
#define TTTTTTTT

#include <memory>
#include <string>
#include <list>

class CC
{
public:
	CC();
	~CC();
	int i;
};

class Base
{
public:
	Base(CC& ii, int jj) 
	{
		i = (int*)malloc(sizeof(int) * 100);
		*i = jj;	
	}
	virtual ~Base() 
	{
		if(i)
		{
			free(i);
		}
	}

protected:
	int *i = NULL;
};

class Derived : public Base
{
public:
	Derived(CC &ii, int jj) : Base(ii, jj) {}
	~Derived() {}
};

#endif
