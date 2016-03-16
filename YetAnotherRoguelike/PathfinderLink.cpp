#include "PathfinderLink.h"

PathfinderLink::PathfinderLink(PathfinderNode& target, int cost) : Target(target)
{
	Cost = cost;
}

PathfinderLink::~PathfinderLink()
{
}
