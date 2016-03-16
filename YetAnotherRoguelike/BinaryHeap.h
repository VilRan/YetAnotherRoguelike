#pragma once
#include "Math.h"

using namespace std;

class IComparable;
class BinaryHeap
{
public:
	BinaryHeap();
	~BinaryHeap();

	IComparable& Peek();
	IComparable& Remove();
	void Add(IComparable& item);
	void Clear();
	BinaryHeap* Copy();
	int GetCount();
	int GetCapacity();
	void SetCapacity(int value);

private:
	const int DEFAULT_SIZE = 1024;

	IComparable** _Data;
	int _Count = 0;
	int _Capacity = DEFAULT_SIZE;
	bool _Sorted;

	BinaryHeap(IComparable** data, int count);

	void UpHeap();
	void DownHeap();
	void EnsureSort();
	static int Parent(int index);
	static int Child1(int index);
	static int Child2(int index);
};