#include "Clyde.h"

Clyde::Clyde(FXMVECTOR pos, FXMVECTOR vel, float radius) : Ghost(pos, vel, radius)
{
	XMStoreFloat3(&mPos, pos);
	XMStoreFloat3(&mVel, vel);
	this->mGhostStates = GHOST_STATES::IDLE;
	this->mScatterTile.x = -13.0f;
	this->mScatterTile.z = -14.5f;
	this->waypointIterator = 0;
	this->mScatterTimer = 0;
	this->mChaseTimer = 0;

	//Draw the path to his scatter area prior to the start of the game to prevent bottlenecks
	mStart = new PathNode((int)this->mPos.x, (int)this->mPos.z);
	mGoal = new PathNode((int)this->mScatterTile.x, (int)this->mScatterTile.z);
	waypoints = test.FindPath(mStart, mGoal);
	scatterPathDrawn = true;
}

Clyde::~Clyde()
{
}

const XMFLOAT3 Clyde::mScatterWaypoints[34] = 
{
	{ XMFLOAT3(-13.0f, 0.0f, -13.5f) },
	{ XMFLOAT3(-12.0f, 0.0f, -13.5f) },
	{ XMFLOAT3(-11.0f, 0.0f, -13.5f) },
	{ XMFLOAT3(-10.0f, 0.0f, -13.5f) },
	{ XMFLOAT3(-9.0f, 0.0f, -13.5f) },
	{ XMFLOAT3(-8.0f, 0.0f, -13.5f) },
	{ XMFLOAT3(-7.0f, 0.0f, -13.5f) },
	{ XMFLOAT3(-6.0f, 0.0f, -13.5f) },
	{ XMFLOAT3(-5.0f, 0.0f, -13.5f) },
	{ XMFLOAT3(-4.0f, 0.0f, -13.5f) },
	{ XMFLOAT3(-3.0f, 0.0f, -13.5f) },
	{ XMFLOAT3(-2.0f, 0.0f, -13.5f) },
	{ XMFLOAT3(-2.0f, 0.0f, -12.5f) },
	{ XMFLOAT3(-2.0f, 0.0f, -11.5f) },
	{ XMFLOAT3(-2.0f, 0.0f, -10.5f) },
	{ XMFLOAT3(-3.0f, 0.0f, -10.5f) },
	{ XMFLOAT3(-4.0f, 0.0f, -10.5f) },
	{ XMFLOAT3(-5.0f, 0.0f, -10.5f) },
	{ XMFLOAT3(-5.0f, 0.0f, -9.5f) },
	{ XMFLOAT3(-5.0f, 0.0f, -8.5f) },
	{ XMFLOAT3(-5.0f, 0.0f, -7.5f) },
	{ XMFLOAT3(-6.0f, 0.0f, -7.5f) },
	{ XMFLOAT3(-7.0f, 0.0f, -7.5f) },
	{ XMFLOAT3(-8.0f, 0.0f, -7.5f) },
	{ XMFLOAT3(-8.0f, 0.0f, -8.5f) },
	{ XMFLOAT3(-8.0f, 0.0f, -9.5f) },
	{ XMFLOAT3(-8.0f, 0.0f, -10.5f) },
	{ XMFLOAT3(-9.0f, 0.0f, -10.5f) },
	{ XMFLOAT3(-10.0f, 0.0f, -10.5f) },
	{ XMFLOAT3(-11.0f, 0.0f, -10.5f) },
	{ XMFLOAT3(-12.0f, 0.0f, -10.5f) },
	{ XMFLOAT3(-13.0f, 0.0f, -10.5f) },
	{ XMFLOAT3(-13.0f, 0.0f, -11.5f) },
	{ XMFLOAT3(-13.0f, 0.0f, -12.5f) }
};

