#pragma once
#include <vector>
#include "WeightedList.h"

using namespace std;

class ItemSetInfo;
class InfoManager;
class ItemInfo;


class ItemSetBase
{
public:
	double Chance = 1.0;

	ItemSetBase();
	virtual ~ItemSetBase();

	virtual void GetItems(vector<ItemInfo*>* out) = 0;
};

class ItemSetItem
	: public ItemSetBase
{
public:
	ItemInfo* Item;

	ItemSetItem();
	virtual ~ItemSetItem();

	virtual void GetItems(vector<ItemInfo*>* out) override;
};

class ItemSetSubset
	: public ItemSetBase
{
public:
	ItemSetInfo* Subset;

	ItemSetSubset();
	virtual ~ItemSetSubset();

	virtual void GetItems(vector<ItemInfo*>* out) override;
};

class ItemSetExclusive
	: public ItemSetBase
{
public:
	WeightedList<ItemSetBase*> Subsets;

	ItemSetExclusive();
	virtual ~ItemSetExclusive();

	virtual void GetItems(vector<ItemInfo*>* out) override;
};

class ItemSetInfo
{
public:
	string ID;
	vector<ItemSetBase*> Subsets;

	ItemSetInfo();
	virtual ~ItemSetInfo();

	void GetItems(vector<ItemInfo*>* out);
};

