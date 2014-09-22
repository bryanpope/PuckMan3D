#include "MeshGeometry.h"


MeshGeometry::MeshGeometry(LitMatEffect* effect) : 
	BasicMeshGeometry(effect)
{
}

MeshGeometry::MeshGeometry(LitTexEffect* effect) :
	BasicMeshGeometry(effect)
{
}


MeshGeometry::~MeshGeometry(void)
{
	for(int i = 0; i < mSubsetTable.size(); ++i)
	{
		ReleaseCOM(mSubsetTable[i].srv);
	}
}

void MeshGeometry::SetSubsetTable(std::vector<Subset>& subsetTable)
{
	mSubsetTable = subsetTable;
}

void MeshGeometry::Draw(ID3D11DeviceContext* context, UINT subsetID)
{
	UINT offset = 0;
	UINT stride = sizeof(Vertex::NormalTexVertex);

	context->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	context->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

	context->DrawIndexed(mSubsetTable[subsetID].faceCount * 3, mSubsetTable[subsetID].faceStartIndex * 3, 0);
}

void MeshGeometry::Draw(ID3D11DeviceContext* context, CXMMATRIX world, CXMMATRIX ITWorld, CXMMATRIX vp)
{
	mEffectTex->SetPerObjectParams(world, ITWorld, world * vp, mSubsetTable[0].srv);
	for(int i = 0; i <  mSubsetTable.size(); ++i)
	{
		mEffectTex->SetDiffuseMap(mSubsetTable[i].srv);
		mEffectTex->Draw(context, mVB, mIB, mSubsetTable[i].faceStartIndex * 3, mSubsetTable[i].faceCount * 3);
	}
}

