#pragma once
#include "graphicalgeometry.h"
#include "Effect.h"

class Terrain : public GraphicalGeometry
{
public:
	struct InitInfo
	{
		std::wstring heightMapFilename;
		std::wstring layerMapFilename0;
		std::wstring layerMapFilename1;
		std::wstring layerMapFilename2;
		std::wstring layerMapFilename3;
		std::wstring blendMapFilename;
		float heightScale;
		UINT width; //the width and height of the heightmap / grid vertices
		UINT height;
		float cellWidth;
		float cellHeight;
		float texTilesWide;
		float texTilesHigh;
	};

private:
	InitInfo mTerrainData;
	std::vector<float> mHeightMap;
	ID3D11ShaderResourceView** mDiffuseMaps;
	ID3D11ShaderResourceView* mBlendMap;

	TerrainEffect* mEffect;

public:
	Terrain(void){}
	virtual ~Terrain(void);

	void Init(ID3D11Device* device, const InitInfo& info);

	void LoadHeightMap();

	void InitVB(ID3D11Device* device);
	void InitIB(ID3D11Device* device);

	void Draw(ID3D11DeviceContext* context, CXMMATRIX vp);

	TerrainEffect* GetEffect() const
	{
		return mEffect;
	}

	float GetHeight(float x, float z);

private:
	void LoadTextures(ID3D11Device* device);
};

