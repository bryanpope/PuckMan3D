#include "Inky.h"

Inky::Inky(FXMVECTOR pos, FXMVECTOR vel, float radius) : Ghost(pos, vel, radius)
{
	XMStoreFloat3(&mPos, pos);
	XMStoreFloat3(&mVel, vel);
	this->mGhostStates = GHOST_STATES::IDLE;
	this->mScatterTile.x = 12;
	this->mScatterTile.z = -14.5f;
	waypointIterator = 0;

	//Draw the path to his scatter area prior to the start of the game to prevent bottlenecks
	mStart = new PathNode((int)this->mPos.x, (int)this->mPos.z);
	mGoal = new PathNode((int)this->mScatterTile.x, (int)this->mScatterTile.z);
	waypoints = test.FindPath(mStart, mGoal);
	scatterPathDrawn = true;
}

Inky::~Inky()
{
}

const XMFLOAT3 Inky::mScatterWaypoints[34] = 
{
	{ XMFLOAT3(12.0f, 0.0f, -13.5f) },
	{ XMFLOAT3(11.0f, 0.0f, -13.5f) },
	{ XMFLOAT3(10.0f, 0.0f, -13.5f) },
	{ XMFLOAT3(9.0f, 0.0f, -13.5f) },
	{ XMFLOAT3(8.0f, 0.0f, -13.5f) },
	{ XMFLOAT3(7.0f, 0.0f, -13.5f) },
	{ XMFLOAT3(6.0f, 0.0f, -13.5f) },
	{ XMFLOAT3(5.0f, 0.0f, -13.5f) },
	{ XMFLOAT3(4.0f, 0.0f, -13.5f) },
	{ XMFLOAT3(3.0f, 0.0f, -13.5f) },
	{ XMFLOAT3(2.0f, 0.0f, -13.5f) },
	{ XMFLOAT3(1.0f, 0.0f, -13.5f) },
	{ XMFLOAT3(1.0f, 0.0f, -12.5f) },
	{ XMFLOAT3(1.0f, 0.0f, -11.5f) },
	{ XMFLOAT3(1.0f, 0.0f, -10.5f) },
	{ XMFLOAT3(2.0f, 0.0f, -10.5f) },
	{ XMFLOAT3(3.0f, 0.0f, -10.5f) },
	{ XMFLOAT3(4.0f, 0.0f, -10.5f) },
	{ XMFLOAT3(4.0f, 0.0f, -9.5f) },
	{ XMFLOAT3(4.0f, 0.0f, -8.5f) },
	{ XMFLOAT3(4.0f, 0.0f, -7.5f) },
	{ XMFLOAT3(5.0f, 0.0f, -7.5f) },
	{ XMFLOAT3(6.0f, 0.0f, -7.5f) },
	{ XMFLOAT3(7.0f, 0.0f, -7.5f) },
	{ XMFLOAT3(7.0f, 0.0f, -8.5f) },
	{ XMFLOAT3(7.0f, 0.0f, -9.5f) },
	{ XMFLOAT3(7.0f, 0.0f, -10.5f) },
	{ XMFLOAT3(8.0f, 0.0f, -10.5f) },
	{ XMFLOAT3(9.0f, 0.0f, -10.5f) },
	{ XMFLOAT3(10.0f, 0.0f, -10.5f) },
	{ XMFLOAT3(11.0f, 0.0f, -10.5f) },
	{ XMFLOAT3(12.0f, 0.0f, -10.5f) },
	{ XMFLOAT3(12.0f, 0.0f, -11.5f) },
	{ XMFLOAT3(12.0f, 0.0f, -12.5f) },
};

