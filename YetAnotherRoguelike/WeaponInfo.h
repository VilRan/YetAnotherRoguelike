#pragma once
#include "ItemInfo.h"

using namespace std;

class WeaponInfo : public ItemInfo
{
public:
	short DamageMin, DamageMax;
	unsigned char Accuracy;
	signed char Parry;

	WeaponInfo();
	virtual ~WeaponInfo();

	virtual ItemType GetType() override;
};

