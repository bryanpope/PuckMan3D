#pragma once
#include "Ghost.h"
class Blinky : public Ghost
{
protected:
	std::vector<PathNode*> mPath;
	PathNode* mStart;
	PathNode* mGoal;
public:
	Blinky();
	~Blinky();
	virtual void Update();
};