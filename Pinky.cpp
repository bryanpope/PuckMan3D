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
		//GetFacing();
		if (facingState == PuckMan::Facing::F_FORWARD)
		{
			mStart = new PathNode(this->mPos.x, this->mPos.z);
			mGoal = new PathNode(round(MazeLoader::GetPacManData().at(0).pos.x + 4), round(MazeLoader::GetPacManData().at(0).pos.z));
			test.FindPath(mStart, mGoal);
			break;
		}

		if (facingState == PuckMan::Facing::F_BACKWARD)
		{
			mStart = new PathNode(this->mPos.x, this->mPos.z);
			mGoal = new PathNode(round(MazeLoader::GetPacManData().at(0).pos.x - 4), round(MazeLoader::GetPacManData().at(0).pos.z));
			test.FindPath(mStart, mGoal);
			break;
		}

		if (facingState == PuckMan::Facing::F_LEFT)
		{
			mStart = new PathNode(this->mPos.x, this->mPos.z);
			mGoal = new PathNode(round(MazeLoader::GetPacManData().at(0).pos.x), round(MazeLoader::GetPacManData().at(0).pos.z - 4));
			test.FindPath(mStart, mGoal);
			break;
		}

		if (facingState == PuckMan::Facing::F_RIGHT)
		{
			mStart = new PathNode(this->mPos.x, this->mPos.z);
			mGoal = new PathNode(round(MazeLoader::GetPacManData().at(0).pos.x), round(MazeLoader::GetPacManData().at(0).pos.z + 4));
			test.FindPath(mStart, mGoal);
			break;
		}

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