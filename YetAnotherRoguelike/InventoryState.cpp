#include "InventoryState.h"
#include "curses.h"
#include "Creature.h"
#include "Program.h"
#include "Session.h"
#include "Item.h"
#include "MapState.h"
#include "Map.h"
#include "Menu.h"

InventoryState::InventoryState(::Program& program)
	: ProgramState::ProgramState(program)
{
	InventoryMenu = new Menu();

	Session& session = *program.GetSession();
	Creature* player = session.Player;
	if (player != nullptr)
	{
		for (Item* item : player->Inventory)
		{
			InventoryMenu->AddItem(new InventoryMenuItem(item, *this));
		}
	}
}

InventoryState::~InventoryState()
{
	delete InventoryMenu;
}

void InventoryState::OnInitialize()
{
	//Draw();
}

void InventoryState::OnUpdate(char key, bool isCommand)
{
	Input(key, isCommand);
	//Draw();
}

void InventoryState::Input(char key, bool isCommand)
{
	Creature& player = *Program.GetSession()->Player;
	InfoManager& info = Program.GetInfo();

	switch (key)
	{
	case UpArrow:
		InventoryMenu->DecrementSelection();
		break;
	case DownArrow:
		InventoryMenu->IncrementSelection();
		break;
	case 'u':
		Mode = Use;
		InventoryMenu ->TriggerSelected();
		break;
	case 'd':
		Mode = Drop;
		InventoryMenu->TriggerSelected();
		break;
	case 'e':
		Mode = Equip;
		InventoryMenu->TriggerSelected();
		break;
	case 'w':
		Mode = Wear;
		InventoryMenu->TriggerSelected();
		break;
	case EscapeKey:
		Program.ChangeState(new MapState(Program));
		break;
	default:
		break;
	}
}

void InventoryState::OnDraw()
{
	Creature* player = Program.GetSession()->Player;
	if (player != nullptr)
	{
		printw("W: ");
		int i = 0;
		for (Item* weapon : player->Hands)
		{
			if (i > 0)
				printw(" & ");
			printw("%s", ((WeaponInfo&)weapon->Info).Name.data());
			i++;
		}
		printw("\n");

		printw("A: ");
		if (player->Armor != nullptr)
		{
			printw("A: %s\n", player->Armor->Info.Name.data());
		}
		printw("\n");

		InventoryMenu->Draw(4, 4, 12, 20);
		/*
		for (Item* item : player->Inventory)
		{
			printw("	%s\n", item->Info.Name.data());
		}
		*/
	}
	move(24, 0);
}

InventoryMenuItem::InventoryMenuItem(::Item* item, InventoryState & state)
	: MenuItem(item->Info.Name), State(state), Item(item)
{
}

InventoryMenuItem::~InventoryMenuItem()
{
}

void InventoryMenuItem::Select()
{
	Program& program = State.Program;
	Session& session = *program.GetSession();
	Creature& player = *session.Player;
	bool success = false;
	switch (State.Mode)
	{
	case Use:
		success = player.UseItem(Item);
		break;
	case Drop:
		success = player.DropItem(Item);
		break;
	case Equip:
		success = player.Equip(Item);
		break;
	case Wear:
		success = player.Wear(Item);
		break;
	default:
		break;
	}

	if (success)
	{
		MapState* newState = new MapState(program);
		newState->IsLocked = true;
		program.ChangeState(newState);
		player.Initiative += player.GetSlowness();
	}
}
