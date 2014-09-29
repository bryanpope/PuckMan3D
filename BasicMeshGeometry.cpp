#include "BasicMeshGeometry.h"


BasicMeshGeometry::BasicMeshGeometry(LitMatEffect* effect) :
	mEffectMat(effect)
{
}

BasicMeshGeometry::BasicMeshGeometry(LitTexEffect* effect) :
	mEffectTex(effect)
{
}


BasicMeshGeometry::~BasicMeshGeometry()
{
}

void BasicMeshGeometry::SetMaterial(Material mat)
{
	mMaterial = mat;
}

void BasicMeshGeometry::SetVertices(ID3D11Device* device, const Vertex::NormalTexVertex* verts,	UINT count)
{
	ReleaseCOM(mVB);

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex::NormalTexVertex) * count;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = verts;

	HR(device->CreateBuffer(&vbd, &vinitData, &mVB));
}

void BasicMeshGeometry::SetIndices(ID3D11Device* device, const UINT* indices, UINT count)
{
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * count;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA iInitData;
	iInitData.pSysMem = indices;

	mIndexCount = count;

	HR(device->CreateBuffer(&ibd, &iInitData, &mIB));
}

void BasicMeshGeometry::SetInstancesFloor(ID3D11Device* device, const Vertex::InstancedData* instances, UINT count)
{
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_DYNAMIC;
	ibd.ByteWidth = sizeof(Vertex::InstancedData) * count;
	ibd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	ibd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	HR(device->CreateBuffer(&ibd, 0, &mInstanceBFloor));
}

void BasicMeshGeometry::SetInstancesWallsBent(ID3D11Device* device, const Vertex::InstancedData* instances, UINT count)
{
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_DYNAMIC;
	ibd.ByteWidth = sizeof(Vertex::InstancedData) * count;
	ibd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	ibd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	HR(device->CreateBuffer(&ibd, 0, &mInstanceBWallsBent));
}

void BasicMeshGeometry::SetInstancesWallsStraight(ID3D11Device* device, const Vertex::InstancedData* instances, UINT count)
{
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_DYNAMIC;
	ibd.ByteWidth = sizeof(Vertex::InstancedData) * count;
	ibd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	ibd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	HR(device->CreateBuffer(&ibd, 0, &mInstanceBWallsStraight));
}

void BasicMeshGeometry::SetInstancesPellets(ID3D11Device* device, const Vertex::InstancedData* instances, UINT count)
{
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_DYNAMIC;
	ibd.ByteWidth = sizeof(Vertex::InstancedData) * count;
	ibd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	ibd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	HR(device->CreateBuffer(&ibd, 0, &mInstanceBPellets));
}

void BasicMeshGeometry::SetInstancesPowerUps(ID3D11Device* device, const Vertex::InstancedData* instances, UINT count)
{
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_DYNAMIC;
	ibd.ByteWidth = sizeof(Vertex::InstancedData) * count;
	ibd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	ibd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	HR(device->CreateBuffer(&ibd, 0, &mInstanceBPowerUps));
}

void BasicMeshGeometry::SetInstancesPacMans(ID3D11Device* device, const Vertex::InstancedData* instances, UINT count)
{
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_DYNAMIC;
	ibd.ByteWidth = sizeof(Vertex::InstancedData) * count;
	ibd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	ibd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	HR(device->CreateBuffer(&ibd, 0, &mInstanceBPacMans));
}

void BasicMeshGeometry::SetInstancesGhosts(ID3D11Device* device, const Vertex::InstancedData* instances, UINT count)
{
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_DYNAMIC;
	ibd.ByteWidth = sizeof(Vertex::InstancedData) * count;
	ibd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	ibd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	HR(device->CreateBuffer(&ibd, 0, &mInstanceBGhosts));
}

void BasicMeshGeometry::Draw(ID3D11DeviceContext* context, CXMMATRIX world, CXMMATRIX ITWorld, CXMMATRIX vp)
{
	mEffectMat->SetPerObjectParams(world, ITWorld, world * vp, vp, mMaterial);
	mEffectMat->Draw(context, mVB, mIB, mIndexCount);
}

