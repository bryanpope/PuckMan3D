#pragma once
#include "Ghost.h"
class Blinky : public Ghost
{
public:
	Blinky(UINT row, UINT col, FXMVECTOR vel, float radius);
	~Blinky();
	void Update();
};