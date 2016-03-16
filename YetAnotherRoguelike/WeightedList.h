#pragma once
//#include <vector>
#include <list>

using namespace std;

template <typename T>
class WeightedList
{
public:
	WeightedList();
	~WeightedList();

	T GetRandom();
	void Add(T item, double weight);
	void Remove(T item);
	void ChangeWeight(T item, double newWeight);
	// Removes all items from the list without deleting them.
	void Clear();
	// Deletes all items on the list.
	void DeleteAll();
	int GetCount();

private:
	list<pair<double, T>> Items;
	double TotalWeight;
};

template<typename T>
WeightedList<T>::WeightedList()
{
}

template<typename T>
WeightedList<T>::~WeightedList()
{
}

template<typename T>
T WeightedList<T>::GetRandom()
{
	if (TotalWeight <= 0)
		throw exception("Total weight of weighted list cannot be zero or negative.");

	double rn = (double)rand() / RAND_MAX * TotalWeight;
	for (pair<double, T> item : Items)
	{
		if (rn <= item.first)
			return item.second;
		rn -= item.first;
	}

	throw exception("Could not get an item. The list is probably empty.");
}

template<typename T>
void WeightedList<T>::Add(T item, double weight)
{
	if (weight <= 0)
		throw exception("Weight must be greater than zero.");

	Items.push_back(pair<double, T>(weight, item));
	TotalWeight += weight;
}

template<typename T>
void WeightedList<T>::Remove(T item)
{
	for (pair<double, T> _item : Items)
	{
		if (item == _item.second)
		{
			TotalWeight -= _item.first;
			Items.remove(_item);
			return;
		}
	}
}

template<typename T>
void WeightedList<T>::ChangeWeight(T item, double newWeight)
{
	for (pair<double, T> _item : Items)
	{
		if (item = _item.second)
		{
			TotalWeight -= _item.first;
			_item.first = newWeight;
			TotalWeight += newWeight;
			return;
		}
	}
}

template<typename T>
inline void WeightedList<T>::Clear()
{
	Items.clear();
	TotalWeight = 0;
}

template<typename T>
inline void WeightedList<T>::DeleteAll()
{
	for (pair<double, T> item : Items)
	{
		delete item.second;
	}
}

template<typename T>
inline int WeightedList<T>::GetCount()
{
	return Items.size();
}
