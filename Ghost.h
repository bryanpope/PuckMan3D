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
		IDLE = 4
	};

	enum GHOST_FACING
	{
		NORTH = 1,
		EAST = 2,
		SOUTH = 3,
		WEST = 4
	};

	const float GHOST_SPEED = 100.0f;
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
	bool reachedEnd = false;
	GHOST_FACING mFacing = GHOST_FACING::NORTH;

	Pathfinding path;

	GameTimer mGhostStateTimer;
	GHOST_STATES mGhostStates;

	void UpdateCurrentTweenPoint(float dt);
	void SnapTweenPoint();

	GHOST_FACING GetFacing() const
	{
		return mFacing;
	}

	struct TweenPoint
	{
		int startPosX;
		int startPosZ;
		int endPosX;
		int endPosZ;
		UINT distance;
		XMFLOAT3 vector;
		float startTween;
		float endTween;
		float tweenTime;
	};

	std::vector<PathNode*> mWaypoints;
	int waypointIterator;
	PathNode* mStart;
	PathNode* mGoal;

	std::vector<TweenPoint> mTweenPoints;
	XMFLOAT3 mCurrTweenPoint;
	UINT mCurrTweenIndex;

public:
	Ghost();
	Ghost(FXMVECTOR pos, float radius);
	~Ghost();
	virtual void Update();
	void SetWayPoints(std::vector<PathNode*> wayP);
	void SetSpeed(int levelCounter, GHOST_STATES ghostState);

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