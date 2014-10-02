#pragma once
#include "MazeLoader.h"

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

	enum Facing
	{
		F_FORWARD,
		F_BACKWARD,
		F_LEFT,
		F_RIGHT
	};

	void Move(float dt, float mSpeed, std::string direction);
	float CalculateSpeed(int levelNumber, bool powerUpActivated);

	FXMVECTOR GetPos() const
	{
		return XMLoadFloat3(&mPos);
	}

	void SetPos(FXMVECTOR pos)
	{
		XMStoreFloat3(&mPos, pos);
	}

	Facing GetFacing() const
	{
		return mFacing;
	}

	FXMVECTOR GetVelocity() const
	{
		return XMLoadFloat3(&mVel);
	}

protected:
	Facing mFacing = F_LEFT;
};