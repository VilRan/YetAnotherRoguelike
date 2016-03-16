#include "MapGenerator.h"
#include "Point.h"
#include <vector>
#include "VectorUtility.h"
#include "InfoManager.h"
#include "WeightedList.h"
#include "Math.h"

Room::Room()
{
}

Room::~Room()
{
}

void Room::Reset()
{
	ConnectsWest = false;
	ConnectsEast = false;
	ConnectsNorth = false;
	ConnectsSouth = false;
	Visited = false;
}

MapGenerator::MapGenerator(InfoManager& info, int width, int height, int roomSize)
	: Info(info)
{
	RoomSize = roomSize;
	Width = width / RoomSize + 1;
	Height = height / RoomSize + 1;
	int n = Width * Height;
	Rooms = new Room*[n];
	for (int i = 0; i < n; i++)
	{
		Rooms[i] = new Room();
	}
}

MapGenerator::~MapGenerator()
{
	int n = Width * Height;
	for (int i = 0; i < n; i++)
		delete Rooms[i];
	delete Rooms;
}

void MapGenerator::Generate(unsigned int seed, int tilesMin)
{
	srand(seed);
	do
		RunGeneration();
	while (GetNumberOfRooms() < tilesMin / RoomSize / RoomSize);
}

TileInfo & MapGenerator::GetTileAt(int x, int y)
{
	int roomX = x / RoomSize;
	int roomY = y / RoomSize;
	if (roomX >= Width || roomY >= Height)
		return *Info.Tiles["Wall"];

	Room* room = GetRoomAt(roomX, roomY);
	if (!room->Visited)
		return *Info.Tiles["Wall"];

	if (RoomSize > 1)
	{
		if (x % RoomSize == RoomSize - 1 && y % RoomSize == RoomSize - 1)
			return *Info.Tiles["Wall"];

		if (!room->ConnectsEast && x % RoomSize == RoomSize - 1)
			return *Info.Tiles["Wall"];

		if (!room->ConnectsSouth && y % RoomSize == RoomSize - 1)
			return *Info.Tiles["Wall"];
	}

	return *Info.Tiles["Floor"];
}

Point MapGenerator::GetEntrance()
{
	return Point(EntryX * RoomSize + (RoomSize - 1) / 2, EntryY * RoomSize + (RoomSize - 1) / 2);
}

int MapGenerator::GetNumberOfRooms()
{
	int count = 0;
	int n = Width * Height;
	for (int i = 0; i < n; i++)
	{
		if (Rooms[i]->Visited)
			count++;
	}
	return count;
}

void MapGenerator::RunGeneration()
{
	int n = Width * Height;
	for (int i = 0; i < n; i++)
		Rooms[i]->Reset();

	WeightedList<Direction> directionOfNeighbors = WeightedList<Direction>();
	WeightedList<int> numberOfNeighbors = WeightedList<int>();
	numberOfNeighbors.Add(0, 100);
	numberOfNeighbors.Add(1, 200);
	numberOfNeighbors.Add(2, 100);
	numberOfNeighbors.Add(3, 50);

	vector<Point*> open = vector<Point*>();
	EntryX = rand() % (Width - 1);
	EntryY = rand() % (Height - 1);
	open.push_back(new Point(EntryX, EntryY));

	do
	{
		Point* active = open[rand() % open.size()];
		int x = active->X, y = active->Y;
		Room* room = GetRoomAt(x, y);
		room->Visited = true;

		Direction origin = East;
		if (room->ConnectsNorth)
			origin = North;
		else if (room->ConnectsSouth)
			origin = South;
		else if (room->ConnectsWest)
			origin = West;

		Room* west = GetRoomAt(x - 1, y);
		Room* east = GetRoomAt(x + 1, y);
		Room* north = GetRoomAt(x, y - 1);
		Room* south = GetRoomAt(x, y + 1);

		if (west != nullptr && !west->Visited)
		{
			int weight = 50;
			if (origin == East)
				weight = 75;
			directionOfNeighbors.Add(West, weight);
		}
		if (east != nullptr && !east->Visited)
		{
			int weight = 50;
			if (origin == West)
				weight = 75;
			directionOfNeighbors.Add(East, weight);
		}
		if (north != nullptr && !north->Visited)
		{
			int weight = 50;
			if (origin == South)
				weight = 75;
			directionOfNeighbors.Add(North, weight);
		}
		if (south != nullptr && !south->Visited)
		{
			int weight = 50;
			if (origin == North)
				weight = 75;
			directionOfNeighbors.Add(South, weight);
		}

		int n = Math::Min(numberOfNeighbors.GetRandom(), directionOfNeighbors.GetCount());
		while (n > 0)
		{
			Direction dir = directionOfNeighbors.GetRandom();
			switch (dir)
			{
			case West:
				room->ConnectsWest = true;
				west->ConnectsEast = true;
				open.push_back(new Point(x - 1, y));
				break;

			case East:
				room->ConnectsEast = true;
				east->ConnectsWest = true;
				open.push_back(new Point(x + 1, y));
				break;

			case North:
				room->ConnectsNorth = true;
				north->ConnectsSouth = true;
				open.push_back(new Point(x, y - 1));
				break;

			case South:
				room->ConnectsSouth = true;
				south->ConnectsNorth = true;
				open.push_back(new Point(x, y + 1));
				break;
			}
			n--;
		}
		
		directionOfNeighbors.Clear();
		VectorUtility::Remove(open, active);
		delete active;
	} while (open.size() > 0);
}

Room * MapGenerator::GetRoomAt(int x, int y)
{
	if (x < 0 || x >= Width || y < 0 || y >= Height)
		return nullptr;
	return Rooms[x + y * Width];
}
