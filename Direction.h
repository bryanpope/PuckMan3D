#pragma once
#include "d3dUtil.h"
class Direction
{
public:

	static enum DIRECTION
	{
		NORTH,
		EAST,
		WEST,
		SOUTH,
		OVER_YONDER //This one is my psuedo-witty way of saying it's looking nowhere, think of it as DEFAULT
	};

	Direction(){}
	~Direction(){}
	static const DIRECTION turnRight(DIRECTION dir);
	static const DIRECTION turnLeft(DIRECTION dir);
	static const XMVECTOR getDirecitonVector(DIRECTION dir);
};

