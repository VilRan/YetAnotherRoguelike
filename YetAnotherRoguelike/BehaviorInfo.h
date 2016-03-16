#pragma once
#include <string>

using namespace std;

class Creature;
class BehaviorInfo
{
public:
	string ID;

	BehaviorInfo();
	virtual ~BehaviorInfo();

	virtual void Run(Creature& creature) = 0;
};

class MonsterBehavior : public BehaviorInfo
{
public:
	int EnemySearchRange;

	MonsterBehavior();
	virtual ~MonsterBehavior();

	virtual void Run(Creature& creature) override;
};

class CivilizedBehavior : public BehaviorInfo
{
public:
	int EnemySearchRange;
	int AllySearchRange;

	CivilizedBehavior();
	virtual ~CivilizedBehavior();

	virtual void Run(Creature& creature) override;
};