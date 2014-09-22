#pragma once
#include "graphicalgeometry.h"
#include "BasicMeshGeometry.h"
#include "Vertex.h"
#include "Effect.h"

class MeshGeometry : public BasicMeshGeometry
{
public:
	MeshGeometry(LitTexEffect* effect);
	~MeshGeometry(void);

	virtual void SetSubsetTable(std::vector<Subset>& subsetTable);

	void Draw(ID3D11DeviceContext* dc, UINT subsetID);

	virtual void Draw(ID3D11DeviceContext* context, CXMMATRIX world, CXMMATRIX ITWorld, CXMMATRIX vp);

protected:

private:
	MeshGeometry(LitMatEffect* effect);
};

