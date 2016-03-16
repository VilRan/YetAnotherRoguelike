#pragma once
#include "ProgramState.h"
#include <string>

class LogState : public ProgramState
{
public:
	LogState(::Program& program);
	virtual ~LogState();


protected:
	virtual void OnInitialize() override;
	virtual void OnUpdate(char key, bool isCommand) override;
	virtual void OnDraw() override;

private:
	string Filter;
	int Offset = 0;

	void Input(char key, bool isCommand);
};

