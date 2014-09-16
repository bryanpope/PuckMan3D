#pragma once
#include "Ghost.h"
class Pinky : public Ghost
{
protected:
	std::vector<PathNode*> mPath;
	PathNode* mStart;
	PathNode* mGoal;
public:
	Pinky();
	~Pinky();
	virtual void Update();
};