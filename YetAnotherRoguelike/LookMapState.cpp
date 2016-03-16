#include "LookMapState.h"
#include "Session.h"
#include "Map.h"
#include "Creature.h"
#include "CreatureInfo.h"
#include "Tile.h"
#include "Item.h"
#include "Program.h"

LookMapState::LookMapState(::Program& program) 
	: CursorMapState(program)
{
	UpdateLog();
}

LookMapState::~LookMapState()
{
}

void LookMapState::Input(char key, bool isCommand)
{
	CursorMapState::Input(key, isCommand);
	UpdateLog();

	Session& session = *Program.GetSession();
	switch (key)
	{
	case EscapeKey:
		session.TemporaryLog = "";
		break;
	default:
		break;
	}
}

void LookMapState::UpdateLog()
{
	Session& session = *Program.GetSession();
	session.TemporaryLog = "";

	Map& map = *session.ActiveMap;
	Tile& tile = *map.GetTileAt(CursorX, CursorY);
	Creature* player = session.Player;

	if (player != nullptr && player->GetTile().HasVisibility(tile))
	{
		Creature* creature = tile.Creature;
		if (creature != nullptr)
		{
			if (creature != session.Player)
				session.TemporaryLog += "You see " + creature->Info.GetArticleAndName() + ". "
				+ "( HP: " + to_string(creature->GetHP()) + " / " + to_string(creature->GetMaxHP()) + " )\n";
			else
				session.TemporaryLog += "You see yourself.\n";
		}
		if (tile.Items.size() > 0)
		{
			if (tile.Items.size() > 1)
				session.TemporaryLog += "You see multiple items.\n";
			else
				session.TemporaryLog += "You see " + tile.Items[0]->Info.GetArticleAndName() + ".\n";
		}
	}
}
