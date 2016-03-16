#pragma once

using namespace std;

class PathfinderNode;
class PathfinderLink
{
public:
	PathfinderNode& Target;
	int Cost;

	PathfinderLink(PathfinderNode& target, int cost);
	virtual ~PathfinderLink();
};

