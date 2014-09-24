#include "Blinky.h"

Blinky::Blinky(FXMVECTOR pos, FXMVECTOR vel, float radius) : Ghost(pos, vel, radius)
{
}

Blinky::~Blinky()
{
}

void Blinky::Update()
{
	PathNode* start = new PathNode(this->mPos.x, this->mPos.z);
	PathNode* goal = new PathNode(5, 10);
	std::list<PathNode*> testVec = test.FindPath(start, goal);
	if (testVec.size() != 0)
	{
		//std::cout << "Blinky pos " << mPos.x << ", " << mPos.y << ", " << mPos.z << std::endl;
		this->setPos(XMVectorSet(testVec.front()->xPos, mPos.y, testVec.front()->zPos, 0.0f));
	}
	
	if (testVec.size() == 0)
	{
		//std::cout << "testVec is empty!" << std::endl;
	}
	switch (mGhostStates)
	{
	case SCATTER:
		if(mLevelNumber == 1)
		{
			XMVECTOR vel = XMLoadFloat3(&mVel);
			vel = vel * 0.75f;
			XMStoreFloat3(&mVel, vel);
			break;
		}
		else if(mLevelNumber >= 2 || mLevelNumber <= 4)
		{
			XMVECTOR vel = XMLoadFloat3(&mVel);
			vel = vel * 0.85f;
			XMStoreFloat3(&mVel, vel);
			break;
		}
		else if(mLevelNumber >= 5)
		{
			XMVECTOR vel = XMLoadFloat3(&mVel);
			vel = vel * 0.95f;
			XMStoreFloat3(&mVel, vel);
			break;
		}
	case CHASE:
		//A simple call to pathfinding will suffice
		//Pathfinding::FindPath(this->mCol, this->mRow, 15, 30);
		
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
		//XMVectorSet(0.0f, 0.75f, 3.5f, 0.0f)
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