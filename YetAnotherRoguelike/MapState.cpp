#include "MapState.h"
#include "curses.h"
#include <stdlib.h>
#include "MainMenuState.h"
#include "LogState.h"
#include "WeaponInfo.h"
#include "ArmorInfo.h"
#include "Map.h"
#include "Session.h"
#include "Item.h"
#include "InventoryState.h"
#include "LookMapState.h"
#include "Creature.h"
#include "Tile.h"
#include "Program.h"
#include "Pathfinder.h"
#include "AttackMapState.h"
#include "Math.h"
#include "Point.h"
#include "Graphics.h"

MapState::MapState(::Program& program)
	: ProgramState::ProgramState(program)
{
	CameraX = 0;
	CameraY = 0;
	CameraWidth = 59;
	CameraHeight = 19;
	Creature* player = Program.GetSession()->Player;
	if (player != nullptr)
		CenterCameraOn(player->GetX(), player->GetY());
}

MapState::~MapState()
{
	//delete Map;
}

void MapState::OnInitialize()
{
	//Map = new ::Map(Program->Info, 80, 20);
	//Draw();
}

void MapState::OnUpdate(char key, bool isCommand)
{
	Map& map = *Program.GetSession()->ActiveMap;
	Creature* player = map.GetPlayer();
	if (IsLocked)
	{
		if (player != nullptr)
			if (player->CanMove())
				IsLocked = false;
			else
				map.Update();
		else
			IsLocked = false;
	}
	else
	{
		Input(key, isCommand);
	}

	//Draw();
}

void MapState::Input(char key, bool isCommand)
{
	Session& session = *Program.GetSession();
	Map& map = *session.ActiveMap;
	Creature* player = session.Player;
	bool moved = false;
	
	switch (key)
	{
	case UpArrow:	moved = MovePlayerBy(0, -1);	break;
	case LeftArrow: moved = MovePlayerBy(-1, 0);	break;
	case DownArrow: moved = MovePlayerBy(0, 1);		break;
	case RightArrow:moved = MovePlayerBy(1, 0);		break;
	case SouthWest: moved = MovePlayerBy(-1, 1);	break;
	case SouthEast: moved = MovePlayerBy(1, 1);		break;
	case NorthWest: moved = MovePlayerBy(-1, -1);	break;
	case NorthEast: moved = MovePlayerBy(1, -1);	break;
	case '.': 
		moved = true; 
		break;
	case ',':
		if (player != nullptr)
			moved = player->PickUpItem();
		break;
	case 'l':
		Program.ChangeState(new LogState(Program));
		break;
	case 'i':
		Program.ChangeState(new InventoryState(Program));
		break;
	case 'a':
		Program.ChangeState(new AttackMapState(Program));
		break;
	case 'k':
		Program.ChangeState(new LookMapState(Program));
		break;
	case 'r':
		//Program.GetSession()->ChangeMap(new Map(Program, 80, 20));
		break;
	case EnterKey:
		break;
	case EscapeKey:
		Program.ChangeState(new MainMenuState(Program));
		break;
	default:
		break;
	}

	if (player == nullptr)
		moved = false;

	if (moved)
	{
		IsLocked = true;
		player->Initiative += player->GetSlowness();
		map.Update();
	}
}

