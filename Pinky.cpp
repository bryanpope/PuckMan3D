#include "Pinky.h"

Pinky::Pinky(FXMVECTOR pos, float radius) : Ghost(pos, radius)
{
	XMStoreFloat3(&mPos, pos);
	LoadScatterWaypoints();
	this->mGhostStates = GHOST_STATES::SCATTER;
	this->mScatterTile.x = -13.0f;
	this->mScatterTile.z = 14.5f;
	this->mScatterTimer = 0.0f;
	this->mChaseTimer = 0.0f;

	//Draw the path to his scatter area prior to the start of the game to prevent bottlenecks
	mStart = new PathNode(this->mPos.x, this->mPos.z);
	mGoal = new PathNode(this->mScatterTile.x, this->mScatterTile.z);
	mWaypoints = path.FindPath(mStart, mGoal);
	scatterPathDrawn = true;
}

Pinky::~Pinky()
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

void Pinky::LoadScatterWaypoints()
{
	mScatterWaypoints.push_back(new PathNode(-13.0f, 14.5f));
	mScatterWaypoints.push_back(new PathNode(-13.0f, 10.5f));
	mScatterWaypoints.push_back(new PathNode(-8.0f, 10.5f));
	mScatterWaypoints.push_back(new PathNode(-8.0f, 14.5f));
	mScatterWaypoints.push_back(new PathNode(-13.0f, 14.5f));
}

void Pinky::Update(float dt, bool powerUpActivated, Direction::DIRECTION facingState, int levelNumber)
{
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
				mScatterTimer += 5.7142f * dt; //dt currently takes (without mutliplying) 40 seconds to reach 7.0f, 5.7142 comes from 40 / 7 to get the number as accurate as possible.
				//When the timer has reached 7.0f switch to the CHASE state
				if (mScatterTimer >= 7.0f)
				{
					this->mGhostStates = GHOST_STATES::CHASE;
					mScatterTimer = 0.0f;
					reachedEnd = false;
					isLooping = false;
					CleanUpNodesWaypoints();
					mTweenPoints.clear();
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
				mTweenPoints.clear();
			}
			break;
		case CHASE:
			SetSpeed(levelNumber, GHOST_STATES::CHASE);
			if (!firstChasePathDrawn)
			{
				PrePathFinding(this->mPos.x, this->mPos.z, this->mScatterWaypoints[0]->xPos, this->mScatterWaypoints[0]->zPos);
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
					//Target 4 tiles in front of PuckMan's facing
					if (facingState == Direction::DIRECTION::NORTH || facingState == Direction::DIRECTION::SOUTH)
					{
						//Clamp the Z value of the target so it cannot choose a tile outside of the level
						float clampedZ = MathHelper::Clamp((int)round(MazeLoader::GetPacManData().at(0).pos.z + (4 * Direction::getDirecitonVector(facingState).m128_f32[2])),
							(int)round(MazeLoader::GetPacManData().at(0).pos.z + (4 * Direction::getDirecitonVector(facingState).m128_f32[2])),
							(int)MazeLoader::GetMazeHeight());

						//Check if clampedZ is a valid tile, if not move the target tile one more in the opposite direction and try again
						int goalRow = (MazeLoader::GetMazeHeight()) - round(clampedZ + 15.5f);
						int goalCol = (round(MazeLoader::GetPacManData().at(0).pos.x + 14.5f) - 1);
						while (MazeLoader::IsBlocked(goalRow, goalCol))
						{
							clampedZ -= 1 * Direction::getDirecitonVector(facingState).m128_f32[2];
							goalRow = (MazeLoader::GetMazeHeight()) - round(clampedZ + 15.5f);
							goalCol = (round(MazeLoader::GetPacManData().at(0).pos.x + 14.5f) - 1);
						}

						PrePathFinding(this->mPos.x, this->mPos.z, round(MazeLoader::GetPacManData().at(0).pos.x), round(MazeLoader::GetPacManData().at(0).pos.z));
						if (PostPathFinding())
						{
							this->UpdateCurrentTweenPoint(dt);
							mPathNext += (1.0f / 10.0f);
						}
					}

					else if (facingState == Direction::DIRECTION::WEST || facingState == Direction::DIRECTION::EAST)
					{
						//Clamp the X value of the target so it cannot choose a tile outside of the level
						float clampedX = MathHelper::Clamp((int)round(MazeLoader::GetPacManData().at(0).pos.x + (4 * Direction::getDirecitonVector(facingState).m128_f32[0])),
							(int)round(MazeLoader::GetPacManData().at(0).pos.x + (4 * Direction::getDirecitonVector(facingState).m128_f32[0])),
							(int)MazeLoader::GetMazeWidth());

						//Check if clampedX is a valid tile, if not move the target tile one more in the opposite direction and try again
						int goalRow = (MazeLoader::GetMazeHeight()) - round(MazeLoader::GetPacManData().at(0).pos.z + 15.5f);
						int goalCol = (round(clampedX + 14.5f) - 1);
						while (MazeLoader::IsBlocked(goalRow, goalCol))
						{
							clampedX -= 1 * Direction::getDirecitonVector(facingState).m128_f32[0];
							goalRow = (MazeLoader::GetMazeHeight()) - round(MazeLoader::GetPacManData().at(0).pos.z + 15.5f);
							goalCol = (round(clampedX + 14.5f) - 1);
						}

						PrePathFinding(this->mPos.x, this->mPos.z, round(MazeLoader::GetPacManData().at(0).pos.x), round(MazeLoader::GetPacManData().at(0).pos.z));
						if (PostPathFinding())
						{
							this->UpdateCurrentTweenPoint(dt);
							mPathNext += (1.0f / 10.0f);
						}
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
					mTweenPoints.clear();
				}
			}
			//If the powerup is activated, switch to the FRIGHTENED state
			else if (powerUpActivated)
			{
				SetSpeed(levelNumber, GHOST_STATES::FRIGHTENED);
				CleanUpNodesWaypoints();
				mTweenPoints.clear();
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
				mTweenPoints.clear();
			}
			break;
		}
	}
}

void Pinky::Reset()
{
	this->mGhostStates = GHOST_STATES::SCATTER;
	mTweenPoints.clear();
	mWaypoints.clear();
	mChaseTimer = 0.0f;
	mScatterTimer = 0.0f;
	mPathCurrent = 0.0f;
	mPathNext = 0.0f;
	firstChasePathDrawn = false;
	scatterPathDrawn = false;
	isLooping = false;
	reachedEnd = false;
	CleanUpNodesWaypoints();
}