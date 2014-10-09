#include "Blinky.h"

Blinky::Blinky(FXMVECTOR pos, float radius) : Ghost(pos, radius)
{
	XMStoreFloat3(&mPos, pos);
	LoadScatterWaypoints();
	this->mGhostStates = GHOST_STATES::SCATTER;
	this->mScatterTimer = 0.0f;
	this->mChaseTimer = 0.0f;
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
	if (!isDead)
	{
		switch (mGhostStates)
		{
		case SCATTER:

			if (!scatterPathDrawn)
			{
				if (PostPathFinding())
				{
					this->UpdateCurrentTweenPoint(dt);
					scatterPathDrawn = true;
				}
				PrePathFinding(this->mPos.x, this->mPos.z, this->mScatterWaypoints[0]->xPos, this->mScatterWaypoints[0]->zPos);
				/*CleanUpNodesWaypoints();
				mStart = new PathNode(this->mPos.x, this->mPos.z);
				mGoal = new PathNode(this->mScatterWaypoints[0]->xPos, this->mScatterWaypoints[0]->zPos);
				mWaypoints = path.FindPath(mStart, mGoal);
				this->SetWayPoints(mWaypoints);
				this->UpdateCurrentTweenPoint(dt);
				scatterPathDrawn = true;*/
			}
			if (mWaypoints.size() != 0)
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
				SetSpeed(levelNumber, GHOST_STATES::SCATTER);
				mScatterTimer += 5.7142 * dt; //dt currently takes (without mutliplying) 40 seconds to reach 7.0f, 5.7142 comes from 40 / 7 to get the number as accurate as possible.
				if (mScatterTimer >= 7.0f)
				{
					this->mGhostStates = GHOST_STATES::CHASE;
					mScatterTimer = 0.0f;
					scatterPathDrawn = false;
					mWaypoints.clear();
				}
			}
			else if (powerUpActivated)
			{
				SetSpeed(levelNumber, GHOST_STATES::FRIGHTENED);
				mScatterTimer += 8.0f * dt; //dt currently takes (without mutliplying) 40 seconds to reach 5.0f, 8 comes from 40 / 5 to get the number as accurate as possible.
				if (mScatterTimer >= 5.0f)
				{
					this->mGhostStates = GHOST_STATES::SCATTER;
					mScatterTimer = 0.0f;
					scatterPathDrawn = false;
					mWaypoints.clear();
				}
			}
			break;

		case CHASE:
			SetSpeed(levelNumber, GHOST_STATES::CHASE);
			if (!firstChasePathDrawn)
			{
				//CleanUpNodesWaypoints();
				//mStart = new PathNode(this->mPos.x, this->mPos.z);
				//mGoal = new PathNode(round(MazeLoader::GetPacManData().at(0).pos.x), round(MazeLoader::GetPacManData().at(0).pos.z));
				//mWaypoints = path.FindPath(mStart, mGoal);
				//this->SetWayPoints(mWaypoints);
				if (PostPathFinding())
				{
					firstChasePathDrawn = true;
				}
				PrePathFinding(this->mPos.x, this->mPos.z, round(MazeLoader::GetPacManData().at(0).pos.x), round(MazeLoader::GetPacManData().at(0).pos.z));
				//firstChasePathDrawn = true;
			}
			else
			{
				int row = MazeLoader::GetMazeHeight() - round(this->mPos.x + 15.5f);
				int col = round(this->mPos.z + 14.5f) - 1;
				if (MazeLoader::IsDivergent(row, col))
				{
					//CleanUpNodesWaypoints();
					//mStart = new PathNode(this->mPos.x, this->mPos.z);
					//mGoal = new PathNode(round(MazeLoader::GetPacManData().at(0).pos.x), round(MazeLoader::GetPacManData().at(0).pos.z));
					//mWaypoints = path.FindPath(mStart, mGoal);
					//this->SetWayPoints(mWaypoints);
					if (PostPathFinding())
					{
					}
					PrePathFinding(this->mPos.x, this->mPos.z, round(MazeLoader::GetPacManData().at(0).pos.x), round(MazeLoader::GetPacManData().at(0).pos.z));
				}
			}
			if (mWaypoints.size() != 0)
			{
				this->UpdateCurrentTweenPoint(dt);
				this->mPos = this->mCurrTweenPoint;
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
			//SetSpeed(levelNumber, GHOST_STATES::FRIGHTENED);
			break;
		}
	}
}

void Blinky::Reset()
{
	this->mGhostStates = GHOST_STATES::SCATTER;
	mWaypoints.clear();
	mChaseTimer = 0.0f;
	mScatterTimer = 0.0f;
	firstChasePathDrawn = false;
	scatterPathDrawn = false;
	isLooping = false;
	reachedEnd = false;
}