#include "PathfinderNode.h"


PathfinderNode::PathfinderNode()
{
}

PathfinderNode::~PathfinderNode()
{
}

int PathfinderNode::GetValue()
{
	return PathCost + Heuristic;
}
