#include "Pinky.h"

Pinky::Pinky(FXMVECTOR pos, FXMVECTOR vel, float radius) : Ghost(pos, vel, radius)
{
	this->mGhostStates = GHOST_STATES::DEAD;
}

Pinky::~Pinky()
{
}

void Pinky::Update(float dt, PuckMan::Facing facingState)
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
		if (facingState == PuckMan::Facing::F_FORWARD)
		{
			mStart = new PathNode(this->mPos.x, this->mPos.z);
			mGoal = new PathNode(round(MazeLoader::GetPacManData().at(0).pos.x), round(MazeLoader::GetPacManData().at(0).pos.z + 4));
			waypoints = test.FindPath(mStart, mGoal);

			if (waypoints.size() != 0)
			{
				this->setPos(XMVectorSet(waypoints.front()->xPos, mPos.y, waypoints.front()->zPos, 0.0f));
			}

			break;
		}

		if (facingState == PuckMan::Facing::F_BACKWARD)
		{
			mStart = new PathNode(this->mPos.x, this->mPos.z);
			mGoal = new PathNode(round(MazeLoader::GetPacManData().at(0).pos.x), round(MazeLoader::GetPacManData().at(0).pos.z - 4));
			waypoints = test.FindPath(mStart, mGoal);

			if (waypoints.size() != 0)
			{
				this->setPos(XMVectorSet(waypoints.front()->xPos, mPos.y, waypoints.front()->zPos, 0.0f));
			}

			break;
		}

		if (facingState == PuckMan::Facing::F_LEFT)
		{
			mStart = new PathNode(this->mPos.x, this->mPos.z);
			mGoal = new PathNode(round(MazeLoader::GetPacManData().at(0).pos.x - 4), round(MazeLoader::GetPacManData().at(0).pos.z));
			waypoints = test.FindPath(mStart, mGoal);

			if (waypoints.size() != 0)
			{
				this->setPos(XMVectorSet(waypoints.front()->xPos, mPos.y, waypoints.front()->zPos, 0.0f));
			}

			break;
		}

		if (facingState == PuckMan::Facing::F_RIGHT)
		{
			mStart = new PathNode(this->mPos.x, this->mPos.z);
			mGoal = new PathNode(round(MazeLoader::GetPacManData().at(0).pos.x + 4), round(MazeLoader::GetPacManData().at(0).pos.z));
			waypoints = test.FindPath(mStart, mGoal);

			if (waypoints.size() != 0)
			{
				this->setPos(XMVectorSet(waypoints.front()->xPos, mPos.y, waypoints.front()->zPos, 0.0f));
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