#include "PathNode.h"

PathNode::PathNode(int row, int col, int gCost, int fCost, PathNode* parent, std::string facing)
{
	mRow = row;
	mCol = col;
}