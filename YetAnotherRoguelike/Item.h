#pragma once

class Creature;
class ItemInfo;
class Item
{
public:
	ItemInfo& Info;

	Item(ItemInfo& info);
	~Item();

	// Returns false if the user could not use the item.
	bool Use(Creature& user);
	bool IsDroppable();
};

