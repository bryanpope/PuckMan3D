#pragma once
#include "d3dUtil.h"
#include "PathNode.h"
#include "Pathfinding.h"
#include "GameTimer.h"

class Ghost
{
protected:
	enum GHOST_STATES
	{
		SCATTER = 1,
		CHASE = 2,
		FRIGHTENED = 3,
		DEAD = 4
		//IN_TUNNEL = 5 //Maybe make this a state?
	};

	/*UINT mRow; //Originally set as XMVectorSet(0.0f, 0.75f, 3.5f, 0.0f)
	UINT mCol;*/
	XMFLOAT3 mPos;
	XMFLOAT3 mVel; //Originally set as XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f)
	float mRadius; //Originally set as 0.75f
	int mLevelNumber;
	std::string mFacing;

	Pathfinding test;

	GameTimer mGhostStateTimer;
	GHOST_STATES mGhostStates;

public:
	Ghost();
	//Ghost(UINT row, UINT col, FXMVECTOR vel, float radius);
	Ghost(FXMVECTOR pos, FXMVECTOR vel, float radius);
	~Ghost();
	virtual void Update();

	XMFLOAT3 getPos()
	{
		return mPos;
	}
	
	void setPos(FXMVECTOR pos)
	{
		XMStoreFloat3(&mPos, pos);
	}

	/*virtual UINT getCol()
	{
		return this->mCol;
	}

	virtual UINT getRow()
	{
		return this->mRow;
	}*/
};