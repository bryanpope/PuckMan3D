#include "PathNode.h"


PathNode::PathNode(int row, int col, int gCost, int fCost, PathNode* parentNode, std::string facing)
{
	this->row = row;
	this->col = col;
	this->gCost = gCost;
	this->fCost = fCost;
	this->mParent = parentNode;
	this->facing = facing;
}

PathNode::~PathNode()
{
}