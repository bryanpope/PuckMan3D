#include "Clyde.h"

Clyde::Clyde(FXMVECTOR pos, float radius) : Ghost(pos, radius)
{
	XMStoreFloat3(&mPos, pos);
	LoadScatterWaypoints();
	this->mGhostStates = GHOST_STATES::IDLE;
	this->mScatterTile.x = -13.0f;
	this->mScatterTile.z = -14.5f;
	this->waypointIterator = 0;
	this->mScatterTimer = 0;
	this->mChaseTimer = 0;

	//Draw the path to his scatter area prior to the start of the game to prevent bottlenecks
	mStart = new PathNode(this->mPos.x, this->mPos.z);
	mGoal = new PathNode(this->mScatterTile.x, this->mScatterTile.z);
	mWaypoints = path.FindPath(mStart, mGoal);
	scatterPathDrawn = true;
}

Clyde::~Clyde()
{
}

void Clyde::LoadScatterWaypoints()
{
	mScatterWaypoints.push_back(new PathNode(-13.0f, -14.5f));
	mScatterWaypoints.push_back(new PathNode(-12.0f, -14.5f));
	mScatterWaypoints.push_back(new PathNode(-11.0f, -14.5f));
	mScatterWaypoints.push_back(new PathNode(-10.0f, -14.5f));
	mScatterWaypoints.push_back(new PathNode(-9.0f, -14.5f));
	mScatterWaypoints.push_back(new PathNode(-8.0f, -14.5f));
	mScatterWaypoints.push_back(new PathNode(-7.0f, -14.5f));
	mScatterWaypoints.push_back(new PathNode(-6.0f, -14.5f));
	mScatterWaypoints.push_back(new PathNode(-5.0f, -14.5f));
	mScatterWaypoints.push_back(new PathNode(-4.0f, -14.5f));
	mScatterWaypoints.push_back(new PathNode(-3.0f, -14.5f));
	mScatterWaypoints.push_back(new PathNode(-2.0f, -14.5f));
	mScatterWaypoints.push_back(new PathNode(-2.0f, -13.5f));
	mScatterWaypoints.push_back(new PathNode(-2.0f, -12.5f));
	mScatterWaypoints.push_back(new PathNode(-2.0f, -11.5f));
	mScatterWaypoints.push_back(new PathNode(-3.0f, -11.5f));
	mScatterWaypoints.push_back(new PathNode(-4.0f, -11.5f));
	mScatterWaypoints.push_back(new PathNode(-5.0f, -11.5f));
	mScatterWaypoints.push_back(new PathNode(-5.0f, -10.5f));
	mScatterWaypoints.push_back(new PathNode(-5.0f, -9.5f));
	mScatterWaypoints.push_back(new PathNode(-5.0f, -8.5f));
	mScatterWaypoints.push_back(new PathNode(-6.0f, -8.5f));
	mScatterWaypoints.push_back(new PathNode(-7.0f, -8.5f));
	mScatterWaypoints.push_back(new PathNode(-8.0f, -8.5f));
	mScatterWaypoints.push_back(new PathNode(-8.0f, -9.5f));
	mScatterWaypoints.push_back(new PathNode(-8.0f, -10.5f));
	mScatterWaypoints.push_back(new PathNode(-8.0f, -11.5f));
	mScatterWaypoints.push_back(new PathNode(-9.0f, -11.5f));
	mScatterWaypoints.push_back(new PathNode(-10.0f, -11.5f));
	mScatterWaypoints.push_back(new PathNode(-11.0f, -11.5f));
	mScatterWaypoints.push_back(new PathNode(-12.0f, -11.5f));
	mScatterWaypoints.push_back(new PathNode(-13.0f, -11.5f));
	mScatterWaypoints.push_back(new PathNode(-13.0f, -12.5f));
	mScatterWaypoints.push_back(new PathNode(-13.0f, -13.5f));
}

void Clyde::Update(float dt, bool powerUpActivated, int levelNumber, int pelletCounter)
{
	/*if (powerUpActivated)
	{
		this->mGhostStates = GHOST_STATES::FRIGHTENED;
	}
	else
	{
		this->mGhostStates = prevState;
	}*/

	//this->UpdateCurrentTweenPoint(dt);

	if (pelletCounter >= 90)
	{
		this->isIdle = false;
		mGhostStates = GHOST_STATES::SCATTER;
	}

	if (!isIdle)
	{
		//Get clyde's position in row/col space
		/*int clydePos = ((MazeLoader::GetMazeHeight()) - round(this->getPos().z + 15.5f)) +
			((MazeLoader::GetMazeWidth()) - round(this->getPos().x + 15.5f));
		//Get puckman's position in row/col space
		int puckmanPos = ((MazeLoader::GetMazeHeight()) - round(MazeLoader::GetPacManData().at(0).pos.x + 15.5f)) +
			((MazeLoader::GetMazeWidth()) - round(MazeLoader::GetPacManData().at(0).pos.z + 15.5f));
		//Determine the euclidean distance between clyde and puckman
		int euclidDistance = abs(clydePos - puckmanPos);
		//If the distance between Clyde and PuckMan is 8 tiles or more in Euclidean space, target PuckMan and chase like Blinky does
		if (euclidDistance >= 8)
		{
			mGhostStates = GHOST_STATES::CHASE;
			mChaseTimer = 0.0f;
			waypointIterator = 0;
			firstChasePathDrawn = false;
		}
		//If the distance between Clyde and Puckman is less than 8 tiles, go back to Scatter mode
		else
		{
			mGhostStates = GHOST_STATES::SCATTER;
			mScatterTimer = 0.0f;
			scatterPathDrawn = false;
			this->mCurrWaypointIndex = 0;
			this->waypointIterator = 0;
		}*/

		switch (mGhostStates)
		{
		case SCATTER:
			std::cout << "Clyde pos " << mPos.x << ", " << mPos.z << std::endl;
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
				this->mCurrWaypointIndex = 0;
				this->waypointIterator = 0;
			}
			break;
		case CHASE:
			if (!firstChasePathDrawn)
			{
				mStart = new PathNode(this->mPos.x, this->mPos.z);
				mGoal = new PathNode(round(MazeLoader::GetPacManData().at(0).pos.x), round(MazeLoader::GetPacManData().at(0).pos.z));
				mWaypoints = path.FindPath(mStart, mGoal);
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
					waypointIterator = 0;
				}
			}
			if (mWaypoints.size() != 0)
			{
				if (waypointIterator < mWaypoints.size())
				{
					this->setPos(XMVectorSet(this->mWaypoints.at(waypointIterator)->xPos, mPos.y, this->mWaypoints.at(waypointIterator)->zPos, 0.0f));
					waypointIterator++;
				}
				else if (waypointIterator >= mWaypoints.size())
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
			break;
		}
	}
}

void Clyde::Reset()
{
	this->mGhostStates = GHOST_STATES::IDLE;
	waypointIterator = 0;
	mCurrWaypointIndex = 0;
	scatterPathDrawn = false;
	isLooping = false;
	isIdle = true;
}