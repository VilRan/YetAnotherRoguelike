#pragma once
#include <string>
#include <vector>

using namespace std;

class WeaponInfo;
class ArmorInfo;
class FactionInfo;
class BehaviorInfo;
class ItemSetInfo;

class CreatureInfo
{
public:
	string ID;
	string Name;
	string Article;
	vector<WeaponInfo*> NaturalWeapons;
	vector<WeaponInfo*> DefaultWeapons;
	ArmorInfo* DefaultArmor;
	FactionInfo& DefaultFaction;
	BehaviorInfo& DefaultBehavior;
	ItemSetInfo& ItemSet;
	short MaxHP, SpeedMin, SpeedMax;
	signed char DodgeMin, DodgeMax;
	unsigned char Color;
	char Symbol;

	CreatureInfo(FactionInfo& defaultFaction, BehaviorInfo& defaultBehavior, ItemSetInfo& itemset);
	~CreatureInfo();

	string GetArticleAndName();
	string GetArticleAndName(bool isCapitalized);
};

