#pragma once
#include <string>

using namespace std;

class Program;
class MenuItem
{
public:
	string Label;

	MenuItem(string label);
	virtual ~MenuItem();

	virtual void Select() = 0;
};

