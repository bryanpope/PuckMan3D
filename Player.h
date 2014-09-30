#pragma once
//#include "Character.h"
#include "MazeLoader.h"

class PuckMan// :	public Character
{
protected:

	const float PUCKMAN_SPEED = 1000.0f;
	XMFLOAT3 mVel;
	XMFLOAT3 mPos;
	float mSpeed;

public:
	PuckMan(void)
	{}

	/*PuckMan(FXMVECTOR pos, FXMVECTOR look, FXMVECTOR up, BasicModel& model,
		float speed = 0.0f, float sprintSpeed = 0.0f, float health = 0.0f)
		: Character(pos, look, up, model, speed, sprintSpeed, health)
	{
		//mFacing = Facing::F_DEFAULT;
	}*/

	PuckMan(FXMVECTOR pos)
	{
		XMStoreFloat3(&mPos, pos);
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

