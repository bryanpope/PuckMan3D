#include "Clyde.h"

Clyde::Clyde(FXMVECTOR pos, float radius) : Ghost(pos, radius)
{
	XMStoreFloat3(&mPos, pos);
	LoadScatterWaypoints();
	this->mGhostStates = GHOST_STATES::IDLE;
	this->mScatterTile.x = -13.0f;
	this->mScatterTile.z = -14.5f;
	this->mScatterTimer = 0.0f;
	this->mChaseTimer = 0.0f;

	//Draw the path to his scatter area prior to the start of the game to prevent bottlenecks
	mStart = new PathNode(this->mPos.x, this->mPos.z);
	mGoal = new PathNode(this->mScatterTile.x, this->mScatterTile.z);
	mWaypoints = path.FindPath(mStart, mGoal);
	scatterPathDrawn = true;
}

Clyde::~Clyde()
{
	for (int i = 0; i < mScatterWaypoints.size(); ++i)
	{
		if (mScatterWaypoints[i])
		{
			delete mScatterWaypoints[i];
			mScatterWaypoints[i] = NULL;
		}
	}
	CleanUpNodesWaypoints();
}

void Clyde::LoadScatterWaypoints()
{
	mScatterWaypoints.push_back(new PathNode(-13.0f, -14.5f));
	mScatterWaypoints.push_back(new PathNode(-13.0f, -11.5f));
	mScatterWaypoints.push_back(new PathNode(-8.0f, -11.0f));
	mScatterWaypoints.push_back(new PathNode(-8.0f, -8.5f));
	mScatterWaypoints.push_back(new PathNode(-5.0f, -8.5f));
	mScatterWaypoints.push_back(new PathNode(-5.0f, -11.5f));
	mScatterWaypoints.push_back(new PathNode(-2.0f, -11.5f));
	mScatterWaypoints.push_back(new PathNode(-2.0f, -14.5f));
	mScatterWaypoints.push_back(new PathNode(-13.0f, -14.5f));	
}

