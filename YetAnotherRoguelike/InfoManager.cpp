#include "InfoManager.h"
#include "curses.h"
#include "WeaponInfo.h"
#include "ArmorInfo.h"
#include "FactionInfo.h"
#include "CreatureInfo.h"
#include "BehaviorInfo.h"
#include "ItemUseEffect.h"
#include "TileInfo.h"
#include "RoomInfo.h"
#include "ItemSetInfo.h"
#include "pugixml.hpp"

using namespace pugi;

InfoManager::InfoManager()
{
	LoadFactions();
	LoadBehaviors();
	LoadItemEffects();
	LoadItems();
	LoadItemSets();
	LoadCreatures();
	LoadTiles();
}

InfoManager::~InfoManager()
{
	for (pair<string, CreatureInfo*> info : Creatures)
	{
		delete info.second;
	}
	for (pair<string, BehaviorInfo*> info : Behaviors)
	{
		delete info.second;
	}
	for (pair<string, ItemUseEffect*> info : ItemEffects)
	{
		delete info.second;
	}
	for (pair<string, ItemInfo*> info : Items)
	{
		delete info.second;
	}
	for (pair<string, ItemSetInfo*> info : ItemSets)
	{
		delete info.second;
	}
	for (pair<string, FactionInfo*> info : Factions)
	{
		delete info.second;
	}
	for (pair<string, TileInfo*> info : Tiles)
	{
		delete info.second;
	}
	for (pair<string, RoomInfo*> info : Rooms)
	{
		delete info.second;
	}
}

void InfoManager::LoadFactions()
{
	FactionInfo* faction;

	faction = new FactionInfo();
	faction->ID = "Player";
	faction->Name = "Player";
	Factions.insert({ faction->ID, faction });

	faction = new FactionInfo();
	faction->ID = "Civilized";
	faction->Name = "Civilized";
	Factions.insert({ faction->ID, faction });

	faction = new FactionInfo();
	faction->ID = "Undead";
	faction->Name = "Undead";
	Factions.insert({ faction->ID, faction });

	faction = new FactionInfo();
	faction->ID = "Dragon";
	faction->Name = "Dragon";
	Factions.insert({ faction->ID, faction });

	Factions["Player"]->SetMutualRelation(*Factions["Civilized"], 100);
}

void InfoManager::LoadBehaviors()
{
	BehaviorInfo* behavior;

	behavior = new CivilizedBehavior();
	behavior->ID = "Civilized";
	((CivilizedBehavior*)behavior)->EnemySearchRange = 150;
	((CivilizedBehavior*)behavior)->AllySearchRange = 150;
	Behaviors.insert({ behavior->ID, behavior });

	behavior = new MonsterBehavior();
	behavior->ID = "Monster";
	((MonsterBehavior*)behavior)->EnemySearchRange = 150;
	Behaviors.insert({ behavior->ID, behavior });
}

void InfoManager::LoadItemEffects()
{
	ItemUseEffect* effect;

	effect = new HealingItemUseEffect();
	effect->ID = "SmallHeal";
	((HealingItemUseEffect*)effect)->Amount = 20;
	ItemEffects.insert({ effect->ID, effect });
}

