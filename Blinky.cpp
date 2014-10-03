#include "Blinky.h"

Blinky::Blinky(FXMVECTOR pos, FXMVECTOR vel, float radius) : Ghost(pos, vel, radius)
{
	XMStoreFloat3(&mPos, pos);
	XMStoreFloat3(&mVel, vel);
	this->mGhostStates = GHOST_STATES::SCATTER;
	this->mScatterTile.x = 12.0f;
	this->mScatterTile.z = 14.5f;
	this->waypointIterator = 0;
	this->mScatterTimer = 0.0f;
	this->mChaseTimer = 0.0f;

	//Draw the path to his scatter area prior to the start of the game to prevent bottlenecks
	mStart = new PathNode((int)this->mPos.x, (int)this->mPos.z);
	mGoal = new PathNode((int)this->mScatterTile.x, (int)this->mScatterTile.z);
	waypoints = test.FindPath(mStart, mGoal);
	scatterPathDrawn = true;
}

Blinky::~Blinky()
{
}

const XMFLOAT3 Blinky::mScatterWaypoints[18] =
{
	{ XMFLOAT3(12.0f, 0.0f, 13.5f) },
	{ XMFLOAT3(12.0f, 0.0f, 12.5f) },
	{ XMFLOAT3(12.0f, 0.0f, 11.5f) },
	{ XMFLOAT3(12.0f, 0.0f, 10.5f) },
	{ XMFLOAT3(11.0f, 0.0f, 10.5f) },
	{ XMFLOAT3(10.0f, 0.0f, 10.5f) },
	{ XMFLOAT3(9.0f, 0.0f, 10.5f) },
	{ XMFLOAT3(8.0f, 0.0f, 10.5f) },
	{ XMFLOAT3(7.0f, 0.0f, 10.5f) },
	{ XMFLOAT3(7.0f, 0.0f, 11.5f) },
	{ XMFLOAT3(7.0f, 0.0f, 12.5f) },
	{ XMFLOAT3(7.0f, 0.0f, 13.5f) },
	{ XMFLOAT3(7.0f, 0.0f, 14.5f) },
	{ XMFLOAT3(8.0f, 0.0f, 14.5f) },
	{ XMFLOAT3(9.0f, 0.0f, 14.5f) },
	{ XMFLOAT3(10.0f, 0.0f, 14.5f) },
	{ XMFLOAT3(11.0f, 0.0f, 14.5f) },
	{ XMFLOAT3(12.0f, 0.0f, 14.5f) }
};

const std::string Blinky::mScatterFacing[18] =
{
	{ "backward" },
	{ "backward" },
	{ "backward" },
	{ "backward" },
	{ "left" },
	{ "left" },
	{ "left" },
	{ "left" },
	{ "left" },
	{ "up" },
	{ "up" },
	{ "up" },
	{ "up" },
	{ "right" },
	{ "right" },
	{ "right" },
	{ "right" },
	{ "right" }
};

void Blinky::Update(float dt, bool powerUpActivated, PuckMan::Facing facingState, int levelNumber)
{
	std::cout << "Scatter timer: " << mScatterTimer << ", Chase Timer: " << mChaseTimer << std::endl;
	switch (mGhostStates)
	{
	case SCATTER:
		if (!scatterPathDrawn)
		{
			mStart = new PathNode((int)this->mPos.x, (int)this->mPos.z);// , GhostFacingToString(this->mFacing));
			mGoal = new PathNode((int)this->mScatterTile.x, (int)this->mScatterTile.z);//, "");
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
				//std::cout << waypointIterator << std::endl;
			}
			else if (waypointIterator == waypoints.size())
			{
				this->isLooping = true;
				if (isLooping == true)
				{
					this->setPos(XMVectorSet(mScatterWaypoints[this->mCurrWaypointIndex].x, mScatterWaypoints[this->mCurrWaypointIndex].y, mScatterWaypoints[this->mCurrWaypointIndex].z, 0.0f));
					this->mCurrWaypointIndex++;
					if (this->mCurrWaypointIndex == 18)
					{
						this->mCurrWaypointIndex = 0;
					}
				}
			}
		}
		/*mScatterTimer += 5.7142 * dt; //dt currently takes (without mutliplying) 40 seconds to reach 7.0f, 5.7142 comes from 40 / 7 to get the number as accurate as possible.
		if (mScatterTimer >= 7.0f)
		{
			this->mGhostStates = GHOST_STATES::CHASE;
			mScatterTimer = 0.0f;
			scatterPathDrawn = false;
			this->mCurrWaypointIndex = 0;
			this->waypointIterator = 0;
		}*/


/*if (levelNumber == 1)
{

	/*XMVECTOR vel = XMLoadFloat3(&mVel);
	vel = vel * 0.75f;
	XMStoreFloat3(&mVel, vel);// hello from Gumby.
	break;					  // Hey babe
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
}*/
			
		break;
	case CHASE:
		if (!firstChasePathDrawn)
		{
			mStart = new PathNode((int)this->mPos.x, (int)this->mPos.z);
			mGoal = new PathNode((int)round(MazeLoader::GetPacManData().at(0).pos.x), (int)round(MazeLoader::GetPacManData().at(0).pos.z));
			waypoints = test.FindPath(mStart, mGoal);
			waypointIterator = 0;
		}
		else
		{
			int row = MazeLoader::GetMazeHeight() - (int)round(this->mPos.x + 15.5f);
			int col = (int)round(this->mPos.z + 14.5f) - 1;
			if (MazeLoader::IsDivergent(row, col))
			{
				mStart = new PathNode((int)this->mPos.x, (int)this->mPos.z);
				mGoal = new PathNode((int)round(MazeLoader::GetPacManData().at(0).pos.x), (int)round(MazeLoader::GetPacManData().at(0).pos.z));
				waypoints = test.FindPath(mStart, mGoal);
				waypointIterator = 0;
			}
		}
		if (waypoints.size() != 0)
		{
			if (waypointIterator < waypoints.size())
			{
				//this->setPos(XMVectorSet((float)waypoints.front()->xPos, mPos.y, (float)waypoints.front()->zPos, 0.0f));
				this->setPos(XMVectorSet(this->waypoints.at(waypointIterator)->xPos, mPos.y, this->waypoints.at(waypointIterator)->zPos, 0.0f));
				waypointIterator++;
				std::cout << waypointIterator++ << std::endl;
			}
			else if (waypointIterator >= waypoints.size())
			{
				waypointIterator = 0;
			}
		}

		this->mChaseTimer += 5.7142 * dt;
		if (mChaseTimer >= 20.0f)
		{
			this->mGhostStates = GHOST_STATES::SCATTER;
			mChaseTimer = 0.0f;
			waypointIterator = 0;
			firstChasePathDrawn = false;
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
		//XMVectorSet(0.0f, 0.75f, 3.5f, 0.0f)
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

void Blinky::Reset()
{
	this->mGhostStates = GHOST_STATES::SCATTER;
	waypointIterator = 0;
	mCurrWaypointIndex = 0;
	mChaseTimer = 0.0f;
	mScatterTimer = 0.0f;
	firstChasePathDrawn = false;
	scatterPathDrawn = false;
	isLooping = false;
}