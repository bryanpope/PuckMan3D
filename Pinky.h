#pragma once
#include "Ghost.h"
#include "Blinky.h"
class Pinky : public Ghost
{
protected:
	static const XMFLOAT3 mScatterWaypoints[18]; //This is an array of waypoints in which Pinky will loop through during scatter mode
	static const std::string mScatterFacing[18]; //This is an array of facings to match the waypoints

public:
	Pinky(FXMVECTOR pos, FXMVECTOR vel, float radius);
	~Pinky();
	virtual void Update(float dt, bool powerUpActivated, PuckMan::Facing facingState, int levelNumber);
};