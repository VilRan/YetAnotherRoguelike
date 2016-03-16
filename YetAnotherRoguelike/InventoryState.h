#pragma once
#include "ProgramState.h"
#include "MenuItem.h"

using namespace std;

enum InventoryMode
{
	Use, Drop, Equip, Wear
};

class Program;
class Menu;
class Item;

class InventoryState 
	: public ProgramState
{
public:
	InventoryMode Mode = Use;

	InventoryState(::Program& program);
	virtual ~InventoryState();

protected:
	virtual void OnInitialize() override;
	virtual void OnUpdate(char key, bool isCommand) override;
	virtual void OnDraw() override;

private:
	Menu* InventoryMenu;

	void Input(char key, bool isCommand);
};

class InventoryMenuItem
	: public MenuItem
{
public:

	InventoryMenuItem(::Item* item, InventoryState& state);
	virtual ~InventoryMenuItem();

	virtual void Select() override;

private:
	InventoryState& State;
	::Item* Item;
};
