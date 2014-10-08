#include "PathNode.h"

PathNode::PathNode(float x, float z, int g, int fC, PathNode* p, Direction::DIRECTION f)
{
	xPos = x;
	zPos = z;
	gCost = g;
	fCost = fC;
	parent = p;
	facing = f;
}

PathNode::PathNode(float x, float z, Direction::DIRECTION f)
{
	xPos = x;
	zPos = z;
	gCost = 0;
	fCost = 0;
	parent = NULL;
	facing = f;
}

PathNode::PathNode(float x, float z)
{
	xPos = x;
	zPos = z;
	gCost = 0;
	fCost = 0;
	parent = NULL;
	facing = Direction::DIRECTION::NORTH;
}


//initial is the node you are feeding the data INTO
//target is the node you are taking the data FROM
PathNode* PathNode::combineNode(PathNode* initial, PathNode* target)
{
	initial->xPos = target->xPos;
	initial->zPos = target->zPos;
	initial->gCost = target->gCost;
	initial->fCost = target->fCost;
	initial->facing = target->facing;

	return initial;
}

int PathNode::getDistance(PathNode* goal, int x, int z)
{
	return abs(x - goal->xPos) + abs(z - goal->zPos);
	//return sqrt((goal->zPos - z) * (goal->zPos - z) + (goal->xPos - x) * (goal->xPos - x));
}

int PathNode::getDistanceFromParent(PathNode child, PathNode* parent)
{
	auto directionLeft = Direction::turnLeft(parent->facing);
	auto directionRight = Direction::turnRight(parent->facing);
	auto behind = Direction::turnAround(parent->facing);

	if (child.facing != parent->facing)
	{
		if (child.facing == directionLeft || child.facing == directionRight)
		{
			return 1;//abs(child.xPos - parent->xPos) + abs(child.zPos - parent->zPos);
		}
		else if (child.facing == behind)
		{
			return 1;
		}
	}
	return 500;
	//return sqrt((child.zPos - parent->zPos) * (child.zPos - parent->zPos) + (child.xPos - parent->xPos) * (child.xPos - parent->xPos));*/
}