void Inky::Update(float dt, bool powerUpActivated, PuckMan::Facing facingState, XMFLOAT3 blinkyPos, int levelNumber, int pelletCounter)
{
	/*if (powerUpActivated)
	{
		this->mGhostStates = GHOST_STATES::FRIGHTENED;
	}
	else
	{
		this->mGhostStates = GHOST_STATES::CHASE;
	}*/
	if (pelletCounter == 30)
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
				std::cout << waypointIterator << std::endl;
			}
			else if (waypointIterator == waypoints.size())
			{
				this->isLooping = true;
				if (isLooping == true)
				{
					this->setPos(XMVectorSet(mScatterWaypoints[this->mCurrWaypointIndex].x, mScatterWaypoints[this->mCurrWaypointIndex].y, mScatterWaypoints[this->mCurrWaypointIndex].z, 0.0f));
					this->mCurrWaypointIndex++;
					if (this->mCurrWaypointIndex == 34)
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
		PathNode* offsetTile;
		
		if (facingState == PuckMan::Facing::F_FORWARD)
		{
			//Offset tile = 2 spaces in PuckMan's facing
			offsetTile = new PathNode((int)round(MazeLoader::GetPacManData().at(0).pos.x), (int)round(MazeLoader::GetPacManData().at(0).pos.z + 2), "");
			//Draw a vector from Blinky's current position to the offset tile's position
			XMVECTOR targetTile = XMVectorSet(blinkyPos.x - offsetTile->xPos, 0.0f, blinkyPos.z - offsetTile->zPos, 0.0f);
			//Double the vector length extending forward, this is Inky's target
			targetTile = XMVectorScale(targetTile, 2.0f);

			mStart = new PathNode((int)this->mPos.x, (int)this->mPos.z, "");
			mGoal = new PathNode((int)targetTile.m128_f32[0], (int)targetTile.m128_f32[2], "");
			waypoints = test.FindPath(mStart, mGoal);

			if (waypoints.size() != 0)
			{
				this->setPos(XMVectorSet((float)waypoints.front()->xPos, mPos.y, (float)waypoints.front()->zPos, 0.0f));
			}

			break;
		}

		if (facingState == PuckMan::Facing::F_BACKWARD)
		{
			//Offset tile = 2 spaces in PuckMan's facing
			offsetTile = new PathNode((int)round(MazeLoader::GetPacManData().at(0).pos.x), (int)round(MazeLoader::GetPacManData().at(0).pos.z - 2), "");
			//Draw a vector from Blinky's current position to the offset tile's position
			XMVECTOR targetTile = XMVectorSet(blinkyPos.x - offsetTile->xPos, 0.0f, blinkyPos.z - offsetTile->zPos, 0.0f);
			//Double the vector length extending forward, this is Inky's target
			targetTile = XMVectorScale(targetTile, 2.0f);

			mStart = new PathNode((int)this->mPos.x, (int)this->mPos.z, "");
			mGoal = new PathNode((int)round(MazeLoader::GetPacManData().at(0).pos.x), (int)round(MazeLoader::GetPacManData().at(0).pos.z - 4), "");
			waypoints = test.FindPath(mStart, mGoal);

			if (waypoints.size() != 0)
			{
				this->setPos(XMVectorSet((float)waypoints.front()->xPos, mPos.y, (float)waypoints.front()->zPos, 0.0f));
			}

			break;
		}

		if (facingState == PuckMan::Facing::F_LEFT)
		{
			//Offset tile = 2 spaces in PuckMan's facing
			offsetTile = new PathNode((int)round(MazeLoader::GetPacManData().at(0).pos.x - 2), (int)round(MazeLoader::GetPacManData().at(0).pos.z), "");
			//Draw a vector from Blinky's current position to the offset tile's position
			XMVECTOR targetTile = XMVectorSet(blinkyPos.x - offsetTile->xPos, 0.0f, blinkyPos.z - offsetTile->zPos, 0.0f);
			//Double the vector length extending forward, this is Inky's target
			targetTile = XMVectorScale(targetTile, 2.0f);


			mStart = new PathNode((int)this->mPos.x, (int)this->mPos.z, "");
			mGoal = new PathNode((int)round(MazeLoader::GetPacManData().at(0).pos.x - 4), (int)round(MazeLoader::GetPacManData().at(0).pos.z), "");
			waypoints = test.FindPath(mStart, mGoal);

			if (waypoints.size() != 0)
			{
				this->setPos(XMVectorSet((float)waypoints.front()->xPos, mPos.y, (float)waypoints.front()->zPos, 0.0f));
			}

			break;
		}

		if (facingState == PuckMan::Facing::F_RIGHT)
		{
			//Offset tile = 2 spaces in PuckMan's facing
			offsetTile = new PathNode((int)round(MazeLoader::GetPacManData().at(0).pos.x + 2), (int)round(MazeLoader::GetPacManData().at(0).pos.z), "");
			//Draw a vector from Blinky's current position to the offset tile's position
			XMVECTOR targetTile = XMVectorSet(blinkyPos.x - offsetTile->xPos, 0.0f, blinkyPos.z - offsetTile->zPos, 0.0f);
			//Double the vector length extending forward, this is Inky's target
			targetTile = XMVectorScale(targetTile, 2.0f);


			mStart = new PathNode((int)this->mPos.x, (int)this->mPos.z, "");
			mGoal = new PathNode((int)round(MazeLoader::GetPacManData().at(0).pos.x + 4), (int)round(MazeLoader::GetPacManData().at(0).pos.z), "");
			waypoints = test.FindPath(mStart, mGoal);

			if (waypoints.size() != 0)
			{
				this->setPos(XMVectorSet((float)waypoints.front()->xPos, mPos.y, (float)waypoints.front()->zPos, 0.0f));
			}

			break;
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
		//XMVectorSet(-2.0f, 0.75f, 0.0f, 0.0f)
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

void Inky::Reset()
{
	this->mGhostStates = GHOST_STATES::IDLE;
	waypointIterator = 0;
	mCurrWaypointIndex = 0;
}