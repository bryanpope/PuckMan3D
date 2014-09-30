#pragma once
#include "Ghost.h"
#include "Blinky.h"
class Pinky : public Ghost
{
public:
	Pinky(FXMVECTOR pos, FXMVECTOR vel, float radius);
	~Pinky();
	virtual void Update(float dt, PuckMan::Facing facingState);
};