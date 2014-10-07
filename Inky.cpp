#include "Inky.h"

Inky::Inky(FXMVECTOR pos, FXMVECTOR vel, float radius) : Ghost(pos, vel, radius)
{
	XMStoreFloat3(&mPos, pos);
	XMStoreFloat3(&mVel, vel);
	this->mGhostStates = GHOST_STATES::SCATTER;
	this->mScatterTile.x = 12;
	this->mScatterTile.z = -14.5f;
	this->waypointIterator = 0;
	this->mScatterTimer = 0;
	this->mChaseTimer = 0;

	//Draw the path to his scatter area prior to the start of the game to prevent bottlenecks
	mStart = new PathNode((int)this->mPos.x, (int)this->mPos.z);
	mGoal = new PathNode((int)this->mScatterTile.x, (int)this->mScatterTile.z);
	waypoints = test.FindPath(mStart, mGoal);
	scatterPathDrawn = false;
	firstChasePathDrawn = true;
}

Inky::~Inky()
{
}

const XMFLOAT3 Inky::mScatterWaypoints[MAX_WAYPOINTS] =
{
	{ XMFLOAT3(12.0f, 0.0f, -13.5f) },
	{ XMFLOAT3(11.0f, 0.0f, -13.5f) },
	{ XMFLOAT3(10.0f, 0.0f, -13.5f) },
	{ XMFLOAT3(9.0f, 0.0f, -13.5f) },
	{ XMFLOAT3(8.0f, 0.0f, -13.5f) },
	{ XMFLOAT3(7.0f, 0.0f, -13.5f) },
	{ XMFLOAT3(6.0f, 0.0f, -13.5f) },
	{ XMFLOAT3(5.0f, 0.0f, -13.5f) },
	{ XMFLOAT3(4.0f, 0.0f, -13.5f) },
	{ XMFLOAT3(3.0f, 0.0f, -13.5f) },
	{ XMFLOAT3(2.0f, 0.0f, -13.5f) },
	{ XMFLOAT3(1.0f, 0.0f, -13.5f) },
	{ XMFLOAT3(1.0f, 0.0f, -12.5f) },
	{ XMFLOAT3(1.0f, 0.0f, -11.5f) },
	{ XMFLOAT3(1.0f, 0.0f, -10.5f) },
	{ XMFLOAT3(2.0f, 0.0f, -10.5f) },
	{ XMFLOAT3(3.0f, 0.0f, -10.5f) },
	{ XMFLOAT3(4.0f, 0.0f, -10.5f) },
	{ XMFLOAT3(4.0f, 0.0f, -9.5f) },
	{ XMFLOAT3(4.0f, 0.0f, -8.5f) },
	{ XMFLOAT3(4.0f, 0.0f, -7.5f) },
	{ XMFLOAT3(5.0f, 0.0f, -7.5f) },
	{ XMFLOAT3(6.0f, 0.0f, -7.5f) },
	{ XMFLOAT3(7.0f, 0.0f, -7.5f) },
	{ XMFLOAT3(7.0f, 0.0f, -8.5f) },
	{ XMFLOAT3(7.0f, 0.0f, -9.5f) },
	{ XMFLOAT3(7.0f, 0.0f, -10.5f) },
	{ XMFLOAT3(8.0f, 0.0f, -10.5f) },
	{ XMFLOAT3(9.0f, 0.0f, -10.5f) },
	{ XMFLOAT3(10.0f, 0.0f, -10.5f) },
	{ XMFLOAT3(11.0f, 0.0f, -10.5f) },
	{ XMFLOAT3(12.0f, 0.0f, -10.5f) },
	{ XMFLOAT3(12.0f, 0.0f, -11.5f) },
	{ XMFLOAT3(12.0f, 0.0f, -12.5f) },
};

