#pragma once
#include "ProgramState.h"
#include "MenuItem.h"

class Menu;
class LoadGameState
	: public ProgramState
{
public:
	LoadGameState(::Program& program, string directory);
	virtual ~LoadGameState();

protected:
	virtual void OnInitialize() override;
	virtual void OnUpdate(char key, bool isCommand) override;
	virtual void OnDraw() override;

private:
	Menu* Files;
	string Directory;

};

class LoadFileMenuItem
	: public MenuItem
{
public:

	LoadFileMenuItem(string label, ::Program& program, string fullDirectory);
	virtual ~LoadFileMenuItem();

	virtual void Select() override;

private:
	::Program& Program;
	string FullDirectory;
};

class LoadDirMenuItem
	: public MenuItem
{
public:

	LoadDirMenuItem(string label, ::Program& program, string fullDirectory);
	virtual ~LoadDirMenuItem();

	virtual void Select() override;

private:
	::Program& Program;
	string FullDirectory;
};