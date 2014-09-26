#include "Blinky.h"

Blinky::Blinky(FXMVECTOR pos, FXMVECTOR vel, float radius) : Ghost(pos, vel, radius)
{
}

Blinky::~Blinky()
{
}

void Blinky::Update(float dt)
{
	PathNode* start = new PathNode(this->mPos.x, this->mPos.z);
	PathNode* goal = new PathNode(10, 10);
	std::list<PathNode*> waypoints = test.FindPath(start, goal);

	if (waypoints.size() != 0)
	{
	
		//PathNode* currWaypoint = waypoints.front();
		//this->setPos(XMVectorSet(waypoints.front()->xPos, mPos.y, waypoints.front()->zPos, 0.0f));
		this->MoveGhost(waypoints.front());
		/*XMVECTOR toWaypoint = XMVectorSet(currWaypoint->xPos - mPos.x, mPos.y, currWaypoint->zPos - mPos.z, 0.0f);
		float distance = sqrt((toWaypoint.m128_f32[0] * toWaypoint.m128_f32[0]) + (toWaypoint.m128_f32[2] * toWaypoint.m128_f32[2]));

		if (distance <= mSpeed + 0.01)
		{
			currWaypoint = waypoints.front() + 1;
		}
		
		//currWaypoint = waypoints.front();
		toWaypoint = XMVectorSet(currWaypoint->xPos - mPos.x, mPos.y, currWaypoint->zPos - mPos.z, 0.0f);*/
		
		std::cout << "Blinky pos " << mPos.x << ", " << mPos.y << ", " << mPos.z << std::endl;

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