void Inky::Update(float dt, bool powerUpActivated, Direction::DIRECTION facingState, XMFLOAT3 blinkyPos, int levelNumber, int pelletCounter)
{
	/*if (powerUpActivated)
	{
		this->mGhostStates = GHOST_STATES::FRIGHTENED;
	}
	else
	{
		this->mGhostStates = GHOST_STATES::CHASE;
	}*/
	
	if (pelletCounter == 30)
	{
		this->isIdle = false;
		mGhostStates = GHOST_STATES::SCATTER;
	}
	if (!isIdle)
	{
		switch (mGhostStates)
		{
		case SCATTER:
			if (!scatterPathDrawn)
			{
				mStart = new PathNode((int)this->mPos.x, (int)this->mPos.z);
				mGoal = new PathNode((int)this->mScatterTile.x, (int)this->mScatterTile.z);
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
						if (this->mCurrWaypointIndex == 34)
						{
							this->mCurrWaypointIndex = 0;
						}
					}
				}
			}

			mScatterTimer += 5.7142 * dt; //dt currently takes (without mutliplying) 40 seconds to reach 7.0f, 5.7142 comes from 40 / 7 to get the number as accurate as possible.
			if (mScatterTimer >= 7.0f)
			{
				this->mGhostStates = GHOST_STATES::CHASE;
				mScatterTimer = 0.0f;
				scatterPathDrawn = false;
				firstChasePathDrawn = false;
				this->mCurrWaypointIndex = 0;
				this->waypointIterator = 0;
				//Draw an initial path to PuckMan so Inky has somewhere to go
				mStart = new PathNode((int)this->mPos.x, (int)this->mPos.z);
				mGoal = new PathNode((int)round(MazeLoader::GetPacManData().at(0).pos.x), (int)round(MazeLoader::GetPacManData().at(0).pos.z));
				waypoints = test.FindPath(mStart, mGoal);
			}
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
			PathNode* offsetTile;
			if (!firstChasePathDrawn)
			{
				int row = MazeLoader::GetMazeHeight() - (int)round(this->mPos.z + 15.5f);
				int col = (int)round(this->mPos.x + 14.5f) - 1;
				if (MazeLoader::IsDivergent(row, col))
				{
					if (facingState == Direction::DIRECTION::NORTH || facingState == Direction::DIRECTION::SOUTH)
					{
						//Offset tile = 2 spaces in PuckMan's facing
						offsetTile = new PathNode((int)round(MazeLoader::GetPacManData().at(0).pos.x),
									 (int)round(MazeLoader::GetPacManData().at(0).pos.z + (2 * Direction::getDirecitonVector(facingState).m128_f32[2])));
						//Draw a vector from Blinky's current position to the offset tile's position
						XMVECTOR targetTile = XMVectorSet(blinkyPos.x - offsetTile->xPos, 0.0f, blinkyPos.z - offsetTile->zPos, 0.0f);
						//Double the vector length extending forward, this is Inky's target
						targetTile = XMVectorScale(targetTile, 2.0f);

						//Clamp the X and Z value of the targetTile so it cannot choose a tile outside of the level
						float clampedX = clamp(targetTile.m128_f32[0], MazeLoader::GetMazeWidth() - MazeLoader::GetMazeWidth(), MazeLoader::GetMazeWidth());
						float clampedZ = clamp(targetTile.m128_f32[2], MazeLoader::GetMazeHeight() - MazeLoader::GetMazeHeight(), MazeLoader::GetMazeHeight());

						//Check if clampedZ is a valid tile, if not move the target tile one more in the opposite direction and try again
						int goalRow = (MazeLoader::GetMazeHeight()) - (int)round(clampedZ + 15.5f);
						int goalCol = (int)round(clampedX + 14.5f) - 1;
						while (MazeLoader::IsBlocked(goalRow, goalCol))
						{
							clampedX -= 1 * Direction::getDirecitonVector(facingState).m128_f32[0];
							clampedZ -= 1 * Direction::getDirecitonVector(facingState).m128_f32[2];
							if (!MazeLoader::IsBlocked(clampedZ, clampedX))
							{
								break;
							}
							/*goalRow = (MazeLoader::GetMazeHeight()) - (int)round(clampedZ + 15.5f);
							goalCol = (int)round(clampedX + 13.5f);*/
						}

						mStart = new PathNode((int)this->mPos.x, (int)this->mPos.z);
						mGoal = new PathNode((int)clampedX, (int)clampedZ);
						waypoints = test.FindPath(mStart, mGoal);
					}

					else if (facingState == Direction::DIRECTION::WEST || facingState == Direction::DIRECTION::EAST)
					{
						//Offset tile = 2 spaces in PuckMan's facing
						offsetTile = new PathNode((int)round(MazeLoader::GetPacManData().at(0).pos.x + (2 * Direction::getDirecitonVector(facingState).m128_f32[0])),
												  (int)round(MazeLoader::GetPacManData().at(0).pos.z));
						//Draw a vector from Blinky's current position to the offset tile's position
						XMVECTOR targetTile = XMVectorSet(blinkyPos.x - offsetTile->xPos, 0.0f, blinkyPos.z - offsetTile->zPos, 0.0f);
						//Double the vector length extending forward, this is Inky's target
						targetTile = XMVectorScale(targetTile, 2.0f);

						//Clamp the X and Z value of the targetTile so it cannot choose a tile outside of the level
							//Subract one from the highest value to zero base it
						float clampedX = clamp(targetTile.m128_f32[0], MazeLoader::GetMazeWidth() - MazeLoader::GetMazeWidth(), MazeLoader::GetMazeWidth() - 1);
						float clampedZ = clamp(targetTile.m128_f32[2], MazeLoader::GetMazeHeight() - MazeLoader::GetMazeHeight(), MazeLoader::GetMazeHeight() - 1);

						//Check if clampedZ is a valid tile, if not move the target tile one more in the opposite direction and try again
							//Because clampedZ and clampedX are already tile based, we can pass it into the function without conversion
						while (MazeLoader::IsBlocked(clampedZ, clampedX))
						{
							clampedX -= 1 * Direction::getDirecitonVector(facingState).m128_f32[0];
							clampedZ -= 1 * Direction::getDirecitonVector(facingState).m128_f32[0];
							if (!MazeLoader::IsBlocked(clampedZ, clampedX))
							{
								break;
							}
							//goalRow = (MazeLoader::GetMazeHeight()) - (int)round(clampedZ + 15.5f);
							//goalCol = (int)round(clampedX + 13.5f);
						}

						mStart = new PathNode((int)this->mPos.x, (int)this->mPos.z);
						mGoal = new PathNode((int)clampedX, (int)clampedZ);
						waypoints = test.FindPath(mStart, mGoal);
					}
				}

				if (waypoints.size() != 0)
				{
					if (waypointIterator < waypoints.size())
					{
						this->setPos(XMVectorSet(this->waypoints.at(waypointIterator)->xPos, mPos.y, this->waypoints.at(waypointIterator)->zPos, 0.0f));
						waypointIterator++;
					}
					else if (waypointIterator >= waypoints.size())
					{
						waypointIterator = 0;
					}
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
		case DEAD:
			//Spawn in box
			//XMVectorSet(-2.0f, 0.75f, 0.0f, 0.0f)
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
}

void Inky::Reset()
{
	this->mGhostStates = GHOST_STATES::IDLE;
	waypointIterator = 0;
	mCurrWaypointIndex = 0;
	scatterPathDrawn = false;
	isLooping = false;
	isIdle = true;
}