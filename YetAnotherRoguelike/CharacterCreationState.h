#pragma once
#include "ProgramState.h"
#include <string>

using namespace std;

class CharacterCreationState : public ProgramState
{
public:
	CharacterCreationState(::Program& program);
	virtual ~CharacterCreationState();

protected:
	virtual void OnInitialize() override;
	virtual void OnUpdate(char key, bool isCommand) override;
	virtual void OnDraw() override;

private:
	std::string Name;

	void Input(char key, bool isCommand);
};

