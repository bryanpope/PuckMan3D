#pragma once
#include <cmath>
#include "d3dUtil.h"

class PathNode
{
public:
	int mRow;
	int mCol;
	int mGCost;
	int mHCost;
	int mFCost;
	bool isWalkable;
	bool isOpen;
	bool isClosed;
	std::string mFacing;
	PathNode* mParent;

	PathNode(int row, int col, int gCost, int fCost, PathNode* parent, std::string facing);
	~PathNode(){}

	PathNode* getParent()
	{
		return mParent;
	}

	int getGCost(PathNode* n)
	{
		return n->mGCost + ((mCol == n->mCol || mRow == n->mRow) ? 10 : 14);
	}

	int getHCost(PathNode* n)
	{
		return (abs(n->mCol - mCol) + abs(n->mRow - mRow)) * 10;
	}

	int getRow()
	{
		return mRow;
	}

	int getCol()
	{
		return mCol;
	}

	void setParent(PathNode* parent)
	{
		mParent = parent;
	}

	void calculateCosts(PathNode* goal)
	{
		mGCost = getGCost(mParent);
		mHCost = getHCost(goal);
		mFCost = mGCost + mHCost;
	}

	PathNode combineNode(PathNode initial, PathNode* target)
	{
		initial.mCol = target->mCol;
		initial.mRow = target->mRow;
		initial.mGCost = target->mGCost;
		initial.mFCost = target->mFCost;
		initial.mFacing = target->mFacing;

		return initial;
	}

	//May not need these overloads, currently work in progress
	bool operator==(const PathNode& rhs)
	{
		if (mRow == rhs.mRow && mCol == rhs.mCol && mGCost == rhs.mGCost && mFCost == rhs.mFCost && mFacing == rhs.mFacing)
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
		if (mRow != rhs.mRow && mCol != rhs.mCol)
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
		mRow = rhs.mRow;
		mCol = rhs.mCol;
		mGCost = rhs.mGCost;
		mFCost = rhs.mFCost;
		mFacing = rhs.mFacing;
		isWalkable = rhs.isWalkable;
		isOpen = rhs.isOpen;
		isClosed = rhs.isOpen;
	}
};