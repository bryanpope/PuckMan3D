#include "Blinky.h"

Blinky::Blinky(FXMVECTOR pos, float radius) : Ghost(pos, radius)
{
	XMStoreFloat3(&mPos, pos);
	LoadScatterWaypoints();
	this->mGhostStates = GHOST_STATES::SCATTER;
	this->mScatterTile.x = 7.0f;
	this->mScatterTile.z = 14.5f;
	this->mScatterTimer = 0.0f;
	this->mChaseTimer = 0.0f;
	this->mFrightenedTimer = 0.0f;
	mChaseTimer = 0.0f;
	firstChasePathDrawn = false;
	isLooping = false;

	//Draw the path to his scatter area prior to the start of the game to prevent bottlenecks
	mStart = new PathNode(this->mPos.x, this->mPos.z);
	mGoal = new PathNode(this->mScatterTile.x, this->mScatterTile.z);
	mWaypoints = path.FindPath(mStart, mGoal);
	scatterPathDrawn = true;
}

Blinky::~Blinky()
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

void Blinky::LoadScatterWaypoints()
{
	mScatterWaypoints.push_back(new PathNode(7.0f, 14.5f));
	mScatterWaypoints.push_back(new PathNode(12.0f, 14.5f));
	mScatterWaypoints.push_back(new PathNode(12.0f, 10.5f));
	mScatterWaypoints.push_back(new PathNode(7.0f, 10.5f));
	mScatterWaypoints.push_back(new PathNode(7.0f, 14.5f));
}

void Blinky::Update(float dt, bool powerUpActivated, int levelNumber)
{
	std::cout << "Blinky chase timer " << mChaseTimer << std::endl;
	std::cout << "Blinky scatter timer " << mScatterTimer << std::endl;
	//isDead = true;
	if (!isDead)
	{
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
				mScatterTimer += 5.7142 * dt; //dt currently takes (without mutliplying) 40 seconds to reach 7.0f, 5.7142 comes from 40 / 7 to get the number as accurate as possible.
				//When the timer has reached 7.0f switch to the CHASE state
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
				scatterPathDrawn = false;
				reachedEnd = false;
				isLooping = false;
				mPrevState = mGhostStates;
				this->mGhostStates = GHOST_STATES::FRIGHTENED;
				CleanUpNodesWaypoints();
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
				mPathCurrent += dt * 2;
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
				mChaseTimer += 5.7142 * dt; //dt currently takes (without mutliplying) 40 seconds to reach 7.0f, 5.7142 comes from 40 / 7 to get the number as accurate as possible.
				//When the timer has reached 7.0f switch to the CHASE state
				if (mChaseTimer >= 7.0f)
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

void Blinky::Reset()
{
	this->mGhostStates = GHOST_STATES::SCATTER;
	mChaseTimer = 0.0f;
	mScatterTimer = 0.0f;
	firstChasePathDrawn = false;
	scatterPathDrawn = false;
	isLooping = false;
	reachedEnd = false;
	CleanUpNodesWaypoints();
}