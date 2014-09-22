#pragma once

#include "GraphicalGeometry.h"
#include "d3dUtil.h"

class Cube : public GraphicalGeometry
{
private:
	

public:
	Cube(void) : GraphicalGeometry()
	{}

	//inits the quad  of the size specified so that it is facing down the positive z axis.
	Cube(ID3D11Device* device, float width, float height, float depth);

	~Cube(void)
	{
		if(mVB)
		{
			mVB->Release();
		}

		if(mIB)
		{
			mIB->Release();
		}
	}

	ID3D11Buffer* GetVB()
	{
		return mVB;
	}

	ID3D11Buffer* GetIB()
	{
		return mIB;
	}

	void InitBuffers(ID3D11Device* device, float width, float height, float depth);


private:

	//inits the quad to face down the positive x axis
	void InitVB(ID3D11Device* device, float width, float height, float depth);

	void InitIB(ID3D11Device* device);
};

