#pragma once
#include <list>

using namespace std;

class PathfinderNode;

class Path
{
public:
	list<PathfinderNode*> Nodes;

	Path();
	~Path();

	PathfinderNode *GetNext();
};

