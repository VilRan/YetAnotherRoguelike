#include "Session.h"
#include "Map.h"
#include "Program.h"
#include "pugixml.hpp"
#include "Creature.h"
#include "VectorUtility.h"

using namespace pugi;

Session::Session(::Program & program) : Program(program)
{
}

Session::~Session()
{
	for (Creature* creature : Creatures)
	{
		delete creature;
	}
	for (Map* map : Maps)
	{
		delete map;
	}
}

void Session::Initialize()
{
	Player = new Creature(*Program.GetInfo().Creatures["Player"], nullptr, true);
	Creatures.push_back(Player);
	ActiveMap = new ::Map(Program, 99, 99, rand());
	Maps.push_back(ActiveMap);
}

void Session::Initialize(string file)
{
	InfoManager& info = Program.GetInfo();

	xml_document doc;
	//file += ".save";
	doc.load_file(file.data());
	xml_node save = doc.child("SaveData");

	Name = save.child("Name").child_value();

	xml_node log = save.child("Log");
	for (xml_node entry : log.children())
		Log.push_back(entry.first_child().value());

	for (xml_node map : save.child("Maps"))
	{
		Maps.push_back(Map::LoadFromXml(map, Program));
	}
	ActiveMap = Maps.front();

	for (xml_node creature : save.child("Creatures"))
	{
		Creatures.push_back(Creature::LoadFromXml(creature, Program));
	}

	Player = Creature::LoadFromXml(save.child("Player"), Program);
	Creatures.push_back(Player);
}

void Session::DeleteCreature(Creature * creature)
{
	if (creature == Player)
		Player = nullptr;

	VectorUtility::Remove(Creatures, creature);
	delete creature;
}

void Session::AddLogEntry(string string)
{
	string.insert(0, GetTimeString() + " - ");
	Log.push_front(string);
}

void Session::SaveToFile(string name)
{
	xml_document doc;
	xml_node save = doc.append_child("SaveData");

	save.append_child("Name").append_child(node_pcdata).set_value(Name.data());

	xml_node player = save.append_child("Player");
	Player->SaveToXml(player);

	xml_node creatures = save.append_child("Creatures");
	for (Creature* creature : Creatures)
	{
		if (creature != Player)
			creature->SaveToXml(creatures.append_child("Creature"));
	}

	xml_node maps = save.append_child("Maps");
	for (Map* map : Maps)
	{
		map->SaveToXml(maps.append_child("Map"));
	}

	xml_node log = save.append_child("Log");
	for (string entry : Log)
	{
		log.append_child("Entry").append_child(node_pcdata).set_value(entry.data());
	}

	name += ".save";
	doc.save_file(name.data());
}

string Session::GetDateString()
{
	string s = "";

	int days = 1 + Time / 1000 / 60 / 60 / 24;
	s += to_string(days);

	return s;
}

string Session::GetTimeString()
{
	string s = "";
	int hours = Time / 1000 / 60 / 60 % 24;
	if (hours < 10)
		s += "0";
	s += to_string(hours) + ":";

	int minutes = Time / 1000 / 60 % 60;
	if (minutes < 10)
		s += "0";
	s += to_string(minutes) + ":";

	int seconds = Time / 1000 % 60;
	if (seconds < 10)
		s += "0";
	s += to_string(seconds);

	return s;
}
