#include "Blinky.h"

Blinky::Blinky(FXMVECTOR pos, FXMVECTOR vel, float radius) : Ghost(pos, vel, radius)
{
	this->mGhostStates = GHOST_STATES::DEAD;
	this->mScatterTile.x = 12.0f;
	this->mScatterTile.z = 14.5f;
}

Blinky::~Blinky()
{
}

void Blinky::Update(float dt, bool powerUpActivated, PuckMan::Facing facingState)
{
	/*if (powerUpActivated)
	{
		this->mGhostStates = GHOST_STATES::FRIGHTENED;
	}
	else
	{
		this->mGhostStates = GHOST_STATES::CHASE;
	}*/

	switch (mGhostStates)
	{
	case SCATTER:
		mStart = new PathNode((int)this->mPos.x, (int)this->mPos.z, "");
		mGoal = new PathNode((int)this->mScatterTile.x, (int)this->mScatterTile.z, "");
		waypoints = test.FindPath(mStart, mGoal);
		if (waypoints.size() != 0)
		{
			this->setPos(XMVectorSet((float)waypoints.front()->xPos, mPos.y, (float)waypoints.front()->zPos, 0.0f));
		}
		
		if(mLevelNumber == 1)
		{
			XMVECTOR vel = XMLoadFloat3(&mVel);
			vel = vel * 0.75f;
			XMStoreFloat3(&mVel, vel);// hello from Gumby.
									  // Hey babe
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
		mStart = new PathNode((int)this->mPos.x, (int)this->mPos.z, "");
		mGoal = new PathNode((int)round(MazeLoader::GetPacManData().at(0).pos.x), (int)round(MazeLoader::GetPacManData().at(0).pos.z), FacingToString(facingState));

		waypoints = test.FindPath(mStart, mGoal);

		if (waypoints.size() != 0)
		{
			this->setPos(XMVectorSet((float)waypoints.front()->xPos, mPos.y, (float)waypoints.front()->zPos, 0.0f));
		}
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