#include "CreatureInfo.h"



CreatureInfo::CreatureInfo(FactionInfo& defaultFaction, BehaviorInfo& defaultBehavior, ItemSetInfo& itemset)
	: DefaultFaction(defaultFaction), DefaultBehavior(defaultBehavior), ItemSet(itemset)
{
}


CreatureInfo::~CreatureInfo()
{
}

string CreatureInfo::GetArticleAndName()
{
	return Article + Name;
}

string CreatureInfo::GetArticleAndName(bool isCapitalized)
{
	if (isCapitalized)
	{
		string s = GetArticleAndName();
		s[0] = toupper(s[0]);
		return s;
	}
	return GetArticleAndName();
}
