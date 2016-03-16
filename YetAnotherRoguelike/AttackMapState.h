#pragma once
#include "LookMapState.h"
class AttackMapState :
	public LookMapState
{
public:
	AttackMapState(::Program& program);
	virtual ~AttackMapState();

protected:
	virtual void Input(char key, bool isCommand) override;
};

