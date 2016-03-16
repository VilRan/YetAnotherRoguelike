#include "Pathfinder.h"
#include <algorithm>
#include "PathfinderNodeComparer.h"
#include "PathfinderNode.h"
#include "Path.h"
#include "BinaryHeap.h"

unsigned long Pathfinder::CurrentRun = 0;
/*
Pathfinder::Pathfinder()
{
}

Pathfinder::~Pathfinder()
{
}
*/

PathfinderNode *Pathfinder::FindNextHop(PathfinderNode &start, PathfinderNode &destination)
{
	return FindPath(start, destination).GetNext();
}

Path Pathfinder::FindPath(PathfinderNode &start, PathfinderNode &destination)
{
	if (&start == &destination)
		return Path();

	CurrentRun++;

	BinaryHeap open = BinaryHeap();
	//priority_queue<PathfinderNode*, vector<PathfinderNode*>, PathfinderNodeComparer> open
	//	= priority_queue<PathfinderNode*, vector<PathfinderNode*>, PathfinderNodeComparer>();
	start.LastVisit = CurrentRun;
	start.PathCost = 0;
	start.Parent = nullptr;
	open.Add(start);

	while (open.GetCount() > 0)
	{
		//sort(open.begin(), open.end(), PathfinderNodeComparer());
		//PathfinderNode *active = open.back(); open.pop_back();
		PathfinderNode* active = (PathfinderNode*)&open.Remove();
		active->Status = Closed;

		for (PathfinderLink link : active->Neighbors)
		{
			PathfinderNode& neighbor = link.Target;

			if (neighbor.LastVisit != CurrentRun) // Reset nodes that haven't been visited yet this run.
			{
				neighbor.Status = Unvisited;
				neighbor.LastVisit = CurrentRun;
			}

			if (&neighbor == &destination)
			{
				destination.Parent = active;
				return GeneratePath(destination);
			}

			if (neighbor.Status != Closed)
			{
				int cost = active->PathCost + link.Cost;

				if (!neighbor.Status == Open)
				{
					neighbor.Parent = active;
					neighbor.PathCost = cost;
					neighbor.Heuristic = neighbor.CalculateHeuristic(destination);
					neighbor.Status = Open;
					open.Add(neighbor);
				}
				else if (cost < neighbor.PathCost)
				{
					neighbor.Parent = active;
					neighbor.PathCost = cost;
				}
			}
		}
	}

	return Path();
}

vector<PathfinderNode*> Pathfinder::FindNodesWithRange(PathfinderNode &start, int range)
{
	CurrentRun++;

	vector<PathfinderNode*> closed = vector<PathfinderNode*>();
	//vector<PathfinderNode*> open = vector<PathfinderNode*>();
	BinaryHeap open = BinaryHeap();
	start.LastVisit = CurrentRun;
	start.PathCost = 0;
	start.Parent = nullptr;
	//open.push_back(&start);
	open.Add(start);

	while (open.GetCount() > 0)
	{
		//sort(open.begin(), open.end(), PathfinderNodeComparer());
		//PathfinderNode *active = open.back(); open.pop_back();
		PathfinderNode* active = (PathfinderNode*)&open.Remove();
		active->Status = Closed;
		closed.push_back(active);

		for (PathfinderLink link : active->Neighbors)
		{
			PathfinderNode &neighbor = link.Target;

			if (neighbor.LastVisit != CurrentRun) // Reset nodes that haven't been visited yet this run.
			{
				neighbor.Status = Unvisited;
				neighbor.LastVisit = CurrentRun;
			}

			if (neighbor.Status != Closed)
			{
				int cost = active->PathCost + link.Cost;

				if (cost <= range)
				{
					if (!neighbor.Status == Open)
					{
						neighbor.Parent = active;
						neighbor.PathCost = cost;
						neighbor.Status = Open;
						open.Add(neighbor);
						//open.push_back(&neighbor);
					}
					else if (cost < neighbor.PathCost)
					{
						neighbor.Parent = active;
						neighbor.PathCost = cost;
					}
				}
			}
		}
	}

	return closed;
}

Path Pathfinder::GeneratePath(PathfinderNode &destination)
{
	Path path = Path();
	PathfinderNode *active = &destination;
	while (active->Parent != nullptr)
	{
		path.Nodes.push_front(active);
		active = active->Parent;
	}
	return path;
}