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