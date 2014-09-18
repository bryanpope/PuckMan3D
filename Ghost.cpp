#include "Ghost.h"

Ghost::Ghost()
{
}

Ghost::Ghost(UINT row, UINT col, FXMVECTOR vel, float radius)
{
	mRow = row;
	mCol = col;
	XMStoreFloat3(&mVel, vel);
	mRadius = radius;
}

Ghost::~Ghost()
{
}

void Ghost::Update()
{
	
}