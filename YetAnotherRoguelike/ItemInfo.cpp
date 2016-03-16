#include "ItemInfo.h"

ItemInfo::ItemInfo()
{
}

ItemInfo::~ItemInfo()
{
}

string ItemInfo::GetArticleAndName()
{
	return Article + Name;
}

string ItemInfo::GetArticleAndName(bool isCapitalized)
{
	if (isCapitalized)
	{
		string s = GetArticleAndName();
		s[0] = toupper(s[0]);
		return s;
	}
	return GetArticleAndName();
}

ConsumableInfo::ConsumableInfo()
{
}

ConsumableInfo::~ConsumableInfo()
{
}

ItemType ConsumableInfo::GetType()
{
	return Consumable;
}
