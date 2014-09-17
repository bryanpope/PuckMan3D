#include "PathNode.h"


PathNode::PathNode(UINT row, UINT col, UINT gCost, UINT fCost, PathNode* parentNode, std::string facing)
{
	this->row = row;
	this->col = col;
	this->gCost = gCost;
	this->fCost = fCost;
	this->mParent = parentNode;
	this->facing = facing;
}

PathNode::PathNode(UINT row, UINT col)
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

PathNode PathNode::combineNode(PathNode initial, PathNode* target)
{
	initial.col = target->col;
	initial.row = target->row;
	initial.gCost = target->gCost;
	initial.fCost = target->fCost;
	initial.facing = target->facing;

	return initial;
}

void PathNode::calculateCosts(PathNode* goal)
{
	gCost = getGCost(mParent);
	hCost = 1;//getHCost(goal);
	fCost = gCost + hCost;
}