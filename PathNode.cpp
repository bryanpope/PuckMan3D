#include "PathNode.h"

PathNode::PathNode(int row, int col, int gCost, int fCost, PathNode* parent, std::string facing)
{
	mRow = row;
	mCol = col;
	mGCost = gCost;
	mFCost = fCost;
	mParent = parent;
	mFacing = facing;
}

PathNode::PathNode(int row, int col)
{
	mRow = row;
	mCol = col;
	mGCost = 0;
	mFCost = 0;
	mParent = NULL;
	mFacing = "";
}


//initial is the node you are feeding the data INTO
//target is the node you are taking the data FROM
PathNode* PathNode::combineNode(PathNode* initial, PathNode* target)
{
	initial->mCol = target->mCol;
	initial->mRow = target->mRow;
	initial->mGCost = target->mGCost;
	initial->mFCost = target->mFCost;
	initial->mFacing = target->mFacing;

	return initial;
}

void PathNode::calculateCosts(PathNode* goal)
{
	mGCost = getGCost(mParent);
	mHCost = getHCost(goal);
	mFCost = mGCost + mHCost;
}