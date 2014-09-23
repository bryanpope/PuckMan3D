#pragma once
#include <cmath>
#include "d3dUtil.h"

class PathNode
{
public:
	int xPos;
	int zPos;
	int gCost;
	//int hCost;
	int fCost;
	bool isWalkable;
	bool isOpen; //If true, the node is Open and therefore in the openList already. If false, the node is Closed and therefore in the closedList
	std::string facing;
	PathNode* parent;

	PathNode(int x, int z, int gCost, int fCost, PathNode* parent, std::string facing);
	PathNode(int x, int z);
	~PathNode(){}

	PathNode* getParent()
	{
		return parent;
	}

	int getGCost(PathNode* n)
	{
		return n->gCost + ((xPos == n->xPos || zPos == n->zPos) ? 10 : 14);
	}

	/*int getHCost(PathNode* n)
	{
		return (abs(n->xPos - xPos) + abs(n->zPos - zPos)) * 10;
	}*/

	int getZ()
	{
		return zPos;
	}

	int getX()
	{
		return xPos;
	}

	void setParent(PathNode* p)
	{
		parent = p;
	}

	//void calculateCosts(PathNode* goal);
	int getDistance(PathNode* goal, int x, int z);
	int getDistanceFromParent(PathNode child, PathNode* goal);

	PathNode* combineNode(PathNode* initial, PathNode* target);

	//May not need these overloads, currently work in progress
	bool operator==(const PathNode& rhs)
	{
		if (zPos == rhs.zPos && xPos == rhs.xPos)// && gCost == rhs.gCost && fCost == rhs.fCost && facing == rhs.facing)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool operator!=(const PathNode& rhs)
	{
		if (zPos != rhs.zPos && xPos != rhs.xPos)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void operator=(const PathNode& rhs)
	{
		xPos = rhs.xPos;
		zPos = rhs.zPos;
		gCost = rhs.gCost;
		fCost = rhs.fCost;
		facing = rhs.facing;
		isWalkable = rhs.isWalkable;
		isOpen = rhs.isOpen;
	}
};