#pragma once
#include <cmath>
#include "d3dUtil.h"

class PathNode
{
public:
	struct FCostSort
	{
		bool operator()(const PathNode* first, const PathNode* second) const
		{
			// Return true if first should go before second
			return first->fCost > second->fCost;
		}
	};
	int xPos;
	int zPos;
	int gCost;
	int fCost;
	bool isWalkable;
	std::string facing;

	PathNode* parent;

	PathNode(int x, int z, int g, int f, PathNode* parent, std::string facing);
	PathNode(int x, int z, std::string facing);
	PathNode(int x, int z);
	~PathNode(){}

	PathNode* getParent()
	{
		return parent;
	}

	void setParent(PathNode* p)
	{
		parent = p;
	}

	int getDistance(PathNode* goal, int x, int z);
	int getDistanceFromParent(PathNode child, PathNode* parent);

	PathNode* combineNode(PathNode* initial, PathNode* target);
};