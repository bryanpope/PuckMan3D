#pragma once
#include "Ghost.h"
class Clyde : public Ghost
{
protected:
	std::vector<PathNode*> mPath;
	PathNode* mStart;
	PathNode* mGoal;
public:
	Clyde();
	~Clyde();
	virtual void Update();
};