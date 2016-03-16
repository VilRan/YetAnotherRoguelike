#include "Tile.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "curses.h"
#include "Item.h"
#include "Map.h"
#include <iostream>
#include <fstream>
#include "InfoManager.h"
#include "Creature.h"
#include "TileInfo.h"
#include "Math.h"

using namespace pugi;

Tile::Tile(unsigned char x, unsigned char y, TileInfo* info, Map& map)
	: Info(info)
{
	X = x;
	Y = y;
	VisibilityMinX = Math::Max(X - VisibilityRadius, 0);
	VisibilityMinY = Math::Max(Y - VisibilityRadius, 0);
	int maxX = Math::Min(X + VisibilityRadius, map.Width);
	int maxY = Math::Min(Y + VisibilityRadius, map.Height);
	VisibilityWidth = maxX - VisibilityMinX;
	int visibilityHeight = maxY - VisibilityMinY;
	Visibility = vector<bool>(VisibilityWidth * visibilityHeight, false);
}

Tile::~Tile()
{
	for (Item* item : Items)
	{
		delete item;
	}
}

char Tile::GetSymbol()
{
	if (Creature != nullptr)
		return Creature->GetSymbol();
	if (Items.size() > 0)
		return Items.back()->Info.Symbol;
	return Info->Symbol;
}

char Tile::GetTerrainSymbol()
{
	return Info->Symbol;
}

unsigned char Tile::GetColor()
{
	if (Creature != nullptr)
		return Creature->GetColor();
	if (Items.size() > 0)
		return Items.back()->Info.Color;
	return Info->Color;
}

void Tile::Initialize(Map& map)
{
	InitializeNeighbors(map);
	InitializeVisibility(map);
}

int Tile::CalculateHeuristic(PathfinderNode& destination)
{
	Tile& other = (Tile&)destination;
	int heuristic = (int)(10 * GetDistance(other));
	if (Creature != nullptr)
		heuristic += 100;
	return heuristic;
}

double Tile::GetDistance(Tile & other)
{
	int dx = X - other.X;
	int dy = Y - other.Y;
	return sqrt(dx * dx + dy * dy);
}

bool Tile::IsPassable()
{
	return Info->IsPassable();
}

bool Tile::IsTransparent()
{
	return Info->IsTransparent();
}

bool Tile::HasVisibility(Tile & target)
{
	if (abs(target.X - X) >= VisibilityRadius || abs(target.Y - Y) >= VisibilityRadius)
		return false;

	int minX = Math::Max(X - VisibilityRadius, 0);
	int minY = Math::Max(Y - VisibilityRadius, 0);

	int i = (target.X - minX) + (target.Y - minY) * VisibilityWidth;
	return Visibility[i];
}

void Tile::SaveToXml(xml_node node)
{
	node.append_child("Type").append_child(node_pcdata).set_value(Info->ID.data());
	node.append_child("Explored").append_child(node_pcdata).set_value(to_string(IsExplored).data());

	if (Items.size() > 0)
	{
		xml_node items = node.append_child("Items");
		for (Item* item : Items)
		{
			items.append_child("Item").append_child(node_pcdata).set_value(item->Info.ID.data());
		}
	}
}

void Tile::LoadFromXml(pugi::xml_node node, InfoManager& info)
{
	Info = info.Tiles[node.child("Type").child_value()];
	IsExplored = node.child("Explored").child_value()[0] == '1';

	xml_node items = node.child("Items");
	for (xml_node item : items.children())
	{
		Items.push_back( new Item( *info.Items[item.child_value()] ) );
	}
}

void Tile::InitializeNeighbors(Map & map)
{
	Neighbors.clear();

	int minX = X > 0 ? X - 1 : 0;
	int minY = Y > 0 ? Y - 1 : 0;
	int maxX = X < map.Width - 1 ? X + 1 : map.Width - 1;
	int maxY = Y < map.Height - 1 ? Y + 1 : map.Height - 1;

	for (int x = minX; x <= maxX; x++)
		for (int y = minY; y <= maxY; y++)
		{
			Tile* other = map.GetTileAt(x, y);

			if (other != this && other->IsPassable())
			{
				bool diagonal = (abs(X - x) + abs(Y - y) == 2);
				int cost = diagonal ? 15 : 10;
				Neighbors.push_back(PathfinderLink(*other, cost));
			}
		}
}

void Tile::InitializeVisibility(Map & map)
{
	for (double a = 0; a < M_PI * 2; a += M_PI * 2 / 180)
	{
		double x = X + 0.5;
		double y = Y + 0.5;
		double stepX = cos(a);
		double stepY = sin(a);

		for (int d = 0; d < VisibilityRadius; d++)
		{
			int iX = (int)x;
			int iY = (int)y;
			if (iX < 0 || iX >= map.Width || iY < 0 || iY >= map.Height)
				break;

			int i = GetVisibilityIndex(iX, iY);
			Visibility[i] = true;
			Tile& other = *map.GetTileAt(iX, iY);
			//other.Visibility[(X - other.VisibilityMinX) + (Y - other.VisibilityMinY) * other.VisibilityWidth] = true;
			if (!other.IsTransparent())
				break;

			x += stepX;
			y += stepY;
		}
		
	}
}

int Tile::GetVisibilityIndex(int x, int y)
{
	return (x - VisibilityMinX) + (y - VisibilityMinY) * VisibilityWidth;
}
