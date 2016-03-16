/*
#include "WeightedList.h"

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

	double rn = (double)rand() / TotalWeight;

	for each (pair<double, T> item in Items)
	{
		if (item.first <= rn)
			return item.second;
		rn -= item.first;
	}
}
template<typename T>
void WeightedList<T>::Add(T item, double weight)
{
	if (weight <= 0)
		throw exception("Weight must be greater than zero.")

	Items.push_back(pair<double, T>(weight, item));
	TotalWeight += weight;
}

template<typename T>
void WeightedList<T>::Remove(T item)
{
	int i = 0;
	for each (pair<double, T> _item in Items)
	{
		if (item = _item.second)
		{
			TotalWeight -= _item.first;
			break;
		}
		i++;
	}
	Items.erase(i);
}

template<typename T>
void WeightedList<T>::ChangeWeight(T item, double newWeight)
{
	for each (pair<double, T> _item in Items)
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
*/
