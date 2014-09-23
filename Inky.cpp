#include "Inky.h"

Inky::Inky(FXMVECTOR pos, FXMVECTOR vel, float radius) : Ghost(pos, vel, radius)
{
}

Inky::~Inky()
{
}

void Inky::Update()
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
		//Offset tile = 2 spaces in PuckMan's facing
			//Draw a vector from Blinky's current location to the offset tile
			//Double the vector length extending forward, this is Inky's target


						/*std::string goalFacing = mPuckMan->getFacing();
						PathNode* offsetTile;
						if(goalFacing == "forward" || goalFacing == "backward")
						{
							offsetTile->row = mPuckMan->getRow() + (2 * goalFacing);
							offsetTile->col = mPuckman->getCol();
						}
						if(goalFacing == "left" || goalFacing == "right")
						{
							offsetTile->getRow() = mPuckMan->getRow();
							offsetTile->getCol() = mPuckman->getCol() + (2 * goalFacing);
						}
		
						//Get distance between Blinky and the offset tile
						XMVECTOR distanceVec = XMVectorSet(offsetTile->getCol() - mCol, offsetTile->getRow() - mRow, 0.0f, 0.0f);
						distanceVec *= 2;
		
						PathNode tempNode = getNode(distanceVec.m128_f32[1], distanceVec.m128_f32[0]);*/
						


		//mStart = new PathNode(mRow, mCol, 0, 0, NULL, mFacing);
		//mGoal = new PathNode(mPuckMan->getRow(), mPuckMan->getCol(), 0, 0, NULL, mPuckMan->getFacing());
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
		//XMVectorSet(-2.0f, 0.75f, 0.0f, 0.0f)
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