#include "Pinky.h"

Pinky::Pinky(FXMVECTOR pos, FXMVECTOR vel, float radius) : Ghost(pos, vel, radius)
{
	XMStoreFloat3(&mPos, pos);
	XMStoreFloat3(&mVel, vel);
	this->mGhostStates = GHOST_STATES::SCATTER;
	this->mScatterTile.x = -13.0f;
	this->mScatterTile.z = 14.5f;
	this->waypointIterator = 0;
	this->mScatterTimer = 0;
	this->mChaseTimer = 0;

	//Draw the path to his scatter area prior to the start of the game to prevent bottlenecks
	mStart = new PathNode((int)this->mPos.x, (int)this->mPos.z);
	mGoal = new PathNode((int)this->mScatterTile.x, (int)this->mScatterTile.z);
	waypoints = test.FindPath(mStart, mGoal);
	scatterPathDrawn = true;
}

Pinky::~Pinky()
{
}

const XMFLOAT3 Pinky::mScatterWaypoints[18] =
{
	{ XMFLOAT3(-13.0f, 0.0f, 13.5f) },
	{ XMFLOAT3(-13.0f, 0.0f, 12.5f) },
	{ XMFLOAT3(-13.0f, 0.0f, 11.5f) },
	{ XMFLOAT3(-13.0f, 0.0f, 10.5f) },
	{ XMFLOAT3(-12.0f, 0.0f, 10.5f) },
	{ XMFLOAT3(-11.0f, 0.0f, 10.5f) },
	{ XMFLOAT3(-10.0f, 0.0f, 10.5f) },
	{ XMFLOAT3(-9.0f, 0.0f, 10.5f) },
	{ XMFLOAT3(-8.0f, 0.0f, 10.5f) },
	{ XMFLOAT3(-8.0f, 0.0f, 11.5f) },
	{ XMFLOAT3(-8.0f, 0.0f, 12.5f) },
	{ XMFLOAT3(-8.0f, 0.0f, 13.5f) },
	{ XMFLOAT3(-8.0f, 0.0f, 14.5f) },
	{ XMFLOAT3(-9.0f, 0.0f, 14.5f) },
	{ XMFLOAT3(-10.0f, 0.0f, 14.5f) },
	{ XMFLOAT3(-11.0f, 0.0f, 14.5f) },
	{ XMFLOAT3(-12.0f, 0.0f, 14.5f) },
	{ XMFLOAT3(-13.0f, 0.0f, 14.5f) },
};

const std::string Pinky::mScatterFacing[18] =
{
	{ "backward" },
	{ "backward" },
	{ "backward" },
	{ "backward" },
	{ "right" },
	{ "right" },
	{ "right" },
	{ "right" },
	{ "right" },
	{ "up" },
	{ "up" },
	{ "up" },
	{ "up" },
	{ "left" },
	{ "left" },
	{ "left" },
	{ "left" },
	{ "left" }
};

void Pinky::Update(float dt, bool powerUpActivated, PuckMan::Facing facingState, int levelNumber)
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
					if (this->mCurrWaypointIndex == 18)
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
		//Target 4 tiles in front of PuckMan's facing
		if (facingState == PuckMan::Facing::F_FORWARD)
		{
			mStart = new PathNode((int)this->mPos.x, (int)this->mPos.z, "");
			mGoal = new PathNode((int)round(MazeLoader::GetPacManData().at(0).pos.x), (int)round(MazeLoader::GetPacManData().at(0).pos.z + 4.0f), PuckManFacingToString(facingState));
			waypoints = test.FindPath(mStart, mGoal);

			if (waypoints.size() != 0)
			{
				this->setPos(XMVectorSet((float)waypoints.front()->xPos, mPos.y, (float)waypoints.front()->zPos, 0.0f));
			}

			break;
		}

		if (facingState == PuckMan::Facing::F_BACKWARD)
		{
			mStart = new PathNode((int)this->mPos.x, (int)this->mPos.z, "");
			mGoal = new PathNode((int)round(MazeLoader::GetPacManData().at(0).pos.x), (int)round(MazeLoader::GetPacManData().at(0).pos.z - 4.0f), PuckManFacingToString(facingState));
			waypoints = test.FindPath(mStart, mGoal);

			if (waypoints.size() != 0)
			{
				this->setPos(XMVectorSet((float)waypoints.front()->xPos, mPos.y, (float)waypoints.front()->zPos, 0.0f));
			}

			break;
		}

		if (facingState == PuckMan::Facing::F_LEFT)
		{
			mStart = new PathNode((int)this->mPos.x, (int)this->mPos.z, "");
			mGoal = new PathNode((int)round(MazeLoader::GetPacManData().at(0).pos.x - 4.0f), (int)round(MazeLoader::GetPacManData().at(0).pos.z), PuckManFacingToString(facingState));
			waypoints = test.FindPath(mStart, mGoal);

			if (waypoints.size() != 0)
			{
				this->setPos(XMVectorSet((float)waypoints.front()->xPos, mPos.y, (float)waypoints.front()->zPos, 0.0f));
			}

			break;
		}

		if (facingState == PuckMan::Facing::F_RIGHT)
		{
			mStart = new PathNode((int)this->mPos.x, (int)this->mPos.z, "");
			mGoal = new PathNode((int)round(MazeLoader::GetPacManData().at(0).pos.x + 4.0f), (int)round(MazeLoader::GetPacManData().at(0).pos.z), PuckManFacingToString(facingState));
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
		//XMVectorSet(0.0f, 0.75f, 0.0f, 0.0f)
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

void Pinky::Reset()
{
	this->mGhostStates = GHOST_STATES::SCATTER;
	waypointIterator = 0;
	mCurrWaypointIndex = 0;
	scatterPathDrawn = false;
	isLooping = false;
}