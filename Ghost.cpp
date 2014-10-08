#include "Ghost.h"

Ghost::Ghost()
{
}

Ghost::Ghost(FXMVECTOR pos, float radius)
{
	XMStoreFloat3(&mPos, pos);
	//XMStoreFloat3(&mVel, vel);
	mVel = XMFLOAT3(1000.0f, 0.0f, 1000.0f);
	mRadius = radius;
	mSpeed = 1.0f;
}

Ghost::~Ghost()
{
}

void Ghost::Update()
{}

void Ghost::SetSpeed(int levelCounter, GHOST_STATES ghostState)
{
	if (ghostState == GHOST_STATES::CHASE || ghostState == GHOST_STATES::SCATTER)
	{
		if (levelCounter == 1)
		{
			mSpeed = GHOST_SPEED * 0.75f;
		}
		else if (levelCounter >= 2 || levelCounter <= 4)
		{
			mSpeed = GHOST_SPEED * 0.85f;
		}
		else if (levelCounter >= 5)
		{
			mSpeed = GHOST_SPEED * 0.95f;
		}
	}
	else if (ghostState == GHOST_STATES::FRIGHTENED)
	{
		if (levelCounter == 1)
		{
			mSpeed = GHOST_SPEED * 0.50f;
		}
		else if (levelCounter >= 2 || levelCounter <= 4)
		{
			mSpeed = GHOST_SPEED * 0.55f;
		}
		else if (levelCounter >= 5)
		{
			mSpeed = GHOST_SPEED * 0.60f;
		}
	}
}

void Ghost::UpdateCurrentTweenPoint(float dt)
{
	XMVECTOR pos = XMLoadFloat3(&mCurrTweenPoint);
	UINT tIndex = mCurrTweenIndex;
	XMFLOAT3 vector = mTweenPoints[tIndex].vector;
	XMVECTOR vel = XMLoadFloat3(&vector);
	pos = pos + (vel * dt * mSpeed);
	XMStoreFloat3(&mCurrTweenPoint, pos);
	if (abs(vector.x) == 1.0f && vector.z == 0.0f)
	{
		if (mTweenPoints[tIndex].endPosX > mTweenPoints[tIndex].startPosX)
		{
			if (mCurrTweenPoint.x >= mTweenPoints[tIndex].endPosX)
			{
				SnapTweenPoint();
				++mCurrTweenIndex;
			}
		}
		else
		{
			if (mCurrTweenPoint.x <= mTweenPoints[tIndex].endPosX)
			{
				SnapTweenPoint();
				++mCurrTweenIndex;
			}
		}
	}
	else  // vector.x == 0.0f && vector.z == 1.0f;
	{
		if (mTweenPoints[tIndex].endPosZ > mTweenPoints[tIndex].startPosZ)
		{
			if (mCurrTweenPoint.z >= mTweenPoints[tIndex].endPosZ)
			{
				SnapTweenPoint();
				++mCurrTweenIndex;
			}
		}
		else
		{
			if (mCurrTweenPoint.z <= mTweenPoints[tIndex].endPosZ)
			{
				SnapTweenPoint();
				++mCurrTweenIndex;
			}
		}
	}
	if (mCurrTweenIndex >= mTweenPoints.size())
	{
		mCurrTweenIndex = 0;
		mCurrTweenPoint.x = mTweenPoints[0].startPosX;
		mCurrTweenPoint.z = mTweenPoints[0].startPosZ;
		reachedEnd = true;
	}
}

void Ghost::SnapTweenPoint()
{
	mCurrTweenPoint.x = mTweenPoints[mCurrTweenIndex].endPosX;
	mCurrTweenPoint.z = mTweenPoints[mCurrTweenIndex].endPosZ;
}

