#pragma once
#include "Ghost.h"
class Inky : public Ghost
{
public:
	Inky(FXMVECTOR pos, FXMVECTOR vel, float radius);
	~Inky();
	virtual void Update(float dt, PuckMan::Facing facingState, XMFLOAT3 blinkyPos);
};