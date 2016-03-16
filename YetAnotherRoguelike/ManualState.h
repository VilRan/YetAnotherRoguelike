#pragma once
#include "ProgramState.h"

class ManualState :
	public ProgramState
{
public:
	ManualState(::Program& program);
	virtual ~ManualState();


protected:
	virtual void OnInitialize() override;
	virtual void OnUpdate(char key, bool isCommand) override;
	virtual void OnDraw() override;
};

