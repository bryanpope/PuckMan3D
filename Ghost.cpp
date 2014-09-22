#include "Ghost.h"

Ghost::Ghost()
{
}

Ghost::Ghost(UINT row, UINT col, FXMVECTOR vel, float radius)
{
	mRow = row;
	mCol = col;
	XMStoreFloat3(&mVel, vel);
	mRadius = radius;
}

Ghost::~Ghost()
{
}

void Ghost::Update()
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
		//Target 4 tiles in front of PuckMan's facing
		//std::string goalFacing = mPuckMan->getFacing();
		/*if (goalFacing == "forward" || goalFacing == "backward")
		{
			//mStart = new PathNode(mRow, mCol, 0, 0, NULL, mFacing);
			//mGoal = new PathNode(mPuckMan->getRow() + (4 * goalFacing), mPuckMan->getCol(), 0, 0, NULL, mPuckMan->getFacing());
		}*/

		/*if (goalFacing == "left" || goalFacing == "right")
		{
			//mStart = new PathNode(mRow, mCol, 0, 0, NULL, mFacing);
			//mGoal = new PathNode(mPuckMan->getRow(), mPuckMan->getCol() + (4 * goalFacing), 0, 0, NULL, mPuckMan->getFacing());
		}*/

		//mPath = FindPath(mStart, mGoal);
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
		//XMVectorSet(0.0f, 0.75f, 0.0f, 0.0f)
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