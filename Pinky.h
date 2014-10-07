#pragma once
#include "Ghost.h"
#include "Blinky.h"
class Pinky : public Ghost
{
protected:
	std::vector<PathNode*> mScatterWaypoints;
	void LoadScatterWaypoints();

public:
	Pinky(FXMVECTOR pos, float radius);
	~Pinky();
	virtual void Update(float dt, bool powerUpActivated, Direction::DIRECTION facingState, int levelNumber);
	void Reset();
};