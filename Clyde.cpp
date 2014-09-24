#include "Clyde.h"

Clyde::Clyde(FXMVECTOR pos, FXMVECTOR vel, float radius) : Ghost(pos, vel, radius)
{}

Clyde::~Clyde()
{
}

void Clyde::Update()
{
	switch (mGhostStates)
	{
	case SCATTER:
		if (mLevelNumber == 1)
		{
			XMVECTOR vel = XMLoadFloat3(&mVel);
			vel = vel * 0.75f;
			XMStoreFloat3(&mVel, vel);
			break;
		}
		else if (mLevelNumber >= 2 || mLevelNumber <= 4)
		{
			XMVECTOR vel = XMLoadFloat3(&mVel);
			vel = vel * 0.85f;
			XMStoreFloat3(&mVel, vel);
			break;
		}
		else if (mLevelNumber >= 5)
		{
			XMVECTOR vel = XMLoadFloat3(&mVel);
			vel = vel * 0.95f;
			XMStoreFloat3(&mVel, vel);
			break;
		}
	case CHASE:
		//If the distance between Clyde and PuckMan is 8 tiles or more in Euclidean space, target PuckMan and chase like Blinky does
			//If the distance between Clyde and Puckman is less than 8 tiles, go back to Scatter mode
		//mStart = new PathNode(mRow, mCol, 0, 0, NULL, mFacing);
		//mGoal = new PathNode(mPuckMan->getRow(), mPuckMan->getCol(), 0, 0, NULL, mPuckMan->getFacing());
		//mPath = FindPath(mStart, mGoal);
		/*if (mPath.size() >= 8)
		{
			//Chase
		}
		else
		{
			//Scatter
		}*/
		break;
	case FRIGHTENED:
		if (mLevelNumber == 1)
		{
			XMVECTOR vel = XMLoadFloat3(&mVel);
			vel = vel * 0.50f;
			XMStoreFloat3(&mVel, vel);
			break;
		}
		else if (mLevelNumber >= 2 || mLevelNumber <= 4)
		{
			XMVECTOR vel = XMLoadFloat3(&mVel);
			vel = vel * 0.55f;
			XMStoreFloat3(&mVel, vel);
			break;
		}
		else if (mLevelNumber >= 5)
		{
			XMVECTOR vel = XMLoadFloat3(&mVel);
			vel = vel * 0.60f;
			XMStoreFloat3(&mVel, vel);
			break;
		}
	case DEAD:
		//Spawn in box
		//XMVectorSet(2.0f, 0.75f, 0.0f, 0.0f)
		break;
	/*case IN_TUNNEL:
		if (mLevelNumber == 1)
		{
			XMVECTOR vel = XMLoadFloat3(&mVel);
			vel = vel * 0.40f;
			XMStoreFloat3(&mVel, vel);
			break;
		}
		else if(mLevelNumber >= 2 || mLevelNumber <= 4)
		{
			XMVECTOR vel = XMLoadFloat3(&mVel);
			vel = vel * 0.45f;
			XMStoreFloat3(&mVel, vel);
			break;
		}
		else if(mLevelNumber >= 5)
		{
			XMVECTOR vel = XMLoadFloat3(&mVel);
			vel = vel * 0.50f;
			XMStoreFloat3(&mVel, vel);
			break;
		}*/
	}
}