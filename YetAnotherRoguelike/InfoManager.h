#pragma once
#include <unordered_map>
#include <string>
#include "WeaponInfo.h"
#include "ArmorInfo.h"
#include "FactionInfo.h"
#include "BehaviorInfo.h"
#include "ItemUseEffect.h"

class FactionInfo;
class ItemInfo;
class CreatureInfo;
class ItemUseEffect;
class BehaviorInfo;
class TileInfo;
class RoomInfo;
class ItemSetInfo;

class InfoManager
{
public:
	unordered_map<string, FactionInfo*> Factions;
	unordered_map<string, BehaviorInfo*> Behaviors;
	unordered_map<string, ItemUseEffect*> ItemEffects;
	unordered_map<string, ItemInfo*> Items;
	unordered_map<string, ItemSetInfo*> ItemSets;
	unordered_map<string, CreatureInfo*> Creatures;
	unordered_map<string, TileInfo*> Tiles;
	unordered_map<string, RoomInfo*> Rooms;

	InfoManager();
	~InfoManager();

private:
	void LoadFactions();
	void LoadBehaviors();
	void LoadItemEffects();
	void LoadItems();
	void LoadItemSets();
	void LoadCreatures();
	void LoadTiles();
	void LoadRooms();
};

