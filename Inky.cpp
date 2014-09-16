#include "Inky.h"


Inky::Inky()
{
}


Inky::~Inky()
{
}

void Inky::Update()
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
		//Targets 2 tiles in from of Pac-Man in the direction of his facing, make a vector from Blinky's CURRENT tile to the center of the offset tile
		//Double the length of the vector by extending it out just as far beyond the offset tile, this is Inky's target
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