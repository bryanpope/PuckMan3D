#include "Direction.h"

const Direction::DIRECTION Direction::turnRight(DIRECTION dir)
{
	if (dir == NORTH)
	{
		return EAST;
	}

	if (dir == EAST)
	{
		return SOUTH;
	}

	if (dir == SOUTH)
	{
		return WEST;
	}

	if (dir == WEST)
	{
		return NORTH;
	}
}

const Direction::DIRECTION Direction::turnLeft(DIRECTION dir)
{
	if (dir == NORTH)
	{
		return WEST;
	}

	if (dir == EAST)
	{
		return NORTH;
	}

	if (dir == SOUTH)
	{
		return EAST;
	}

	if (dir == WEST)
	{
		return SOUTH;
	}
}

const Direction::DIRECTION Direction::turnAround(DIRECTION dir)
{
	if (dir == NORTH)
	{
		return SOUTH;
	}

	if (dir == EAST)
	{
		return WEST;
	}

	if (dir == SOUTH)
	{
		return NORTH;
	}

	if (dir == WEST)
	{
		return EAST;
	}
}

const XMVECTOR Direction::getDirecitonVector(DIRECTION dir)
{
	XMVECTOR vectorDirection = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	switch (dir)
	{
	case NORTH:
		vectorDirection.m128_f32[0] = 0;
		vectorDirection.m128_f32[2] = 1;
		break;
	case EAST:
		vectorDirection.m128_f32[0] = 1;
		vectorDirection.m128_f32[2] = 0;
		break;
	case SOUTH:
		vectorDirection.m128_f32[0] = 0;
		vectorDirection.m128_f32[2] = -1;
		break;
	case WEST:
		vectorDirection.m128_f32[0] = -1;
		vectorDirection.m128_f32[2] = 0;
		break;
	}
	return vectorDirection;
}