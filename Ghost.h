#pragma once
#include "d3dUtil.h"
#include "PathNode.h"
#include "Pathfinding.h"
#include "GameTimer.h"
#include "Player.h"

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

	enum GHOST_FACING
	{
		NORTH = 1,
		EAST = 2,
		SOUTH = 3,
		WEST = 4
	};

	XMFLOAT3 mPos;
	XMFLOAT3 mVel; //Originally set as XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f)
	XMFLOAT3 mScatterTile;
	float mSpeed;
	float mRadius; //Originally set as 0.75f
	const float Z_OFFSET = 0.5f;
	int mLevelNumber;
	std::string mFacing;

	Pathfinding test;

	std::list<PathNode*> waypoints;
	PathNode* mStart;
	PathNode* mGoal;

	GameTimer mGhostStateTimer;
	GHOST_STATES mGhostStates;

	void MoveGhost(PathNode* target, float dt);
	void MoveToPuckMan();

public:
	Ghost();
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
		MazeLoader::SetGhostPos(pos, 0);
	}
};