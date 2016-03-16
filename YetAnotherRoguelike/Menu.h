#pragma once
#include <vector>

using namespace std;

class MenuItem;
class Menu
{
public:

	Menu();
	~Menu();

	void AddItem(MenuItem* item);
	MenuItem* GetItem(int index);
	void TriggerSelected();
	void IncrementSelection();
	void DecrementSelection();
	int GetSelection();
	int GetCount();
	void Input(char key, bool isCommand);
	void Draw(int x, int y, int width, int height);

private:
	vector<MenuItem*> Items;
	int Selection = 0;
};

