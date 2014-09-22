#pragma once

#include "GraphicalGeometry.h"
#include "Vertex.h"
#include "Effect.h"

class BasicMeshGeometry : public GraphicalGeometry
{
public:
	struct Subset
	{
		UINT id;
		UINT vertexStart;
		UINT vertexCount;
		UINT faceCount;
		UINT faceStartIndex;
		ID3D11ShaderResourceView* srv;
		std::wstring name;
	};

public:
	BasicMeshGeometry(LitMatEffect* effect);
	BasicMeshGeometry(LitTexEffect* effect);
	~BasicMeshGeometry();

	virtual void SetVertices(ID3D11Device* device, const Vertex::NormalTexVertex* verts, UINT count);

	virtual void SetIndices(ID3D11Device* device, const UINT* indices, UINT count);

	virtual void SetSubsetTable(std::vector<Subset>& subsetTable){}

	void SetMaterial(Material mat);

	virtual void Draw(ID3D11DeviceContext* context, CXMMATRIX world, CXMMATRIX ITWorld, CXMMATRIX vp);

protected:
	LitMatEffect* mEffectMat;
	LitTexEffect* mEffectTex;

	std::vector<Subset> mSubsetTable;

	Material mMaterial;

	virtual void InitIB(ID3D11Device* device){}
};

