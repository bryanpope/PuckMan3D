#pragma once
#include "Ghost.h"
class Clyde : public Ghost
{
public:
	Clyde(FXMVECTOR pos, FXMVECTOR vel, float radius);
	~Clyde();
	virtual void Update(float dt, bool powerUpActivated, PuckMan::Facing facingState);
};