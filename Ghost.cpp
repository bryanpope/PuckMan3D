#include "Ghost.h"
#include "GeometryGenerator.h"
#include "Vertex.h"

Ghost::Ghost()
{
}

Ghost::Ghost(FXMVECTOR pos, FXMVECTOR vel, float radius)
{
	XMStoreFloat3(&mPos, pos);
	XMStoreFloat3(&mVel, vel);
	mRadius = radius;
}

Ghost::~Ghost()
{
}

void Ghost::Update()
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
		//Pathfinding!
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