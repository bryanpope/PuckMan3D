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

PathNode::PathNode(int row, int col)
{
	this->row = row;
	this->col = col;
	gCost = 0;
	fCost = 0;
	mParent = NULL;
	facing = "random";
}

PathNode::~PathNode()
{
}