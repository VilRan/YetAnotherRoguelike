#include "Menu.h"
#include "MenuItem.h"
#include "curses.h"
#include "ProgramState.h"

Menu::Menu()
{
}

Menu::~Menu()
{
	for (MenuItem* item : Items)
	{
		delete item;
	}
}

void Menu::AddItem(MenuItem * item)
{
	Items.push_back(item);
}

MenuItem * Menu::GetItem(int index)
{
	return Items[index];
}

void Menu::TriggerSelected()
{
	if (Items.size() > 0)
		Items[Selection]->Select();
}

void Menu::IncrementSelection()
{
	if (Items.size() == 0)
		return;

	Selection++;

	int max = Items.size() - 1;
	if (Selection > max)
		Selection = 0;
}

void Menu::DecrementSelection()
{
	if (Items.size() == 0)
		return;

	Selection--;

	int max = Items.size() - 1;
	if (Selection < 0)
		Selection = max;
}

int Menu::GetSelection()
{
	return Selection;
}

int Menu::GetCount()
{
	return Items.size();
}

void Menu::Input(char key, bool isCommand)
{
	switch (key)
	{
	case ProgramState::UpArrow:
		DecrementSelection();
		break;
	case ProgramState::DownArrow:
		IncrementSelection();
		break;
	case ProgramState::EnterKey:
		TriggerSelected();
		break;
	default:
		break;
	}
}

void Menu::Draw(int x, int y, int width, int height)
{
	int row = y;
	for (MenuItem* item : Items)
	{
		mvprintw(row++, x, "%s", item->Label.data());
		if (row > y + height - 1)
			break;
	}
	mvprintw(y + Selection, x - 2, "->");
}
