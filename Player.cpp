#include "Player.h"

PuckMan::~PuckMan(void)
{
}

void PuckMan::Move(float dt, float mSpeed, Direction::DIRECTION direction)
{
	XMVECTOR pos = XMLoadFloat3(&mPos);
	XMVECTOR vel = XMLoadFloat3(&mVel);

	if (direction == Direction::DIRECTION::NORTH)
	{
		mFacing = Direction::DIRECTION::NORTH;
		vel.m128_f32[0] = 0.0f * dt;
		vel.m128_f32[1] = 0.0f * dt;
		vel.m128_f32[2] = 1.0f * dt;
		if (vel.m128_f32[2] < 0.00826695096f)
		{
			vel.m128_f32[2] = 0.00826695096f;
		}
	}

	if (direction == Direction::DIRECTION::SOUTH)
	{
		mFacing = Direction::DIRECTION::SOUTH;
		vel.m128_f32[0] = 0.0f * dt;
		vel.m128_f32[1] = 0.0f * dt;
		vel.m128_f32[2] = -1.0f * dt;
		if (vel.m128_f32[2] > -0.00826695096f)
		{
			vel.m128_f32[2] = -0.00826695096f;
		}
	}

	if (direction == Direction::DIRECTION::WEST)
	{
		mFacing = Direction::DIRECTION::WEST;
		vel.m128_f32[0] = -1.0f * dt;
		if (vel.m128_f32[0] > -0.00826695096f)
		{
			vel.m128_f32[0] = -0.00826695096f;
		}
		vel.m128_f32[1] = 0.0f * dt;
		vel.m128_f32[2] = 0.0f * dt;
	}

	if (direction == Direction::DIRECTION::EAST)
	{
		mFacing = Direction::DIRECTION::EAST;
		vel.m128_f32[0] = 1.0f * dt;
		if (vel.m128_f32[0] < 0.00826695096f)
		{
			vel.m128_f32[0] = 0.00826695096f;
		}

		vel.m128_f32[1] = 0.0f * dt;
		vel.m128_f32[2] = 0.0f * dt;
	}

	pos = pos +  (vel * mSpeed * dt);

	XMStoreFloat3(&mPos, pos);
	XMStoreFloat3(&mVel, vel);
}


