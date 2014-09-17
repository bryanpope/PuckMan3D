#pragma once
#include <cmath>
#include <iostream>
#include "d3dUtil.h"

class PathNode
{
public:
	UINT row;
	UINT col;
	UINT gCost;
	UINT hCost;
	UINT fCost;
	bool isWalkable;
	bool isOpen;
	bool isClosed;
	std::string facing;
	PathNode* mParent;

	PathNode(UINT row, UINT col, UINT gCost, UINT fCost, PathNode* parentNode, std::string facing);
	PathNode(UINT row, UINT col);
	~PathNode();

	PathNode* getParent()
	{
		return mParent;
	}

	UINT getGCost(PathNode* n)
	{
		return n->gCost + ((col == n->col || row == n->row) ? 10 : 14);
	}

	UINT getHCost(PathNode* n)
	{
		return abs((int)n->col - (int)col) + abs((int)n->row - (int)row);
	}

	UINT getRow()
	{
		return row;
	}

	UINT getCol()
	{
		return col;
	}

	void setParent(PathNode* parent)
	{
		mParent = parent;
	}

	void calculateCosts(PathNode* goal);

	PathNode combineNode(PathNode initial, PathNode* target);

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