#pragma once
#pragma once
#include "MapState.h"

class Program;
class CursorMapState : public MapState
{
public:
	CursorMapState(::Program& program);
	virtual ~CursorMapState();

protected:
	signed char CursorX, CursorY;

	virtual void Input(char key, bool isCommand) override;
	virtual void OnDraw() override;

};
