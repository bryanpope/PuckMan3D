#pragma once
#include "Ghost.h"

class Blinky : public Ghost
{
protected:
	std::vector<PathNode*> mScatterWaypoints;
	void LoadScatterWaypoints();

public:
	Blinky(FXMVECTOR pos, float radius);
	~Blinky();
	void Update(float dt, bool powerUpActivated, int levelNumber);
	void Reset();
};