#pragma once
#include <vector>
#include "PathfinderLink.h"
#include "IComparable.h"

using namespace std;

enum PathfinderNodeStatus : unsigned char
{
	Unvisited,
	Open,
	Closed
};

class PathfinderLink;
class PathfinderNode
	: public IComparable
{
public:
	vector<PathfinderLink> Neighbors = vector<PathfinderLink>();
	unsigned long LastVisit;
	PathfinderNode* Parent;
	PathfinderNodeStatus Status = Unvisited;
	int PathCost;
	int Heuristic;

	PathfinderNode();
	virtual ~PathfinderNode();

	virtual int CalculateHeuristic(PathfinderNode& destination) = 0;
	virtual int GetValue() override;
};