void InfoManager::LoadItems()
{
	xml_document doc;
	doc.load_file("XML/Items.xml");
	for (xml_node node : doc.child("Items").child("Weapons").children())
	{
		WeaponInfo* weapon = new WeaponInfo();
		weapon->ID = node.child("ID").child_value();
		weapon->Name = node.child("Name").child_value();
		weapon->Article = node.child("Article").child_value();
		weapon->Symbol = node.child("Symbol").child_value()[0];
		weapon->Color = stoi(node.child("Color").child_value());
		weapon->DamageMin = stoi(node.child("DamageMin").child_value());
		weapon->DamageMax = stoi(node.child("DamageMax").child_value());
		weapon->Accuracy = stoi(node.child("Accuracy").child_value());
		weapon->Parry = stoi(node.child("Parry").child_value());
		weapon->IsDroppable = node.child("Droppable").child_value()[0] == '1';
		Items.insert({ weapon->ID, weapon });
	}
	for (xml_node node : doc.child("Items").child("Armors").children())
	{
		ArmorInfo* armor = new ArmorInfo();
		armor->ID = node.child("ID").child_value();
		armor->Name = node.child("Name").child_value();
		armor->Article = node.child("Article").child_value();
		armor->Symbol = node.child("Symbol").child_value()[0];
		armor->Color = stoi(node.child("Color").child_value());
		armor->DefenseMin = stoi(node.child("DefenseMin").child_value());
		armor->DefenseMax = stoi(node.child("DefenseMax").child_value());
		armor->IsDroppable = node.child("Droppable").child_value()[0] == '1';
		Items.insert({ armor->ID, armor });
	}
	for (xml_node node : doc.child("Items").child("Consumables").children())
	{
		ConsumableInfo* consumable = new ConsumableInfo();
		consumable->ID = node.child("ID").child_value();
		consumable->Name = node.child("Name").child_value();
		consumable->Article = node.child("Article").child_value();
		consumable->Symbol = node.child("Symbol").child_value()[0];
		consumable->Color = stoi(node.child("Color").child_value());
		for (xml_node effect : node.child("Effects").children())
		{
			consumable->Effects.push_back(ItemEffects[effect.child_value()]);
		}
		consumable->IsDroppable = node.child("Droppable").child_value()[0] == '1';
		Items.insert({ consumable->ID, consumable });
	}
}

void InfoManager::LoadItemSets()
{
	xml_document doc;
	doc.load_file("XML/ItemSets.xml");
	for (xml_node node : doc.child("ItemSets").children())
	{
		ItemSetInfo* itemset = new ItemSetInfo();
		itemset->ID = node.child("ID").child_value();
		ItemSets.insert({ itemset->ID, itemset });
	}

	// Because ItemSets can reference other ItemSets, they must be created and initialized separately.
	for (xml_node node : doc.child("ItemSets").children())
	{
		ItemSetInfo* itemset = ItemSets[node.child("ID").child_value()];
		for (xml_node itemNode : node.children("Item"))
		{
			ItemSetItem* item = new ItemSetItem();
			item->Chance = stoi(itemNode.child("Chance").child_value());
			item->Item = Items[itemNode.child("ID").child_value()];
			itemset->Subsets.push_back(item);
		}

		for (xml_node subsetNode : node.children("Subset"))
		{
			ItemSetSubset* subset = new ItemSetSubset();
			subset->Chance = stoi(subsetNode.child("Chance").child_value());
			subset->Subset = ItemSets[subsetNode.child("ID").child_value()];
			itemset->Subsets.push_back(subset);
		}

		for (xml_node exclusiveNode : node.children("Exclusive"))
		{
			ItemSetExclusive* exclusive = new ItemSetExclusive();
			exclusive->Chance = stoi(exclusiveNode.child("Chance").child_value());

			for (xml_node itemNode : exclusiveNode.children("Item"))
			{
				ItemSetItem* item = new ItemSetItem();
				item->Item = Items[itemNode.child("ID").child_value()];
				exclusive->Subsets.Add(item, stod(itemNode.child("Weight").child_value()));
			}

			for (xml_node subsetNode : exclusiveNode.children("Subset"))
			{
				ItemSetSubset* subset = new ItemSetSubset();
				subset->Subset = ItemSets[subsetNode.child("ID").child_value()];
				exclusive->Subsets.Add(subset, stod(subsetNode.child("Weight").child_value()));
			}
		}
	}
}

