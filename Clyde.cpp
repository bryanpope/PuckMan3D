#include "Clyde.h"

Clyde::Clyde(FXMVECTOR pos, FXMVECTOR vel, float radius) : Ghost(pos, vel, radius)
{
	this->mGhostStates = GHOST_STATES::DEAD;
}

Clyde::~Clyde()
{
}

void Clyde::Update(float dt, bool powerUpActivated)
{
	if (powerUpActivated)
	{
		this->mGhostStates = GHOST_STATES::FRIGHTENED;
	}

	/*//Get clyde's position in row/col space
	int clydePos = ((MazeLoader::GetMazeHeight()) - (int)round(this->getPos().z + 15.5f)) + 
					((MazeLoader::GetMazeWidth()) - (int)round(this->getPos().x + 15.5f));
	//Get puckman's position in row/col space
	int puckmanPos = ((MazeLoader::GetMazeHeight()) - (int)round(MazeLoader::GetPacManData().at(0).pos.x + 15.5f)) + 
					((MazeLoader::GetMazeWidth()) - (int)round(MazeLoader::GetPacManData().at(0).pos.z + 15.5f));
	//Determine the euclidean distance between clyde and puckman
	int euclidDistance = abs(clydePos - puckmanPos);
	//If the distance between Clyde and PuckMan is 8 tiles or more in Euclidean space, target PuckMan and chase like Blinky does
	if (euclidDistance >= 8)
	{
		mGhostStates = GHOST_STATES::CHASE;
	}
	//If the distance between Clyde and Puckman is less than 8 tiles, go back to Scatter mode
	else
	{
		mGhostStates = GHOST_STATES::DEAD;
	}*/

	switch (mGhostStates)
	{
	case SCATTER:
		//Head to 30, 2 (row, col)
		mStart = new PathNode((int)this->mPos.x, (int)this->mPos.z);
		mGoal = new PathNode((int)this->mScatterTile.x, (int)this->mScatterTile.z);
		waypoints = test.FindPath(mStart, mGoal);
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
		mStart = new PathNode((int)this->mPos.x, (int)this->mPos.z);
		mGoal = new PathNode((int)round(MazeLoader::GetPacManData().at(0).pos.x), (int)round(MazeLoader::GetPacManData().at(0).pos.z));
		waypoints = test.FindPath(mStart, mGoal);
		if (waypoints.size() != 0)
		{
			this->setPos(XMVectorSet((float)waypoints.front()->xPos, 0.0f, (float)waypoints.front()->zPos, 0.0f));
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