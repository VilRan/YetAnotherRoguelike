#pragma once
#include "ProgramState.h"
#include <vector>
#include "MenuItem.h"

using namespace std;

class Program;
class Menu;
class MainMenuState: public ProgramState
{
public:
	//vector<MenuItem*> Items;
	Menu* MainMenu;
	//int Selection;

	MainMenuState(::Program& program);
	virtual ~MainMenuState();

protected:
	virtual void OnInitialize() override;
	virtual void OnUpdate(char key, bool isCommand) override;
	virtual void OnDraw() override;

private:
	void Input(char key, bool isCommand);
};

class NewGameMenuItem : public MenuItem
{
public:
	NewGameMenuItem(string label, ::Program& program);
	virtual ~NewGameMenuItem();

	virtual void Select() override;

private:
	Program& Program;
};

class ExitGameMenuItem : public MenuItem
{
public:
	ExitGameMenuItem(string label, Program& program);
	virtual ~ExitGameMenuItem();

	virtual void Select() override;

private:
	Program& Program;
};

class ResumeGameMenuItem : public MenuItem
{
public:
	ResumeGameMenuItem(string label, Program& program);
	virtual ~ResumeGameMenuItem();

	virtual void Select() override;

private:
	Program& Program;
};

class SaveGameMenuItem : public MenuItem
{
public:
	SaveGameMenuItem(string label, Program& program);
	virtual ~SaveGameMenuItem();

	virtual void Select() override;

private:
	Program& Program;
};

class LoadGameMenuItem : public MenuItem
{
public:
	LoadGameMenuItem(string label, Program& program);
	virtual ~LoadGameMenuItem();

	virtual void Select() override;

private:
	Program& Program;
};