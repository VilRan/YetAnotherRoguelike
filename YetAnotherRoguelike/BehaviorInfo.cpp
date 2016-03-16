#include "BehaviorInfo.h"
#include "Creature.h"

BehaviorInfo::BehaviorInfo()
{
}
BehaviorInfo::~BehaviorInfo()
{
}


MonsterBehavior::MonsterBehavior()
{
}
MonsterBehavior::~MonsterBehavior()
{
}
void MonsterBehavior::Run(Creature & creature)
{
	Creature* enemy = creature.FindNearestCreature(EnemySearchRange, false);
	if (enemy != nullptr)
	{
		creature.MoveTowards(enemy->GetTile());
	}
	else
	{
		creature.MoveRandomly();
	}
}


CivilizedBehavior::CivilizedBehavior()
{
}
CivilizedBehavior::~CivilizedBehavior()
{
}
void CivilizedBehavior::Run(Creature & creature)
{
	Creature* enemy = creature.FindNearestCreature(EnemySearchRange, false);
	if (enemy != nullptr)
	{
		creature.MoveTowards(*enemy);
	}
	else
	{
		Creature* friendly = creature.FindNearestCreature(AllySearchRange, true);
		if (friendly != nullptr)
		{
			creature.MoveTowards(*friendly);
		}
		else
		{
			creature.MoveRandomly();
		}
	}
}
