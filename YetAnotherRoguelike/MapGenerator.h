#pragma once

using namespace std;

enum Direction
{
	West,
	East,
	North,
	South
};

class InfoManager;
class TileInfo;
class Point;
class Room
{
public:
	bool 
		ConnectsWest = false, 
		ConnectsEast = false, 
		ConnectsNorth = false, 
		ConnectsSouth = false,
		Visited = false;

	Room();
	~Room();

	void Reset();
};

class MapGenerator
{
public:
	MapGenerator(InfoManager& info, int width, int height, int roomSize);
	~MapGenerator();

	/*
	* seed = seed for random generation.
	* roomsMin = the minimum number of rooms the map should contain.
	*/
	void Generate(unsigned int seed, int tilesMin);
	TileInfo& GetTileAt(int x, int y);
	Point GetEntrance();
	int GetNumberOfRooms();

private:
	InfoManager& Info;
	Room** Rooms;
	int Width, Height, EntryX, EntryY, RoomSize;

	void RunGeneration();
	Room* GetRoomAt(int x, int y);
};

