#include "IComparable.h"

int IComparable::CompareTo(IComparable& other)
{
	int valueSelf = GetValue();
	int valueOther = other.GetValue();

	if (valueSelf > valueOther)
		return 1;
	if (valueSelf < valueOther)
		return -1;

	return 0;
}

int IComparable::GetValue()
{
	return 0;
}
