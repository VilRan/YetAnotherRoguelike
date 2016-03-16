#include "Path.h"

Path::Path()
{
}

Path::~Path()
{
}

PathfinderNode *Path::GetNext()
{
	if (Nodes.size() > 0)
	{
		PathfinderNode *next = Nodes.front();
		Nodes.pop_front();
		return next;
	}
	else
		return nullptr;
}