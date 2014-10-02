#pragma once
#include "Ghost.h"

class Blinky : public Ghost
{
protected:
	static const XMFLOAT3 mScatterWaypoints[18]; //This is an array of waypoints in which Blinky will loop through during scatter mode
	static const std::string mScatterFacing[18]; //This is an array of facings to match the waypoints

public:
	Blinky(FXMVECTOR pos, FXMVECTOR vel, float radius);
	~Blinky();
	void Update(float dt, bool powerUpActivated, PuckMan::Facing facingState, int levelNumber);
	void Reset();
};