void Ghost::SetWayPoints(std::vector<PathNode*> wayP)
{
	//mWaypoints = wayP;

	mTweenPoints.clear();
	mTweenPoints.push_back({ wayP[0]->xPos, wayP[0]->zPos, 0, 0, 0, XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f });
	UINT lastAdd = 0;
	XMFLOAT2 lastSame = XMFLOAT2(0.0f, 0.0f);

	for (int i = 1; i < wayP.size(); ++i)
	{
		if (lastSame.x == 0.0f && lastSame.y == 0.0f)
		{
			if (wayP[i]->xPos == mTweenPoints[lastAdd].startPosX)
			{
				lastSame = XMFLOAT2(1.0f, 0.0f);
				continue;
			}
			if (wayP[i]->zPos == mTweenPoints[lastAdd].startPosZ)
			{
				lastSame = XMFLOAT2(0.0f, 1.0f);
				continue;
			}
			//mTweenPoints.push_back({ wayP[i]->xPos, wayP[i]->zPos, 0.0f });
			//++lastAdd;
			//continue;
		}
		if (lastSame.x == 1.0f && lastSame.y == 0.0f)
		{
			if (wayP[i]->xPos == mTweenPoints[lastAdd].startPosX)
			{
				continue;
			}
			lastSame = XMFLOAT2(0.0f, 0.0f);
			mTweenPoints[lastAdd] = { mTweenPoints[lastAdd].startPosX, mTweenPoints[lastAdd].startPosZ, wayP[i - 1]->xPos, wayP[i - 1]->zPos,
				abs(mTweenPoints[lastAdd].startPosZ - wayP[i - 1]->zPos), XMFLOAT3(0.0f, 0.0f, wayP[i - 1]->zPos > mTweenPoints[lastAdd].startPosZ ? 1.0f : -1.0f), 0.0f };
			mTweenPoints.push_back({ wayP[i - 1]->xPos, wayP[i - 1]->zPos, 0, 0, 0, XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f });
			++lastAdd;
			--i;
			continue;
		}
		if (lastSame.x == 0.0f && lastSame.y == 1.0f)
		{
			if (wayP[i]->zPos == mTweenPoints[lastAdd].startPosZ)
			{
				continue;
			}
			lastSame = XMFLOAT2(0.0f, 0.0f);
			mTweenPoints[lastAdd] = { mTweenPoints[lastAdd].startPosX, mTweenPoints[lastAdd].startPosZ, wayP[i - 1]->xPos, wayP[i - 1]->zPos,
				abs(mTweenPoints[lastAdd].startPosX - wayP[i - 1]->xPos), XMFLOAT3(wayP[i - 1]->xPos > mTweenPoints[lastAdd].startPosX ? 1.0f : -1.0f, 0.0f, 0.0f), 0.0f };
			mTweenPoints.push_back({ wayP[i - 1]->xPos, wayP[i - 1]->zPos, 0, 0, 0, XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f });
			++lastAdd;
			--i;
			continue;
		}
	}
	if (lastSame.x == 1.0f && lastSame.y == 0.0f)
	{
		mTweenPoints[lastAdd] = { mTweenPoints[lastAdd].startPosX, mTweenPoints[lastAdd].startPosZ, wayP[wayP.size() - 1]->xPos, wayP[wayP.size() - 1]->zPos,
			abs(mTweenPoints[lastAdd].startPosZ - wayP[wayP.size() - 1]->zPos), XMFLOAT3(0.0f, 0.0f, wayP[wayP.size() - 1]->zPos > mTweenPoints[lastAdd].startPosZ ? 1.0f : -1.0f), 0.0f };
	}
	else
	{
		mTweenPoints[lastAdd] = { mTweenPoints[lastAdd].startPosX, mTweenPoints[lastAdd].startPosZ, wayP[wayP.size() - 1]->xPos, wayP[wayP.size() - 1]->zPos,
			abs(mTweenPoints[lastAdd].startPosX - wayP[wayP.size() - 1]->xPos), XMFLOAT3(wayP[wayP.size() - 1]->xPos > mTweenPoints[lastAdd].startPosX ? 1.0f : -1.0f, 0.0f, 0.0f), 0.0f };
	}

	UINT totalDistance = 0;
	for (int i = 0; i < mTweenPoints.size(); ++i)
	{
		totalDistance += mTweenPoints[i].distance;
	}
	mTweenPoints[0].startTween = 0.0f;
	mTweenPoints[0].tweenTime = (float)mTweenPoints[0].distance / totalDistance;
	mTweenPoints[0].endTween = mTweenPoints[0].tweenTime;
	for (int i = 1; i < mTweenPoints.size(); ++i)
	{
		mTweenPoints[i].startTween = mTweenPoints[i - 1].endTween;
		mTweenPoints[i].tweenTime = (float)mTweenPoints[i].distance / totalDistance;
		mTweenPoints[i].endTween = mTweenPoints[i].startTween + mTweenPoints[i].tweenTime;
	}

	mCurrTweenPoint.x = mTweenPoints[0].startPosX;
	mCurrTweenPoint.y = 0.0f;
	mCurrTweenPoint.z = mTweenPoints[0].startPosZ;
	mCurrTweenIndex = 0;
}