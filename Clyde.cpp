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
	mStart = new PathNode((int)this->mPos.x, (int)this->mPos.z);
	mGoal = new PathNode((int)this->mScatterTile.x, (int)this->mScatterTile.z);
	mWaypoints = path.FindPath(mStart, mGoal);
	scatterPathDrawn = true;
}

Clyde::~Clyde()
{
}

void Clyde::LoadScatterWaypoints()
{
	mScatterWaypoints.push_back(new PathNode((int)-13.0f, (int)-13.5f));
	mScatterWaypoints.push_back(new PathNode((int)-12.0f, (int)-13.5f));
	mScatterWaypoints.push_back(new PathNode((int)-11.0f, (int)-13.5f));
	mScatterWaypoints.push_back(new PathNode((int)-10.0f, (int)-13.5f));
	mScatterWaypoints.push_back(new PathNode((int)-9.0f, (int)-13.5f));
	mScatterWaypoints.push_back(new PathNode((int)-8.0f, (int)-13.5f));
	mScatterWaypoints.push_back(new PathNode((int)-7.0f, (int)-13.5f));
	mScatterWaypoints.push_back(new PathNode((int)-6.0f, (int)-13.5f));
	mScatterWaypoints.push_back(new PathNode((int)-5.0f, (int)-13.5f));
	mScatterWaypoints.push_back(new PathNode((int)-4.0f, (int)-13.5f));
	mScatterWaypoints.push_back(new PathNode((int)-3.0f, (int)-13.5f));
	mScatterWaypoints.push_back(new PathNode((int)-2.0f, (int)-13.5f));
	mScatterWaypoints.push_back(new PathNode((int)-2.0f, (int)-12.5f));
	mScatterWaypoints.push_back(new PathNode((int)-2.0f, (int)-11.5f));
	mScatterWaypoints.push_back(new PathNode((int)-2.0f, (int)-10.5f));
	mScatterWaypoints.push_back(new PathNode((int)-3.0f, (int)-10.5f));
	mScatterWaypoints.push_back(new PathNode((int)-4.0f, (int)-10.5f));
	mScatterWaypoints.push_back(new PathNode((int)-5.0f, (int)-10.5f));
	mScatterWaypoints.push_back(new PathNode((int)-5.0f, (int)-9.5f));
	mScatterWaypoints.push_back(new PathNode((int)-5.0f, (int)-8.5f));
	mScatterWaypoints.push_back(new PathNode((int)-5.0f, (int)-7.5f));
	mScatterWaypoints.push_back(new PathNode((int)-6.0f, (int)-7.5f));
	mScatterWaypoints.push_back(new PathNode((int)-7.0f, (int)-7.5f));
	mScatterWaypoints.push_back(new PathNode((int)-8.0f, (int)-7.5f));
	mScatterWaypoints.push_back(new PathNode((int)-8.0f, (int)-8.5f));
	mScatterWaypoints.push_back(new PathNode((int)-8.0f, (int)-9.5f));
	mScatterWaypoints.push_back(new PathNode((int)-8.0f, (int)-10.5f));
	mScatterWaypoints.push_back(new PathNode((int)-9.0f, (int)-10.5f));
	mScatterWaypoints.push_back(new PathNode((int)-10.0f, (int)-10.5f));
	mScatterWaypoints.push_back(new PathNode((int)-11.0f, (int)-10.5f));
	mScatterWaypoints.push_back(new PathNode((int)-12.0f, (int)-10.5f));
	mScatterWaypoints.push_back(new PathNode((int)-13.0f, (int)-10.5f));
	mScatterWaypoints.push_back(new PathNode((int)-13.0f, (int)-11.5f));
	mScatterWaypoints.push_back(new PathNode((int)-13.0f, (int)-12.5f));
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
		/*int clydePos = ((MazeLoader::GetMazeHeight()) - (int)round(this->getPos().z + 15.5f)) +
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
			if (!scatterPathDrawn)
			{
				mStart = new PathNode((int)this->mPos.x, (int)this->mPos.z);
				mGoal = new PathNode((int)this->mScatterTile.x, (int)this->mScatterTile.z);
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

			/*mScatterTimer += 5.7142 * dt; //dt currently takes (without mutliplying) 40 seconds to reach 7.0f, 5.7142 comes from 40 / 7 to get the number as accurate as possible.
			if (mScatterTimer >= 7.0f)
			{
				this->mGhostStates = GHOST_STATES::CHASE;
				mScatterTimer = 0.0f;
				scatterPathDrawn = false;
				this->mCurrWaypointIndex = 0;
				this->waypointIterator = 0;
			}*/
			break;

			/*if (levelNumber == 1)
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
			}*/
		case CHASE:
			if (!firstChasePathDrawn)
			{
				mStart = new PathNode((int)this->mPos.x, (int)this->mPos.z);
				mGoal = new PathNode((int)round(MazeLoader::GetPacManData().at(0).pos.x), (int)round(MazeLoader::GetPacManData().at(0).pos.z));
				mWaypoints = path.FindPath(mStart, mGoal);
				waypointIterator = 0;
				firstChasePathDrawn = true;
			}
			else
			{
				int row = MazeLoader::GetMazeHeight() - (int)round(this->mPos.x + 15.5f);
				int col = (int)round(this->mPos.z + 14.5f) - 1;
				if (MazeLoader::IsDivergent(row, col))
				{
					mStart = new PathNode((int)this->mPos.x, (int)this->mPos.z);
					mGoal = new PathNode((int)round(MazeLoader::GetPacManData().at(0).pos.x), (int)round(MazeLoader::GetPacManData().at(0).pos.z));
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