void Clyde::Update(float dt, bool powerUpActivated, int levelNumber, int pelletCounter)
{
	if (!isDead)
	{
		if (pelletCounter >= 90 && isIdle)
		{
			this->isIdle = false;
			mGhostStates = GHOST_STATES::SCATTER;
		}

		if (!isIdle)
		{
			//Get clyde's position in row/col space
			int clydePos = ((MazeLoader::GetMazeHeight()) - round(this->getPos().z + 15.5f)) +
				((MazeLoader::GetMazeWidth()) - round(this->getPos().x + 15.5f));
			//Get puckman's position in row/col space
			int puckmanPos = ((MazeLoader::GetMazeHeight()) - round(MazeLoader::GetPacManData().at(0).pos.x + 15.5f)) +
				((MazeLoader::GetMazeWidth()) - round(MazeLoader::GetPacManData().at(0).pos.z + 15.5f));
			//Determine the euclidean distance between clyde and puckman
			int euclidDistance = abs(clydePos - puckmanPos);
			//If the distance between Clyde and PuckMan is 8 tiles or more in Euclidean space, target PuckMan and chase like Blinky does
				//For the sake of less bouncing, we'll only check this every second
			mPathCurrent += dt * 2;
			if (mPathCurrent >= mPathNext)
			{
				if (euclidDistance >= 8 && !powerUpActivated)
				{
					mGhostStates = GHOST_STATES::CHASE;
					mChaseTimer = 0.0f;
					firstChasePathDrawn = false;
				}
				//If the distance between Clyde and Puckman is less than 8 tiles, go back to Scatter mode
				else if (euclidDistance < 8 && !powerUpActivated)
				{
					mGhostStates = GHOST_STATES::SCATTER;
					mScatterTimer = 0.0f;
					scatterPathDrawn = false;
					this->mCurrWaypointIndex = 0;
				}
				mPathNext += 1.0f;
			}

			switch (mGhostStates)
			{
			case SCATTER:
				SetSpeed(levelNumber, GHOST_STATES::SCATTER);
				if (!scatterPathDrawn)
				{
					PrePathFinding(this->mPos.x, this->mPos.z, this->mScatterWaypoints[0]->xPos, this->mScatterWaypoints[0]->zPos);
					if (PostPathFinding())
					{
						this->UpdateCurrentTweenPoint(dt);
						scatterPathDrawn = true;
					}
				}
				if (mTweenPoints.size() != 0)
				{
					if (!this->reachedEnd)
					{
						this->mPos = this->mCurrTweenPoint;
						this->UpdateCurrentTweenPoint(dt);
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
				if (!powerUpActivated)
				{
					this->mGhostStates = GHOST_STATES::SCATTER;
					mScatterTimer += 5.7142f * dt; //dt currently takes (without mutliplying) 40 seconds to reach 7.0f, 5.7142 comes from 40 / 7 to get the number as accurate as possible.
					if (mScatterTimer >= 7.0f)
					{
						this->mGhostStates = GHOST_STATES::CHASE;
						mScatterTimer = 0.0f;
						reachedEnd = false;
						isLooping = false;
						CleanUpNodesWaypoints();
					}
				}
				//If the powerup is activated, switch to the FRIGHTENED state
				else if (powerUpActivated)
				{
					SetSpeed(levelNumber, GHOST_STATES::FRIGHTENED);
					CleanUpNodesWaypoints();
					scatterPathDrawn = false;
					reachedEnd = false;
					isLooping = false;
					mPrevState = mGhostStates;
					this->mGhostStates = GHOST_STATES::FRIGHTENED;
				}

				break;
			case CHASE:
				SetSpeed(levelNumber, GHOST_STATES::CHASE);
				if (!firstChasePathDrawn)
				{
					PrePathFinding(this->mPos.x, this->mPos.z, round(MazeLoader::GetPacManData().at(0).pos.x), round(MazeLoader::GetPacManData().at(0).pos.z));
					if (PostPathFinding())
					{
						this->UpdateCurrentTweenPoint(dt);
						firstChasePathDrawn = true;
					}
				}
				else
				{
					mPathCurrent += dt;
					if (mPathCurrent >= mPathNext)
					{
						PrePathFinding(this->mPos.x, this->mPos.z, round(MazeLoader::GetPacManData().at(0).pos.x), round(MazeLoader::GetPacManData().at(0).pos.z));
						if (PostPathFinding())
						{
							this->UpdateCurrentTweenPoint(dt);
							mPathNext += (1.0f / 10.0f);
						}
					}
				}
				if (mTweenPoints.size() != 0)
				{
					this->mPos = this->mCurrTweenPoint;
					this->UpdateCurrentTweenPoint(dt);
				}

				if (!powerUpActivated)
				{
					mGhostStates = GHOST_STATES::CHASE;
					mChaseTimer += 5.7142f * dt; //dt currently takes (without mutliplying) 40 seconds to reach 7.0f, 5.7142 comes from 40 / 7 to get the number as accurate as possible.
					if (mChaseTimer >= 7.0f) //Chase time is over, time to scatter
					{
						this->mGhostStates = GHOST_STATES::SCATTER;
						mChaseTimer = 0.0f;
						scatterPathDrawn = false;
						reachedEnd = false;
						isLooping = false;
						CleanUpNodesWaypoints();
					}
				}
				//If the powerup is activated, switch to the FRIGHTENED state
				else if (powerUpActivated)
				{
					SetSpeed(levelNumber, GHOST_STATES::FRIGHTENED);
					CleanUpNodesWaypoints();
					mPrevState = mGhostStates;
					this->mGhostStates = GHOST_STATES::FRIGHTENED;
					scatterPathDrawn = false;
					firstChasePathDrawn = false;
				}
				break;

			case FRIGHTENED:
				SetSpeed(levelNumber, GHOST_STATES::FRIGHTENED);
				if (!scatterPathDrawn)
				{
					PrePathFinding(this->mPos.x, this->mPos.z, this->mScatterWaypoints[0]->xPos, this->mScatterWaypoints[0]->zPos);
					if (PostPathFinding())
					{
						this->UpdateCurrentTweenPoint(dt);
						scatterPathDrawn = true;
					}
				}
				if (mTweenPoints.size() != 0)
				{
					if (!this->reachedEnd)
					{
						this->mPos = this->mCurrTweenPoint;
						this->UpdateCurrentTweenPoint(dt);

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
				if (!powerUpActivated)
				{
					mGhostStates = mPrevState;
				}
				break;
			}
		}
	}
}

void Clyde::Reset()
{
	this->mGhostStates = GHOST_STATES::IDLE;
	mWaypoints.clear();
	mChaseTimer = 0.0f;
	mScatterTimer = 0.0f;
	isIdle = true;
	firstChasePathDrawn = false;
	scatterPathDrawn = false;
	isLooping = false;
	reachedEnd = false;
	CleanUpNodesWaypoints();
}