#pragma once
#include <string>

using namespace std;

class Creature;

class ItemUseEffect
{
public:
	string ID;

	ItemUseEffect();
	virtual ~ItemUseEffect();

	virtual bool Apply(Creature& user) = 0;
};

class HealingItemUseEffect : public ItemUseEffect
{
public:
	short Amount;

	HealingItemUseEffect();
	virtual ~HealingItemUseEffect();

	virtual bool Apply(Creature& user) override;
};