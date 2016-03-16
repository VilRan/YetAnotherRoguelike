#include "Creature.h"
#include "curses.h"
#include "Pathfinder.h"
#include "CreatureInfo.h"
#include "WeaponInfo.h"
#include "ArmorInfo.h"
#include "BehaviorInfo.h"
#include "Item.h"
#include "Map.h"
#include "Tile.h"
#include "VectorUtility.h"
#include "ProgramState.h"
#include "Program.h"
#include "Session.h"
#include "Point.h"
#include "ItemSetInfo.h"

using namespace pugi;

#pragma region Constructor and Destructor
Creature::Creature(CreatureInfo& info, ::Map* map, bool initialize) 
	: Info(info), Map(map)
{
	//UID = map->Program.GetSession()->CreatureCounter++;

	if (!initialize)
		return;

	HP = MaxHP = Info.MaxHP;
	
	if (Info.SpeedMax != Info.SpeedMin)
		Speed = rand() % (Info.SpeedMax - Info.SpeedMin) + Info.SpeedMin;
	else
		Speed = Info.SpeedMax;

	if (Info.DodgeMax != Info.DodgeMin)
		Dodge = rand() % (Info.DodgeMax - Info.DodgeMin) + Info.DodgeMin;
	else
		Dodge = Info.DodgeMax;

	Initiative = rand() % Speed;

	Faction = &info.DefaultFaction;

	vector<ItemInfo*> items = vector<ItemInfo*>();
	info.ItemSet.GetItems(&items);
	for (ItemInfo* itemInfo : items)
	{
		Item* item = new Item(*itemInfo);
		switch (itemInfo->GetType())
		{
		case ItemType::Weapon:
			if (!Equip(item))
				Inventory.push_back(item);
			break;
		case ItemType::Armor:
			if (!Wear(item))
				Inventory.push_back(item);
			break;
		case ItemType::Consumable:
			Inventory.push_back(item);
			break;
		default:
			Inventory.push_back(item);
			break;
		}
	}
	/*
	if (info.DefaultArmor != nullptr)
		Armor = new Item(*info.DefaultArmor);
	else
		Armor = nullptr;

	for (WeaponInfo* weapon : Info.DefaultWeapons)
	{
		Item* item = new Item(*weapon);
		if (!Equip(item))
			delete item;
	}
	*/
}

Creature::~Creature()
{
	delete Armor;
	for (Item* item : Hands)
	{
		delete item;
	}
	for (Item* item : Inventory)
	{
		delete item;
	}
}
#pragma endregion

void Creature::Update(int ticks)
{
	if (IsDying())
		return;

	Initiative -= ticks;

	if (IsPlayer())
		return;

	if (CanMove())
	{
		Info.DefaultBehavior.Run(*this);
		Initiative += GetSlowness();
	}
}

#pragma region Items
bool Creature::Equip(Item * item)
{
	if (item->Info.GetType() != ItemType::Weapon)
		return false;

	if (Hands.size() < 2)
	{
		VectorUtility::Remove(Inventory, item);
		Hands.push_back(item);
		return true;
	}
	return false;
}

void Creature::Unequip(Item * item)
{
	bool success = false;
	if (VectorUtility::Contains(Hands, item))
	{
		VectorUtility::Remove(Hands, item);
		success = true;
	}

	if (Armor == item)
	{
		Armor = nullptr;
		success = true;
	}

	if (success)
	{
		if (item->IsDroppable())
			Inventory.push_back(item);
		else
			delete item;
	}
}

bool Creature::Wear(Item * item)
{
	if (item->Info.GetType() != ItemType::Armor)
		return false;
	Unwear();
	VectorUtility::Remove(Inventory, item);
	Armor = item;
	return true;
}

void Creature::Unwear()
{
	if (Armor != nullptr)
	{
		if (Armor->IsDroppable())
			Inventory.push_back(Armor);
		else
			delete Armor;
		Armor = nullptr;
	}
}

void Creature::RemoveItem(Item * item)
{
	VectorUtility::Remove(Hands, item);
	VectorUtility::Remove(Inventory, item);

	if (Armor == item)
		Armor = nullptr;
}

