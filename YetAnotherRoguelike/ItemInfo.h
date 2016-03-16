#pragma once
#include <vector>
#include <string>

using namespace std;

enum ItemType
{
	Weapon,
	Armor,
	Consumable
};

class ItemUseEffect;

class ItemInfo
{
public:
	vector<ItemUseEffect*> Effects;
	string ID;
	string Name;
	string Article;
	char Symbol;
	unsigned char Color;
	bool IsDroppable = true;

	ItemInfo();
	virtual ~ItemInfo();

	string GetArticleAndName();
	string GetArticleAndName(bool isCapitalized);
	virtual ItemType GetType() = 0;
};

class ConsumableInfo : public ItemInfo
{
public:
	ConsumableInfo();
	virtual ~ConsumableInfo();

	virtual ItemType GetType() override;
};



