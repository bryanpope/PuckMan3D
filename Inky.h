#pragma once
#include "Ghost.h"
class Inky : public Ghost
{
protected:
	std::vector<PathNode*> mPath;
	PathNode* mStart;
	PathNode* mGoal;
public:
	Inky();
	~Inky();
	virtual void Update();
};