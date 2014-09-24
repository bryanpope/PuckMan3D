#pragma once

#include "d3dUtil.h"


class Vertex
{
public:

	struct BasicVertex
	{
		XMFLOAT3 pos;

		BasicVertex() : pos(0.0f, 0.0f, 0.0f)
		{
		}
	};

	struct NormalTexVertex
	{
		XMFLOAT3 pos;
		XMFLOAT3 normal;
		XMFLOAT2 tex;
	};

	struct TerrainVertex
	{
		XMFLOAT3 pos;
		XMFLOAT3 normal;
		XMFLOAT2 tiledTex;
		XMFLOAT2 tex;
	};

	struct ParticleVertex
	{
		XMFLOAT3 pos;
		XMFLOAT2 size;
	};

	struct InstancedData
	{
		XMFLOAT4X4 World;
		XMFLOAT4 Color;
	};

private:
	static ID3D11InputLayout* mBasicVertLayout;
	static ID3D11InputLayout* mNormalTexVertLayout;
	static ID3D11InputLayout* mTerrainVertLayout;
	static ID3D11InputLayout* mParticleVertLayout;
	static ID3D11InputLayout* mNormalMatVertInstanceLayout;


public:
	static void InitBasicLayout(ID3D11Device* device, ID3DX11EffectTechnique* tech);
	static void InitLitTexLayout(ID3D11Device* device, ID3DX11EffectTechnique* tech);
	static void InitLitMatLayout(ID3D11Device* device, ID3DX11EffectTechnique* tech);
	static void InitLitMatInstanceLayout(ID3D11Device* device, ID3DX11EffectTechnique* tech);
	static void InitTerrainVertLayout(ID3D11Device* device, ID3DX11EffectTechnique* tech);
	static void InitParticleVertLayout(ID3D11Device* device, ID3DX11EffectTechnique* tech);

	
	static ID3D11InputLayout* GetBasicVertLayout()
	{
		return mBasicVertLayout;
	}

	static ID3D11InputLayout* GetNormalTexVertLayout()
	{
		return mNormalTexVertLayout;
	}

	static ID3D11InputLayout* GetTerrainVertLayout()
	{
		return mTerrainVertLayout;
	}

	static ID3D11InputLayout* GetParticleVertLayout()
	{
		return mParticleVertLayout;
	}

	static ID3D11InputLayout* GetNormalMatVertInstanceLayout()
	{
		return mNormalMatVertInstanceLayout;
	}

	static void CleanLayouts();

private:
	Vertex(){}
};