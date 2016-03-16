#pragma once
#include <vector>
#include "pugixml.hpp"

using namespace std;

class InfoManager;
class CreatureInfo;
class WeaponInfo;
class ArmorInfo;
class FactionInfo;
class Tile;
class Map;
class Item;
class Program;
class Point;

class Creature
{
public:
	CreatureInfo& Info;
	FactionInfo* Faction;
	Map* Map;
	Item* Armor;
	vector<Item*> Hands;
	vector<Item*> Inventory;

	// How many time units it will take until the creature can perform its next move.
	short Initiative = 1000;

	Creature(CreatureInfo& info, ::Map* map, bool initialize = true);
	~Creature();

	void Update(int ticks);

	// Returns false if the item is not ItemType::Weapon, or if the creature has no hands left to grasp the item with.
	bool Equip(Item* item);

	// Removes the given item from the creatures Hands and places it into the Inventory.
	void Unequip(Item* item);

	// Returns false if the item is not ItemType::Armor. Otherwise Unwears current Armor and replaces it with the given Armor.
	bool Wear(Item* item);

	// The creature unwears it current Armor and places it into the creature's Inventory.
	void Unwear();

	// Removes the item from the creature's inventory without releasing the memory used by it.
	void RemoveItem(Item* item);

	// Completely deletes the target item, removing it from the creature's inventory and releasing the memory used by it.
	void DeleteItem(Item* item);

	// Makes the creature attempt to pick up the first item lying on the same tile.
	bool PickUpItem();

	// Returns false if the creature is not on any map.
	bool DropItem(Item* item);
	bool UseItem(Item* item);
	int RollDamageVs(Creature& target, Item& weapon);

	// For melee combat, evasion is the sum of the creature's Dodge and Parry scores.
	int GetChanceToHit(Creature& target, Item& weapon);

	// Rolls a random number between 0 and the creature's accuracy with the chosen weapon.
	int RollAccuracy(Item& weapon);

	// Rolls a random number between 0 and the sum of the creature's evasion modifiers.
	int RollEvasion();

	// Final dodge value does not only depend on skill, but also on how much maneuvering room they have around them.
	int GetDodge();
	int GetParry();

	//int GetInitiative();
	int GetSlowness();

	// Returns true if the creature's Initiative score is zero or less.
	bool CanMove();

	// Returns true if the creature is player controlled.
	bool IsPlayer();

	// Sets the creature's current HP, taking MaxHP into account.
	void SetHP(short value);

	short GetHP();
	short GetMaxHP();
	short GetSpeed();
	Point GetPosition();
	signed char GetX();
	signed char GetY();
	char GetSymbol();
	unsigned char GetColor();

	// Returns true if the target creature's faction is the same or the relation score between the creatures' factions is 50 or greater.
	bool IsAlliedWith(Creature& creature);

	// Returns true if the creature is willing to swap places with the specified creature.
	bool WillSwapWith(Creature& initiator);

	// Returns true if the creature's HP is 0 or below.
	bool IsDying();

	// Returns true if the creature has line of sight to the target creature.
	bool CanSee(Creature& target);
	bool CanHear(Creature& target);

	// Returns true if the creature can either see or hear the target creature.
	bool CanDetect(Creature& target);

	// Returns false if the creature is incapable of attacking the target.
	bool TryAttack(Creature& other);

	// The creature attacks the target with all equipped weapons.
	void Attack(Creature& other);

	// The creature attacks the target with the referenced weapon.
	void Attack(Creature& other, Item& weapon);

	// Returns false if the creature fails to move to the target tile;
	// for example, if the target tile is blocked by another creature or is impassable.
	bool MoveTo(Tile& target);

	// Returns false if the creature fails to move to the target coordinates on its current map,
	// for example, if the target tile is blocked by another creature or is impassable.
	bool MoveTo(int x, int y);

	// Returns false if the creature fails to move by the determined amount;
	// for example, if the target tile is blocked by another creature or is impassable.
	bool MoveBy(int dx, int dy);

	// The creature finds a path and moves one step towards the target.
	bool MoveTowards(Tile& target);

	// The creature finds a path and moves one step towards the target.
	bool MoveTowards(Creature& target);

	// The creature finds a path and moves one step towards the target.
	bool MoveTowards(int x, int y);

	// Moves one step in a random direction, or doesn't move at all.
	bool MoveRandomly();

	// Returns the nearest creature matching the given parameters.
	Creature* FindNearestCreature(int range, bool isAlly);

	// Gets the tile the creature is currently standing on.
	Tile& GetTile();

	// Saves the creature under the given node.
	void SaveToXml(pugi::xml_node node);

	// Creates a new creature based on an xml node and returns it.
	static Creature* LoadFromXml(pugi::xml_node node, Program& program);

private:
	short HP = 100, MaxHP = 100, Speed = 1000;
	signed char X = -1, Y = -1, Dodge = 0;
	unsigned int UID = 0;

};

