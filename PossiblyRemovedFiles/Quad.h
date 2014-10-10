#pragma once

#include "GraphicalGeometry.h"

class Quad : public GraphicalGeometry
{
public:
	Quad(void) : GraphicalGeometry()
	{}

	//inits the quad  of the size specified so that it is facing down the positive z axis.
	Quad(ID3D11Device* device, float width, float height);

	virtual ~Quad(void)
	{
		
	}

	void InitBuffers(ID3D11Device* device, float width, float height);


private:

	//inits the quad to face down the positive x axis
	void InitVB(ID3D11Device* device, float width, float height);

	void InitIB(ID3D11Device* device);
};

