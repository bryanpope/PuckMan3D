#include "Pinky.h"

Pinky::Pinky(FXMVECTOR pos, float radius) : Ghost(pos, radius)
{
	XMStoreFloat3(&mPos, pos);
	LoadScatterWaypoints();
	this->mGhostStates = GHOST_STATES::SCATTER;
	this->waypointIterator = 0;
	this->mScatterTimer = 0;
	this->mChaseTimer = 0;

	//Draw the path to his scatter area prior to the start of the game to prevent bottlenecks
	mStart = new PathNode(this->mPos.x, this->mPos.z);
	mGoal = new PathNode(this->mScatterTile.x, this->mScatterTile.z);
	mWaypoints = path.FindPath(mStart, mGoal);
	scatterPathDrawn = true;
}

Pinky::~Pinky()
{
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
	/*if (powerUpActivated)
	{
		this->mGhostStates = GHOST_STATES::FRIGHTENED;
	}
	else
	{
		this->mGhostStates = GHOST_STATES::CHASE;
	}*/

	//this->UpdateCurrentTweenPoint(dt);

	switch (mGhostStates)
	{
	case SCATTER:
		SetSpeed(levelNumber, GHOST_STATES::SCATTER);
		if (!scatterPathDrawn)
		{
			mStart = new PathNode(this->mPos.x, this->mPos.z);
			mGoal = new PathNode(this->mScatterWaypoints[0]->xPos, this->mScatterWaypoints[0]->zPos);
			mWaypoints = path.FindPath(mStart, mGoal);
			this->SetWayPoints(mWaypoints);
			this->UpdateCurrentTweenPoint(dt);
			scatterPathDrawn = true;
			waypointIterator = 0;
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
		/*if (!powerUpActivated)
		{
			mScatterTimer += 5.7142 * dt; //dt currently takes (without mutliplying) 40 seconds to reach 7.0f, 5.7142 comes from 40 / 7 to get the number as accurate as possible.
			if (mScatterTimer >= 7.0f)
			{
				this->mGhostStates = GHOST_STATES::CHASE;
				mScatterTimer = 0.0f;
				scatterPathDrawn = false;
				this->mCurrWaypointIndex = 0;
				this->waypointIterator = 0;
				waypoints.clear();
			}
		}
		else if (powerUpActivated)
		{
			std::cout << mScatterTimer << std::endl;
			mScatterTimer += 8.0f * dt; //dt currently takes (without mutliplying) 40 seconds to reach 5.0f, 8 comes from 40 / 5 to get the number as accurate as possible.
			if (mScatterTimer >= 5.0f)
			{
				this->mGhostStates = GHOST_STATES::CHASE;
				mScatterTimer = 0.0f;
				scatterPathDrawn = false;
				this->mCurrWaypointIndex = 0;
				this->waypointIterator = 0;
				waypoints.clear();
			}
		}*/
		break;
	case CHASE:
		SetSpeed(levelNumber, GHOST_STATES::CHASE);
		if (powerUpActivated)
		{
			this->mGhostStates = GHOST_STATES::SCATTER;
			mChaseTimer = 0.0f;
			waypointIterator = 0;
			firstChasePathDrawn = false;
			scatterPathDrawn = false;
			mWaypoints.clear();
		}
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
					while(MazeLoader::IsBlocked(goalRow, goalCol))
					{
						clampedZ -= 1 * Direction::getDirecitonVector(facingState).m128_f32[2];
						goalRow = (MazeLoader::GetMazeHeight()) - round(clampedZ + 15.5f);
						goalCol = (round(MazeLoader::GetPacManData().at(0).pos.x + 14.5f) - 1);
					}

					mStart = new PathNode(this->mPos.x, this->mPos.z);
					mGoal = new PathNode(round(MazeLoader::GetPacManData().at(0).pos.x), clampedZ);
					mWaypoints = path.FindPath(mStart, mGoal);
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

					mStart = new PathNode(this->mPos.x, this->mPos.z);
					mGoal = new PathNode(clampedX, round(MazeLoader::GetPacManData().at(0).pos.z));
					mWaypoints = path.FindPath(mStart, mGoal);
				}
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
			scatterPathDrawn = false;
			mWaypoints.clear();
		}

		break;
	case FRIGHTENED:
		SetSpeed(levelNumber, GHOST_STATES::FRIGHTENED);
		break;
	}
}

void Pinky::Reset()
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