//PuckManís Speed while eating dots :
//In First level PuckManís speed is 0.71 meters per second.
//In Second to Fourth level speed is 0.79 meters per second.
//In Fifth to Twentieth level speed is 0.87 meters per second.
//Levels Twenty - one plus speed is 0.79 meters per second.
//
//PuckManís Speed while NOT eating dots :
//In First level PuckManís speed is 0.8 meters per second.
//In Second to Fourth level speed is 0.9 meters per second.
//In Fifth to Twentieth level speed is 1.0 meters per second.
//Levels Twenty - one plus speed is 0.9 meters per second.
//
//PuckManís Speed while eating dots and ghost frightened :
//In First level PuckManís speed is 0.79 meters per second.
//In Second to Fourth level speed is 0.83 meters per second.
//In Fifth to Twentieth level speed is 0.87 meters per second.
//Levels Twenty - one plus speed is 0.87 meters per second.
//
//PuckManís Speed while NOT eating dots and ghost frightened :
//In First level PuckManís speed is 0.9 meters per second.
//In Second to Fourth level speed is 0.95 meters per second.
//In Fifth to Twentieth level speed is 1.0 meters per second.
//Levels Twenty - one plus speed is 1.0 meters per second.
//
float PuckMan::CalculateSpeed(int levelNumber, bool powerUpActivated)
{
	std::vector<MazeLoader::MazeElementSpecs> pacMans = MazeLoader::GetPacManData();
	XMVECTOR pos = XMLoadFloat3(&pacMans[0].pos);
	float posX = pacMans[0].pos.x;
	float posZ = pacMans[0].pos.z;

	//translate Puckmans Position to Pellet space
	int transX = (int)floor(posX + (MazeLoader::GetMazeWidth() * 0.5));
	int transZ = MazeLoader::GetMazeHeight() - (int)round(posZ + (MazeLoader::GetMazeHeight() * 0.5)); //invert the z
	int row = transZ;
	int col = transX;

	switch (mFacing)
	{
	case Direction::DIRECTION::NORTH:
		if (levelNumber == 1)
		{
			if (MazeLoader::IsPellet(row - 1, col))
			{
				mSpeed = PUCKMAN_SPEED * 0.71f; //eating pellet speed
			}
			if (!(MazeLoader::IsPellet(row - 1, col)))
			{
				mSpeed = PUCKMAN_SPEED * 0.8f; // no pellet to eat speed
			}
			if (MazeLoader::IsPellet(row - 1, col) && powerUpActivated)
			{
				mSpeed = PUCKMAN_SPEED * 0.79f; // eating pellet and ghost frightened speed
			}
			if (!MazeLoader::IsPellet(row - 1, col) && powerUpActivated)
			{
				mSpeed = PUCKMAN_SPEED * 0.9f; // no pellet to eat and ghost frightened speed
			}
			return mSpeed;
		}
		if (levelNumber > 1 && levelNumber < 5) // levels 2 through 4
		{
			if (MazeLoader::IsPellet(row - 1, col))
			{
				mSpeed = PUCKMAN_SPEED * 0.79f; //eating pellet speed
			}
			if (!(MazeLoader::IsPellet(row - 1, col)))
			{
				mSpeed = PUCKMAN_SPEED * 0.9f; // no pellet to eat speed
			}
			if (MazeLoader::IsPellet(row - 1, col) && powerUpActivated)
			{
				mSpeed = PUCKMAN_SPEED * 0.83f; // eating pellet and ghost frightened speed
			}
			if (!MazeLoader::IsPellet(row - 1, col) && powerUpActivated)
			{
				mSpeed = PUCKMAN_SPEED * 0.95f; // no pellet to eat and ghost frightened speed
			}
			return mSpeed;
		}
		if (levelNumber > 4 && levelNumber < 21) // levels 5 through 20
		{
			if (MazeLoader::IsPellet(row - 1, col))
			{
				mSpeed = PUCKMAN_SPEED * 0.87f; //eating pellet speed
			}
			if (!(MazeLoader::IsPellet(row - 1, col)))
			{
				mSpeed = PUCKMAN_SPEED * 1.0f; // no pellet to eat speed
			}
			if (MazeLoader::IsPellet(row - 1, col) && powerUpActivated)
			{
				mSpeed = PUCKMAN_SPEED * 0.87f; // eating pellet and ghost frightened speed
			}
			if (!MazeLoader::IsPellet(row - 1, col) && powerUpActivated)
			{
				mSpeed = PUCKMAN_SPEED * 1.0f; // no pellet to eat and ghost frightened speed
			}
			return mSpeed;
		}
		if (levelNumber >= 21) // Level 21 and up
		{
			if (MazeLoader::IsPellet(row - 1, col))
			{
				mSpeed = PUCKMAN_SPEED * 0.79f; //eating pellet speed
			}
			if (!(MazeLoader::IsPellet(row - 1, col)))
			{
				mSpeed = PUCKMAN_SPEED * 0.9f; // no pellet to eat speed
			}
			if (MazeLoader::IsPellet(row - 1, col) && powerUpActivated)
			{
				mSpeed = PUCKMAN_SPEED * 0.87f; // eating pellet and ghost frightened speed
			}
			if (!MazeLoader::IsPellet(row - 1, col) && powerUpActivated)
			{
				mSpeed = PUCKMAN_SPEED * 1.0f; // no pellet to eat and ghost frightened speed
			}
			return mSpeed;
		}

		break;

	case Direction::DIRECTION::SOUTH:
		if (levelNumber == 1)
		{
			if (MazeLoader::IsPellet(row + 1, col))
			{
				mSpeed = PUCKMAN_SPEED * 0.71f; //eating pellet speed
			}
			if (!(MazeLoader::IsPellet(row + 1, col)))
			{
				mSpeed = PUCKMAN_SPEED * 0.8f; // no pellet to eat speed
			}
			if (MazeLoader::IsPellet(row + 1, col) && powerUpActivated)
			{
				mSpeed = PUCKMAN_SPEED * 0.79f; // eating pellet and ghost frightened speed
			}
			if (!MazeLoader::IsPellet(row + 1, col) && powerUpActivated)
			{
				mSpeed = PUCKMAN_SPEED * 0.9f; // no pellet to eat and ghost frightened speed
			}
			return mSpeed;
		}
		if (levelNumber > 1 && levelNumber < 5) // levels 2 through 4
		{
			if (MazeLoader::IsPellet(row + 1, col))
			{
				mSpeed = PUCKMAN_SPEED * 0.79f; //eating pellet speed
			}
			if (!(MazeLoader::IsPellet(row + 1, col)))
			{
				mSpeed = PUCKMAN_SPEED * 0.9f; // no pellet to eat speed
			}
			if (MazeLoader::IsPellet(row + 1, col) && powerUpActivated)
			{
				mSpeed = PUCKMAN_SPEED * 0.83f; // eating pellet and ghost frightened speed
			}
			if (!MazeLoader::IsPellet(row + 1, col) && powerUpActivated)
			{
				mSpeed = PUCKMAN_SPEED * 0.95f; // no pellet to eat and ghost frightened speed
			}
			return mSpeed;
		}
		if (levelNumber > 4 && levelNumber < 21) // levels 5 through 20
		{
			if (MazeLoader::IsPellet(row + 1, col))
			{
				mSpeed = PUCKMAN_SPEED * 0.87f; //eating pellet speed
			}
			if (!(MazeLoader::IsPellet(row + 1, col)))
			{
				mSpeed = PUCKMAN_SPEED * 1.0f; // no pellet to eat speed
			}
			if (MazeLoader::IsPellet(row + 1, col) && powerUpActivated)
			{
				mSpeed = PUCKMAN_SPEED * 0.87f; // eating pellet and ghost frightened speed
			}
			if (!MazeLoader::IsPellet(row + 1, col) && powerUpActivated)
			{
				mSpeed = PUCKMAN_SPEED * 1.0f; // no pellet to eat and ghost frightened speed
			}
			return mSpeed;
		}
		if (levelNumber >= 21) // Level 21 and up
		{
			if (MazeLoader::IsPellet(row + 1, col))
			{
				mSpeed = PUCKMAN_SPEED * 0.79f; //eating pellet speed
			}
			if (!(MazeLoader::IsPellet(row + 1, col)))
			{
				mSpeed = PUCKMAN_SPEED * 0.9f; // no pellet to eat speed
			}
			if (MazeLoader::IsPellet(row + 1, col) && powerUpActivated)
			{
				mSpeed = PUCKMAN_SPEED * 0.87f; // eating pellet and ghost frightened speed
			}
			if (!MazeLoader::IsPellet(row + 1, col) && powerUpActivated)
			{
				mSpeed = PUCKMAN_SPEED * 1.0f; // no pellet to eat and ghost frightened speed
			}
			return mSpeed;
		}

		break;

	case Direction::DIRECTION::EAST:
		if (levelNumber == 1)
		{
			if (MazeLoader::IsPellet(row, col + 1))
			{
				mSpeed = PUCKMAN_SPEED * 0.71f; //eating pellet speed
			}
			if (!(MazeLoader::IsPellet(row, col + 1)))
			{
				mSpeed = PUCKMAN_SPEED * 0.8f; // no pellet to eat speed
			}
			if (MazeLoader::IsPellet(row, col + 1) && powerUpActivated)
			{
				mSpeed = PUCKMAN_SPEED * 0.79f; // eating pellet and ghost frightened speed
			}
			if (!MazeLoader::IsPellet(row, col + 1) && powerUpActivated)
			{
				mSpeed = PUCKMAN_SPEED * 0.9f; // no pellet to eat and ghost frightened speed
			}
			return mSpeed;
		}
		if (levelNumber > 1 && levelNumber < 5) // levels 2 through 4
		{
			if (MazeLoader::IsPellet(row, col + 1))
			{
				mSpeed = PUCKMAN_SPEED * 0.79f; //eating pellet speed
			}
			if (!(MazeLoader::IsPellet(row, col + 1)))
			{
				mSpeed = PUCKMAN_SPEED * 0.9f; // no pellet to eat speed
			}
			if (MazeLoader::IsPellet(row, col + 1) && powerUpActivated)
			{
				mSpeed = PUCKMAN_SPEED * 0.83f; // eating pellet and ghost frightened speed
			}
			if (!MazeLoader::IsPellet(row, col + 1) && powerUpActivated)
			{
				mSpeed = PUCKMAN_SPEED * 0.95f; // no pellet to eat and ghost frightened speed
			}
			return mSpeed;
		}
		if (levelNumber > 4 && levelNumber < 21) // levels 5 through 20
		{
			if (MazeLoader::IsPellet(row, col + 1))
			{
				mSpeed = PUCKMAN_SPEED * 0.87f; //eating pellet speed
			}
			if (!(MazeLoader::IsPellet(row, col + 1)))
			{
				mSpeed = PUCKMAN_SPEED * 1.0f; // no pellet to eat speed
			}
			if (MazeLoader::IsPellet(row, col + 1) && powerUpActivated)
			{
				mSpeed = PUCKMAN_SPEED * 0.87f; // eating pellet and ghost frightened speed
			}
			if (!MazeLoader::IsPellet(row, col + 1) && powerUpActivated)
			{
				mSpeed = PUCKMAN_SPEED * 1.0f; // no pellet to eat and ghost frightened speed
			}
			return mSpeed;
		}
		if (levelNumber >= 21) // Level 21 and up
		{
			if (MazeLoader::IsPellet(row, col + 1))
			{
				mSpeed = PUCKMAN_SPEED * 0.79f; //eating pellet speed
			}
			if (!(MazeLoader::IsPellet(row, col + 1)))
			{
				mSpeed = PUCKMAN_SPEED * 0.9f; // no pellet to eat speed
			}
			if (MazeLoader::IsPellet(row, col + 1) && powerUpActivated)
			{
				mSpeed = PUCKMAN_SPEED * 0.87f; // eating pellet and ghost frightened speed
			}
			if (!MazeLoader::IsPellet(row, col + 1) && powerUpActivated)
			{
				mSpeed = PUCKMAN_SPEED * 1.0f; // no pellet to eat and ghost frightened speed
			}
			return mSpeed;
		}

		break;

	case Direction::DIRECTION::WEST:
		if (levelNumber == 1)
		{
			if (MazeLoader::IsPellet(row, col - 1))
			{
				mSpeed = PUCKMAN_SPEED * 0.71f; //eating pellet speed
			}
			if (!(MazeLoader::IsPellet(row, col - 1)))
			{
				mSpeed = PUCKMAN_SPEED * 0.8f; // no pellet to eat speed
			}
			if (MazeLoader::IsPellet(row, col - 1) && powerUpActivated)
			{
				mSpeed = PUCKMAN_SPEED * 0.79f; // eating pellet and ghost frightened speed
			}
			if (!MazeLoader::IsPellet(row, col - 1) && powerUpActivated)
			{
				mSpeed = PUCKMAN_SPEED * 0.9f; // no pellet to eat and ghost frightened speed
			}
			return mSpeed;
		}
		if (levelNumber > 1 && levelNumber < 5) // levels 2 through 4
		{
			if (MazeLoader::IsPellet(row, col - 1))
			{
				mSpeed = PUCKMAN_SPEED * 0.79f; //eating pellet speed
			}
			if (!(MazeLoader::IsPellet(row, col - 1)))
			{
				mSpeed = PUCKMAN_SPEED * 0.9f; // no pellet to eat speed
			}
			if (MazeLoader::IsPellet(row, col - 1) && powerUpActivated)
			{
				mSpeed = PUCKMAN_SPEED * 0.83f; // eating pellet and ghost frightened speed
			}
			if (!MazeLoader::IsPellet(row, col - 1) && powerUpActivated)
			{
				mSpeed = PUCKMAN_SPEED * 0.95f; // no pellet to eat and ghost frightened speed
			}
			return mSpeed;
		}
		if (levelNumber > 4 && levelNumber < 21) // levels 5 through 20
		{
			if (MazeLoader::IsPellet(row, col - 1))
			{
				mSpeed = PUCKMAN_SPEED * 0.87f; //eating pellet speed
			}
			if (!(MazeLoader::IsPellet(row, col - 1)))
			{
				mSpeed = PUCKMAN_SPEED * 1.0f; // no pellet to eat speed
			}
			if (MazeLoader::IsPellet(row, col - 1) && powerUpActivated)
			{
				mSpeed = PUCKMAN_SPEED * 0.87f; // eating pellet and ghost frightened speed
			}
			if (!MazeLoader::IsPellet(row, col - 1) && powerUpActivated)
			{
				mSpeed = PUCKMAN_SPEED * 1.0f; // no pellet to eat and ghost frightened speed
			}
			return mSpeed;
		}
		if (levelNumber >= 21) // Level 21 and up
		{
			if (MazeLoader::IsPellet(row, col - 1))
			{
				mSpeed = PUCKMAN_SPEED * 0.79f; //eating pellet speed
			}
			if (!(MazeLoader::IsPellet(row, col - 1)))
			{
				mSpeed = PUCKMAN_SPEED * 0.9f; // no pellet to eat speed
			}
			if (MazeLoader::IsPellet(row, col - 1) && powerUpActivated)
			{
				mSpeed = PUCKMAN_SPEED * 0.87f; // eating pellet and ghost frightened speed
			}
			if (!MazeLoader::IsPellet(row, col - 1) && powerUpActivated)
			{
				mSpeed = PUCKMAN_SPEED * 1.0f; // no pellet to eat and ghost frightened speed
			}
			return mSpeed;
		}
		return mSpeed;
		break;
	}
	return mSpeed;
}