#include "Terrain.h"
#include "Vertex.h"


Terrain::~Terrain(void)
{
	for(int i = 0; i < 4; ++i)
	{
		ReleaseCOM(mDiffuseMaps[i]);
	}

	delete [] mDiffuseMaps;

	ReleaseCOM(mBlendMap);

	delete mEffect;
}

void Terrain::Init(ID3D11Device* device, const InitInfo& info)
{
	mTerrainData = info;
	LoadHeightMap();
	InitVB(device);
	InitIB(device);

	mEffect = new TerrainEffect();
	mEffect->LoadEffect(L"FX/terrain.fx", device);

	LoadTextures(device);
}

void Terrain::LoadTextures(ID3D11Device* device)
{
	mDiffuseMaps = new ID3D11ShaderResourceView*[4];
	
	HR(D3DX11CreateShaderResourceViewFromFile(device, 
		mTerrainData.layerMapFilename0.c_str(), 0, 0,
		&mDiffuseMaps[0], 0));

	HR(D3DX11CreateShaderResourceViewFromFile(device, 
		mTerrainData.layerMapFilename1.c_str(), 0, 0,
		&mDiffuseMaps[1], 0));

	HR(D3DX11CreateShaderResourceViewFromFile(device, 
		mTerrainData.layerMapFilename2.c_str(), 0, 0,
		&mDiffuseMaps[2], 0));

	HR(D3DX11CreateShaderResourceViewFromFile(device, 
		mTerrainData.layerMapFilename3.c_str(), 0, 0,
		&mDiffuseMaps[3], 0));

	HR(D3DX11CreateShaderResourceViewFromFile(device, 
		mTerrainData.blendMapFilename.c_str(), 0, 0,
		&mBlendMap, 0));
}

void Terrain::LoadHeightMap()
{
	// A height for each vertex
	std::vector<unsigned char> in( mTerrainData.width * mTerrainData.height);

	// Open the file.
	std::ifstream inFile;
	inFile.open(mTerrainData.heightMapFilename, std::ios_base::binary);

	if(inFile)
	{
		// Read the RAW bytes.
		inFile.read((char*)&in[0], (std::streamsize)in.size());

		// Done with file.
		inFile.close();
	}

	// Copy the array data into a float array and scale it.
	mHeightMap.resize(mTerrainData.height *  mTerrainData.width, 0);
	for(UINT i = 0; i < mTerrainData.height *  mTerrainData.width; ++i)
	{
		mHeightMap[i] = (in[i] / 255.0f)*mTerrainData.heightScale;
	}
}

