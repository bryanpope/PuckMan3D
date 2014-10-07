#pragma once
#include "Ghost.h"
class Clyde : public Ghost
{
protected:
	std::vector<PathNode*> mScatterWaypoints;
	void LoadScatterWaypoints();

public:
	Clyde(FXMVECTOR pos, float radius);
	~Clyde();
	virtual void Update(float dt, bool powerUpActivated, int levelNumber, int pelletCounter);
	void Reset();
};