#pragma once
#include "Ghost.h"
#include "PathNode.h"
class Blinky : public Ghost
{
protected:
	std::vector<PathNode*> mPath;
	PathNode* mStart;
	PathNode* mGoal;
public:
	Blinky(UINT row, UINT col, FXMVECTOR vel, float radius);
	~Blinky();
	void Update();
};