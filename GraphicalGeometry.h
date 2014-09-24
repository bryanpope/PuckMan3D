#pragma once

#include "d3dUtil.h"

class GraphicalGeometry
{
protected:
	ID3D11Buffer* mVB;
	ID3D11Buffer* mIB;
	ID3D11Buffer* mInstanceBWalls;
	ID3D11Buffer* mInstanceBPellets;
	ID3D11Buffer* mInstanceBPowerUps;
	ID3D11Buffer* mInstanceBPacMans;
	ID3D11Buffer* mInstanceBGhosts;
	UINT mIndexCount;

public:
	GraphicalGeometry(void) : mVB(0), mIB(0), mInstanceBWalls(0), mInstanceBPellets(0), mInstanceBPowerUps(0), mInstanceBPacMans(0), mInstanceBGhosts(0), mIndexCount(0)
	{
	}

	virtual ~GraphicalGeometry(void)
	{
		if(mVB)
		{
			mVB->Release();
		}

		if(mIB)
		{
			mIB->Release();
		}

		if (mInstanceBWalls)
		{
			mInstanceBWalls->Release();
		}
		if (mInstanceBPellets)
		{
			mInstanceBPellets->Release();
		}
		if (mInstanceBPowerUps)
		{
			mInstanceBPowerUps->Release();
		}
		if (mInstanceBPacMans)
		{
			mInstanceBPacMans->Release();
		}
		if (mInstanceBGhosts)
		{
			mInstanceBGhosts->Release();
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

	ID3D11Buffer* GetInstanceBWalls()
	{
		return mInstanceBWalls;
	}
	ID3D11Buffer* GetInstanceBPellets()
	{
		return mInstanceBPellets;
	}
	ID3D11Buffer* GetInstanceBPowerUps()
	{
		return mInstanceBPowerUps;
	}
	ID3D11Buffer* GetInstanceBPacMans()
	{
		return mInstanceBPacMans;
	}
	ID3D11Buffer* GetInstanceBGhosts()
	{
		return mInstanceBGhosts;
	}

	UINT GetIndexCount() const
	{
		return mIndexCount;
	}

protected:

	virtual void InitIB(ID3D11Device* device) = 0;
};

