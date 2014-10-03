#pragma once
#include "MazeLoader.h"
#include "Direction.h"

class PuckMan
{
protected:

	const float PUCKMAN_SPEED = 1000.0f;
	XMFLOAT3 mVel;
	XMFLOAT3 mPos;
	float mSpeed;
	float mRadius;

public:
	PuckMan(void)
	{}

	PuckMan(FXMVECTOR pos, FXMVECTOR vel, float radius)
	{
		XMStoreFloat3(&mPos, pos);
		XMStoreFloat3(&mVel, vel);
		mRadius = radius;
	}

	~PuckMan(void);

	void Move(float dt, float mSpeed, Direction::DIRECTION direction);
	float CalculateSpeed(int levelNumber, bool powerUpActivated);

	FXMVECTOR GetPos() const
	{
		return XMLoadFloat3(&mPos);
	}

	void SetPos(FXMVECTOR pos)
	{
		XMStoreFloat3(&mPos, pos);
	}

	FXMVECTOR GetVelocity() const
	{
		return XMLoadFloat3(&mVel);
	}

	Direction::DIRECTION GetFacing() const
	{
		return mFacing;
	}

protected:
	Direction::DIRECTION mFacing = Direction::DIRECTION::WEST;
};