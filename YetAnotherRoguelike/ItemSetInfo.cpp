#include "ItemSetInfo.h"


ItemSetBase::ItemSetBase()
{
}

ItemSetBase::~ItemSetBase()
{
}

ItemSetItem::ItemSetItem()
{
}

ItemSetItem::~ItemSetItem()
{
}

void ItemSetItem::GetItems(vector<ItemInfo*>* out)
{
	if ((double)rand() / RAND_MAX < Chance)
		out->push_back(Item);
}


ItemSetSubset::ItemSetSubset()
{
}

ItemSetSubset::~ItemSetSubset()
{
}

void ItemSetSubset::GetItems(vector<ItemInfo*>* out)
{
	if ((double)rand() / RAND_MAX < Chance)
		Subset->GetItems(out);
}


ItemSetExclusive::ItemSetExclusive()
{
}

ItemSetExclusive::~ItemSetExclusive()
{
	Subsets.DeleteAll();
}

void ItemSetExclusive::GetItems(vector<ItemInfo*>* out)
{
	if ((double)rand() / RAND_MAX < Chance)
		Subsets.GetRandom()->GetItems(out);
}


ItemSetInfo::ItemSetInfo()
{
}

ItemSetInfo::~ItemSetInfo()
{
	for (ItemSetBase* subset : Subsets)
		delete subset;
}

void ItemSetInfo::GetItems(vector<ItemInfo*>* out)
{
	for (ItemSetBase* subset : Subsets)
	{
		subset->GetItems(out);
	}
}


