#pragma once
#include <vector>

using namespace std;

class PathfinderNode;
class Path;
class Pathfinder
{
public:
	//Pathfinder();
	//~Pathfinder();

	static PathfinderNode* FindNextHop(PathfinderNode &start, PathfinderNode &destination);
	static Path FindPath(PathfinderNode &start, PathfinderNode &destination);
	static vector<PathfinderNode*> FindNodesWithRange(PathfinderNode &start, int range);
	static Path GeneratePath(PathfinderNode &destination);

private:
	static unsigned long CurrentRun;

	//static void Pathfinder::ResetNodes(vector<PathfinderNode*> closed, vector<PathfinderNode*> open);
	//static void Pathfinder::ResetNodes(vector<PathfinderNode*> closed, priority_queue<PathfinderNode*, vector<PathfinderNode*>, PathfinderNodeComparer> open);
};

