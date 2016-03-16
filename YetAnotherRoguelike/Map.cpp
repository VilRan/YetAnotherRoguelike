#include "Map.h"
#include <stdlib.h>
#include <algorithm>
#include "Program.h"
#include "Session.h"
#include "Item.h"
#include "WeaponInfo.h"
#include "ArmorInfo.h"
#include "CreatureInfo.h"
#include "VectorUtility.h"
#include "Tile.h"
#include "TileInfo.h"
#include "Creature.h"
#include "InfoManager.h"
#include "MapGenerator.h"
#include "Point.h"
#include "WeightedList.h"
#if _DEBUG
#include "curses.h"
#endif

using namespace pugi;

#pragma region Constructors and Destructor
Map::Map(::Program& program) : Program(program)
{
	UID = program.GetSession()->MapCounter++;
}

Map::Map(::Program & program, unsigned char width, unsigned char height, unsigned int seed) : Program(program)
{
	UID = program.GetSession()->MapCounter++;
	Width = width;
	Height = height;
	Tiles = new Tile*[Width * Height];

	InfoManager& info = Program.GetInfo();
	MapGenerator generator = MapGenerator(info, Width, Height, 4);
	generator.Generate(seed, Width * Height / 2);
	Point entry = generator.GetEntrance();
	
	WeightedList<CreatureInfo*> creatures = WeightedList<CreatureInfo*>();
	creatures.Add(nullptr, 5000);
	creatures.Add(info.Creatures["Zombie"], 100);
	creatures.Add(info.Creatures["ZombieWarrior"], 20);
	creatures.Add(info.Creatures["Guard"], 20);
	creatures.Add(info.Creatures["Knight"], 5);
	creatures.Add(info.Creatures["RedDragon"], 1);

	WeightedList<ItemInfo*> treasures = WeightedList<ItemInfo*>();
	treasures.Add(nullptr, 10000);
	treasures.Add(info.Items["HealingPotion"], 100);

	int x, y, i;
	for (x = 0; x < Width; x++)
	for (y = 0; y < Height; y++)
	{
		i = x + y * Width;
		Tiles[i] = new Tile(x, y, &generator.GetTileAt(x, y), *this);

		if (Tiles[i]->IsPassable() && x != entry.X && y != entry.Y)
		{
			CreatureInfo* creature = creatures.GetRandom();
			if (creature != nullptr)
				SpawnCreature(*creature, x, y);

			ItemInfo* treasure = treasures.GetRandom();
			if (treasure != nullptr)
				Tiles[i]->Items.push_back(new Item(*treasure));
		}
	}

	Initialize();
	AddPlayer(entry.X, entry.Y);
}

void Map::Initialize()
{
#if _DEBUG
	clear();
	mvprintw(1, 1, "Initializing Map %i...", UID);
#endif
	int n = Width * Height;
	for (int i = 0; i < n; i++)
	{
#if _DEBUG
		mvprintw(3, 1, "Initializing Tile %i / %i", i, n);
		refresh();
#endif
		Tiles[i]->Initialize(*this);
	}
}

Map::~Map()
{
	int n = Width * Height;
	for (int i = 0; i < n; i++)
	{
		delete Tiles[i];
	}
	delete Tiles;
}
#pragma endregion

void Map::Update()
{
	Session& session = *Program.GetSession();

	int fastestInitiative = 1000000;
	for (Creature * creature : Creatures)
	{
		if (creature->Initiative < fastestInitiative)
		{
			fastestInitiative = creature->Initiative;
		}
	}
	for (Creature * creature : Creatures)
	{
		creature->Update(fastestInitiative);
	}

	for (Creature * creature : Dying)
	{
		VectorUtility::Remove(Creatures, creature);
		session.DeleteCreature(creature);
	}
	Dying.clear();

	session.Time += fastestInitiative;
}

#pragma region Creatures
bool Map::AddCreature(Creature * creature, int x, int y)
{
	if (creature == nullptr)
		return false;

	if (VectorUtility::Contains(Creatures, creature))
		return false;

	creature->Map = this;
	if (creature->MoveTo(x, y))
	{
		Creatures.push_back(creature);
		return true;
	}

	creature->Map = nullptr;
	return false;
}

bool Map::AddPlayer(int x, int y)
{
	return AddCreature(Program.GetSession()->Player, x, y);
}

Creature* Map::SpawnCreature(CreatureInfo & creatureType, int x, int y)
{
	Session& session = *Program.GetSession();
	Creature* creature = new Creature(creatureType, this);
	if (AddCreature(creature, x, y))
	{
		session.Creatures.push_back(creature);
		return creature;
	}
	delete creature;
	return nullptr;
}

Creature * Map::GetPlayer()
{
	return Program.GetSession()->Player;
}

void Map::KillCreature(Creature * creature, string causeOfDeath)
{
	Tile& tile = creature->GetTile();

	if (creature->Armor != nullptr && creature->Armor->IsDroppable())
	{
		tile.Items.push_back(creature->Armor);
		creature->Armor = nullptr;
	}

	for (Item* item : creature->Hands)
	{
		if (item->IsDroppable())
			tile.Items.push_back(item);
		else
			delete item;
	}
	creature->Hands.clear();

	for (Item* item : creature->Inventory)
	{
		if (item->IsDroppable())
			tile.Items.push_back(item);
		else
			delete item;
	}
	creature->Inventory.clear();

	Session& session = *Program.GetSession();
	if (creature == GetPlayer())
	{
		session.AddLogEntry("You have died!");
	}
	else
	{
		if (GetPlayer()->CanSee(*creature))
		session.AddLogEntry("The " + creature->Info.Name + " " + causeOfDeath);
	}

	Dying.push_back(creature);
	tile.Creature = nullptr;
}
#pragma endregion

Tile* Map::GetTileAt(int x, int y)
{
	if (x < 0 || y < 0 || x >= Width || y >= Height)
		return nullptr;

	return Tiles[x + y * Width];
}

Tile * Map::GetTileAt(Point point)
{
	return GetTileAt(point.X, point.Y);
}

#pragma region XML
void Map::SaveToXml(pugi::xml_node node)
{
	node.append_child("Width").append_child(node_pcdata).set_value(to_string(Width).data());
	node.append_child("Height").append_child(node_pcdata).set_value(to_string(Height).data());
	
	xml_node tiles = node.append_child("Tiles");
	int max = Width * Height;
	for (int i = 0; i < max; i++)
	{
		Tiles[i]->SaveToXml(tiles.append_child("Tile"));
	}
}

Map * Map::LoadFromXml(pugi::xml_node node, ::Program & program)
{
	Map* map = new Map(program);
	Session& session = *program.GetSession();
	InfoManager& info = program.GetInfo();

	map->Width = stoi(node.child("Width").child_value());
	map->Height = stoi(node.child("Height").child_value());
	map->Tiles = new Tile*[map->Width * map->Height];

	int x = 0, y = 0;
	xml_node tiles = node.child("Tiles");
	for (xml_node tile : tiles.children())
	{
		int i = x + y * map->Width;
		map->Tiles[i] = new Tile(x, y, info.Tiles["Wall"], *map);
		map->Tiles[i]->LoadFromXml(tile, info);
		x++;
		if (x == map->Width)
		{
			x = 0;
			y++;
		}
	}
	map->Initialize();

	return map;
}
#pragma endregion

unsigned int Map::GetUID()
{
	return UID;
}


