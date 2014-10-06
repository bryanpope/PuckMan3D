#pragma once
#include "Ghost.h"
class Inky : public Ghost
{
protected:
	static const int MAX_WAYPOINTS = 34;
	static const XMFLOAT3 mScatterWaypoints[MAX_WAYPOINTS]; //This is an array of waypoints in which Inky will loop through during scatter mode

public:
	Inky(FXMVECTOR pos, FXMVECTOR vel, float radius);
	~Inky();
	virtual void Update(float dt, bool powerUpActivated, Direction::DIRECTION facingState, XMFLOAT3 blinkyPos, int levelNumber, int pelletCounter);
	void Reset();
};