#pragma once
#include "Ghost.h"
class Clyde : public Ghost
{
protected:
	static const XMFLOAT3 mScatterWaypoints[34]; //This is an array of waypoints in which Clyde will loop through during scatter mode

public:
	Clyde(FXMVECTOR pos, FXMVECTOR vel, float radius);
	~Clyde();
	virtual void Update(float dt, bool powerUpActivated, PuckMan::Facing facingState, int levelNumber);
};