void InfoManager::LoadCreatures()
{
	xml_document doc;
	doc.load_file("XML/Creatures.xml");
	for (xml_node node : doc.child("Creatures").children())
	{
		ArmorInfo* armor = (ArmorInfo*)Items[node.child("DefaultArmor").child_value()];
		FactionInfo* faction = Factions[node.child("Faction").child_value()];
		BehaviorInfo* behavior = Behaviors[node.child("Behavior").child_value()];
		ItemSetInfo* itemset = ItemSets[node.child("ItemSet").child_value()];
		CreatureInfo *creature = new CreatureInfo(*faction, *behavior, *itemset);
		creature->ID = node.child("ID").child_value();
		creature->Name = node.child("Name").child_value();
		creature->Article = node.child("Article").child_value();
		creature->Symbol = node.child("Symbol").child_value()[0];
		for (xml_node weapon : node.child("NaturalWeapons").children())
		{
			creature->NaturalWeapons.push_back((WeaponInfo*)Items[weapon.child_value()]);
		}
		creature->MaxHP = stoi(node.child("MaxHP").child_value());
		creature->SpeedMin = stoi(node.child("SpeedMin").child_value());
		creature->SpeedMax = stoi(node.child("SpeedMax").child_value());
		creature->DodgeMin = stoi(node.child("DodgeMin").child_value());
		creature->DodgeMax = stoi(node.child("DodgeMax").child_value());
		creature->Color = stoi(node.child("Color").child_value());
		
		Creatures.insert({ creature->ID, creature });
	}
}

void InfoManager::LoadTiles()
{
	xml_document doc;
	doc.load_file("XML/Tiles.xml");
	for (xml_node node : doc.child("Tiles"))
	{
		TileInfo* tile = new TileInfo();
		tile->ID = node.child("ID").child_value();
		tile->Symbol = node.child("Symbol").child_value()[0];
		tile->Color = stoi(node.child("Color").child_value());
		tile->Passable = node.child("Passable").child_value()[0] == '1';
		tile->Transparent = node.child("Transparent").child_value()[0] == '1';
		Tiles.insert({ tile->ID, tile });
	}
}

void InfoManager::LoadRooms()
{
	xml_document doc;
	doc.load_file("XML/Rooms.xml");
	for (xml_node node : doc.child("Rooms"))
	{
		RoomInfo* room = new RoomInfo();
		room->ID = node.child("ID").child_value();
		room->Weight = stod(node.child("Weight").child_value());

		unordered_map<char,TileInfo*> definitions = unordered_map<char,TileInfo*>();
		for (xml_node definition : node.child("Definitions").children("Tile"))
		{
			char c = definition.attribute("Symbol").value()[0];
			TileInfo* info = Tiles[definition.attribute("ID").value()];
			definitions.insert({ c, info });
		}

		unordered_map<char, WeightedList<TileInfo*>> randoms = unordered_map<char, WeightedList<TileInfo*>>();
		for (xml_node random : node.child("Definitions").children("Random"))
		{
			WeightedList<TileInfo*> variants = WeightedList<TileInfo*>();
			for (xml_node variant : random.children())
			{
				TileInfo* info = definitions[variant.attribute("Symbol").value()[0]];
				variants.Add(info, variant.attribute("Weight").as_double());
			}
			randoms.insert({ random.attribute("Symbol").value()[0], variants });
		}

		int x = 0, y = 0;
		for (xml_node row : node.child("Rows").children())
		{
			int n = strlen(row.child_value());
			for (int i = 0; i < n; i++)
			{
				char c = row.child_value()[i];
				unordered_map<char, TileInfo*>::const_iterator definition = definitions.find(c);
				if (definition == definitions.end())
				{
					room->AddTile(*definition->second, x, y);
				}
				else
				{
					unordered_map<char, WeightedList<TileInfo*>>::const_iterator random = randoms.find(c);
					if (random == randoms.end())
					{
						
					}
				}

				x++;
			}
			y++;
		}

		Rooms.insert({ room->ID, room });
	}
}

