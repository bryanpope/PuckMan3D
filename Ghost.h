#pragma once
#include "d3dUtil.h"
#include "PathNode.h"
#include "Pathfinding.h"
#include "GameTimer.h"
#include "Player.h"
#include <array>

class Ghost
{
protected:
	enum GHOST_STATES
	{
		SCATTER = 1,
		CHASE = 2,
		FRIGHTENED = 3,
		DEAD = 4,
		IDLE = 5
		//IN_TUNNEL = 6 //Maybe make this a state?
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
	float mScatterTimer;
	float mChaseTimer;
	int mCurrWaypointIndex = 0;
	bool isLooping = false; //This is to determine whether or not the ghost is in their scatter loop, false = not looping
	bool scatterPathDrawn = false; //This is to determine whether or not the path has been drawn for the scatter path to prevent multiple calculations
	bool firstChasePathDrawn = false; //This is to determine whether or not the INITIAL path has been drawn for the chase state
	bool isIdle = true;
	GHOST_FACING mFacing = GHOST_FACING::NORTH;

	Pathfinding test;

	std::vector<PathNode*> waypoints;
	int waypointIterator;
	PathNode* mStart;
	PathNode* mGoal;

	GameTimer mGhostStateTimer;
	GHOST_STATES mGhostStates;

	void MoveGhost(PathNode* target, float dt);
	void MoveToPuckMan();

	GHOST_FACING GetFacing() const
	{
		return mFacing;
	}

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