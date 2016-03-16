#pragma once
#include <vector>
#include "WeightedList.h"

using namespace std;

class TileInfo;
class Point;
class RoomInfoTile
{
public:
	TileInfo& Type;
	int X;
	int Y;

	RoomInfoTile(TileInfo& type, int x, int y);
	~RoomInfoTile();

};

class RoomInfoRandom
{
public:
	WeightedList<TileInfo&> Types;
	vector<Point> Coordinates;

	RoomInfoRandom(WeightedList<TileInfo&> types, vector<Point> coordinates);
	~RoomInfoRandom();
};

class RoomInfo
{
public:
	string ID;
	double Weight;

	RoomInfo();
	~RoomInfo();

	void AddTile(TileInfo& type, int x, int y);
	void AddRandom(WeightedList<TileInfo&> types, vector<Point> coordinates);
	vector<RoomInfoTile> GetTiles();
	unsigned char GetWidth();
	unsigned char GetHeight();

private:
	vector<RoomInfoTile> Static;
	vector<RoomInfoRandom> Random;

	unsigned char Width, Height;
};

