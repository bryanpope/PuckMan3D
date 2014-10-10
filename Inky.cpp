#include "Inky.h"

Inky::Inky(FXMVECTOR pos, float radius) : Ghost(pos, radius)
{
	XMStoreFloat3(&mPos, pos);
	LoadScatterWaypoints();
	this->mGhostStates = GHOST_STATES::IDLE;
	this->mScatterTile.x = 12;
	this->mScatterTile.z = -14.5f;
	this->mScatterTimer = 0.0f;
	this->mChaseTimer = 0.0f;

	//Draw the path to his scatter area prior to the start of the game to prevent bottlenecks
	mStart = new PathNode(this->mPos.x, this->mPos.z);
	mGoal = new PathNode(this->mScatterTile.x, this->mScatterTile.z);
	mWaypoints = path.FindPath(mStart, mGoal);
	scatterPathDrawn = true;
	firstChasePathDrawn = false;
}

Inky::~Inky()
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

void Inky::LoadScatterWaypoints()
{
	mScatterWaypoints.push_back(new PathNode(12.0f, -14.5f));
	mScatterWaypoints.push_back(new PathNode(1.0f, -14.5f));
	mScatterWaypoints.push_back(new PathNode(1.0f, -11.5f));
	mScatterWaypoints.push_back(new PathNode(4.0f, -11.5f));
	mScatterWaypoints.push_back(new PathNode(4.0f, -8.5f));
	mScatterWaypoints.push_back(new PathNode(7.0f, -8.5f));
	mScatterWaypoints.push_back(new PathNode(7.0f, -11.5f));
	mScatterWaypoints.push_back(new PathNode(12.0f, -11.5f));
	mScatterWaypoints.push_back(new PathNode(12.0f, -14.5f));
}

void Inky::Update(float dt, bool powerUpActivated, Direction::DIRECTION facingState, XMFLOAT3 blinkyPos, int levelNumber, int pelletCounter)
{
	if (!isDead)
	{
		if (pelletCounter >= 30 && isIdle)
		{
			this->isIdle = false;
			mGhostStates = GHOST_STATES::SCATTER;
		}
		if (!isIdle)
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
				PathNode* offsetTile;
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
						if (facingState == Direction::DIRECTION::NORTH || facingState == Direction::DIRECTION::SOUTH)
						{
							//Offset tile = 2 spaces in PuckMan's facing
							offsetTile = new PathNode(round(MazeLoader::GetPacManData().at(0).pos.x),
								round(MazeLoader::GetPacManData().at(0).pos.z + (2 * Direction::getDirecitonVector(facingState).m128_f32[2])));
							//Draw a vector from Blinky's current position to the offset tile's position
							XMVECTOR targetTile = XMVectorSet(offsetTile->xPos - blinkyPos.x, 0.0f, offsetTile->zPos - blinkyPos.z, 0.0f);
							//Double the vector length extending forward, this is Inky's target
							targetTile = XMVectorScale(targetTile, 2.0f);

							//Clamp the X and Z value of the targetTile so it cannot choose a tile outside of the level
							float clampedX = MathHelper::Clamp((int)targetTile.m128_f32[0], 0, (int)(MazeLoader::GetMazeWidth()));
							float clampedZ = MathHelper::Clamp((int)targetTile.m128_f32[2], 0, (int)(MazeLoader::GetMazeHeight()));

							//Check if clampedZ is a valid tile, if not move the target tile one more in the opposite direction and try again
							int goalRow = (MazeLoader::GetMazeHeight()) - round(clampedZ + 15.5f);
							int goalCol = round(clampedX + 14.5f) - 1;
							while (MazeLoader::IsBlocked(goalRow, goalCol))
							{
								clampedX -= 1 * Direction::getDirecitonVector(facingState).m128_f32[0];
								clampedZ -= 1 * Direction::getDirecitonVector(facingState).m128_f32[2];
								if (MazeLoader::IsBlocked(clampedZ, clampedX))
								{
									break;
								}
								//goalRow = (MazeLoader::GetMazeHeight()) - round(clampedZ + 15.5f);
								//goalCol = round(clampedX + 13.5f);
							}

							PrePathFinding(this->mPos.x, this->mPos.z, round(MazeLoader::GetPacManData().at(0).pos.x), round(MazeLoader::GetPacManData().at(0).pos.z));
							if (PostPathFinding())
							{
								this->UpdateCurrentTweenPoint(dt);
							}
						}

						else if (facingState == Direction::DIRECTION::WEST || facingState == Direction::DIRECTION::EAST)
						{
							//Offset tile = 2 spaces in PuckMan's facing
							offsetTile = new PathNode(round(MazeLoader::GetPacManData().at(0).pos.x + (2 * Direction::getDirecitonVector(facingState).m128_f32[0])),
								round(MazeLoader::GetPacManData().at(0).pos.z));
							//Draw a vector from Blinky's current position to the offset tile's position
							XMVECTOR targetTile = XMVectorSet(offsetTile->xPos - blinkyPos.x, 0.0f, offsetTile->zPos - blinkyPos.z, 0.0f);
							//Double the vector length extending forward, this is Inky's target
							targetTile = XMVectorScale(targetTile, 2.0f);

							//Clamp the X and Z value of the targetTile so it cannot choose a tile outside of the level
							//Subract one from the highest value to zero base it
							float clampedX = MathHelper::Clamp((int)targetTile.m128_f32[0], 0, (int)(MazeLoader::GetMazeWidth()));
							float clampedZ = MathHelper::Clamp((int)targetTile.m128_f32[2], 0, (int)(MazeLoader::GetMazeHeight()));

							//Check if clampedZ is a valid tile, if not move the target tile one more in the opposite direction and try again
							//Because clampedZ and clampedX are already tile based, we can pass it into the function without conversion
							while (MazeLoader::IsBlocked(clampedZ, clampedX))
							{
								clampedX -= 1 * Direction::getDirecitonVector(facingState).m128_f32[0];
								clampedZ -= 1 * Direction::getDirecitonVector(facingState).m128_f32[2];
								if (MazeLoader::IsBlocked(clampedZ, clampedX))
								{
									break;
								}
								//goalRow = (MazeLoader::GetMazeHeight()) - round(clampedZ + 15.5f);
								//goalCol = round(clampedX + 13.5f);
							}

							PrePathFinding(this->mPos.x, this->mPos.z, round(MazeLoader::GetPacManData().at(0).pos.x), round(MazeLoader::GetPacManData().at(0).pos.z));
							if (PostPathFinding())
							{
								this->UpdateCurrentTweenPoint(dt);
							}
						}
						mPathNext += (1.0f / 10.0f);
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

void Inky::Reset()
{
	this->mGhostStates = GHOST_STATES::IDLE;
	mTweenPoints.clear();
	mChaseTimer = 0.0f;
	mScatterTimer = 0.0f;
	isIdle = true;
	firstChasePathDrawn = false;
	scatterPathDrawn = false;
	isLooping = false;
	reachedEnd = false;
	CleanUpNodesWaypoints();
}