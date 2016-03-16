#pragma once
#include "CursorMapState.h"

class Program;
class LookMapState : public CursorMapState
{
public:
	LookMapState(::Program& program);
	virtual ~LookMapState();

protected:
	virtual void Input(char key, bool isCommand) override;

private:
	void UpdateLog();
};

