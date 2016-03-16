#include "TileInfo.h"



TileInfo::TileInfo()
{
}


TileInfo::~TileInfo()
{
}

bool TileInfo::IsPassable()
{
	return Passable;
}

bool TileInfo::IsTransparent()
{
	return Transparent;
}
