#pragma once
#include <string>

using namespace std;

class TileInfo
{
public:
	string ID;
	char Symbol = '.';
	unsigned char Color;
	bool Passable = false;
	bool Transparent = true;

	TileInfo();
	~TileInfo();

	bool IsPassable();
	bool IsTransparent();
};

