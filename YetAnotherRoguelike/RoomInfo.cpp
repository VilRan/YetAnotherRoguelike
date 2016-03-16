#include "RoomInfo.h"
#include "Point.h"

RoomInfoTile::RoomInfoTile(TileInfo & type, int x, int y)
	: Type(type), X(x), Y(y)
{

}

RoomInfoTile::~RoomInfoTile()
{

}

RoomInfoRandom::RoomInfoRandom(WeightedList<TileInfo&> types, vector<Point> coordinates)
{
}

RoomInfoRandom::~RoomInfoRandom()
{
}

RoomInfo::RoomInfo()
{

}

RoomInfo::~RoomInfo()
{

}

void RoomInfo::AddTile(TileInfo & type, int x, int y)
{
	Static.push_back(RoomInfoTile(type, x, y));
	if (x >= Width)
		Width = x + 1;
	if (y >= Height)
		Height = y + 1;
}

void RoomInfo::AddRandom(WeightedList<TileInfo&> types, vector<Point> coordinates)
{
	Random.push_back(RoomInfoRandom(types, coordinates));
}

vector<RoomInfoTile> RoomInfo::GetTiles()
{
	vector<RoomInfoTile> tiles = vector<RoomInfoTile>();
	for (RoomInfoTile tile : Static)
	{
		tiles.push_back(tile);
	}
	for (RoomInfoRandom random : Random)
	{
		TileInfo& type = random.Types.GetRandom();
		for (Point point : random.Coordinates)
		{
			tiles.push_back(RoomInfoTile(type, point.X, point.Y));
		}
	}
	return tiles;
}

unsigned char RoomInfo::GetWidth()
{
	return Width;
}

unsigned char RoomInfo::GetHeight()
{
	return Height;
}