void Clyde::Update(float dt, bool powerUpActivated, PuckMan::Facing facingState, int levelNumber, int pelletCounter)
{
	/*if (powerUpActivated)
	{
		this->mGhostStates = GHOST_STATES::FRIGHTENED;
	}
	else
	{
		this->mGhostStates = prevState;
	}

	//Get clyde's position in row/col space
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
		mGhostStates = GHOST_STATES::SCATTER;
	}*/

	if (pelletCounter == 90)
	{
		this->mGhostStates = GHOST_STATES::SCATTER;
	}

	switch (mGhostStates)
	{
	case SCATTER:
		if (!scatterPathDrawn)
		{
			mStart = new PathNode((int)this->mPos.x, (int)this->mPos.z, "");
			mGoal = new PathNode((int)this->mScatterTile.x, (int)this->mScatterTile.z, "");
			waypoints = test.FindPath(mStart, mGoal);
			scatterPathDrawn = true;
			waypointIterator = 0;
		}
		if (waypoints.size() != 0)
		{
			if (waypointIterator < waypoints.size())
			{
				this->setPos(XMVectorSet(this->waypoints.at(waypointIterator)->xPos, mPos.y, this->waypoints.at(waypointIterator)->zPos, 0.0f));
				waypointIterator++;
			}
			else if (waypointIterator == waypoints.size())
			{
				this->isLooping = true;
				if (isLooping == true)
				{
					this->setPos(XMVectorSet(mScatterWaypoints[this->mCurrWaypointIndex].x, mScatterWaypoints[this->mCurrWaypointIndex].y, mScatterWaypoints[this->mCurrWaypointIndex].z, 0.0f));
					this->mCurrWaypointIndex++;
					if (this->mCurrWaypointIndex == 34) //Temporarily hard coded
					{
						this->mCurrWaypointIndex = 0;
					}
				}
			}
		}

		if (levelNumber == 1)
		{
			XMVECTOR vel = XMLoadFloat3(&mVel);
			vel = vel * 0.75f;
			XMStoreFloat3(&mVel, vel);
			break;
		}
		else if (levelNumber >= 2 || levelNumber <= 4)
		{
			XMVECTOR vel = XMLoadFloat3(&mVel);
			vel = vel * 0.85f;
			XMStoreFloat3(&mVel, vel);
			break;
		}
		else if (levelNumber >= 5)
		{
			XMVECTOR vel = XMLoadFloat3(&mVel);
			vel = vel * 0.95f;
			XMStoreFloat3(&mVel, vel);
			break;
		}
	case CHASE:					
		mStart = new PathNode((int)this->mPos.x, (int)this->mPos.z, "");
		mGoal = new PathNode((int)round(MazeLoader::GetPacManData().at(0).pos.x), (int)round(MazeLoader::GetPacManData().at(0).pos.z), PuckManFacingToString(facingState));
		waypoints = test.FindPath(mStart, mGoal);
		if (waypoints.size() != 0)
		{
			this->setPos(XMVectorSet((float)waypoints.front()->xPos, 0.0f, (float)waypoints.front()->zPos, 0.0f));
		}		
		break;
	case FRIGHTENED:
		if (levelNumber == 1)
		{
			XMVECTOR vel = XMLoadFloat3(&mVel);
			vel = vel * 0.50f;
			XMStoreFloat3(&mVel, vel);
			break;
		}
		else if (levelNumber >= 2 || levelNumber <= 4)
		{
			XMVECTOR vel = XMLoadFloat3(&mVel);
			vel = vel * 0.55f;
			XMStoreFloat3(&mVel, vel);
			break;
		}
		else if (levelNumber >= 5)
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
		if (levelNumber == 1)
		{
			XMVECTOR vel = XMLoadFloat3(&mVel);
			vel = vel * 0.40f;
			XMStoreFloat3(&mVel, vel);
			break;
		}
		else if(levelNumber >= 2 || levelNumber <= 4)
		{
			XMVECTOR vel = XMLoadFloat3(&mVel);
			vel = vel * 0.45f;
			XMStoreFloat3(&mVel, vel);
			break;
		}
		else if(levelNumber >= 5)
		{
			XMVECTOR vel = XMLoadFloat3(&mVel);
			vel = vel * 0.50f;
			XMStoreFloat3(&mVel, vel);
			break;
		}*/
	}
}

void Clyde::Reset()
{
	this->mGhostStates = GHOST_STATES::IDLE;
	waypointIterator = 0;
	mCurrWaypointIndex = 0;
	scatterPathDrawn = false;
	isLooping = false;
}