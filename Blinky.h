#pragma once
#include "Ghost.h"

class Blinky : public Ghost
{
protected:
	static const int MAX_WAYPOINTS = 18;
	static const XMFLOAT3 mScatterWaypoints[MAX_WAYPOINTS]; //This is an array of waypoints in which Blinky will loop through during scatter mode

public:
	Blinky(FXMVECTOR pos, FXMVECTOR vel, float radius);
	~Blinky();
	void Update(float dt, bool powerUpActivated, int levelNumber);
	void Reset();
};