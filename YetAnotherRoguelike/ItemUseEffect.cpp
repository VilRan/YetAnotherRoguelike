#include "ItemUseEffect.h"
#include "Creature.h"

ItemUseEffect::ItemUseEffect()
{
}
ItemUseEffect::~ItemUseEffect()
{
}

HealingItemUseEffect::HealingItemUseEffect()
{
}
HealingItemUseEffect::~HealingItemUseEffect()
{
}

bool HealingItemUseEffect::Apply(Creature & user)
{
	if (user.GetHP() == user.GetMaxHP())
		return false;

	user.SetHP(user.GetHP() + Amount);
	return true;
}
