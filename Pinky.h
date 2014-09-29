#pragma once
#include "Ghost.h"
#include "Player.h"
class Pinky : public Ghost
{
public:
	//Pinky(UINT row, UINT col, FXMVECTOR vel, float radius);
	Pinky(FXMVECTOR pos, FXMVECTOR vel, float radius);
	~Pinky();
	virtual void Update(float dt, PuckMan::Facing facingState);
};