void MapState::OnDraw()
{
	Session& session = *Program.GetSession();
	Map& map = *session.ActiveMap;
	Creature* player = session.Player;
	Tile* origin = nullptr;
	if (player != nullptr)
	{
		origin = map.GetTileAt(player->GetPosition());
	}
	int maxX = CameraX + CameraWidth;
	int maxY = CameraY + CameraHeight;
	int row = 0, col = 0;
	for (int y = CameraY; y < maxY; y++, row++)
	{
		::move(row, col);
		for (int x = CameraX; x < maxX; x++)
		{
			Tile* tile = map.GetTileAt(x, y);
			if (tile != nullptr)
			{
				Creature* creature = tile->Creature;

				if (origin != nullptr && origin->HasVisibility(*tile))
				{
					Graphics::UseColor(tile->GetColor(), COLOR_BLACK);
					addch(tile->GetSymbol());
					tile->IsExplored = true;
				}
				else if (creature != nullptr && player != nullptr && player->CanHear(*creature))
				{
					Graphics::UseColor(COLOR_YELLOW, COLOR_BLACK);
					addch('!');
				}
				else if (tile->IsExplored)
				{
					Graphics::UseColor(COLOR_BLACK + 8, COLOR_BLACK);
					addch(tile->GetTerrainSymbol());
				}
				else
				{
					Graphics::UseColor(COLOR_BLACK, COLOR_BLACK);
					addch('?');
				}
			}
		}
	}

	row = 0, col = 60;
	if (session.Player != nullptr)
	{
		Creature& player = *session.Player;

		Graphics::UseColor(COLOR_WHITE + 8, COLOR_BLACK);
		mvprintw(row++, col, "HP: ");

		float hpRatio = player.GetHP() / (float)player.GetMaxHP();
		if (hpRatio < 0.333f)
			Graphics::UseColor(COLOR_RED + 8, COLOR_BLACK);
		else if (hpRatio < 0.666f)
			Graphics::UseColor(COLOR_YELLOW + 8, COLOR_BLACK);
		else
			Graphics::UseColor(COLOR_GREEN + 8, COLOR_BLACK);

		printw("%i", player.GetHP());

		Graphics::UseColor(COLOR_WHITE + 8, COLOR_BLACK);
		printw(" / %i ", player.GetMaxHP());

		mvprintw(row++, col, "W: ");
		int i = 0;
		for (Item* weapon : player.Hands)
		{
			if (i > 0)
				mvprintw(row++, col, " & ");
			printw("%s", ((WeaponInfo&)weapon->Info).Name.data());
			i++;
		}

		if (player.Armor != nullptr)
		{
			ArmorInfo& armor = (ArmorInfo&)player.Armor->Info;
			mvprintw(row++, col, "A: %s ", armor.Name.data());
		}
		else
		{
			mvprintw(row++, col, "A: ");
		}

		mvprintw(row++, col, "S: %i ", player.GetSpeed());
		mvprintw(row++, col, "E: %i", player.GetDodge() + player.GetParry());

		row++;
		mvprintw(row++, col, "D: %s", session.GetDateString().data());
		mvprintw(row++, col, "T: %s", session.GetTimeString().data());
		mvprintw(row++, col, "C: %i", map.Creatures.size());
	}

	::move(20, 0);
	Graphics::UseColor(COLOR_WHITE + 8, COLOR_BLACK);
	if (session.TemporaryLog != "")
	{
		printw("%s\n", session.TemporaryLog.data());
	}
	else
	{
		int i = 0;
		for (string entry : session.Log)
		{
			if (i > 0)
				Graphics::UseColor(COLOR_WHITE, COLOR_BLACK);

			printw("%s\n", entry.data());

			i++;
			if (i >= 4)
				break;
		}
	}

	Graphics::UseColor(COLOR_WHITE, COLOR_BLACK);
}

void MapState::MoveCameraTo(int x, int y)
{
	Map& map = *Program.GetSession()->ActiveMap;
	if (x < 0)
		x = 0;
	else if (x >= map.Width - CameraWidth)
		x = map.Width - CameraWidth;

	if (y < 0)
		y = 0;
	else if (y >= map.Height - CameraHeight)
		y = map.Height - CameraHeight;

	CameraX = x;
	CameraY = y;
}

void MapState::CenterCameraOn(int x, int y)
{
	MoveCameraTo(x - CameraWidth / 2, y - CameraHeight / 2);
}

bool MapState::MovePlayerBy(int dx, int dy)
{
	Creature* player = Program.GetSession()->Player;
	if (player != nullptr && player->MoveBy(dx, dy))
	{
		CenterCameraOn(player->GetX(), player->GetY());
		return true;
	}
	return false;
}
