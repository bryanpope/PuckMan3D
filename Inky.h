#pragma once
#include "Ghost.h"
class Inky : public Ghost
{
public:
	//Inky(UINT row, UINT col, FXMVECTOR vel, float radius);
	Inky(FXMVECTOR pos, FXMVECTOR vel, float radius);
	~Inky();
	virtual void Update();
};