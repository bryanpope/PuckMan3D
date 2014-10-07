#pragma once
#include "Ghost.h"
#include "Blinky.h"
class Pinky : public Ghost
{
protected:
	static const int MAX_WAYPOINTS = 18;
	static const XMFLOAT3 mScatterWaypoints[MAX_WAYPOINTS]; //This is an array of waypoints in which Pinky will loop through during scatter mode
	

public:
	Pinky(FXMVECTOR pos, FXMVECTOR vel, float radius);
	~Pinky();
	virtual void Update(float dt, bool powerUpActivated, Direction::DIRECTION facingState, int levelNumber);
	void Reset();
};