void Creature::DeleteItem(Item * item)
{
	RemoveItem(item);
	delete item;
}

bool Creature::PickUpItem()
{
	Session& session = *Map->Program.GetSession();
	Tile& tile = GetTile();
	if (tile.Items.size() > 0)
	{
		Item& item = *tile.Items.back();
		Inventory.push_back(&item);
		tile.Items.pop_back();
		session.AddLogEntry("You picked up " + item.Info.GetArticleAndName() + ".");
		return true;
	}
	return false;
}

bool Creature::DropItem(Item * item)
{
	Session& session = *Map->Program.GetSession();
	if (Map != nullptr)
	{
		RemoveItem(item);
		Tile& tile = GetTile();
		tile.Items.push_back(item);
		session.AddLogEntry("You dropped " + item->Info.GetArticleAndName() + ".");
		return true;
	}
	return false;
}

bool Creature::UseItem(Item * item)
{
	if (item->Use(*this))
	{
		VectorUtility::Remove(Inventory, item);
		delete item;
		return true;
	}
	return false;
}

#pragma endregion

#pragma region Getters
int Creature::RollDamageVs(Creature& target, Item& weapon)
{
	WeaponInfo& weaponInfo = (WeaponInfo&)weapon.Info;
	short attack = weaponInfo.DamageMin;
	if (1 + weaponInfo.DamageMax - weaponInfo.DamageMin > 0)
		attack += rand() % (1 + weaponInfo.DamageMax - weaponInfo.DamageMin);
	
	short defense = 0;
	if (target.Armor != nullptr)
	{
		ArmorInfo& armor = (ArmorInfo&)target.Armor->Info;
		defense = armor.DefenseMin;
		if (1 + armor.DefenseMax - armor.DefenseMin > 0)
			defense += rand() % (1 + armor.DefenseMax - armor.DefenseMin);
	}

	int damage = attack - defense;
	if (damage < 0)
		damage = 0;

	return damage;
}

int Creature::GetChanceToHit(Creature& target, Item& weapon)
{
	int chance = ((WeaponInfo&)weapon.Info).Accuracy;

	chance -= target.GetDodge();
	chance -= target.GetParry();

	return chance;
}

int Creature::RollAccuracy(Item & weapon)
{
	int accuracy = ((WeaponInfo&)weapon.Info).Accuracy;
	if (accuracy == 0)
		return 0;
	accuracy = rand() % accuracy;
	return accuracy;
}

int Creature::RollEvasion()
{
	int evasion = GetDodge() + GetParry();
	if (evasion == 0)
		return 0;
	evasion = rand() % evasion;
	return evasion;
}

int Creature::GetDodge()
{
	if (Map != nullptr)
	{
		int dodge = 0;
		for (PathfinderLink neighbor : GetTile().Neighbors)
		{
			Tile& tile = (Tile&)neighbor.Target;
			if (tile.Creature == nullptr && tile.IsPassable())
				dodge += Dodge;
		}
		return dodge / 8;
	}

	return Dodge;
}

int Creature::GetParry()
{
	int parry = 0;
	for (Item* item : Hands)
	{
		parry += ((WeaponInfo&)item->Info).Parry;
	}
	return parry;
}

int Creature::GetSlowness()
{
	return 1000000 / Speed;
}

bool Creature::CanMove()
{
	return (Initiative <= 0);
}

bool Creature::IsPlayer()
{
	return Map != nullptr && this == Map->GetPlayer();
}

void Creature::SetHP(short value)
{
	if (value > MaxHP)
		value = MaxHP;

	HP = value;
}

short Creature::GetHP()
{
	return HP;
}

short Creature::GetMaxHP()
{
	return MaxHP;
}

short Creature::GetSpeed()
{
	return Speed;
}

Point Creature::GetPosition()
{
	return Point(X, Y);
}

signed char Creature::GetX()
{
	return X;
}

signed char Creature::GetY()
{
	return Y;
}

char Creature::GetSymbol()
{
	return Info.Symbol;
}

