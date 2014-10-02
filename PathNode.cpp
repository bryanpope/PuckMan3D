#include "PathNode.h"

PathNode::PathNode(int x, int z, int g, int fC, PathNode* p, std::string f)
{
	xPos = x;
	zPos = z;
	gCost = g;
	fCost = fC;
	parent = p;
	facing = f;
}

PathNode::PathNode(int x, int z, std::string f)
{
	xPos = x;
	zPos = z;
	gCost = 0;
	fCost = 0;
	parent = NULL;
	facing = f;
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
	if (child.facing != parent->facing)
	{
		if (parent->facing == "left")
		{
			if (child.facing == "forward" || child.facing == "backward")
			{
				return abs(child.xPos - parent->xPos) + abs(child.zPos - parent->zPos);
			}
			else if (child.facing == "right")
			{
				return 200;
			}
		}

		if (parent->facing == "right")
		{
			if (child.facing == "forward" || child.facing == "backward")
			{
				return abs(child.xPos - parent->xPos) + abs(child.zPos - parent->zPos);
			}
			else if (child.facing == "left")
			{
				return 200;
			}
		}

		if (parent->facing == "forward")
		{
			if (child.facing == "left" || child.facing == "right")
			{
				return abs(child.xPos - parent->xPos) + abs(child.zPos - parent->zPos);
			}
			else if (child.facing == "backward")
			{
				return 200;
			}
		}
		if (parent->facing == "backward")
		{
			if (child.facing == "left" || child.facing == "right")
			{
				return abs(child.xPos - parent->xPos) + abs(child.zPos - parent->zPos);
			}
			else if (child.facing == "forward")
			{
				return 200;
			}
		}
		
	}
	//return sqrt((child.zPos - parent->zPos) * (child.zPos - parent->zPos) + (child.xPos - parent->xPos) * (child.xPos - parent->xPos));
}