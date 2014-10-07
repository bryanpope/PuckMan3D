#include "Blinky.h"

Blinky::Blinky(FXMVECTOR pos, float radius) : Ghost(pos, radius)
{
	XMStoreFloat3(&mPos, pos);
	LoadScatterWaypoints();
	this->mGhostStates = GHOST_STATES::SCATTER;
	this->mScatterTile.x = 12.0f;
	this->mScatterTile.z = 14.5f;
	this->waypointIterator = 0;
	this->mScatterTimer = 0.0f;
	this->mChaseTimer = 0.0f;

	//Draw the path to his scatter area prior to the start of the game to prevent bottlenecks
	mStart = new PathNode(this->mPos.x, this->mPos.z);
	mGoal = new PathNode(this->mScatterTile.x, this->mScatterTile.z);
	mWaypoints = path.FindPath(mStart, mGoal);
	scatterPathDrawn = false;
}

Blinky::~Blinky()
{
}

void Blinky::LoadScatterWaypoints()
{
	mScatterWaypoints.push_back(new PathNode(12.0f, 13.5f));
	mScatterWaypoints.push_back(new PathNode(12.0f, 12.5f));
	mScatterWaypoints.push_back(new PathNode(12.0f, 11.5f));
	mScatterWaypoints.push_back(new PathNode(12.0f, 10.5f));
	mScatterWaypoints.push_back(new PathNode(11.0f, 10.5f));
	mScatterWaypoints.push_back(new PathNode(10.0f, 10.5f));
	mScatterWaypoints.push_back(new PathNode(9.0f, 10.5f));
	mScatterWaypoints.push_back(new PathNode(8.0f, 10.5f));
	mScatterWaypoints.push_back(new PathNode(7.0f, 10.5f));
	mScatterWaypoints.push_back(new PathNode(7.0f, 11.5f));
	mScatterWaypoints.push_back(new PathNode(7.0f, 12.5f));
	mScatterWaypoints.push_back(new PathNode(7.0f, 13.5f));
	mScatterWaypoints.push_back(new PathNode(7.0f, 14.5f));
	mScatterWaypoints.push_back(new PathNode(8.0f, 14.5f));
	mScatterWaypoints.push_back(new PathNode(9.0f, 14.5f));
	mScatterWaypoints.push_back(new PathNode(10.0f, 14.5f));
	mScatterWaypoints.push_back(new PathNode(11.0f, 14.5f));
	mScatterWaypoints.push_back(new PathNode(12.0f, 14.5f));
}

void Blinky::Update(float dt, bool powerUpActivated, int levelNumber)
{
	std::cout << "blinky pos " << mPos.x << ", " << mPos.z << std::endl;
	switch (mGhostStates)
	{
	case SCATTER:
		//SetVelocity(levelNumber, GHOST_STATES::SCATTER);
		if (!scatterPathDrawn)
		{
			mStart = new PathNode(this->mPos.x, this->mPos.z);
			mGoal = new PathNode(this->mScatterTile.x, this->mScatterTile.z);
			mWaypoints = path.FindPath(mStart, mGoal);
			this->SetWayPoints(mWaypoints);
			scatterPathDrawn = true;
			waypointIterator = 0;
		}
		if (mWaypoints.size() != 0)
		{
			if (!this->reachedEnd)
			{
				this->UpdateCurrentTweenPoint(dt);
				this->mPos = this->mCurrTweenPoint;
			}
			else if (this->reachedEnd)
			{
				if (!isLooping)
				{
					this->SetWayPoints(mScatterWaypoints);
					this->isLooping = true;
				}
				if (isLooping == true && this->reachedEnd)
				{
					this->UpdateCurrentTweenPoint(dt);
					this->mPos = this->mCurrTweenPoint;
				}
			}
		}
		//mScatterTimer += 5.7142 * dt; //dt currently takes (without mutliplying) 40 seconds to reach 7.0f, 5.7142 comes from 40 / 7 to get the number as accurate as possible.
		if (mScatterTimer >= 7.0f)
		{
			this->mGhostStates = GHOST_STATES::CHASE;
			mScatterTimer = 0.0f;
			scatterPathDrawn = false;
			this->isLooping = false;
			this->reachedEnd = false;
			this->mCurrWaypointIndex = 0;
			this->waypointIterator = 0;
		}			
		break;

	case CHASE:
		//SetVelocity(levelNumber, GHOST_STATES::CHASE);
		if (!firstChasePathDrawn)
		{
			mStart = new PathNode(this->mPos.x, this->mPos.z);
			mGoal = new PathNode(round(MazeLoader::GetPacManData().at(0).pos.x), round(MazeLoader::GetPacManData().at(0).pos.z));
			mWaypoints = path.FindPath(mStart, mGoal);
			this->SetWayPoints(mWaypoints);
			waypointIterator = 0;
			firstChasePathDrawn = true;
		}
		else
		{
			int row = MazeLoader::GetMazeHeight() - round(this->mPos.x + 15.5f);
			int col = round(this->mPos.z + 14.5f) - 1;
			if (MazeLoader::IsDivergent(row, col))
			{
				mStart = new PathNode(this->mPos.x, this->mPos.z);
				mGoal = new PathNode(round(MazeLoader::GetPacManData().at(0).pos.x), round(MazeLoader::GetPacManData().at(0).pos.z));
				mWaypoints = path.FindPath(mStart, mGoal);
				this->SetWayPoints(mWaypoints);
				waypointIterator = 0;
			}
		}
		if (mWaypoints.size() != 0)
		{
			if (waypointIterator < mWaypoints.size())
			{
				this->UpdateCurrentTweenPoint(dt);
				this->mPos = this->mCurrTweenPoint;
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
		SetVelocity(levelNumber, GHOST_STATES::FRIGHTENED);
		break;
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