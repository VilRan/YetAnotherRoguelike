#pragma once

using namespace std;

class PathfinderNode;


class PathfinderNodeComparer
{
public:
	PathfinderNodeComparer();
	~PathfinderNodeComparer();

	bool operator() (PathfinderNode* a, PathfinderNode* b);
};

