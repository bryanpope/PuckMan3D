#pragma once
#include "Ghost.h"
class Clyde : public Ghost
{
protected:
	static const int MAX_WAYPOINTS = 34; 
	static const XMFLOAT3 mScatterWaypoints[MAX_WAYPOINTS]; //This is an array of waypoints in which Clyde will loop through during scatter mode

public:
	Clyde(FXMVECTOR pos, FXMVECTOR vel, float radius);
	~Clyde();
	virtual void Update(float dt, bool powerUpActivated, int levelNumber, int pelletCounter);
	void Reset();
};