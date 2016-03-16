#pragma once
#include <list>
#include <vector>
#include <string>

using namespace std;

class Program;
class Map;
class Creature;
class Session
{
public:
	string Name = "";
	string TemporaryLog = "";
	list<string> Log;
	vector<Map*> Maps;
	vector<Creature*> Creatures;
	Map* ActiveMap;
	Creature* Player;
	unsigned long Time = 0;
	unsigned int MapCounter = 0;
	//unsigned int CreatureCounter = 0;

	Session(Program& program);
	~Session();

	void Initialize();
	void Initialize(string file);
	void DeleteCreature(Creature* creature);
	void AddLogEntry(string string);
	void SaveToFile(string name);

	string GetDateString();
	string GetTimeString();

private:
	Program& Program;
};

