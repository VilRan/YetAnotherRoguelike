#pragma once
#include <vector>
#include "InfoManager.h"
#include "pugixml.hpp"

using namespace std;

class Program;
class Tile;
class Creature;
class Point;
class Map
{
public:
	Program& Program;
	Tile** Tiles;
	vector<Creature*> Creatures;
	vector<Creature*> Dying;
	unsigned char Width = 40;
	unsigned char Height = 20;

	Map(::Program& program);
	Map(::Program& program, unsigned char width, unsigned char height, unsigned int seed);
	~Map();

	void Initialize();
	void Update();

	// Adds an existing creature on the map.
	bool AddCreature(Creature* creature, int x, int y);

	// Adds the current session's player creature on the map.
	bool AddPlayer(int x, int y);

	// Spawns a new creature to the map and automatically adds it to the session.
	Creature* SpawnCreature(CreatureInfo& creatureType, int x, int y);

	// Gets the current session's player creature.
	Creature* GetPlayer();

	void KillCreature(Creature* creature, string causeOfDeath);

	// Returns nullptr if out of bounds.
	Tile* GetTileAt(int x, int y);
	// Returns nullptr if out of bounds.
	Tile* GetTileAt(Point point);

	// Saves the map under the given xml node.
	void SaveToXml(pugi::xml_node node);

	// Creates a new map based on the given xml node and returns it.
	static Map* LoadFromXml(pugi::xml_node node, ::Program& program);


	unsigned int GetUID();

private:
	unsigned int UID = 0;
};

