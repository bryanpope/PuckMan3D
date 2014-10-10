#pragma once
#include "Ghost.h"
class Inky : public Ghost
{
protected:
	std::vector<PathNode*> mScatterWaypoints;
	void LoadScatterWaypoints();
	PathNode offsetTile;

public:
	Inky(FXMVECTOR pos, float radius);
	~Inky();
	virtual void Update(float dt, bool powerUpActivated, Direction::DIRECTION facingState, XMFLOAT3 blinkyPos, int levelNumber, int pelletCounter);
	void Reset();
};