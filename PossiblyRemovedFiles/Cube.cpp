#include "Cube.h"
#include "Vertex.h"

Cube::Cube(ID3D11Device* device, float width, float height, float depth)
{
	InitBuffers(device, width, height, depth);
}

void Cube::InitBuffers(ID3D11Device* device, float width, float height,float depth)
{
	InitVB(device, width, height, depth);
	InitIB(device);
}

void Cube::InitVB(ID3D11Device* device, float width, float height, float depth)
{
	float halfWidth = width / 2.0f;
	float halfHeight = height / 2.0f;

	// Create vertex buffer
	Vertex::NormalTexVertex vertices[] =
    {
		{ XMFLOAT3(-halfWidth, -halfHeight, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f)},
		{ XMFLOAT3(-halfWidth, halfHeight, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f,0.0f)},
		{ XMFLOAT3(halfWidth, -halfHeight, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f)},
		{ XMFLOAT3(halfWidth, halfHeight, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
    };


    D3D11_BUFFER_DESC vbd;
    vbd.Usage = D3D11_USAGE_IMMUTABLE;
    vbd.ByteWidth = sizeof(Vertex::NormalTexVertex) * 4;
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
    D3D11_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = vertices;
    HR(device->CreateBuffer(&vbd, &vinitData, &mVB));
}

void Cube::InitIB(ID3D11Device* device)
{
	UINT indices[] = 
	{
		0, 2, 1,
		2, 3, 1,
	};

	D3D11_BUFFER_DESC ibd;
    ibd.Usage = D3D11_USAGE_IMMUTABLE;
    ibd.ByteWidth = sizeof(UINT) * 6;
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
    D3D11_SUBRESOURCE_DATA iinitData;
    iinitData.pSysMem = indices;
    HR(device->CreateBuffer(&ibd, &iinitData, &mIB));
}