unsigned char Creature::GetColor()
{
	return Info.Color;
}

bool Creature::IsAlliedWith(Creature& creature)
{
	if (this->Faction->GetRelation(*creature.Faction) >= 50)
		return true;

	return false;
}

bool Creature::WillSwapWith(Creature& initiator)
{
	if (initiator.IsPlayer())
		return true;

	return false;
}

bool Creature::IsDying()
{
	if (HP <= 0)
		return true;
	return false;
}
bool Creature::CanSee(Creature & target)
{
	return GetTile().HasVisibility(target.GetTile());
}
bool Creature::CanHear(Creature & target)
{
	if (GetPosition().GetDistance(target.GetPosition()) < 8)
		return true;
	return false;
}
bool Creature::CanDetect(Creature & target)
{
	return CanHear(target) || CanSee(target);
}
#pragma endregion

#pragma region Movement
bool Creature::TryAttack(Creature & other)
{
	if (!IsAlliedWith(other))
	{
		Attack(other);
		return true;
	}
	return false;
}

void Creature::Attack(Creature & other)
{
	for (Item* weapon : Hands)
	{
		Attack(other, *weapon);

		if (other.IsDying())
		{
			Map->KillCreature(&other, "was killed!");
			break;
		}
	}
}

void Creature::Attack(Creature & other, Item & weapon)
{
	Session& session = *Map->Program.GetSession();
	WeaponInfo& weaponInfo = (WeaponInfo&)weapon.Info;
	int accuracy = RollAccuracy(weapon);
	int evasion = other.RollEvasion();
	if (accuracy >= evasion)
	{
		int damage = RollDamageVs(other, weapon);
		other.HP -= damage;

		if (Map->GetPlayer()->CanSee(other))
		{
			session.AddLogEntry(
				Info.GetArticleAndName(true) + " hit " + other.Info.GetArticleAndName()
				+ " with " + weaponInfo.GetArticleAndName() + " for " + to_string(damage) + " damage! "
				+ "(" + to_string(accuracy) + " >= " + to_string(evasion) + ")");
		}
	}
	else
	{
		if (Map->GetPlayer()->CanSee(other))
		{
			session.AddLogEntry(
				Info.GetArticleAndName(true) + " missed " + other.Info.GetArticleAndName()
				+ " with " + weaponInfo.GetArticleAndName() + "! "
				+ "(" + to_string(accuracy) + " < " + to_string(evasion) + ")");
		}
	}
}

bool Creature::MoveTo(Tile& target)
{
	if (!target.IsPassable())
		return false;

	Tile* current = Map->GetTileAt(X, Y);

	if (target.Creature != nullptr)
	{
		if (current == nullptr)
			return false;

		Creature& other = *target.Creature;
		if (TryAttack(other))
			return true;

		if (!other.WillSwapWith(*this))
		{
			return false;
		}
		else
		{
			current->Creature = &other;
			other.X = X;
			other.Y = Y;
		}
	}
	else if (current != nullptr)
	{
		current->Creature = nullptr;
	}

	target.Creature = this;
	X = target.X;
	Y = target.Y;

	return true;
}

bool Creature::MoveTo(int x, int y)
{
	Tile* target = Map->GetTileAt(x, y);
	if (target == nullptr)
		return false;

	return MoveTo(*target);
}

bool Creature::MoveBy(int dx, int dy)
{
	return MoveTo(X + dx, Y + dy);
}

bool Creature::MoveTowards(Tile& target)
{
	Tile & start = *Map->GetTileAt(X, Y);
	return MoveTo((Tile&)*Pathfinder::FindNextHop(start, target));
}

bool Creature::MoveTowards(Creature & target)
{
	Tile & tile = target.GetTile();
	return MoveTowards(tile);
}

bool Creature::MoveTowards(int x, int y)
{
	Tile * target = Map->GetTileAt(x, y);
	if (target == nullptr)
		return false;

	return MoveTowards(*target);
}

bool Creature::MoveRandomly()
{
	int x = rand() % 3 - 1;
	int y = rand() % 3 - 1;
	return MoveBy(x, y);
}
#pragma endregion

