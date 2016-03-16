#pragma once
#include "PathfinderNode.h"
#include <vector>
#include "pugixml.hpp"

using namespace std;

class Item;
class Creature;
class Map;
class PathfinderNode;
class InfoManager;
class TileInfo;

class Tile 
	: public PathfinderNode
{
public:
	TileInfo* Info;
	vector<Item*> Items = vector<Item*>();
	Creature* Creature = nullptr;
	unsigned char X, Y;
	bool IsExplored;

	Tile(unsigned char x, unsigned char y, TileInfo* info, Map& map);
	virtual ~Tile();

	void Initialize(Map& map);
	char GetSymbol();
	char GetTerrainSymbol();
	unsigned char GetColor();
	virtual int CalculateHeuristic(PathfinderNode& destination) override;
	double GetDistance(Tile& other);
	bool IsPassable();
	bool IsTransparent();
	bool HasVisibility(Tile& target);
	void SaveToXml(pugi::xml_node node);
	void LoadFromXml(pugi::xml_node node, InfoManager& info);

private:
	const unsigned char VisibilityRadius = 15;

	vector<bool> Visibility;
	unsigned char VisibilityWidth, VisibilityMinX, VisibilityMinY;

	void InitializeNeighbors(Map& map);
	void InitializeVisibility(Map& map);
	inline int GetVisibilityIndex(int x, int y);
};

