#pragma once
#include "Ghost.h"
class Blinky : public Ghost
{
public:
	Blinky(FXMVECTOR pos, FXMVECTOR vel, float radius);
	~Blinky();
	void Update(float dt, bool powerUpActivated, PuckMan::Facing facingState);
};