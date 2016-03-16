#pragma once
#include <vector>
#include <algorithm>

using namespace std;

class VectorUtility
{
public:
	VectorUtility();
	~VectorUtility();

	// Attempts to remove the item from the vector.
	template<class T> inline static void Remove(vector<T*>* vector, T* item);
	// Attempts to remove the item from the vector.
	template<class T> inline static void Remove(vector<T*>& vector, T* item);
	// Returns true if the item exists in the vector.
	template<class T> inline static bool Contains(vector<T*>* vector, T* item);
	// Returns true if the item exists in the vector.
	template<class T> inline static bool Contains(vector<T*>& vector, T* item);

};

// Attempts to remove the item from the vector.
template<class T>
void VectorUtility::Remove(vector<T*>* vector, T* item)
{
	vector->erase(remove(vector->begin(), vector->end(), item), vector->end());
}

// Attempts to remove the item from the vector.
template<class T>
void VectorUtility::Remove(vector<T*>& vector, T* item)
{
	vector.erase(remove(vector.begin(), vector.end(), item), vector.end());
}

// Returns true if the item exists in the vector.
template<class T>
bool VectorUtility::Contains(vector<T*>* vector, T* item)
{
	return find(vector->begin(), vector->end(), item) != vector->end();
}

// Returns true if the item exists in the vector.
template<class T>
bool VectorUtility::Contains(vector<T*>& vector, T* item)
{
	return find(vector.begin(), vector.end(), item) != vector.end();
}
