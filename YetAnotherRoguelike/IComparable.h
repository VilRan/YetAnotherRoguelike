#pragma once
class IComparable
{
public:
	virtual ~IComparable() {};
	virtual int GetValue();
	int CompareTo(IComparable& other);
};

