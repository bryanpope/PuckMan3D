#include "Pinky.h"

Pinky::Pinky(FXMVECTOR pos, FXMVECTOR vel, float radius) : Ghost(pos, vel, radius)
{
	this->mGhostStates = GHOST_STATES::SCATTER;
	this->mScatterTile.x = -13.0f;
	this->mScatterTile.z = -14.5f;
}

Pinky::~Pinky()
{
}

void Pinky::Update(float dt, bool powerUpActivated, PuckMan::Facing facingState)
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
		mStart = new PathNode((int)this->mPos.x, (int)this->mPos.z);
		mGoal = new PathNode((int)this->mScatterTile.x, (int)this->mScatterTile.z);
		waypoints = test.FindPath(mStart, mGoal);

		if (waypoints.size() != 0)
		{
			this->setPos(XMVectorSet((float)waypoints.front()->xPos, mPos.y, (float)waypoints.front()->zPos, 0.0f));
		}

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
		if (facingState == PuckMan::Facing::F_FORWARD)
		{
			mStart = new PathNode((int)this->mPos.x, (int)this->mPos.z);
			mGoal = new PathNode((int)round(MazeLoader::GetPacManData().at(0).pos.x), (int)round(MazeLoader::GetPacManData().at(0).pos.z + 4.0f));
			waypoints = test.FindPath(mStart, mGoal);

			if (waypoints.size() != 0)
			{
				this->setPos(XMVectorSet((float)waypoints.front()->xPos, mPos.y, (float)waypoints.front()->zPos, 0.0f));
			}

			break;
		}

		if (facingState == PuckMan::Facing::F_BACKWARD)
		{
			mStart = new PathNode((int)this->mPos.x, (int)this->mPos.z);
			mGoal = new PathNode((int)round(MazeLoader::GetPacManData().at(0).pos.x), (int)round(MazeLoader::GetPacManData().at(0).pos.z - 4.0f));
			waypoints = test.FindPath(mStart, mGoal);

			if (waypoints.size() != 0)
			{
				this->setPos(XMVectorSet((float)waypoints.front()->xPos, mPos.y, (float)waypoints.front()->zPos, 0.0f));
			}

			break;
		}

		if (facingState == PuckMan::Facing::F_LEFT)
		{
			mStart = new PathNode((int)this->mPos.x, (int)this->mPos.z);
			mGoal = new PathNode((int)round(MazeLoader::GetPacManData().at(0).pos.x - 4.0f), (int)round(MazeLoader::GetPacManData().at(0).pos.z));
			waypoints = test.FindPath(mStart, mGoal);

			if (waypoints.size() != 0)
			{
				this->setPos(XMVectorSet((float)waypoints.front()->xPos, mPos.y, (float)waypoints.front()->zPos, 0.0f));
			}

			break;
		}

		if (facingState == PuckMan::Facing::F_RIGHT)
		{
			mStart = new PathNode((int)this->mPos.x, (int)this->mPos.z);
			mGoal = new PathNode((int)round(MazeLoader::GetPacManData().at(0).pos.x + 4.0f), (int)round(MazeLoader::GetPacManData().at(0).pos.z));
			waypoints = test.FindPath(mStart, mGoal);

			if (waypoints.size() != 0)
			{
				this->setPos(XMVectorSet((float)waypoints.front()->xPos, mPos.y, (float)waypoints.front()->zPos, 0.0f));
			}

			break;
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