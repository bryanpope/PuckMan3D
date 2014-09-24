#pragma once
#include "Ghost.h"
class Clyde : public Ghost
{
public:
	//Clyde(UINT row, UINT col, FXMVECTOR vel, float radius);
	Clyde(FXMVECTOR pos, FXMVECTOR vel, float radius);
	~Clyde();
	virtual void Update();
};