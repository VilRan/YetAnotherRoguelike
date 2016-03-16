#include "PathfinderNodeComparer.h"
#include "PathfinderNode.h"

PathfinderNodeComparer::PathfinderNodeComparer()
{
}

PathfinderNodeComparer::~PathfinderNodeComparer()
{
}

bool PathfinderNodeComparer::operator() (PathfinderNode* a, PathfinderNode* b)
{
	int valA = a->PathCost + a->Heuristic;
	int valB = b->PathCost + b->Heuristic;

	if (valA > valB)
		return true;
	else
		return false;
}