Creature* Creature::FindNearestCreature(int range, bool isAlly)
{
	Tile& origin = GetTile();
	Creature* nearest = nullptr;
	int nearestCost = INT_MAX;
	vector<PathfinderNode*> nodes = Pathfinder::FindNodesWithRange(GetTile(), range);
	for (PathfinderNode* node : nodes)
	{
		Tile* tile = (Tile*)node;
		Creature* other = tile->Creature;
		if ( other != this
			&& other != nullptr
			&& !other->IsDying()
			&& (isAlly == IsAlliedWith(*other))
			&& tile->PathCost < nearestCost
			&& (origin.HasVisibility(*tile) || CanHear(*other)))
		{
			nearest = other;
			nearestCost = tile->PathCost;
		}
	}

	return nearest;
}

Tile& Creature::GetTile()
{
	return *Map->GetTileAt(X, Y);
}

#pragma region XML
void Creature::SaveToXml(pugi::xml_node node)
{
	node.append_child("Type").append_child(node_pcdata).set_value(Info.ID.data());
	node.append_child("Faction").append_child(node_pcdata).set_value(Faction->ID.data());

	if (Armor != nullptr)
		node.append_child("Armor").append_child(node_pcdata).set_value(Armor->Info.ID.data());
	else
		node.append_child("Armor").append_child(node_pcdata).set_value("NULL");

	xml_node hands = node.append_child("Hands");
	for (Item* item : Hands)
		hands.append_child("Item").append_child(node_pcdata).set_value(item->Info.ID.data());

	xml_node inventory = node.append_child("Inventory");
	for (Item* item : Inventory)
		inventory.append_child("Item").append_child(node_pcdata).set_value(item->Info.ID.data());

	node.append_child("Initiative").append_child(node_pcdata).set_value(to_string(Initiative).data());
	node.append_child("HP").append_child(node_pcdata).set_value(to_string(HP).data());
	node.append_child("MaxHP").append_child(node_pcdata).set_value(to_string(MaxHP).data());
	node.append_child("Speed").append_child(node_pcdata).set_value(to_string(Speed).data());
	node.append_child("Dodge").append_child(node_pcdata).set_value(to_string(Dodge).data());

	if (Map != nullptr)
		node.append_child("Map").append_child(node_pcdata).set_value(to_string(Map->GetUID()).data());
	else
		node.append_child("Map").append_child(node_pcdata).set_value(to_string(-1).data());
	node.append_child("X").append_child(node_pcdata).set_value(to_string(X).data());
	node.append_child("Y").append_child(node_pcdata).set_value(to_string(Y).data());
}

Creature * Creature::LoadFromXml(pugi::xml_node node, Program& program)
{
	InfoManager& info = program.GetInfo();
	Session& session = *program.GetSession();

	string type = node.child("Type").child_value();
	CreatureInfo& creatureInfo = *info.Creatures[type];
	Creature* creature = new Creature(creatureInfo, nullptr, false);

	creature->Faction = info.Factions[node.child("Faction").child_value()];

	if ((string)node.child("Armor").child_value() != "NULL")
		creature->Armor = new Item(*info.Items[node.child("Armor").child_value()]);
	else
		creature->Armor = nullptr;

	for (xml_node item : node.child("Hands").children())
	{
		creature->Equip(new Item(*info.Items[item.child_value()]));
	}
	for (xml_node item : node.child("Inventory").children())
	{
		creature->Inventory.push_back(new Item(*info.Items[item.child_value()]));
	}

	creature->Initiative = stoi(node.child("Initiative").child_value());
	creature->HP = stoi(node.child("HP").child_value());
	creature->MaxHP = stoi(node.child("MaxHP").child_value());
	creature->Speed = stoi(node.child("Speed").child_value());
	creature->Dodge = stoi(node.child("Dodge").child_value());

	int x = stoi(node.child("X").child_value());
	int y = stoi(node.child("Y").child_value());
	int map = stoi(node.child("Map").child_value());
	if (map > -1)
		session.Maps[map]->AddCreature(creature, x, y);

	return creature;
}
#pragma endregion

