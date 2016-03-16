#include "AttackMapState.h"
#include "Program.h"
#include "Session.h"
#include "Creature.h"
#include "Map.h"
#include "Tile.h"


AttackMapState::AttackMapState(::Program& program) 
	: LookMapState(program)
{
}

AttackMapState::~AttackMapState()
{
}

void AttackMapState::Input(char key, bool isCommand)
{
	LookMapState::Input(key, isCommand);

	Session& session = *Program.GetSession();
	Map& map = *session.ActiveMap;
	Creature* player = session.Player;

	switch (key)
	{
	case EnterKey:
		if (player != nullptr)
		{
			Tile* tile = map.GetTileAt(CursorX, CursorY);
			Creature* creature = tile->Creature;
			if (creature != nullptr)
			{
				if (player->TryAttack(*creature))
				{
					player->Initiative += player->GetSlowness();
					MapState* mapState = new MapState(Program);
					mapState->IsLocked = true;
					Program.ChangeState(mapState);
					session.TemporaryLog = "";
				}
			}
		}
		break;
	default:
		break;
	}
}