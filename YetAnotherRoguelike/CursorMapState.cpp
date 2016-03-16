#include "CursorMapState.h"
#include "curses.h"
#include "Map.h"
#include "Session.h"
#include "Creature.h"
#include "CreatureInfo.h"
#include "Item.h"
#include "Program.h"
#include "Graphics.h"

CursorMapState::CursorMapState(::Program& program) : MapState(program)
{
	Creature* player = Program.GetSession()->Player;
	if (player != nullptr)
	{
		CursorX = player->GetX();
		CursorY = player->GetY();
	}
	else
	{
		CursorX = 0;
		CursorY = 0;
	}
	CenterCameraOn(CursorX, CursorY);
}

CursorMapState::~CursorMapState()
{
}

void CursorMapState::Input(char key, bool isCommand)
{
	Session& session = *Program.GetSession();

	switch (key)
	{
	case UpArrow:	CursorY--;	break;
	case LeftArrow: CursorX--;	break;
	case DownArrow: CursorY++;	break;
	case RightArrow:CursorX++;	break;
	case SouthWest: CursorY++; CursorX--;	break;
	case SouthEast: CursorY++; CursorX++;	break;
	case NorthWest: CursorY--; CursorX--;	break;
	case NorthEast: CursorY--; CursorX++;	break;
	case EscapeKey:
		Program.ChangeState(new MapState(Program));
		return;
	default:
		break;
	}

	Map& map = *session.ActiveMap;

	if (CursorX < 0)
		CursorX = 0;
	if (CursorY < 0)
		CursorY = 0;
	if (CursorX >= map.Width)
		CursorX = map.Width - 1;
	if (CursorY >= map.Height)
		CursorY = map.Height - 1;

	CenterCameraOn(CursorX, CursorY);
}

void CursorMapState::OnDraw()
{
	MapState::OnDraw();

	int rows, cols;
	getmaxyx(stdscr, rows, cols);
	Graphics::UseColor(COLOR_WHITE + 8, COLOR_BLACK);
	mvaddch(CursorY - CameraY + 0, CursorX - CameraX, 'X');
	move(rows - 1, 0);
	Graphics::UseColor(COLOR_WHITE, COLOR_BLACK);
}