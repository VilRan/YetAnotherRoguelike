#include "BinaryHeap.h"
#include "IComparable.h"
#include <algorithm>

BinaryHeap::BinaryHeap()
{
	_Data = new IComparable*[DEFAULT_SIZE];
}

BinaryHeap::~BinaryHeap()
{
	delete _Data;
}

IComparable& BinaryHeap::Peek()
{
	return *_Data[0];
}

IComparable& BinaryHeap::Remove()
{
	if (_Count == 0)
		throw exception("Cannot remove item, heap is empty.");
	IComparable* v = _Data[0];
	_Count--;
	_Data[0] = _Data[_Count];
	_Data[_Count] = nullptr;
	DownHeap();
	return *v;
}

void BinaryHeap::Add(IComparable& item)
{
	if (_Count == _Capacity)
		SetCapacity(_Capacity * 2);
	_Data[_Count] = &item;
	UpHeap();
	_Count++;
}

void BinaryHeap::Clear()
{
	_Count = 0;
	delete _Data;
	_Data = new IComparable*[_Capacity];
}

BinaryHeap* BinaryHeap::Copy()
{
	return new BinaryHeap(_Data, _Count);
}

int BinaryHeap::GetCount()
{
	return _Count;
}

int BinaryHeap::GetCapacity()
{
	return _Capacity;
}

void BinaryHeap::SetCapacity(int value)
{
	int previous = _Capacity;
	_Capacity = Math::Max(value, _Count);
	if (_Capacity != previous)
	{
		IComparable** temp = new IComparable*[_Capacity];
		for (int i = 0; i < _Count; i++)
			temp[i] = _Data[i];
		delete _Data;
		_Data = temp;
	}
}

BinaryHeap::BinaryHeap(IComparable** data, int count)
{
	SetCapacity(count);
	_Count = count;
	for (int i = 0; i < count; i++)
		_Data[i] = data[i];
}

void BinaryHeap::UpHeap()
{
	_Sorted = false;
	int p = _Count;
	IComparable& item = *_Data[p];
	int par = Parent(p);
	while (par > -1 && item.CompareTo(*_Data[par]) < 0)
	{
		_Data[p] = _Data[par];
		p = par;
		par = Parent(p);
	}
	_Data[p] = &item;
}

void BinaryHeap::DownHeap()
{
	_Sorted = false;
	int n;
	int p = 0;
	IComparable& item = *_Data[p];
	while (true)
	{
		int ch1 = Child1(p);
		if (ch1 >= _Count) break;
		int ch2 = Child2(p);

		if (ch2 >= _Count)
			n = ch1;
		else
			n = _Data[ch1]->CompareTo(*_Data[ch2]) < 0 ? ch1 : ch2;

		if (item.CompareTo(*_Data[n]) > 0)
		{
			_Data[p] = _Data[n];
			p = n;
		}
		else break;
	}
	_Data[p] = &item;
}

void BinaryHeap::EnsureSort()
{
	if (_Sorted) return;
	sort(_Data, _Data + _Count);
	_Sorted = true;
}

int BinaryHeap::Parent(int index)
{
	return (index - 1) >> 1;
}

int BinaryHeap::Child1(int index)
{
	return (index << 1) + 1;
}

int BinaryHeap::Child2(int index)
{
	return (index << 1) + 2;
}
