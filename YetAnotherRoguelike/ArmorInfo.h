#pragma once
#include "ItemInfo.h"

using namespace std;

class ArmorInfo : public ItemInfo
{
public:
	short DefenseMin, DefenseMax;

	ArmorInfo();
	virtual ~ArmorInfo();

	virtual ItemType GetType() override;
};

