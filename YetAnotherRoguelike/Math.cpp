#include "Math.h"

Math::Math()
{
}


Math::~Math()
{
}

int Math::Max(int a, int b)
{
	if (a > b)
		return a;
	return b;
}

int Math::Min(int a, int b)
{
	if (a < b)
		return a;
	return b;
}

int Math::Signum(int v)
{
	if (v > 0)
		return 1;
	if (v < 0)
		return -1;
	return 0;
}
