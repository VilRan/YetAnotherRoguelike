#include "Item.h"
#include "ItemInfo.h"
#include "ItemUseEffect.h"

Item::Item(ItemInfo &info) : Info(info)
{
}


Item::~Item()
{
}

bool Item::Use(Creature& user)
{
	bool success = false;
	for (ItemUseEffect* effect : Info.Effects)
	{
		if (effect->Apply(user))
		{
			success = true;
		}
	}

	return success;
}

bool Item::IsDroppable()
{
	return Info.IsDroppable;
}