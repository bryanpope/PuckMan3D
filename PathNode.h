#pragma once
#include <cmath>
#include "string.h"

class PathNode
{
public:
	int row;
	int col;
	int gCost;
	int hCost;
	int fCost;
	bool isWalkable;
	bool isOpen;
	bool isClosed;
	std::string facing;
	PathNode* mParent;

	PathNode();
	~PathNode();

	PathNode* getParent()
	{
		return mParent;
	}

	int getGCost(PathNode* n)
	{
		return n->gCost + ((col == n->col || row == n->row) ? 10 : 14);
	}

	int getHCost(PathNode* n)
	{
		return (abs(n->col - col) + abs(n->row - row)) * 10;
	}

	int getRow()
	{
		return row;
	}

	int getCol()
	{
		return col;
	}

	void setParent(PathNode* parent)
	{
		mParent = parent;
	}

	void calculateCosts(PathNode* goal)
	{
		gCost = getGCost(mParent);
		hCost = getHCost(goal);
		fCost = gCost + hCost;
	}

	PathNode combineNode(PathNode initial, PathNode* target)
	{
		initial.col = target->col;
		initial.row = target->row;
		initial.gCost = target->gCost;
		initial.fCost = target->fCost;
		initial.facing = target->facing;

		return initial;
	}

	//May not need these overloads, currently work in progress
	bool operator==(const PathNode& rhs)
	{
		if (row == rhs.row && col == rhs.col && gCost == rhs.gCost && fCost == rhs.fCost && facing == rhs.facing)
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
		if (row != rhs.row && col != rhs.col)
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
		row = rhs.row;
		col = rhs.col;
		gCost = rhs.gCost;
		fCost = rhs.fCost;
		facing = rhs.facing;
		isWalkable = rhs.isWalkable;
		isOpen = rhs.isOpen;
		isClosed = rhs.isOpen;
	}
};