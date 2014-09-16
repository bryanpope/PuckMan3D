#include "Clyde.h"


Clyde::Clyde()
{
}


Clyde::~Clyde()
{
}

void Clyde::Update()
{
	/*switch (GHOST_STATES)
	{
	case SCATTER:
		if(level 1)
		{
			XMVECTOR vel = XMLoadFloat3(&mVel);
			vel = vel * 0.75;
			XMStoreFloat3(&mVel, vel);
			break;
		}
		else if(level 2-4)
		{
			XMVECTOR vel = XMLoadFloat3(&mVel);
			vel = vel * 0.85;
			XMStoreFloat3(&mVel, vel);
			break;
		}
		else if(level 5+)
		{
			XMVECTOR vel = XMLoadFloat3(&mVel);
			vel = vel * 0.95;
			XMStoreFloat3(&mVel, vel);
			break;
		}
	case CHASE:
		//Calculate the Euclidean distance between Clyde's tile and Pac-Man's tile, if the distance between them is 8+ tiles, target Pac-Man
			//If the distance is less than eight tiles, Clyde enters Scatter mode until he is far enough away to target Pac-Man again
		break;
	case FRIGHTENED:
		if(level 1)
		{
			XMVECTOR vel = XMLoadFloat3(&mVel);
			vel = vel * 0.50;
			XMStoreFloat3(&mVel, vel);
			break;
		}
		else if(level 2-4)
		{
			XMVECTOR vel = XMLoadFloat3(&mVel);
			vel = vel * 0.55;
			XMStoreFloat3(&mVel, vel);
			break;
		}
		else if(level 5+)
		{
			XMVECTOR vel = XMLoadFloat3(&mVel);
			vel = vel * 0.60;
			XMStoreFloat3(&mVel, vel);
			break;
		}
	case DEAD:
		//Spawn in box
		break;
	case IN_TUNNEL:
		if(level 1)
		{
			XMVECTOR vel = XMLoadFloat3(&mVel);
			vel = vel * 0.40;
			XMStoreFloat3(&mVel, vel);
			break;
		}
		else if(level 2-4)
		{
			XMVECTOR vel = XMLoadFloat3(&mVel);
			vel = vel * 0.45;
			XMStoreFloat3(&mVel, vel);
			break;
		}
		else if(level 5+)
		{
			XMVECTOR vel = XMLoadFloat3(&mVel);
			vel = vel * 0.50;
			XMStoreFloat3(&mVel, vel);
			break;
		}
	}*/
}