void Terrain::InitVB(ID3D11Device* device)
{
	std::vector<Vertex::TerrainVertex> vertexData(mTerrainData.width *
													mTerrainData.height);

	UINT cellsWide = mTerrainData.width - 1;
	UINT cellsHigh = mTerrainData.height - 1;

	for(int row = 0; row < mTerrainData.height; ++row)
	{
		for(int col = 0; col < mTerrainData.width; ++col)
		{
			UINT index = row * mTerrainData.width + col;
			vertexData[index].pos = XMFLOAT3(col * mTerrainData.cellWidth,
											 mHeightMap[index],
											 row * mTerrainData.cellHeight); 

			float u = mTerrainData.texTilesWide / mTerrainData.width;
			u *= col;

			float v = mTerrainData.texTilesHigh / mTerrainData.height;
			v *= row;

			vertexData[index].tiledTex.x = u;
			vertexData[index].tiledTex.y = v;

			u = (float)col / (mTerrainData.width - 1);
			v = (float)row / (mTerrainData.height - 1);

			vertexData[index].tex.x = u;
			vertexData[index].tex.y = v;

		}
	}

	std::vector<XMFLOAT3> normals;
	for(int row = 0; row < cellsHigh; ++row)
	{
		for(int col = 0; col < cellsWide; ++col)
		{
			//determine the row and column in terms of vertices given the 
			//cell row and column
			UINT top = row + 1;
			UINT right = col + 1;
			UINT bot = row;
			UINT left = col;

			XMVECTOR A = XMVectorSet(0.0f, mHeightMap[top * mTerrainData.width + left] -
				mHeightMap[bot * mTerrainData.width + left], mTerrainData.cellHeight, 0.0f);

			XMVECTOR B = XMVectorSet(mTerrainData.cellWidth, mHeightMap[bot * mTerrainData.width + right] -
				mHeightMap[bot * mTerrainData.width + left], 0.0f, 0.0f);
			
			XMVECTOR C = XMVectorSet(mTerrainData.cellWidth, mHeightMap[top * mTerrainData.width + right] -
				mHeightMap[top * mTerrainData.width + left], 0.0f, 0.0f);

			XMVECTOR D = XMVectorSet(0.0f, mHeightMap[top * mTerrainData.width + right] -
				mHeightMap[bot * mTerrainData.width + right], mTerrainData.cellHeight, 0.0f);

			XMVECTOR topNormal = XMVector3Cross(D, C);

			XMVECTOR botNormal = XMVector3Cross(A, B);

			topNormal = XMVector3Normalize(topNormal);
			botNormal = XMVector3Normalize(botNormal);

			XMFLOAT3 topNormalF3, botNormalF3;

			XMStoreFloat3(&topNormalF3, topNormal);
			XMStoreFloat3(&botNormalF3, botNormal);

			normals.push_back(topNormalF3);
			normals.push_back(botNormalF3);
		}
	}

	for(int row = 0; row < mTerrainData.height; ++row)
	{
		for(int col = 0; col < mTerrainData.width; ++col)
		{
			UINT index = row * mTerrainData.width + col;

			int top = row;
			int bot = row - 1;
			int left = col - 1;
			int right = col;

			int normalCounter = 0;
			XMVECTOR normsCombined = XMVectorZero();
			//get the two top left normals
			if(top < cellsHigh && left >= 0)
			{
				UINT topLeftIndex = (top * cellsWide + left) * 2;

				normsCombined = normsCombined + XMLoadFloat3(&normals[topLeftIndex]);
				normsCombined = normsCombined + XMLoadFloat3(&normals[topLeftIndex + 1]);
				normalCounter += 2;
			}

			//get the bottom left normal
			if(bot >= 0 && left >= 0)
			{
				UINT botLeftIndex = (bot * cellsWide + left) * 2;

				normsCombined = normsCombined + XMLoadFloat3(&normals[botLeftIndex]);
				normalCounter++;
			}

			//get the top right normal
			if(top < cellsHigh && right < cellsWide)
			{
				UINT topRightIndex = (top * cellsWide + right) * 2;

				normsCombined = normsCombined + XMLoadFloat3(&normals[topRightIndex]);
				normalCounter++;
			}

			//get the two bot right normals
			if(bot >= 0 && right < cellsWide)
			{
				UINT botRightIndex = (bot * cellsWide + right) * 2;

				normsCombined = normsCombined + XMLoadFloat3(&normals[botRightIndex]);
				normsCombined = normsCombined + XMLoadFloat3(&normals[botRightIndex + 1]);
				normalCounter += 2;
			}

			normsCombined = normsCombined / (float)normalCounter;
			XMStoreFloat3(&(vertexData[index].normal), normsCombined);
		}
	}

	D3D11_BUFFER_DESC vbd;
    vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex::TerrainVertex) * vertexData.size();
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = &vertexData[0];
    HR(device->CreateBuffer(&vbd, &vinitData, &mVB));

}
void Terrain::InitIB(ID3D11Device* device)
{
	UINT cellsWide = mTerrainData.width - 1;
	UINT cellsHigh = mTerrainData.height - 1;

	std::vector<UINT> indices(cellsWide * cellsHigh * 6);
	for(int row = 0; row < cellsHigh; ++row)
	{
		for(int col = 0; col < cellsWide; ++col)
		{
			UINT currCell = row * cellsWide + col;
			UINT currIndex = currCell * 6;

			UINT top = row + 1;
			UINT bot = row;
			UINT right = col + 1;
			UINT left = col;

			indices[currIndex] = top * mTerrainData.width + left;
			indices[currIndex + 1] = top * mTerrainData.width + right;
			indices[currIndex + 2] = bot * mTerrainData.width + right;

			indices[currIndex + 3] = bot * mTerrainData.width + right;
			indices[currIndex + 4] = bot * mTerrainData.width + left;
			indices[currIndex + 5] = top * mTerrainData.width + left;
		}
	}

	mIndexCount = indices.size();

	D3D11_BUFFER_DESC ibd;
    ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * mIndexCount;
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA iinitData;
    iinitData.pSysMem = &indices[0];
    HR(device->CreateBuffer(&ibd, &iinitData, &mIB));
}

void Terrain::Draw(ID3D11DeviceContext* context, CXMMATRIX vp)
{
	context->IASetInputLayout(Vertex::GetTerrainVertLayout());
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//XMMATRIX wvp = mTestPlayer.GetWorld()* mCam->GetView() * proj;
	XMMATRIX world = XMMatrixIdentity();
	XMMATRIX wvp = world * vp;

	mEffect->SetPerObjectParams(world, MathHelper::InverseTranspose(world),
							    wvp, mDiffuseMaps[0], mDiffuseMaps[1],
								mDiffuseMaps[2], mDiffuseMaps[3], mBlendMap);

	mEffect->Draw(context, mVB, mIB, GetIndexCount());
}


float Terrain::GetHeight(float x, float z)
{
	x = min(mTerrainData.width * mTerrainData.cellWidth,
			x);
	x = max(0.0f, x);

	z = min(mTerrainData.height * mTerrainData.cellHeight,
			z);
	z = max(0.0f, z);

	float c = x / mTerrainData.cellWidth;
	float r = z / mTerrainData.cellHeight;

	int row = (int)floorf(r);
	int col = (int)floorf(c);

	//	A  __ C
	//   |\  |
	//	 | \ |
	//	 |__\|
	//	B	  D
	//

	float A = mHeightMap[(row + 1) * mTerrainData.width + col];
	float B = mHeightMap[row * mTerrainData.width + col];
	float C = mHeightMap[(row + 1) * mTerrainData.width + col + 1];
	float D = mHeightMap[row * mTerrainData.width + col + 1];

	float s = r - row;
	float t = c - col;

	if(s + t <= 1.0f)
	{
		float BD = D - B;
		float BA = A - B;

		return B + BD * t + BA * s;
	}
	else
	{
		float CA = A - C;
		float CD = D - C;

		return C + (CA * (1.0f - t)) + (CD * (1.0f - s));
	}
}