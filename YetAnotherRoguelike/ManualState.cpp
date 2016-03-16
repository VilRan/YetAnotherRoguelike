#include "ManualState.h"
#include "Program.h"
#include "curses.h"

ManualState::ManualState(::Program& program)
	: ProgramState(program)
{
}


ManualState::~ManualState()
{
}

void ManualState::OnInitialize()
{
}

void ManualState::OnUpdate(char key, bool isCommand)
{
	switch (key)
	{
	case EscapeKey:
		Program.PopState();
		break;
	default:
		break;
	}
}

void ManualState::OnDraw()
{
	int row = 1, col = 1;

	mvprintw(row++, col, "Menus");
	row++; col += 2;
	mvprintw(row++, col, "Use the up and down arrow keys or keypad to select an item.");
	mvprintw(row++, col, "Enter key usually triggers the selected item.");

	row += 2; col -= 2;
	mvprintw(row++, col, "Map Mode");
	row++; col += 2;
	mvprintw(row++, col, "Move around using the numpad.");
	row++;

	mvprintw(row++, col, "a = ranged attack");
	mvprintw(row++, col, "i = inventory");
	mvprintw(row++, col, "k = look");
	mvprintw(row++, col, "l = log");

	row += 2;  col -= 2;
	mvprintw(row++, col, "Inventory Mode");

	row++; col += 2;
	mvprintw(row++, col, "u = use");
	mvprintw(row++, col, "d = drop");
	mvprintw(row++, col, "e = equip");
	mvprintw(row++, col, "w = wear");

	move(24, 0);
}
